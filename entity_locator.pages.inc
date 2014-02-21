<?php

function entity_locator_config($form, $form_state) {
  $form['initialization'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Map Initialize'),
  );

  $form['initialization']['entity_locator_gmap_api_key'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('entity_locator_gmap_api_key', ''),
    '#size' => 50,
    '#maxlength' => 255,
    '#title' => t('Google Maps API Key'),
    '#description' => t('This key is currently not required by Google.  Entering a Key will allow you to track map usage in Google.  If you want to use an API key, you can get one at <a href="http://code.google.com/apis/maps/signup.html">Google Map API website</a>.'),
  );

  return system_settings_form($form);
}
