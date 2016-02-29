/*
 * File morph/alghist.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/alghist.cpp
 */

/* alghist.cpp -- algoritm histabbruti l-xi$$uv ha-nittux ha-sabir bioter */

#include <iomanip.h> 
#include <cpplib/wordcnt.h>
#include <cpplib/histgram.h>
#include <morph/morphst2.h>
#include <morph/morphanl.h>
#include <morph/similar.h>
#include <morph/alghist.h>
#include <morph/bw2fw.h>
#include <morph/mone-nit.h>
#include <morph/mone-bw.h>
#include <morph/mone-yid.h>
#include <morph/hpmaio.h>

bool ktov_et_kl_tocaot_jlv_hmila;


/***************************************************************************/
/**********        The Databases                                    ********/
/***************************************************************************/

BasewordToFullwordMap the_bw2fw_map;
	// maps each baseword to all it's derived fullwords. See the file morph/bw2fw.h

struct GlobalDatabase {

	MorphOptionsWithSikuiTrie hmyda_hmqorv;
		// this database contains, for each word, the approximate sikui for each option, 
		//	as calculated by the algorithm. See the file morph/morphst.h

	MorphOptionsWithSikuiTrie hmyda_hamiti;
		// this database contains, for each word, the number of real analyses (approved by the user)
		//	for each option-index. See the file morph/morphst.h

	WordCounter mone_milim_jlmot;
		// counts full-words in the corpus. See the file cpplib/wordcnt.h

	MoneBaseword mone_arkim_miloniim;
		// counts basewords (ignoring analyses). See the file morph/mone-bw.h

	MoneNituxim mone_nituxim_domim;
		// counts analyses (ignoring lexical-values). See the file morph/mone-nit.h

	MoneNituxim mone_nituxim_zhim;
		// counts analyses (ignoring lexical-values). See the file morph/mone-nit.h

	MoneYidua mone_yidua;
		// counts yidua;

	HistogramCounter mone_mony_arkim_miloniim;
		// counts number of numbers of occurrences of basewords (for each r, how many basewords occured r times).

	//HistogramCounter mone_mony_nituxim_domim;
		// counts number of numbers of occurrences of analyses (for each r, how many analyses occured r times).

	HistogramCounter mone_mony_nituxim_zhim;
		// counts number of numbers of occurrences of analyses (for each r, how many analyses occured r times).

	uint total_word_count;

	bool idkun_mony_nituxim;
		// controls updating of "the-mone-nituxim" during "xjv_sikuiim_lkol_hmilim".

	bool xijuv_jaqet; 
		// controls message-displaying during "xjv_sikuiim_lkol_hmilim".
	bool xijuv_jaqet_meod;
		// controls logging during "xjv_sikuiim".


	#define LOOP_OVER_ALL_THE_WORDS(cursor) for (MorphOptionsWithSikuiTrie::Cursor cursor(hmyda_hmqorv);  cursor.isvalid();  ++cursor)

	bool isempty() const { return total_word_count==0; }

	MorphOptionsWithSikui analysis_with_approximate_sikuiim (CStr hamila) {
		MorphOptionsWithSikui theoptions;
		if (!hmyda_hmqorv.contains(hamila, theoptions)) {
			theoptions = natax_im_sikui_axid(hamila); 
			if (!last_analysis_used_the_default()) {
				hmyda_hmqorv.insert(hamila,theoptions);
				mone_milim_jlmot.insert(hamila);
				xjv_sikuiim(hamila,true /* maher */ );
			}
		}
		return theoptions;
	}

	MorphOptionsWithSikui independent_analysis_with_approximate_sikuiim (CStr hamila) {
		return duplicate
			(analysis_with_approximate_sikuiim(hamila));
	}



	/****************** Database update ******************************/
	/******             Database update             ******************/
	/****************** Database update ******************************/

	status adkn (CStr hamila, MorphInfoCR hanitux) {
		DOr(hmyda_hamiti.insert (hamila,hanitux));
		DOr(mone_milim_jlmot.remove_one (hamila));
		return OK;
	}

	status adkn (CStr hamila, MorphOptionsWithSikuiCR theoptions) {
		DOr(hmyda_hmqorv.insert(hamila,theoptions));
		DOr(hmyda_hamiti.insert(hamila,theoptions));
		return OK;
	}

	status havr_mila_axt_mhmyda_hmqorv_lmyda_hamiti (CStr hamila, MorphOptionsWithSikuiCR the_realistic_options) {
		DOx(hmyda_hamiti.add (hamila,the_realistic_options));
		DOx(mone_milim_jlmot.remove_one (hamila));
		return OK;
	}

