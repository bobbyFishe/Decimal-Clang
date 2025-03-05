from decimal import *

class Decimals:
    def decimal_to_128bit_parts(value: Decimal):
        sign = 1 if value.is_signed() else 0
        abs_value = abs(value)
        coefficient, exponent = abs_value.as_tuple().digits, abs_value.as_tuple().exponent
        coefficient = int(''.join(map(str, coefficient)))
        exponent = -exponent
        mantissa_bin = bin(coefficient)[2:].zfill(96)
        mantissa_parts = [
            int(mantissa_bin[-32:], 2),
            int(mantissa_bin[-64:-32], 2),
            int(mantissa_bin[:-64] or '0', 2)
        ]
        control_word = (sign << 31) | (exponent << 16)

        parts = mantissa_parts + [control_word]
        return [bin(p)[2:].zfill(32) for p in parts]

    def binary_string_to_int(bin_str):
        return int(bin_str, 2)


    def decimal_from_128bit_binary(bin_parts):
        if len(bin_parts) != 4:
            raise ValueError("Неверный формат данных: ожидается 4 числа на одно значение Decimal")

        parts = [Decimals.binary_string_to_int(b) for b in bin_parts]
        mantissa = (parts[2] << 64) | (parts[1] << 32) | parts[0]

        sign = (parts[3] >> 31) & 1
        exponent = (parts[3] >> 16) & 0x7FFF

        result = Decimal(mantissa) * Decimal(10) ** -exponent
        if sign:
            result = -result

        return result

    def read_binary_decimals_from_file(filename):
        decimals = []
        with open(filename, 'r') as file:
            for line in file:
                binary_groups = line.strip().split(';')
                for group in binary_groups:
                    bin_parts = group.split(',')
                    decimals.append(Decimals.decimal_from_128bit_binary(bin_parts))

        return decimals
    
    def print_to_text(start_text, new_text):
        with open(start_text) as file:
                with open (new_text, 'w') as new_file:
                    for line in file:
                        parts = map(Decimal, line.strip('\n').split(','))
                        new_file.write(';'.join(','.join(map(str, Decimals.decimal_to_128bit_parts(x))) for x in parts) + '\n')

def main():
    Decimals.print_to_text('test.txt', 'dec.txt')

if __name__ == '__main__':
    main()