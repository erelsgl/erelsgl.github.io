/*
 * File morph/corpus.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/corpus.cpp
 */

/* corpus.cpp -- An analyzed corpus structure */

#include <cpplib/ivrit.h>
#include <morph/corpus.h>
#include <morph/hpmaio.h>
#include <morph/alghist.h>
#include <time.h>


bool plt_cfuf, plt_brur, plt_hstbruyot;



/********************************************************************************/
/**********************        Corpus     ***************************************/
/********************************************************************************/

// $igrot ha-qri^a
// $igrot ha-nittux
// ha$wa^a bein "hanitux_jelanu_laqlt" lbein "hanitux_hanakon_laqlt"
// @ippul b-mi$pa@im

/**********************        ^itxul     **********************************/
/**********************        ^itxul     **********************************/

void Corpus::atxl (Size ms_milim_baqlt) {
	haqlt.reset (ms_milim_baqlt);
	hamila_loazit.reset (ms_milim_baqlt);
	hanitux_hanakon_laqlt.reset (ms_milim_baqlt);
}



/**********************        qri^a     **********************************/
/**********************        qri^a     **********************************/


void Corpus::qra_nituxim_nkonim (CStr correct_analysis_filename, Index from_wordnum, Index to_wordnum) {
	open_infile (correct_analysis_filename,correct_analysis);
	MorphInfo hanitux_hanakon_lamila;
	for (Index wordnum=0; wordnum<to_wordnum; ++wordnum) {
		DOEOFx(read(correct_analysis,hanitux_hanakon_lamila));

		if (wordnum<from_wordnum)   // skip words until from_wordnum;
			continue;

		DOx(hanitux_hanakon_laqlt.append(hanitux_hanakon_lamila));
	}
}

void Corpus::qra_pelet_xelqi (CStr partial_output_filename) {
	open_infile (partial_output_filename,correct_analysis);
	MorphInfo hanitux_hanakon_lamila;
	for (;;) {
		DOEOFx(read(correct_analysis,hanitux_hanakon_lamila));
		DOx(hanitux_jelanu_laqlt.append(hanitux_hanakon_lamila));
	}
}

void Corpus::qra_qlt (CStr input_filename, Index from_wordnum, Index to_wordnum) {
	LOOPVECTOR(;,haqlt,w) vecfree(haqlt[w]);
	haqlt.truncate();
	hamila_loazit.truncate();
	open_infile_with_messages (input_filename,input);
	StringTemp hamila(30);
	for (Index wordnum=0; wordnum<to_wordnum;) {
		ws(input);
		DOEOFx(hamila.readword(input));

		if (hamila.len==0) continue;
		if (hamila[0]=='*') continue;  // End Of Article symbol

		++wordnum;
		if (wordnum<=from_wordnum)   // skip words until from_wordnum;
			continue;

		hosf_mila_laqlt(hamila.duplicate(), false);
	}
}

status Corpus::qra_qlt (istream& input, uint article_limit) {
	LOOPVECTOR(;,haqlt,w) vecfree(haqlt[w]);
	haqlt.truncate();
	hamila_loazit.truncate();
	StringTemp hamila(30);
	for (;;) {
		//input.ipfx();
		DOEOFr(hamila.readword(input));
		if (hamila.len==0) continue;
		if (hamila[0]=='*') { // End Of Article symbol
			article_limit--;
			if (article_limit==0) return OK;
			continue;  
		}
		hosf_mila_laqlt (hamila.duplicate(), false);
	}
	if (haqlt.count()==0) return EOF;
	else return OK;
}


