(function ($) {
  Drupal.behaviors.testsSelectionCfml = {
    attach: function(context, settings) {
      
      $('#filter_pc').parent().hide();

      $('#entity-locator-block-preview-places .place').mouseenter(function(){
        $(this).children('.caption').fadeIn();
      }).mouseleave(function(){
        $(this).children('.caption').fadeOut();
      });

      $('#entity-locator-block-preview-filters #filter_type').change(function(){
        if($(this).val() == 'pc'){
          $('#filter_pc').parent().show();
          $('#filter_pt').parent().hide();
        }else{
          $('#filter_pc').parent().hide();
          $('#filter_pt').parent().show();
        }
      });
      
    }
  };
})(jQuery);

