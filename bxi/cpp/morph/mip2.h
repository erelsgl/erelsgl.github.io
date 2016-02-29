/*
 * File morph/mip2.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/mip2.h
 */

/* mip2.h --   2 morphological info patterns */

#ifndef __MIP2_H
#define __MIP2_H

#include <morph/mip.h>

inline bool yej_hatama (MorphInfoPatternCR x, MorphInfoPatternCR y, MorphInfoCR the_x, MorphInfoCR the_y) {
			//if (x.yidua_carik_lhatim && !identical_yidua(the_x.yidua(),the_y.yidua())) return false;
			if (x.yidua_carik_lhatim && (the_x.meyuda()!=the_y.meyuda())) return false;
			if (x.meen_carik_lhatim && !identical_meen(the_x.meen(),the_y.meen())) return false;
			if (x.mispar_carik_lhatim && !identical_mispar(the_x.mispar(),the_y.mispar())) return false;
			if (x.guf_carik_lhatim && !identical_guf(the_x.guf(),the_y.guf())) return false;
			return true;
}


inline bool match (MorphInfoPatternCR x, MorphInfoPatternCR y, MorphInfoCR the_x, MorphInfoCR the_y) {
			if (!x.match(the_x)) return false;
			if (!y.match(the_y)) return false;
			return yej_hatama (x,y,the_x,the_y);
}


inline void ktov_barur (MorphInfoPatternCR x, MorphInfoPatternCR y, ostream& out) {
	x.ktov_barur (out, false);
	out << "   ";
	y.ktov_barur (out, true);
}

#endif

