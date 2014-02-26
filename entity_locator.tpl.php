<?php

/**
 * @file
 * Template for entity locator template
 */

?>
<div id="entity_locator" style="height:<?php echo $map_height; ?>;">Supposed to be a map.</div>
  <div id="filters-menu" class="clearfix">
    <div class="select-style">
      <select name="countries" id="countries">
        <option value=""><?php echo t('Pays'); ?> :</option>
          <?php foreach (array_keys($entities_by_country) as $key) : ?>
            <option value="pc-<?php echo strtoupper($key); ?>"><?php echo $countries[strtoupper($key)]; ?></option>
          <?php endforeach; ?>
      </select>
    </div>
  </div>
  
  <div id="entity_locator_list">
    <h2 class="title"><?php echo t($prepend_text); ?></h2>
    <?php if (! empty($categories)): ?>
      <div class="filters-prepend"><?php echo t('Filter by'); ?> :</div>
      <div class="filters clearfix">
        <?php foreach($categories as $category) {
           echo '<input type="checkbox" checked="checked" name="pt-' . $category->tid . '" id="pt-' . $category->tid . '" />';
           echo '<label for="pt-' . $category->tid . '" >' . $category->name . '</label>';
        } ?>
      </div>
    <?php endif; ?>
    <?php foreach ($entities_by_country as $name => $entities): ?>
      <div class="entities-section clearfix" data-pc="pc-<?php echo strtoupper($name); ?>">
        <a href="#" class="title"><h3><?php echo $countries[strtoupper($name)]; ?></h3><div class="arrow up"></div></a>
        <div class="wrap">
          <?php foreach ($entities as $entity) : ?>
            <?php
              $build = node_view($entity, 'teaser');
              $pt = "";
              if (! empty($node_category)) {
                $pt = implode(",", $entity->place_types);
              }
              $pc = strtoupper($build['locations']['#locations'][0]['country']);
              echo '<article data-pt="' . $pt . '" data-pc="pc-' . $pc . '" >' . drupal_render($build) . '</article>';
            ?>
          <?php endforeach; ?>
        </div>  
      </div>
    <?php endforeach; ?>
  </div>
