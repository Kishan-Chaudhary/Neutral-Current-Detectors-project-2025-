#!/bin/bash
#SBATCH --account=def-jillings-ab
#SBATCH --array=1-9
#SBATCH --time=3:00:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=1G
#SBATCH --output=logs/run_%A_%a.out
#SBATCH --error=logs/run_%A_%a.err

set -e

cd $SLURM_SUBMIT_DIR

sleep $(( (SLURM_ARRAY_TASK_ID - 1) * 10 ))

MACRO_FILE="macro/Run${SLURM_ARRAY_TASK_ID}.mac"

echo "[$(date)] Starting simulation for $MACRO_FILE on task $SLURM_ARRAY_TASK_ID"
./NCD $MACRO_FILE
echo "[$(date)] Finished simulation for $MACRO_FILE"