	Sikui sikui_en (MorphOptionsWithSikuiCR myda_mqorv, uint mspr_milim_im_myda_mqorv, MorphOptionsWithSikuiCR myda_amiti, Index mspr_nitux) {
		return (Sikui)(myda_mqorv.sikui(mspr_nitux) * (mspr_milim_im_myda_mqorv/*+0.5*/) + 
									myda_amiti.sikui(mspr_nitux));
	}

	void adkn_monim () { // m&adkn ^et 2 ha-monim (erk-miloni, nituxim)
		// loop over all the words in the corpus, and insert all distinct MorphInfoBasic's to the analysis counter. 
		assert(MorphOptionsWithSikuiTrie::Cursor(hmyda_hmqorv).isvalid());  // there are words in the corpus.

		if (idkun_mony_nituxim) {
			LOOP_OVER_ALL_THE_WORDS(cursor1) {
				CStr cur_fullword = cursor1.string().str;
				MorphOptionsWithSikui cur_options = hmyda_hmqorv.item(cur_fullword);
				LOOPVECTOR(;,cur_options,i)  {
					mone_nituxim_domim.setcount(cur_options.info(i),0);
					mone_nituxim_zhim.setcount(cur_options.info(i),0);
				}
			}
		}
		mone_yidua.zero_all();
		mone_arkim_miloniim.zero_all();

		// loop over all the words in the corpus, and if they are similar to the main_word -- add their probabilities to the sums. 
		LOOP_OVER_ALL_THE_WORDS(cursor) {
			CStr cur_fullword = cursor.string().str;
			logfile << cur_fullword << "; "; logfile.flush();
			MorphOptionsWithSikuiCR myda_mqorv_lmila = hmyda_hmqorv.item(cur_fullword);
			uint mspr_milim_im_myda_mqorv = mone_milim_jlmot.count(cur_fullword);
			MorphOptionsWithSikuiCR myda_amiti_lmila = hmyda_hamiti.item(cur_fullword);
			LOOPVECTOR(;,myda_mqorv_lmila,i) {
				Sikui cur_sikui = sikui_en(myda_mqorv_lmila, mspr_milim_im_myda_mqorv, myda_amiti_lmila, i);
				mone_arkim_miloniim.add(myda_mqorv_lmila.info(i), cur_sikui);
				if (idkun_mony_nituxim) {
					mone_nituxim_domim.add(myda_mqorv_lmila.info(i), cur_sikui);
					mone_nituxim_zhim.add_exact(myda_mqorv_lmila.info(i), cur_sikui);
				}
				mone_yidua.add (myda_mqorv_lmila.info(i), cur_sikui);
			}
		}

		for (MoneBaseword::Cursor cursor2(mone_arkim_miloniim); cursor2.isvalid(); ++cursor2)
			mone_mony_arkim_miloniim.insert((int)cursor2.count());
		mone_mony_arkim_miloniim.calculate_goodturing_counts(/*20000*/);

		//for (MoneNituxim::Cursor cursor3(mone_nituxim_domim); cursor3.isvalid(); ++cursor3)
		//	mone_mony_nituxim_domim.insert((int)cursor3.count());
		//mone_mony_nituxim_domim.calculate_goodturing_counts(/*200*/);

		for (MoneNituxim::Cursor cursor4(mone_nituxim_zhim); cursor4.isvalid(); ++cursor4)
			mone_mony_nituxim_zhim.insert((int)cursor4.count());
		mone_mony_nituxim_zhim.calculate_goodturing_counts(/*200*/);
	}


	void qra_mone_nituxim_domim (CStr thefilename) {
		ifstream input;
		open_infile_with_messages (thefilename,input);
		assert (OK==read(input,mone_nituxim_domim));
		input.close();
		idkun_mony_nituxim = false;
	}



	/**************************** sfor-sikuiim ************************************/
	/***********                  sfor-sikuiim                        *************/
	/**************************** sfor-sikuiim ************************************/

	// en == &erk-milloni + nittux
	// e  == &erk-milloni
	// n  == nittux

	ArrayOfSikuiim 
		sikuiim_enz,    // milla zha
		sikuiim_end,    // erk-miloni & nittux domim
		sikuiim_e,      // erk-miloni dome
		sikuiim_nd,     // nittux dome
		sikuiim_nz,     // nittux zhe
		sikuiim_yidua,  // mcv yidua dome
		sikuiim_mjuqllim;

