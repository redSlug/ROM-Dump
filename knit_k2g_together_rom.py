




def get_data(file_name):
    with open(file_name, 'r') as f:
        bits = f.readlines()[1:]
        return [int(l.split(',')[1], 16) for l in bits]



high_bits = get_data('a19_high_d0_d5_plus_d7.txt')
low_bits = get_data('a19_high_d0_d6.txt')

length = min(len(low_bits), len(high_bits))

#print(length)
#diffing_indices = [(i, a, b) for i, (a, b) in enumerate(zip(high_bits[:length], low_bits[:length])) if ((a & 0x3F) != (b & 0x3F)) and (a != 0 and b != 0)]

#print(diffing_indices)

#print(len(diffing_indices))

rom = bytes(((high & 0x40)) << 1 | (low & 0x7F) for high, low in zip(high_bits[:length], low_bits[:length]))
with open('rom_high.bin', 'wb') as f:
    f.write(rom)


