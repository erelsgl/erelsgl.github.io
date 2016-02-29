/*
 * File morph/MntxCurniHstbrutiTxbiri.main.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/MntxCurniHstbrutiTxbiri.main.cpp
 */

/* MntxCurniHstbrutiTxbiri.main.cpp */

#include <math.h>
#include <time.h>

#include <morph/lexicon.h>
#include <morph/addlex.h>
#include <morph/morphanl.h>

#include <morph/alghist.h>
#include <morph/tiqun3.h>
//#include <morph/hajlamot.h>

#include <cpplib/argsfile.h>
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
#include <morph/hpmainit.h>

#include <morph/sntncas.h>



void ntx_mijpat (Index& wordnum_at_sentence_start) {
	static ArrayOfMOWS hanituxim_lkol_mila_bamijpat;
	static Array2<SentenceInfo>  kol_hanituxim_lamijpat;
	static SentenceInfo hanitux_jelanu_lamijpat;
	static Array2<CStr> hamijpat;

	clock_t before = clock();
	thetext.qra_mijpat_mehamaarak (wordnum_at_sentence_start,hanituxim_lkol_mila_bamijpat,hamijpat);
	//camcem_bituyim (hanituxim_lkol_mila_bamijpat);
	natax2(hanituxim_lkol_mila_bamijpat,kol_hanituxim_lamijpat,1.);
	Index best_option = kol_hanituxim_lamijpat.len()-1;
	hanitux_jelanu_lamijpat = kol_hanituxim_lamijpat.at(best_option);
	thetext.ktov_mijpat_lamaarak (wordnum_at_sentence_start,hanitux_jelanu_lamijpat);
	wordnum_at_sentence_start += hamijpat.count();
	cerr << " [" << hamijpat.count() << " words, " << (clock()-before)/CLOCKS_PER_SEC << " sec]";
}


void ntx_qlt_3(Index start_from_word_num=0) {
	for (Index w=start_from_word_num; w<thetext.haqlt.count(); ) {
		cerr << endl << "MILA # " << w << "(" << thetext.haqlt[w] << ") ";
		ntx_mijpat (w);
	}
}

void ntx_qlt (Index start_from_word_num=0) {
	cerr 
		<< "*** $LB 1: $LB HMILA"  << endl
		<< "***		QovcQltTocaotJlvMila=" << (qorpus_input_data_from_previous_analysis? qorpus_input_data_from_previous_analysis: "<none>") << endl
		<< "***		XlqImunJlvMila=[" << int_argument("TxiltXlqImunJlvMila",0) << "..." << int_argument("SofXlqImunJlvMila",0) << "]" << endl;
	thetext.ntx_qlt_1(start_from_word_num);
	thetext.ntx_qlt_2(start_from_word_num);
	if (thetext.hanitux_hanakon_laqlt.count() > 0) {
		thetext.hajwe_nituxim();
		thetext.log_ms_jgiot();
	}

	if (string_argument("QovcTiqunim",NULL)) {
		cerr 
			<< "*** $LB 2: $LB HZUG"  << endl
			<< "***		QovcTiqunim=" << string_argument("QovcTiqunim") << endl
			<< "***		MsprTiqunim=" << mspr_tiqunim() << endl;
		thetext.taqen_nituxim(start_from_word_num);
		if (thetext.hanitux_hanakon_laqlt.count() > 0) {
			thetext.hajwe_nituxim();
			thetext.log_ms_jgiot();
		}
	}
	if (string_argument("QovcCmcumim",NULL)) {
		cerr 
			<< "*** $LB 3: $LB HM$P@"  << endl
			<< "***		QovcCmcumim=" << string_argument("QovcCmcumim") << endl;
		ntx_qlt_3(start_from_word_num);
		if (thetext.hanitux_hanakon_laqlt.count() > 0) {
			thetext.hajwe_nituxim();
			thetext.log_ms_jgiot();
		}
	}
}


/***************************************************************************/
/***********        The Main Program                                ********/
/***************************************************************************/



void open_all_the_logs() {
	open_logfile(7); 
	open_lexlogfile(log_path);
	open_sntncsiklogfile(log_path);
	open_sntncanllogfile(log_path);
	open_errorlogfile(log_path);
}

void close_all_the_logs() {
	logfile.close();  
	close_errorlogfile();
	close_lexlogfile();
	close_sntncsiklogfile();
	close_sntncanllogfile();
}


void atxl_jlv_mjpt() {
	if (string_argument("QovcCmcumim",NULL)) 
		initialize_the_sentence_analyzer(string_argument("QovcCmcumim"));
}


int main (int argc, char* argv[]) {
	process_arguments_file(argc, argv);
	initialize_the_analyzer(lex_path,lex_path);
	open_all_the_logs();

	atxl_jlv_mila();
	atxl_jlv_zug();
	atxl_jlv_mjpt();

	logfile << "% command line:\n%\t " << argv[0] << " " << argv[1] << endl;
	lexlogfile << "% command line:\n%\t " << argv[0] << " " << argv[1] << endl;
	errorlogfile << "% command line:\n%\t " << argv[0] << " " << argv[1] << endl;


	if (int_argument("SofXlqBdiqa",0)) {          // bdoq
		thetext.atxl(500);
		thetext.qra_qlt (qorpus_mnutx_qlt, int_argument("TxiltXlqBdiqa", 0), int_argument("SofXlqBdiqa"));
		thetext.qra_nituxim_nkonim (qorpus_mnutx_nitux_nkon, int_argument("TxiltXlqBdiqa", 0), int_argument("SofXlqBdiqa"));

		thetext.ms_milim = thetext.ms_tauyot = thetext.ms_tauyot_baerek_hamiloni=0;
		ntx_qlt();
	}


	if (mamr_lnitux_al_ydi_tokna_qlt) {
		thetext.atxl(500);
		thetext.qra_qlt (mamr_lnitux_al_ydi_tokna_qlt);

		Index start_from_word_num;
		if (int_argument("HmjkNitux",0)) {               // hosef
			thetext.qra_pelet_xelqi (mamr_lnitux_al_ydi_tokna_plt);
			open_outfile_with_messages (mamr_lnitux_al_ydi_tokna_plt, output, ios::app);
			start_from_word_num = thetext.hanitux_jelanu_laqlt.count();
		}
		else {
			open_outfile_with_messages (mamr_lnitux_al_ydi_tokna_plt, output, ios::trunc);
			start_from_word_num = 0;
		}
		ntx_qlt(start_from_word_num);
		thetext.ktov_et_hanitux_jelanu(output);
	}


	if (mamr_lnitux_al_ydi_mjtmj_qlt) {
		thetext.atxl(500);
		thetext.qra_qlt (mamr_lnitux_al_ydi_mjtmj_qlt);

		Index start_from_word_num;
		if (int_argument("HmjkNitux",0)) {               // hosef
			thetext.qra_pelet_xelqi (mamr_lnitux_al_ydi_mjtmj_plt);
			open_outfile_with_messages (mamr_lnitux_al_ydi_mjtmj_plt, output, ios::app);
			start_from_word_num = thetext.hanitux_jelanu_laqlt.count();
		}
		else {
			open_outfile_with_messages (mamr_lnitux_al_ydi_mjtmj_plt, output, ios::trunc);
			start_from_word_num = 0;
		}
		ntx_qlt(start_from_word_num);
		bdoq_et_hanitux_jelanu_laqlt(start_from_word_num);
		ktov_et_hanituxim_lapelet();
		ktov_tocaot_nitux_al_ydi_mjtmj();
	}

	close_all_the_logs();

	return 0;
}

