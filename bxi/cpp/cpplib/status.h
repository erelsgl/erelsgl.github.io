/*
 * File cpplib/status.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/status.h
 */

/* status.h -- a unit to handle errors and return statuses */
/*
	Error handling is based on a 'status'-value returned from procedures. 
	Each procedure returns a status-value 'OK' if the procedure execution is successful, 
		and if not -- it returns a status-value according to the type of error
		(for example:	ENOMEM == not enough memory;
						ENOTFOUND == element not found;
						...
		)

  This unit defines the following macros for error-handling (see their description below):
  DO,		DOr,		DOx;
  DOEOF,	DOEOFr,		DOEOFx;
  DOP,		DOPr,		DOPx;
  TESTCHAR
*/

#ifndef __STATUS_H
#define __STATUS_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* 'status' is used as a return types from most functions in CPPLIB */
typedef short status;

/* Additional Error Codes */
#define OK			0

#define EEMPTY      43
#define ENOTFOUND   44
#define EBADFORMAT  45
#define EFULL       46

#define print_on_error(p) \
	cerr << endl << "function " << #p << " returned status " << the_status << " (" << error_messages[the_status] << ") file " << __FILE__ << " line " << __LINE__ << endl;

/***************************************************************
 * The following macros are used to DO (execute) a command,
 *	and then test its status and react accordingly.
 *  DOx(action) does the action, and exits if the result is not ok.
 *  DOr(action) does the action, and returns the same status if
 *		the result is not OK.
 *  DO(action) does the action, and returns without returning
 *		any value if the result is not OK.
 ***************************************************************/


#  define DOx(action)  { \
	the_status=action;\
	if(the_status!=OK) {\
		print_on_error(action);  \
		exit(the_status);\
    }\
}



#  define DOr(action)  { \
	the_status=action;\
	if(the_status!=OK) \
		return the_status; \
}



#  define DO(action)  { \
	the_status=action;\
	if(the_status!=OK) \
		return; \
}


/***************************************************************
 * The following macros are used to DO (execute) a command in a for-loop that reads from an input stream.
 * In such loops, it is required to break out of the loop if some function returns EOF (end-of-file). 
 *  DOEOF(action) does the action, and then: continues if the result is OK, 
 *		and breaks the loop if the result means EOF.
 *  DOEOFx(action) does the action, and then: continues if the result is OK, 
 *		breaks the loop if the result means EOF, and exits if the result is not OK and not EOF.
 *  DOEOFr(action) does the action, and then: continues if the result is OK, 
 *		breaks the loop if the result means EOF, and returns the same result to the calling routine if the result is not OK and not EOF.
 ***************************************************************/

#  define DOEOF(action)  { \
	the_status=action;\
	if (the_status==EOF) break;\
}


#  define DOEOFx(action)  { \
	the_status=action;\
	if (the_status==EOF) break;\
	else if (the_status!=OK) {\
		print_on_error(action);  \
		exit(the_status);\
    }\
}


#  define DOEOFr(action)  { \
	the_status=action;\
	if (the_status==EOF) break;\
	else if (the_status!=OK) \
		return the_status; \
}





/***************************************************************
 * The following macros are used to DO (execute) a command which returns a poiner
 *	(in particular - a pointer assignment, such as:
 *		my_link = new Link;
 *  )
 *	and then test its status and react accordingly.
 *  DOPx(action) does the action, and exits if the result is NULL (meaning 'not enough memory').
 *  DOPr(action) does the action, and returns ENOMEM ('not enough memory') if the result is NULL.
 *  DOP(action) does the action, and returns without returning any value if the result is NULL.
 *****************************************/


#  define DOPx(action)  { \
	the_pointer=(action);\
	if(the_pointer==NULL) {\
		print_on_error(action);  \
		exit(ENOMEM);\
    }\
}



#  define DOPr(action)  { \
	the_pointer=(action);\
	if(the_pointer==NULL) \
		return ENOMEM; \
}



#  define DOP(action)  { \
	the_pointer=(action);\
	if(the_pointer==NULL) \
		return; \
}



/* 
	The following macro is useful in input routines that expect a specific character.
	If the next character in the input won't be the character expected, the function
	will return EBADFORMAT.
*/
#define TESTCHAR(in,c) { \
	if ( in.peek() != c ) \
		return EBADFORMAT; \
	else \
		in.get(); \
}



/************ global symbols used for error handling ***********************/

extern status the_status;
extern void* the_pointer;
extern char* error_messages[];
/* These symbols are defined in klli1.cpp */

#endif
