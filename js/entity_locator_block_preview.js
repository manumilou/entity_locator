(function ($) {
  Drupal.behaviors.testsSelectionCfml = {
    attach: function(context, settings) {
      
      $('#filter_pc').parent().hide();
      $('#entity-locator-block-preview-filters #filter_type').change(function(){
        var val = $(this).val();
        $('#filter_pc').parent().toggle(val == 'pc');
        $('#filter_pt').parent().toggle(val != 'pc');
      });
      
    }
  };
})(jQuery);

