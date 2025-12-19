#include "G4NistManager.hh"
#include "DetectorConstruction.hh"
#include "G4Tubs.hh"
#include "NCDGeometry.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"         // Include SD Manager
#include "G4Element.hh"
#include "G4Material.hh"
#include "CADMesh.hh"
#include "G4PhysicalConstants.hh"

// Constructor
DetectorConstruction::DetectorConstruction()
    : fHe3TubeL(He3TubeL205cm),             // Length of the He3 Tube
    fHe3TubeRadius(He3NickelIR),          // Inside radius of the Nickel wall
    fHe3TubeThickness(He3NickelTh),       // Thickness of the Nickel wall
    fSteelCapThickness(steelCapFaceThickness), // Thickness of the Steel Cap Face
    fHe3VolumeMiddlePointOffset(He3VolumeMiddlePointOffset),  // Offset for He3 Volume's Middle Point
    fHe3AnodeDiameter(He3AnodeDiameter),
    fHe3AnodeProtrustion(He3AnodeProtrusion),
    lNickelTube(nullptr),
    lHe3CuTube(nullptr),
    lHe3GasTube(nullptr),
    lBackSteelCap(nullptr),
    lBackSteelCapFace(nullptr),
    lFrontSteelCap(nullptr),
    lFrontSteelCapFace(nullptr),
    lAnodeWire(nullptr),
   /* lNickelTube2(nullptr),
    lHe3CuTube2(nullptr),
    lHe3GasTube2(nullptr),
    lBackSteelCap2(nullptr),
    lFrontSteelCap2(nullptr),
    lAnodeWire2(nullptr),*/
    worldMaterial(nullptr)
{
   
}

// Destructor
DetectorConstruction::~DetectorConstruction() {
    // Destructor implementation
}

