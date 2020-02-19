#include <iostream>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "CompositeTM.h"
#include "IfMachine.h"
#include "WhileMachine.h"

int TuringMachine::nextStateId = 1;

//starts another turing machine with the same input based on the output of the cond one
#include "Turing_Machine_Tests.h"


int main() {
	doctest::Context().run();
}
