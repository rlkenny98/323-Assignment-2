#pragma once
//This file defines the State Machine class 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum State {
	STARTING_STATE,				// 0
	IN_IDENTIFIER,				// 1
	END_OF_IDENTIFIER,			// 2 final state
	IN_NUMBER,					// 3
	IN_FLOAT,					// 4
	END_OF_NUMBER,				// 5 final state
	IN_COMMENT,					// 6
	END_OF_COMMENT,				// 7 final state
	SYMBOLS						// 8 final state
};

enum Input {
	LETTER,			// 0
	DIGIT,			// 1
	SPACE,			// 2
	EXCLAMATION,	// 3
	DOLLAR_SIGN,	// 4
	PERIOD,			// 5
	OTHER,			// 6
	BACKUP			// 7 not an input, but a flag that tells the lexer when to back up 
};

class StateMachine {

private:

	const int NUM_OF_STATES = 9;
	const int NUM_OF_INPUTS = 8;
	const int NUM_OF_FINAL_STATES = 4;
	const int final_states[4] = { END_OF_IDENTIFIER, END_OF_NUMBER, END_OF_COMMENT, SYMBOLS };
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

		if (state == END_OF_IDENTIFIER) {
			if (lexeme == "int" || lexeme == "float" || lexeme == "bool" || lexeme == "if" || lexeme == "else" ||
				lexeme == "then" || lexeme == "do" || lexeme == "while" || lexeme == "whileend" || lexeme == "do" ||
				lexeme == "doend" || lexeme == "for" || lexeme == "and" || lexeme == "or" || lexeme == "function") {
				return "KEYWORD";
			}
			else {
				return "IDENTIFIER";
			}
		}

		else if (state == END_OF_NUMBER) {
			return "NUMBER";
		}

		else if (state == END_OF_COMMENT) {
			return "COMMENT";
		}

		else if (state == SYMBOLS) {
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
			return LETTER;
		else if (isdigit(code))
			return DIGIT;
		else if (isspace(code))
			return SPACE;
		else if (code == '!')
			return EXCLAMATION;
		else if (code == '$')
			return DOLLAR_SIGN;
		else if (code == '.')
			return PERIOD;
		else
			return OTHER;
	}

	bool is_final_state(int curr_state) {
		for (int i = 0; i < NUM_OF_FINAL_STATES; i++) {
			if (curr_state == final_states[i])
				return 1;
		}
		return 0;
	}

};