/*
 * File morph/lexiconb.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/lexiconb.h
 */

/* lexiconb.h -- lexicon-bi@utyim */

#ifndef LEXICONB_H
#define LEXICONB_H

#include <morph/cimcumim.h>
#include <cpplib/stringt.h>

//bool yej_bituy_jematxil_b (MorphInfoCR x);
bool yej_bituy (MorphInfoCR x, MorphInfoCR y);
bool yej_bituy (MorphInfoCR x, MorphInfoCR y,  MorphInfo& tocaa);

//status add_bituy (MorphInfoPatternCR x, MorphInfoPatternCR y);

void ktov_lexicon_bituyim (CStr thepath);
status qra_lexicon_bituyim (CStr thepath);


#endif
