/*
 * File morph/mone.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/mone.h
 */

/* mone.h -- An interface of a general morphological counter */

#include <morph/morphinf.h>


/*        ***** USAGE EXAMPLE *****
	[not written yet]
*/



class Mone {
public:
	void zero_all();
	status setcount (MorphInfoCR theinfo, Count thecount=0);
	status add (MorphInfoCR theinfo, Sikui thesikui=SIKUI1);
	Count count (MorphInfoCR theinfo) const;
	friend void write (ostream& out, MoneCR theinfo);
	friend status read (istream& in, Mone& theinfo);
};

void write (ostream& out, MoneCR theinfo);
status read (istream& in, Mone& theinfo);
