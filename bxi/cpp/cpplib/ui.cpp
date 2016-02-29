/*
 * File cpplib/ui.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/ui.cpp
 */

/* ui.cpp */

#ifndef UNIX
#include <conio.h>
#define cputs _cputs
#else
#define cputs puts
#endif

#include <iostream.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>	
#include <cpplib/ui.h>
#include <cpplib/stringt.h>

void print_spaces (int num) {
  for (int i=1; i<=num; i++)  cout << ' ';
}


char getche_from (CStr valid_chars) {
	char c;
	do {
#ifdef UNIX
  cin >> c;
#else
		c = getche();
#endif
		if (strchr(valid_chars,tolower(c)))   return tolower(c);
		if (strchr(valid_chars,toupper(c)))   return toupper(c);
	} while(1);   
}

char prompt_from (CStr prompt, CStr valid_chars) 
{
 cout << prompt << " [" << valid_chars << "]?";
	return getche_from (valid_chars);
}




StringTemp input_string_from_console (CStr prompt, Size maxlen, CStr the_initial_string) {
	StringTemp result(maxlen);
	cputs(prompt); cputs(": ");
	result.read_from_console(the_initial_string);
	cputs("\n");
	return result;
}

StringTemp input_string (CStr prompt, Size maxlen, CStr the_initial_string,  bool from_console) {
	if (from_console) return input_string_from_console (prompt,maxlen,the_initial_string);
	StringTemp result(maxlen);
	cout << prompt << ": " << flush;
	//if (ignore_one_char)  { cin.ignore();  cout << "\b";  }
	result.readline(cin,"\n");
	cout << endl;
	return result;
}



long input_long (CStr prompt, long from, long to, bool console) 
{
	char* endptr;
	StringTemp numstr;
	long num;
	for (;;) {
		numstr = input_string(prompt,10,"",console);
		num = strtol(numstr.str,&endptr,10);
		if (*endptr != '\0')
			cout << "wrong number format " << numstr << endl;
		else if (num<from || to<num)
			cout << "number out of range " << numstr << endl;
		else return num;
	}
}