// Construct the detector's physical volume
G4VPhysicalVolume* DetectorConstruction::Construct() {
    // Define materials
    G4double atomicMass = 3.016 * g / mole;
    G4Isotope* he3 = new G4Isotope("he3", 2, 3, atomicMass);
    G4Element* He3 = new G4Element("He3", "He3", 1);
    He3->AddIsotope(he3, 100. * perCent);

    G4Element* elC = G4NistManager::Instance()->FindOrBuildElement("C");
    G4Element* elF = G4NistManager::Instance()->FindOrBuildElement("F");

    // CF4 molecule
    G4Material* CF4 = new G4Material("CF4", 3.72 * mg / cm3, 2); // Density placeholder
    CF4->AddElement(elC, 1);
    CF4->AddElement(elF, 4);


    /*G4Element* He = new G4Element("Helium", "He", 2., 4.002602 * g / mole);*/
    G4Material* He3Gas = new G4Material("He3", 0.00163 * g / cm3, 2, kStateGas, 293 * kelvin, 2.5 * atmosphere); //original density 0.0001786 new G4Material("matXenon1",density1, ncomponents=1, // kStateGas,temperature1, pressure1);
    He3Gas->AddElement(He3, 85. * perCent);
    He3Gas->AddMaterial(CF4, 15. * perCent);



    G4Element* Ni = new G4Element("Nickel", "Ni", 28., 58.6934 * g / mole);
    G4Material* nickelMaterial = new G4Material("Nickel", 8.90 * g / cm3, 1);
    nickelMaterial->AddElement(Ni, 1);

    G4Element* Fe = new G4Element("Iron", "Fe", 26., 55.845 * g / mole);
    G4Element* Cr = new G4Element("Chromium", "Cr", 24., 52.00 * g / mole);
    G4Material* stainlessSteelMaterial = new G4Material("G4_STAINLESS_STEEL", 8.00 * g / cm3, 3);
    stainlessSteelMaterial->AddElement(Fe, 0.74);
    stainlessSteelMaterial->AddElement(Cr, 0.18);
    stainlessSteelMaterial->AddElement(Ni, 0.08);

    G4Element* N = new G4Element("Nitrogen", "N", 7., 14.01 * g / mole);
    G4Element* O = new G4Element("Oxygen", "O", 8., 16.00 * g / mole);
    G4double density = 1.29 * mg / cm3;
    G4int ncomponents = 2;
    G4Material* airMaterial = new G4Material("Air", 0.01 * mg/cm3, ncomponents, kStateGas, 293.15 * kelvin, 0.01 * atmosphere);
    airMaterial->AddElement(N, 0.79);
    airMaterial->AddElement(O, 0.21);

    // New elements from isotopes 
    G4Isotope* H11 = new G4Isotope("Protium", 1, 1, 1.007825 * g / mole);
    G4Isotope* H22 = new G4Isotope("Deuterium", 1, 2, 2.014 * g / mole);
    G4Element* Hnew = new G4Element("Hydrogen", "Hnew", 2);
    Hnew->AddIsotope(H11, 99.985 * perCent);
    Hnew->AddIsotope(H22, 0.015 * perCent);

    //Custom Hydrogen for thermal scattering model
    G4Element* elTSH = new G4Element( "TS_H_of_Polyethylene" ,"H_POLYETHYLENE" , 1.0 , 1.0079*g/mole );

    // H2O
    G4Material* H2O = new G4Material("H2O", 1.0 * g / cm3, 2);
    H2O->AddElement(Hnew, 2);
    H2O->AddElement(O, 1);

    G4Element* H = new G4Element("Hydrogen", "H", 1., 1.008 * g / mole);
    // --- Create isotopes for natural boron (B10 and B11) ---
    G4Isotope* isoB10 = new G4Isotope("B10", 5, 10, 10.012936 * g / mole);
    G4Isotope* isoB11 = new G4Isotope("B11", 5, 11, 11.0093054 * g / mole);

    // Create boron element with natural abundances (~19.9% B10, 80.1% B11)
    G4Element* elB = new G4Element("Boron", "B", 2);
    elB->AddIsotope(isoB10, 19.9 * perCent);
    elB->AddIsotope(isoB11, 80.1 * perCent);

    G4Material* PolyEthylene = new G4Material("PolyEthylene", 0.970 * g / cm3, 2);
    PolyEthylene->AddElement(elC, 2);
    PolyEthylene->AddElement(elTSH, 4);

    // G4NistManager* nist = G4NistManager::Instance();
    // G4Material* PolyEthylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4Material* boratedHDPe = new G4Material("Borated PolyEthylene", 0.979 * g / cm3, 3);
    boratedHDPe->AddElement(elC, 0.81327);
    boratedHDPe->AddElement(elB, 0.05);
    boratedHDPe->AddElement(H, 0.13673);


    // Vacuum
    auto vacuum = new G4Material("Galactic", 1., 1.01 * g / mole, universe_mean_density,
        kStateGas, 2.73 * kelvin, 3.e-18 * pascal);

    //Set World Material
    worldMaterial = vacuum; //airMaterial , vacumm
    // Define world size and radius
    G4double outerRadiusOfDetector = fHe3TubeRadius + fHe3TubeThickness;

    G4Tubs* solidWorld = new G4Tubs("WorldTube", 0, 19 * cm, 113.5 * cm, 0., 360. * deg); //default = 19cm radius, 2nd = 22 cm, 3rd = 25 cm
    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld, worldMaterial, "World");
    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);


	//Scorer volume - to count neutrons
     // Just bigger than NCD, only use when main neutron scorer is commented
    // G4Tubs* NeutronScorer = new G4Tubs("NeutronScorer", fHe3TubeRadius + fHe3TubeThickness, fHe3TubeRadius + fHe3TubeThickness + 1* mm, (fHe3TubeL / 2.)+ 1*mm , 0., 360. * deg);
    // G4LogicalVolume* neutronScorerlogic = new G4LogicalVolume(NeutronScorer, vacuum, "NeutronScorer");
    // new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, 0.), neutronScorerlogic, "NeutronScorer1", logicWorld, false, 0);
    //Shifting the setup
   
    // He3 Gas Tube (inner tube) - First NCD
    G4Tubs* solidHe3Tube1 = new G4Tubs("He3Tube1", 0, fHe3TubeRadius, fHe3TubeL / 2., 0., 360. * deg);
    lHe3CuTube = new G4LogicalVolume(solidHe3Tube1, He3Gas, "He3CuTube");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, 0.), lHe3CuTube, "He3CuTube", logicWorld, false, 0); // 

    // Nickel Tube (outer tube) - First NCD
    G4Tubs* solidNickelTube1 = new G4Tubs("NickelTube1", fHe3TubeRadius, fHe3TubeRadius + fHe3TubeThickness, fHe3TubeL / 2., 0., 360. * deg);
    lNickelTube = new G4LogicalVolume(solidNickelTube1, nickelMaterial, "NickelTube1");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, 0.), lNickelTube, "NickelTube1", logicWorld, false, 0);

    // Steel Caps (front and back) - First NCD
    G4Tubs* solidFrontSteelCap1 = new G4Tubs("FrontSteelCap1", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
    lFrontSteelCap = new G4LogicalVolume(solidFrontSteelCap1, stainlessSteelMaterial, "FrontSteelCap");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, -(fHe3TubeL / 2. + fSteelCapThickness / 2.)), lFrontSteelCap, "FrontSteelCap1", logicWorld, false, 0);

    G4Tubs* solidBackSteelCap1 = new G4Tubs("BackSteelCap1", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
    lBackSteelCap = new G4LogicalVolume(solidBackSteelCap1, stainlessSteelMaterial, "BackSteelCap");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, fHe3TubeL / 2. + fSteelCapThickness / 2.), lBackSteelCap, "BackSteelCap1", logicWorld, false, 0);

    // Anode Wire - First NCD
    G4double anodeWireLength = fHe3TubeL + 2.0 * (fHe3AnodeProtrustion - fSteelCapThickness);
    G4Tubs* solidAnodeWire1 = new G4Tubs("AnodeWire1", 0, fHe3AnodeDiameter / 2., anodeWireLength / 2., 0., 360. * deg);
    lAnodeWire = new G4LogicalVolume(solidAnodeWire1, stainlessSteelMaterial, "AnodeWire");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, 0.), lAnodeWire, "AnodeWire1", logicWorld, false, 0);
    

        // He3 Gas Tube (inner tube) - Second NCD (Placed in opposite corner)
