/*
 * File morph/hajlamot.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/hajlamot.h
 */

/* hajlamot.h -- ha$lamot l-p&alim */

#include <cpplib/countc.hxx>
#include <cpplib/map.hxx>
#include <cpplib/trie0.hxx>
#include <cpplib/sikui.h>
#include <morph/mip.h>


struct MoneHajlamot1: public CounterVector1<MorphInfoPattern> {
// mone, &abur po&l msuyyam, kamma p&amim mopi&a ^axaraw koll ha$lama ^ep$arit.
	Sikui sikui_kolel;
	MoneHajlamot1(Size size=0) { sikui_kolel=0; }
	bool operator==(int ZERO) const { return sikui_kolel==0; }
	void operator= (int ZERO) { free(); sikui_kolel=0; }
	void cor_tavnit_hajlama (MorphInfoCR y, MorphInfoPattern& ha_hajlama);
	status add (MorphInfoCR y, Sikui ha_sikui);
	Sikui sikui (MorphInfoCR y);
	double sikui_yaxasi (MorphInfoCR y);
	double sikui_yaxasi_mtuqan (MorphInfoCR y);
};
typedef const MoneHajlamot1& MoneHajlamot1CR;


class MoneHajlamot: public Trie0<MoneHajlamot1> {
public:
	MoneHajlamot1* mone_hajlamot_p (MorphInfoCR ha_poal);
	status add (MorphInfoCR x, MorphInfoCR y, Sikui hasikui);
	Sikui sikui (MorphInfoCR x, MorphInfoCR y);
	double sikui_yaxasi (MorphInfoCR x, MorphInfoCR y);
	double sikui_yaxasi_mtuqan (MorphInfoCR x, MorphInfoCR y);
};


inline void write (ostream& out, MoneHajlamot1CR theinfo, Format format="") {
	out << theinfo.sikui_kolel;
	write (out, (const CounterVector1<MorphInfoPattern>&)theinfo, Format("P\n") );
}
inline status read  (istream& in, MoneHajlamot1& theinfo,Format format="") {
	DOr(read(in,theinfo.sikui_kolel));
	return read(in, (CounterVector1<MorphInfoPattern>&)theinfo, Format("P\n") );
}



extern MoneHajlamot mone_hajlamot_pealim;



void sfor_hajlama(MorphInfoCR x, MorphInfoCR y, Sikui hasikui);
void ktov_hajlamot(MoneHajlamot& hahajlamot, Sikui hasaf);
void ktov_hajlamot(CStr path);
status qra_hajlamot(CStr path);
