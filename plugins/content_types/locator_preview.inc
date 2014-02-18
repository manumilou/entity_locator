<?php

/**
 * This plugin array is more or less self documenting
 */
$plugin = array(
  // the title in the admin
  'title' => t('Entity Locator Preview'),
  'description' => t('Provides quick access and filtering options for Drupal entities to be displayed on the map'),
  // no one knows if "single" defaults to FALSE...
  'single' => TRUE,
  // oh joy, I get my own section of panel panes
  'category' => array(t('Entity Locator'), -9),
  'edit form' => 'locator_preview_content_type_edit_form',
  'render callback' => 'locator_preview_content_type_render',
  'defaults' => array(),
);

/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info
 */
function locator_preview_content_type_render($subtype, $conf, $context = NULL) {
   // Make sure that context variable is not empty
  if(empty($context)) {
  }

  if(empty($conf['locator_preview_map_path'])) {
    //dpm(t('Please select at least one content type to display on the map'));
    return;
  }
  drupal_add_js(drupal_get_path('module', 'entity_locator').'/js/entity_locator_preview.js');
  
  $values = array_values($conf['node_types']);
  $bundle = array_shift($values);
  $entityLocator = new EntityLocator($bundle);
  $params = array(
    'houses' =>   $entityLocator->get_categories($conf['node_category']),
    'countries' => $entityLocator->get_entities_countries(),
    'path_to_map' => $conf['locator_preview_map_path'],
  );

  // Initialize variables
  $block = new StdClass();
  $block->content = theme('entity_locator_preview', $params);
  $block->title = t('View a location on the map');
  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function locator_preview_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  // List of all available node types
  $form['locator_preview_map_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to node map'),
    '#description' => t('Path to the node where map is displayed.'),
    '#default_value' => !empty($conf['locator_preview_map_path']) ? $conf['locator_preview_map_path'] : '',  
    '#required' => TRUE,
  );

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

  return $form;
}

/**
 * Submit function, note anything in the formstate[conf] automatically gets saved
 */
function locator_preview_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['locator_preview_map_path'] = $form_state['values']['locator_preview_map_path'];
  $form_state['conf']['node_types'] = $form_state['values']['node_types'];
  $form_state['conf']['node_category'] = $form_state['values']['node_category'];
}

