/*
 * File morph/mna.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/mna.h
 */

/* mna.h -- monei-nittuxim-axrei (w-gamm lipnei) */

#ifndef __MNA_H
#define __MNA_H

#include <morph/morphinf.h>

void atxel_monei_nituxim (CStr correct_analysis_filename);
void adken_monei_nituxim (CStr correct_analysis_filename);
void ktov_monei_nituxim (ostream& out);
status qra_monei_nituxim(istream& in);
void qra_monei_nituxim (CStr path);

//Count count (MorphInfoCR qodem, MorphInfoCR nokxi);
double relative_count_lipnei (MorphInfoCR qodem, MorphInfoCR nokxi);
double relative_count_axrei (MorphInfoCR qodem, MorphInfoCR nokxi);

#endif

