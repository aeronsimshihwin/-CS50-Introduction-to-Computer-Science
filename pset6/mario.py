#define height as variable n
n=int(input('Enter the height of the half-pyramid Mario is climbing: '))

#prompt user if n>23 or n<1
while n>23 or n<1:
    print('Height of half-pyramid must be a positive non-zero integer, no greater than 23. Try Again.')
    n=int(input('Enter the height of the half-pyramid Mario is climbing: '))

#define space and hash as variables s and h respectively
s=n-1
h=2
while s>=0:
    print(' '*s + '#'*h)
    s-=1
    h+=1