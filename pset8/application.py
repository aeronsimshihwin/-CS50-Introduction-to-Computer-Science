import os
import re
from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue

from cs50 import SQL
from helpers import lookup

# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")

@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))

@app.route("/articles")
def articles():
    """Look up articles for geo."""

    # Zamyla's help - retrieve geo argument from HTML form
    request.args.get("geo")

    # Zamyla's help - check for missing argument and triggering RuntimeError if necessary
    # mimic how app.route("/update") does this check and raise RuntimeError
    if not request.args.get("geo"):
        raise RuntimeError("missing geo")

    # lookup for articles aka news in that particular geo
    articles = lookup(request.args.get("geo"))

    if len(articles)>5:
        return jsonify([articles[0], articles[1], articles[2], articles[3], articles[4]])
    else:
        return jsonify([articles])

@app.route("/search")
def search():
    """Search for places that match query."""

    # retrieve q0 from HTML form
    q0 = request.args.get("q")
    # "%" is appended to user's input as it is SQL's "wildcard" character aka "match any number of characters"
    q1 = q0 + "%"

    # if q0 is a 'nice' input like solely postal code/ solely place name/ solely admin name
    # scan through SQL table "places" for postal_code, place_name(city) or admin_name(state) that starts with 'q'
    address = db.execute("SELECT * FROM places WHERE postal_code = :q OR place_name = :q OR admin_name1 = :q", q=q0)

    # if user did not give a 'nice' input, address will be NULL, then need to either use the wildcard '%' or help split the 'ugly' input by user
    if not address:
        address = db.execute("SELECT * FROM places WHERE postal_code LIKE :q OR place_name LIKE :q OR admin_name1 LIKE :q", q=q1)
        # after line 67 if wildcard did not work and address is NULL, means user gave 'ugly' input
        if not address:
            # remove commas in input if there are
            qnow = q0.replace(",","")
            # remove semicolons in inputs if there are
            qnow = qnow.replace(";","")
            # use split() function which will automatically split the qnow into individual words at the spaces)
            number_of_words = qnow.split()

            for i in range(len(number_of_words)):
                # retrieve last word in qnow by using splittosearch() function
                qnow = splittosearch(qnow)
                address = db.execute("SELECT * FROM places WHERE postal_code LIKE :q OR place_name LIKE :q OR admin_name1 LIKE :q", q=qnow)
                # if address is still NULL, it's ok, just continue in the for loop and remove the next last word from qnow
                if len(address)>0:
                    break

    #continued from line 63 if address is not NULL
    if len(address)>5:
        return jsonify([address[0], address[1], address[2], address[3], address[4]])
    else:
        return jsonify(address)

@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # output places as JSON
    return jsonify(rows)

def splittosearch(n):
    data = n.split()
    result=""
    for i in range(len(data)-1):
        if (i<len(data)-2):
            result=result+data[i]+" "
        else:
            result=result+data[i]
    return result