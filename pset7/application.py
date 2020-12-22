from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Main Page for User"""
    #SELECT all symbols from user's portfolio to transfer them to table
    portfolio_symbols=db.execute("SELECT symbol, quantity FROM portfolio WHERE id=:id", id=session["user_id"])

    #assign variable to tabulate total value of holding stocks
    total_value_of_holding_stocks=0

    #iterate over all the selected symbols to retrieve their name, number of shares, price and total value
    for portfolio_symbol in portfolio_symbols:
        symbol=portfolio_symbol["symbol"]
        quantity=portfolio_symbol["quantity"]
        stock=lookup(symbol)
        price=stock["price"]
        total=price*quantity
        total_value_of_holding_stocks+=total
        db.execute("UPDATE portfolio SET price=:price, total=:total WHERE id=:id AND symbol=:symbol", price=stock["price"], total=total, id=session["user_id"], symbol=stock["symbol"])

    #SELECT user's updated_cash in portfolio after buying or selling stocks
    updated_cash=db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])

    #grand_total = stocks' total value + newcash
    grand_total=total_value_of_holding_stocks+updated_cash[0]["cash"]

    #updated_portfolio used as placeholder to equal to portfolio in index.html, cos need to print the id, name, symbol, quantity, price and total
    updated_portfolio=db.execute("SELECT * FROM portfolio WHERE id=:id", id=session["user_id"])

    return render_template("index.html", portfolio=updated_portfolio, VALUE=usd(total_value_of_holding_stocks), CASH=usd(updated_cash[0]["cash"]), TOTAL=usd(grand_total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    #if user reached route via POST (as by submitting a form via POST)
    if request.method=="POST":

        #Stock symbol field is empty
        if not request.form.get("symbol"):
            return apology("Missing Stock Symbol!")

        #Stock symbol field is filled in but needs to be checked against yahoo finance if valid
        stock=lookup(request.form.get("symbol"))

        if not stock:
            return apology("Please enter a valid Stock Symbol!")

        #SEE NOTES for try and except clauses
        try:
            #convert user's input of quantity of shares into int
            quantity=int(request.form.get("shares"))

            #check if user's input is a positive integer
            if quantity<1:
                return apology("To buy shares, please enter a positive integer for quantity of shares!")
        except:
            return apology("To buy shares, please enter a positive integer for quantity of shares!")

        else:
            #SELECT user's cash to buy shares
            money=db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])

            #check if user has enough cash to buy shares
            if not money or float(money[0]["cash"])<(stock["price"]*quantity):
                return apology("Not enough cash!")

            else:
                #UPDATE user's cash after buying shares
                db.execute("UPDATE users SET cash=:cashleft WHERE id = :id", cashleft=money[0]["cash"]-(stock["price"]*quantity), id=session["user_id"])

                #INSERT symbol, quantity, price, total, id into history after buying shares
                db.execute("INSERT INTO history (symbol, quantity, price, total, id, buyorsell) VALUES (:symbol, :quantity, :price, :total, :id, :buy)", symbol=stock["symbol"], quantity=quantity, price=usd(stock["price"]), total=usd(stock["price"]*quantity), id=session["user_id"], buy='bought')

                #SELECT shares FROM portfolio that users have been bought before
                user_shares=db.execute("SELECT quantity FROM portfolio WHERE id = :id AND symbol = :symbol", id=session["user_id"], symbol=stock["symbol"])

                #match share bought to SELECTed shares
                #if not the same, create new row in portfolio
                if not user_shares:
                    db.execute("INSERT INTO portfolio (id, name, symbol, quantity, price, total) VALUES (:id, :name, :symbol, :quantity, :price, :total)", id=session["user_id"], name=stock["name"], symbol=stock["symbol"], quantity=quantity, price=usd(stock["price"]), total=usd(stock["price"]*quantity))

                #else if same, add to current row and increment shares and total
                else:
                    user_total_shares=user_shares[0]["quantity"]+quantity
                    db.execute("UPDATE portfolio SET quantity =:quantity WHERE id=:id AND symbol=:symbol", quantity=user_total_shares, id=session["user_id"], symbol=stock["symbol"])
                    db.execute("UPDATE portfolio SET total =:totalupdated WHERE id =:id AND symbol=:symbol", totalupdated=usd(stock["price"]*user_total_shares), id=session["user_id"], symbol=stock["symbol"])

            # redirect user to home page
            return redirect(url_for("index"))

    #else if user reached route via GET (as by clicking a link or via redirect)
    else:
        username=db.execute("SELECT username FROM users WHERE id=:id", id=session["user_id"])
        return render_template("buy.html", username=username[0]["username"])

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    history=db.execute("SELECT * FROM history WHERE id=:id", id=session["user_id"])

    return render_template("history.html", history=history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method=="POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows=db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows)!=1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"]=rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""
    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    #if user reached route via POST (as by submitting a form via POST)
    if request.method=="POST":

        #lookup function in helpers.py applied to symbol searched by user and that's the stock
        stock=lookup(request.form.get("symbol"))

        #if lookup function unable to find a stock with that symbol, tell user symbol invalid
        if not stock:
            return apology("Invalid Symbol!")

        #lookup function successful, return quoted html where stock name, price and symbol are shown
        return render_template("quoted.html", Stock=stock, VALUE=usd(stock["price"]))

    #else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    #if user reached route via POST (as by submitting a form via POST)
    if request.method=="POST":

        #username field is blank
        if not request.form.get("username"):
            return apology("Missing Username!")

        #password field is blank
        elif not request.form.get("password"):
            return apology("Missing Password!")

        #password confirmation field is blank
        elif not request.form.get("confirmation"):
            return apology("Missing Password Confirmation")

        #password and password confirmation do not match
        elif request.form.get("password")!=request.form.get("confirmation"):
            return apology("2 Passwords do not match!")

        #add user to database, usernames are a UNIQUE field in database
        result=db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"), hash=pwd_context.hash(request.form.get("password")))

        #username already exists
        if not result:
            return apology("Username already exists!")
        else:
            return redirect(url_for("index"))

    #else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    #if user reached route via POST (as by submitting a form via POST)
    if request.method=="POST":

        #Stock symbol field is empty
        if not request.form.get("symbol"):
            return apology("Missing Stock Symbol!")

        #Stock symbol field is filled in but needs to be checked against yahoo finance if valid
        stock=lookup(request.form.get("symbol"))

        if not stock:
            return apology("Please enter a valid Stock Symbol!")

        #SEE NOTES for try and except clauses
        try:
            #convert user's input of quantity of shares into int
            quantity=int(request.form.get("shares"))

            #check if user's input is a positive integer
            if quantity<1:
                return apology("To sell shares, please enter a positive integer for quantity of shares!")
        except:
            return apology("To sell shares, please enter a positive integer for quantity of shares!")

        else:
            #SELECT shares FROM portfolio to sell
            sell_shares=db.execute("SELECT quantity FROM portfolio WHERE id=:id AND symbol=:symbol", id=session["user_id"], symbol=stock["symbol"])

            #check if user has that much shares to sell in the first place
            if not sell_shares or int(sell_shares[0]["quantity"])<quantity:
                return apology("You do not have that much shares to sell!")

            else:
                #SELECT currentcash FROM users before selling shares
                currentcash=db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])

                #UPDATE cash in users after selling shares, so should increase
                db.execute("UPDATE users SET cash=:morecash WHERE id=:id", morecash=currentcash[0]["cash"]+(stock["price"]*quantity), id=session["user_id"])

                #INSERT symbol, quantity, price, total, id into history after selling shares
                db.execute("INSERT INTO history (symbol, quantity, price, total, id, buyorsell) VALUES (:symbol, :quantity, :price, :total, :id, :sell)", symbol=stock["symbol"], quantity=-(quantity), price=usd(stock["price"]), total=usd(stock["price"]*quantity), id=session["user_id"], sell='sold')

                #UPDATE quantity in portfolio after selling shares, so should decrease or become zero
                if not sell_shares or int(sell_shares[0]["quantity"])==quantity:
                    #then user will have zero quantity of this share, so can DELETE FROM portfolio
                    db.execute("DELETE FROM portfolio WHERE id=:id AND symbol=:symbol", id=session["user_id"], symbol=stock["symbol"])

                else:
                    user_total_shares=sell_shares[0]["quantity"]-quantity
                    #UPDATE the remaining quantity of share in portfolio that user has after selling shares
                    db.execute("UPDATE portfolio SET quantity=:leftover WHERE id=:id AND symbol=:symbol", leftover=user_total_shares, id=session["user_id"], symbol=stock["symbol"])

            # redirect user to home page
            return redirect(url_for("index"))

    #else if user reached route via GET (as by clicking a link or via redirect)
    else:

        portfolio = db.execute("SELECT * FROM portfolio WHERE id = :id", id=session["user_id"])
        return render_template("sell.html", portfolio=portfolio)

@app.route("/helpp", methods=["GET", "POST"])
@login_required
def helpp():
    """User's guide to how to use C$50 Finance"""
    #if user reached route via POST (as by submitting a form via POST)
    if request.method=="POST":
        return redirect(url_for("index"))
    else:
        return render_template("helpp.html")

