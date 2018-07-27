
def get_data(file_name):
    with open(file_name, 'r') as f:
        bits = f.readlines()[1:]
        return [int(l.split(',')[1], 16) for l in bits]

low_bits_measured = get_data('full_dump_d0_d6_rising_edge_10us.txt')
high_bits_measured = get_data('full_dump_d0d5_d7_rising_edge.txt')

length = 2**20
bit0_6 = 0x7F
bit_6 = 0x40

# shifting bit 6 to bit 7
rom = bytes(((high & bit_6)) << 1 | (low & bit0_6) for low, high in zip(low_bits_measured[:length], high_bits_measured[:length]))
with open('rom_full.bin', 'wb') as f:
    f.write(rom)


# verify
with open('rom_official.bin', 'rb') as f:
    actual_rom = f.read()[:2**19]

first_6_bit_mask = 0x3F
full_mask = 0xFF
diff_mask = full_mask

diffing_indices = [(i, a, b) for i, (a, b) in enumerate(zip(rom[:length], actual_rom[:length])) if ((a & diff_mask) != (b & diff_mask))]
print(length)
print(len(diffing_indices))
# 14056 / 531007 = 3% noisy with falling edge