	double 
		mjql_e,
		mjql_nd,
		mjql_nz,
		mjql_end,
		mjql_enz,
		mjql_yidua; 
/*
	// count-similar-words for one main MorphOptions, in the entire database.
	void sfor_sikuiim_lat (CStr main_word, MorphOptionsWithSikuiCR myda_mqorv_lmila) {

		// loop over all the words in the corpus, and if they are similar to the main_word -- add their probabilities to the sums. 
		LOOP_OVER_ALL_THE_WORDS(cursor) {
			CStr cur_fullword = cursor.string().str;
			MorphOptionsWithSikuiCR myda_mqorv_lmila_nokxit = hmyda_hmqorv.item(cur_fullword);
			uint mspr_milim_im_myda_mqorv = mone_milim_jlmot.count(cur_fullword);
			MorphOptionsWithSikuiCR myda_amiti_lmila_nokxit = hmyda_hamiti.item(cur_fullword);

			Dimion sbw, sast; 
			LOOPVECTOR(;,myda_mqorv_lmila_nokxit,j) {
				MorphInfoCR cur_info = myda_mqorv_lmila_nokxit.info(j);
				Sikui cur_sikui = sikui_en(myda_mqorv_lmila_nokxit, mspr_milim_im_myda_mqorv, myda_amiti_lmila_nokxit, j);
				LOOPVECTOR(;,myda_mqorv_lmila,i) {
					bdoq_dimion(cur_info, myda_mqorv_lmila.info(i), sbw, sast);

					sikuiim_nd[i] += (sast * cur_sikui) / ZEHUT;
					//if (sast==ZEHUT) sikuiim_nd[i] += cur_sikui;
					if (sbw==ZEHUT) {
						sikuiim_e[i] += cur_sikui;
						sikuiim_end[i] += (sast * cur_sikui) / ZEHUT;
						//if (sast==ZEHUT) sikuiim_end[i] += cur_sikui;
					}
				}
			}
		}
	}
*/
	// count-similar-words for one main MorphOptions, in the entire database.
	// This function should be called only after 'adkn_monim' has been called.
	void sfor_sikuiim_mhr (CStr main_word, MorphOptionsWithSikuiCR myda_mqorv_lmila) {
		Dimion sbw,sast;

		uint mspr_milim_im_myda_mqorv = mone_milim_jlmot.count(main_word);
		MorphOptionsWithSikuiCR myda_amiti_lmila = hmyda_hamiti.item(main_word);

		LOOPVECTOR (;, myda_mqorv_lmila, i) {
			MorphInfoCR cur_option = myda_mqorv_lmila.info(i);
			uint mmd = mispar_milim_domot (cur_option);

			sikuiim_nd[i] += mone_nituxim_domim.count(cur_option);
			sikuiim_nz[i] += mone_nituxim_zhim.count(cur_option);
			sikuiim_enz[i] += sikui_en(myda_mqorv_lmila, mspr_milim_im_myda_mqorv, myda_amiti_lmila, i);
			sikuiim_e[i] += mone_arkim_miloniim.count(cur_option);

			if (cur_option.hu(ECEM))
				sikuiim_yidua[i] += mone_yidua.shifted_count(cur_option)/2;

			// loop over all the words in the corpus with SBW, and if they are similar to the main_word -- add their probabilities to the sums. 
			FullwordArray& the_fullword_array = the_bw2fw_map.fullword_array(cur_option.baseword());
			for (FullwordArray::Cursor cursor(the_fullword_array); cursor.isvalid(); ++cursor) {
				CStr cur_fullword = cursor.item();
				//if (identical (cur_fullword,main_word)) continue;
				uint cur_count = mone_milim_jlmot.count(cur_fullword);	if (cur_count==0) continue;
				MorphOptionsWithSikui cur_options = hmyda_hmqorv.item(cur_fullword);
				LOOPVECTOR(;,cur_options,j) {
					Sikui cur_sikui = sikui_en(cur_options, cur_count, hmyda_hamiti.item(cur_fullword),j);
					bdoq_dimion (cur_options.info(j),cur_option, sbw,sast);
					if (sbw==ZEHUT) sikuiim_end[i] += cur_sikui*sast/mmd;
					//if (sbw==ZEHUT) sikuiim_e[i] += cur_sikui;
				}
			}

		}
	}


