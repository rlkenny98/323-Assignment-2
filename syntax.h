#pragma once
#include <cassert>
#include <iomanip>
#include <iostream>

using namespace std;

struct tokens {
	string token;
	string lexeme;
	tokens(string tok, string lex) {
		token = tok, lexeme = lex;
	}
};

// Changed function names here already
bool syntaxAnalyze(std::vector<tokens>&, ofstream&);
int stringIndexed(string);
void ruleOutput(string, string, ofstream&);
// Changed variable names

bool syntaxAnalyze(std::vector<tokens>& token_vect, ofstream& opFile) {
	// 2D vector to store our predictive table

	//!!!!!!!!! FOCUS ON CHANGING PREDICTIVE TABLE RULES !!!!!!!!!!!!!!!!!!!
	std::vector<std::vector<string>> predictive_table = {
		//			i    =     +     -      *     /      (       )      $ 
		/*S*/	{ "i=E", "", ""   , ""   , ""   , ""   , ""   , ""   , "" },
		/*E*/	{ "TQ" , "", ""   , ""   , ""   , ""   , "TQ" , ""   , "" },
		/*Q*/	{ ""   , "", "+TQ", "-TQ", ""   , ""   , ""   , "e"  , "e" },
		/*R*/	{ "FR" , "", ""   , ""   , ""   , ""   , "FR" , ""   , "" },
		/*T*/	{ ""   , "", "e"  , "e"  , "*FR", "/FR", ""   , "e"  , "e" },
		/*F*/	{ "i"  , "", ""   , ""   , ""   , ""   , "(E)", ""   , "" }
	};
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	std::vector<string> stack;
	std::vector<tokens> string_line;
	cout << "Hello from the syntax analyzer " << endl;
	for (std::vector<tokens>::iterator it = token_vect.begin(); it != token_vect.end(); it++)
	{	
		// Only works if the input has semicolons
		if (it->lexeme != ";")
		{	cout << "we aint a semicolon"<< endl;
			string_line.push_back(tokens(it->token, it->lexeme));
		}

		else
		{
			stack.push_back("$");
			stack.push_back("S");

			string_line.push_back(tokens("$", "$"));

			std::vector<tokens>::iterator current = string_line.begin();
			string TOS; // top of stack 
			string token;

			cout << "------------------------------------------" << endl;
			cout << "Token: " << left << setw(25) << current->token <<
				"Lexeme: " << current->lexeme << endl;
			opFile << "------------------------------------------" << endl;
			opFile << "Token: " << left << setw(25) << current->token <<
				"Lexeme: " << current->lexeme << endl;

			while (!stack.empty())
			{	cout << "Stack is not empty" << endl;
				TOS = stack.back();
				if (current->token == "IDENTIFIER") { token = "i"; }
				else { token = current->lexeme; }

				if (TOS == "i" || TOS == "=" || TOS == "+" ||
					TOS == "-" || TOS == "*" || TOS == "/" ||
					TOS == "(" || TOS == ")" || TOS == "$")
				{
					if (TOS == token)
					{	cout << "Top of stack is a token\n";
						stack.pop_back();
						current++;

						if ((!stack.empty()) && current->lexeme != "$")
						{
							cout << endl << "------------------------------------------" << endl;
							cout << "Token: " << left << setw(25) << current->token <<
								"Lexeme: " << current->lexeme << endl;
							opFile << endl << "------------------------------------------" << endl;
							opFile << "Token: " << left << setw(25) << current->token <<
								"Lexeme: " << current->lexeme << endl;
						}
					}

					else
					{	//Error in syntax
						return false;
					}
				}

				else
				{
					assert(stringIndexed(TOS) != -1);
					cout << "Asserting top of stack index'" << TOS << "' is working..\n";

					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					cout << "Some whacky shit going wrong here\n"
						<< "This is trying to call assert while converting token '" 
						<< token << "' to index: " << stringIndexed(token) << endl;
					// String to index will depend on the predictive table
					assert(stringIndexed(token) != -1);
					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					
					cout << "Finding rule"<< endl;
					string prodRule = predictive_table[stringIndexed(TOS)][stringIndexed(token)];

					if (!prodRule.empty())
					{	cout << "Prod rule is empty"<< endl;
						ruleOutput(TOS, prodRule, opFile);

						stack.pop_back();
						while (!prodRule.empty())
						{
							if (prodRule != "e") { stack.push_back(string(1, prodRule.back())); }
							prodRule.pop_back();
						}
					}

					else
					{	// Error in syntax
						return false;
					}
				}
			}

			cout << endl << "------------------------------------------" << endl;
			cout << "Token: " << left << setw(25) << it->token <<
				"Lexeme: " << it->lexeme << endl;
			cout << "<Empty> -> Epsilon" << endl << endl;
			opFile << endl << "------------------------------------------" << endl;
			opFile << "Token: " << left << setw(25) << it->token <<
				"Lexeme: " << it->lexeme << endl;
			opFile << "<Empty> -> Epsilon" << endl << endl;

			string_line.clear();
		}
	}

	return true;
}

