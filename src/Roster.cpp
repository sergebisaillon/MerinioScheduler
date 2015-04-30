#include "MyTools.h"
#include "Scenario.h"
#include "Nurse.h"
#include "Roster.h"

// Constructor: initialize planning from nothing
//
Roster::Roster(int nbDays, int firstDay) {

  // initialize the roster with a rest week
  init(nbDays, firstDay);
}

// Constructor: initialize planning from an input set of shifts for the nurse
//
Roster::Roster(int nbDays, int firstDay, vector<int> shifts):
nbDays_(nbDays), firstDay_(firstDay), shifts_(shifts) {

  // initialize the states at each day
  // states_.push_back(initialState);
  // for (int day = 0; day < nbDays_; day++) {
  //   State nextState;
  //   nextState.addDayToState(states_[day], shifts_[day]);
  //   states_.push_back(nextState);
  // }

}

// Constructor: initialize planning from an input set of shifts and skills
//
Roster::Roster(int nbDays, int firstDay, vector<int> shifts, vector<int> skills):
nbDays_(nbDays), firstDay_(firstDay), shifts_(shifts) {

  // set the skill assignment
  for (int day = 0; day < nbDays_; day++) skills_.push_back(skills[day]);

}

// Destructor
Roster::~Roster(){
}


// initialize the roster
//
void Roster::init(int nbDays, int firstDay, int skillDefault) {

  nbDays_ = nbDays;
  firstDay_ = firstDay;

  // initialize the vectors of skills and shifts
  for (int day = 0; day < nbDays_; day++) skills_.push_back(skillDefault);
  for (int day = 0; day < nbDays_; day++) shifts_.push_back(0);
}

// get a vector of consecutive states that will result from applying the
// the roster from a given initial state
//
vector<State> Roster::getStates(const State& stateIni) {

  vector<State> states;

  // initialize the states at each day
  states.push_back(stateIni);
  for (int day = 0; day < nbDays_; day++) {
    State nextState;
    nextState.addDayToState(states[day], shifts_[day]);
    states.push_back(nextState);
  }

  return states;
}

// assign a task at on a given day
//
void Roster::assignTask(int day, int shift, int skill) {
  shifts_[day] = shift;
  skills_[day] = skill;
}

// add a roster at the end of the roster
//
void Roster::push_back(Roster& roster) {
  if (!this->nbDays_) {
    this->copy(roster);
  }
  else if (!Tools::isSunday(firstDay_+nbDays_)) {
    Tools::throwError("Roster::push_back: The last day of the current roster is not a sunday!");
  }
  else {
    nbDays_ += roster.nbDays();
    for (int day = 0; day < roster.nbDays(); day++) {
      shifts_.push_back(roster.shift(day));
      skills_.push_back(roster.skill(day));
    }
  }

  if (!Tools::isSunday(firstDay_+nbDays_)) {
    Tools::throwError("Roster::push_back: The last day of the updated roster is not a sunday!");
  }
}

// copy the input roster
//
void Roster::copy(Roster& roster) {
  firstDay_ = roster.firstDay();
  nbDays_ = roster.nbDays();
  if (!shifts_.empty()) {
    shifts_.clear();
    skills_.clear();
  }
  for (int day = 0; day < nbDays_; day++) {
    shifts_.push_back(roster.shift(day));
    skills_.push_back(roster.skill(day));
  }
}