@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():
    """Settings for user to change password"""
    #if user reached route via POST (as by submitting a form via POST)
    if request.method=="POST":
        try:
            correctoldpw=db.execute("SELECT hash FROM users WHERE id=:id", id=session["user_id"])
            if not pwd_context.hash(request.form.get("oldpw"))!=correctoldpw:
                return apology("Your old password is incorrect!")
        except:
            return apology("Your old password is incorrect!")

        if request.form.get("newpw")==request.form.get("oldpw"):
            return apology("Old and new passwords same! Change what?!")
        else:
            if request.form.get("newpw")!=request.form.get("newpwc"):
                return apology("Your new password and new password confirmation do not match!")
            else:
                newpw=pwd_context.hash(request.form.get("newpw"))
                db.execute("UPDATE users SET hash=:newhash WHERE id=:id", newhash=newpw, id=session["user_id"])

            return render_template("success.html")
    else:
        return render_template("settings.html")

@app.route("/topup", methods=["GET", "POST"])
@login_required
def topup():
    """User's guide to how to use C$50 Finance"""
    #if user reached route via POST (as by submitting a form via POST)
    if request.method=="POST":
        if int(request.form.get("topup"))<0 or int(request.form.get("topup"))==None:
            return apology("Please enter a positive integer to top up!")
        else:
            usercash=db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
            userupdatedcash=usercash[0]["cash"]+int(request.form.get("topup"))
            db.execute("UPDATE users SET cash=:updatedcash WHERE id=:id", updatedcash=userupdatedcash, id=session["user_id"])
            return render_template("joke.html")
    else:
        return render_template("topup.html")