// Changed this already 
int stringIndexed(string word)
{
	if (word == "S" || word == "i") 
	{ return 0; }
	else if (word == "E" || word == "=") 
	{ return 1; }
	else if (word == "Q" || word == "+") 
	{ return 2; }
	else if (word == "T" || word == "-") 
	{ return 3; }
	else if (word == "R" || word == "*") 
	{ return 4; }
	else if (word == "F" || word == "/") 
	{ return 5; }
	else if (word == "(") 
	{ return 6; }
	else if (word == ")") 
	{ return 7; }
	else if (word == "$") 
	{ return 8; }
	else return -1;
}

// Changed variable names here already
void ruleOutput(string statement, string prodRule, ofstream& opFile)
{
	// Statement
	if (statement == "S")
	{
		if (prodRule == "i=E")
		{
			cout << "<Statement> -> Identifier = <Expression>" << endl;
			opFile << "<Statement> -> Identifier = <Expression>" << endl;
		}
	}
	// Create a rule for Statement List
	// Expression
	else if (statement == "E")
	{
		if (prodRule == "TQ")
		{
			cout << "<Expression> -> <Term> <Expression Prime>" << endl;
			opFile << "<Expression> -> <Term> <Expression Prime>" << endl;
		}
	}

	// Expression Prime
	else if (statement == "Q")
	{
		if (prodRule == "+TQ")
		{
			cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
			opFile << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
		}
		else if (prodRule == "-TQ")
		{
			cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
			opFile << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
		}
		if (prodRule == "e")
		{
			cout << "<Expression Prime> -> <Epsilon>" << endl;
			opFile << "<Expression Prime> -> <Epsilon>" << endl;
		}
	}
	// Term
	else if (statement == "T")
	{
		if (prodRule == "FR")
		{
			cout << "<Term> -> <Factor> <Term Prime>" << endl;
			opFile << "<Term> -> <Factor> <Term Prime>" << endl;
		}
	}
	// Term Prime
	else if (statement == "R")
	{
		if (prodRule == "*FR")
		{
			cout << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
			opFile << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
		}
		else if (prodRule == "/FR")
		{
			cout << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
			opFile << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
		}
		else if (prodRule == "e")
		{
			cout << "<Term Prime> -> <Epsilon>" << endl;
			opFile << "<Term Prime> -> <Epsilon>" << endl;
		}
	}
	// Factor
	else if (statement == "F")
	{
		if (prodRule == "i")
		{
			cout << "<Factor> -> Identifier" << endl;
			opFile << "<Factor> -> Identifier" << endl;
		}
		else if (prodRule == "(E)")
		{
			cout << "<Factor> -> ( <Expression> )" << endl;
			opFile << "<Factor> -> ( <Expression> )" << endl;
		}
	}
}