	void sfor_sikuiim (CStr main_word, MorphOptionsWithSikuiCR main_options_s, bool mhr) {
		/*** ^atxel ^et veq@orei ha-toca^a ***/
		atxel (sikuiim_e,main_options_s.count() );
		atxel (sikuiim_nd,main_options_s.count() );
		atxel (sikuiim_nz,main_options_s.count() );
		atxel (sikuiim_end,main_options_s.count() );
		atxel (sikuiim_enz,main_options_s.count() );
		atxel (sikuiim_yidua,main_options_s.count() );

		//if (mhr)
			sfor_sikuiim_mhr (main_word, main_options_s);
		//else 
		//	sfor_sikuiim_lat (main_word, main_options_s);
	}


	/**************************** xjv-sikuiim ************************************/
	/*********                    xjv-sikuiim                         ************/
	/**************************** xjv-sikuiim ************************************/


	// smooth the counts using the Good-Turing formula.
	void hxlq_sikuiim () {
		LOOPVECTOR (;, sikuiim_e, i) {
			sikuiim_e[i] = (Sikui)mone_mony_arkim_miloniim.goodturing_count(sikuiim_e[i]);
			//sikuiim_nd[i] = (Sikui)mone_mony_nituxim_domim.goodturing_count(sikuiim_nd[i]);
			sikuiim_nz[i] = (Sikui)mone_mony_nituxim_zhim.goodturing_count(sikuiim_nz[i]);
		}
	}

	
	void jqll_sikuiim (MorphOptionsWithSikui main_options_s) {
		atxel (sikuiim_mjuqllim,main_options_s.count());

		normalize (sikuiim_nd,SIKUI1R,1.);
		normalize (sikuiim_e,SIKUI1R,1.);
		multiply_ew (sikuiim_nd,sikuiim_e);
		set(sikuiim_mjuqllim,sikuiim_nd,mjql_nd);
		normalize (sikuiim_mjuqllim,SIKUI1R,1.);

		normalize (sikuiim_nz,SIKUI1R,1.);
		normalize (sikuiim_e,SIKUI1R,1.);
		multiply_ew (sikuiim_nz,sikuiim_e);
		add(sikuiim_mjuqllim,sikuiim_nz,mjql_nz);
		normalize (sikuiim_mjuqllim,SIKUI1R,1.);

		normalize (sikuiim_end,SIKUI1R,1.);
		add (sikuiim_mjuqllim,sikuiim_end, mjql_end);
		normalize (sikuiim_mjuqllim,SIKUI1R,1.);

		normalize (sikuiim_enz,SIKUI1R,1.);
		add (sikuiim_mjuqllim,sikuiim_enz, mjql_enz);
		normalize (sikuiim_mjuqllim,SIKUI1R,1.);

		add (sikuiim_mjuqllim,sikuiim_yidua, mjql_yidua);
	}



	// calculate sikuiim of all the analyses of the main_word, and adkn the database
	virtual void xjv_sikuiim (CStr main_word, bool mhr) {
		MorphOptionsWithSikui main_options_s = hmyda_hmqorv.item(main_word);
		if (main_options_s.count()<=1)  return;      // no need to calculate probabilities
		if (!xijuv_jaqet_meod) logfile << endl << main_word << (mhr? "| ": ": ");
		sfor_sikuiim (main_word, main_options_s, mhr);
		if (!xijuv_jaqet_meod) logfile << "GLOBAL  enz " << sikuiim_enz << "  end " << sikuiim_end << "  e " << sikuiim_e << "  nd " << sikuiim_nd  << "  nz " << sikuiim_nz << "  yidua " << sikuiim_yidua << endl;
		//hxlq_sikuiim ();    // use the Good-Turing formula
		//if (!xijuv_jaqet_meod) logfile << "GLOBAL  en " << sikuiim_end << "  e " << sikuiim_e << "  n " << sikuiim_nd << endl;
		jqll_sikuiim (main_options_s);
		if (!xijuv_jaqet_meod) logfile << "GLOBAL  zhut " << sikuiim_enz << "  en " << sikuiim_end << "  e " << sikuiim_e << "  n " << sikuiim_nd  << "  nz " << sikuiim_nz << "  yidua " << sikuiim_yidua << "  sh\"k " << sikuiim_mjuqllim << endl;
		main_options_s.set (sikuiim_mjuqllim);
		hmyda_hmqorv.insert (main_word,main_options_s);   // adkn the database
		if (!xijuv_jaqet_meod) logfile << " " << main_options_s << endl;
	}

	void xjv_sikuiim_lkol_hmilim () {
		adkn_monim();
		LOOP_OVER_ALL_THE_WORDS(cursor) {
			if (!xijuv_jaqet)
				cerr << cursor.string() << " ";
			xjv_sikuiim (cursor.string().str, /* mhr:= */ true);
		}
	}