void Corpus::qra_mxrozt (CStr hmxrozt) {
// qra^ milim mitok ha-maxrozet. 
// "hmxrozt" ntuna b-^otiyot &ibriyon (&' "ivrit.h"). 
// bein koll 2 millim ye$ ' '.
	LOOPVECTOR(;,haqlt,w) vecfree(haqlt[w]);
	
	haqlt.truncate();
	hamila_loazit.truncate();

	StringTemp hamila(30);
	bool hamila_hanokxit_loazit = false;

	char c;
	for (Index i=0; i<strlen(hmxrozt); ++i) {
		c = hmxrozt[i];
		if (c==' ' && hamila.len > 0) {
			hosf_mila_laqlt(hamila.duplicate(), hamila_hanokxit_loazit);
			hamila.truncate();
			hamila_hanokxit_loazit = false;
		}
		else if (is_simn_pisuq(c)) {
			if (hamila.len==0) {  // simman pissuq b-txilat milla
				hamila.append(c);
				hosf_mila_laqlt(hamila.duplicate(), hamila_hanokxit_loazit);
				hamila.truncate();
				hamila_hanokxit_loazit = false;
			}
			else if (!is_heb(hmxrozt[i+1])) {  // simman pissuq b-sof milla
				hosf_mila_laqlt(hamila.duplicate(), hamila_hanokxit_loazit);
				hamila.truncate();
				hamila_hanokxit_loazit = false;
				hamila.append(c);
			}
			else {   // simman pissuq b-^emca& milla
				hamila.append(c);
			}
		}
		else if (c!=' ') {
			hamila.append(ot_ivrit_ltatiq_ltini(c));
			if (('A' <=c && c <= 'Z') || ('a' <=c && c <= 'z'))
				hamila_hanokxit_loazit = true;
		}

	}
	if (hamila.len > 0)
		hosf_mila_laqlt(hamila.duplicate(), hamila_hanokxit_loazit);
}


/**********************        nittux     **********************************/
/**********************        nittux     **********************************/

void Corpus::ntx_qlt_1(Index start_from_word_num) {
	free (kol_hanituxim_laqlt);
	kol_hanituxim_laqlt.reset (haqlt.count());
	for (Index w=start_from_word_num; w<haqlt.count(); ++w) {
		CStr hamila = haqlt[w];
		MorphOptionsWithSikui kol_hanituxim_lamila = hamila_loazit[w]?		
			natax_kjem_prati_llo_txiliot (hamila):
			independent_analysis_with_approximate_sikuiim (hamila);
		kol_hanituxim_laqlt[w] = kol_hanituxim_lamila;
	}
}

void Corpus::ntx_qlt_1_axid(Index start_from_word_num) {
	free (kol_hanituxim_laqlt);
	kol_hanituxim_laqlt.reset (haqlt.count());
	for (Index w=start_from_word_num; w<haqlt.count(); ++w) {
		CStr hamila = haqlt[w];
		MorphOptionsWithSikui kol_hanituxim_lamila = hamila_loazit[w]?		
			natax_kjem_prati_llo_txiliot (hamila):
			independent_analysis_with_approximate_sikuiim (hamila);
		kol_hanituxim_lamila.set_sikui_axid();
		kol_hanituxim_laqlt[w] = kol_hanituxim_lamila;
	}
}

void Corpus::ntx_qlt_1(CStr hamila) {
	Index start_from_word_num=0;
	for (Index w=start_from_word_num; w<haqlt.count(); ++w) {
		if (identical(hamila,haqlt[w])) {
			MorphOptionsWithSikui kol_hanituxim_lamila = independent_analysis_with_approximate_sikuiim (hamila);
			kol_hanituxim_laqlt[w] = kol_hanituxim_lamila;
		}
	}
}


void Corpus::ntx_qlt_2(Index start_from_word_num) {
	hanitux_jelanu_laqlt.reset (haqlt.count());
	for (Index w=start_from_word_num; w<haqlt.count(); ++w) {
		MorphOptionsWithSikui& kol_hanituxim_lamila = kol_hanituxim_laqlt[w];
		if (kol_hanituxim_lamila.isempty()) continue;
		hanitux_jelanu_laqlt[w] = kol_hanituxim_lamila.info_with_greatest_sikui();
	}
}

void Corpus::ntx_qlt_2_baqrai(Index start_from_word_num) {
	srand( (unsigned)time( NULL ) );
	hanitux_jelanu_laqlt.reset (haqlt.count());
	for (Index w=start_from_word_num; w<haqlt.count(); ++w) {
		MorphOptionsWithSikui& kol_hanituxim_lamila = kol_hanituxim_laqlt[w];
		if (kol_hanituxim_lamila.isempty()) continue;
		int aqrai = rand() % kol_hanituxim_lamila.count();
		hanitux_jelanu_laqlt[w] = kol_hanituxim_lamila.info(aqrai);
	}
}



