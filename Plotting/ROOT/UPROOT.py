import uproot
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# =============================================================================
# 1. FILE LOADING & DATA EXTRACTION
# =============================================================================

# Open ROOT files containing simulation results
# file: Energy deposition in the detector
# file2 & file3: Neutron energy spectra at different stages/locations
file  = uproot.open("NCD_Poly_AmBeTest_t0.root")
file2 = uproot.open("NCD_Poly_AmBeSpectra.root")
file3 = uproot.open("NeutronFluence_145cm_NoTS_0%Water_POLY.root")

# Extract specific TTrees (data tables) from the files
tree_dep = file["EnergyDeposited"]
tree_spec = file2["Primary Neutron spectra"]
tree_fluence = file3["Total Neutron Spectra"]

# Convert ROOT trees into Pandas DataFrames for high-level manipulation
df_dep = tree_dep.arrays(library="pd")
df_spec = tree_spec.arrays(library="pd")
df_fluence = tree_fluence.arrays(library="pd")

# =============================================================================
# 2. DETECTOR ENERGY DEPOSITION ANALYSIS
# =============================================================================

# Filter and count entries within the main He-3 peak (approx 0.18 to 0.765 MeV)
# This represents valid neutron capture events
count_peak = df_dep[(df_dep["Total_Energy_Deposited"] > 0.18) & 
                    (df_dep["Total_Energy_Deposited"] < 0.765)].shape[0]

print(f"Neutron Capture Peak Entries (0.18 - 0.765 MeV): {count_peak}")

# =============================================================================
# 3. NEUTRON SPECTRUM CATEGORIZATION
# =============================================================================

# Categorize neutrons into physical energy groups (Units in MeV)
thermal_mask     = (df_fluence["Total_Neutron_Spectra"] > 1e-12) & (df_fluence["Total_Neutron_Spectra"] < 1e-6)
epithermal_mask  = (df_fluence["Total_Neutron_Spectra"] > 1e-6)  & (df_fluence["Total_Neutron_Spectra"] < 0.1)
fast_mask        = (df_fluence["Total_Neutron_Spectra"] > 0.1)   & (df_fluence["Total_Neutron_Spectra"] < 11.0)

# Calculate counts for each category
count_thermal    = df_fluence[thermal_mask].shape[0]
count_epithermal = df_fluence[epithermal_mask].shape[0]
count_fast       = df_fluence[fast_mask].shape[0]

total_counts = count_thermal + count_epithermal + count_fast

# Print the relative fractions (Useful for understanding moderation efficiency)
if total_counts > 0:
    print(f"Thermal Fraction:    {count_thermal/total_counts:.4f}")
    print(f"Epithermal Fraction: {count_epithermal/total_counts:.4f}")
    print(f"Fast Fraction:       {count_fast/total_counts:.4f}")

# =============================================================================
# 4. SPECTRAL PLOTTING (LOG-LINEAR HISTOGRAM)
# =============================================================================

# Generate hybrid bins: Logarithmic for low energies, Linear for high energies
# This allows detail in the thermal peak while maintaining resolution in fast regions
log_bins   = np.logspace(-12, -1, num=12, base=10)
linear_bins = np.arange(0.1, 11.1, 0.1)
energy_bins = np.unique(np.concatenate([log_bins, linear_bins]))

plt.figure(figsize=(10, 6))

# Plotting the neutron fluence spectrum
plt.hist(df_fluence["Total_Neutron_Spectra"], 
         bins=energy_bins, 
         histtype='step',  # 'step' is standard for flux spectra
         linewidth=1.5, 
         label="Total Neutron Spectrum")

# Logarithmic X-axis is essential for neutron energy ranges
plt.xscale('log')

plt.xlabel("Neutron Energy (MeV)", fontsize=14)
plt.ylabel("Counts", fontsize=14)
plt.title("Neutron Fluence Energy Spectrum", fontsize=16)
plt.grid(True, which='both', ls='--', alpha=0.4)
plt.tight_layout()
plt.show()