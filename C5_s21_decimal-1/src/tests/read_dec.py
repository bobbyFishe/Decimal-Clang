from decimal import *
from tests.to_our_num import Decimals
import sys
def main(num_of_dec_per_line):
    decimals = Decimals.read_binary_decimals_from_file('dec.txt')
    for i in range(0, len(decimals), num_of_dec_per_line):
        print(*decimals[i:i+num_of_dec_per_line], sep=';')

if __name__ == '__main__':
    main(int(sys.argv[1]))