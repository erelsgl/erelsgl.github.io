/* sntncas.h -- mnattex taxbiri &imm sikkuyim. */

#ifndef __SNTNCAS_H
#define __SNTNCAS_H

#include <morph/sntncinf.h>
#include <cpplib/array2.hxx>
#include <morph/soi.h>

struct SentenceInfoWithCiyun;
typedef const SentenceInfoWithCiyun& SentenceInfoWithCiyunCR;
struct SentenceInfoWithCiyun: public SentenceInfo {
	double ciyun_cimcumi;
	SentenceInfoWithCiyun(): SentenceInfo(), ciyun_cimcumi(0) {}
	SentenceInfoWithCiyun(Size thesize): SentenceInfo(thesize), ciyun_cimcumi(0) {}
	void operator= (int ZERO) { ciyun_cimcumi=0; SentenceInfo::operator=(0); }
	void truncate() { 
		SentenceInfo::truncate();  
		ciyun_cimcumi=0; 
	}
	status append(MorphInfoCR theinfo, double theciyun=0) {
		ciyun_cimcumi += theciyun;
		return SentenceInfo::append(theinfo);
	}
	status append(SentenceInfoWithCiyunCR theinfo) {
		ciyun_cimcumi += theinfo.ciyun_cimcumi;
		return SentenceInfo::append(theinfo);
	}
};

inline void write (ostream& out, SentenceInfoWithCiyunCR theinfo, Format format="") {
	write (out, (SentenceInfoCR)theinfo);  out << " [";
	write (out, theinfo.ciyun_cimcumi); out << "]";
}

//inline status read  (istream& in, SentenceInfoWithCiyun& theinfo, Format format="") {
//	return read (in,(Array2<MorphInfo>)theinfo, Format(" ")); }

inline void duplicate (SentenceInfoWithCiyun& to, SentenceInfoWithCiyunCR from) {
	duplicate( (SentenceInfo&)to , (SentenceInfoCR)from ); 
	to.ciyun_cimcumi = from.ciyun_cimcumi;
}



typedef Array2<SentenceInfoWithCiyun> SentenceOptions;
typedef const SentenceOptions& SentenceOptionsCR;


void initialize_the_sentence_analyzer (CStr path_to_cimcumim);

void natax (SentenceInfoWithCiyunCR the_sentence, SentenceOptions& the_sentence_options);
	// qelet: nitux curani 1 lkoll milla bami$pa@.
	// pelet: ma&arak ("the_sentence_options") $e-kolel ^et koll ha-nituxim ha-taxbiriyim 
	//        ha-^ep$ariyim la-mi$pa@.

void natax (ArrayOfMOWSCR the_eachword_options, SentenceOptions& the_sentece_options);
	// qelet: ma&arak ("the_eachword_options") $e-kolel ^et koll ha-nituxim ha-morpologiyim 
	//        l-koll mila ba-mi$pa@.
	// pelet: ma&arak ("the_sentence_options") $e-kolel ^et koll ha-nituxim ha-taxbiriyim 
	//        ha-^ep$ariyim la-mi$pa@.

void natax2 (ArrayOfMOWSCR the_eachword_options, Array2<SentenceInfo>& the_sentece_options, double mijqal_taxbir=1.);
	// qelet: ma&arak ("the_eachword_options") $e-kolel ^et koll ha-nituxim ha-morpologiyim 
	//        l-koll mila ba-mi$pa@.
	// pelet: ma&arak ("the_sentence_options") $e-kolel ^et koll ha-nituxim ha-taxbiriyim 
	//        ha-^ep$ariyim la-mi$pa@.


double ciyun_taxbiri (SentenceInfoWithCiyunCR the_partially_analyzed_sentence);

#endif
