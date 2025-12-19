import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate

# =============================================================================
# 1. DATA LOADING
# =============================================================================
# Load the high-resolution simulation results
data2 = pd.read_csv("3NCD_HighResolutionData.csv", encoding='latin1')
df2 = pd.DataFrame(data2)

# =============================================================================
# 2. EXTRACTION & ARRAY CONVERSION
# =============================================================================
# We extract 6 series (SH1-SH5 are moderated PE, SH6 is the Bare detector)
# Range [2:70] selects the relevant energy rows
Energies = df2.iloc[2:70, 0].to_numpy(dtype=float)

def extract_series(col_idx, err_idx):
    """Helper to extract data and error columns as float arrays."""
    val = df2.iloc[2:70, col_idx].to_numpy(dtype=float)
    err = df2.iloc[2:70, err_idx].to_numpy(dtype=float)
    return val, err

SH1, SH1ERR = extract_series(3, 4)   # 1 inch Polyethylene
SH2, SH2ERR = extract_series(9, 10)  # 2 inch Polyethylene
SH3, SH3ERR = extract_series(15, 16) # 3 inch Polyethylene
SH4, SH4ERR = extract_series(21, 22) # 4 inch Polyethylene
SH5, SH5ERR = extract_series(27, 28) # 5 inch Polyethylene
SH6, SH6ERR = extract_series(33, 34) # Bare Detector

# =============================================================================
# 3. INTERPOLATION (CURVE FITTING)
# =============================================================================
# Cubic splines help visualize the response between discrete simulation points
interp_funcs = {}
for i, data in enumerate([SH1, SH2, SH3, SH4, SH5, SH6], 1):
    interp_funcs[f'SH{i}'] = interpolate.interp1d(Energies, data, kind='cubic')
    # UnivariateSpline provides additional smoothing if the data is 'noisy'
    interp_funcs[f'smoothSH{i}'] = interpolate.UnivariateSpline(Energies, data)

# =============================================================================
# 4. PLOTTING - NEUTRON RESPONSE FUNCTIONS
# =============================================================================
# We use a Logarithmic X-scale because neutron energy ranges are vast
plt.figure(figsize=(12, 8))

# Plotting each detector configuration with error bars
plt.errorbar(Energies, SH1, SH1ERR, marker='.', color='k', label="1 inch PE", linewidth=2)
plt.errorbar(Energies, SH2, SH2ERR, marker='.', color='r', label="2 inch PE", linewidth=2)
plt.errorbar(Energies, SH3, SH3ERR, marker='.', color='b', label="3 inch PE", linewidth=2)
plt.errorbar(Energies, SH4, SH4ERR, marker='.', color='g', label="4 inch PE", linewidth=2)
plt.errorbar(Energies, SH5, SH5ERR, marker='.', color='c', label="5 inch PE", linewidth=2)
plt.errorbar(Energies, SH6, SH6ERR, marker='.', color='#C71585', label="Bare", linewidth=2)

# Image of a neutron moderator efficiency curve for various thicknesses


# Formatting the plot
plt.xscale('log') # Essential for neutron spectra
plt.xlabel("Neutron Initial Energy (MeV, log scale)", fontsize=16)
plt.ylabel("Response (Efficiency Probability)", fontsize=16)
plt.title("GEANT4 3NCD Detector Response Comparison", fontsize=18)
plt.xticks(rotation=40, fontsize=12)
plt.yticks(fontsize=12)
plt.legend(loc='upper right', fontsize=12)
plt.grid(True, which="both", ls="-", alpha=0.2)
plt.tight_layout()

# Display results
plt.show()