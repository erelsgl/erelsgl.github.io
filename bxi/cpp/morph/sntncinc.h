/*
 * File morph/sntncinc.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/sntncinc.h
 */

/* sntncinc.h -- SentenceInfoWithCiyun */

#ifndef __SNTNCINC_H
#define __SNTNCINC_H

#include <morph/sntncinf.h>

#define MIN_CIYUN -1000
#define MAX_CIYUN 0

double ciyun_taxbiri (MorphInfoCR);
double ciyun_taxbiri0 (MorphInfoCR);

struct SentenceInfoWithCiyun;
typedef const SentenceInfoWithCiyun& SentenceInfoWithCiyunCR;
struct SentenceInfoWithCiyun: public SentenceInfo {
	void set_ciyun_cimcumi(double theciyun) { my_ciyun_cimcumi=theciyun; } ;    // nobe& me-xuqqei hacimcum $eyacru ^et harkib (bgirsa zo - tamid 0).
	void set_ciyun_curani(double theciyun) { my_ciyun_curani=theciyun; } ;    // nobe& me-xuqqei hacimcum $eyacru ^et harkib (bgirsa zo - tamid 0).
	double ciyun_cimcumi() const { return my_ciyun_cimcumi; } ;    // nobe& me-xuqqei hacimcum $eyacru ^et harkib (bgirsa zo - tamid 0).
	double ciyun_curani() const { return my_ciyun_curani; }    // nobe& me-haciyyunim hacuraniyim $ell hamillim $emarkivot ^et harkib.
	double ciyun_taxbiri() const;    // nobe& me-haq$arim bein hamillim.
	double ciyun_taxbiri0() const;    // nobe& me-haq$arim bein hamillim.
	double ciyun_sofi()  const { return ciyun_cimcumi() + ciyun_curani() + ciyun_taxbiri(); } 

	SentenceInfoWithCiyun(): SentenceInfo(), my_ciyun_cimcumi(0), my_ciyun_curani(0) {}
	SentenceInfoWithCiyun(Size thesize): SentenceInfo(thesize), my_ciyun_cimcumi(0), my_ciyun_curani(0) {}
	void operator= (int ZERO) { my_ciyun_cimcumi=my_ciyun_curani=0; SentenceInfo::operator=(0); }
	void truncate() { 
		SentenceInfo::truncate();  
		my_ciyun_cimcumi=my_ciyun_curani=0; 
	}
	status append(MorphInfoCR theinfo, double theciyun_cimcumi=0, double theciyun_curani=0) {
		my_ciyun_cimcumi += theciyun_cimcumi;
		my_ciyun_curani += theciyun_curani;
		return SentenceInfo::append(theinfo);
	}
	status append(SentenceInfoWithCiyunCR theinfo) {
		my_ciyun_cimcumi += theinfo.ciyun_cimcumi();
		my_ciyun_curani += theinfo.ciyun_curani();
		return SentenceInfo::append(theinfo);
	}
private:
	double my_ciyun_cimcumi, my_ciyun_curani;
};

inline void write (ostream& out, SentenceInfoWithCiyunCR theinfo, Format format="") {
	write (out, (SentenceInfoCR)theinfo);  
	out << " [" 
		<< theinfo.ciyun_cimcumi() << " " 
		<< theinfo.ciyun_curani()  << " " 
		<< theinfo.ciyun_taxbiri() << " "
		<< "]";
}

//inline status read  (istream& in, SentenceInfoWithCiyun& theinfo, Format format="") {
//	return read (in,(Array2<MorphInfo>)theinfo, Format(" ")); }

inline void duplicate (SentenceInfoWithCiyun& to, SentenceInfoWithCiyunCR from) {
	duplicate( (SentenceInfo&)to , (SentenceInfoCR)from ); 
	to.set_ciyun_cimcumi (from.ciyun_cimcumi());
	to.set_ciyun_curani (from.ciyun_curani());
}


#endif
