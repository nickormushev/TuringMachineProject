#include "TuringMachine.h"
#include <map>

TuringMachine::TuringMachine(std::string fileName) : currentState(nullptr)
{
	if (fileName != "") {
		this->validDeserialize(fileName);
	}
}

TuringMachine::TuringMachine(const TuringMachine& other) {
	this->copy(other.startState, other.states);
}

TuringMachine& TuringMachine::operator=(const TuringMachine& other) {
	this->copy(other.startState, other.states);
	return *this;
}

void TuringMachine::destroy() {
	
	if (!this->states.empty()) {
		for (std::pair<int, State*> i : this->states) {
			if (i.second != nullptr) {
				delete i.second;
			}	
		}
		this->states.erase(this->states.begin(), this->states.end());
	}
}

TuringMachine::~TuringMachine() {
	this->destroy();
}


void TuringMachine::copy(State* _startState, std::map<int, State*> _states) {
	for (std::pair<int, State*> i : _states) {
		if (_startState == i.second) {
			this->startState = new State(*i.second);
			this->currentState = this->startState;
			this->states.insert(std::pair<int, State*>(i.first, this->startState));
		} else {
			this->states.insert(std::pair<int, State*>(i.first, new State(*i.second)));
		}
	}
}

TuringMachine* TuringMachine::copy() {
	return new TuringMachine(*this);
}

//asks user if he wants to quit
bool TuringMachine::checkQuit() {
	char quitCommand = ' ';
	std::cout << "Are you sure that what you entered is correct?" <<
		" Press q to quit or anything else to continue" << std::endl;
	std::cout << "$/> ";
	std::cin >> quitCommand;

	if (quitCommand == 'q') {
		return true;
	}

	return false;
}

int TuringMachine::headLocation() {
	std::list<char>::iterator _headCopy = head;
	int counter = 0;
	while (_headCopy != this->memoryTape.begin()) {
		counter++;
		_headCopy--;
	}

	return counter;
}

void TuringMachine::moveHead(char direction, char nextSym) {
	*this->head = nextSym;
	if (direction == 'L') {
		if (this->head == this->memoryTape.begin()) {
			this->memoryTape.push_front('_');
		}
		this->head--;
	} else if (direction == 'R') {
		auto lastElement = this->memoryTape.end();
		lastElement--;
		if (this->head == lastElement) {
			this->memoryTape.push_back('_');
		}
		this->head++;
	}
}

bool TuringMachine::runPos(std::string fileName, int headPos, std::string destFile) {
	if(fileName != ""){
		if (!this->readTapeInput(fileName)) {
			return false;
		};
	}

	if (headPos != 0) {
		std::advance(this->head,headPos);
	}

	unsigned counter = 0;
	Rule currentRule;

	while (this->currentState->getName() != "halt" && this->currentState->getName() != "error") {
		currentRule = this->currentState->getRule(*this->head);
		this->moveHead(currentRule.getDirection(), currentRule.getReplaceSym());
		currentState = this->states[currentRule.getNextStateId()];
		if (counter == 500 && checkQuit()) { return false; }
		if (counter > 500) { counter = 0; }
		counter++;
	}

	this->saveTape(destFile);

	if (currentState->getName() == "halt") {
		return true;
	} 

	return false;
}

bool TuringMachine::readTapeInput(std::string fileName) {
	
	//memory tape reset and current state reset
	this->currentState = startState;

	if (!this->memoryTape.empty()) {
		this->memoryTape.erase(this->memoryTape.begin(), this->memoryTape.end());
	}

	std::ifstream in(fileName);
	if (in.is_open()) {
		char line;
		//populates the tapes
		while (in >> line) {
			this->memoryTape.push_back(line);
		}
		head = this->memoryTape.begin();
		in.close();
		return true;
	} else return false;
}

void TuringMachine::addState(std::string newStateName, std::vector<std::string>& addedStates) {
	if (std::find(addedStates.begin(), addedStates.end(), newStateName) == addedStates.end()) {
		addedStates.push_back(newStateName);
		this->states.insert(std::pair<int, State*>(this->nextStateId, new State(newStateName, this->nextStateId)));
		this->nextStateId++;
	}
}

int TuringMachine::findStateId(std::string stateName) {
	for (std::pair<int, State*> i : this->states) {
		if (i.second->getName() == stateName) {
			return i.first;
		}
	}
	
	return this->nextStateId;
}

std::string getWordBetweenBrackets(std::string line) {
	
	unsigned start = line.find('{');
	unsigned end = line.find('}');
	//+ 1 so i skip '{' and -1 because if it isn't there '}' will be included
	return line.substr(start + 1, end - start - 1);
}

bool TuringMachine::validDeserialize(std::string fileName) {
	std::ifstream in(fileName);
	std::string line;
	char sym;
	std::string nextState;
	std::string currState;

	std::vector<std::string> addedStates;
	
	if (in.is_open()) {
		in >> line;

		this->currentState = new State(line, this -> nextStateId);
		this->startState = this->currentState;
		this->states.insert(std::pair<int, State*>(this->nextStateId++, this->currentState));
		addedStates.push_back(this->currentState->getName());

		while (in >> line) {
			sym = line[0];
			currState = getWordBetweenBrackets(line);

			this->addState(currState, addedStates);

			in >> line; //skip this symbol ->
			in >> line;

			nextState = getWordBetweenBrackets(line);
			int nextId = this->findStateId(nextState);
			int currStateId = this->findStateId(currState);
			this->addState(nextState, addedStates);
			
			char replaceSym = line[0];
			char direction = line[line.size() - 1];
			this->states[currStateId] -> addRule(nextId, sym, replaceSym, direction);
		}

		in.close();
		return true;
	} else return false;

};

void TuringMachine::saveTape(std::string destFile) {
	std::ofstream out(destFile);
	for (char i : this->memoryTape) {
		out << i << " ";
	}
	out.close();
}

void TuringMachine::addStatesForWhileMachine(std::map<int, State*> _states) {
	for (std::pair<int, State*> i : _states) {
		if (i.second->getName() != "halt") {
			this->states.insert(std::pair<int, State*>(i.first, new State(*i.second)));
		}
	}
}

void TuringMachine::reroute(int oldStateId, int newStateId) {
	for (std::pair<int, State*> i : this->states) {
		i.second->rerouteRules(oldStateId, newStateId);
	}
}

TuringMachine& TuringMachine::generateWhileMachine(TuringMachine& cond, TuringMachine& body) {
	
	//deletes states
	this->destroy();

	//I want to skip the halt state of the first and automata, because I don't want my function to end when I reach it
	//I want the edges that poin to the previous halt state to poin to the beggining of the second automata
	//And in the same logic for the second automata to point to the start of the first
	//In this way we achieve the loop of the while

	addStatesForWhileMachine(cond.states);
	addStatesForWhileMachine(body.states);


	int bodyStartId = body.startState->getId();
	int condStartId = cond.startState->getId();
	int condHaltId = cond.findStateId("halt");
	int bodyHaltId = body.findStateId("halt");

	this->reroute(condHaltId, bodyStartId);
	this->reroute(bodyHaltId, condStartId);

	this->startState = this->states[condStartId];
	this->currentState = this->startState;

	//Make all the error states, added from the cond, halt states
	//because if we reach them that means the loop ended properly
	//If the automata of the second Turing Machine reaches an error state that means the loop failed  
	int errorStateId = cond.findStateId("error");
	this->states[errorStateId]->setName("halt");
	
	return *this;
}