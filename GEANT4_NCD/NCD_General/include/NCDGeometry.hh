#ifndef NCDGeometry_H
#define NCDGeometry_H 1

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh" // Needed for universe_mean_density

// =========================================================================
// GEOMETRY CONSTANTS RELEVANT TO THE NCD (NEUTRON CAPTURE DETECTOR)
// =========================================================================

// --- NCD Tube and Nickel Wall Constants ---
static const G4double He3NickelIR = 2.542 * cm;     // Inside radius of NCD's nickel walls
static const G4double He3NickelTh = 0.0371 * cm;    // Thickness of the nickel walls
static const G4double He3NickelOR = He3NickelIR + He3NickelTh; // Outer radius of the nickel tube
static const G4double He3NickelEndCapL = 4.0 * cm;       // Length of the nickel end cap
static const G4double He3TubeL205cm = 205.0 * cm;     // Length of the NCD (Neutral Current Detector) tube

// --- Steel Cap Dimensions (Face and Volume) ---
static const G4double frontSteelCapLength = 2.69 * 2.54 * cm; // Length of the front steel cap
static const G4double backSteelCapLength = 1.12 * 2.54 * cm; // Length of the back steel cap
static const G4double frontSteelCapSideThickness = 0.316 * cm;   // Side thickness of the front steel cap
static const G4double backSteelCapSideThickness = 0.1458 * cm;  // Side thickness of the back steel cap
static const G4double steelCapFaceThickness = 0.125 * 2.54 * cm; // Thickness of the steel cap face (1/8")
static const G4double steelCapInsertDistance = He3NickelEndCapL / 2.0; // Distance the steel caps are inserted into the tube
static const G4double He3VolumeMiddlePointOffset = (backSteelCapLength - frontSteelCapLength) / 2.0; // Offset of the middle point of the He-3 volume
static const G4double He3AirContainerAdditionalLength = frontSteelCapLength + backSteelCapLength - 2.0 * steelCapInsertDistance; // Additional length due to steel caps

// --- Anode Wire Properties ---
static const G4double He3AnodeDiameter = 50.0e-6 * m;    // Diameter of the anode wire
static const G4double He3AnodeProtrusion = 4.0 * cm;       // Protrusion length of the anode wire

// --- NCD Dead Zone ---
static const G4double He3VolumeDeadL = 4.65 * cm; // Length of the dead zone in the detector

// --- Feedthrough Coupler Plate Dimensions ---
static const G4double feedthroughCouplerPlateInnerRadius = 0.3491 * cm;
static const G4double feedthroughCouplerPlateThickness = 0.0980 * cm;

// =========================================================================
// MATERIAL AND COMPONENT CONSTANTS
// =========================================================================

// --- Counter Gas Parameters (He3 + CF4) ---
static const G4double He3MolarMass = 3.016 * g / mole;       // Molar mass of He-3
static const G4double DENSITY_CF4 = 3.72 * mg / cm3; // Placeholder density     // CF4 gas density (original)
static const G4double DENSITY_HE3GAS = 0.00163  * g / cm3;      // He-3 gas density (original)
static const G4double TEMP_HE3GAS = 293.88 * kelvin; // Temperature of the counter gas (original)
static const G4double PRESSURE_HE3GAS = 2.50 * atmosphere; // Pressure of the counter gas (original)
static const G4double CounterGasDensity = 0.00163 * g / cm3; // Density of the counter gas (original)
static const G4double PERCENT_HE3 = 85 * perCent; // Mass fraction of CF4 (original)
static const G4double PERCENT_CF4 = 15 * perCent; // Mass fraction of He-3 (original)

// --- Hardcoded Dimensions from DetectorConstruction::Construct() ---

// World/Global Dimensions
static const G4double WORLD_OUTER_RADIUS = 19. * cm;
static const G4double WORLD_HALF_LENGTH = 113.5 * cm;

// Polyethylene Castle 
static const G4double ATOMIC_MASS_TSH = 1.0079 * g / mole; // Atomic mass of Hydrogen in Polyethylene Shield
static const G4double ATOMIC_MASS_B10 = 10.0129 * g / mole; // Atomic mass of Boron-10 in Borated Polyethylene Shield
static const G4double ATOMIC_MASS_B11 = 11.0093 * g / mole; // Atomic mass of Boron-11 in Borated Polyethylene Shield
static const G4double PERCENT_B10 = 19.9 * perCent; // Natural abundance of Boron-10 in Boron
static const G4double PERCENT_B11 = 80.1 * perCent; // Natural abundance of Boron-11 in Boron
static const G4bool POLYBOOL = true; //Flag to enable Polyethylene shield around NCD array
static const G4String POLY_SHIELD_MATERIAL = "BoratedHDPE"; //Material of the Polyethylene shield around NCD array(options BoratedHDPE or PolyEthylene), ignored if USE_LAYERED_SHIELD is "true".
static const G4String USE_LAYERED_SHIELD = "true";   //Flag to enable the layered shield (Polyethylene core + Borated HDPE shell), If "true", POLY_SHIELD_MATERIAL is ignored.
static const G4double INNER_POLY_THICKNESS = 1.27 * cm;    // Thickness of the inner pure Polyethylene layer (PE)
static const G4double THICKNESS_BORATED_POLY = 1.27 * cm;  // Thickness of the outer Borated Polyethylene layer (BHDPE)
static const G4double POLY_BASE_OFFSET = 3. * cm;
static const G4double POLY_HEIGHT_OFFSET = 3. * cm;
static const G4double POLY_WALL_THICKNESS = 2.54 * cm; // 1 inch
static const G4double NEUTRON_SCORER_OFFSET = 0.1 * mm;

// Material Properties (from hardcoded values)
static const G4double DENSITY_NICKEL = 8.90 * g / cm3;
static const G4double DENSITY_STEEL = 8.00 * g / cm3;
static const G4double DENSITY_AIR = 1.29 * mg / cm3;
static const G4double TEMP_AIR = 293.15 * kelvin;
static const G4double PRESSURE_AIR = 1.5 * atmosphere;
static const G4double DENSITY_H2O = 1.0 * g / cm3;
static const G4double DENSITY_POLYETHYLENE = 0.948 * g / cm3;
static const G4double DENSITY_BORATED_HDPE = 1.08 * g / cm3;

// Material Fractions (Simplified for Geant4 Materials in Construct())
static const G4double FRAC_STEEL_FE = 0.74;
static const G4double FRAC_STEEL_CR = 0.18;
static const G4double FRAC_STEEL_NI = 0.08;
static const G4double FRAC_AIR_N = 0.79;
static const G4double FRAC_AIR_O = 0.21;
static const G4double FRAC_BHDPE_C = 0.81327;
static const G4double FRAC_BHDPE_B = 0.05;
static const G4double FRAC_BHDPE_H = 0.13673;

// Vacuum Properties
static const G4double VACUUM_DENSITY_MOLAR = 1.01 * g / mole;
static const G4double VACUUM_TEMP = 2.73 * kelvin;
static const G4double VACUUM_PRESSURE = 3.e-18 * pascal;


// --- Component Counts (G4int for clarity, though G4int is just an int) ---
static const G4int N_ELEM_CF4 = 2;
static const G4int N_COMP_HE3GAS = 2;
static const G4int N_ELEM_STEEL = 3;
static const G4int N_ELEM_AIR = 2;
static const G4int N_ELEM_H2O = 2;
static const G4int N_ELEM_POLYETHYLENE = 2;
static const G4int N_ELEM_BORATED_HDPE = 3;


#endif