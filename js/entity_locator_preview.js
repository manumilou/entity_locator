(function ($) {
  Drupal.behaviors.testsSelectionCfml = {
    attach: function(context, settings) {

      var $filter_pc = $('#entity-locator-preview #filter_pc'); 
      var $filter_type = $('#entity-locator-preview #filter_type');
      var $filter_pt = $('#entity-locator-preview #filter_pt');

      $filter_pc.parent().hide();
      $filter_type .change(function(){
        var val = $(this).val();
        $filter_pc.parent().toggle(val == 'pc');
        $filter_pt.parent().toggle(val != 'pc');
      });
      
    }
  };
})(jQuery);
