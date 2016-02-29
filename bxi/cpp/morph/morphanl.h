/*
 * File morph/morphanl.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphanl.h
 */

/* morphanl.h -- morphological analysis functions */

#ifndef __MORPHANL_H
#define __MORPHANL_H

#include <morph/morphopt.h>


void initialize_the_analyzer (CStr path_to_tavniot, CStr path_to_lexicon);

void natax (CStr thefullword, MorphOptions& theoptions);
MorphOptions natax (CStr thefullword);

void natax_txiliot (CStr thefullword, MorphOptions& theoptions);
MorphOptions natax_txiliot (CStr thefullword);


extern HeleqDiber default_heleq_diber;  // if default_heleq_diber != NO_HD, then if no analyses are found,
                                        //	a default analysis will be generated. (currently default_heleq_diber=JEM_PRATI).
bool last_analysis_used_the_default();


#endif
