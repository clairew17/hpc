for i in `seq 1 1 4`
do
echo "submitting round $i..."
sbatch submit_63.batch $1
sbatch submit_127.batch $1
sbatch submit_511.batch $1
sbatch submit_1023.batch $1
done
