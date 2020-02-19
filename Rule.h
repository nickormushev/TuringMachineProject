#pragma once
#include <string>


///This represents a change of state for the turing machine and what to do with the tape
class Rule
{
	char direction;
	char replaceSym;
	int nextStateId;

public:
	Rule(char _direction = 'S', char _replaceSym = ' ', int _next = -1) {
		this->direction = _direction;
		this->nextStateId = _next;
		this->replaceSym = _replaceSym;
	}
	~Rule();

	void setNextStateId(int newId) { this->nextStateId = newId; }
	char getDirection() const { return this->direction;  }
	char getReplaceSym() const { return this->replaceSym;  }
	int getNextStateId() const { return this->nextStateId; }
};

