/* MntxCurniHstbrutiTxbiri.main.cpp */

#include <math.h>

#include <morph/lexicon.h>
#include <morph/addlex.h>
#include <morph/morphanl.h>

#include <morph/alghist.h>
#include <morph/tiqun3.h>
//#include <morph/hajlamot.h>

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
#include <morph/hpmaconf.h>

#include <morph/sntncas.h>


void ntx_mijpat (Index& wordnum_at_sentence_start) {
	static ArrayOfMOWS hanituxim_lkol_mila_bamijpat;
	static Array2<SentenceInfo> kol_hanituxim_lamijpat;
	static SentenceInfo hanitux_jelanu_lamijpat;
	static Array2<CStr> hamijpat;

	thetext.qra_mijpat_mehamaarak (wordnum_at_sentence_start,hanituxim_lkol_mila_bamijpat,hamijpat);
	//camcem_bituyim (hanituxim_lkol_mila_bamijpat);
	natax2(hanituxim_lkol_mila_bamijpat,kol_hanituxim_lamijpat,1.);
	Index best_option = kol_hanituxim_lamijpat.len()-1;
	hanitux_jelanu_lamijpat = kol_hanituxim_lamijpat.at(best_option);
	thetext.ktov_mijpat_lamaarak (wordnum_at_sentence_start,hanitux_jelanu_lamijpat);
	wordnum_at_sentence_start += hamijpat.count();
}


void ntx_qlt_3(Index start_from_word_num=0) {
	for (Index w=start_from_word_num; w<thetext.haqlt.count(); ) {
		cerr << endl << "MILA # " << w << "(" << thetext.haqlt[w] << ") ";
		ntx_mijpat (w);
	}
}

void ntx_qlt (Index start_from_word_num=0) {
	if (!swtch('a')) {
		cerr << "*** $LB 1: $LB HMILA (XI$UB HSTBRUYOT MTOK " 
			<< (swtch('t')? "BICCU& QODM $L $LB HMILA": option('o')? "QORPUS MNUTX": "QORPUS LO MNUTX")
			<< ") ***" << endl;
		thetext.ntx_qlt_1(start_from_word_num);
		thetext.ntx_qlt_2(start_from_word_num);
	}
	else{
		thetext.ntx_qlt_1_axid(start_from_word_num);
		thetext.ntx_qlt_2_baqrai(start_from_word_num);
	}
	thetext.hajwe_nituxim();
	thetext.log_ms_jgiot();

	if (!swtch('m')) {
		cerr << "*** $LB 2: $LB HZUG (" << mspr_tiqunim() << " PQUDOT TIQUN) ***" << endl;
		thetext.taqen_nituxim(start_from_word_num);
		thetext.hajwe_nituxim();
		thetext.log_ms_jgiot();
	}
	if (!swtch('n')) {
		cerr << "*** $LB 3: $LB HM$P@ (NITUX TXBIRI) ***" << endl;
		ntx_qlt_3(start_from_word_num);
		thetext.hajwe_nituxim();
		thetext.log_ms_jgiot();
	}
}



/***************************************************************************/
/***********        analysis modes                              ********/
/***************************************************************************/

void test_mode_analysis () {
	thetext.ms_milim = thetext.ms_tauyot = thetext.ms_tauyot_baerek_hamiloni=0;
	ntx_qlt();
}


void batch_mode_analysis (Index start_from_word_num=0) {
	ntx_qlt(start_from_word_num);
	thetext.ktov_et_hanitux_jelanu(output);
}


void interactive_mode_analysis(Index start_from_word_num=0) {
	ntx_qlt(start_from_word_num);
	bdoq_et_hanitux_jelanu_laqlt(start_from_word_num);
	ktov_et_hanituxim_lapelet();
	ktov_tocaot();
}


/***************************************************************************/
/***********        The Main Program                                ********/
/***************************************************************************/

CStr lex_path;
CStr article_path, article_output_path;
CStr corpus_path, corpus_input_data_from_previous_analysis, corpus_output_data_from_this_analysis;
CStr training_corpus_path, training_corpus_output_path;


