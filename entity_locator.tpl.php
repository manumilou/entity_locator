<div id="entity_locator" style="height:450px;">Supposed to be a map.</div>
  <div id="filters-menu" class="clearfix">
    <div class="select-style">
      <select name="countries" id="countries">
        <option value=""><?php echo t('Pays'); ?> :</option>
          <?php foreach(array_keys($entities_by_country) as $key) {
            echo '<option value="pc-'.strtoupper($key).'">'.$countries[strtoupper($key)].'</option>';
          } ?>
      </select>
    </div>
  </div>

  <div id="entity_locator_list">
    <h2 class="title"><?php echo t('Liste des oeuvres'); ?></h2>
    <div class="filters-prepend"><?php echo t('Trier par'); ?> :</div>
    <div class="filters clearfix">
      <?php foreach($categories as $category) {
         echo '<input type="checkbox" checked="checked" name="pt-'.$category->tid.'" id="pt-'.$category->tid.'" /><label for="pt-'.$category->tid.'" >'.$category->name.'</label>';
      } ?>
    </div>
    <?php foreach($entities_by_country as $name => $entities): ?>
      <div class="entities-section clearfix" data-pc="pc-<?php echo strtoupper($name); ?>">
        <a href="#" class="title"><h3><?php echo $countries[strtoupper($name)]; ?></h3><div class="arrow up"></div></a>
        <div class="wrap">
          <?php foreach($entities as $entity) : ?>
            <?php
              $build = node_view($entity, 'teaser');
              $entityWrapper = entity_metadata_wrapper('node', $entity);
              $pt = $entityWrapper->field_type->getIdentifier();
              $pc = strtoupper($build['locations']['#locations'][0]['country']);
              echo '<article data-pt="pt-'.$pt.'" data-pc="pc-'.$pc.'" >'. drupal_render($build) .'</article>';
            ?>
          <?php endforeach; ?>
        </div>  
      </div>
    <?php endforeach; ?>
  </div>