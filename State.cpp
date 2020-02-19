#include "State.h"

State& State::operator=(State& other) {
	this->copy(other.name, other.rules, other.id);
	return *this;
}

State::State(std::string _name, int _id, std::map<char, Rule*> _rules) {
	this->copy(_name, _rules, _id);
};

State::State(State& other) {
	this->copy(other.name, other.rules, other.id);
}

State::~State() {
	for (std::pair<char, Rule*> i : this->rules) {
		if (i.second != nullptr) {
			delete i.second;
		}
	}
}

void State::copy(std::string _name, std::map<char, Rule*> _rules, int _id) {
	this->name = _name;
	this->id = _id;

	for (std::pair<char, Rule*> i : _rules) {
		this->rules.insert(std::pair<char, Rule*>(i.first, new Rule(i.second->getDirection(), 
										i.second->getReplaceSym(), i.second->getNextStateId())));
	}
};

void State::addRule(int id, char sym, char replaceSym, char direction) {
	rules.insert(std::pair<char, Rule*>(sym, new Rule(direction, replaceSym, id)));
}

void State::rerouteRules(int oldId, int newId) {

	for (std::pair<char, Rule*> i : this->rules) {
		if (i.second->getNextStateId() == oldId) {
			i.second->setNextStateId(newId);
		}
	}
}