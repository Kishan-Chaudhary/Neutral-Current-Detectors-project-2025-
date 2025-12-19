import uproot
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


file = uproot.open("NCD_Poly_AmBeSpectra.root")

tree = file["Primary Neutron spectra"]


df = tree.arrays(library="pd")


custom_bins = [10e-10, 10e-6, 0.1, 2, 4, 7, 11]
plott = [10e-8,10e-2, 1, 3, 5.5, 9.5]
energies_29 = [0, 1e-10, 1e-9, 1e-8, 1e-7, 1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]


# Get counts and bin edges
counts, bin_edges = np.histogram(df["Primary_Neutron_Spectra"], bins=energies_29)

unfolding_counts= [1.28535472e2,1.92249734e-5, 1.96980167e6, 1.66079906e5,7.80832377e6, 9.30381907e2 ]
print(unfolding_counts)
# Print counts for each bin
for i in range(len(counts)):
    print({counts[i]})

#plt.plot(plott,unfolding_counts, marker='.', color="red", label="ip 1 inch PE")
plt.hist(df["Primary_Neutron_Spectra"], bins=bin_edges, color="skyblue", edgecolor="black")
plt.xlabel("Energy (MeV)", fontsize=22)
plt.ylabel("Counts (log scale)",fontsize=22)
#plt.yscale('log')
plt.grid(True)
plt.show()



bin_labels = [
    "10^(-10)-10^(-6)",
    "10^(-6)-0.1",
    "0.1-2",
    "2-4",
    "4-7",
    "7-11"
]


# Your bin edges
custom_bins = [1e-10, 1e-6, 0.1, 2.0, 4.0, 7.0, 11.0]

# Histogram calculation (assumes df["Primary_Neutron_Spectra"] exists)
counts1, bin_edges1 = np.histogram(df["Primary_Neutron_Spectra"], bins=custom_bins)

# Calculate bin centers for plotting
bin_centers = 0.5 * (np.array(custom_bins[:-1]) + np.array(custom_bins[1:]))


for i in range (1,6):
    print(f"Bin{i+1}: Count = {counts1[i]}")

# Now plot
plt.bar(bin_centers, counts1, width=np.diff(custom_bins), align='center', edgecolor='black')

plt.xscale("log")
plt.yscale("log")
plt.xlabel("Neutron Energy (MeV)")
plt.ylabel("Neutron Counts")
plt.title("AmBe Spectrum Binned into 6 Regions")
plt.grid(True, which='both', ls='--')

# Optional: Label x-axis
plt.xticks(bin_centers, bin_labels, rotation=45)

plt.tight_layout()
plt.show()