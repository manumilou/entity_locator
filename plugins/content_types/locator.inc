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
  // @todo Check for dependencies
  $countries = country_get_list();
  $markers = array();

  // Include javascript resources
  $api_key = variable_get('entity_locator_gmap_api_key', '');
  drupal_add_js('https://maps.googleapis.com/maps/api/js?key='.$api_key.'&sensor=true');
  drupal_add_js(drupal_get_path('module', 'entity_locator').'/js/entity_locator.js');
  if($conf['map_type'] != 'original'){
    drupal_add_js('http://maps.stamen.com/js/tile.stamen.js');
  }

  // Get the bundle from the pane configuration
  $values = array_values($conf['node_types']);
  $bundle = array_shift($values);
  
  $entityLocator = new EntityLocator($bundle);
  $entities_by_country = $entityLocator->get_entities_by_country();

  foreach($entities_by_country as $entities) {
    foreach($entities as $entity) {
      $wrapper = entity_metadata_wrapper('node', $entity);
      if($entity->location && $entity->location['lid'] != NULL) {
        // Set the markers
        $markers[] = array(
          'title' => $entity->title,
          'lat' => $entity->location['latitude'],
          'lng' => $entity->location['longitude'],
          'places_type' => 'pt-'.$wrapper->field_type->getIdentifier(),
          'places_country' => 'pc-'.strtoupper($entity->location['country']),
        );
      }
    }
  }

  // Add Drupal Behaviour
  $setting = array('entity_locator' => array('markers' => $markers, 'bundle' => $bundle, 'get_params' => drupal_get_query_parameters(), 'map_type' => $conf['map_type']));
  drupal_add_js($setting, 'setting');

  //Display the map
  $map = '<div id="entity_locator" style="width:960px; height:450px;">Supposed to be a map.</div>';
  
  //Display country filter
  $map .= '<div id="filters-menu" class="clearfix">';
  
  //Countries filter
  $map .= '<div class="select-style"><select name="countries" id="countries"><option value="">'.t('Pays').' :</option>';
  foreach(array_keys($entities_by_country) as $key) {
    $map .= '<option value="pc-'.strtoupper($key).'">'.$countries[strtoupper($key)].'</option>';
  }
  $map .= '</select></div></div>';

  // Format the list
  $list = '<div id="entity_locator_list"><h2 class="title">' . t('Liste des oeuvres') . '</h2>';
  
  //Add filters
  $list .= '<div id="filters" class="clearfix">' . t('Trier par') . ' : ';
  

  $categories = $entityLocator->get_categories($conf['node_category']);
  foreach($categories as $category) {
     $list .= '<input type="checkbox" checked="checked" name="pt-'.$category->tid.'" id="pt-'.$category->tid.'" class="css-checkbox" /><label for="pt-'.$category->tid.'" class="css-label">'.$category->name.'</label>';
  }
  $list .= '</div>';
  
  //Display each of places by section
  foreach($entities_by_country as $name => $entities) {
    $list .= '<div class="section clearfix" data-pc="pc-'.strtoupper($name).'">';
      $list .= '<a href="#" class="title"><h3>'.$countries[strtoupper($name)].'</h3><div class="arrow up"></div></a>';
      $list .= '<div class="warper">';
      foreach($entities as $entity) {
        $build = node_view($entity, 'teaser');
        $entityWrapper = entity_metadata_wrapper('node', $entity);
        $list .= '<article data-pt="pt-'.$entityWrapper->field_type->getIdentifier().'" data-pc="pc-'. strtoupper($build['locations']['#locations'][0]['country']) .'" >'. drupal_render($build) .'</article>';
      }
      $list .= '</div>';
    $list .= '</div>';
  }
  $list .= '</div>';

  $block->content = $map . $list;
  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function locator_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $options = array();  
  $node_types = node_type_get_types();

  foreach($node_types as $node_type) {
    $options[$node_type->type] = $node_type->name;
  } 

  // List of all available node types
  $form['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select node types to display on the map'),
    '#options' => $options,
    '#default_value' => !empty($conf['node_types']) ? $conf['node_types'] : '',
  );

  $vocabulary = taxonomy_get_vocabularies();
  $checklist_vocab_array = array();
  foreach ($vocabulary as $item) {
    $key = $item->machine_name;
    $value = $item->name;
    $checklist_vocab_array[$key] = $value;
  }

  $form['node_category'] = array(
    '#type' => 'select',
    '#title' => t('Select the Taxonomy'),
    '#description' => t('Set the id of the Taxonomy who used for categorized nodes.'),
    '#options' => $checklist_vocab_array ,
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
    '#description' => t('Set the map type of your gmaps'),
    '#options' => $map_types ,
    '#default_value' => !empty($conf['map_type']) ? $conf['map_type'] : 'original',
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
}