	bool contains_sbwsast (MorphOptionsWithSikuiCR theoptions, MorphInfoCR theinfo) {
		LOOPVECTOR (;,theoptions,i)
			if (are_SBW_SA_ST(theoptions.info(i),theinfo)) return true;
		return false;
	}

	void xjv_sikuiim_lkol_hmilim_im_sbwsast (MorphInfoCR theinfo) {
		FullwordArray& the_fullword_array = the_bw2fw_map.fullword_array(theinfo.baseword());
		bool there_are_many_fullwords = the_fullword_array.count() > 10;
		if (there_are_many_fullwords) 
			adkn_monim(); 
		for (FullwordArray::Cursor cursor(the_fullword_array); cursor.isvalid(); ++cursor) {
			CStr cur_fullword = cursor.item();
			MorphOptionsWithSikui& curoptions = *(hmyda_hmqorv.itemp(cur_fullword));
			if (contains_sbwsast(curoptions,theinfo)) {
				curoptions.set_sikui_axid();
				xjv_sikuiim (cur_fullword, there_are_many_fullwords );
			}
		}
	}



	bool contains_sbw (MorphOptionsWithSikuiCR theoptions, MorphInfoCR theinfo) {
		LOOPVECTOR (;,theoptions,i)
			if (are_SBW(theoptions.info(i),theinfo)) return true;
		return false;
	}

	void xjv_sikuiim_lkol_hmilim_im_sbw (MorphInfoCR theinfo) {
		FullwordArray& the_fullword_array = the_bw2fw_map.fullword_array(theinfo.baseword());
		bool there_are_many_fullwords = the_fullword_array.count() > 3;
		if (there_are_many_fullwords) 
			adkn_monim(); 
		for (FullwordArray::Cursor cursor(the_fullword_array); cursor.isvalid(); ++cursor) {
			xjv_sikuiim (cursor.item(), there_are_many_fullwords );
		}
	}


	/****************** Database I/O ******************************/
	/********           Database I/O                  *************/
	/****************** Database I/O ******************************/


	GlobalDatabase(): 
		hmyda_hmqorv(), 
		hmyda_hamiti(),
		mone_milim_jlmot(),
		mone_arkim_miloniim(),
		mone_nituxim_domim(),
		mone_nituxim_zhim(),
		mone_mony_arkim_miloniim(SIKUI1),
		//mone_mony_nituxim_domim(SIKUI1),
		mone_mony_nituxim_zhim(SIKUI1),
		sikuiim_enz(20),
		sikuiim_end(20),
		sikuiim_e(20),   
		sikuiim_nd(20),
		sikuiim_nz(20),
		sikuiim_yidua(20),
		sikuiim_mjuqllim(20)
		{ 
			xijuv_jaqet = xijuv_jaqet_meod = false;
			idkun_mony_nituxim = true;
			mjql_e = 1;
			mjql_nd = 1;
			mjql_nz = 1;
			mjql_end = 1;
			mjql_enz = 1;
			mjql_yidua = 0;
		}


	void log_the_database (ofstream& logfile) {
		//hmyda_hmqorv.remove(":");
		//hmyda_hamiti.remove(":");
		//mone_milim_jlmot.remove(":");
		logfile << "% total_word_count:" << endl;  
		logfile << total_word_count << endl;  
		logfile << "% hmyda_hmqorv:" << endl;  
		writeln (logfile, hmyda_hmqorv);
		logfile << "% mone_milim_jlmot:" << endl;  
		writeln (logfile, mone_milim_jlmot);
		logfile << "% mone_arkim_milonyim (lfi xlq-dibr):" << endl;  
		writeln (logfile, mone_arkim_miloniim);
		logfile << "% mone_nituxim_domim:" << endl;  
		writeln (logfile, mone_nituxim_domim);
		logfile << "% mone_nituxim_zhim:" << endl;  
		writeln (logfile, mone_nituxim_zhim);
		logfile << "% mone_mony_arkim_miloniim:" << endl;  
		writeln (logfile, mone_mony_arkim_miloniim,"P ");
		//logfile << "% mone_mony_nituxim_domim:" << endl;  
		//writeln (logfile, mone_mony_nituxim_domim,"P ");
		logfile << "% mone_mony_nituxim_zhim:" << endl;  
		writeln (logfile, mone_mony_nituxim_zhim,"P ");
		logfile << "% hmyda_hamiti:" << endl;  
		writeln (logfile, hmyda_hamiti);
	}