/**********************        mi$pa@im     **********************************/
/**********************        mi$pa@im     **********************************/

#define SOF_MIJPAT(c) (c=='.'||c=='?'||c=='!')

void Corpus::qra_mijpat_mehamaarak (Index w, SentenceInfo& hanitux_hanakon_lamijpat) {
	hanitux_hanakon_lamijpat.truncate();
	for(Index i=0;;++i) {
		CStr hamila = haqlt[w];
		hanitux_hanakon_lamijpat.append(hanitux_hanakon_laqlt[w]);
		++w;
		if (SOF_MIJPAT(hamila[0]) || w>=hanitux_hanakon_laqlt.count()) break;
	}
}

void Corpus::qra_mijpat_mehamaarak (Index w, ArrayOfMOWS& kol_hanituxim_lamijpat, SentenceInfo& hanitux_hanakon_lamijpat) {
	static ArrayOfMOWS kol_hanituxim_lamijpat___ (50);
	kol_hanituxim_lamijpat___.truncate();
	hanitux_hanakon_lamijpat.truncate();
	for(Index i=0;;++i) {
		CStr hamila = haqlt[w];
		kol_hanituxim_lamijpat___.append(kol_hanituxim_laqlt[w]);
		hanitux_hanakon_lamijpat.append(hanitux_hanakon_laqlt[w]);
		++w;
		if (SOF_MIJPAT(hamila[0])) break;
	}
	free(kol_hanituxim_lamijpat);
	duplicate (kol_hanituxim_lamijpat,kol_hanituxim_lamijpat___);
}

void Corpus::qra_mijpat_mehamaarak (Index w, ArrayOfMOWS& kol_hanituxim_lamijpat, Array2<CStr>& hamijpat) {
	static ArrayOfMOWS kol_hanituxim_lamijpat___ (50);
	kol_hanituxim_lamijpat___.truncate();
	hamijpat.truncate();
	for(Index i=0;;++i) {
		if (w>=haqlt.count())  break;   
		CStr hamila = haqlt[w];
		kol_hanituxim_lamijpat___.append(kol_hanituxim_laqlt[w]);
		hamijpat.append(haqlt[w]);
		++w;
		if (SOF_MIJPAT(hamila[0])) break;
	}
	free(kol_hanituxim_lamijpat);
	duplicate (kol_hanituxim_lamijpat,kol_hanituxim_lamijpat___);
}

void Corpus::ktov_mijpat_lamaarak (Index w, ArrayOfMOWSCR kol_hanituxim_lamijpat) {
	LOOPVECTOR (;,kol_hanituxim_lamijpat,i) {
		kol_hanituxim_laqlt[w] = kol_hanituxim_lamijpat[i];
		++w;
	}
}

void Corpus::ktov_mijpat_lamaarak (Index w, SentenceInfoCR hanitux_jelanu_lamijpat) {
	LOOPVECTOR (;,hanitux_jelanu_lamijpat,i) {
		hanitux_jelanu_laqlt[w] = hanitux_jelanu_lamijpat[i];
		++w;
	}
}




/**********************        ha$wa^a     **********************************/
/**********************        ha$wa^a     **********************************/


void Corpus::hajwe_nituxim (SentenceInfoCR hanitux_jelanu_lamijpat, SentenceInfoCR hanitux_hanakon_lamijpat, SentenceCR hamijpat) {
	if (hanitux_jelanu_lamijpat.count() != hanitux_hanakon_lamijpat.count()) {
		if (errorlogfile.is_open()) {
			LOG(errorlogfile,"\nM$P@ $LM $GWI! (" << hamijpat << ")"); }
		return;
	}
	LOOPVECTOR (;,hanitux_jelanu_lamijpat,w) {
		StringTemp hamila = hamijpat.word(w);  if (hamila.len==0) { continue; }
		MorphInfoCR hanitux_jelanu_lamila = hanitux_jelanu_lamijpat[w];
		MorphInfoCR hanitux_hanakon_lamila = hanitux_hanakon_lamijpat[w];
		if (!identical(hanitux_jelanu_lamila,hanitux_hanakon_lamila))  {
			++ms_tauyot;
			if (!identical_baseword(hanitux_jelanu_lamila,hanitux_hanakon_lamila))  ++ms_tauyot_baerek_hamiloni;
			log_the_mistake (ms_milim,haqlt[w], hanitux_jelanu_lamila,hanitux_hanakon_lamila);
		}
		++ms_milim;
	}
}

