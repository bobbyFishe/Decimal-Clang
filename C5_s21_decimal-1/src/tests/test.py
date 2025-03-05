from decimal import *
def main():
    a = Decimal('7922816251426433759354395033')
    b = Decimal('0.6')
    print(Decimal.__add__(a,b)) 
if __name__ == '__main__':
    main()