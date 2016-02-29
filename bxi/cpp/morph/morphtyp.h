/*
 * File morph/morphtyp.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphtyp.h
 */

/* morphtyp.h --   morphological basic types */

#ifndef __MORPHTYP_H
#define __MORPHTYP_H

#include <cpplib/typedefs.h>

/***************************************************************************************/
/*      Types for field in the morphological-info structure:						   */
/*			Meen(gender), Mispar(number), Smikut(status),							   */
/*			HeleqDiber(category), Guf(person), Zman(tense),							   */
/*			Binyan.																	   */
/***************************************************************************************/

// this constant is used in all fields.
#define NONE 0

// part of speech
enum HeleqDiber {         
	NO_HD=0,
	POAL=1,			// verb
	ECEM,			// noun
	TOAR,			// adjective
	MISPAR,			// number
	MILAT_YAXAS,	// preposition
	MILAT_GUF,		// pronoun
	MILAT_JEELA=7,	// question word
	MILAT_XIBUR=-8,	// conjunction
	MILIT,			// particle
	TOAR_POAL,		// adverb
	POAL_EZER,		// modal
	JEM_PRATI       // proper noun
}; 

// This enum is for verbs only
enum Zman {         // tense
	NO_ZMAN=0,
	AVAR=1,		    // past
	HOWE=2,			// present
	ATID=3,			// future
	HOWE_SAVIL=-4,  // present passive: "PA&UL".
	CIWUI=-3,			// imperative
	MAQOR=-2  };	    // infinitive



// these constant is for Meen, Mispar, Smikut, Guf, Yidua.
#define BOTH -2

typedef int Meen;
#define ZAKAR 1
#define NQEVA -1

typedef int Mispar;
#define YAXID 1
#define RABIM -1

typedef int Smikut;
#define NIFRAD 1
#define NISMAK -1

typedef int Guf;
#define GUF1 1
#define GUF2 2
#define GUF3 3




enum Jiabud {
	NO_JIABUD=0,
	JIABUD_JE=1,
	JIABUD_KJE=2,
	JIABUD_LKJE=3,
	JIABUD_JEKJE=-4,
	JIABUD_HA=-3,
	ANY_JIABUD=-2,
	JIABUD_JEHA=-1
};

enum OtiotYaxas {
	NO_OY=0,
	OY_BET=1,
	OY_MEM=2,
	OY_LAMED=3,
	OY_KAF=-1,
	ANY_OY=-2
};

typedef int Yidua;
#define LOMEYUDA 1
#define KNMEYUDA -1


/*********************************************************************************/
/*******        IO ROUTINES          *********************************************/
/*********************************************************************************/

/* routines for output */

char hd2char(HeleqDiber hd);
char yidua2char(Yidua y);
char jiabud2char(Jiabud y);
char otiotyaxas2char(OtiotYaxas y);
char vav2char(bool y);
char zman2char(Zman y);
char meen2char(Meen y);
char mispar2char(Mispar y);
char smikut2char(Smikut y);
char guf2char(Guf y);

CStr hd2str(HeleqDiber hd);
CStr yidua2str(Yidua y);
CStr jiabud2str(Jiabud y);
CStr otiotyaxas2str(OtiotYaxas y);
CStr zman2str(Zman y);
CStr meen2str(Meen y);
CStr mispar2str(Mispar y);
CStr smikut2str(Smikut y);
CStr guf2str(Guf y);


/* routines for input */

HeleqDiber char2hd (char c);
Yidua char2yidua(char c);
Jiabud char2jiabud(char c);
OtiotYaxas char2otiotyaxas(char c);
bool char2vav (char c);
Zman char2zman(char c);
Meen char2meen (char c);
Mispar char2mispar (char c);
Smikut char2smikut (char c);
Guf char2guf (char c);

CStr maxrozet_jiabud (Jiabud thejiabud);
CStr maxrozet_otiotyaxas (OtiotYaxas theotiotyaxas);
CStr maxrozet_siomet_jayakut (Meen themeen, Mispar themispar, Guf theguf);
CStr maxrozet_kinui_guf (Meen themeen, Mispar themispar, Guf theguf);
CStr maxrozet_ribui (Meen themeen, Mispar themispar);



/* functions for comparing values */ 

inline bool identical_meen(Meen a, Meen b) { return a==b || (a==BOTH && b!=NONE) || (b==BOTH && a!=NONE); }
inline bool identical_mispar(Mispar a, Mispar b) { return a==b || (a==BOTH && b!=NONE) || (b==BOTH && a!=NONE); }
inline bool identical_smikut(Smikut a, Smikut b) { return a==b || (a==BOTH && b!=NONE) || (b==BOTH && a!=NONE); }
inline bool identical_guf(Guf a, Guf b) { return a==b || (a==BOTH && b!=NONE) || (b==BOTH && a!=NONE); }
inline bool identical_zman(Zman a, Zman b) { return a==b; }
inline bool identical_yidua(Yidua a, Yidua b) { return a==b ||  (a==BOTH && b!=NONE) || (b==BOTH && a!=NONE); }
inline bool identical_otiotyaxas(OtiotYaxas a, OtiotYaxas b) { return a==b || (a==ANY_OY && b!=NO_OY) || (b==ANY_OY && a!=NO_OY); }
inline bool identical_jiabud(Jiabud a, Jiabud b) { return a==b || (a==ANY_JIABUD && b!=NO_JIABUD) || (b==ANY_JIABUD && a!=NO_JIABUD); }
inline bool identical_vav(bool a, bool b) { return (a&&b) || (!a && !b); }
inline bool identical_heleqdiber(HeleqDiber a, HeleqDiber b) { return a==b; }
inline bool identical_baseword (CStr a, CStr b) { return strcmp(a,b)==0; }


/* functions for determining whether a certain attribute is relevalt for the given HeleqDiber. */

inline bool yej_meen(HeleqDiber hd) { return hd==POAL||hd==POAL_EZER||hd==ECEM||hd==TOAR||hd==MISPAR||hd==MILAT_GUF||hd==JEM_PRATI; }
inline bool yej_mispar(HeleqDiber hd) { return hd==POAL||hd==POAL_EZER||hd==ECEM||hd==TOAR||hd==MISPAR||hd==MILAT_GUF; }
inline bool yej_smikut(HeleqDiber hd) { return hd==ECEM||hd==TOAR||hd==MISPAR; }
inline bool yej_guf(HeleqDiber hd) { return hd==POAL||hd==POAL_EZER||hd==MILAT_GUF; }
inline bool yej_zman(HeleqDiber hd) { return hd==POAL||hd==POAL_EZER; }
inline bool yej_yidua(HeleqDiber hd) { return hd==ECEM||hd==TOAR||hd==MISPAR||hd==MILAT_GUF; }
inline bool yej_otiotyaxas(HeleqDiber hd) { return hd==ECEM||hd==TOAR||hd==MISPAR||hd==MILAT_GUF||hd==JEM_PRATI; }
inline bool yej_siomet(HeleqDiber hd) { return hd==ECEM||hd==MISPAR||hd==MILAT_YAXAS; }

#endif

