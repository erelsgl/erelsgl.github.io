/*
 * File morph/similar.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/similar.h
 */

/* similar.h */

#ifndef __SIMILAR_H
#define __SIMILAR_H

/*
	This unit defines 3 kinds of similarity:
		SBW == same baseword (including heleq-diber)
		SA	== similar analysis (including heleq-diber)
		ST	== similar txiliot.
*/

#include <morph/morphinf.h>

/*
	Dimion is a number that represents the ammount of similarity between 
		2 analyses (MorphInfoBasic's).
	The maximum Dimion is 'ZEHUT' (it means that the 2 analyses are identical).
*/

typedef uint Dimion;  
#define ZEHUT 64


void bdoq_dimion (MorphInfoBasicCR a, MorphInfoBasicCR b, Dimion& sast);
void bdoq_dimion (MorphInfoCR a, MorphInfoCR b, Dimion& sbw, Dimion& sast);

Dimion dimion (MorphInfoBasicCR a, MorphInfoBasicCR b);
Dimion dimion_hatama (MorphInfoBasicCR a, MorphInfoBasicCR b);
Dimion dimion_sof (MorphInfoBasicCR a, MorphInfoBasicCR b);
Dimion dimion_hatama_sof (MorphInfoBasicCR a, MorphInfoBasicCR b);


bool are_SBW_SA_ST (MorphInfoCR a, MorphInfoCR b);
// checks if a and b are similar  (same baseword, similar analysis, similar txiliot).

bool are_SA_ST (MorphInfoBasicCR a, MorphInfoBasicCR b);
// This version ignores the lexical value (it looks only on the analysis, including HD and txiliot).

bool are_SBW (MorphInfoCR a, MorphInfoCR b);
// This version ignores the analysis (it looks only on the lexical value and the HD).

uint mispar_milim_domot (MorphInfoBasicCR theinfo);

#endif
