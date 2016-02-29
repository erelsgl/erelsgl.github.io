/* sntncas.cpp -- mnattex taxbiri l-&ibrit, &im sikkuyim.
		mi$tamme$ b-^algoritm "BOTTOM-UP CHART-PARSING" 
			[cf. JAMES ALLEN: NATURAL LANGUAGE UNDERSTANDING, 2nd ed., 1995, & 3.4]
*/

#include <morph/sntncas.h>
#include <morph/SentenceAnalyzer.h>


void write (ostream& out, SentenceOptionsCR theoptions) {
	write(out,theoptions,Format("*\n")); }

DEFINE_INLINE_IO_OPERATORS(SentenceOptions)

/********************************************************************************/
/*************       Sentence Analyzer               ****************************/
/********************************************************************************/

struct SentenceAnalyzerWithSikuiim: public SentenceAnalyzer {

	status natax_et_kol_hacerufim (ArrayOfMOWSCR the_original_sentence, SentenceOptions& the_options) {
		DOr(start(the_original_sentence.count(),the_options));
		LOOPVECTOR (;,the_original_sentence,w)  {
			MorphOptionsWithSikuiCR curword = the_original_sentence[w];
			LOOPVECTOR(;,curword,o) {
				DOr(the_complete_constituents.enqueue (
					Constituent(curword.info(o),w,w+1)));
			}
			DOr(process_all_complete_constituents())
		}
		DOr(create_final_options());
		return OK;
	}

	status natax_et_haceruf_hasavir_bioter (ArrayOfMOWSCR the_original_sentence, SentenceOptions& the_options) {
		DOr(start(the_original_sentence.count(),the_options));
		LOOPVECTOR (;,the_original_sentence,w)  {
			MorphOptionsWithSikuiCR curword = the_original_sentence[w];
			DOr(the_complete_constituents.enqueue (
				Constituent(curword.info(0),w,w+1)));
			DOr(process_all_complete_constituents())
		}
		DOr(create_final_options());
		return OK;
	}
	
	status natax_et_haceruf_hasavir_bioter_im_bituyim (ArrayOfMOWSCR the_original_sentence, SentenceOptions& the_options) {
		/* NOT IMPLEMENTED
		DOr(start(the_original_sentence.count(),the_options));
		LOOPVECTOR (;,the_original_sentence,w)  {
			MorphOptionsWithSikuiCR curword = the_original_sentence[w];
			if (w+1<the_original_sentence.count()) {
				MorphOptionsWithSikuiCR nextword = the_original_sentence[w+1];
				MorphInfo bituy_mcumcam;
				if (yej_bituy(curword.info(0),nextword.info(0), bituy_mcumcam)) {
					DOr(the_complete_constituents.enqueue (
						Constituent(bituy_mcumcam,w,w+2)));
					++w;
					goto process;
				}
				if (nextword.is_punctuation('-')&&w+2<the_original_sentence.count()) {
					MorphOptionsWithSikuiCR nextnextword = the_original_sentence[w+2];
					if (yej_bituy(curword.info(0),nextnextword.info(0), bituy_mcumcam)) {
						DOr(the_complete_constituents.enqueue (
							Constituent(bituy_mcumcam,w,w+3)));
						w+=2;
						goto process;
					}
				}
			}
			DOr(the_complete_constituents.enqueue (
				Constituent(curword.info(0),w,w+1)));

			process:
			DOr(process_all_complete_constituents())
		}
		DOr(create_final_options());
		return OK;
		*/
	}
};



/*******************************************************************/
/*********  public routines						      **************/
/*******************************************************************/


void natax (SentenceInfoWithCiyunCR thesentence, SentenceOptions& theoptions) {
	assert (the_sentence_analyzer_is_initialized);
	SentenceAnalyzer myanalyzer;
	DOx(myanalyzer.natax(thesentence, theoptions));
}


