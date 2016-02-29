/*
 * File cpplib/stattemp.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/stattemp.h
 */

/* stattemp.h -- statistical template functions */

/* 
	This file defines the following template functions:
		sumproducts (x,y, s):        s = sigma[i=1..n]{xi * yi}
		averageproducts (x,y, s):    s = sigma[i=1..n]{xi * yi} / n
		getcovariance (x,y, s):      s = cov(x,y)
		getvariance (v, s):          s = var(v)
		getstd (v, s):               s = std(v)
		regress_linear (x,y, slope,intercept):   y ~ slope*x+intercept
		regress_power (x,y, slope,intercept):    y ~ intercept*x^slope
*/

#ifndef __STATTEMP_H 
#define __STATTEMP_H

#include <cpplib/mathtemp.h>

#define TEMPLATEV template <class Vector>
#define TEMPLATEVS template <class Vector, class Scalar>
#define VectorCR const Vector&

#pragma warning ( disable: 4244 )

// s := sigma(i=1..n){x[i]*y[i]}
TEMPLATEVS void sumproducts (VectorCR x, VectorCR y, Scalar& s) { 
	assert(x.count()==y.count());
	LOOPVECTOR(s=0, x, i) s+=(x[i]*y[i]);
}

TEMPLATEVS void averageproducts (VectorCR x, VectorCR y, Scalar& s) { 
	sumproducts(x,y,s); s/=x.count(); }

TEMPLATEVS void getcovariance (VectorCR x, VectorCR y, Scalar& s) { 
	Scalar Exy, Ex, Ey;
	averageproducts(x,y,Exy);
	average(x,Ex);
	average(y,Ey);
	s = Exy - Ex*Ey;
}

TEMPLATEVS void getvariance (VectorCR v, Scalar& s) { 
	getcovariance(v,v,s); }

TEMPLATEVS void getstd (VectorCR v, Scalar& s) { 
	getvariance(v,s);  s=sqrt(s);  }


//calculate a linear regression line, given by: 
//	y = slope*x + intercept
//      { y-Ey = slope*(x-Ex) }
TEMPLATEVS void regress_linear(VectorCR x, VectorCR y, Scalar& slope, Scalar& intercept) { 
	Scalar Ex, Ey, COVxy, COVxx;
	average(x,Ex);
	average(y,Ey);
	getcovariance(x,y,COVxy);
	getcovariance(x,x,COVxx);
	assert (COVxx>0);
	slope = COVxy / COVxx;
	intercept = Ey - slope*Ex;
}

//calculate a power regression line, given by: 
//	y = intercept*(x^slope)
//      { log(y) = log(intercept) + slope*log(x) }
TEMPLATEVS void regress_power(VectorCR x, VectorCR y, Scalar& slope, Scalar& intercept) { 
	Vector logx(x.count()), logy(y.count());
	set(logx,x,1);  log(logx, 1e-8);
	set(logy,y,1);  log(logy, 1e-8);

	regress_linear(logx,logy, slope,intercept);
	intercept = exp(intercept);

	logx.free(); logy.free();
}



#endif
