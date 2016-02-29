/*
 * File morph/bituy.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/bituy.h
 */

/* bituy.h -- bituyim */

#ifndef __BITUY_H
#define __BITUY_H

#include <morph/mip.h>
#include <cpplib/pair.hxx>

/*
struct Bituy { 
	MorphInfoPattern x, y; 
	Bituy (MorphInfoPatternCR thex,MorphInfoPatternCR they): x(thex), y(they) {}
	Bituy (uint ZERO=0) {x=0; y=0;}
	void operator=(uint ZERO) {x=0; y=0;}
	bool operator==(uint ZERO) const {return (x==0&&y==0);}
	StringTemp bitui_str() const { return concat_bw(x,y); }
};
*/

#undef PAIR
#define PAIR Pair<MorphInfoPattern>

struct Bituy: PAIR {
	StringTemp bitui_str() const { return concat_bw(x(),y()); }
	void set (MorphInfo the_x, MorphInfo the_y) {
		x().set(the_x.baseword(),the_x.heleqdiber());
		y().set(the_y.baseword(),the_y.heleqdiber());
	}
	Bituy (MorphInfo thex, MorphInfo they) { set(thex,they); }
	Bituy (MorphInfoPatternCR thex,MorphInfoPatternCR they): PAIR(thex,they) {}
	Bituy (uint ZERO): PAIR(ZERO) {}
	Bituy (): PAIR() {}
	void operator=(uint ZERO) {PAIR::operator=(ZERO);}
	bool operator==(uint ZERO) const {return PAIR::operator==(ZERO);}
};
typedef const Bituy& BituyCR;

inline long hashvalue (BituyCR thebituy) {
	return hashvalue(thebituy.x().baseword()); }

DEFINE_INLINE_IO_OPERATORS(Bituy)

#undef PAIR
#endif
