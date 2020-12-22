// Google Map
var map;

// markers for map
var markers = [];

// info window
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    var styles = [

        // hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },

        // hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "off"}
            ]
        }

    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        center: {lat: 37.4236, lng: -122.1619}, // Stanford, California
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);

});

/**
 * Adds marker for place to map.
 */
function addMarker(place)
{
    // retrieve position (coordinates)
    var myLatLng = {lat: place.latitude, lng: place.longitude};

    // https://developers.google.com/maps/documentation/javascript/markers#simple_icons
    // Simple icons vs Complex icons
    // Simple icons : Set the marker's icon property to the URL of an image. The Google Maps JavaScript API will size the icon automatically.
    // Complex icons : Specify complex shapes to indicate regions that are clickable and specify how icons should appear relative to other overlays (their "stack order").
                    // Icons specified in this manner should set their icon properties to an object of type Icon. Icon objects define an image.
                    // They also define the size of the icon, the origin of the icon (if the image you want is part of a larger image in a sprite, for example),
                    // and the anchor where the icon's hotspot should be located (which is based on the origin).
                    // If you are using a label with a custom marker, you can position the label with the labelOrigin property in the Icon object.

    var image = {
        labelOrigin: new google.maps.Point(15, 45),
        url: 'http://www.btcpa.net/content/themes/bt2017/img/icon-newsletter.svg'
    }

    var marker = new google.maps.Marker({
        position: myLatLng,
        map: map,
        icon: image,
        label: place.place_name + ", "+ place.admin_name1,

    });

    // getting JSON file
    var articlecontent

    $.getJSON("articles", {geo: place.postal_code}, function(result) {

        articlecontent = '<ul>\n';
        for (var i = 0; i < result.length; i++)
            {
                // bullet point form together with url link
                articlecontent += '<li><a href="' + result[i].link + '">' + result[i].title + '</a></li>\n';
            }
            articlecontent += "</ul>"
    });

    google.maps.event.addListener(marker, 'click', function() {
        showInfo(marker, articlecontent);

    });

    marker.addListener('dblclick', function() {
        map.setZoom(8);
        map.setCenter(marker.getPosition());
    });

    markers.push(marker)
}

/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {

        // if info window isn't open
        // http://stackoverflow.com/a/12410385
        if (!info.getMap || !info.getMap())
        {
            update();
        }
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // configure typeahead
    $("#q").typeahead({
        highlight: false,
        minLength: 1
    },
    {
        display: function(suggestion) { return null; },
        limit: 10,
        source: search,
        templates: {
            suggestion: Handlebars.compile(
                "<div>" + "{{place_name}}, " + "{{admin_name1}}, " + "{{postal_code}}" + "</div>"
            )
        }
    });

    // re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // set map's center
        map.setCenter({lat: parseFloat(suggestion.latitude), lng: parseFloat(suggestion.longitude)});

        // update UI
        update();
    });

    // hide info window when text box has focus
    $("#q").focus(function(eventData) {
        info.close();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true;
        event.stopPropagation && event.stopPropagation();
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box
    $("#q").focus();
}

/**
 * Removes markers from map.
 */
function removeMarkers()
{
    // to manage a set of markers, create an array to hold the markers.
    // call setMap() on each marker in the array in turn when you need to remove the markers. (Iteration)
    for (var j = 0; j < markers.length; j++)
    {
        // To remove a marker from the map, call the setMap() method passing null as the argument.
        markers[j].setMap(null);
    }
    // delete the markers by removing them from the map
    // then setting the array's length to 0, which removes all references to the markers.
    markers = [];
}

/**
 * Searches database for typeahead's suggestions.
 */
function search(query, syncResults, asyncResults)
{
    // get places matching query (asynchronously)
    var parameters = {
        q: query
    };
    $.getJSON(Flask.url_for("search"), parameters)
    .done(function(data, textStatus, jqXHR) {

        // call typeahead's callback with search results (i.e., places)
        asyncResults(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());

        // call typeahead's callback with no results
        asyncResults([]);
    });
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) == "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='/static/ajax-loader.gif'/>";
    }
    else
    {
        div += content;
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);

    // open info window (if not already open)
    info.open(map, marker);
}

/**
 * Updates UI's markers.
 */
function update()
{
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng()
    };
    $.getJSON(Flask.url_for("update"), parameters)
    .done(function(data, textStatus, jqXHR) {

       // remove old markers from map
       removeMarkers();

       // add new markers to map
       for (var i = 0; i < data.length; i++)
       {
           addMarker(data[i]);
       }
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
};
