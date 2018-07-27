# ROM-Dump

Scripts and code used for ROM dump.

## Bash commands used
Copies first 2**19 bytes; the size of the ROM because there are 19 address bits; copies without filesystem `/usr/share/file/magic`
```bash
dd if=rom.bin of=rom_low_clean.bin bs=1 count=$((2**19))
```

Breaks binary into smaller image files, adds headers
```bash
for n in $(seq 0 10); do dd if=../legend.bin skip=$(($n * 80000)) bs=1 count=80000 of=$n.bin; echo "P4 8 10000 " > $n.pgm; cat $n.bin >> $n.pgm; done
```

Rename with curlies
```bash
mv full_dump_d0_d5_{falling,rising}_edge.txt
```

Misc.
```bash
hexdump rom_full.bin            # see all hex values
xxd -c 32 rom_full.bin
binwalk -E rom_full.bin         # to find where interesting data might be
strings rom_full.bin            # find strings
```

![Legend Of Zelda in strings output](images/zelda_string.png)


