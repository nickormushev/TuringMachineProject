#pragma once
#include "TuringMachine.h"

///Creates a turing machine that runs another based on the output of the first. 
class IfMachine : public TuringMachine {
	TuringMachine* condition;
	TuringMachine* ifTM;
	TuringMachine* elseTM;
	///ifElse flag is used to see if we entered the if or the else on run. This is important for the headLocation
	bool ifEleseFlag;

public:
	void copy(TuringMachine& cond, TuringMachine& _TM1, TuringMachine& _TM2) {
		this->condition = cond.copy();
		this->ifTM = _TM1.copy();
		this->elseTM = _TM2.copy();
	}

	IfMachine() : condition(nullptr), ifTM(nullptr), elseTM(nullptr) {}

	IfMachine(TuringMachine& cond, TuringMachine& _TM1, TuringMachine& _TM2) { copy(cond, _TM1, _TM2); }

	IfMachine(const IfMachine& other) { copy(*other.condition, *other.ifTM, *other.elseTM); }

	IfMachine& operator=(IfMachine& other) {
		copy(*other.condition, *other.ifTM, *other.elseTM);
		return *this;
	}

	~IfMachine() {
		delete ifTM;
		delete elseTM;
		delete condition;
	}
	
	///used for more complicated composition of ifMachines, compMachines and whileMachines
	TuringMachine * copy() override {
		return new IfMachine(*this);
	}

	int headLocation() override {
		if (ifEleseFlag) {
			return this->ifTM->headLocation();
		} else {
			return this->elseTM->headLocation();
		}
	}

	bool run(std::string fileName = "") override {
		return this->runPos(fileName);
	}

	bool runPos(std::string fileName = "", int headPos = 0, std::string destFile = "result.txt") override {
		if (condition -> runPos(fileName, headPos, destFile)) {
			this->ifEleseFlag = true;
			return ifTM -> runPos(fileName, headPos, destFile);
		} else {
			this->ifEleseFlag = false;
			return elseTM -> runPos(fileName, headPos, destFile);
		}
	}
};

