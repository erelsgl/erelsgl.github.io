/*
 * File morph/tiqun3.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/tiqun3.h
 */

/* tiqun3.h */

#ifndef __TIQUN3_H
#define __TIQUN3_H

#include <morph/corpus.h>


void lmad_tiqunim (CStr input_filename, CStr correct_analysis_filename, Index from_wordnum=0, Index to_wordnum=0x7fffffff, int min_tiqun_score=3);
void qra_tiqunim (CStr thepath);
void ktov_tiqunim (CStr thepath, CStr kotrt=NULL);
void ktov_tiqunim_barur (CStr thepath, CStr kotrt=NULL);
int mspr_tiqunim ();

struct CorpusImTiqunim: public Corpus {
	void taqen_nituxim (Index start_from_word_num=0);  
		// mtaqqen ^et ha-nitux_jelanu lpi ha-tiqunim $e-nilmdu ^o niqr^u.
};

#endif
