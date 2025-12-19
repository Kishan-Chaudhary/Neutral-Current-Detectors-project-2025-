import pandas as pd
import numpy as np

"""
This script  designed to rebin a high-resolution neutron detector response matrix from the GEANT4 simulation
The binning is based on weighted averages to ensure that the integral properties of the response are preserved.
The script also includes a specialized calculation for the thermal neutron bin, using a Maxwell-Boltzmann distribution to weight the response appropriately.
The cript processes data from a CSV file, performs the rebinning, and exports the reduced response matrix for use in neutron spectrum unfolding applications using the execute.py script.
Author: Kishan Chaudhary, aknowledge use of CHATGPT for code optimization and comments.
"""
# =============================================================================
# 1. LOAD EXPERIMENTAL/SIMULATED DATA
# =============================================================================
# Load the high-resolution response matrix (e.g., from Geant4/MCNP)
df2 = pd.read_csv("3NCD_Response_32_Nibi.csv", encoding='latin1')

# =============================================================================
# 2. PROCESSING FUNCTIONS
# =============================================================================

def bin_average_response(E, R, error, bins):
    """
    Groups high-res data into broad energy bins using a dE-weighted average.
    Ensures the 'area' of the response is preserved across the new bins.
    """
    E, R, error, bins = map(np.asarray, (E, R, error, bins))

    # Calculate the energy width (dE) for each point
    dE = np.empty_like(E)
    dE[0] = (E[1] - E[0]) / 2
    dE[1:-1] = (E[2:] - E[:-2]) / 2
    dE[-1] = (E[-1] - E[-2]) / 2

    # Integrate response over energy and divide by bin width
    num, _ = np.histogram(E, bins=bins, weights=R * dE)
    den, _ = np.histogram(E, bins=bins, weights=dE)
    R_bin = num / den

    # Propagate uncertainties
    w2_sigma2, _ = np.histogram(E, bins=bins, weights=(dE**2) * (error**2))
    R_err = np.sqrt(w2_sigma2) / den

    return R_bin, R_err


def thermal_weighted_efficiency(E, R, error, bins, T=293.15):
    """
    Specially calculates the thermal bin efficiency by weighting it with
    a Maxwell-Boltzmann distribution at temperature T.
    """
    E, R, error, bins = map(np.asarray, (E, R, error, bins))

    dE = np.empty_like(E)
    dE[0] = (E[1] - E[0]) / 2
    dE[1:-1] = (E[2:] - E[:-2]) / 2
    dE[-1] = (E[-1] - E[-2]) / 2

    # Maxwell-Boltzmann weighting factor
    kT = 8.617333262e-11 * T # Boltzmann constant in MeV/K
    mb = np.sqrt(E) * np.exp(-E / kT)
    w = mb * dE

    # Weighted histogram integration
    num, _ = np.histogram(E, bins=bins, weights=R * w)
    den, _ = np.histogram(E, bins=bins, weights=w)
    R_bin = num / den

    # Propagate uncertainties
    w2_sigma2, _ = np.histogram(E, bins=bins, weights=(w**2) * (error**2))
    R_err = np.sqrt(w2_sigma2) / den

    return R_bin, R_err

# =============================================================================
# 3. DATA EXTRACTION
# =============================================================================
# Extract Energies (Row 2 onwards, Column 0)
EnergiesNew = df2.iloc[2:, 0].astype(float).to_numpy()

# Column indices for Response (R) and their Errors (Err)
R_cols  = [3, 10, 17, 24]
Err_cols = [4, 11, 18, 25]

# Loop through columns and extract numpy arrays
R = [df2.iloc[2:, c].astype(float).to_numpy() for c in R_cols]
R_err = [df2.iloc[2:, c].astype(float).to_numpy() for c in Err_cols]

R1, R2, R3, R4 = R
R1_err, R2_err, R3_err, R4_err = R_err

# =============================================================================
# 4. REBINNING EXECUTION
# =============================================================================
# Define the 3 major energy bins (MeV): Thermal, Intermediate, Fast
bins = [0, 1e-6, 0.1, 11]

# Rebin all detector responses
R_bin1, R_err1 = bin_average_response(EnergiesNew, R1, R1_err, bins)
R_bin2, R_err2 = bin_average_response(EnergiesNew, R2, R2_err, bins)
R_bin3, R_err3 = bin_average_response(EnergiesNew, R3, R3_err, bins)
R_bin4, R_err4 = bin_average_response(EnergiesNew, R4, R4_err, bins)

# Calculate a physically accurate thermal efficiency for the R4 detector
R_thermal, R_thermal_err = thermal_weighted_efficiency(EnergiesNew, R4, R4_err, bins)
print(f"Maxwell-Boltzmann Weighted Thermal Efficiency: {R_thermal[0]:.4e} +/- {R_thermal_err[0]:.4e}")

# =============================================================================
# 5. EXPORT REDUCED MATRIX
# =============================================================================
# Combine processed columns into 3x3 matrices
R_rebinned       = np.column_stack([R_bin1, R_bin2, R_bin4])
R_rebinned_error = np.column_stack([R_err1, R_err2, R_err4])

# Save as CSV for use in unfolding code
np.savetxt("Rebinned_HGResponseMatrix_3x3.csv", R_rebinned, delimiter=",", fmt="%.8e")
np.savetxt("Rebinned_HGResponseMatrix_3x3_Error.csv", R_rebinned_error, delimiter=",", fmt="%.8e")

print("\nRebinned matrices successfully saved.")