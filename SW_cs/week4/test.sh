for f in ../testcases/eval*
do
	echo "===" $f "==="
	java Week4 $f
done
