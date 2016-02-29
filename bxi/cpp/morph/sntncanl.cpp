/*
 * File morph/sntncanl.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/sntncanl.cpp
 */

/* sntncanl.cpp -- mnattex taxbiri l-&ibrit 
		mi$tamme$ b-^algoritm "BOTTOM-UP CHART-PARSING" 
			[cf. JAMES ALLEN: NATURAL LANGUAGE UNDERSTANDING, 2nd ed., 1995, & 3.4]
*/

#include <morph/sntncanl.h>
#include <morph/sa-aux.h>





/*******************************************************************/
/*********  public routines						      **************/
/*******************************************************************/

void mxaq_et_hanituxim_hapaxot_tovim (SentenceOptions& the_options, double the_ciyun_saf) {
	LOOPVECTOR(;, the_options, o) {
		while (ciyun_taxbiri(the_options[o]) < the_ciyun_saf  &&  o<the_options.count()) {
			the_options.removeat(o);    
		}
	}
}


void natax (SentenceInfoCR thesentence, SentenceOptions& theoptions) {
	assert (the_sentence_analyzer_is_initialized);
	SentenceAnalyzer myanalyzer;
	DOx(myanalyzer.natax(thesentence, theoptions));
}

double ciyun_hanitux_hataxbiri_hatov_bioter (SentenceInfoCR the_sentence) {
	SentenceOptions the_options;
	natax (the_sentence,the_options);
	double CGB = ciyun_taxbiri_gavoh_bioter(the_options);
	free (the_options);
	return CGB;
}


/*******************************************************************************************************/
/****************  SELF TEST          ******************************************************************/
/*******************************************************************************************************/


#ifdef TEST_SNTNCANL

#include <cpplib/sentence.h>
#include <morph/hpmaio.h>
#include <morph/corpus.h>


void bdoq() {
	for(;;) {
		Sentence cursentence(500);
		SentenceInfo cursentence_info;
		SentenceOptions cursentence_options;
		DOEOFx(cursentence.readline(input));
		DOEOFx(cursentence_info.read(correct_analysis));
		natax (cursentence_info, cursentence_options);
		mxaq_et_hanituxim_hapaxot_tovim (cursentence_options, ciyun_taxbiri_gavoh_bioter(cursentence_options) );
		LOG (logfile,cursentence_info<<endl<<endl<<cursentence<<endl);
		writeln (logfile,cursentence_options, Format("*\n"));  writeln (cout,cursentence_options, Format("*\n"));
		logfile << endl << endl << endl;
	}
}



void bdoq_zikkaron_1() {
	SentenceInfo the_sentence_info;
	SentenceOptions the_sentence_options;
	DOx(the_sentence_info.read(correct_analysis));
	SentenceAnalyzer myanalyzer;
	for(Index i=0;;i++) {
		DOx(myanalyzer.natax(the_sentence_info, the_sentence_options));
		LOG (logfile,i<<endl);
		writeln (logfile,the_sentence_options, Format("*\n"));
		writeln (cout,the_sentence_options, Format("*\n"));
		LOG (logfile,endl<<endl<<endl);
		myanalyzer.finish();
	}
}


void bdoq_zikkaron_2() {
	SimpleArrayOfActiveArcs the_aa;
	for(Index i=0;i<10000;i++) {
		LOG (logfile,i<<endl);
		//DOx(the_aa.reset(5000));
		for(Index j=0; j<5000; ++j) {
			ActiveArc thearc (hacimcumim[0],0,0);
			the_aa.append(thearc);
		}
		free(the_aa);
	}
}



void bdoq_zikkaron_3() {
	Array2<float> the_aa;
	for(Index i=0;i<10000;i++) {
		LOG (logfile,i<<endl);
		//DOx(the_aa.reset(50));
		for(Index j=0; j<20000; ++j) {
			the_aa.append(0.);
		}
		free(the_aa);
	}
}



void bdoq_zikkaron_4() {
	Vector1<float> the_aa;
	for(Index i=0;i<10000;i++) {
		LOG (logfile,i<<endl);
		//DOx(the_aa.reset(50));
		for(Index j=0; j<20000; ++j) {
			the_aa.append(0.);
		}
		free(the_aa);
	}
}


void main (int argc, char* argv[]) {
	set_synopsis ("SNTNCANL source.TXT source.TO [/t path-to-cimcumim]");
	parse_command_line (argc,argv,2,2,"t","");
	CStr path_to_cimcumim = log_path = option('t')==NULL? ".": option('t');
	initialize_the_sentence_analyzer (path_to_cimcumim);
	ktov_cimcumim_barur(path_to_cimcumim);
	open_logfile(0);
	open_sntncanllogfile(log_path);
	open_infile_with_messages (concat_path_to_filename(log_path,variable(0)).str,input);
	open_infile_with_messages (concat_path_to_filename(log_path,variable(1)).str,correct_analysis);
	bdoq_zikkaron_1();
}

#endif
