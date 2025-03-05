from decimal import *; from to_our_num import Decimals; import sys

def main():
    getcontext().prec = 28
    getcontext().rounding = ROUND_FLOOR
    with open('test.txt') as file:
        Decimals.print_to_text('test.txt', 'sub,add,div,mul_start.txt')
        new = open('new.txt','w')
        for line in file:
            a,b = map(Decimal, line.strip('\n').split(','))
            new.write(f"{Decimal.__sub__(a,b)}, {Decimal.__add__(a,b)}, {Decimal(a)/Decimal(b)+Decimal('0')}, {Decimal.__mul__(a,b)} \n")
        new.close() 
    Decimals.print_to_text('new.txt', 'sub,add,div,mul.txt')

if __name__ == '__main__':
    main()