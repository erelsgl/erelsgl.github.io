/*
 * File cpplib/sikui.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/sikui.h
 */

/* sikui.h -- probability functions */
/*
	This unit defines the type Sikui.
		Sikui approximates a probability (between 0 and 1) by a number (between 0 and 1000),
			which is equal to 1000 times the real probability.
		To convert Sikui to a real-sikui, use realsikui(the_sikui).
		To convert a real-sikui to a Sikui, use sikui(the_realsikui).
*/

#ifndef __SIKUI_H
#define __SIKUI_H

typedef long Sikui;    // probability is represented as an integer from 0 to 1000.
#define SIKUI1 1000
#define SIKUI1R 1000.
#define SIKUI_MAX 0x7fffffff

#pragma warning( disable : 4244 )
inline Sikui sikui (double real_sikui) { return (Sikui)(real_sikui*SIKUI1); }
#pragma warning( default : 4244 )

inline double realsikui (Sikui s) { return s/double(SIKUI1); }

inline Sikui product (Sikui s1, Sikui s2) { return sikui(realsikui(s1)*realsikui(s2)); }
inline Sikui product (Sikui s1, Sikui s2, double weight) { return sikui(realsikui(s1)*realsikui(s2)*weight); }

#endif

