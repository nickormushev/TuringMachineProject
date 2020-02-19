#pragma once
#include <string>
#include <fstream>
#include <list>
#include <iterator>

#include "State.h"

class TuringMachine {
public:

	TuringMachine(std::string filename = "");
	TuringMachine(const TuringMachine& other);
	virtual ~TuringMachine();
	TuringMachine& operator=(const TuringMachine& other);
	
	///runPos is used if you want to change the start position of run
	virtual bool runPos(std::string fileName = "", int headPos = 0, std::string destFile = "result.txt");
	virtual bool run(std::string fileName = "") { return this->runPos(fileName); };

	///this validDeserialize is based on the syntax specified in the project overview. Reads machine from file
	bool validDeserialize(std::string fileName);
	///takes in Turing machines M1 and M2 and generetae a turing machine that does while(M1) do M2 by rerouting their automatas
	TuringMachine& generateWhileMachine(TuringMachine& cond, TuringMachine& body);
	///Finds how far along we are from the start of the tape
	virtual int headLocation();
	std::list<char>::iterator getMemoryTapeBegin() { return this->memoryTape.begin(); }
	State* getCurrentState() const { return this->currentState; }
	bool readTapeInput(std::string fileName);
	///used for creating more complex compisitions of while if and composite turing machines
	virtual TuringMachine* copy();
protected:
	bool checkQuit();

private:
	/// deletes the states
	void destroy();
	///adds the cond and body to the states of the while machine
	void addStatesForWhileMachine(std::map<int, State*> _states);
	void moveHead(char direction, char nextSym);
    ///saves the tape to file
	void saveTape(std::string destFile);
	void copy(State* _startState, std::map<int, State*> _states);
	///reroutes the edges of all states that have e rule that points to the oldStateId.
	void reroute(int oldStateId, int newStateId);
	///adds a state for deserialization and checks if it is already added
	void addState(std::string newStateName, std::vector<std::string>& addedStates);
	///Searches for the id of a state and if not found returns the id that will be given to the next state that is created
	int findStateId(std::string stateName);

	std::list<char> memoryTape;
	State* currentState;
	State* startState;
	std::string tapeInputFileName;
	std::map<int, State*> states;
	std::list<char>::iterator head;
	static int nextStateId;
};

