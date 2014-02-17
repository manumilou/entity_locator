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
  drupal_add_js(drupal_get_path('module', 'entity_locator').'/js/entity_locator_block_preview.js');
  drupal_add_css(drupal_get_path('module', 'entity_locator').'/css/entity_locator_block_preview.css');
  
  $entityLocator = new EntityLocator('places');
  $params = array(
    'houses' => EntityLocator::houses(),
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

  return $form;
}

/**
 * Submit function, note anything in the formstate[conf] automatically gets saved
 */
function locator_preview_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['locator_preview_map_path'] = $form_state['values']['locator_preview_map_path'];
}

