/*
 * File morph/hpmamain.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/hpmamain.cpp
 */

/* hpmamain.cpp -- utilities for main programs using HPMA */

#include <math.h>

#include <morph/lexicon.h>
#include <morph/addlex.h>
#include <morph/morphanl.h>

#include <morph/alghist.h>
#include <morph/tiqun3.h>

#include <cpplib/ui.h>
#include <morph/morphui.h>
#include <morph/corpus.h>
#include <cpplib/sentence.h>

#include <cpplib/wordcnt.h>
#include <cpplib/map.hxx>
#include <cpplib/hashtab.hxx>
#include <cpplib/countc.hxx>
#include <morph/hpmaio.h>
#include <morph/hpmamain.h>

CorpusImTiqunim thetext;

bool sof;

/********************* select option ****************************************/
/********************* select option ****************************************/

void display_options (MorphOptionsWithSikuiCR kol_hanituxim, CStr hamila) {
	LOOPVECTOR(;,kol_hanituxim,i)  {
		cout << "      " << i << ". ";
		kol_hanituxim.info(i).ktov_barur (cout, hamila);
		cout << " (";
		kol_hanituxim.info(i).ktov_meforat(cout);
		cout << ") " << kol_hanituxim.sikui(i) << endl;
	}
	cout << "      " << kol_hanituxim.count() << ". " << "AP AXD MHN\"L" << endl;
}

Index select_option (MorphOptionsWithSikui& kol_hanituxim, CStr hamila, bool& hanitux_xadaj) {
	display_options (kol_hanituxim,hamila);
	Index the_index_of_the_selected_analysis = Index (input_int("       MHW HNITUX HNKWN?",0,kol_hanituxim.count()));
	hanitux_xadaj = ( the_index_of_the_selected_analysis >= kol_hanituxim.count() ) ; 
	if (!hanitux_xadaj) {  // nivxar nitux $e-kvar qayam
		return the_index_of_the_selected_analysis; 
	}
	else {                 // nivxar nitux '^axer' (xada$)
		MorphInfo new_option;  input_morphinfo(hamila,new_option);
		kol_hanituxim.append_option(new_option,1.);
		adkn_database (hamila, kol_hanituxim);
		thetext.ntx_qlt_1(hamila);
		lexlogfile << hamila << ": " << new_option << endl;
		return kol_hanituxim.count()-1;
	}
}


/********************* get command from the user ****************************************/
/********************* get command from the user ****************************************/

Index
	wordnum,
	wordnum_at_previous_save,
	mispar_tiqunim,
	mispar_nituxim_xadajim;
bool 
	hanitux_xadaj;

Array2<Index> indexei_milim_im_nituxim_xadajim;

void ktov_et_hanituxim_lapelet() {
	thetext.ktov_et_hanitux_jelanu(output, wordnum_at_previous_save, wordnum);
	for (Index w=wordnum_at_previous_save; w<wordnum; ++w) {
		CStr hamila = thetext.haqlt[w];
		MorphInfoCR hanitux_hanakon = thetext.hanitux_jelanu_laqlt[w];
		if (wordnum<thetext.haqlt.count()) {
			assert(OK==adkn_database (hamila, hanitux_hanakon));
			//xjv_sikuiim_lkol_hmilim_im_sbwsast (hanitux_hanakon);
		}
		if (contains(indexei_milim_im_nituxim_xadajim,w) || // bdoq ^im carik lharxib ^et ha-lexicon
			(hanitux_hanakon.hu(JEM_PRATI) && !lexicon_contains_jem_prati(hanitux_hanakon.baseword())))
			DOx(extend_lexicon (hamila, hanitux_hanakon));
	}
	indexei_milim_im_nituxim_xadajim.truncate();
	wordnum_at_previous_save = wordnum;
	if (wordnum<thetext.haqlt.count()) {
		thetext.ntx_qlt_1(wordnum);
		thetext.ntx_qlt_2(wordnum);
		thetext.taqen_nituxim(wordnum);
	}
}


void taqen_et_hanitux(CStr hamila, MorphInfo& hanitux_jelanu_lamila, MorphOptionsWithSikui& kol_hanituxim_lamila) {
	Index index_hanitux_hamtuqan = select_option (kol_hanituxim_lamila, hamila, hanitux_xadaj);
	MorphInfoCR hanitux_hamtuqan = kol_hanituxim_lamila.info(index_hanitux_hamtuqan);
	if (!identical(hanitux_hamtuqan, hanitux_jelanu_lamila))  ++mispar_tiqunim;
	hanitux_jelanu_lamila = hanitux_hamtuqan;
}


void bdoq_et_hanitux_jelanu_laqlt(Index start_from_word_num) {
	Index wordnum_at_sentence_start = wordnum = wordnum_at_previous_save = start_from_word_num;
	for (; wordnum<thetext.haqlt.count() ;)  {
		//if (wordnum==wordnum_at_sentence_start)
		//	ntx_mijpat (wordnum_at_sentence_start);

		CStr hamila = thetext.haqlt[wordnum];
		MorphInfo& hanitux_jelanu_lamila = thetext.hanitux_jelanu_laqlt[wordnum];
		MorphOptionsWithSikui& kol_hanituxim_lamila = thetext.kol_hanituxim_laqlt[wordnum];
		hanitux_xadaj = false;
		cerr << "   ";
		hanitux_jelanu_lamila.ktov_barur (cerr, hamila);  
		cerr << " (";
		hanitux_jelanu_lamila.ktov_meforat (cerr);
		cerr << ")   ";
		char c=prompt_from("[A=A$R T=TQN X=XZWR K=KTWB S=SIIM]","atxks"); cerr<<endl;
		switch(c){
			case 'a':  ++wordnum; break;                 
			case 't':  
				taqen_et_hanitux (hamila, hanitux_jelanu_lamila, kol_hanituxim_lamila); 
				if (hanitux_xadaj) {
					++mispar_nituxim_xadajim;
					indexei_milim_im_nituxim_xadajim.append(wordnum);
				}		
				break; 
			case 'x':  if (wordnum>wordnum_at_previous_save) --wordnum; break;
			case 'k':  ktov_et_hanituxim_lapelet(); break;
			case 's':  return;
		}
	}
}

void ktov_tocaot_nitux_al_ydi_mjtmj () {
	double axuz_tiqunim = 100.*mispar_tiqunim/wordnum;
	double axuz_xadajim = 100.*mispar_nituxim_xadajim/wordnum;
	LOG(logfile,"MSPR MILIM MNUTXOT: " << wordnum << endl 
		<< "MSPR TIQWNIM: " << mispar_tiqunim << " (" << axuz_tiqunim << "%)" << endl
		<< "MSPR NITWXIM XD$IM: " << mispar_nituxim_xadajim << " (" << axuz_xadajim << "%)" << endl);
}


