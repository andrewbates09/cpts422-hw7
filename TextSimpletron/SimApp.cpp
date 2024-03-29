// Author: Andrew S. O'Fallon

#include "SimApp.h"

SimApp::SimApp ()
{
}

SimApp::~SimApp ()
{
}

string SimApp::getFileName ()
{
	string fileName;

	cout << "Please enter the name of the SML file (i.e. filename.sml): ";
	cin >> fileName;

	return fileName;
}

void SimApp::openFile (string fileName, ifstream &inStream)
{
	// Need to convert from "string" type to "char *" --> c_str ()
	inStream.open (fileName.c_str (), ios::in);
}

// Read the instructions from a file and place them into memory
void SimApp::loadProgram (ifstream &inStream)
{
	SmlInstruction *tempPtr = mSmlComputer.getProgramMemoryPtr ();
	int index = 0;
	string instruction = "";

	while (!inStream.eof ())
	{
		std::getline (inStream, instruction);
		char opcode[4] = {0};
		strncpy(opcode, instruction.c_str(), 3);

		tempPtr[index].setOpcode ((OpcodeType) (atoi(opcode) ));
		tempPtr[index].setOperand ((atoi (&instruction.c_str()[3]) % mSmlComputer.getDataSize ()));
		index++;
	}

	mSmlComputer.setProgramSize (index);
}

void SimApp::displayProgram ()
{
	mSmlComputer.dumpMemory ();
}

void SimApp::closeFile (ifstream &inStream)
{
	inStream.close ();
}

void SimApp::runApp ()
{
	string fileName;
	ifstream inStream;

	fileName = getFileName ();
	openFile (fileName, inStream);

	loadProgram (inStream);

	if(mSmlComputer.getProgramSize() == 0)
	{
		printf("File is empty. Exiting...\n");
		system("pause");
		exit(0);
	}

	displayProgram ();

	system ("pause");

	do
	{
		mSmlComputer.fetch (); // get the instruction
		mSmlComputer.decode (); // determine the opcode and operands
		displayProgram ();
		mSmlComputer.execute (); // execute the correct operation
	} while (mSmlComputer.getOpcode () != HALT);

	displayProgram ();

	closeFile (inStream);
}