void natax (ArrayOfMOWSCR thesentence, SentenceOptions& theoptions) {
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
	//hagdel_sikui_jel_bituyim(cursentence_eachword_options);
	//sader_lfi_sikui_bseder_yored(cursentence_eachword_options);
	//sntncsiklogfile<<cursentence_eachword_options<<endl;
	SentenceOptionIdentifier cur_soi (cursentence_eachword_options);
	SentenceOptions temp_sentence_options;

	cur_soi.zero();  cur_soi.xajev_sikui_golmi();
	cursentence_options.append(cur_soi.sentenceinfo_lo_mufrad());

	SentenceAnalyzerWithSikuiim myanalyzer;
	myanalyzer.natax_et_haceruf_hasavir_bioter(cursentence_eachword_options,temp_sentence_options);
	double haciyun_hataxbiri_jel_heceruf_hasavir_bioter = ciyun_taxbiri_gavoh_bioter(temp_sentence_options);
	double haciyun_hamorpologi_jel_haceruf_hasavir_bioter = cur_soi.ciyun_curani;
	double haciyun_hasofi_jel_haceruf_hasavir_bioter = haciyun_hamorpologi_jel_haceruf_hasavir_bioter + haciyun_hataxbiri_jel_heceruf_hasavir_bioter*mijqal_taxbir;
	sntncsiklogfile
		<<haciyun_hamorpologi_jel_haceruf_hasavir_bioter<<"+"
		<<haciyun_hataxbiri_jel_heceruf_hasavir_bioter<<"="
		<<haciyun_hasofi_jel_haceruf_hasavir_bioter<<endl;
	double haciyun_hatov_bioter_beintayim = haciyun_hasofi_jel_haceruf_hasavir_bioter;

	for(;;) {
		sntncsiklogfile<<cur_soi<<endl;
		sntncsiklogfile<<temp_sentence_options<<endl<<endl;
		Index wordindex_hatov_bioter=0, optionindex_hatov_bioter=0;

		LOOPVECTOR(;,cursentence_eachword_options,wordindex) {
			if (cur_soi[wordindex]>0) continue;		// hit&allem mi-millim $e-kbar $innita!
			MorphOptionsWithSikuiCR curword = cursentence_eachword_options[wordindex];
			for (Index optionindex=1; optionindex<curword.count(); ++optionindex) {
				SentenceAnalyzerWithSikuiim new_analyzer;
				MorphInfo curoption = curword.info(0);
				MorphInfo newoption = curword.info(optionindex);
				if (!meod_jone(curoption,newoption)) continue;
				double haciyun_hamorpologi_jel_haceruf_hanokxi = cur_soi.ciyun_curani + log10(curword.realsikui(optionindex)+0.01) - log10(curword.realsikui(0)+0.01);
				//if (haciyun_hamorpologi_jel_haceruf_hanokxi < haciyun_hatov_bioter_beintayim) { // branch & bound
				//	sntncsiklogfile
				//		<<wordindex<<" "<<newoption<<" "
				//		<<haciyun_hamorpologi_jel_haceruf_hanokxi<<"(^ein cork b-nittux taxbiri)"<<endl;
				//	continue;
				//}
				duplicate(new_analyzer,myanalyzer);
				new_analyzer.natax_jenit(wordindex, newoption);
				double haciyun_hataxbiri_jel_haceruf_hanokxi = ciyun_taxbiri_gavoh_bioter(temp_sentence_options);
				double haciyun_hasofi_jel_haceruf_hanokxi = haciyun_hamorpologi_jel_haceruf_hanokxi + haciyun_hataxbiri_jel_haceruf_hanokxi*mijqal_taxbir;
				sntncsiklogfile
					<<wordindex<<" "<<newoption<<" "
				    <<haciyun_hamorpologi_jel_haceruf_hanokxi<<"+"
				    <<haciyun_hataxbiri_jel_haceruf_hanokxi<<"="
					<<haciyun_hasofi_jel_haceruf_hanokxi<<endl;
				#ifdef TEST_SNTNCAS
				cerr<<wordindex<<" "<<newoption<<" "<<haciyun_hasofi_jel_haceruf_hanokxi<<endl;
				#else
				cerr<<'.';
				#endif

				if (haciyun_hasofi_jel_haceruf_hanokxi > haciyun_hasofi_jel_haceruf_hasavir_bioter) {
					sntncsiklogfile<<temp_sentence_options;
					sntncsiklogfile<<haciyun_hasofi_jel_haceruf_hanokxi<<endl<<endl;
				}
				if (haciyun_hasofi_jel_haceruf_hanokxi > haciyun_hatov_bioter_beintayim) {
					haciyun_hatov_bioter_beintayim = haciyun_hasofi_jel_haceruf_hanokxi;
					wordindex_hatov_bioter = wordindex;
					optionindex_hatov_bioter = optionindex;
				}
				new_analyzer.finish();
			}
		}

		if (haciyun_hatov_bioter_beintayim > haciyun_hasofi_jel_haceruf_hasavir_bioter) { // nasse ^ep$arut ^axert
			cur_soi.haxlef (wordindex_hatov_bioter, optionindex_hatov_bioter);
			cursentence_options.append(cur_soi.sentenceinfo_lo_mufrad());
			myanalyzer.natax_jenit (wordindex_hatov_bioter, 
				cursentence_eachword_options[wordindex_hatov_bioter].info(optionindex_hatov_bioter) );
			haciyun_hasofi_jel_haceruf_hasavir_bioter = haciyun_hatov_bioter_beintayim;
		}
		else break;
	}
	myanalyzer.finish();
	sntncsiklogfile<<endl<<endl<<endl<<endl;
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
	SentenceOptions cursentence_options;
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
	SentenceOptions cursentence_options;
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
	SentenceOptions the_sentence_options;
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
