/*
 * File cpplib/cmdline1.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/cmdline1.cpp
 */

/* cmdline1.cpp -- command-line handling routines */

//#include <process.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <cpplib/cmdline1.h>
#include <cpplib/klli1.h>

#define OPTION_INDICATOR '-'

const char* synopsis="";

void set_synopsis (const char* s)   { synopsis=s; }

char* options[26];   // saves one option for each letter A-Z
bool switches[26];

unsigned variables_num;
char* *variables;	   // variable number of variables;


void parse_command_line (int argc, char* argv[],
	unsigned min_num_of_variables, unsigned max_num_of_variables,
	const char* legal_option_chars, const char* legal_switch_chars)
{
	if (max_num_of_variables>0) {
		variables = new char* [max_num_of_variables];  assert(variables!=NULL); 
	}

	variables_num=0;
	int i;
	for (i=0; i<25; i++)  options[i]=NULL;   // init option array

	for (i=1; i<argc; i++)  {
		char* cur_arg = argv[i];

		/* Help char: */
		if (cur_arg[0]=='?' || cur_arg[1]=='?') {
			cout << endl << "SYNOPSIS: " << synopsis << endl;
			exit (0);
		}

		/* Parse an option or a switch: */
		if (cur_arg[0]==OPTION_INDICATOR)  {
			char option_char = toupper(cur_arg[1]);
			if (option_char < 'A' || 'Z' < option_char)
				exit_with_error ("Illegal option",cur_arg,synopsis);

			/* parse an option: */
			if (strchr(legal_option_chars,option_char) || strchr(legal_option_chars,tolower(option_char))) {
					if ( strlen(cur_arg) > 2 )							// option string is adjacent to option char
						options[option_char-'A'] = cur_arg+2;
					else {																	// option string is next argument
						if (i+1 < argc) {
							options[option_char-'A'] = argv[i+1];
							++i;
						}
						else
							exit_with_error ("Missing operand to option",cur_arg,synopsis);
					}
			}

			else if (strchr(legal_switch_chars,option_char) || strchr(legal_switch_chars,tolower(option_char))) {
					switches[option_char-'A'] = true;
			}

			else
				exit_with_error ("Illegal option",cur_arg,synopsis);
		}

		/* parse a variable: */
		else {
			if (variables_num >= max_num_of_variables)
				exit_with_error ("Too many arguments","",synopsis);
			else
				variables[variables_num++] = cur_arg;
		}
	}

	if (variables_num < min_num_of_variables)
		exit_with_error ("Too few arguments","",synopsis);
}


char* variable (unsigned var_num)
{
	if (variables_num<=var_num)  return NULL;
	else  return variables[var_num];
}

char* option (char option_char)
{
	return options[ toupper(option_char)-'A' ];
}

bool swtch (char c)
{
	return switches[ toupper(c) - 'A' ];
}

static int string_to_int (char* var, int from, int to)
{
	if (var==NULL)
		exit_with_error ("missing number argument","");
	char* endptr;
	int num = strtol(var,&endptr,10);
	if (*endptr != '\0')
		exit_with_error ("wrong number format",var);
	if (num<from || to<num)
		exit_with_error ("number out of range",var);
	return num;
}

int variable_as_int (unsigned var_num, int from, int to)
{
	return string_to_int ( variable(var_num), from, to);
}

int option_as_int (char option_char, int from, int to)
{
	return string_to_int ( option(option_char), from, to);
}


/*************************************************************************/
/***              Open routines           ********************************/
/*************************************************************************/

void open_infile (CStr filename, ifstream& stream, open_mode or_mode/*=0*/) {
	stream.clear();
	if (open(filename,stream,or_mode)!=OK)
		exit_with_error ("can't open input file ",filename); 
}

void open_outfile (CStr filename, ofstream& stream, open_mode or_mode/*=ios::app*/) { 
	stream.clear();
	if (open(filename,stream,or_mode)!=OK)
		exit_with_error ("can't open output file ",filename); 
}

