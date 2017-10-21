#!/bin/bash
#SBATCH --time=10:00:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem-per-cpu=2000

# Code to run
echo "    ---    "
echo "improved-Sequential"
echo "    ---    "
#   ----   #
#  Case 1  #
#   ----   #
echo "Case 1"
date +%s%3N
srun out/improved-sequential 12 resources/case_1.txt
date +%s%3N
srun out/improved-sequential 12 resources/case_1.txt
date +%s%3N
srun out/improved-sequential 12 resources/case_1.txt
date +%s%3N
srun out/improved-sequential 12 resources/case_1.txt
date +%s%3N
srun out/improved-sequential 12 resources/case_1.txt
date +%s%3N
#   ----   #
#  Case 2  #
#   ----   #
echo "Case 2"
date +%s%3N
srun out/improved-sequential 12 resources/case_2.txt
date +%s%3N
srun out/improved-sequential 12 resources/case_2.txt
date +%s%3N
srun out/improved-sequential 12 resources/case_2.txt
date +%s%3N
srun out/improved-sequential 12 resources/case_2.txt
date +%s%3N
srun out/improved-sequential 12 resources/case_2.txt
date +%s%3N
#   ----   #
#  Case 3  #
#   ----   #
echo "Case 3"
date +%s%3N
srun out/improved-sequential 1000000000 resources/case_3.txt
date +%s%3N
srun out/improved-sequential 1000000000 resources/case_3.txt
date +%s%3N
srun out/improved-sequential 1000000000 resources/case_3.txt
date +%s%3N
srun out/improved-sequential 1000000000 resources/case_3.txt
date +%s%3N
srun out/improved-sequential 1000000000 resources/case_3.txt
date +%s%3N
#   ----   #
#  Case 4  #
#   ----   #
echo "Case 4"
date +%s%3N
srun out/improved-sequential 9513 resources/case_4.txt
date +%s%3N
srun out/improved-sequential 9513 resources/case_4.txt
date +%s%3N
srun out/improved-sequential 9513 resources/case_4.txt
date +%s%3N
srun out/improved-sequential 9513 resources/case_4.txt
date +%s%3N
srun out/improved-sequential 9513 resources/case_4.txt
date +%s%3N
#   ----   #
#  Case 4  #
#   ----   #
echo "Case 5"
date +%s%3N
srun out/improved-sequential 458965 resources/case_5.txt
date +%s%3N
srun out/improved-sequential 458965 resources/case_5.txt
date +%s%3N
srun out/improved-sequential 458965 resources/case_5.txt
date +%s%3N
srun out/improved-sequential 458965 resources/case_5.txt
date +%s%3N
srun out/improved-sequential 458965 resources/case_5.txt
date +%s%3N
#   ----   #
#  Case 6  #
#   ----   #
echo "Case 6"
date +%s%3N
srun out/improved-sequential 24676 resources/case_6.txt
date +%s%3N
srun out/improved-sequential 24676 resources/case_6.txt
date +%s%3N
srun out/improved-sequential 24676 resources/case_6.txt
date +%s%3N
srun out/improved-sequential 24676 resources/case_6.txt
date +%s%3N
srun out/improved-sequential 24676 resources/case_6.txt
date +%s%3N
