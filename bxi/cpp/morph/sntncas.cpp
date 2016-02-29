/*
 * File morph/sntncas.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/sntncas.cpp
 */

/* sntncas.cpp -- mnattex taxbiri l-&ibrit, &im sikkuyim.
		mi$tamme$ b-^algoritm "BOTTOM-UP CHART-PARSING" 
			[cf. JAMES ALLEN: NATURAL LANGUAGE UNDERSTANDING, 2nd ed., 1995, & 3.4]
*/

#include <morph/sntncas.h>
#include <morph/SentenceAnalyzer.h>


/********************************************************************************/
/*************       Sentence Analyzer               ****************************/
/********************************************************************************/

struct SentenceAnalyzerWithSikuiim: public SentenceAnalyzer {

	status natax_et_kol_hacerufim (ArrayOfMOWSCR the_original_sentence, Array2<SentenceInfo>& the_options) {
		DOr(start(the_original_sentence.count(),the_options));
		LOOPVECTOR (;,the_original_sentence,w)  {
			MorphOptionsWithSikuiCR curword = the_original_sentence[w];
			LOOPVECTOR(;,curword,o) {
				Constituent thecons(curword,o, w);
				DOr(the_complete_constituents.enqueue (thecons));
			}
			DOr(process_all_complete_constituents())
		}
		DOr(create_final_options());
		return OK;
	}

	status natax_et_haceruf_hasavir_bioter (ArrayOfMOWSCR the_original_sentence, Array2<SentenceInfo>& the_options) {
		DOr(start(the_original_sentence.count(),the_options));
		LOOPVECTOR (;,the_original_sentence,w)  {
			MorphOptionsWithSikuiCR curword = the_original_sentence[w];
			Constituent thecons(curword,0, w);
			DOr(the_complete_constituents.enqueue (thecons));
			DOr(process_all_complete_constituents())
		}
		DOr(create_final_options());
		return OK;
	}
};



/*******************************************************************/
/*********  public routines						      **************/
/*******************************************************************/


void natax (SentenceInfoWithCiyunCR thesentence, Array2<SentenceInfo>& theoptions) {
	assert (the_sentence_analyzer_is_initialized);
	SentenceAnalyzer myanalyzer;
	DOx(myanalyzer.natax(thesentence, theoptions));
}


void natax (ArrayOfMOWSCR thesentence, Array2<SentenceInfo>& theoptions) {
	assert (the_sentence_analyzer_is_initialized);
	SentenceAnalyzerWithSikuiim myanalyzer;
	DOx(myanalyzer.natax_et_kol_hacerufim(thesentence, theoptions));
}



bool meod_jone (MorphInfoCR a, MorphInfoCR b) {
//	return (a.heleqdiber()!=b.heleqdiber() || a.txiliot()!=b.txiliot()); 
//	return (a.heleqdiber()!=b.heleqdiber() || a.txiliot()!=b.txiliot() || a.smikut()!=b.smikut()); 
//	return (a.heleqdiber()!=b.heleqdiber() || a.txiliot()!=b.txiliot() || a.smikut()!=b.smikut() || a.yidua()!=b.yidua() || a.meen()!=b.meen()); 
	return true;
}


void natax2 (ArrayOfMOWSCR the_cursentence_eachword_options, Array2<SentenceInfo>& cursentence_options, double mijqal_taxbir) {
	extern ofstream sntncsiklogfile;
	static ArrayOfMOWS cursentence_eachword_options;
	assert (the_sentence_analyzer_is_initialized);
	::free (cursentence_options);
	::free (cursentence_eachword_options);
	duplicate (cursentence_eachword_options, the_cursentence_eachword_options);
	sader_lfi_sikui_bseder_yored(cursentence_eachword_options);
	sntncsiklogfile<<cursentence_eachword_options<<endl;

	SentenceAnalyzerWithSikuiim my_analyzer;
	DOx(my_analyzer.natax_et_kol_hacerufim(the_cursentence_eachword_options, cursentence_options));
	//my_analyzer.natax_et_haceruf_hasavir_bioter(the_cursentence_eachword_options, cursentence_options);

	SentenceOptionIdentifier cur_soi (the_cursentence_eachword_options, my_analyzer.my_final_option_indices);
	DOx(cursentence_options.append(cur_soi.sentenceinfo_lo_mufrad()));
	my_analyzer.finish();
}


