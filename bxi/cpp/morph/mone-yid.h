/*
 * File morph/mone-yid.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/mone-yid.h
 */

/* mone-yid.h -- yidua counter */
/*
	The structure MoneYidua holds, for each ECEM in the corpus, two
		numbers: the total sikui that it is meyuda, and the total sikui
		that it is not meyuda.
*/

#ifndef __MONEYID_H
#define __MONEYID_H

#include <cpplib/pair.hxx>
#include <cpplib/countc.hxx>
#include <cpplib/trie0.hxx>
#include <cpplib/sikui.h>


/*        YiduaCount - 
	zug sikuyim: 
		ha-ri^$on ("x") mba@@e^ ^et ha-sikuy $e-&ecm msuyyam myudda, 
		w-ha-$eni ("y") mba@@e^ ^et ha-sikuy $e-hu^ lo-myudda, 
*/

struct YiduaCount: public Pair<Sikui> {
	Sikui& count (bool meyuda) {
		return meyuda? x(): y(); }
	Sikui count (bool meyuda) const {
		return meyuda? x(): y(); }
	Sikui shifted_count (bool meyuda) const {
		Sikui avg = (x()+y())/2;
		return meyuda? x()-avg: y()-avg; }
	void zero() { x()=y()=1; }
	YiduaCount (Sikui m=0, Sikui l=0): Pair<Sikui>(m,l) {}
};
/*
	Sikui meyuda;
	Sikui lomeyuda;
	YiduaCount(Sikui m=0, Sikui l=0) { meyuda=m; lomeyuda=l; }
	void operator= (uint ZERO) { meyuda=lomeyuda=0; }
	bool operator== (uint ZERO) { return meyuda==0 && lomeyuda==0; }
};*/
typedef const YiduaCount& YiduaCountCR;



class MoneYidua: public Trie0<YiduaCount> {
public:

	void zero_all() {
		LOOPTHISTRIE(cursor)
			cursor.item().zero();
	}

	status add (MorphInfoCR theinfo, Sikui thesikui)   { 
		if (!theinfo.hu(ECEM)) return OK;
		YiduaCount* curcountp = itemp (theinfo.baseword());
		if (curcountp==NULL)
			DOr( insert (theinfo.baseword(), YiduaCount(1,1), curcountp) );
		curcountp->count (theinfo.meyuda()) += thesikui;
		return OK;
	}

	Sikui count (MorphInfo theinfo) const {
		const YiduaCount* curcountp = itemp (theinfo.baseword());
		if (curcountp==NULL) return 0;
		else return curcountp->count(theinfo.meyuda());
	}
	Sikui shifted_count (MorphInfo theinfo) const {
		const YiduaCount* curcountp = itemp (theinfo.baseword());
		if (curcountp==NULL) return 0;
		else return curcountp->shifted_count(theinfo.meyuda());
	}

};
typedef const MoneYidua& MoneYiduaCR;

void write (ostream& out, MoneYiduaCR theinfo) { 
	write(out, theinfo, Format("T"));				}
status read (istream& in, MoneYidua& theinfo) { 
	return read(in, theinfo, Format("T")); }

#endif