void Corpus::hajwe_mijpat_lamaarak (Index w, SentenceInfoCR hanitux_jelanu_lamijpat) {
	LOOPVECTOR(;,hanitux_jelanu_lamijpat,i) {
		MorphInfoCR hanitux_jelanu = hanitux_jelanu_lamijpat[i];
		MorphInfoCR hanitux_hanakon = hanitux_hanakon_laqlt[w];
		if (!identical(hanitux_jelanu,hanitux_hanakon))  {
			++ms_tauyot;
			if (!identical_baseword(hanitux_jelanu,hanitux_hanakon))  ++ms_tauyot_baerek_hamiloni;
			log_the_mistake (ms_milim,haqlt[w], hanitux_jelanu,hanitux_hanakon);
		}
		++w;
		++ms_milim;
	}
}

void Corpus::hajwe_nituxim () {
	ms_milim=ms_tauyot=ms_tauyot_baerek_hamiloni=0;
	for(Index w=0; w<godel_haqlt(); ++w) {
		MorphInfoCR hanitux_jelanu = hanitux_jelanu_laqlt[w];
		MorphInfoCR hanitux_hanakon = hanitux_hanakon_laqlt[w];
		if (!identical(hanitux_jelanu,hanitux_hanakon))  {
			++ms_tauyot;
			if (!identical_baseword(hanitux_jelanu,hanitux_hanakon))  ++ms_tauyot_baerek_hamiloni;
			log_the_mistake (ms_milim,haqlt[w], hanitux_jelanu,hanitux_hanakon);
		}
		++ms_milim;
	}
}

void Corpus::hajwe_nituxim (Array2<Index>& indexei_tauyot) {
	indexei_tauyot.truncate();
	ms_milim=ms_tauyot=ms_tauyot_baerek_hamiloni=0;
	for(Index w=0; w<godel_haqlt(); ++w) {
		MorphInfoCR hanitux_jelanu = hanitux_jelanu_laqlt[w];
		MorphInfoCR hanitux_hanakon = hanitux_hanakon_laqlt[w];
		if (!identical(hanitux_jelanu,hanitux_hanakon))  {
			++ms_tauyot;
			log_the_mistake (ms_milim,haqlt[w],hanitux_jelanu,hanitux_hanakon);
			indexei_tauyot.append(w);
		}
		++ms_milim;
	}
}

double Corpus::score (ArrayOfMOWSCR kol_hanituxim_lamijpat, SentenceInfo hanitux_hanakon_lamijpat, Index& w) {
	LOOPVECTOR(double ciyun=0,kol_hanituxim_lamijpat,i) {
		MorphInfoCR hanitux_hanakon = hanitux_hanakon_lamijpat[i];
		MorphOptionsWithSikuiCR kol_hanituxim = kol_hanituxim_lamijpat[i];
		MorphInfoCR hanitux_jelanu = kol_hanituxim.info_with_greatest_sikui();
		if (!identical(hanitux_jelanu,hanitux_hanakon)) {
			Index o = kol_hanituxim.indexof(hanitux_hanakon);
			ciyun = ciyun - 1 + kol_hanituxim.realsikui(o);
			log_the_mistake (w,haqlt[w],hanitux_jelanu,hanitux_hanakon);
		}
		++w;
	}
	return ciyun;
}	

