#pragma once
#include "TuringMachine.h"
#include <list>

///realisation of the while turing machine(while (TM1) do TM2) with a class and not rerouting automatas.
class WhileMachine : public TuringMachine {
	TuringMachine * condition;
	TuringMachine * body;

public:
	void copy(TuringMachine& _cond, TuringMachine& _body) {
		this->condition = _cond.copy();
		this->body = _body.copy();
	}

	///used for more complicated composition of ifMachines and whileMachines
	TuringMachine * copy() override {
		return new WhileMachine(*this);
	}

	WhileMachine() : condition(nullptr), body(nullptr){}

	WhileMachine(TuringMachine& _cond, TuringMachine& _body) { copy(_cond, _body); }

	WhileMachine(const WhileMachine& other) { copy(*other.condition, *other.body); }

	WhileMachine& operator=(WhileMachine& other) {
		copy(*other.condition, *other.body);
		return *this;
	}

	~WhileMachine() {
		delete body;
		delete condition;
	}
	
	///As the name says it tells the headLocation after calculations. If we are successful we are in the condition. That is why we only return it's head location
	int headLocation() override {
		return this->condition->headLocation();
	}

	/*! If the body returns an error then we should return false. An error has occured
	 *  If the condition returns false then we have exited the loop successfully so we return true 
	 */
	bool run(std::string srcFileName = "") {
		return this->runPos(srcFileName);
	}

	bool runPos(std::string srcFileName = "", int headLoc = 0, std::string destFile = "result.txt") override {
		//first I must read from the file given and then i start reading from the destination
		int headLocation = 0;

		if (this->condition->runPos(srcFileName, 0, destFile)) {
			headLocation = this->condition->headLocation();
			if (!this->body->runPos(destFile, headLocation, destFile)) {
				return false;
			}
		} else {
			return true;
		}
		
		headLocation = this->body->headLocation();

		int counter = 0;
		while (this->condition->runPos(destFile, headLocation, destFile)) {
			headLocation = this->condition->headLocation();
			
			if (!this->body->runPos(destFile, headLocation, destFile)) {
				return false;
			}
			
			headLocation = this->body->headLocation();
			counter++;
			if (counter == 500 && checkQuit()) {
				return false;
			}

			if (counter > 500) {
				counter = 0;
			}
		}
		return true;
	}
};