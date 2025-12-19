#ifndef NCDGeometry_H
#define NCDGeometry_H 1

#include "G4SystemOfUnits.hh"


// Geometry constants relevant to the NCD (Neutron Capture Detector)

// He-3 Tube and Nickel Wall Constants
static const G4double He3NickelIR        = 2.542 * cm;    // Inside radius of NCD's nickel walls
static const G4double He3NickelTh        = 0.0371 * cm;  // Thickness of the nickel walls
static const G4double He3NickelOR        = He3NickelIR + He3NickelTh;  // Outer radius of the nickel tube
static const G4double He3NickelEndCapL   = 4.0 * cm;      // Length of the nickel end cap

// Physical length of the NCD tube for 205 cm
static const G4double He3TubeL205cm   = 205.0 * cm;  // Length of the NCD (Neutral Current Detector) tube

// Anode wire properties (not used in this example but might be useful for future reference)
static const G4double He3AnodeDiameter   = 50.0e-6 *  m ;  // Diameter of the anode wire
static const G4double He3AnodeProtrusion = 4.0 * cm ;       // Protrusion length of the anode wire

// Counter gas parameters
static const G4double CF4Density = 1.355e-03*g/cm3;  // CF4 gas density
static const G4double He3MolarMass = 3.016*g/mole;    // Molar mass of He-3
static const G4double He3Density = 2.631e-04*g/cm3;   // He-3 gas density
static const G4double CounterGasTemperature = 296.88*kelvin;  // Temperature of the counter gas
static const G4double CounterGasPressure = 2.50*atmosphere;  // Pressure of the counter gas
static const G4double CounterGasDensity = 1.618e-03*g/cm3;   // Density of the counter gas
static const G4double CounterGasCF4MassFraction = 83.74*perCent;  // Mass fraction of CF4 in the counter gas
static const G4double CounterGasHe3MassFraction = 16.26*perCent;  // Mass fraction of He-3 in the counter gas

// Dead zone length, for the full NCD radius cylinder
static const G4double He3VolumeDeadL = 4.65*cm;  // Length of the dead zone in the detector

// Feedthrough coupler plate dimensions
static const G4double feedthroughCouplerPlateInnerRadius = 0.3491*cm;  // Inner radius of the feedthrough coupler plate
static const G4double feedthroughCouplerPlateThickness = 0.0980*cm;    // Thickness of the feedthrough coupler plate

// Front and back end cap dimensions (adjusted for known weights and density)
static const G4double frontSteelCapLength = 2.69*2.54*cm;   // Length of the front steel cap
static const G4double backSteelCapLength = 1.12*2.54*cm;    // Length of the back steel cap
static const G4double frontSteelCapSideThickness = 0.316*cm;  // Side thickness of the front steel cap
static const G4double backSteelCapSideThickness = 0.1458*cm; // Side thickness of the back steel cap
static const G4double steelCapFaceThickness = 0.125*2.54*cm; // Thickness of the steel cap face (1/8")

// Insert distance and middle point offset for the NCD
static const G4double steelCapInsertDistance = He3NickelEndCapL/2.0;  // Distance the steel caps are inserted into the tube
static const G4double He3AirContainerAdditionalLength = frontSteelCapLength + backSteelCapLength - 2.0*steelCapInsertDistance;  // Additional length due to steel caps
static const G4double He3VolumeMiddlePointOffset = (backSteelCapLength - frontSteelCapLength)/2.0;  // Offset of the middle point of the He-3 volume

#endif
