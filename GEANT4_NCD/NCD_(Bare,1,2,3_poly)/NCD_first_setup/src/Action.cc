#include "Action.hh"
#include "Run.hh"
#include "MySteppingAction.hh"
#include "MyEventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "MyStackingAction.hh"

ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const
{
    // Primary generator
         auto generator = new PrimaryGeneratorAction();
             SetUserAction(generator);
    
               // Run action
                auto runAction = new MyRunAction();
                    SetUserAction(runAction);
                 // Event action (should be created before SteppingAction so both can link to RunAction)
                          auto eventAction = new MyEventAction(runAction);
                      SetUserAction(eventAction);

                                  // Stepping action (linked to RunAction and possibly EventActio
                                       auto steppingAction = new MySteppingAction(runAction);
                                              SetUserAction(steppingAction);
 
                                          // Stacking action (controls which particles are tracked)
                                                auto stackingAction = new MyStackingAction();
                                              SetUserAction(stackingAction);
                                                  G4cout << "Actions set: PrimaryGeneratorAction, RunAction, MyEventAction, MySteppingAction, MyStackingAction" << G4endl;
 }
                                                     void ActionInitialization::BuildForMaster() const 
                                                   {
							// The master thread only uses RunAction
                                                               SetUserAction(new MyRunAction());
                                                                 }
