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
    <div id="entity-locator-block-preview-places" class="clearfix">
      <div class="wrapper clearfix">
        <div class="left">
          <div class="place">
            <div class="caption">
              <div class="title">Le Pére Éternel</div>
              <div class="link"><a href="#">Visionner sur la carte</a></div>
            </div>
            <img alt="" src="/sites/all/themes/scsl/images/data/Untitled-2.png" />
          </div>
          <div class="place">
            <div class="caption">
              <div class="title">Le Pére Éternel</div>
              <div class="link"><a href="#">Visionner sur la carte</a></div>
            </div>
            <img alt="" src="/sites/all/themes/scsl/images/data/Untitled-3.png" />
          </div>
        </div>

        <div class="right">
          <div class="place">
            <div class="caption">
              <div class="title">Le Pére Éternel</div>
              <div class="link"><a href="#">Visionner sur la carte</a></div>
            </div>
            <img alt="" src="/sites/all/themes/scsl/images/data/Untitled-5.png" />
          </div>
          <div class="place">
            <div class="caption">
              <div class="title">Le Pére Éternel</div>
              <div class="link"><a href="#">Visionner sur la carte</a></div>
            </div>
            <img alt="" src="/sites/all/themes/scsl/images/data/Untitled-7.png" />
          </div>
        </div>
      </div>

      <div class="bottom">
        <div class="place">
          <div class="caption">
            <div class="title">Le Pére Éternel</div>
            <div class="link"><a href="#">Visionner sur la carte</a></div>
          </div>
          <img alt="" src="/sites/all/themes/scsl/images/data/Untitled-4.png" />
        </div>
      </div>

    </div>
  </div>
</div>