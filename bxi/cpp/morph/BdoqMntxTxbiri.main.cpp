/*
 * File morph/BdoqMntxTxbiri.main.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/BdoqMntxTxbiri.main.cpp
 */

/* BdoqMntxTxbiri.main.cpp */

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
//#include <morph/sntncanl.h>


ofstream plt ("bdoq_mntx_txbiri.txt");

void ntx_et_hnitux_hnkon_lmijpat (Index& wordnum_at_sentence_start) {
	static Array2<SentenceInfoWithCiyun> kol_hanituxim_lamijpat;
	static SentenceInfoWithCiyun hanitux_jelanu_lamijpat;
	//static Array2<CStr> hamijpat;
	static SentenceInfoWithCiyun hanitux_hanakon_lamijpat;

	clock_t before = clock();
	thetext.qra_mijpat_mehamaarak (wordnum_at_sentence_start, hanitux_hanakon_lamijpat);
	natax(hanitux_hanakon_lamijpat, kol_hanituxim_lamijpat);
	Index best_option = kol_hanituxim_lamijpat.len()-1;
	hanitux_jelanu_lamijpat = kol_hanituxim_lamijpat.at(best_option);
	plt 
		<< setw(12) << ciyun_taxbiri(hanitux_jelanu_lamijpat) << "  " 
		<< best_option << ": " << hanitux_jelanu_lamijpat
		<< " [" << hanitux_hanakon_lamijpat.count() << " words, " << (clock()-before)/CLOCKS_PER_SEC << " sec]" << endl;
	wordnum_at_sentence_start += hanitux_hanakon_lamijpat.count();
}




void ntx_qlt (Index start_from_word_num=0) {
	for (Index i=start_from_word_num; i<thetext.hanitux_hanakon_laqlt.count();)
		ntx_et_hnitux_hnkon_lmijpat(i);
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

	logfile << "% command line:\n%\t " << argv[0] << argv[1] << endl;
	lexlogfile << "% command line:\n%\t " << argv[0] << argv[1] << endl;
	errorlogfile << "% command line:\n%\t " << argv[0] << argv[1] << endl;


	if (int_argument("SofXlqBdiqa",0)) {          // bdoq
		thetext.atxl(500);
		thetext.qra_qlt (qorpus_mnutx_qlt, int_argument("TxiltXlqBdiqa", 0), int_argument("SofXlqBdiqa"));
		thetext.qra_nituxim_nkonim (qorpus_mnutx_nitux_nkon, int_argument("TxiltXlqBdiqa", 0), int_argument("SofXlqBdiqa"));

		thetext.ms_milim = thetext.ms_tauyot = thetext.ms_tauyot_baerek_hamiloni=0;
		ntx_qlt();
	}
	else {
		exit_with_error("this project is for 'bdiqa' only!", "", "");
	}


	close_all_the_logs();

	return 0;
}