/*G4Tubs* solidHe3Tube2 = new G4Tubs("He3Tube2", 0, fHe3TubeRadius, fHe3TubeL / 2., 0., 360. * deg);
lHe3CuTube2 = new G4LogicalVolume(solidHe3Tube, He3Gas, "He3CuTube2");*/
// Placing the second NCD in the opposite corner, using lHe3CuTube from previous NCD
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector , -outerRadiusOfDetector, 0.), lHe3CuTube, "He3CuTube2", logicWorld, false, 0);

    // Nickel Tube (outer tube) - Second NCD
    /*G4Tubs* solidNickelTube2 = new G4Tubs("NickelTube2", fHe3TubeRadius, fHe3TubeRadius + fHe3TubeThickness, fHe3TubeL / 2., 0., 360. * deg);
    lNickelTube2 = new G4LogicalVolume(solidNickelTube1, nickelMaterial, "NickelTube2");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, -outerRadiusOfDetector, 0.), lNickelTube, "NickelTube2", logicWorld, false, 0);

    // Steel Caps (front and back) - Second NCD
 /*   G4Tubs* solidFrontSteelCap2 = new G4Tubs("FrontSteelCap2", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
    lFrontSteelCap2 = new G4LogicalVolume(solidFrontSteelCap1, stainlessSteelMaterial, "FrontSteelCap");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, -outerRadiusOfDetector, -(fHe3TubeL / 2. + fSteelCapThickness / 2.)), lFrontSteelCap, "FrontSteelCap2", logicWorld, false, 0);

    /* G4Tubs* solidBackSteelCap2 = new G4Tubs("BackSteelCap2", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
     lBackSteelCap2 = new G4LogicalVolume(solidBackSteelCap1, stainlessSteelMaterial, "BackSteelCap");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, -outerRadiusOfDetector, fHe3TubeL / 2. + fSteelCapThickness / 2.), lBackSteelCap, "BackSteelCap2", logicWorld, false, 0);


    //Neutron Scorer for second NCD, only use when main neutron scorer is commented
    //new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, -outerRadiusOfDetector, 0.), neutronScorerlogic, "NeutronScorer2", logicWorld, false, 0);
    // Anode Wire - Second NCD
    /*G4Tubs* solidAnodeWire2 = new G4Tubs("AnodeWire2", 0, fHe3AnodeDiameter / 2., anodeWireLength / 2., 0., 360. * deg);
    lAnodeWire2 = new G4LogicalVolume(solidAnodeWire1, stainlessSteelMaterial, "AnodeWire");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, -outerRadiusOfDetector, 0.), lAnodeWire, "AnodeWire2", logicWorld, false, 0);




        //Neutron Scorer for third NCD, only use when main neutron scorer is commented
    //new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, 0.), neutronScorerlogic, "NeutronScorer3", logicWorld, false, 0);
    // He3 Gas Tube (inner tube)- Third NCD
    //G4Tubs* solidHe3Tube3 = new G4Tubs("He3Tube3", 0, fHe3TubeRadius, fHe3TubeL / 2., 0., 360. * deg);
    //lHe3CuTube3 = new G4LogicalVolume(solidHe3Tube3, He3Gas, "He3CuTube3");
    // Placing the third NCD in the opposite corner, using lHe3CuTube from previous NCD
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, 0.), lHe3CuTube, "He3CuTube3", logicWorld, false, 0);

    // Nickel Tube (outer tube) - Third NCD
    /*G4Tubs* solidNickelTube3 = new G4Tubs("NickelTube3", fHe3TubeRadius, fHe3TubeRadius + fHe3TubeThickness, fHe3TubeL / 2., 0., 360. * deg);
    lNickelTube3 = new G4LogicalVolume(solidNickelTube1, nickelMaterial, "NickelTube2");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, 0.), lNickelTube, "NickelTube3", logicWorld, false, 0);

    // Steel Caps (front and back) - Third NCD
  /*  G4Tubs* solidFrontSteelCap3 = new G4Tubs("FrontSteelCap3", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
    lFrontSteelCap3 = new G4LogicalVolume(solidFrontSteelCap1, stainlessSteelMaterial, "FrontSteelCap");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, -(fHe3TubeL / 2. + fSteelCapThickness / 2.)), lFrontSteelCap, "FrontSteelCap3", logicWorld, false, 0);

    /* G4Tubs* solidBackSteelCap3 = new G4Tubs("BackSteelCap3", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
     lBackSteelCap3 = new G4LogicalVolume(solidBackSteelCap1, stainlessSteelMaterial, "BackSteelCap");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, fHe3TubeL / 2. + fSteelCapThickness / 2.), lBackSteelCap, "BackSteelCap3", logicWorld, false, 0);

    // Anode Wire - Third NCD
   /* G4Tubs* solidAnodeWire3 = new G4Tubs("AnodeWire3", 0, fHe3AnodeDiameter / 2., anodeWireLength / 2., 0., 360. * deg);
    lAnodeWire3 = new G4LogicalVolume(solidAnodeWire1, stainlessSteelMaterial, "AnodeWire");*/
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, 0.), lAnodeWire, "AnodeWire3", logicWorld, false, 0);
    
    return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    //Setting sensitive detectors as the logical volumes of the inner BDS
    SensitiveDetector* sensDet =
        new SensitiveDetector("SensitiveDetector");
    //Set the sensitive detector
    lHe3CuTube->SetSensitiveDetector(sensDet); //logicWorld, lHe3CuTube
    G4cout << " Sensitive Detector Set";

       // Print message to confirm
    G4cout << "Sensitive Detectors set for both He3Tube." << G4endl;

}