/*
 * File cpplib/ivrit.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/ivrit.h
 */

/* ivrit.h -- hebrew support unit */

#ifndef __IVRIT_H
#define __IVRIT_H

#include <ctype.h>
#include <cpplib/stringt.h>

const uchar	ALEPH=uchar(224), 
			BET = ALEPH+1,
			GIMEL = ALEPH+2,
			DALET = ALEPH+3,
			HEY = ALEPH+4,
			VAV = ALEPH+5,
			ZAYIN = ALEPH+6,
			HET = ALEPH+7,
			TET = ALEPH+8,
			YUD = ALEPH+9,
			KAFSOFIT = ALEPH+10,
			KAF = ALEPH+11,
			LAMED = ALEPH+12,
			MEMSOFIT = ALEPH+13,
			MEM = ALEPH+14,
			NUNSOFIT = ALEPH+15,
			NUN = ALEPH+16,
			SAMEK = ALEPH+17,
			AYN = ALEPH+18,
			PEYSOFIT = ALEPH+19,
			PEY = ALEPH+20,
			CADIQSOFIT = ALEPH+21,
			CADIQ = ALEPH+22,
			QUF = ALEPH+23,
			REJ = ALEPH+24,
			JIN = ALEPH+25,
			TAV = ALEPH+26;

const uint MSPR_OTIOT_BIVRIT = 27;

extern const char thiliot[];
extern const char sofiot[];

//#undef isspace
inline bool is_space (int c)
	{  return (c==0x20 || (0x09<=c&&c<=0x0d) ); }

inline bool is_digit (int c)
	{  return ('0'<=c && c<='9'); }

inline bool is_heb (uchar c)
	{ return (ALEPH<=c && c<=TAV); }


char to_sofit   (char ot);   // makes 'ot' an 'ot sofit'
char to_thilit	(char ot);	 // makes 'ot' an 'ot thilit'

inline bool is_sofit(char ot) { return (charinstring(ot,sofiot)); }
inline bool is_thilit(char ot) { return (charinstring(ot,thiliot)); }
inline bool is_simn_pisuq(char ot) { return (charinstring(ot,",;:.!?\"-–'()[]{}")); }

StringTemp num_to_heb (int num, Size maxsize=10);  // converts num to hebrew letters ('gimatria').  
int heb_to_num (CStr heb);  // returns the numerical value ('gimatria') of the word 'heb'. 

//StringTemp cgetsheb (int maxlen, CStr first_string="");
  /* gets a hebrew string (until EOLN) from console */
	/* backslash and ESC cancel string and restart.   */
	/* backspace erases last char.										*/



char ot_ivrit_ltatiq_ltini (uchar ot_ivrit);
uchar ot_ivrit_mtatiq_ltini (char ot_btatiq);



#endif
