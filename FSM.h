#ifndef FSM_H
#define FSM_H

#pragma once
//This file defines the State Machine class 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;


// this is changed and all its references 
enum State {
	initialState,					// 0
	initialIdentifier,				// 1
	endIdentifier,					// 2 final state
	initialNumber,					// 3
	initialFloat,					// 4
	endNumber,						// 5 final state
	initialComment,					// 6
	endComment,						// 7 final state
	symbol							// 8 final state
};

enum Input {
	letter,			// 0
	digit,			// 1
	space,			// 2
	exclamationMark,// 3
	dollarSign,		// 4
	period,			// 5
	other,			// 6
	BACKUP			// 7 not an input, but a flag that tells the lexer when to back up 
};

class FSM {

private:

	const int NUM_OF_STATES = 9;
	const int NUM_OF_INPUTS = 8;
	const int NUM_OF_FINAL_STATES = 4;
	const int final_states[4] = { endIdentifier, endNumber, endComment, symbol };
	int state_transition_table[9][8] =
	{
		{ 1, 3, 0, 6, 8, 4, 8, 0 },
		{ 1, 1, 2, 2, 1, 2, 2, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 5, 3, 5, 5, 5, 4, 5, 0 },
		{ 5, 4, 5, 5, 5, 5, 5, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 6, 6, 6, 7, 6, 6, 6, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	};

public:

	int check_input(int state, int input) {
		return state_transition_table[state][input];
	}

	bool should_back_up(int curr_state) {
		if (state_transition_table[curr_state][BACKUP] == 1)
			return 1;
		else
			return 0;
	}

	string getTokenName(int state, string lexeme) {

		if (state == endIdentifier) {
			if (lexeme == "int" || lexeme == "float" || lexeme == "bool" || lexeme == "if" || lexeme == "else" ||
				lexeme == "then" || lexeme == "do" || lexeme == "while" || lexeme == "whileend" || lexeme == "do" ||
				lexeme == "doend" || lexeme == "for" || lexeme == "and" || lexeme == "or" || lexeme == "function") {
				return "KEYWORD";
			}
			else {
				return "IDENTIFIER";
			}
		}

		else if (state == endNumber) {
			return "NUMBER";
		}

		else if (state == endComment) {
			return "COMMENT";
		}

		else if (state == symbol) {
			if (lexeme == "*" || lexeme == "+" || lexeme == "-" || lexeme == "=" || lexeme == "/" ||
				lexeme == ">" || lexeme == "<" /*|| lexeme == "%"*/) {
				return "OPERATOR";
			}
			else if (lexeme == "'" || lexeme == "(" || lexeme == ")" || lexeme == "{" || lexeme == "}" ||
				lexeme == "[" || lexeme == "]" || lexeme == "," || lexeme == "." || lexeme == ":" || lexeme == ";" ||
				lexeme == "!") {
				return "SEPARATOR";
			}
			else
				return "OTHER";
		}
		else
			return "ERROR";
	}

	int char_to_input(char code) {
		if (isalpha(code))
			return letter;
		else if (isdigit(code))
			return digit;
		else if (isspace(code))
			return space;
		else if (code == '!')
			return exclamationMark;
		else if (code == '$')
			return dollarSign;
		else if (code == '.')
			return period;
		else
			return other;
	}

	bool is_final_state(int curr_state) {
		for (int i = 0; i < NUM_OF_FINAL_STATES; i++) {
			if (curr_state == final_states[i])
				return 1;
		}
		return 0;
	}
};
#endif