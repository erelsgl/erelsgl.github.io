/*
 * File morph/Constituent.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/Constituent.h
 */

/* constituent.h -- an auxiliary structure for SentenceAnalyzer */

#ifndef __CONSTITUENT_H
#define __CONSTITUENT_H


#include <morph/morphsik.h>
#include <cpplib/vector1.hxx>

struct Constituent;
typedef const Constituent& ConstituentCR;
struct Constituent {               
	MorphInfo word;
	Index from, to;
	Vector1<Index> my_option_indices;  // The indices of the options chosen for each word, from "from" to "to".
	double ciyun_cimcumi() const { return my_ciyun_cimcumi; }
	double ciyun_curani() const { return my_ciyun_curani; }
	double ciyun_taxbiri() const { return ::ciyun_taxbiri(word); }
	double ciyun_sofi() const { return my_ciyun_cimcumi + my_ciyun_curani + ciyun_taxbiri(); }
	void set_ciyun_cimcumi(double theciyun) { my_ciyun_cimcumi = theciyun; }
	void set_ciyun_curani(double theciyun) { my_ciyun_curani = theciyun; }
	bool contains (Index wordnum) const { return from<=wordnum && wordnum<to; }
	Constituent(): my_ciyun_cimcumi(0), my_ciyun_curani(0), my_option_indices(0) {}
	Constituent(MorphInfoCR theword, Index thefrom, Index theto):
		word(theword), from(thefrom), to(theto), 
		my_ciyun_cimcumi(0), my_ciyun_curani(0)
		,my_option_indices(theto-thefrom)
		{  }
	Constituent(MorphOptionsWithSikuiCR theword, Index theoptionindex, Index thefrom):
		word(theword.info(theoptionindex)), from(thefrom), to(thefrom+1), 
		my_ciyun_cimcumi(0), 
		my_ciyun_curani(log10(theword.realsikui(theoptionindex)+0.01))
		,my_option_indices(1)
		{
			my_option_indices[0] = theoptionindex;
		}
private:
	double my_ciyun_cimcumi;    // nobe& me-xuqqei hacimcum $eyacru ^et harkib (bgirsa zo - tamid 0).
	double my_ciyun_curani;     // nobe& me-haciyyunim hacuraniyim $ell hamillim $emarkivot ^et harkib.
	friend bool identical (ConstituentCR  a, ConstituentCR b);
};
inline void free (Constituent& m) {::free(m.my_option_indices); };
inline void write (ostream& out, ConstituentCR thecons, Format format="") {
	out << thecons.word << ":" << thecons.from << "=>" << thecons.to << "["<<thecons.ciyun_cimcumi()<<" "<<thecons.ciyun_curani()<<" "<<thecons.ciyun_taxbiri()<<"]";
}
inline void read (istream& in, Constituent& thecons, Format format="") {
	// not implemented
}
inline bool identical (ConstituentCR  a, ConstituentCR b) {
	return (a.from==b.from && a.to==b.to && identical(a.word,b.word)); } // && a.my_ciyun_curani==b.my_ciyun_curani); }
inline void duplicate (Constituent& to, ConstituentCR from) { 
	to=from; 
	::duplicate(to.my_option_indices, from.my_option_indices);
}

DEFINE_INLINE_IO_OPERATORS(Constituent)

#endif
