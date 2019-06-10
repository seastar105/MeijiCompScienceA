for f in ../testcases/sem-err*
do
	echo "===" $f "==="
	c0x -3 $f
done
