#pragma once

bool testFunction(std::list<char>::iterator& iter, char b) {
	
	for(int i = 0; i < 5; i++) {
		if (*iter != b) { return false; }
		iter++;
	}

	return true;
};

//File name explanation: Valid means the input or output is correct
//TuringMachineABC means that the TM reads a^n . b^n . c^N
//TuringMachineX means that the TM reads x^n
//TuringMachineSpaces reads _^n and stops when it sees an a
//ValidTuringMachine means I have used the syntax in the project specification(Used a different one at first, my mistake)
TEST_CASE("Print cool logo for my turing machine") {
	std::cout << R"(
'||\   ||`        '||             |' '||' '|  '||\   /||` 
 ||\\  ||     ''   ||      ''         ||       ||\\.//||  
 || \\ ||     ||   || //`  ||         ||       ||     ||  
 ||  \\||     ||   ||<<    ||         ||       ||     ||
.||   \||.   .||. .|| \\. .||.       .||.     .||     ||.)" << std::endl << std::endl;

	std::cout << "I have 3 infinite tests so please press q 3 times." << std::endl <<
		"I've left a comment why they exist in the while composition test case" << std::endl << std::endl;
}

TEST_CASE("ReadFromFile") {
	TuringMachine tmach("ValidTuringMachineABC.txt");
	tmach.readTapeInput("ValidInput.txt");
	State* startState = tmach.getCurrentState();

	SUBCASE("reading tape") {
		auto iter = tmach.getMemoryTapeBegin();
		CHECK(true == testFunction(iter, 'a'));
		CHECK(true == testFunction(iter, 'b'));
		CHECK(true == testFunction(iter, 'c'));
	}

	SUBCASE("reading states") {
		CHECK(startState->getName() == "replaceA");
	}

	SUBCASE("Testing rules by making a circle to start") {
		std::map<char, Rule*> firstRule = startState->getRules();
		CHECK(firstRule['a']->getDirection() == 'R');
		CHECK(firstRule['_']->getDirection() == 'S');
		CHECK(firstRule['_']->getReplaceSym() == '_');
	}
}

TEST_CASE("TuringMachine a^n b^n c^n") {
	TuringMachine tmach("ValidTuringMachineABC.txt");
	CHECK(tmach.run("ValidInput.txt") == true);
	CHECK(tmach.run("InvalidInput.txt") == false);
}

TEST_CASE("ComposeTuringMachine testing the compose") {
	//reads x^n
	TuringMachine tmach("ValidTuringMachineX.txt");
	//reads a^n b^n c^n
	TuringMachine tmach2("ValidTuringMachineABC.txt");

	CompositeTM compTM(tmach2, tmach);
	CompositeTM compCompTM(compTM, tmach);

	//compTM reads abc and then xs. In the second example with the two composition the xs machine reads the xs again. So should return true and it does
	CHECK(compTM.run("ValidInput.txt"));
	CHECK(compCompTM.run("ValidInput.txt"));
}

TEST_CASE("TuringMachine testing the ifMachine function") {
	TuringMachine tmachCond("ValidTuringMachineABC.txt");
	TuringMachine tmachIf(tmachCond);
	TuringMachine tmachElse("ValidTuringMachineAll.txt");
	IfMachine ifMachine(tmachCond, tmachIf, tmachElse);
	//testing if I can create an if machine with an ifmachine
	IfMachine ifMachineComp(ifMachine, tmachIf, tmachElse);

	CHECK(ifMachine.run("InvalidInput.txt"));
	CHECK(ifMachine.run("ValidInput.txt"));
	CHECK(ifMachineComp.run("ValidInput.txt"));
}

TEST_CASE("TuringMachine testing if the creation of a whileMachine is valid") {
	TuringMachine tmachCond("ValidTuringMachineABC.txt");
	TuringMachine tmachBody("ValidTuringMachineX.txt");
	TuringMachine result;

	//should return true because the condition is invalid and not the body
	WhileMachine test(tmachCond, tmachBody);
	result.generateWhileMachine(tmachCond, tmachBody);
	CHECK(result.run("InvalidInput.txt")); 
	CHECK(test.run("InvalidInput.txt"));
	
	TuringMachine newTmachBody("ValidTuringMachineSpaces.txt");
	result.generateWhileMachine(tmachCond, newTmachBody);
	WhileMachine test2(tmachCond, newTmachBody);
	CHECK(test2.run("ValidWhileMachineTestInput.txt"));
	CHECK(!test2.run("InvalidWhileMachineTestInput.txt"));
	CHECK(result.run("ValidWhileMachineTestInput.txt"));
	CHECK(!result.run("InvalidWhileMachineTestInput.txt"));
}

TEST_CASE("TuringMachine testing composition of while, if and comp") {
	TuringMachine tmachCond("ValidTuringMachineABC.txt");
	TuringMachine tmachIf(tmachCond);
	TuringMachine tmachElse("ValidTuringMachineAll.txt");
	TuringMachine tmachWhiteSpace("ValidTuringMachineSpaces.txt");

	IfMachine ifMachine(tmachCond, tmachIf, tmachElse);
	CompositeTM compTM(tmachCond, tmachIf);

	//testing if i can create an if machine with an ifmachine and if I can create a WhileMachine with an ifMachine
	WhileMachine validCondMachine(tmachCond, tmachWhiteSpace);
	WhileMachine ifWhileCompTest(ifMachine, tmachWhiteSpace);
	WhileMachine whileCompTest(compTM, ifMachine);
	WhileMachine whileWhileCompTest(validCondMachine, whileCompTest);

	//All three lead to infinite loops. I wanted to test the checkQuit function as well. Press q 3 times
	//I also lacked the imagination to conceive a valid input for such complicated machines
	CHECK(!ifWhileCompTest.run("ValidInput.txt"));
	CHECK(!whileCompTest.run("ValidInput.txt"));
	CHECK(!whileWhileCompTest.run("ValidWhileMachineTestInput.txt"));
}