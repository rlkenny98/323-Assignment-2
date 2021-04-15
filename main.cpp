/*############## Welcome to main ##############

- Use g++ -Wall -c -g main.cpp -o main.o 
  followed by g++ main.o -o main 

- Run the program with ./main 
#################### Authors ##################
-Robert Kenny 
-
-
-
###############################################
*/

/* ############## TO-DO ################
- Change grammar rukes in syntax.h
- Clean up and change code up 
	- Code has been modified a bit already
	- If unsure, compare with the original file 
*/

// Include Libraries and header files
#include <iostream>
#include "FSM.h"
#include "syntax.h"
using namespace std;

int main(int argc, char *argv[])
{	
	vector<string> codeVector;
	ofstream fout;
	ifstream fin;
	string file_name;
	string input;
	string line;

  	if(argc < 2)
		{cout << "ERROR - format should be: ./main inputFile\n"; exit(1);}
  	else
    	input = argv[1];
	//Open our input
	fin.open(input);
	if (!fin.is_open())
		{cerr << "File Opening Error\n"; exit(-1);}

	while (getline(fin, line)) {codeVector.push_back(line);}
	fin.close();

	//LEXER
	cout << "Time for lexer to do its thing...." << endl;
	// These have been changed and are good to go
	vector<tokens> lexerStorage;
	FSM machine;
	int state = 0;
	int lexStart = 0;
	
	// Change variable names and logic/port to function
		// These for loops need the ugly ass integer type because of the return type of .size() and .length()
	for (long long unsigned int vecString = 0; vecString < codeVector.size(); vecString++) {
		for (long long unsigned int vecChar = 0; vecChar <= codeVector[vecString].length(); vecChar++) {
			if (state == 0) {
				lexStart = vecChar;
			}
			// Combined two lines
			state = machine.check_input(state, machine.char_to_input(codeVector[vecString][vecChar]));
			if (machine.is_final_state(state)) {
				if (machine.should_back_up(state)) {
					vecChar--;
				}
				if (state != 7) {
					string lex = "";
					for (long long unsigned int i = lexStart; i <= vecChar; i++) {
						lex += codeVector[vecString][i];
					}
					if (machine.getTokenName(state, lex) != "OTHER") {

						string tok = machine.getTokenName(state, lex);
						cout << "token: " << tok << endl;
						lexerStorage.push_back(tokens(tok, lex));
					}
				}
				state = 0;
			}
		}
	}

	fout.open("output.txt");
	if(!fout.is_open()){ cout << "Output File Error\n"; exit(1);}
	if (!syntaxAnalyze(lexerStorage, fout)) {
		cout << "Syntax error" << endl;
		fout << "ERROR: syntax error found in the source code" << endl;
	}

	fout.close();

	return 0;
}