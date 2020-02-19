#pragma once
#include "TuringMachine.h"

///Creates a turing machine composition. If the first ends it runs the second
class CompositeTM : public TuringMachine
{
	TuringMachine* TM1;
	TuringMachine* TM2;

public:

	void copy(TuringMachine& _TM1, TuringMachine& _TM2) {
		this->TM1 = _TM1.copy();
		this->TM2 = _TM2.copy();
	}

	///used for more complicated composition of ifMachines, compMachines and whileMachines
	TuringMachine * copy() override {
		return new CompositeTM(*this);
	}

	CompositeTM() : TM1(nullptr), TM2(nullptr) {}
	CompositeTM(TuringMachine& _TM1, TuringMachine& _TM2) { copy(_TM1, _TM2); }
	CompositeTM(const CompositeTM& other) { copy(*other.TM1, *other.TM2); }

	CompositeTM& operator=(CompositeTM& other) {
		copy(*other.TM1, *other.TM2);
		return *this;
	}

	~CompositeTM() {
		delete TM1;
		delete TM2;
	}
	
	///I return only the TM2 head location because if we hit a false the while will end
	int headLocation() override {
		return this->TM2->headLocation();
	}

	bool run(std::string fileName = "") override {
		return this->runPos(fileName, 0);
	}

	bool runPos(std::string fileName = "", int headPos = 0, std::string destFile = "result.txt") override {
		if (TM1 -> runPos(fileName, 0, destFile)) {
			return TM2 -> runPos(destFile, 0, destFile);
		} else {
			return false;
		}
	}
};