/*
 * File morph/tavnit.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/tavnit.h
 */

/* tavnit.h */

#ifndef __TAVNIT_H
#define __TAVNIT_H

/********************************************************************************/
/*************       Tavnit                       *******************************/
/********************************************************************************/

#include <morph/morphinf.h>
#include <cpplib/stringt.h>
#include <ctype.h>

/*
	A Tavnit is used to match a word to a pattern, usually in verbs.
	A Tavnit has 2 parts: the ??? and the base.
	The 'constant' letters of the source are all letters that are non-lowercase, and the 'variables' are 
		lowercase English letters ('a','b','c').
	For example, if my  is "ITabcW" and my base is "HTabc" (Yitpa&alu ==> hitpa&el),
		then the routine 'match("ITB@LW",thebase) will return thebase="HTB@L".
	Another type of pattern-matching is for txiliot/siomot. For example, if my tavnit is "L?WT" and
		my base is "?H", then 'match("L&LWT",thebase)' will return thebase="&LH".
*/

#define EMISMATCH EILSEQ

class Tavnit {
		CStr mytavnit;  Size mytavnitlen;
		CStr mytxilit;  Size mytxilitlen;
		CStr mysiomet;  Size mysiometlen;

		MorphInfo myinfo;
		CStr mybase;
		Size mybaselen;

		uint mysug;  


	public:
		void set_tavnit (CStr thetavnit) {
			mytavnit = thetavnit;
			mytavnitlen = strlen(mytavnit);
			uint middle = charindex ('?',mytavnit);
			mytxilit = mytavnit;            mytxilitlen=middle;
			mysiomet = mytavnit+middle+1;   mysiometlen=mytavnitlen-middle-1;
		}
		void set_info (CStr thebase, HeleqDiber thehd) {
			myinfo.init(thebase,thehd);
			mybase = myinfo.baseword();
			mybaselen = strlen(mybase);
		}

		Tavnit () {}	
		Tavnit (CStr thetavnit, CStr thebase, HeleqDiber thehd, uint thesug=0) {
			set_tavnit(thetavnit);  
			set_info (thebase,thehd); 
			mysug=thesug;
		}

		CStr base() const { return mybase; }
		Size baselen() const { return mybaselen; }
		uint sug() const{ return mysug; }
		Meen meen() const{ return myinfo.meen(); }	
		Mispar mispar() const{ return myinfo.mispar(); }	
		Guf guf() const{ return myinfo.guf(); }	
		Smikut smikut() const{ return myinfo.smikut(); }	
		Zman zman() const{ return myinfo.zman(); }	
		Meen meen_siomet() const{ return myinfo.meen_siomet(); }	
		Mispar mispar_siomet() const{ return myinfo.mispar_siomet(); }	
		Guf guf_siomet() const{ return myinfo.guf_siomet(); }	
		MorphInfoCR info() const { return myinfo; }
		MorphInfoR info() { return myinfo; }

		status match (CStr theword, StringTemp& thebase) const;
		bool match (CStr theword) const;
		void write (ostream& out, Format format="") const;
		status read (istream& in, Format format="");

	private:
		status match_and_set_variables (CStr thetavnit, Size thelen, CStr theword) const;  // sets the variables
		status match_variables (CStr theword, StringTemp& thebase) const ;
		status match_txilit_siomet (CStr theword, StringTemp& thebase) const;
};			

typedef Tavnit& TavnitR;
typedef const Tavnit& TavnitCR;


inline void write (ostream& out, TavnitCR thetavnit, Format format="") { thetavnit.write(out,format); }
inline status read(istream& in, TavnitR thetavnit, Format format="") { return thetavnit.read(in,format); }

inline void duplicate (Tavnit& to, TavnitCR from) { to=from; }
inline void free (Tavnit&) {  }


#endif