/*******************************************************************************************************/
/****************  SELF TEST          ******************************************************************/
/*******************************************************************************************************/


#ifdef TEST_SNTNCAS

#include <cpplib/sentence.h>
#include <morph/hpmaio2.h>
#include <morph/alghist.h>
#include <morph/tiqun2.h>
#include <morph/mn.h>

#define LOG_PATH "..\\..\\..\\harc\\"

void atxel_klali() {
	atxel_global_database (NULL, LOG_PATH "harc10.nts");
	atxel_local_database (NULL, LOG_PATH "harc10a.nts");
	qra_cimcumim(LOG_PATH);
	//atxel_tiqunim (LOG_PATH);
	atxel_tiqunim_from_cimcumim ();
	adken_tiqunim (LOG_PATH "harc10.to");
	ktov_tiqunim (concat_path_to_filename(LOG_PATH,"tiqunim3.ma"));
	adken_monei_zugot_nituxim (LOG_PATH "harc10.to");
	open_logfile(1); ktov_monei_zugot_nituxim(logfile); logfile.close();
}




void bdoq() {
	Sentence cursentence(500);
	ArrayOfMOWS cursentence_eachword_options;
	Array2<SentenceInfoWithCiyun> cursentence_options;
	for(;;) {
		DOEOFx(read_and_analyze_the_next_sentence (input, cursentence, cursentence_eachword_options));
		taqen_sikuiim_zugot (cursentence_eachword_options);
		LOG (logfile,cursentence_eachword_options<<endl<<endl<<cursentence<<endl);
		natax(cursentence_eachword_options,cursentence_options);
		LOG (logfile,cursentence_options<<endl<<endl<<endl<<endl);
	}
}

void bdoq2() {
	Sentence cursentence(500);
	ArrayOfMOWS cursentence_eachword_options;
	Array2<SentenceInfoWithCiyun> cursentence_options;
	for(;;) {
		DOEOFx(read_and_analyze_the_next_sentence (input, cursentence, cursentence_eachword_options));
		taqen_sikuiim_zugot (cursentence_eachword_options);
		LOG (logfile,cursentence_eachword_options<<endl<<endl<<cursentence<<endl);
		natax2 (cursentence_eachword_options, cursentence_options);
		writeln (logfile,cursentence_options, Format("P\n"));  writeln (cout,cursentence_options, Format("P\n"));
		logfile << endl << endl << endl;
	}
}


void bdoq3() {
	ArrayOfActiveArcs aa1;
	for(Index i=0;i<10;i++) {
		LOG (logfile,i<<endl);
		DOx(aa1.reset(50));
		for(Index j=0; j<5000; ++j) {
			ActiveArc thearc (hacimcumim[0][0],0,0);
			aa1.append(thearc);
		}
		ArrayOfActiveArcs aa2;
		duplicate (aa2,aa1);
		::free (aa1);
		::free (aa2);
	}
}

/*
void bdoq_zikkaron() {
	SentenceInfo the_sentence_info;
	Array2<SentenceInfoWithCiyun> the_sentence_options;
	DOx(the_sentence_info.read(correct_analysis));
	SentenceAnalyzer myanalyzer;
	for(Index i=0;;i++) {
		DOx(myanalyzer.natax(the_sentence_info, the_sentence_options));
		LOG (logfile,i<<endl);
		writeln (logfile,the_sentence_options, Format("*\n"));
		writeln (cout,the_sentence_options, Format("*\n"));
		LOG (logfile,endl<<endl<<endl);
	}
}
*/

void main (int argc, char* argv[]) {
	set_synopsis ("SNTNCANL source.TXT source.TO [/t path-to-cimcumim]");
	parse_command_line (argc,argv,2,2,"t","");
	CStr path_to_cimcumim = log_path = option('t')==NULL? ".": option('t');
	initialize_the_sentence_analyzer (path_to_cimcumim);
	atxel_klali();
	open_logfile(0);
	open_sntncanllogfile(log_path);
	open_sntncsiklogfile(log_path);
	open_tiqunimlogfile(log_path);
	open_infile_with_messages (concat_path_to_filename(log_path,variable(0)).str,input);
	open_infile_with_messages (concat_path_to_filename(log_path,variable(1)).str,correct_analysis);
	bdoq2();
}

#endif
