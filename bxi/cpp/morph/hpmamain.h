/*
 * File morph/hpmamain.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/hpmamain.h
 */

/* hpmamain.h -- utilities for main programs using HPMA */

#include <morph/corpus.h>
#include <morph/tiqun3.h>

extern CorpusImTiqunim thetext;

Index select_option (MorphOptionsWithSikui& kol_hanituxim, CStr hamila, bool& hanitux_xadaj);

void ktov_et_hanituxim_lapelet();
void bdoq_et_hanitux_jelanu_laqlt(Index start_from_word_num=0);
void ktov_tocaot_nitux_al_ydi_mjtmj();

