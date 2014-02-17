<?php $countriesList = country_get_list(); ?>
<div id="entity-locator-block-preview">
  <div class="inside">
    <div id="entity-locator-block-preview-filters">
      <div class="prepend"><?php echo t('Search by'); ?> :</div>
      <form method="get" action="<?php echo $path_to_map ?>">
        <div class="select-style">
          <select id="filter_type" name="filter_type">
            <option value="pt"><?php echo t('Category'); ?></option>
            <option value="pc"><?php echo t('Countries'); ?></option>
          </select>
        </div>
        <div class="select-style">
          <select id="filter_pt" name="filter_pt">
            <?php foreach ($houses as $key => $house) {
            	echo '<option value="pt-'.$house->tid.'">'.$house->name.'</option>';
            } ?>
          </select>
        </div>
        <div class="select-style">
          <select id="filter_pc" name="filter_pc">
            <?php foreach ($countries as $key => $country) {
            	echo '<option value="pc-'.strtoupper($country).'">'.$countriesList[strtoupper($country)].'</option>';
            } ?>
          </select>
        </div>
        <input type="submit" id="btn_search" value="<?php echo t('Search'); ?>" />
      </form>
    </div>
</div>