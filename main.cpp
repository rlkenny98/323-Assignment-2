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
#include "state_machine.h"
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
	vector<tokens> token_lexeme;
	StateMachine FSM;
	int curr_state = 0;
	int lexeme_start = 0;
	
	for (long long  unsigned int line = 0; line < codeVector.size(); line++) {
		for (long long unsigned int char_ = 0; char_ <= codeVector[line].length(); char_++) {
			if (curr_state == 0) {
				lexeme_start = char_;
			}
			int curr_input = FSM.char_to_input(codeVector[line][char_]);
			curr_state = FSM.check_input(curr_state, curr_input);
			if (FSM.is_final_state(curr_state)) {
				if (FSM.should_back_up(curr_state)) {
					char_--;
				}
				if (curr_state != 7) {
					string lex = "";
					for (long long unsigned int i = lexeme_start; i <= char_; i++) {
						lex += codeVector[line][i];
					}
					if (FSM.getTokenName(curr_state, lex) != "OTHER") {
						token_lexeme.push_back(tokens(FSM.getTokenName(curr_state, lex), lex));
					}
				}
				curr_state = 0;
			}
		}
	}
	fout.open("output.txt");
	if(!fout.is_open()){ cout << "Output File Error\n"; exit(1);}
	if (!analyze_syntax(token_lexeme, fout)) {
		cout << "Syntax error" << endl;
		fout << "ERROR: syntax error found in the source code" << endl;
	}

	fout.close();

	return 0;
}