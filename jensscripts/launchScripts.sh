#!/bin/bash -l

num_procs=30

for i in $(seq 1  1 $num_procs)
do
  
rm batchfile_$i

#write the file
echo "### name of the job" >> batchfile_$i
echo "#PBS -N ex2_script_$i" >> batchfile_$i
echo "#PBS -l walltime=10:00:00" >> batchfile_$i
echo "#PBS -l nodes=1:ppn=1" >> batchfile_$i
echo "#PBS -q serial_onecore" >> batchfile_$i


#define env variables
echo export INDEX=$i >> batchfile_$i

echo cd $HOME/Desktop/Ex_2 >> batchfile_$i
echo pwd >> batchfile_$i

#Matlab section: to activate Matlab, remove the # chartacters from the 2 subsequent lines
#echo "module load matlab" >> batchfile_$i
#echo "matlab -r add -nodisplay -nodesktop -nojvm -nosplash <exitcmd.txt" >> batchfile_$i

#Python section: to activate Python, remove the # chartacters from the 3 subsequent lines
#echo "module load anaconda-python3" >> batchfile_$i
#echo "source /software/python/anaconda3/etc/profile.d/conda.sh"  >> batchfile_$i
#echo "python add.py" >> batchfile_$i


#R section: to activate R, remove the # chartacters from the 3 subsequent lines
echo "module load R-3.6.1-new" >> batchfile_$i
echo "Rscript add.R" >> batchfile_$i

qsub batchfile_$i

sleep 2
done
