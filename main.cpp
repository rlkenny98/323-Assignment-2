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

void lexicalAnalysis(vector<string>, vector<tokens>&);

int main(int argc, char *argv[])
{	
	vector<string> code;
	vector<tokens> tokenVec;
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

	while (getline(fin, line)) {code.push_back(line);}
	fin.close();

	// Lex the code 
	lexicalAnalysis(code,tokenVec);
	fout.open("output.txt");
	if(!fout.is_open()){ cout << "Output File Error\n"; exit(1);}
	if (!syntaxAnalyze(tokenVec, fout)) {
		cout << "Syntax error" << endl;
		fout << "ERROR: syntax error found in the source code" << endl;
	}

	fout.close();

	return 0;
}

// Lexical analysis implementation with finite state machine
void lexicalAnalysis(vector<string>codeVector, vector<tokens> &tokenizedCode){
	FSM machine;
	int state = 0;
	long long unsigned int lexStart = 0;
	cout << "Time for lexer to do its thing...." << endl;
	// Change variable names and logic/port to function
		// These for loops need the ugly ass integer type because of the return type of .size() and .length()
	for (long long unsigned int vecString = 0; vecString < codeVector.size(); vecString++) {
		for (long long unsigned int vecChar = 0; vecChar <= codeVector[vecString].length(); vecChar++) {
			if (state == 0) {
				lexStart = vecChar;
			}
			// Combined two lines
			state = machine.inputCheck(state, machine.char_to_input(codeVector[vecString][vecChar]));
			if (machine.is_final_state(state)) {
				if (machine.backup(state)) {
					vecChar--;
				}
				if (state != 7) {
					string lex = "";
					for (long long unsigned int i = lexStart; i <= vecChar; i++) {
						lex += codeVector[vecString][i];
					}
					if (machine.getToken(state, lex) != "OTHER") {
						string tok = machine.getToken(state, lex);
						cout << "token: " << tok << " lex: "<< lex << endl;
						tokenizedCode.push_back(tokens(tok, lex));
					}
				}
				state = 0;
			}
		}
	}
	return;
}