# -*- coding: utf-8 -*-
"""
Neutron Spectrum Unfolding Script: Parent Code: Tyler Dolezal(https://github.com/tylerdolezal/Neutron-Unfolding), Edited by Kishan Chaudhary, aknwoledge use of CHATGPT for code optimization and comments.
---------------------------------
This script processes experimental neutron count data and 'unfolds' the 
energy spectrum using four different mathematical approaches: 
GRAVEL, MLEM, LU, and SVD. 

It uses Monte Carlo sampling to propagate uncertainties from both the 
measured counts and the detector response matrix.
"""

import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from matplotlib.ticker import (MultipleLocator, AutoMinorLocator)
import math

# Custom algorithm imports
from mlem import mlem 
from gravel import gravel 
from Read_Response import R1, R2, R2_error, R3, R3_error, EnergiesHR, bin_labels_2 

# Visual Styling
sns.set_theme()
sns.set_style('ticks')
plt.rc('font', family='serif')

# =============================================================================
# DATA PRE-PROCESSING FUNCTIONS
# =============================================================================

def CorrectedCounts(TotalCounts, Count_Correction, Count_Correction_Error):
    """
    Applies calibration factors and calculates error propagation.
    Returns: corrected counts, statistical error, and systematic error.
    """
    counts = np.array(TotalCounts)
    # Statistical error (Poisson) is the square root of raw counts
    counts_stat = np.sqrt(counts)
    
    # Apply Correction Factor
    counts_corr = counts / np.array(Count_Correction)
    counts_corr_stat = counts_stat / np.array(Count_Correction)
    
    # Systematic error propagation: (Counts * (Error_Correction / Correction))
    counts_corr_sys = counts_corr * (np.array(Count_Correction_Error) / np.array(Count_Correction))
    
    return counts_corr, counts_corr_stat, counts_corr_sys

def ConvertToCPS(Counts, Counts_stat, Counts_sys, time):
    """Converts counts and errors to Counts Per Second (CPS)."""
    t = np.array(time)
    return np.array(Counts)/t, np.array(Counts_stat)/t, np.array(Counts_sys)/t

# =============================================================================
# UNFOLDING EXECUTION HELPERS
# =============================================================================

def execute(initial_guess, R, N, tol):
    """
    Runs a single iteration of GRAVEL and MLEM algorithms.
    initial_guess: 0 for constant (ones), 1 for random.
    """
    x_ones = np.ones(N.shape)
    x_rand = np.random.random(N.shape)

    if initial_guess == 0:
        xg, errorg = gravel(R, N, x_ones, tol)
        xm, errorm = mlem(R, N, x_ones, tol)
    else:
        xg, errorg = gravel(R, N, x_rand, tol)
        xm, errorm = mlem(R, N, x_rand, tol)

    return xg, errorg, xm, errorm

# =============================================================================
# MONTE CARLO ERROR PROPAGATION METHODS
# =============================================================================

def MonteCarlo_unfolding(Count_Matrix, Count_Errors, Response_Matrix, Response_Errors, initialGuess, tolerance, iterations):
    """Propagates uncertainties through iterative algorithms (GRAVEL/MLEM)."""
    Count_sample = np.random.normal(loc=Count_Matrix, scale=Count_Errors, size=(iterations, len(Count_Matrix)))
    rows, cols = Response_Matrix.shape
    Response_sample = np.random.normal(loc=Response_Matrix, scale=Response_Errors, size=(iterations, rows, cols))
    
    shape = (iterations, Response_Matrix.shape[1])
    X_GRAVEL, X_MLEM = np.zeros(shape), np.zeros(shape)

    for i in range(iterations):
        X_GRAVEL[i], _, X_MLEM[i], _ = execute(initialGuess, Response_sample[i], Count_sample[i,:], tolerance)
    
    return np.mean(X_GRAVEL, axis=0), np.std(X_GRAVEL, axis=0), \
           np.mean(X_MLEM, axis=0), np.std(X_MLEM, axis=0)

