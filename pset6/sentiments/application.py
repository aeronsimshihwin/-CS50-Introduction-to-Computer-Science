from flask import Flask, redirect, render_template, request, url_for

import helpers
import os
import sys
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name)
    #returns to index if None
    if tweets==None:
        return redirect(url_for("index"))

    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    #analyzes the sentiment of each of those tweets
    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)

    #define positive, negative and neutral and start from 0 count
    positive, negative, neutral = 0, 0, 0

    #iterate over every tweet and tabulate score
    for tweet in tweets:
        score = analyzer.analyze(tweet)
        if score>0.0:
            positive+=1
        elif score<0.0:
            negative+=1
        else:
            neutral+=1

    totaltweets=len(tweets)

    #convert positive, negative, and neutral into percentages
    positive = (positive / totaltweets) * 100
    negative = (negative / totaltweets) * 100
    neutral = (neutral / totaltweets) * 100

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
