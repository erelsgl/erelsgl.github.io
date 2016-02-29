
/*
 * File morph/mip.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/mip.h
 */

/* mip.h --   morphological info pattern */

#ifndef __MIP_H
#define __MIP_H

#include <morph/morphinf.h>

// DONTCARE is the character that means that the field it represents is not significant for the pattern-matching (it is like a wildcard).
#define DONTCARE '='

/***************************************************************************************/
/*      MorphInfoPattern                                                               */
/***************************************************************************************/

/*
	In general, MorphInfoPattern has the same fields as MorphInfo, only that any field
		can be 'xajuv' (significant) or 'loxajuv' (insignificant). 
	This information is used in the 'match' function: only the significant fields will 
		be compared.
	If a certain field is 'loxajuv', it can still be 'carik_lhatim'. This means that if 
		there are 2 adjacent MorphInfoPatterns, that field should be the same in the 2 
		MorphInfo's that are matched to it (it is used, for example, when matching
		ECEM with TOAR of the same  meen, mispar and yidua).
*/


class MorphInfoPattern;
typedef const MorphInfoPattern& MorphInfoPatternCR;
class MorphInfoPattern: public MorphInfo {
	public:
		bool baseword_xajuv:1, 
			 heleqdiber_xajuv:1,
			 vav_xajuv:1,
			 jiabud_xajuv:1,
			 otiotyaxas_xajuv:1,
			 yidua_xajuv:1,
			 meen_xajuv:1,
			 mispar_xajuv:1,
			 guf_xajuv:1,
			 smikut_xajuv:1,
			 zman_xajuv:1,
			 meen_siomet_xajuv:1,
			 mispar_siomet_xajuv:1,
			 guf_siomet_xajuv:1;

		bool yidua_carik_lhatim:1,
			 meen_carik_lhatim:1,
			 mispar_carik_lhatim:1,
			 guf_carik_lhatim:1;

	public:
		/*----- queries -----*/
		bool match (MorphInfoCR theinfo) const;   // does the info matches the pattern?

		/*----- init -----*/
		void klum_lo_xajuv();
		void init ();
		void set_hakol_xajuv(MorphInfoCR theinfo);
		void set(MorphInfoCR theinfo);
		void set_hatxala_lo_xajuva(MorphInfoCR theinfo);
		void set_sof_lo_xajuv(MorphInfoCR theinfo);
		void set_klum_lo_xajuv(MorphInfoCR theinfo);
		void set_baseword (MorphInfoCR theinfo);
		void set_heleqdiber (MorphInfoCR theinfo);
		void set_txiliot (MorphInfoCR theinfo);
		void set (char* the_pattern); /* sets according to a pattern in the usual I/O format */			

		MorphInfoPattern() { init(); }			
		MorphInfoPattern(char* the_pattern) { init(); set(the_pattern); } 
		MorphInfoPattern(MorphInfoCR theinfo) { init(); set(theinfo); }			

		/*----- operations -----*/
		void xituk (MorphInfoPatternCR a, MorphInfoPatternCR b);
			// puts in this MIP the properties that are common to both a and b.
		void xituk (MorphInfoCR a);
			// intersects this MIP with a.


		/*----- IO -----*/

		void ktov_barur (ostream& out, bool ktov_hatama) const;  
		void ktov_cafuf (ostream& out) const;  

		status read (istream& in);
		MorphInfoPattern (int i): MorphInfo(i) {}
		bool operator== (int i) const { return MorphInfo::operator==(i); }
};

void hattel (MorphInfo& theinfo, MorphInfoPatternCR thepattern);
	// mxa$$eb ^et ha-hei@el $ell "theinfo" &al "thepattern", klomar:
	// m$anne ^et koll ha-tkunot $ell "theinfo" kak $e-yihyu $awot la-tkunot ha-xa$ubot 
	// $ell "thepattern"


void write (ostream& out, MorphInfoPatternCR theinfo, Format format="");
status read (istream& in, MorphInfoPattern& theinfo, Format format="");

inline bool identical (MorphInfoPatternCR a, MorphInfoPatternCR b) {
	return identical((MorphInfoCR)a,(MorphInfoCR)b); }
inline short compare (MorphInfoPatternCR a, MorphInfoPatternCR b) { return identical(a,b)? 0: 1; }
inline long hashvalue (MorphInfoPatternCR a) { return hashvalue(a.baseword()); }

inline void duplicate (MorphInfoPattern& to, MorphInfoPatternCR from) { to = from; }
inline void free (MorphInfoPattern) {}


DEFINE_INLINE_IO_OPERATORS (MorphInfoPattern)
//DEFINE_INLINE_RELATIVE_OPERATORS(MorphInfoPattern)


extern MorphInfoPattern MIP_KOLEL_HAKOL;


#endif

