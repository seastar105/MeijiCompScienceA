for f in ../testcases/syn-err*
do
	echo "===" $f "==="
	java Week2 $f
done
