/*
 * File morph/morphsik.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphsik.cpp
 */

/* morphsik.cpp -- morphological options structures with sikuiim */
#include <morph/morphinf.h>
#include <morph/morphsik.h>
#include <morph/morphanl.h>
#include <cpplib/stringt.h>



void natax_im_sikui_axid (CStr theword, MorphOptionsWithSikui& theoptions_s) {
	MorphOptions theoptions = natax(theword);
	theoptions_s.set_options(theoptions);
	free(theoptions);
}

MorphOptionsWithSikui natax_im_sikui_axid (CStr theword) {
	MorphOptionsWithSikui theoptions_s;
	natax_im_sikui_axid (theword,theoptions_s);
	return theoptions_s;
} 

MorphOptionsWithSikui natax_kjem_prati_llo_txiliot(CStr theword) {
	MorphInfo theoption(JEM_PRATI);
	theoption.setbaseword(theword);
	theoption.setvav(false);
	theoption.setjiabud(NO_JIABUD);
	theoption.setotiotyaxas(NO_OY);
	theoption.setmeen(BOTH);
	MorphOptionsWithSikui theoptions_s;
	theoptions_s.set_single_option(theoption);
	return theoptions_s;
}



void write (ostream& out, MorphOptionsWithSikuiCR theoptions, Format format) { 
	write_vector (out,theoptions,str(" "),Format(" ")); 
}


status read  (istream& in, MorphOptionsWithSikui& theoptions_s, Format format) { 
	if (format[0]=='I') {          // info only
		MorphOptions theoptions;
		DOr(read (in,theoptions,Format("L")));
		theoptions_s.set_options(theoptions);
		theoptions.free();
		return OK;
	}
	else if (format[0]=='A') {          // natax
		skip_comments (in,'%');
		StringTemp theword = next_line(in,30," ");
		natax_im_sikui_axid (theword.str,theoptions_s);
		return OK;
	}
	else {                              // normal, line
		return read_container(in,theoptions_s,' ','\n',Format(" ")); 
	}
}

