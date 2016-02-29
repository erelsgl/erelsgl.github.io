/*
 * File morph/addlex.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/addlex.h
 */

/* addlex.h -- functions to extend the lexicon */

#ifndef __ADDLEX_H
#define __ADDLEX_H

#include <morph/morphinf.h>

status extend_lexicon (CStr thefullword, MorphInfo theanalysis);
	// this function extends the lexicon, so that it will know that 'the-analysis' matches 'the-fullword'.
	// Use this function if the analyzer doesn't find some analysis.

#endif
