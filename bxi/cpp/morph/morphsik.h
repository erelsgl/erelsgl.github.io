/*
 * File morph/morphsik.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphsik.h
 */

/* morphsik.h -- morphological options structures, with sikui attached to each option */

#ifndef __MORPHSIK_H
#define __MORPHSIK_H

#include <math.h>
#include <cpplib/fuzzyv1.hxx>
#include <cpplib/array2.hxx>
#include <cpplib/mathtemp.h>
#include <morph/morphopt.h>

#include <morph/mip.h>


class MorphOptionsWithSikui;
typedef const MorphOptionsWithSikui& MorphOptionsWithSikuiCR;

class MorphOptionsWithSikui: public FuzzySetAsVector1<MorphInfo>   {
	public:

		/************************* editing the options ************************/

		void set_single_option (MorphInfoCR theoption) {       // copy the option
			realloc (1);
			info(0) = theoption;
			sikui(0) = SIKUI1;
		}

		void set_options (MorphOptionsCR theoptions) {       // copy the options from the given array (assign uniform sikui to each option)
			realloc (theoptions.count());
			if (theoptions.count()==0) return;
			LOOPVECTOR(;,theoptions,i)
				info(i) = theoptions[i];
			set_sikui_axid();
		}

		status append_option (MorphInfoCR theinfo, double thesikui=1.) { // append a new option, with the given sikui 
			DOr(append_empty());
			last().data() = theinfo;
			last().key() = ::sikui(thesikui);
			normalize();
			return OK;
		}


		/******** constructors *************/
		MorphOptionsWithSikui (Size thesize=0): 
			FuzzySetAsVector1<MorphInfo> (thesize) {}

		Sikui sikui_kolel (HeleqDiber hd) const { XAJEV_SIKUI_KOLEL (item.heleqdiber()==hd);  }
		Sikui sikui_kolel (MorphInfoPatternCR the_mip) const {XAJEV_SIKUI_KOLEL ( the_mip.match(item) ); }
		Sikui sikui_kolel_meen (Meen meen) const { XAJEV_SIKUI_KOLEL (item.meen()==meen);  }
		bool is_punctuation (char c) const { if (count()==0) return false; else return info(0).baseword()[0]==c; } 
};

void natax_im_sikui_axid (CStr theword, MorphOptionsWithSikui& theoptions_s); 
MorphOptionsWithSikui natax_im_sikui_axid (CStr theword); 
MorphOptionsWithSikui natax_kjem_prati_llo_txiliot (CStr theword); 


/* The meaning of format:
	"", "*": normal:							option1 sikui1 option2 sikui2 ...
	"I":     only info (uniform sikui):			option1 option2 ...
	"A":	 analyze (only a single fullword).
*/
void write (ostream& out, MorphOptionsWithSikuiCR theinfo, Format format="");
status read  (istream& in, MorphOptionsWithSikui& theinfo,Format format="");
inline void duplicate (MorphOptionsWithSikui& to, const MorphOptionsWithSikui& from) { 
	//duplicate ( (FuzzySetAsVector1<MorphInfo>&)to, (const FuzzySetAsVector1<MorphInfo>&)from );
 to.duplicate(from);
}
inline void free (MorphOptionsWithSikui& m) { 
	free ((FuzzySetAsVector1<MorphInfo>&)m);
}

DEFINE_INLINE_IO_OPERATORS(MorphOptionsWithSikui);



#endif
