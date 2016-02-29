/*
 * File morph/sntncas.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/sntncas.h
 */

/* sntncas.h -- mnattex taxbiri &imm sikkuyim. */

#ifndef __SNTNCAS_H
#define __SNTNCAS_H

#include <morph/sntncinc.h>
#include <morph/soi.h>
#include <cpplib/array2.hxx>

inline double ciyun_taxbiri_gavoh_bioter (const Array2<SentenceInfoWithCiyun>&  the_options) {
	LOOPVECTOR (double cur=MIN_CIYUN, the_options, o)
		if (the_options[o].ciyun_taxbiri() > cur)
			cur = the_options[o].ciyun_taxbiri();
	return cur;
}

void initialize_the_sentence_analyzer (CStr path_to_cimcumim);

void natax (SentenceInfoWithCiyunCR the_sentence, Array2<SentenceInfo>&  the_sentence_options);
	// qelet: nitux curani 1 lkoll milla bami$pa@.
	// pelet: ma&arak ("the_sentence_options") $e-kolel ^et koll ha-nituxim ha-taxbiriyim 
	//        ha-^ep$ariyim la-mi$pa@.

void natax (ArrayOfMOWSCR the_eachword_options, Array2<SentenceInfo>&  the_sentece_options);
	// qelet: ma&arak ("the_eachword_options") $e-kolel ^et koll ha-nituxim ha-morpologiyim 
	//        l-koll mila ba-mi$pa@.
	// pelet: ma&arak ("the_sentence_options") $e-kolel ^et koll ha-nituxim ha-taxbiriyim 
	//        ha-^ep$ariyim la-mi$pa@.

//void natax2 (ArrayOfMOWSCR the_eachword_options, Array2<SentenceInfo>& the_sentece_options, double mijqal_taxbir=1.);
void natax2 (ArrayOfMOWSCR the_eachword_options, Array2<SentenceInfo>&  the_sentece_options, double mijqal_taxbir=1.);
	// qelet: ma&arak ("the_eachword_options") $e-kolel ^et koll ha-nituxim ha-morpologiyim 
	//        l-koll mila ba-mi$pa@.
	// pelet: ma&arak ("the_sentence_options") $e-kolel ^et koll ha-nituxim ha-taxbiriyim 
	//        ha-^ep$ariyim la-mi$pa@.


//double ciyun_hanitux_hataxbiri_hatov_bioter (SentenceInfoCR the_sentence);

#endif
