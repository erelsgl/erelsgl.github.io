/*
 * File cpplib/argsfile.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/argsfile.h
 */

/* argsfile.h -- a unit to read a program's arguments file */

/*
	an example of an argument file:

		% orek meravi $ell maxrozet = 30 tawim.
		% ha-maxrozot "case-sensitive".

		%%%%%%% maxrozot %%%%%%%%
		{
		JmQovcQorpus:              harc20         % kolel gamm ^et ha"Mamr", wgamm ^et ha"QorpusMnutx".
		JmQovcMamr:                harc10a        % hama^mar $erocim lnattex ^o livdoq.
		JmQovcQorpusMnutx:         testa5         % haqorpus ham$amme$ l^imun.

		MslulLogim:                ./tqstim
		MslulMilon:                .
		}

		%%%%%%% misparim %%%%%%%
		{
		% bdyuq 1 mehami$tannim haba^im carik lihyot 1, ha$^ar 0:
		NituxAlYdiMjtmj:                0   % hami$tamme$ mnattex ^et ha"Mamr", milla ^axar milla, whatokna kotevet ltok qovec pele@ (lma$al "harc10.out").
		NituxAlYdiTokna:                0   % hatokna mnattaxat ^et ha"Mamr", wkotevet ltok qovec pele@ (lma$al "harc10.out").
		Bdiqa:                          1   % hatokna mnattaxat ^et ha"Mamr", wma$wa lqovc nittux nakon (lma$al "harc10.to").
		}

*/



#ifndef __ARGSFILE_H
#define __ARGSFILE_H

#include <cpplib/typedefs.h>

void read_arguments_file (CStr thePath);  


// The following 2 functions return the argument with the given name.
// If the argument does not exist, it is an error.
CStr string_argument(CStr theName);
int int_argument(CStr theName);

// The following 2 functions return the argument with the given name.
// If the argument does not exist, "theDefault" is returned.
CStr string_argument(CStr theName, CStr theDefault);
int int_argument(CStr theName, int theDefault);

#endif
