/*
 * File morph/sntncinc.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/sntncinc.cpp
 */

/* sntncinc.cpp -- SentenceInfoWithCiyun */


#include <morph/sntncinc.h>
#include <morph/cimcumim.h>
#include <math.h>


double ciyun_taxbiri0 (MorphInfoCR theword) {
	if (theword.hu(MILAT_XIBUR)) return 0;
	return 
		(theword.otiotyaxas()!=NO_OY? log10(0.5): 0) + 
		log10(0.5);
}

double ciyun_taxbiri (MorphInfoCR theword) {
	if (theword.hu(MILAT_XIBUR)) return 0;

	if (theword.hu(POAL)) {  // ha&ne$ koll po&l $e-^ein lo nose^
		if (theword.guf()==GUF3 && !lapoal_yej_nose(theword))
			return log10(0.9);  
		else
			return 0;
	}

	// ha&ne$ koll millit ^o milla $e-^einah po&l ^o millat xibbur:
	return 
		(theword.otiotyaxas()!=NO_OY? log10(0.5): 0) + 
		log10(0.5);
}


double SentenceInfoWithCiyun::ciyun_taxbiri() const {
	if (count()==0) return 0;
	LOOPVECTOR (double acc=MAX_CIYUN,(*this),w) {
		MorphInfoCR curword = at(w);
		acc += ::ciyun_taxbiri(curword);

		if (w+1<count()) {
			MorphInfoCR nextword = at(w+1);
			// ha&ne$ $tei millim $e-^einan mxubbarot &"i millat xibbur.
			if (!curword.hu(MILAT_XIBUR) && !nextword.hu(MILAT_XIBUR) &&
				(!nextword.vav() && (nextword.jiabud()==NO_JIABUD||nextword.jiabud()==JIABUD_HA)))
					acc += log10(0.3);
			// ha&ne$ nismak bli somek.
			if (curword.nismak() && !curword.poal() && !nextword.can_be_somek())  
					acc += log10(0.1);
		}
		else {
			// ha&ne$ nismak bli somek.
			if (curword.nismak() && !curword.poal()) acc += log10(0.1);
		}
	}

	return acc;
}


double SentenceInfoWithCiyun::ciyun_taxbiri0 () const {
	LOOPVECTOR (double acc=MAX_CIYUN,(*this),w)
		acc += ::ciyun_taxbiri0(at(w));
	return acc;
}



