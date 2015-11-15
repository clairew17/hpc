for i in `seq 1 1 4`
do
	echo "submit $i..."
	sbatch submit_serial.batch $1
done

