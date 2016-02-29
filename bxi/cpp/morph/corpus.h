/*
 * File morph/corpus.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/corpus.h
 */

/* corpus.h -- An analyzed corpus structure */

#ifndef __CORPUS_H
#define __CORPUS_H

#include <morph/soi.h>
#include <cpplib/sentence.h>





struct Corpus {
// ha-ma&arakim
	Array2<char*> haqlt;
	Vector1<bool> hamila_loazit;  // ^im ha-milla lo&azit - ^ei cork lnattex ^otah.
	Vector1<MorphOptionsWithSikui> kol_hanituxim_laqlt;
	Vector1<MorphInfo> hanitux_jelanu_laqlt;       
	Array2<MorphInfo> hanitux_hanakon_laqlt;             // m$amme$ l-bdiqat ha-nitux $ellanu

	Size godel_haqlt() const { return min (hanitux_hanakon_laqlt.count(), hanitux_jelanu_laqlt.count(), haqlt.count()); }
	Index index_hanitux_jelanu_laqlt(Index w) const { return kol_hanituxim_laqlt[w].indexof(hanitux_jelanu_laqlt[w]); }

	void hosf_mila_laqlt (char* hamila, bool loazit) {
		haqlt.append(hamila);
		hamila_loazit.append(loazit);
	}

	void atxl (Size ms_milim_baqlt);
	Corpus(Size ms_milim_baqlt=0) { atxl(ms_milim_baqlt); }

// $igrot ha-qri^a
	void qra_qlt (CStr input_filename, Index from_wordnum=0, Index to_wordnum=0x7fffffff);                   // qra^ ^et koll ha-qobc
	status qra_qlt (istream& input, uint article_limit);  // qra^ lo^ yoter mi-"article_limit" ma^amarim
	void qra_pelet_xelqi (CStr partial_output_filename);
	void qra_nituxim_nkonim (CStr correct_analysis_filename, Index from_wordnum=0, Index to_wordnum=0x7fffffff);
	void qra_mxrozt (CStr hmxrozt);  // qra^ milim mitok ha-maxrozet. bein koll 2 millim ye$ ' '.

// $igrot ha-nittux
	void ntx_qlt_1(Index start_from_word_num=0);  // m^atxl ^et "kol_hanituxim_laqlt"
	void ntx_qlt_1_axid(Index start_from_word_num=0);  // m^atxl ^et "kol_hanituxim_laqlt"
	void ntx_qlt_2(Index start_from_word_num=0);  // m^atxl ^et "hanitux_jelanu_laqlt"
	void ntx_qlt_2_baqrai(Index start_from_word_num=0);  // m^atxl ^et "hanitux_jelanu_laqlt"
	void ntx_qlt_1(CStr hamila);                  // mtaqqen ^et "kol_hanituxim_laqlt" ba-mqomot $e-mat^imim l-"hamila"

// ha$wa^a bein "hanitux_jelanu_laqlt" lbein "hanitux_hanakon_laqlt"
	uint ms_milim;
	uint ms_tauyot;
	uint ms_tauyot_baerek_hamiloni;
	void hajwe_nituxim ();
	void hajwe_nituxim (Array2<Index>& indexei_tauyot);
	void hajwe_nituxim (SentenceInfoCR hanitux_jelanu_lamijpat, SentenceInfoCR hanitux_hanakon_lamijpat, SentenceCR hamijpat);
	double score ();
	double score (ArrayOfMOWSCR kol_hanituxim_lamijpat, SentenceInfo hanitux_hanakon_lamijpat, Index& w);

	void log_ms_jgiot ();

// @ippul b-mi$pa@im
	void qra_mijpat_mehamaarak (Index w, SentenceInfo& hanitux_hanakon_lamijpat);
	void qra_mijpat_mehamaarak (Index w, ArrayOfMOWS& kol_hanituxim_lamijpat, SentenceInfo& hanitux_hanakon_lamijpat);
	void qra_mijpat_mehamaarak (Index w, ArrayOfMOWS& kol_hanituxim_lamijpat, Array2<CStr>& hamijpat);
	void ktov_mijpat_lamaarak (Index w, ArrayOfMOWSCR kol_hanituxim_lamijpat);
	void ktov_mijpat_lamaarak (Index w, SentenceInfoCR hanitux_jelanu_lamijpat);
	void hajwe_mijpat_lamaarak (Index w, SentenceInfoCR hanitux_jelanu_lamijpat);

// $igrot ha-ktiva l-qovc
	void ktov_et_hanitux_jelanu(ostream& out, Index from=0, Index to=0 /* i.e. all. */ ) const;
	CStr arkim_milonyim_mhanitux_jelanu(Index from=0, Index to=0 /* i.e. all. */ ) const;
};










#endif
