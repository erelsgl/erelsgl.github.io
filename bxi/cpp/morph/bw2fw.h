/*
 * File morph/bw2fw.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/bw2fw.h
 */

/* bw2fw.h -- baseword-to-fullword map */

#ifndef __BW2FW_H
#define __BW2FW_H

#include <cpplib/vector1.hxx>
#include <cpplib/trie0.hxx>
#include <cpplib/mapc.hxx>

/*        ***** USAGE EXAMPLE *****
*/



typedef Vector1Sorted<CStr> FullwordArray;
typedef const FullwordArray& FullwordArrayCR;

extern FullwordArray the_empty_fullword_array;

class BasewordToFullwordMap: public Trie0OfContainerType(Vector1Sorted,CStr) {
	public:
		// insert thefullword to thebaseword's list of fullwords, but only if it does not exist!
		status insert (CStr thebaseword, CStr thefullword) {   
			FullwordArray* thearray = infop(thebaseword);
			if (thearray==NULL || !thearray->contains(thefullword))
				return Trie0OfContainerType(Vector1Sorted,CStr)::insert (thebaseword,thefullword);
			else
				return OK;
		}
		FullwordArray& fullword_array (CStr thebaseword) { 
			FullwordArray* thearray = infop(thebaseword);
			if (thearray!=NULL) return *thearray;
			else return the_empty_fullword_array;
		}
};

typedef const BasewordToFullwordMap& BasewordToFullwordMapCR;

#endif
