/*
 * File cpplib/ui.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/ui.h
 */

/* ui.h -- user interface basic routines */

/* 
	This unit defines the following functions:
		char getche_from (valid_chars);
		char prompt_from (prompt, valid_chars);
		char input_yn (prompt);
		char input_ync (prompt);
		char input_yncq (prompt);
		bool input_bool (prompt);
		StringTemp input_string (prompt, the_max_len, the_initial_string="", from_console=true);
		long input_long (prompt, from, to, from_console=true);   // returns a value between 'from' and 'to'.
		int input_int (prompt, from, to, from_console=true);
*/


#ifndef __UI_H
#define __UI_H


#include <cpplib/stringt.h>

void print_spaces (int num);

char getche_from (CStr valid_chars);
	/* gets a char from the keyboard, until a valid char is read.
			the case of the letter is like the case in valid_chars.     */

char prompt_from (CStr prompt, CStr valid_chars);

inline char input_yn (CStr prompt) { return prompt_from(prompt,"yn"); }
inline char input_ync (CStr prompt) { return prompt_from(prompt,"ync"); }
inline char input_yncq (CStr prompt) { return prompt_from(prompt,"yncq"); }
inline bool input_bool (CStr prompt) { return input_yn(prompt)=='y'; }

StringTemp input_string (CStr prompt, Size maxlen, CStr the_initial_string = "", bool from_console = true);

long input_long (CStr prompt, long from, long to, bool from_console=true);
inline int input_int (CStr prompt, long from, long to, bool from_console=true) { return int(input_long(prompt,from,to,from_console)); }
	/* returns an integer between 'from' and 'to' */

#endif
