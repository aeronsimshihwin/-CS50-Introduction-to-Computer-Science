import cs50
import sys

if len(sys.argv) != 2:
    print('Usage: python caesar.py (non-negative integer)')
    exit(1)
else:
    k=int(sys.argv[1])
    print('Plaintext: ', end="")
    Plaintext=cs50.get_string()
    if Plaintext == None:
        exit(1)
    else:
        print('Ciphertext: ', end="")
        for c in Plaintext:
            #.isalpha() from cs50 to check if char c is alphabet
            if c.isalpha():
                #.isupper() from cs50 to check if char c is alphabet
                if c.isupper():
                    #ord function converts char to ascii numeric numbers
                    x=((ord(c)-65+k)%26)+65
                    #chr function converts ascii numeric numbers to char
                    print(chr(x), end="")
                else:
                    x=((ord(c)-97+k)%26)+97
                    print(chr(x), end="")
            else:
                print(c, end="")
        print()
    exit(0)