	void log_the_database_qcr (ofstream& logfile) {
		logfile << "% total_word_count:" << endl;  
		logfile << 0 << endl;  
		logfile << "% hmyda_hmqorv:" << endl;  
		logfile << "{}" << endl;
		logfile << "% mone_milim_jlmot:" << endl;  
		logfile << "{}" << endl;
		logfile << "% mone_arkim_milonyim (lfi xlq-dibr):" << endl;  
		writeln (logfile, mone_arkim_miloniim);
		logfile << "% mone_nituxim_domim:" << endl;  
		writeln (logfile, mone_nituxim_domim);
		logfile << "% mone_nituxim_zhim:" << endl;  
		writeln (logfile, mone_nituxim_zhim);
		logfile << "% mone_mony_arkim_miloniim:" << endl;  
		writeln (logfile, mone_mony_arkim_miloniim,"P ");
		//logfile << "% mone_mony_nituxim_domim:" << endl;  
		//writeln (logfile, mone_mony_nituxim_domim,"P ");
		logfile << "% mone_mony_nituxim_zhim:" << endl;  
		writeln (logfile, mone_mony_nituxim_zhim,"P ");
		logfile << "% hmyda_hamiti:" << endl;  
		logfile << "{}" << endl;
	}

	void atxl_myda_amiti() {
		LOOP_OVER_ALL_THE_WORDS(cursor) {
			CStr cur_fullword = cursor.string().str;
			MorphOptionsWithSikui cur_options; 
			duplicate (cur_options,hmyda_hmqorv.item(cur_fullword));
			cur_options.zero();
			hmyda_hamiti.insert(cur_fullword,cur_options);
		}
	}

	void atxl_mqovc_log (CStr analysis_with_sikui_filename) {
		ifstream input;
		open_infile_with_messages (analysis_with_sikui_filename,input);
		cerr << "\treading total_word_count:" << endl;
		skip_comments(input,'%');
		assert (OK==read(input,total_word_count));
		cerr << "\treading hmyda_hmqorv:" << endl;
		skip_comments(input,'%');
		assert (OK==read(input,hmyda_hmqorv));
		cerr << "\treading mone_milim_jlmot:" << endl;
		skip_comments(input,'%');
		assert (OK==read(input,mone_milim_jlmot));	
		cerr << "\treading mone_arkim_miloniim:" << endl;
		skip_comments(input,'%');
		assert (OK==read(input,mone_arkim_miloniim));
		cerr << "\treading mone_nituxim_domim:" << endl;
		skip_comments(input,'%');
		assert (OK==read(input,mone_nituxim_domim));
		cerr << "\treading mone_nituxim_zhim:" << endl;
		skip_comments(input,'%');
		assert (OK==read(input,mone_nituxim_zhim));
		cerr << "\treading mone_mony_arkim_miloniim:" << endl;
		skip_comments(input,'%');
		assert (OK==read(input,mone_mony_arkim_miloniim,"P "));
		//cerr << "\treading mone_mony_nituxim_domim:" << endl;
		//skip_comments(input,'%');
		//assert (OK==read(input,mone_mony_nituxim_domim,"P "));
		cerr << "\treading mone_mony_nituxim_zhim:" << endl;
		skip_comments(input,'%');
		assert (OK==read(input,mone_mony_nituxim_zhim,"P "));
		cerr << "\treading hmyda_hamiti:" << endl;
		skip_comments(input,'%');
		assert (OK==read(input,hmyda_hamiti));
	}

	void atxl_mqovc_txt (CStr input_filename, Index from_wordnum, Index to_wordnum) {
		ifstream input;
		open_infile_with_messages (input_filename,input);
		cerr << endl << "counting words in input file " << input_filename << " from word #" << from_wordnum << " to word #" << to_wordnum << endl;
		mone_milim_jlmot.count_words_in_file(input, from_wordnum, to_wordnum);
		open_infile_with_messages (input_filename,input);
		default_heleq_diber=JEM_PRATI;
		DOx(hmyda_hmqorv.analyze_words_in_file (input, from_wordnum, to_wordnum));
		total_word_count = mone_milim_jlmot.wordcount();
		mone_milim_jlmot.remove("*");
		hmyda_hmqorv.remove("*");
		atxl_myda_amiti();
	}

