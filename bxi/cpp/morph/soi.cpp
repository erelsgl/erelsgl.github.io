/*
 * File morph/soi.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/soi.cpp
 */

/* soi.cpp -- Sentence Option Identifier
	An auxiliary structure that identifies one option to analyze a sentence.
*/

#include <morph/soi.h>


void sader_lfi_sikui_bseder_yored(ArrayOfMOWS& the_eachword_options) {
	LOOPVECTOR (;,the_eachword_options,w0) 
		the_eachword_options[w0].sader_lfi_sikui_bseder_yored();
}



void duplicate (SentenceOptionIdentifier& to, SentenceOptionIdentifierCR from) {
	duplicate (to.myinfo, from.myinfo);
	to.my_eachword_options = from.my_eachword_options;
	to.ciyun_curani = from.ciyun_curani;
}


bool identical (SentenceOptionIdentifierCR a, SentenceOptionIdentifierCR b) {
	if (a.ciyun_curani!=b.ciyun_curani)  return false;
	if (a.count()!=b.count())  return false;
	LOOPVECTOR(;,a,w) 
		if (a[w] != b[w]) return false;
	return true;
}



