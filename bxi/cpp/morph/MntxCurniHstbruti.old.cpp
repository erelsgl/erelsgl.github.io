/* MntxCurniHstbruti.main.cpp */

#include <math.h>

#include <morph/lexicon.h>
#include <morph/addlex.h>
#include <morph/morphanl.h>

#include <morph/alghist.h>
#include <morph/tiqun3.h>

#include <cpplib/ui.h>
#include <morph/morphui.h>
#include <morph/corpus.h>

#include <cpplib/wordcnt.h>
#include <cpplib/map.hxx>
#include <cpplib/hashtab.hxx>
#include <cpplib/countc.hxx>
#include <morph/hpmaio.h>
#include <morph/hpmamain.h>




void ntx_qlt (Index start_from_word_num=0) {
		cerr << "*** $LB 1: $LB HMILA (XI$UB HSTBRUYOT MTOK @QS@ " 
			<< (swtch('t')||option('o')? "MNUTX": "LO MNUTX") 
			<< ") ***" << endl;
	thetext.ntx_qlt_1(start_from_word_num);
	thetext.ntx_qlt_2(start_from_word_num);
	thetext.taqen_nituxim(start_from_word_num);
}

/***************************************************************************/
/***********        analysis modes                              ********/
/***************************************************************************/

void test_mode_analysis () {
	thetext.ms_milim = thetext.ms_tauyot = thetext.ms_tauyot_baerek_hamiloni=0;
	thetext.ntx_qlt_1();
	thetext.ntx_qlt_2();
	thetext.hajwe_nituxim();
	thetext.log_ms_jgiot();

	thetext.ms_milim = thetext.ms_tauyot = thetext.ms_tauyot_baerek_hamiloni=0;
	cerr << "*** $LB 2: $LB HZUG (" << mspr_tiqunim() << " PQUDOT TIQUN) ***" << endl;
	thetext.taqen_nituxim();
	thetext.hajwe_nituxim();
	thetext.log_ms_jgiot();
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
CStr corpus_path, corpus_correct_analysis_path, corpus_output_data_from_this_analysis, corpus_input_data_from_previous_analysis;
int 
	start_of_training_part_wordnum, end_of_training_part_wordnum, 
	start_of_statistics_part_wordnum, end_of_statistics_part_wordnum,
	start_of_test_part_wordnum, end_of_test_part_wordnum;

void read_the_command_line(int argc, char* argv[]) {
	set_synopsis ("\n"
		"Interactive mode (first time):        MntxCurniHstbruti -i    corpus-filename article-filename [options]\n"
		"Interactive mode (continue):          MntxCurniHstbruti -i -h corpus-filename article-filename [options]\n"
		"Batch mode (first time):              MntxCurniHstbruti -b    corpus-filename article-filename [options]\n"
		"Batch mode (continue):                MntxCurniHstbruti -b -h corpus-filename article-filename [options]\n"
		"	[NOTE: use the 'continue' mode only if you used the switch '-d' in all previous runs!]\n"
		"Compare mode:                         MntxCurniHstbruti -q    corpus-filename end-of-training-part-wordnum start-of-statistics-part-wordnum [options]\n"
		"    options:  [-l path-of-lexicon] [-g path-for-logfiles-and-inputfiles] [-o training-corpus-filename]\n"
		"    switches: [-d=dense-output-format] [-s=load-corpus-NTS-file] [-t=load-corpus-NT1-file]");
	parse_command_line (argc,argv,2,3,"lgxo","iqb h d st");
	lex_path = option('l')==NULL? ".": option('l');
	log_path = option('g')==NULL? ".": option('g');

	corpus_path=filename(variable(0),"txt").finalstr();
	corpus_correct_analysis_path=filename(variable(0),"to").finalstr();

	if (swtch('q')) {
		start_of_test_part_wordnum = 0;
		end_of_test_part_wordnum = variable_as_int(1,0,0x7ffffff);

		start_of_training_part_wordnum = end_of_test_part_wordnum;
		end_of_training_part_wordnum = variable_as_int(2,0,0x7ffffff);

		start_of_statistics_part_wordnum = end_of_training_part_wordnum;
		end_of_statistics_part_wordnum = 0x7fffffff;

		article_path=NULL;
		article_output_path=NULL;
	}
	else {
		start_of_training_part_wordnum = 0;
		end_of_training_part_wordnum = 0;

		start_of_test_part_wordnum = 0;
		end_of_test_part_wordnum = 0;

		start_of_statistics_part_wordnum = 0;
		end_of_statistics_part_wordnum = 0;

		article_path=filename(variable(1),"txt").finalstr();
		article_output_path=filename(variable(1),"out").finalstr();
	}


	if (swtch('t')) {
		corpus_input_data_from_previous_analysis=filename(variable(0),"nt1").finalstr();
		corpus_output_data_from_this_analysis=NULL;
	}
	else if (swtch('s')) {
		corpus_input_data_from_previous_analysis=filename(variable(0),"nts").finalstr();
		corpus_output_data_from_this_analysis= option('o')?
			corpus_output_data_from_this_analysis=filename(variable(0),"nt1").finalstr():
			corpus_output_data_from_this_analysis=NULL;
	}
	else {      
		corpus_input_data_from_previous_analysis=NULL;
		corpus_output_data_from_this_analysis= option('o')?
			filename(variable(0),"nt1").finalstr():
			corpus_output_data_from_this_analysis=filename(variable(0),"nts").finalstr();
	}



	/*
	if (option('o')) {
		training_corpus_path=filename(option('o'),"txt").finalstr();
		training_corpus_output_path=filename(option('o'),"to").finalstr();
	}
	else if (option('x')) {
		training_corpus_path=filename(option('x'),"txt").finalstr();
		training_corpus_output_path=filename(option('x'),"to").finalstr();
	}
	else {
		training_corpus_path=training_corpus_output_path=NULL;
	}
	*/

	pelet_cafuf = swtch('d');
}



void atxl () {
	if (option('o'))  {    // read correct analysis file for corpus
		//if (swtch('s')) {
		//	atxl_global_database (NULL,corpus_input_data_from_previous_analysis);
		//else
		//	atxl_global_database(corpus_path,NULL);
		qra_nitux_nakon_global (corpus_path, corpus_correct_analysis_path, start_of_statistics_part_wordnum, end_of_statistics_part_wordnum);
		xjv_sikuiim_lkol_hmilim_global(); 
		ktov_global_database (corpus_output_data_from_this_analysis);
	}
	else if (swtch('t')) {
		atxl_global_database (NULL,corpus_input_data_from_previous_analysis);
	}
	else if (swtch('s')) {
		atxl_global_database (NULL,corpus_input_data_from_previous_analysis);
	}
	else {      
		atxl_global_database(corpus_path,NULL);
		xjv_sikuiim_lkol_hmilim_global();
		ktov_global_database (corpus_output_data_from_this_analysis);
	}

}


void open_all_the_logs() {
	open_logfile(7); 
	open_lexlogfile(log_path);
	open_errorlogfile(log_path);
	open_sgtlogfile(log_path);
}

void close_all_the_logs() {
	logfile.close();  
	close_errorlogfile();
	close_lexlogfile();
	close_sgtlogfile();
}




void main (int argc, char* argv[]) {
	Index start_from_word_num;

	lexicon_is_creative=false;
	read_the_command_line(argc, argv);
	initialize_the_analyzer(lex_path,lex_path);
	open_all_the_logs();
	atxl ();

	if (option('x')) {      // lmad tiqqunim lfi ha-corpus
		lmad_tiqunim (corpus_path, corpus_correct_analysis_path, start_of_training_part_wordnum, end_of_training_part_wordnum);
		ktov_tiqunim (lex_path, argc, argv);
	}
	else {
		qra_tiqunim(lex_path);
	}

	thetext.atxl(500);

	if (swtch('q')) {          // bdoq
		thetext.qra_qlt (corpus_path, start_of_test_part_wordnum, end_of_test_part_wordnum);
		thetext.qra_nituxim_nkonim (corpus_correct_analysis_path, start_of_test_part_wordnum, end_of_test_part_wordnum);
	}
	else if (swtch('h')) {               // hosef
		thetext.qra_qlt (article_path);
		thetext.qra_pelet_xelqi (article_output_path);
		open_outfile_with_messages(article_output_path,output,ios::app);
		start_from_word_num = thetext.hanitux_jelanu_laqlt.count();
	}
	else {
		thetext.qra_qlt (article_path);
		open_outfile_with_messages(article_output_path,output,0);
		start_from_word_num = 0;
	}

	if (swtch('q'))	test_mode_analysis();
	else if (swtch('i')) interactive_mode_analysis(start_from_word_num);
	else if (swtch('b')) batch_mode_analysis(start_from_word_num);

	close_all_the_logs();
}