def MonteCarlo_LU(Count_Matrix, Count_Errors, Response_Matrix, Response_Errors, iterations):
    """Propagates uncertainties through direct Matrix Inversion (LU)."""
    Count_sample = np.random.normal(loc=Count_Matrix, scale=Count_Errors, size=(iterations, len(Count_Matrix)))
    Response_sample = np.random.normal(loc=Response_Matrix, scale=Response_Errors, size=(iterations, *Response_Matrix.shape))
    
    Spectrum = np.zeros((iterations, Response_Matrix.shape[1]))
    for i in range(iterations):
        Spectrum[i] = np.linalg.solve(Response_sample[i], Count_sample[i,:])
    
    return np.mean(Spectrum, axis=0), np.std(Spectrum, axis=0)

def MonteCarlo_SVD(Count_Matrix, Count_Errors, Response_Matrix, Response_Errors, iterations):
    """Propagates uncertainties through Singular Value Decomposition (Pseudo-inverse)."""
    Count_sample = np.random.normal(loc=Count_Matrix, scale=Count_Errors, size=(iterations, len(Count_Matrix)))
    Response_sample = np.random.normal(loc=Response_Matrix, scale=Response_Errors, size=(iterations, *Response_Matrix.shape))
    
    Spectrum = np.zeros((iterations, Response_Matrix.shape[1]))
    for i in range(iterations):
        Spectrum[i] = np.linalg.pinv(Response_sample[i]).dot(Count_sample[i,:])
    
    return np.mean(Spectrum, axis=0), np.std(Spectrum, axis=0)

# =============================================================================
# MAIN EXECUTION SCRIPT
# =============================================================================

# 1. Setup Constants & Calibration Factors
CORR_VAL = [0.787, 0.787, 0.787]
CORR_ERR = [0.02, 0.02, 0.02]

# 2. Input Experimental Data (Order: 1-inch, 2-inch, Bare)
Counts_raw = [19385, 12617, 31353] 
Active_time = [1052939.333, 1031207.667, 1463278] 

# 3. Apply corrections and convert to Counts Per Second (CPS)
C_corr, C_stat, C_sys = CorrectedCounts(Counts_raw, CORR_VAL, CORR_ERR)
CPS, CPS_stat, CPS_sys = ConvertToCPS(C_corr, C_stat, C_sys, Active_time)

# 4. Run Unfolding with Monte Carlo Uncertainty (1000 iterations)
total_error = CPS_stat + CPS_sys

X_GRAVEL, G_STD, X_MLEM, M_STD = MonteCarlo_unfolding(CPS, total_error, R3, R3_error, 
                                                      initialGuess = 1, tolerance = 1e-6, iterations = 1000)

Spec_LU, LU_STD = MonteCarlo_LU(CPS, total_error, R3, R3_error, iterations = 1000)

Spec_SVD, SVD_STD = MonteCarlo_SVD(CPS, total_error, R3, R3_error, iterations = 1000)

# 5. Output Results
# We use \u00B1 for the plus-minus sign to avoid encoding errors
print(f"Difference (GRAVEL - MLEM): {sum(abs(X_GRAVEL - X_MLEM))}")

methods = [
    ("GRAVEL", X_GRAVEL, G_STD),
    ("MLEM", X_MLEM, M_STD),
    ("LU", Spec_LU, LU_STD),
    ("SVD", Spec_SVD, SVD_STD)
]

for name, mean, std in methods:
    print(f"\n--- {name} Results (neutrons per m^2 per day) ---")
    # Scaling to daily flux by multiplying CPS by 86400 seconds
    print(f"Thermal (10^-10 to 10^-6): {mean[0]*86400:.4e} \u00B1 {std[0]*86400:.4e}")
    print(f"Intermediate (10^-6 to 0.1): {mean[1]*86400:.4e} \u00B1 {std[1]*86400:.4e}")
    print(f"Fast (0.1 to 11): {mean[2]*86400:.4e} \u00B1 {std[2]*86400:.4e}")