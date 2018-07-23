# ROM-Dump


```bash
for n in $(seq 0 10); do dd if=../legend.bin skip=$(($n * 80000)) bs=1 count=80000 of=$n.bin; echo "P4 8 10000 " > $n.pgm; cat $n.bin >> $n.pgm; done
```