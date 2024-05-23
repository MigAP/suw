#!/bin/sh

strtest()
{
	if ! test "$2" = "$3"
	then
		printf 'FAIL: %s: %s != %s\n' "$1" "$2" "$3"
	fi
}

strtest time "0
1
2
3" "$(../time -i 0 -f 3 -s 1)"

strtest step "0	0
1	0
2	2
3	2" "$(../time -i 0 -f 3 -s 1 | ../step -i 0 -f 2 -t 2)"


strtest z-tf "1	4	11
2	5	16
3	6	22" "$(../z-tf -f z_trivial_tf.txt  -i 7.0 < trivial_test.dat)"