double Corpus::score () {
	double ciyun=0;
	ms_milim=ms_tauyot=ms_tauyot_baerek_hamiloni=0;
	for(Index w=0; w<godel_haqlt(); ++w) {
		MorphInfoCR hanitux_hanakon = hanitux_hanakon_laqlt[w];
		MorphOptionsWithSikuiCR kol_hanituxim = kol_hanituxim_laqlt[w];
		MorphInfoCR hanitux_jelanu = hanitux_jelanu_laqlt[w];
		if (!identical(hanitux_jelanu,hanitux_hanakon)) {
			Index o = kol_hanituxim.indexof(hanitux_hanakon);
			ciyun = ciyun - 1 + kol_hanituxim.realsikui(o);
			++ms_tauyot;
			if (!identical_baseword(hanitux_jelanu,hanitux_hanakon))  ++ms_tauyot_baerek_hamiloni;
			log_the_mistake (w,haqlt[w],hanitux_jelanu,hanitux_hanakon);
		}
		++ms_milim;
	}
	return ciyun;
}



void Corpus::log_ms_jgiot () {
	LOG(errorlogfile,"\nMSPR MILIM KWLL:            " << ms_milim << endl);
	LOG(errorlogfile,"MSPR $GIAWT:                " << ms_tauyot
			<< " (" << (ms_tauyot*100./ms_milim) << "%)" << endl);
	LOG(errorlogfile,"MSPR $GIAWT B&RK MILWNI:    " << ms_tauyot_baerek_hamiloni 
			<< " (" << (ms_tauyot_baerek_hamiloni*100./ms_milim) << "%)" << endl);
	//ktov_bituyim ();
}


/**********************        ktiva l-qovc     **********************************/
/**********************        ktiva l-qovc     **********************************/


void Corpus::ktov_et_hanitux_jelanu(ostream& output, Index from, Index to) const {
	if (to==0)
		to=hanitux_jelanu_laqlt.size();
	for (Index w=from; w<to; ++w) {
		output << (w%10==0? '\n': ' ');
		if (plt_cfuf) 
			hanitux_jelanu_laqlt[w].ktov_cafuf (output);
		else if (plt_brur)
			hanitux_jelanu_laqlt[w].ktov_barur (output, haqlt[w]);
		else if (plt_hstbruyot) {
			MorphOptionsWithSikui kol_hanituxim = kol_hanituxim_laqlt[w];
			output << kol_hanituxim.count() << endl; 
			LOOPVECTOR(;,kol_hanituxim,i)  {
				output << kol_hanituxim.sikui(i) << " ";
				kol_hanituxim.info(i).ktov_cafuf(output);
				output <<" ";     
				kol_hanituxim.info(i).ktov_barur (output, haqlt[w]);
				output << " (";
				kol_hanituxim.info(i).ktov_meforat(output);
				output << ")" <<endl;
			}
		}
	}
}

CStr Corpus::arkim_milonyim_mhanitux_jelanu(Index from, Index to) const {
	if (to==0)
		to=hanitux_jelanu_laqlt.size();
	StringTemp tocaa((to-from+1)*20);
	for (Index w=from; w<to; ++w) {
		bool hamila_hanokxit_loazit = hamila_loazit[w];
		CStr erk_miloni = hanitux_jelanu_laqlt[w].baseword();
		for (Index i=0; i<strlen(erk_miloni)-1; ++i) {
			char ot = erk_miloni[i];
			if (!hamila_hanokxit_loazit)
				ot = to_thilit(ot_ivrit_mtatiq_ltini(ot));
			tocaa.append(ot);
		}
		uchar ot_axrona = erk_miloni[i];
		if (!hamila_hanokxit_loazit)
			ot_axrona = to_thilit(ot_ivrit_mtatiq_ltini(ot_axrona));
		if (!is_simn_pisuq(ot_axrona)) {
			if (hanitux_jelanu_laqlt[w].hu(POAL))
				if (ot_axrona == YUD) 
					ot_axrona = HEY;
			if (i>0 && erk_miloni[i-1]!='"')
				ot_axrona = to_sofit(ot_axrona);
			tocaa.append(ot_axrona);
			tocaa.append(' ');
		}
	}
	return tocaa.finalstr();
}

