Entity Locator is a `Drupal <http://drupal.org>`_ (7.x) module that provides a CTools Content Type plugin to display entities on a Google Map.
It does not depend on the gmap module.

============
Installation
============

Dependencies
------------

* Node location, Location

The CTools plugin allows the user to select which entities to display on the map. As for now, the entity must be a location-enabled content type

* Chaos tools
* Entity API
* Location
* Node Locations
* Locale

Drupal sandbox project: https://drupal.org/sandbox/manumilou/2198535

Pane configuration
------------

For this to work, you need at least:

* A content type with Node Location enabled for it
* Nodes from this content type with a latitude and longitude

The module provides CTools content type plugin available through the Panels module. The plugin has its own category: Entity Locator. 

* Add entity_locator pane in a page
* Configure the entity_locator pane:
 * Select the content type to display
 * Select a taxonomy attached to it, if available
 * Select the map type


Enable filters
--------------

To enable filters along the map, you need to add a *Term reference* field to the content type, and select it from the pane settings form.
This will add checkboxes under the map to filter the list elements.

