SNOLAB Neutral Current Detectors(bare) Simulation - Kishan Chaudhary
----------------------------------------------------------------

1. PURPOSE
----------------------------------------------------------------
	The purpose of the simulation is to determine the response of the three NCD setup as a function of initial neutrons energy from a cylindrical neutron source(cos angular distribution).

	The response of the detectors for various moderation configurations can be built into a response matrix, which is required of NCD project unfolding(de-convolution).	


2. GEOMETRY
----------------------------------------------------------------
	The geometry includes 3 NCD detectors(like the NCD setup using the steel holder) without any moderation. The mother volume was chosen just large enough to contain the 3-inch Polyethylene box.

3. PHYSICS
----------------------------------------------------------------
	Tailored for neutron transport, ionization and gamma background studies: G4NDL, thermal scattering activated, HadronElasticPhysicsHP, G4HadronPhysicsQGSP_BIC_HP, Ion Physics, Electromagnetic, Stopping, physics

4. SENSITIVE DETECTOR HITS
----------------------------------------------------------------
	The He-4 and CF_4 gas mixture is the fiducial volume and hence the sensitive detector of the simulation. A neutron capture in this volume is assumed as a detected neutron count. The simulation counts the tritons produced and kills other particles in the stacking action to save computational time.
 
 The simulation utilizes Run, Event and Stepping action for counting. It can be customized to count other particles or record edep after editing stacking action.

5. Output
----------------------------------------------------------------
  The simulation outputs the triton counts and number of events for each run into a csv.

6. How to Run
----------------------------------------------------------------
	1. cd build
	2. cmake ..
	3. make
  4. make install
	4. Run the macro files from the macro directory, or run batch jobs with the RunAll_bareNCD.sh 


7. REFERENCES
----------------------------------------------------------------
	Various parts of this simulation were made using the official 
geant4 examples, the PhysicsMatters:geant4 tutorial series, and the 
Geant 4 Application Developers Guide as reference.
