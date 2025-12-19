# Neutron Detection & Spectrum Unfolding (NCD)

This repository provides a complete toolchain for characterizing neutron environments using **Neutral Current Detectors (NCDs)**. It bridges the gap between raw **GEANT4** simulations and the final mathematical **Python** analysis required to solve the neutron energy spectrum.

---

## Scientific Context

In neutron physics, detectors often cannot measure a particle's energy directly. Instead, we use a series of detectors wrapped in different thicknesses of moderator (Polyethylene). This creates a "system of equations" where we know the captured counts ($C$) and the detector's sensitivity ($R$), but we must solve for the unknown neutron energy distribution ($\Phi$).

$$C = R \cdot \Phi$$



This repository provides the tools to simulate $R$ for custom geometries and solve for $\Phi$ using robust numerical methods.

---

## 📁 Repository Structure

### 📡 [GEANT4_NCD](./GEANT4_NCD)
This directory contains the C++ source code for Monte Carlo simulations.
* **Response Generation**: Includes the code used to generate the results shown in the project presentations.
* **NCD_General**: A highly modular simulation environment. Users can edit "NCDGeometry.hh" and "DetectorConstruction.cc" to simulate complex geometries, such as **layered HDPE and BHDPE moderators** with varying thicknesses.


### [NCD_PYTHON](./NCD_PYTHON)
The Python suite handles the data processing and mathematical "unfolding" (de-convolution). The scripts were repurposed from Tyler Dolezal's unfolding script(https://github.com/tylerdolezal/Neutron-Unfolding).
* **Solvers**: Implementations of **MLEM** (Maximum Likelihood Expectation Maximization), **GRAVEL**, **LU Decomposition**, and **SVD**.
* **Monte Carlo Analysis**: Tools to propagate experimental and systematic errors through the unfolding process to provide reliable error bars.
* **Data Rebinning**: Scripts to convert high-resolution simulation outputs into 3x3 or nxn matrices.

### [NCD_Literature](./NCD_Literature)
A collection of relevant scientific papers. Beginners are encouraged to review these to understand:
* The physics of **Helium-3 capture cross-sections**.
* The behavior of **moderators** (thermalization).
* The mathematical theory behind **iterative unfolding algorithms**.


---

## 🚀 Beginner's Guide to the Workflow

1.  **Define Geometry**: Open the GEANT4 `NCD_General` project. Use the `NCD_Geometry.hh` flie in include to set your moderator thicknesses (e.g., 1-inch, 2-inch, or layered Borated High Density Polyethylene and High Density Polyethylene).
2.  **Run Simulations**: Execute macros across a range of neutron energies (from $10^{-10}$ to $11$ MeV) to determine how many neutrons your specific design captures.
3.  **Process Output**: Use the Python scripts to extract data from `.root` files and rebin them into standard energy groups (Thermal, Epithermal, and Fast).

4.  **Unfold**: Input your experimental lab counts into the Unfolding scripts to determine the flux of the environment you are measuring.

---

## Prerequisites

* **GEANT4** (v11.0+ recommended)
* **Python 3.10+**
* **Required Libraries**: `uproot`, `numpy`, `pandas`, `scipy`, `matplotlib`, `seaborn`

---

## Troubleshooting & Notes
* **Encoding Errors**: If running on Windows, ensure your Python scripts use `utf-8` encoding (especially when using the $\pm$ symbol in plots).
* **Matrix Stability**: If your unfolding results show negative values, try using the **MLEM** or **GRAVEL** iterative algorithms rather than direct LU decomposition. The LU is mathematically reliable, however its limited for determined systems and linear independence of the repsponse matrix.