void read_the_command_line(int argc, char* argv[]) {
	set_synopsis ("\n"
		"Interactive mode (first time):        MntxCurniHstbruti -i    corpus-filename article-filename [options]\n"
		"Interactive mode (continue):          MntxCurniHstbruti -i -h corpus-filename article-filename [options]\n"
		"Batch mode (first time):              MntxCurniHstbruti -b    corpus-filename article-filename [options]\n"
		"Batch mode (continue):                MntxCurniHstbruti -b -h corpus-filename article-filename [options]\n"
		"	[NOTE: use the 'continue' mode only if you used the switch '-d' in all previous runs!]\n"
		"Compare mode:                         MntxCurniHstbrutiTxbiri -q corpus-filename article-filename [options]\n"
		"One word analysis mode (for debugging): MntxCurniHstbrutiTxbiri train-corpus-filename article-filename -w word [options]\n"
		"    (options include:  [-l path-of-lexicon] [-g path-for-logfiles] [-o training-corpus-filename]\n"
		"         [-s=load-corpus-NTS-file] [-t=load-corpus-NT1-file] [-d=dense-output-format] [-n=NO nitux taxbiri] [-m=NO tiqunim] [-a=NO sikuiim])\n");
	parse_command_line (argc,argv,2,2,"lgwzxo","iqb h st dx nma");
	lex_path = option('l')==NULL? ".": option('l');
	log_path = option('g')==NULL? ".": option('g');

	corpus_path=filename(variable(0),"txt").finalstr();

	if (swtch('t')) {
		//NT1 file - generated after a "word phase" analysis using an analyzed corpus.
		//Note that the NT1 file doesn't contain the correct analyses themselves,
		//	so using a ready NT1 file (using -t) will be much less accurate than performing the word phase
		//	each time (using -o <name>).
		corpus_input_data_from_previous_analysis=filename(variable(0),"nt1").finalstr();
		corpus_output_data_from_this_analysis=NULL;
	}
	else if (swtch('s')) {
		//NTS file - includes only word-counters (generated after a "word phase" analysis using a raw corpus)
		corpus_input_data_from_previous_analysis=filename(variable(0),"nts").finalstr();
		corpus_output_data_from_this_analysis= option('o')?
			//option('o') = perform the word-phase now.
			corpus_output_data_from_this_analysis=filename(variable(0),"nt1").finalstr():
			corpus_output_data_from_this_analysis=NULL;
	}
	else {      
		corpus_input_data_from_previous_analysis=NULL;
		corpus_output_data_from_this_analysis= option('o')?
			//option('o') = perform the word-phase now.
			filename(variable(0),"nt1").finalstr():
			corpus_output_data_from_this_analysis=filename(variable(0),"nts").finalstr();
	}

	if (option('o')) { // perform the word-phase now.
		training_corpus_path=filename(option('o'),"txt").finalstr();
		training_corpus_output_path=filename(option('o'),"to").finalstr();
	}
	else if (option('x')) { // learn new correction-rules now.
		training_corpus_path=filename(option('x'),"txt").finalstr();
		training_corpus_output_path=filename(option('x'),"to").finalstr();
	}
	else {
		training_corpus_path=training_corpus_output_path=NULL;
	}

	article_path=filename(variable(1),"txt").finalstr();
	article_output_path=filename(variable(1), swtch('q')? "to": "out" ).finalstr();

	pelet_cafuf = swtch('d');
}



void atxl () {
	if (swtch('t')) {
		atxl_global_database (NULL,corpus_input_data_from_previous_analysis);  // read previously saved NT1 file
	}
	else if (swtch('s')) {
		atxl_global_database (NULL,corpus_input_data_from_previous_analysis);
		if (option('o'))  {
			qra_nitux_nakon_global (training_corpus_path, training_corpus_output_path); // read correct analysis file for corpus, in order to train it.
			xjv_sikuiim_lkol_hmilim_global();
			ktov_global_database (corpus_output_data_from_this_analysis); // generate NT1 file
		}
	}
	else {
		atxl_global_database(corpus_path,NULL);
		if (option('o'))      // read correct analysis file for corpus
			qra_nitux_nakon_global (training_corpus_path, training_corpus_output_path); // read correct analysis file for corpus, in order to train it.
		xjv_sikuiim_lkol_hmilim_global();
		ktov_global_database (corpus_output_data_from_this_analysis);  // generate NTS/NT1 file
	}

}



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



void main (int argc, char* argv[]) {
	Index start_from_word_num;

	lexicon_is_creative=false;
	read_the_command_line(argc, argv);
	initialize_the_analyzer(lex_path,lex_path);
	initialize_the_sentence_analyzer(lex_path);
	atxl ();
	if (option('x')) {      // lmad tiqqunim lfi ha-corpus
		lmad_tiqunim (training_corpus_path, training_corpus_output_path, 0, 0x7ffffff, 2);
		ktov_tiqunim (lex_path, argc, argv);
	}
	qra_tiqunim(lex_path);
	open_all_the_logs();

	logfile << "% command line:\n%\t ";
	lexlogfile << "% command line:\n%\t ";
	errorlogfile << "% command line:\n%\t ";
	for (int i=0; i<argc; ++i) {
		logfile << argv[i] << " ";
		lexlogfile << argv[i] << " ";
		errorlogfile << argv[i] << " ";
	}
	logfile << endl;
	lexlogfile << endl;
	errorlogfile << endl;


	thetext.atxl(500);
	thetext.qra_qlt (article_path);

	if (swtch('q')) {          // bdoq
		thetext.qra_nituxim_nkonim (article_output_path);
	}
	else if (swtch('h')) {               // hosef
		thetext.qra_pelet_xelqi (article_output_path);
		open_outfile_with_messages (article_output_path,output,ios::app);
		start_from_word_num = thetext.hanitux_jelanu_laqlt.count();
	}
	else {
		open_outfile_with_messages (article_output_path,output,0);
		start_from_word_num = 0;
	}

	if (swtch('q'))	test_mode_analysis();
	else if (swtch('i')) interactive_mode_analysis(start_from_word_num);
	else if (swtch('b')) batch_mode_analysis(start_from_word_num);

	close_all_the_logs();
}

