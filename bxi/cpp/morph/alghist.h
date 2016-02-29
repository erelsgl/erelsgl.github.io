/*
 * File morph/alghist.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/alghist.h
 */

/* alghist.h -- algoritm histabruti l-xi$uv ha-nitux ha-savir bioter */

#ifndef __ALGHIST_H
#define __ALGHIST_H

#include <morph/morphsik.h>

#define END_OF_ARTICLE "*"


/***************************************************************************/
/**********        The Databases                                    ********/
/***************************************************************************/

void qjr_myda_mqomi_lmyda_globli();

void atxl_global_database (CStr input_filename, CStr analysis_with_sikui_filename);
void qra_nitux_nakon_global (CStr input_filename, CStr output_filename, int from_wordnum=0, int to_wordnum=0x7fffffff);
void qra_mone_nituxim_global (CStr input_filename);
void ktov_global_database (CStr output_filename);

MorphOptionsWithSikui analysis_with_approximate_sikuiim (CStr thefullword);
MorphOptionsWithSikui independent_analysis_with_approximate_sikuiim (CStr thefullword);

status adkn_database (CStr hamila, MorphInfoCR hanitux);
status adkn_database (CStr hamila, MorphOptionsWithSikuiCR kol_hanituxim);

void set_mjqlim_global (double mjql_en);
void xjv_sikuiim_lkol_hmilim_global ();
void xjv_sikuiim (CStr hamila); // calculate sikuiim of all the analyses of the main_word, and adken the database

#endif
