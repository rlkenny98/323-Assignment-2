#pragma once
#include <cassert>
#include <iomanip>

using namespace std;

struct tokens {
	string token;
	string lexeme;
	tokens(string tok, string lex) {
		token = tok, lexeme = lex;
	}
};

bool analyze_syntax(std::vector<tokens>&, ofstream&);
int string_to_index(string);
void print_rule(string, string, ofstream&);

bool analyze_syntax(std::vector<tokens>& token_vect, ofstream& output_file) {
	// 2D std::vector to store our predictive table
	std::vector<std::vector<string>> predictive_table = {
		//			i    =     +     -      *     /      (       )      $ 
		/*S*/	{ "i=E", "", ""   , ""   , ""   , ""   , ""   , ""   , "" },
		/*E*/	{ "TQ" , "", ""   , ""   , ""   , ""   , "TQ" , ""   , "" },
		/*Q*/	{ ""   , "", "+TQ", "-TQ", ""   , ""   , ""   , "e"  , "e" },
		/*R*/	{ "FR" , "", ""   , ""   , ""   , ""   , "FR" , ""   , "" },
		/*T*/	{ ""   , "", "e"  , "e"  , "*FR", "/FR", ""   , "e"  , "e" },
		/*F*/	{ "i"  , "", ""   , ""   , ""   , ""   , "(E)", ""   , "" }
	};
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
			string top_of_stack;
			string token;

			cout << "------------------------------------------" << endl;
			cout << "Token: " << left << setw(25) << current->token <<
				"Lexeme: " << current->lexeme << endl;
			output_file << "------------------------------------------" << endl;
			output_file << "Token: " << left << setw(25) << current->token <<
				"Lexeme: " << current->lexeme << endl;

			while (!stack.empty())
			{
				top_of_stack = stack.back();
				if (current->token == "IDENTIFIER") { token = "i"; }
				else { token = current->lexeme; }

				if (top_of_stack == "i" || top_of_stack == "=" || top_of_stack == "+" ||
					top_of_stack == "-" || top_of_stack == "*" || top_of_stack == "/" ||
					top_of_stack == "(" || top_of_stack == ")" || top_of_stack == "$")
				{
					if (top_of_stack == token)
					{
						stack.pop_back();
						current++;

						if ((!stack.empty()) && current->lexeme != "$")
						{
							cout << endl << "------------------------------------------" << endl;
							cout << "Token: " << left << setw(25) << current->token <<
								"Lexeme: " << current->lexeme << endl;
							output_file << endl << "------------------------------------------" << endl;
							output_file << "Token: " << left << setw(25) << current->token <<
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
					assert(string_to_index(top_of_stack) != -1);
					cout << "Asserting top of stack index'" << top_of_stack << "' is working..\n";

					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					cout << "Some whacky shit going wrong here\n"
						<< "This is trying to call assert while converting token '" 
						<< token << "' to index: " << string_to_index(token) << endl;
					// String to index will depend on the predictive table
					assert(string_to_index(token) != -1);
					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					
					cout << "Finding rule"<< endl;
					string prod_rule = predictive_table[string_to_index(top_of_stack)][string_to_index(token)];

					if (!prod_rule.empty())
					{	cout << "Prod rule is empty"<< endl;
						print_rule(top_of_stack, prod_rule, output_file);

						stack.pop_back();
						while (!prod_rule.empty())
						{
							if (prod_rule != "e") { stack.push_back(string(1, prod_rule.back())); }
							prod_rule.pop_back();
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
			output_file << endl << "------------------------------------------" << endl;
			output_file << "Token: " << left << setw(25) << it->token <<
				"Lexeme: " << it->lexeme << endl;
			output_file << "<Empty> -> Epsilon" << endl << endl;

			string_line.clear();
		}
	}

	return true;
}

// THIS CHANGES WITH THE PREDICTIVE TABLE 
int string_to_index(string word)
{
	if (word == "S" || word == "i") { return 0; }
	else if (word == "E" || word == "=") { return 1; }
	else if (word == "Q" || word == "+") { return 2; }
	else if (word == "T" || word == "-") { return 3; }
	else if (word == "R" || word == "*") { return 4; }
	else if (word == "F" || word == "/") { return 5; }
	else if (word == "(") { return 6; }
	else if (word == ")") { return 7; }
	else if (word == "$") { return 8; }
	else return -1;
}

// THIS CHANGES WITH THE PREDICTIVE TABLE 
void print_rule(string statement, string prod_rule, ofstream& output_file)
{
	// Statement
	if (statement == "S")
	{
		if (prod_rule == "i=E")
		{
			cout << "<Statement> -> Identifier = <Expression>" << endl;
			output_file << "<Statement> -> Identifier = <Expression>" << endl;
		}
	}
	// Create a rule for Statement List
	// Expression
	else if (statement == "E")
	{
		if (prod_rule == "TQ")
		{
			cout << "<Expression> -> <Term> <Expression Prime>" << endl;
			output_file << "<Expression> -> <Term> <Expression Prime>" << endl;
		}
	}

	// Expression Prime
	else if (statement == "Q")
	{
		if (prod_rule == "+TQ")
		{
			cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
			output_file << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
		}
		else if (prod_rule == "-TQ")
		{
			cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
			output_file << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
		}
		if (prod_rule == "e")
		{
			cout << "<Expression Prime> -> <Epsilon>" << endl;
			output_file << "<Expression Prime> -> <Epsilon>" << endl;
		}
	}
	// Term
	else if (statement == "T")
	{
		if (prod_rule == "FR")
		{
			cout << "<Term> -> <Factor> <Term Prime>" << endl;
			output_file << "<Term> -> <Factor> <Term Prime>" << endl;
		}
	}
	// Term Prime
	else if (statement == "R")
	{
		if (prod_rule == "*FR")
		{
			cout << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
			output_file << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
		}
		else if (prod_rule == "/FR")
		{
			cout << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
			output_file << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
		}
		else if (prod_rule == "e")
		{
			cout << "<Term Prime> -> <Epsilon>" << endl;
			output_file << "<Term Prime> -> <Epsilon>" << endl;
		}
	}
	// Factor
	else if (statement == "F")
	{
		if (prod_rule == "i")
		{
			cout << "<Factor> -> Identifier" << endl;
			output_file << "<Factor> -> Identifier" << endl;
		}
		else if (prod_rule == "(E)")
		{
			cout << "<Factor> -> ( <Expression> )" << endl;
			output_file << "<Factor> -> ( <Expression> )" << endl;
		}
	}
}
