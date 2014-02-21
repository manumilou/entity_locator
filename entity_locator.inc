<?php

/**
 * @file
 * Provide a class for accessing to entities and caching it
 */

class EntityLocator {

  protected $bundle;

  /**
   * Constructer.
   */
  public function __construct($bundle) {
    $this->bundle = $bundle;
  }

  /**
   * Get entities to display on the map.
   *
   * It uses the bundle(s) specified in the Ctools panel configuration.
   */
  public function get_entities() {
    $entities = &drupal_static(__FUNCTION__);
    if (!isset($entities)) {
      if ($cache = cache_get('entity_locator_entities')) {
        $entities = $cache->data;
      }
      else {
        $entities = $this->_get_entities();
        cache_set('entity_locator_entities', $entities);
      }
    }
    return $entities;
  }

  /**
   * Get entities to display on the map.
   * It uses the bundle(s) specified in the Ctools panel configuration.
   */
  protected function _get_entities() {
    $entities = array();
    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', $this->bundle);

    $result = $query->execute();

    if (isset($result['node'])) {
      $entities = entity_load('node', array_keys($result['node']));
    }
    return $entities;
  }

  /**
   * Get entities to display on the map group by country code.
   * It uses the bundle(s) specified in the Ctools panel configuration.
   */
  public function get_entities_by_country() {
    $entities = &drupal_static(__FUNCTION__);
    if (!isset($entities)) {
      if ($cache = cache_get('entity_locator_entities_by_country')) {
        $entities = $cache->data;
      }
      else {
        $entities = $this->_get_entities_by_country();
        cache_set('entity_locator_entities_by_country', $entities);
      }
    }
    return $entities;
  }

  /**
   * Get entities to display on the map group by country code.
   * It uses the bundle(s) specified in the Ctools panel configuration.
   */
  protected function _get_entities_by_country() {
    $entities = $this->get_entities();
    $entities_by_country = array();
    foreach ($entities as $entity) {
      $country = $entity->location['country'];
      $entities_by_country[$country][] = $entity;
    }
    return $entities_by_country;
  }

  /**
   * Get distinct country code of stored entities.
   * It uses the bundle(s) specified in the Ctools panel configuration.
   */
  public function get_entities_countries() {
    $countries = &drupal_static(__FUNCTION__);
    if (!isset($countries)) {
      if ($cache = cache_get('entity_locator_entities_countries')) {
        $countries = $cache->data;
      }
      else {
        $countries = array_keys($this->get_entities_by_country());
        cache_set('entity_locator_entities_countries', $countries);
      }
    }
    return $countries;
  }

  /**
   * Get a list of taxonmy values.
   * Uses static caching.
   *
   *  @return  array
   *  An array of taxonomy terms entities.
   */
  public function get_categories($field_name) {
    $categories = &drupal_static(__FUNCTION__);
    if (!isset($categories)) {
      if ($cache = cache_get('entity_locator_' . $field_name . '_list')) {
        $categories = $cache->data;
      }
      else {
        $field_info = field_info_field($field_name);
        $category_name = $field_info['settings']['allowed_values'][0]['vocabulary'];
        $vocabulary = taxonomy_vocabulary_machine_name_load($category_name);
        $categories = taxonomy_get_tree($vocabulary->vid, 0, NULL, FALSE);
        cache_set('entity_locator_' . $category_name . '_list', $categories, 'cache');
      }
    }
    return $categories;
  }
}
