/*
 * File morph/tavnit.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/tavnit.cpp
 */

/* tavnit.cpp */

#include <morph/tavnit.h>

static char variables[6];  // used for matching: 'a' is variable[0] .. 'f' is variable[5]


bool Tavnit::match (CStr theword) const {
	static StringTemp thebase (24);
	return match(theword,thebase)==OK;
}

status Tavnit::match (CStr theword, StringTemp& thebase)  const {
	if (mytxilitlen<mytavnitlen)  return match_txilit_siomet(theword,thebase);
	else if (strlen(theword)==mytavnitlen) return match_variables (theword,thebase);
	else return EMISMATCH;
}


status Tavnit::match_and_set_variables (CStr thetavnit, Size thelen, CStr theword) const {
	for (Index i=0; i<thelen; ++i) {
		char c = thetavnit[i];
		if (islower(c)) {       // c is a variable
			if ('a'<=c && c<='f')       // check that it is in range
				variables[c-'a'] = theword[i];
			else return ERANGE;
		}
		else                   // c is a constant -- it must match exactly
			if (c!=theword[i]) return EMISMATCH;
	}
	return OK;
}

		
status Tavnit::match_txilit_siomet (CStr theword, StringTemp& thebase) const {
	uint thewordlen = strlen(theword);
	if (thewordlen < mytxilitlen || thewordlen < mysiometlen) return EMISMATCH;
	CStr thewordtxilit = theword;
	CStr thewordsiomet = theword + thewordlen - mysiometlen;

	DOr(match_and_set_variables (mytxilit,mytxilitlen,thewordtxilit));
	DOr(match_and_set_variables (mysiomet,mysiometlen,thewordsiomet));

	// If we reached here, it means that the word matches the tavnit.
	/* create the base */
	thebase.truncate();
	for (Index i=0; i<mybaselen; ++i) {
		char c = mybase[i];
		if (c=='?')        // c is a variable 
			thebase.append(theword,mytxilitlen,thewordlen-mysiometlen);
		else if (islower(c))        // c is a variable
			thebase.append (variables[c-'a']);
		else                   // c is a constant
			thebase.append(c);
	}
	thebase.end();

	return OK;
}



		
status Tavnit::match_variables (CStr theword,  StringTemp& thebase) const  {
	DOr (match_and_set_variables(mytavnit,mytavnitlen,theword));

	// If we reached here, it means that the word matches the tavnit.
	/* create the base */
	thebase.len=mybaselen;
	for (Index i=0; i<mybaselen; ++i) {
		char c = mybase[i];
		if (islower(c))        // c is a variable
			thebase[i] = variables[c-'a'];
		else                   // c is a constant
			thebase[i] = c;
	}
	thebase.end();
	return OK;
}


/***************/
/*     IO      */
/***************/

void Tavnit::write (ostream& out, Format format) const {
	out << mytavnit << ": " << mysug << " ";
	::write (out,myinfo,format);
}

status Tavnit::read (istream& in, Format format) {
	skip(in," \t\n:");
	set_tavnit ( next_line(in,16,":").finalstr() );
	DOr (::read(in,mysug));
	DOr (::read(in,myinfo,format));
	mybase=myinfo.baseword();  mybaselen=strlen(mybase);
	return OK;
}

