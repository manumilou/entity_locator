<?php

class EntityLocator{

  private $_bundle;

  public function __construct($bundle) {
    $this->_bundle = $bundle;
  }

  /**
   * Get entities to display on the map.
   * It uses the bundle(s) specified in the Ctools panel configuration.
   */
  public function get_entities() {
    $entities = &drupal_static(__FUNCTION__);
    if(!isset($entities)) {
      if($cache = cache_get('entity_locator_entities')){
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
  private function _get_entities() {
    $entities = array();
    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', $this->_bundle);

    $result = $query->execute();

    if(isset($result['node'])) {
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
    if(!isset($entities)) {
      if($cache = cache_get('entity_locator_entities_by_country')){
        $entities = $cache->data;
      } else {
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
  private function _get_entities_by_country(){
    $entities = $this->get_entities();
    $entities_by_country =  array();
    foreach($entities as $entity) {
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
    if(!isset($countries)) {
      if($cache = cache_get('entity_locator_entities_countries')){
        $countries = $cache->data;
      } else {
        $countries = array_keys($this->get_entities_by_country());
        cache_set('entity_locator_entities_countries', $countries);
      }
    }
    return $countries;
  }


  /**
   * Get a list of houses types.
   * Houses are associated to a SCSL place through a CCK field: field_type.
   *
   * Uses static caching.
   *
   * @return
   *  An array of taxonomy terms entities representing houses, keyed by their identifier.
   */
  public static function houses() {
    $houses = &drupal_static(__FUNCTION__);
    if(!isset($houses)) {
      if($cache = cache_get('entity_locator_houses_list')) {
        $houses = $cache->data;
      }
      else {
        // Generate entire tree
        $vocabulary = taxonomy_vocabulary_machine_name_load('houses');
        $houses = taxonomy_get_tree($vocabulary->vid, 0, NULL, FALSE);
        cache_set('entity_locator_houses_list', $houses, 'cache');
      }
    }
    return $houses;
  }
}
