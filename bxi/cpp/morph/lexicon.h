/*
 * File morph/lexicon.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/lexicon.h
 */

/* lexicon.h */

#ifndef __LEXICON_H
#define __LEXICON_H

#include <morph/morphopt.h>


/* editing the lexicon */
status add_milit_to_lexicon (CStr theword, HeleqDiber thehd);
inline status add_word_to_lexicon (CStr theword, HeleqDiber thehd) { return add_milit_to_lexicon(theword,thehd); }

status add_ecem_to_lexicon (CStr theword, uint sug_ribui=0, Meen meen=NONE);

status add_toar_to_lexicon (CStr theword, uint sug_ribui=0);  // sug ribui is not used currently

status add_poal_to_lexicon (CStr theword);
status add_poal_1_to_lexicon (CStr theword, uint sug_howe);
status add_poal_2_to_lexicon (CStr theword, uint sug_atid);

status add_jem_prati_to_lexicon (CStr theword, Meen meen=NONE);

status add_milit_to_lexicon (CStr thefullword, MorphInfoCR theinfo);     // used for milat-yaxas and milat-guf and mispar
status add_miuxd_to_lexicon (CStr thefullword, MorphInfoCR theinfo);     // used for miuxdim

status add_yoce_dofen (CStr theword, MorphOptions theoptions);


/* queries */

bool lexicon_contains_word (CStr theword, HeleqDiber thehd, uint* thesug1=NULL, uint* thesug2=NULL);
bool lexicon_contains_milit (CStr theword, HeleqDiber hd);

bool lexicon_contains_poal (CStr theword, uint* sug_howe_p=NULL, uint* sug_atid_p=NULL);
bool lexicon_contains_poal_1 (CStr theword, uint sug_howe);
bool lexicon_contains_poal_2 (CStr theword, uint sug_atid);

bool lexicon_contains_ecem (CStr theword, uint sug_ribui=0, Meen* meen_p=NULL);
bool lexicon_contains_ecem (CStr theword, uint* sug_ribui_p, Meen* meen_p=NULL);

bool lexicon_contains_toar (CStr theword, uint sug_ribui=0);

bool lexicon_contains_jem_prati (CStr theword, Meen* meen_p=NULL);

bool lexicon_contains_milat_yaxas (CStr thefullword, MorphInfo& theinfo);
bool lexicon_contains_milat_guf (CStr thefullword, MorphInfo& theinfo);
bool lexicon_contains_mispar (CStr thefullword, MorphInfo& theinfo);
bool lexicon_contains_poal_ezer (CStr thefullword, MorphInfo& theinfo);

bool lexicon_contains_ecem_miuxd (CStr theword, MorphInfo& theinfo);
bool lexicon_contains_poal_miuxd (CStr thefullword, Zman thezman, MorphInfo& theinfo);
bool lexicon_contains_toar_miuxd (CStr theword, MorphInfo& theinfo);

bool lexicon_contains_yoce_dofen (CStr thefullword, MorphOptions& theoptions);

extern bool lexicon_is_creative;

bool laecem_yej_ntiat_jayakut_miuxedet (CStr ha_ecem);
bool lapoal_yej_ntiat_maqor_miuxedet (CStr ha_poal);
bool latoar_yej_ntia_miuxedet (CStr ha_toar);


/* IO */
// read/write the simple lexicon
// use Format("T") for table format, Format("D") for dense format.
//status read_the_lexicon (istream& in, Format format);
//void   write_the_lexicon (ostream& out, Format format);  

void   read_the_lexicon (CStr thepath, Format format="T");
void   write_the_lexicon (CStr thepath, Format format="T");  


#endif
