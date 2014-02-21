<?php

/**
 * @file
 * Template file for entity locator preview
 */

?>
<?php $countries_list = country_get_list(); ?>
<div id="entity-locator-preview">
  <div class="inside">
    <div class="filters">
      <div class="prepend"><?php echo t('Search by'); ?> :</div>
      <form method="get" action="<?php echo $path_to_map ?>">
        <div class="select-wrapper">
          <select id="filter_type" name="filter_type">
            <option value="pt"><?php echo t('Category'); ?></option>
            <option value="pc"><?php echo t('Countries'); ?></option>
          </select>
        </div>
        <div class="select-wrapper">
          <select id="filter_pt" name="filter_pt">
            <?php foreach ($houses as $key => $house): ?>
            	<option value="pt-<?php echo $house->tid; ?>"><?php echo $house->name; ?></option>
            <?php endforeach; ?>
          </select>
        </div>
        <div class="select-wrapper">
          <select id="filter_pc" name="filter_pc">
            <?php foreach ($countries as $key => $country): ?>
              <option value="pc-<?php echo strtoupper($country); ?>"><?php echo $countries_list[strtoupper($country)]; ?></option>
            <?php endforeach; ?>
          </select>
        </div>
        <input type="submit" value="<?php echo t('Search'); ?>" />
      </form>
    </div>
  </div>
</div>
