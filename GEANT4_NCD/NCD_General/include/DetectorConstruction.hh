#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"

#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "Detector.hh"
// Forward declaration of necessary classes
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Element;
class G4PVPlacement;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction();

    // The method for constructing the detector and sensitive detectors
    virtual G4VPhysicalVolume* Construct() override;

    // New method to set up sensitive detectors and fields
    virtual void ConstructSDandField();

    //private:
private:
    G4LogicalVolume* lNickelTube;
    G4LogicalVolume* lHe3CuTube;
    G4LogicalVolume* lHe3GasTube;
    G4LogicalVolume* lBackSteelCap;
    G4LogicalVolume* lBackSteelCapFace;
    G4LogicalVolume* lFrontSteelCap;
    G4LogicalVolume* lFrontSteelCapFace;
    G4LogicalVolume* lAnodeWire;

    /*G4LogicalVolume* lNickelTube2;
    G4LogicalVolume* lHe3CuTube2;
    G4LogicalVolume* lHe3GasTube2;
    G4LogicalVolume* lBackSteelCap2;
    G4LogicalVolume* lBackSteelCapFace2;
    G4LogicalVolume* lFrontSteelCap2;
    G4LogicalVolume* lFrontSteelCapFace2;
    G4LogicalVolume* lAnodeWire2;*/
    G4LogicalVolume* logicWorld;

    G4Material* worldMaterial;    // Declare once

    // Materials
    G4Material* nickelMaterial;
    G4Material* stainlessSteelMaterial;
    G4Material* airMaterial;

    // Detector dimensions
    G4double fHe3TubeL;             // Length of the He3 Tube
    G4double fHe3TubeRadius;        // Inside radius of the Nickel wall
    G4double fHe3TubeThickness;     // Thickness of the Nickel wall
    G4double fSteelCapThickness;    // Thickness of the Steel Cap Face
    G4double fHe3VolumeMiddlePointOffset;  // Offset for He3 Volume's Middle Point
    G4double fHe3AnodeDiameter; //Anode Diameter
    G4double fHe3AnodeProtrustion; //Anode Protrustion length
    //  virtual void ConstructSDandFields();
};

#endif
