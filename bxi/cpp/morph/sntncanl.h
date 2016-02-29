/*
 * File morph/sntncanl.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/sntncanl.h
 */

/* sntncanl.h -- morphological analysis functions */

#ifndef __SNTNCANL_H
#define __SNTNCANL_H

#include <morph/sntncinf.h>
#include <cpplib/array2.hxx>


typedef Array2<SentenceInfo> SentenceOptions;
typedef const SentenceOptions& SentenceOptionsCR;


void initialize_the_sentence_analyzer (CStr path_to_cimcumim);
void natax (SentenceInfoCR the_sentence, SentenceOptions& the_options);
double ciyun_hanitux_hataxbiri_hatov_bioter (SentenceInfoCR the_sentence);

//double ciyun (SentenceInfoCR the_partially_analyzed_sentence);
//double ciyun_hanitux_hataxbiri_hatov_bioter (SentenceOptionsCR the_options);
//void mxaq_et_hanituxim_hapaxot_tovim (SentenceOptions& the_options,  double the_ciyun_saf);


#endif
