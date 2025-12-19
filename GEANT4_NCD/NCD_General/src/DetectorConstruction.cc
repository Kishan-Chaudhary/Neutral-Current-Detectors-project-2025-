#include "G4NistManager.hh"
#include "DetectorConstruction.hh"
#include "G4Tubs.hh"
#include "NCDGeometry.hh" // Includes all centralized constants
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "CADMesh.hh"


// Constructor and Destructor (omitted for brevity)
DetectorConstruction::DetectorConstruction()
    // ... (constructor initialization list)
    : fHe3TubeL(He3TubeL205cm),             
    fHe3TubeRadius(He3NickelIR),          
    fHe3TubeThickness(He3NickelTh),       
    fSteelCapThickness(steelCapFaceThickness), 
    fHe3VolumeMiddlePointOffset(He3VolumeMiddlePointOffset), 
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
    worldMaterial(nullptr)
{}

DetectorConstruction::~DetectorConstruction() {}

// Construct the detector's physical volume
G4VPhysicalVolume* DetectorConstruction::Construct() {

    // Define materials

    // He3 Element
    G4Isotope* he3 = new G4Isotope("he3", 2, 3, He3MolarMass);
    G4Element* He3 = new G4Element("He3", "He3", 1);
    He3->AddIsotope(he3, 100. * perCent);

    // Get C and F for CF4
    G4Element* elC = G4NistManager::Instance()->FindOrBuildElement("C");
    G4Element* elF = G4NistManager::Instance()->FindOrBuildElement("F");

    // CF4 molecule
    G4Material* CF4 = new G4Material("CF4", DENSITY_CF4, N_ELEM_CF4);
    CF4->AddElement(elC, 1);
    CF4->AddElement(elF, 4);

    // He3 Gas Mixture (He3 + CF4)
    G4Material* He3Gas = new G4Material("He3", CounterGasDensity, N_COMP_HE3GAS, kStateGas, TEMP_HE3GAS, PRESSURE_HE3GAS);
    He3Gas->AddElement(He3, PERCENT_HE3);
    He3Gas->AddMaterial(CF4, PERCENT_CF4);

    // Nickel Material
    G4Element* Ni = G4NistManager::Instance()->FindOrBuildElement("Ni");
    G4Material* nickelMaterial = new G4Material("Nickel", DENSITY_NICKEL, 1);
    nickelMaterial->AddElement(Ni, 1);

    // Stainless Steel
    G4Element* Fe = G4NistManager::Instance()->FindOrBuildElement("Fe");
    G4Element* Cr = G4NistManager::Instance()->FindOrBuildElement("Cr");
    G4Material* stainlessSteelMaterial = new G4Material("G4_STAINLESS_STEEL", DENSITY_STEEL, N_ELEM_STEEL);
    stainlessSteelMaterial->AddElement(Fe, FRAC_STEEL_FE);
    stainlessSteelMaterial->AddElement(Cr, FRAC_STEEL_CR);
    stainlessSteelMaterial->AddElement(Ni, FRAC_STEEL_NI);

    // Air Material
    G4Element* N = G4NistManager::Instance()->FindOrBuildElement("N");
    G4Element* O = G4NistManager::Instance()->FindOrBuildElement("O");
    G4Material* airMaterial = new G4Material("Air", DENSITY_AIR, N_ELEM_AIR, kStateGas, TEMP_AIR, PRESSURE_AIR);
    airMaterial->AddElement(N, FRAC_AIR_N);
    airMaterial->AddElement(O, FRAC_AIR_O);


    // H2O components
    G4Element* H = G4NistManager::Instance()->FindOrBuildElement("H");

    // H2O
    G4Material* H2O = new G4Material("H2O", DENSITY_H2O, N_ELEM_H2O);
    H2O->AddElement(H, 2);
    H2O->AddElement(O, 1);


    // PolyEthylene components
    // Custom Hydrogen for thermal scattering model
    G4Element* elTSH = new G4Element( "TS_H_of_Polyethylene" ,"H_POLYETHYLENE" , 1.0 , ATOMIC_MASS_TSH );
    
    // Create boron element with natural abundances
    G4Isotope* isoB10 = new G4Isotope("B10", 5, 10, ATOMIC_MASS_B10);
    G4Isotope* isoB11 = new G4Isotope("B11", 5, 11, ATOMIC_MASS_B11);
    G4Element* elB = new G4Element("Boron", "B", 2);
    elB->AddIsotope(isoB10, PERCENT_B10);
    elB->AddIsotope(isoB11, PERCENT_B11);

    G4Material* PolyEthylene = new G4Material("PolyEthylene", DENSITY_POLYETHYLENE, N_ELEM_POLYETHYLENE);
    PolyEthylene->AddElement(elC, 2);
    PolyEthylene->AddElement(elTSH, 4);

    G4Material* boratedHDPe = new G4Material("Borated PolyEthylene", DENSITY_BORATED_HDPE, N_ELEM_BORATED_HDPE);
    boratedHDPe->AddElement(elC, FRAC_BHDPE_C);
    boratedHDPe->AddElement(elB, FRAC_BHDPE_B);
    boratedHDPe->AddElement(H, FRAC_BHDPE_H);

    // Vacuum
    auto vacuum = new G4Material("Galactic", 1., VACUUM_DENSITY_MOLAR, universe_mean_density,
        kStateGas, VACUUM_TEMP, VACUUM_PRESSURE);
    
    //Set World Material
    worldMaterial = vacuum;
    
    // --- GEOMETRY CONSTRUCTION ---
    // ... (World and NCD construction remains unchanged) ...

    // Define outer radius of the NCD (used for placement offset)
    G4double outerRadiusOfDetector = fHe3TubeRadius + fHe3TubeThickness;

    // World Volume
    G4Tubs* solidWorld = new G4Tubs("WorldTube", 0, WORLD_OUTER_RADIUS, WORLD_HALF_LENGTH, 0., 360. * deg);
    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld, worldMaterial, "World");
    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    
    // ... (NCD 1, 2, and 3 construction) ...

    G4double capFrontZ = -(fHe3TubeL / 2. + fSteelCapThickness / 2.);
    G4double capBackZ = fHe3TubeL / 2. + fSteelCapThickness / 2.;
    G4double anodeWireLength = fHe3TubeL + 2.0 * (fHe3AnodeProtrustion - fSteelCapThickness);
    
    // He3 Gas Tube (inner tube)
    G4Tubs* solidHe3Tube1 = new G4Tubs("He3Tube1", 0, fHe3TubeRadius, fHe3TubeL / 2., 0., 360. * deg);
    lHe3CuTube = new G4LogicalVolume(solidHe3Tube1, He3Gas, "He3CuTube");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, 0.), lHe3CuTube, "He3CuTube1", logicWorld, false, 0);

    // Nickel Tube (outer tube)
    G4Tubs* solidNickelTube1 = new G4Tubs("NickelTube1", fHe3TubeRadius, fHe3TubeRadius + fHe3TubeThickness, fHe3TubeL / 2., 0., 360. * deg);
    lNickelTube = new G4LogicalVolume(solidNickelTube1, nickelMaterial, "NickelTube1");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, 0.), lNickelTube, "NickelTube1", logicWorld, false, 0);

    // Steel Caps 
    G4Tubs* solidFrontSteelCap1 = new G4Tubs("FrontSteelCap1", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
    lFrontSteelCap = new G4LogicalVolume(solidFrontSteelCap1, stainlessSteelMaterial, "FrontSteelCap");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, capFrontZ), lFrontSteelCap, "FrontSteelCap1", logicWorld, false, 0);

    G4Tubs* solidBackSteelCap1 = new G4Tubs("BackSteelCap1", 0, fHe3TubeRadius + fHe3TubeThickness, fSteelCapThickness / 2., 0., 360. * deg);
    lBackSteelCap = new G4LogicalVolume(solidBackSteelCap1, stainlessSteelMaterial, "BackSteelCap");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, capBackZ), lBackSteelCap, "BackSteelCap1", logicWorld, false, 0);

    // Anode Wire
    G4Tubs* solidAnodeWire1 = new G4Tubs("AnodeWire1", 0, fHe3AnodeDiameter / 2., anodeWireLength / 2., 0., 360. * deg);
    lAnodeWire = new G4LogicalVolume(solidAnodeWire1, stainlessSteelMaterial, "AnodeWire");
    new G4PVPlacement(0, G4ThreeVector(outerRadiusOfDetector, -outerRadiusOfDetector, 0.), lAnodeWire, "AnodeWire1", logicWorld, false, 0);
    
    // NCD 2
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector , -outerRadiusOfDetector, 0.), lHe3CuTube, "He3CuTube2", logicWorld, false, 0);
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, -outerRadiusOfDetector, 0.), lNickelTube, "NickelTube2", logicWorld, false, 0);
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, -outerRadiusOfDetector, capFrontZ), lFrontSteelCap, "FrontSteelCap2", logicWorld, false, 0);
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, -outerRadiusOfDetector, capBackZ), lBackSteelCap, "BackSteelCap2", logicWorld, false, 0);
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, -outerRadiusOfDetector, 0.), lAnodeWire, "AnodeWire2", logicWorld, false, 0);

    // NCD 3 
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, 0.), lHe3CuTube, "He3CuTube3", logicWorld, false, 0);
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, 0.), lNickelTube, "NickelTube3", logicWorld, false, 0);
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, capFrontZ), lFrontSteelCap, "FrontSteelCap3", logicWorld, false, 0);
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, capBackZ), lBackSteelCap, "BackSteelCap3", logicWorld, false, 0);
    new G4PVPlacement(0, G4ThreeVector(-outerRadiusOfDetector, outerRadiusOfDetector, 0.), lAnodeWire, "AnodeWire3", logicWorld, false, 0);

    // --- Conditional Polyethylene Castle Geometry ---
    if(POLYBOOL)
    {
        // 1. Define Inner Cavity Dimensions (where the NCDs sit)
        G4double cavityHalfLength = (fHe3TubeL / 2.) + POLY_BASE_OFFSET;
        G4double cavityHalfHeight = He3NickelOR + POLY_HEIGHT_OFFSET;
        
        // The innermost solid is the empty box/cavity
        G4Box* innerCavityBox = new G4Box("InnerCavityBox", cavityHalfHeight, cavityHalfHeight, cavityHalfLength);

        // --- CASE 1: Layered Shield (PE Core + Borated Shell) ---
        if (USE_LAYERED_SHIELD == "true") 
        {
            G4double purePE_thickness = INNER_POLY_THICKNESS;
            G4double boratedPoly_thickness = THICKNESS_BORATED_POLY;

            // --- Pure Polyethylene Core (Inner Layer) ---
            
            // Dimensions of the Outer surface of the Pure PE layer (Inner Cavity + Pure PE Thickness)
            G4double innerPE_OuterHalfLength = cavityHalfLength + purePE_thickness;
            G4double innerPE_OuterHalfHeight = cavityHalfHeight + purePE_thickness;
            
            // Solid for the Pure PE Layer
            G4Box* innerPE_OuterBox = new G4Box("InnerPE_OuterBox", innerPE_OuterHalfHeight, innerPE_OuterHalfHeight, innerPE_OuterHalfLength);
            
            // Pure Polyethylene Solid: InnerPE_OuterBox - InnerCavityBox
            G4SubtractionSolid* PurePolyethyleneSolid = new G4SubtractionSolid("PurePolyethyleneSolid", innerPE_OuterBox, innerCavityBox);
            G4LogicalVolume* PurePolyethyleneLogic = new G4LogicalVolume(PurePolyethyleneSolid, PolyEthylene, "PurePolyethyleneLV");
            
            // --- Borated HDPE Shell (Outer Layer) ---
            
            // Dimensions of the Outer surface of the Borated Poly layer (PE Outer + BHDPE Thickness)
            G4double total_thickness = purePE_thickness + boratedPoly_thickness;
            
            G4double boratedPoly_OuterHalfLength = cavityHalfLength + total_thickness;
            G4double boratedPoly_OuterHalfHeight = cavityHalfHeight + total_thickness;
            
            // Solid for the Borated HDPE Layer
            G4Box* outerBox = new G4Box("OuterBox", boratedPoly_OuterHalfHeight, boratedPoly_OuterHalfHeight, boratedPoly_OuterHalfLength);
            
            // Borated HDPE Solid: OuterBox - InnerPE_OuterBox
            // The "inner cavity" for the BHDPE layer is the outer boundary of the PE layer.
            G4SubtractionSolid* BoratedHDPE_Solid = new G4SubtractionSolid("BoratedHDPE_Solid", outerBox, innerPE_OuterBox);
            G4LogicalVolume* BoratedHDPE_Logic = new G4LogicalVolume(BoratedHDPE_Solid, boratedHDPe, "BoratedHDPE_LV");

            // 4. Placement of Layers (Concentric)
            new G4PVPlacement(0, G4ThreeVector(0, 0, 0), PurePolyethyleneLogic, "PurePolyethylenePhys", logicWorld, false, 0, true);
            new G4PVPlacement(0, G4ThreeVector(0, 0, 0), BoratedHDPE_Logic, "BoratedHDPEPhys", logicWorld, false, 0, true);
            
            G4cout << "Polyethylene Castle Created with Layered Shield (PE inner, Borated HDPE outer)." << G4endl;

            // The total outer dimensions are required for the Scorer
            G4double finalOuterHalfLength = boratedPoly_OuterHalfLength;
            G4double finalOuterHalfHeight = boratedPoly_OuterHalfHeight;
            
            // --- Neutron Scorer (Using final outer dimensions) ---
            G4Box* innerNeutronScorer = new G4Box("innerNeutronScorer", finalOuterHalfHeight, finalOuterHalfHeight, finalOuterHalfLength);
            G4Box* outerNeutronScorer = new G4Box("outerNeutronScorer", finalOuterHalfHeight + NEUTRON_SCORER_OFFSET, finalOuterHalfHeight + NEUTRON_SCORER_OFFSET, finalOuterHalfLength + NEUTRON_SCORER_OFFSET);
            
            G4SubtractionSolid * NeutronScorer = new G4SubtractionSolid("NeutronScorerSolid", outerNeutronScorer, innerNeutronScorer);
            G4LogicalVolume* neutronScorerlogic = new G4LogicalVolume(NeutronScorer, vacuum, "NeutronScorer");
            new G4PVPlacement(0, G4ThreeVector(0, 0, 0), neutronScorerlogic, "NeutronScorer", logicWorld, false, 0);

        } 
        // --- CASE 2: Single-Material Shield (Existing Logic) ---
        else 
        {
            // The existing logic uses POLY_WALL_THICKNESS for a single layer
            G4double finalOuterHalfLength = cavityHalfLength + POLY_WALL_THICKNESS;
            G4double finalOuterHalfHeight = cavityHalfHeight + POLY_WALL_THICKNESS;
            
            G4Material* shieldMaterial = nullptr;
            
            if (POLY_SHIELD_MATERIAL == "PolyEthylene") {
                shieldMaterial = PolyEthylene;
            } else if (POLY_SHIELD_MATERIAL == "BoratedHDPE") {
                shieldMaterial = boratedHDPe;
            } else {
                G4cerr << "ERROR: Invalid POLY_SHIELD_MATERIAL specified! Falling back to pure PolyEthylene." << G4endl;
                shieldMaterial = PolyEthylene;
            }

            // Solids for single-layer castle
            G4Box* outerBox = new G4Box("OuterBox", finalOuterHalfHeight, finalOuterHalfHeight, finalOuterHalfLength);
            G4SubtractionSolid* PolyEthyleneSolid = new G4SubtractionSolid("PolyEthyleneSolid", outerBox, innerCavityBox);

            // Logical Volume and Placement (single layer)
            G4LogicalVolume* PolyEthyleneCastle = new G4LogicalVolume(PolyEthyleneSolid, shieldMaterial, "PolyEthyleneCastle");
            new G4PVPlacement(0, G4ThreeVector(0, 0, 0), PolyEthyleneCastle, "PolyEthyleneCastlePhys", logicWorld, false, 0, true);
            
            G4cout << "Polyethylene Castle Created with single material: " << shieldMaterial->GetName() << G4endl;

            // --- Neutron Scorer (Using final outer dimensions) ---
            G4Box* innerNeutronScorer = new G4Box("innerNeutronScorer", finalOuterHalfHeight, finalOuterHalfHeight, finalOuterHalfLength);
            G4Box* outerNeutronScorer = new G4Box("outerNeutronScorer", finalOuterHalfHeight + NEUTRON_SCORER_OFFSET, finalOuterHalfHeight + NEUTRON_SCORER_OFFSET, finalOuterHalfLength + NEUTRON_SCORER_OFFSET);
            
            G4SubtractionSolid * NeutronScorer = new G4SubtractionSolid("NeutronScorerSolid", outerNeutronScorer, innerNeutronScorer);
            G4LogicalVolume* neutronScorerlogic = new G4LogicalVolume(NeutronScorer, vacuum, "NeutronScorer");
            new G4PVPlacement(0, G4ThreeVector(0, 0, 0), neutronScorerlogic, "NeutronScorer", logicWorld, false, 0);
        }
        
        G4cout << "Polyethylene Castle Construction Complete." << G4endl;

    } else {
        G4cout << "Polyethylene Castle construction skipped (POLYBOOL is false)." << G4endl;
    }

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