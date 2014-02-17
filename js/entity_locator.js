/**
 * @file entity_locator.js
 */

(function ($) {

  Drupal.behaviors.testsSelectionCfml = {
    attach: function(context, settings) {
      var queries = Drupal.settings.entity_locator.get_params;

      $(document).ready(function(){
        $sections = $(".section");
        $sections.children(".warper").hide();
        $sections.find('.arrow').removeClass('down').addClass('up');
        $sections.find("article").addClass('active');
      });
      
      $(".section a.title").click(function(){
          $this = $(this);
          var slideDown = $this.children(".arrow").hasClass('up');
          if(slideDown){
            $this.children(".arrow").removeClass('up');
            $this.next(".warper").slideDown();
          }else{
            $this.children(".arrow").addClass('up');
            $this.next(".warper").slideUp();
          } 
          return false;
      });
      
      $("#filters input:checkbox, #filters-menu #countries").change(function(){
        resfreshMarkersAndPlaces();
        refreshEmptySections();
      });

      function resfreshMarkersAndPlaces(){
        var showOrHide;
        var pcSelected = $("#filters-menu #countries").val();

        //refresh Gmaps markers
        for (var i = 0; i < initMarkers.length; i++) {
          var pt = initMarkers[i]['places_type'], ptSelected = $("#filters input#"+pt).is(':checked');
          showOrHide = (pcSelected == '' || pcSelected == initMarkers[i]['places_country']) && ptSelected;
          markers[i].setVisible(showOrHide);
        };

        //Refresh Places
        $('#entity_locator_list article').each(function(i){
          var $this = $(this), pt = $this.data('pt'), ptSelected = $("#filters input#"+pt).is(':checked');
          showOrHide = (pcSelected == '' || pcSelected == $this.data('pc')) && ptSelected;
          $this.toggle(showOrHide).toggleClass('active',showOrHide);
        });
      }

      function refreshEmptySections(){
        $(".section").each(function(i){
          var $this = $(this), isEmpty = $this.find('article.active').length == 0;
          $this.toggleClass('empty', isEmpty);
        });
      }

      function openPopSections(){
        $(".section").each(function(i){
          var $section = $(this), isEmpty = $section.find('article.active').length == 0;
          $section.find(".arrow").removeClass('up');
          $section.find(".warper").slideDown();
        });
      }

      function applyGetFilters(){
        if(typeof queries.filter_type != 'undefined'){
          if(queries.filter_type == 'pt' ){
            $("#filters input").attr('checked', false);
            var pt = queries.filter_pt;
            $("#filters input#"+pt).attr('checked', true);
          } else {
            var pc = queries.filter_pc;
            $('#filters-menu #countries').val(pc);
          }
          $("#filters-menu #countries, #filters input:checkbox").trigger('change');
          openPopSections();
        }
      }

      function initializeCallBack(){
        applyGetFilters();
      }

      var map;
      var initMarkers = [];
      var markers = [];

      // Google map initialization
      function initialize(callBack) {
        var lat = new google.maps.LatLng(47.56109365945179,-75.8935546875);
        var mapOptions = {
          center: lat,
          zoom: 2,
        };

        map = new google.maps.Map(document.getElementById("entity_locator"), mapOptions);

        // @todo Active watercolor
        //map.mapTypes.set("watercolor", new google.maps.StamenMapType("watercolor"));
        //map.setMapTypeId("watercolor");

        var markers = Drupal.settings.entity_locator.markers;
        initMarkers = markers;   

        // Set markers
        $.each(markers, function(index, value) {
          addMarker(value); 
        })

        callBack();
      }

      function addMarker(data){
        // Build the coordinate object
        var location = new google.maps.LatLng(data.lat, data.lng);
        var marker = new google.maps.Marker({
          position: location,
          animation: google.maps.Animation.DROP,
          map: map,
          title: data.title
        });
        
        // Set the content to display when the marker is clicked
        var infowindow = new google.maps.InfoWindow({
          content: data.title
        });

        // Add the click action
        google.maps.event.addListener(marker, 'click', function() {
          infowindow.open(map, marker);
        });

        markers.push(marker);
      }

      // Sets the map on all markers in the array.
      function setAllMap(map) {
        for (var i = 0; i < markers.length; i++) {
          markers[i].setMap(map);
        } 
      }

      // Shows any markers currently in the array.
      function showMarkers() {
        setAllMap(map);
      }

      // Removes the markers from the map, but keeps them in the array.
      function clearMarkers() {
        setAllMap(null);
      }

      function setmarkers(map) {
        // Hide the markers from the map
        clearMarkers();

        // Get bundle from configuration
        var bundle = Drupal.settings.entity_locator.bundle;

        // Fetch entities
        $.ajax(Drupal.settings.basePath + 'entity_locator/ajax/entities/' + bundle)
          .done(function(data) {

            //console.log(data);

            // Iterate over the results
            $.each(data, function(index, value) {
              addMarker(value);              
            })
          })
      }
      google.maps.event.addDomListener(window, 'load', initialize(initializeCallBack));
    }

  };
})(jQuery);

