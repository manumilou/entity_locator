<?php

/**
 * This plugin array is more or less self documenting
 */
$plugin = array(
  // the title in the admin
  'title' => t('Entity Locator'),
  'description' => t('Display Drupal entities on a map'),
  // no one knows if "single" defaults to FALSE...
  'single' => TRUE,
  // oh joy, I get my own section of panel panes
  'category' => array(t('Entity Locator'), -9),
  'edit form' => 'locator_content_type_edit_form',
  'render callback' => 'locator_content_type_render',
  'defaults' => array(),
);

/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info
 */
function locator_content_type_render($subtype, $conf, $context = NULL) {
  // Make sure that context variable is not empty
  if(empty($context)) {
  }

  if(empty($conf['node_types'])) {
    //dpm(t('Please select at least one content type to display on the map'));
    return;
  }
  
  // Initialize variables
  $block = new StdClass();
  $markers = array();

  // Include javascript resources
  $api_key = variable_get('entity_locator_gmap_api_key', '');
  drupal_add_js('https://maps.googleapis.com/maps/api/js?key='.$api_key.'&sensor=true');
  drupal_add_js(drupal_get_path('module', 'entity_locator').'/js/entity_locator.js');
    drupal_add_css(drupal_get_path('module', 'entity_locator').'/css/entity_locator.css');
  if($conf['map_type'] != 'original'){
    drupal_add_js('http://maps.stamen.com/js/tile.stamen.js');
  }

  // Get the bundle from the pane configuration
  $bundle = array_shift(array_values($conf['node_types']));
  $entityLocator = new EntityLocator($bundle);
  $entities_by_country = $entityLocator->get_entities_by_country();

  foreach($entities_by_country as $entities) {
    foreach($entities as $entity) {
      $place_types = array();
      if( ! empty($conf['node_category']) ){
        $wrapper = entity_metadata_wrapper('node', $entity);
        $iterator = $wrapper->$conf['node_category']->getIterator();
        foreach ($iterator as $EntityDrupalWrapper) {
          $place_types[] = 'pt-'.$EntityDrupalWrapper->getIdentifier();
        }
      }

      if($entity->location && $entity->location['lid'] != NULL) {
        // Set the markers
        $markers[] = array(
          'title' => $entity->title,
          'lat' => $entity->location['latitude'],
          'lng' => $entity->location['longitude'],
          'types' => $place_types,
          'places_country' => 'pc-'.strtoupper($entity->location['country']),
        );
      }
    }
  }

  // Add Drupal Behaviour
  $setting = array('entity_locator' => array('markers' => $markers, 'bundle' => $bundle, 'get_params' => drupal_get_query_parameters(), 'map_type' => $conf['map_type']));
  drupal_add_js($setting, 'setting');

  $params = array(
    'entities_by_country' => $entities_by_country,
    'countries' => country_get_list(),
    'node_category' => $conf['node_category'],
    'categories' => $entityLocator->get_categories($conf['node_category']),
    'mapHeight' => $conf['map_height'],
    'prependText' => $conf['prepend_text'],
  );
  $block->content = theme('entity_locator', $params);
   
  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function locator_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $options = array();  
  $node_types = node_type_get_types();
  $vocab_array = array('' => 'None');

  foreach($node_types as $node_type) {
    $options[$node_type->type] = $node_type->name;
    $taxonomy = getTaxonomyFromNodeType($node_type);
    if(!empty($taxonomy)){
      $vocab_array[$node_type->name] = $taxonomy;  
    }
  } 

  // List of all available node types
  $form['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select node types to display on the map'),
    '#options' => $options,
    '#default_value' => !empty($conf['node_types']) ? $conf['node_types'] : '',
    '#required' => TRUE,
  );

  $form['node_category'] = array(
    '#type' => 'select',
    '#title' => t('Select the Taxonomy'),
    '#description' => t('Set the id of the Taxonomy who used for categorized nodes.'),
    '#options' => $vocab_array ,
    '#default_value' => !empty($conf['node_category']) ? $conf['node_category'] : '',
    '#required' => FALSE,
  );

  $map_types = array(
    'original' => 'Original',
    'watercolor' => 'Watercolor',
    'toner' => 'Toner',
  );

  $form['map_type'] = array(
    '#type' => 'select',
    '#title' => t('Select the map type'),
    '#description' => t('Set the map type of your gmaps.'),
    '#options' => $map_types ,
    '#default_value' => !empty($conf['map_type']) ? $conf['map_type'] : 'original',
    '#required' => FALSE,
  );

  $form['map_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Map height'),
    '#description' => t('Set the map height with unit, like 450px or 80%.'),
    '#default_value' => !empty($conf['map_height']) ? $conf['map_height'] : '450px',  
    '#required' => TRUE,
  );

  $form['prepend_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Type of your locations'),
    '#description' => t('Set text displayed for describe the type of location.'),
    '#default_value' => !empty($conf['prepend_text']) ? $conf['prepend_text'] : 'List of location',  
    '#required' => FALSE,
  );

  return $form;
}

/**
 * Submit function, note anything in the formstate[conf] automatically gets saved
 */
function locator_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['node_types'] = $form_state['values']['node_types'];
  $form_state['conf']['node_category'] = $form_state['values']['node_category'];
  $form_state['conf']['map_type'] = $form_state['values']['map_type'];
  $form_state['conf']['map_height'] = $form_state['values']['map_height'];
  $form_state['conf']['prepend_text'] = $form_state['values']['prepend_text'];
}

/**
 * Helper to get Taxonomy from a node type
 */
function getTaxonomyFromNodeType($node_type) {
  $fields_info = field_info_instances('node', $node_type->type);
  $options = array();
  foreach ($fields_info as $field_name => $field) {
    $field_info = field_info_field($field_name);
    if($field_info['type'] == "taxonomy_term_reference"){
      $options[$field_name] = $field['label'];
    }
  }
  return $options;
}