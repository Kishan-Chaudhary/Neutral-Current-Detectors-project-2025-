#include "Action.hh"

// --- User Action Headers ---
#include "PrimaryGeneratorAction.hh"
#include "Run.hh"
#include "MyEventAction.hh"
#include "MySteppingAction.hh"
#include "MyStackingAction.hh"

// Constructor & Destructor
ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

// =========================================================================
// Build() - Called for every worker thread
// =========================================================================
void ActionInitialization::Build() const
{
    // 1. Primary Generator (Mandatory)
    // Defines how primary particles are generated (energy, position, type).
    auto generator = new PrimaryGeneratorAction();
    SetUserAction(generator);

    // 2. Run Action (Optional but recommended)
    // Handles Begin/End of run tasks (e.g., opening/closing files).
    auto runAction = new MyRunAction();
    SetUserAction(runAction);

    // 3. Event Action (Optional)
    // Handles Begin/End of event tasks. 
    // We pass 'runAction' so the event action can accumulate stats into the run object.
    auto eventAction = new MyEventAction(runAction);
    SetUserAction(eventAction);

    // 4. Stepping Action (Optional)
    // Called at every simulation step. Used for detailed tracking or filters.
    // We pass 'runAction' to allow live data accumulation during steps.
    auto steppingAction = new MySteppingAction(runAction);
    SetUserAction(steppingAction);

    // 5. Stacking Action (Optional)
    // Controls track priorities and can kill tracks before they start.
    auto stackingAction = new MyStackingAction();
    SetUserAction(stackingAction);

    G4cout << "Worker Thread Actions Initialized: Generator, Run, Event, Stepping, Stacking." << G4endl;
}

// =========================================================================
// BuildForMaster() - Called ONLY for the Master thread (MT mode)
// =========================================================================
void ActionInitialization::BuildForMaster() const 
{
    // The Master thread does not generate events or step particles.
    // It is responsible for merging results from worker threads.
    // Therefore, it usually only needs the RunAction.
    SetUserAction(new MyRunAction());
}