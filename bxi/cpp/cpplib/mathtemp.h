/*
 * File cpplib/mathtemp.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/mathtemp.h
 */

/* mathtemp.h -- mathematical template functions */

/* 
	This file defines the following template functions:
		void sum (thevector, theresult) -- calculates the sum of thevector and puts it in theresult.
		void average (thevector, theresult) -- calculates the average of thevector and puts it in theresult.
		void multiply (thevector, thescalar) -- multiplies the vector by the scalar.
		void add (vector1, vector2, scalar=1) -- performs the operation: vector1 = vector1 + scalar*vector2
		void zero (thevector) -- sets all thevector's elements to 0.
		void init (thevector, thesize) -- reallocates thevector to thesize and sets all thevector's elements to 0.
		Index maxindex (thevector) -- returns the index of the FIRST maximal element.
		Index minindex (thevector) -- returns the index of the FIRST minimal element.
*/

#ifndef __MATHTEMP_H 
#define __MATHTEMP_H

#include <cpplib/stdtempl.h>
#include <cpplib/typedefs.h>

#define TEMPLATEV template <class Vector>
#define TEMPLATEVS template <class Vector, class Scalar>
#define VectorCR const Vector&

#pragma warning ( disable: 4244 )



TEMPLATEVS void sum (VectorCR v, Scalar& s) { 
	LOOPVECTOR(s=0, v, i) s+=v[i];  }

TEMPLATEVS void average (VectorCR v, Scalar& s) { 
	sum(v,s);  s/=v.count();  }

TEMPLATEVS void  multiply (Vector& v, Scalar s) { 
	LOOPVECTOR(;,v,i) v[i] *= s; }

TEMPLATEVS void add (Vector& a, VectorCR b, Scalar bweight=1) { 
	LOOPVECTOR(;,a,i)  a[i] = (typename Vector::Data)(a[i]+bweight*b[i]); }

TEMPLATEV void  multiply_ew (Vector& a, VectorCR b) {   // elementwise multiplication
	LOOPVECTOR(;,a,i) a[i] *= b[i]; }

TEMPLATEVS void log (Vector& a, Scalar ZERO=0) { // elementwise log. ZERO is used instead of a[i] when a[i]==0, to prevent domain-error
	LOOPVECTOR(;,a,i) a[i] = log(a[i]+ZERO); }

TEMPLATEV void zero (Vector& a) { 
	LOOPVECTOR(;,a,i) a[i]=0; }

TEMPLATEVS void set (Vector& a, VectorCR b, Scalar bweight=1) { 
	LOOPVECTOR(;,a,i)  a[i] = bweight*b[i];  }

TEMPLATEVS void normalize (Vector& a, Scalar target_sum) { 
	Scalar current_sum; sum(a,current_sum);
	if (current_sum!=0)  multiply (a,target_sum/current_sum);
}

TEMPLATEVS void normalize (Vector& a, Scalar target_sum, Scalar ZERO) { 
	Scalar current_sum; sum(a,current_sum);
	if (current_sum!=0)  
		LOOPVECTOR(;,a,i) {
			a[i] = (typename Vector::Data)(a[i]*target_sum/current_sum);
			if (a[i]==0) a[i]=(typename Vector::Data)ZERO;
		}
}

TEMPLATEV void init (Vector& a, Size thesize) { a.realloc(thesize); zero(a); }

TEMPLATEV Index minindex (Vector& a) { LOOPVECTOR(Index result=0,a,i)  if (compare(a[i],a[result])<0)  result=i;   return result; }
TEMPLATEV Index maxindex (Vector& a) { LOOPVECTOR(Index result=0,a,i)  if (compare(a[i],a[result])>0)  result=i;   return result; }



#endif  

