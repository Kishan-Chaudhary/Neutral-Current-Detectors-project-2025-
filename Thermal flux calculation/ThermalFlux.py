"""
Compute thermal neutron flux through a 3-NCD detector system.
All constants in cm, seconds, counts unless specified.
Author: Kishan Chaudhary
"""

import math

# --------------------------------------------------------------------
# Constants
# --------------------------------------------------------------------
OUTER_RADIUS = 2.5791                     # cm
LENGTH_NCD = 205                          # cm
DEAD_LEN_SIDE = 2.65                      # cm (each end)
DEAD_LEN_TOTAL = 2 * DEAD_LEN_SIDE

# Effective thermal efficiency from GEANT4 (already Maxwell-Boltzmann weighted)
EFF_3NCD = 6388.7217                      # cm² 
EFF_3NCD_ERR = 42.225                     # cm² (uncertainty)

EFF_1NCD = EFF_3NCD / 3
EFF_1NCD_ERR = EFF_3NCD_ERR / 3

EFF_2NCD = EFF_3NCD * (2/3)
EFF_2NCD_ERR = EFF_3NCD_ERR * (2/3)

# Correction factor for 600–900 keV cut
CORR_600_900 = 0.787
CORR_600_900_ERR = 0.02

# Dead-length correction
alive_fraction = (LENGTH_NCD - DEAD_LEN_TOTAL) / LENGTH_NCD

EFF_3NCD_NET = EFF_3NCD * alive_fraction
EFF_3NCD_NET_ERR = EFF_3NCD_ERR * alive_fraction

EFF_2NCD_NET = EFF_2NCD * alive_fraction
EFF_2NCD_NET_ERR = EFF_2NCD_ERR * alive_fraction

print(f"Net effective 3NCD efficiency factor: {EFF_3NCD_NET:.2f} ± {EFF_3NCD_NET_ERR:.2f} cm²")


# --------------------------------------------------------------------
# Count correction
# --------------------------------------------------------------------
def CorrectedCounts(total_counts, correction, correction_err):
    """
    Apply event-selection correction and propagate both statistical and
    systematic uncertainty.
    """

    # Raw statistics
    stat = math.sqrt(total_counts)

    # Apply correction
    corr_counts = total_counts / correction

    # Propagate uncertainties:
    corr_stat = stat / correction
    corr_sys = corr_counts * (correction_err / correction)

    return corr_counts, corr_stat, corr_sys


# --------------------------------------------------------------------
# Fluence and flux calculators
# --------------------------------------------------------------------
def thermal_neutron_fluence(counts_corr, stat_corr, sys_corr, response, response_err):
    """
    Fluence = corrected counts / effective efficiency.
    Propagates:
      - statistical uncertainty from corrected counts + response
      - systematic uncertainty from count correction
    """

    fluence = counts_corr / response

    # Stat: contributions from (counts_corrected) and (response)
    stat_rel = (stat_corr / counts_corr)**2 + (response_err / response)**2
    fluence_stat = fluence * math.sqrt(stat_rel)

    # Sys: only from count-correction systematic
    fluence_sys = fluence * (sys_corr / counts_corr)

    return fluence, fluence_stat, fluence_sys


def thermal_neutron_flux(fluence, time, stat_err, sys_err):
    """
    Flux = fluence / measurement_time.
    """

    flux = fluence / time
    flux_stat = stat_err / time
    flux_sys = sys_err / time

    return flux, flux_stat, flux_sys


# --------------------------------------------------------------------
# Weighted average of two fluxes (stat independent, sys fully correlated)
# --------------------------------------------------------------------
def weighted_average_max_sys(v1, s1, sy1, v2, s2, sy2):
    """
    Weighted average of two measurements where:
     - statistical uncertainties are independent
     - systematic uncertainties are fully correlated → take max
    """
    w1 = 1 / s1**2
    w2 = 1 / s2**2

    avg = (v1*w1 + v2*w2) / (w1 + w2)
    stat_avg = math.sqrt(1 / (w1 + w2))
    sys_avg = max(sy1, sy2)

    return avg, stat_avg, sys_avg


# --------------------------------------------------------------------
# Print helpers
# --------------------------------------------------------------------
def PrintFlux_in_sqm_day_units(flux, stat, sys):
    CONV = 86400 * 1e4     # (cm^-2 s^-1) → (m^-2 day^-1)
    print(f"Thermal flux (m⁻² day⁻¹): {flux*CONV:.5e} ± {stat*CONV:.5e} (stat) ± {sys*CONV:.5e} (sys)")


def PrintFlux_in_sqcm_second_units(flux, stat, sys):
    print(f"Thermal flux (cm⁻² s⁻¹): {flux:.4e} ± {stat:.4e} (stat) ± {sys:.4e} (sys)")


# --------------------------------------------------------------------
# Measurement Inputs
# --------------------------------------------------------------------
N_CAPTURES_3NCD = 31353
T_MEAS_3NCD = 1463278

N_CAPTURES_2NCD = 21572
T_MEAS_2NCD = 1466904

# Correct counts
C3, C3s, C3sys = CorrectedCounts(N_CAPTURES_3NCD, CORR_600_900, CORR_600_900_ERR)
C2, C2s, C2sys = CorrectedCounts(N_CAPTURES_2NCD, CORR_600_900, CORR_600_900_ERR)

# Fluence
F3, F3s, F3sys = thermal_neutron_fluence(C3, C3s, C3sys, EFF_3NCD_NET, EFF_3NCD_NET_ERR)
F2, F2s, F2sys = thermal_neutron_fluence(C2, C2s, C2sys, EFF_2NCD_NET, EFF_2NCD_NET_ERR)

# Flux
flux3, flux3s, flux3sys = thermal_neutron_flux(F3, T_MEAS_3NCD, F3s, F3sys)
flux2, flux2s, flux2sys = thermal_neutron_flux(F2, T_MEAS_2NCD, F2s, F2sys)

# Average (optional)
# flux_avg, flux_avg_stat, flux_avg_sys = weighted_average_max_sys(
#     flux3, flux3s, flux3sys,
#     flux2, flux2s, flux2sys
# )

# --------------------------------------------------------------------
# Output
# --------------------------------------------------------------------
print("\n--- Thermal Flux from 3NCD Measurement ---")
PrintFlux_in_sqcm_second_units(flux3, flux3s, flux3sys)
PrintFlux_in_sqm_day_units(flux3, flux3s, flux3sys)