	void atxl_mnitux_nkon (CStr input_filename, CStr output_filename, Index from_wordnum, Index to_wordnum) {
		if (mone_milim_jlmot.wordcount()<=1)
			atxl_mqovc_txt(input_filename, from_wordnum, to_wordnum);
		ifstream correct_analysis, input;
		cerr << "reading correct output file " << output_filename << " from word #" << from_wordnum << " to word #" << to_wordnum << endl;
		open_infile_with_messages (output_filename,correct_analysis);
		open_infile_with_messages (input_filename,input);
		StringTemp curword(30);
		MorphInfo curanalysis;
		for (Index wordnum=0; wordnum<to_wordnum; ++wordnum) {
			skip_spaces_and_stars(correct_analysis);  
			if (correct_analysis.eof()) break;
			DOEOFx (read(correct_analysis,curanalysis));
			skip_spaces_and_stars(input);			
			DOEOFx (curword.readword(input));

			if (wordnum < from_wordnum)   // skip words until from_wordnum;
				continue;

			if (wordnum%100==0) cerr << (wordnum/100) << " " << curword << " ";
			if (adkn(curword.str,curanalysis) != OK) {
				//cerr << endl << "$GIAH BMILH " << curword << ": HNITUX " << curanalysis << " LA NMCA!";
				lexlogfile << curword << ": " << curanalysis << " %  word#" << wordnum << endl;
			}
		}
		correct_analysis.close();
		input.close();
	}


	void atxl (CStr input_filename, CStr analysis_counter_filename, CStr analysis_with_sikui_filename) {
		ifstream input;
		if (analysis_with_sikui_filename!=NULL) 
			atxl_mqovc_log (analysis_with_sikui_filename);
		else 
			atxl_mqovc_txt (input_filename,0,0x7ffffff);
		if (analysis_counter_filename!=NULL) 
			qra_mone_nituxim_domim (analysis_counter_filename);
	}
}; // end of class GlobalDatabase

GlobalDatabase globaldb;	// The global DB holds the data about the whole corpus.


/***************************************************************************/
/**********        atxl the databases                              ********/
/***************************************************************************/


void atxl_bw2fw_map_global() {
	for (MorphOptionsWithSikuiTrie::Cursor cursor(globaldb.hmyda_hmqorv);  cursor.isvalid();  ++cursor) {
		CStr cur_fullword = cursor.string().duplicate();
		MorphOptionsWithSikuiCR cur_options = cursor.data();
		LOOPVECTOR (;,cur_options,i) 
			the_bw2fw_map.insert (cur_options.info(i).baseword(), cur_fullword);
	}
}


void atxl_global_database (CStr input_filename, CStr analysis_with_sikui_filename) {
	globaldb.atxl(input_filename, NULL, analysis_with_sikui_filename);
	atxl_bw2fw_map_global();
}

void qra_nitux_nakon_global (CStr input_filename, CStr output_filename, int from_wordnum, int to_wordnum) {
	globaldb.atxl_mnitux_nkon(input_filename, output_filename, from_wordnum, to_wordnum);
}

void qra_mone_nituxim_domim_global (CStr input_filename) {
	globaldb.qra_mone_nituxim_domim(input_filename);
}



void set_mjqlim_global (double mjql_end) {
	globaldb.mjql_end = mjql_end;
}



/***************************************************************************/
/**********        adkn databases                                  ********/
/***************************************************************************/


MorphOptionsWithSikui analysis_with_approximate_sikuiim (CStr hamila) {
	return globaldb.analysis_with_approximate_sikuiim(hamila);
}

MorphOptionsWithSikui independent_analysis_with_approximate_sikuiim (CStr hamila) {
	return globaldb.independent_analysis_with_approximate_sikuiim(hamila);
}


status adkn_database (CStr hamila, MorphInfoCR hanitux) {
	return globaldb.adkn (hamila, hanitux); 
}

status adkn_database (CStr hamila, MorphOptionsWithSikuiCR kol_hanituxim) {
	return globaldb.adkn (hamila,kol_hanituxim);
}


status havr_mila_axt_mhmyda_hmqorv_lmyda_hamiti (CStr hamila, MorphOptionsWithSikuiCR the_realistic_options) {
	return globaldb.havr_mila_axt_mhmyda_hmqorv_lmyda_hamiti (hamila, the_realistic_options); 
}

void ktov_global_database (CStr output_filename) { 
	ofstream output;
	open_outfile (output_filename, output, ios::trunc);
	if (ktov_et_kl_tocaot_jlv_hmila)
		globaldb.log_the_database(output); 
	else
		globaldb.log_the_database_qcr(output); 
	output.close();
}

