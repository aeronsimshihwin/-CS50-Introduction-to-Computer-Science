def main():
    while True:
        #ask user how much x, change, is owed
        #assumption made: input, x will be in $ and cents, not cents only
        x=float(input('Enter how much change is owed: $'))
        if x<0:
            print("Monetary change should be postive. Try again.")
            x=float(input('Enter how much change is owed: $'))
        else:
            break
    change(x)

def change(x):
    #y in cents
    y=x*100
    #number of quarters, q
    q=0
    while y>=25:
        q+=1
        y-=25
    #number of dimes, d
    d=0
    while y>=10:
        d+=1
        y-=10
    #number of nickels, n
    n=0
    while y>=5:
        n+=1
        y-=5
    #number of pennies, p
    p=0
    while y>=1:
        p+=1
        y-=1
    #total coins received for change, c
    c=q+d+n+p
    #f'' string used to input whatever I want in { } and insert curly brackets anywhere
    print(f'You will get {q} quarters, {d} dimes, {n} nickels, {p} pennies and {c} coins in total as your change.')

if __name__ == "__main__":
    main()