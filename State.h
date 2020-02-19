#pragma once
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include "Rule.h"


///This class represents a state of the turing machine automata
class State {
	int id;
	std::string name;
	std::map<char, Rule*> rules;
	
	void copy(std::string _name, std::map<char, Rule*> _rules, int _id);

public:
	State(std::string _name = "", int _id = -1, std::map<char, Rule*> _rules = std::map<char, Rule*>());
	State(State& other);
	State& operator=(State& other);
	~State();
	
	int getId() const { return this->id; }
	std::string getName() const { return this->name; }
	void setName(std::string _name) { this->name = _name;  }
	std::map<char, Rule*> getRules() const { return this->rules; }
	Rule getRule(char x) { return *this->rules[x]; }
	void addRule(int id, char sym, char replaceSym, char direction);
	///reroutes all rules / edges that point to the oldStateId to the new
	void rerouteRules(int oldId, int newId);
};