void xjv_sikuiim_lkol_hmilim_global () { 
	open_logfile(2);
	logfile << "globaldb.xjv_sikuiim_lkol_hmilim()" << endl;
	globaldb.xjv_sikuiim_lkol_hmilim();  
	logfile.close();
}

void xjv_sikuiim_lkol_hmilim_im_sbwsast (MorphInfoCR theinfo) {
	globaldb.xjv_sikuiim_lkol_hmilim_im_sbwsast(theinfo);
}


void xjv_sikuiim_lkol_hmilim_im_sbw (MorphInfoCR theinfo) {
	globaldb.xjv_sikuiim_lkol_hmilim_im_sbw(theinfo);
}

void xjv_sikuiim (CStr main_word) {
	globaldb.xjv_sikuiim (main_word,false); }

void xjv_sikuiim_mhr (CStr main_word) {
	globaldb.xjv_sikuiim (main_word,true);        }


/***************************************************************************/
/**********        TEST ALGHIST                                     ********/
/***************************************************************************/

#ifdef TEST_ALGHIST

#include <cpplib/hcs.hxx>
#include <cpplib/sentence.h>
#include <morph/corpus.h>

#define LOG_PATH "..\\..\\..\\harc"

ofstream hcs_logfile;

Corpus the_text;


void atxl_klali_01() {
	initialize_the_analyzer(log_path,log_path);
	atxl_global_database(filename(variable(0),"txt").str, NULL);
	open_outfile (filename(variable(0),"nt0").str,logfile,0); globaldb.log_the_database(logfile); logfile.close();

	if (option('n')!=NULL)
		globaldb.atxl_mnitux_nkon(filename(option('n'),"txt").str, filename(option('n'),"to").str);

	globaldb.xijuv_jaqet = true;
	qjr_myda_mqomi_lmyda_globli();
}

void atxl_klali() {
	initialize_the_analyzer(log_path,log_path);
	atxl_global_database(NULL, filename(variable(0),"nt0").str);

	if (option('n')!=NULL)
		globaldb.atxl_mnitux_nkon(filename(option('n'),"txt").str, filename(option('n'),"to").str);

	globaldb.xijuv_jaqet = true;
	qjr_myda_mqomi_lmyda_globli();
}





class MijqalimSearcher_klali: public HillClimbingSearcher<double> {

	void init_the_parameters() {}

	double score () const {
		//globaldb.mjql_sbwsast=max(par[0],0.); globaldb.mjql_sbw=max(par[1],0.); globaldb.mjql_sast=max(par[2],0.); globaldb.mjql_yidua=max(par[3],0.); 
		globaldb.mjql_end=max(par[0],0.); 
		globaldb.hmyda_hmqorv.set_sikui_axid();
		xjv_sikuiim_lkol_hmilim_global();

		the_text.ntx_qlt_1();
		the_text.ntx_qlt_2();
		return the_text.score();
	}


	void log (double thestep, double thescore) {
		write(hcs_logfile,par);   write(cerr,par);
		LOG(hcs_logfile," " << thestep << ": " << thescore<<endl);  
	}
	
	void finish() {
		open_errorlogfile(log_path);
		score();
		the_text.log_ms_jgiot();
		close_errorlogfile();
		ktov_global_database(filename(variable(0),option('n')==NULL? "nts": "nt1").str);
	}

public:
	MijqalimSearcher_klali(): HillClimbingSearcher<double>(1) {}

	void run (double step0, double step1, double m0 /*,double m1,double m2,double m3*/) {
		par[0]=m0; //par[1]=m1; par[2]=m2; par[3]=m3;
		open_outfile (LOG_PATH "loghcs.ma2",hcs_logfile,0);
		the_text.atxl (500);
		CStr mamr_lbdiqa = variable(1)==NULL? variable(0): variable(1);
		the_text.qra_qlt (filename(mamr_lbdiqa,"txt").str);
		the_text.qra_nituxim_nkonim (filename(mamr_lbdiqa,"to").str);
		run_algorithm_a(step0,step1);
		hcs_logfile.close();
	}

};

void test_klali() {
	MijqalimSearcher_klali the_searcher;
	the_searcher.run(10,12, 0);
	//2.0708 0 1.1338 0.440251 0.0107374
}





void main (int argc, char* argv[]) {
	log_path = LOG_PATH;
	parse_command_line(argc,argv,1,2,"n","mks");
	if (swtch('k')) {       // klali
		if (swtch('s'))  atxl_klali(); else  atxl_klali_01();
		test_klali();
	}
}

#endif

