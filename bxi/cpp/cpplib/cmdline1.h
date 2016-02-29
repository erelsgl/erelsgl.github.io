/*
 * File cpplib/cmdline1.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/cmdline1.h
 */

/* cmdline1.h -- command-line handling routines */
#ifndef __CMDLINE1_H
#define __CMDLINE1_H

#include <cpplib/typedefs.h>
#include <fstream.h>
//#include <fstream>
//#include <iosfwd>

void set_synopsis (CStr s);
/*
	used to set a string which is printed in case of an error,
	to help the user understand what he has done wrong.
*/


/*
	The following functions suppose that the command line contains "options" (strings preceded
		with /x , when x is an "option-char"), "switches" (boolean variables
		controlled by an argument of the form /x), and "variables" (other strings,
		usually filenames). Also, ? in the command line means to display the
		synopsis.
	There should be a space between the '/x' and the option after it, for
		example:   /f MYFILE.C
*/

void parse_command_line (int argc, char* argv[],
	unsigned min_num_of_variables, unsigned max_num_of_variables,
	CStr legal_option_chars, CStr legal_switch_chars);
/*
	The parse_command_line function reads all legal options and all variables
		from the command line, and stores them for later use (using the
		option(), variable() and swtch() functions below).
	It also handles error conditions such as illegal options or too many
		variables and special conditions like the ? char.
	In the option-chars, the case is not important, as is common in DOS.
	Usage example:
		parse_command_line (argc,argv, 2,2, "fg","")
			means that the command line must have 2 variables, and can have either
			the '/f' or the '/g' option, and no switches.
*/

/* The following are defined only after parse_command_line is called: */
char* variable (unsigned var_num /* begins from 0 */);
	// returns a char* pointing the var_num's var as a string
	// returns NULL if there were less than var_num+1 arguments.
int variable_as_int (unsigned var_num, int from, int to);
	// same as above, but also converts the variable to integer and exits with error if
	// it fails, or if the result is not between 'from' to 'to'.
	// For example, variable_as_int (2,0,10) returns the second variable,
	// 	which must be an integer between 0 and 10.
char* option (char option_char);  // returns NULL if there was no option with this char.
int option_as_int (char option_char, int from, int to);
bool swtch (char switch_char);

typedef short open_mode;

#ifdef UNIX
void open_infile (CStr filename, ifstream& stream, open_mode or_mode=1);
void open_outfile (CStr filename, ofstream& stream, open_mode or_mode=8);
#else
void open_infile (CStr filename, ifstream& stream, open_mode or_mode=ios::in);
void open_outfile (CStr filename, ofstream& stream, open_mode or_mode=ios::app);
#endif
/*
	These routines try to open the file, and exit with an error message
		if this fails.
*/

#endif

