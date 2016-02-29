/*
 * File morph/tavniot.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/tavniot.h
 */

/* tavniot.h-- tavniot morfologiot */


#ifndef __TAVNIOT_H
#define __TAVNIOT_H

#include <morph/tavnit.h>
#include <cpplib/array2.hxx>


typedef Array2<Tavnit> Tavniot;
//class Tavniot;
typedef Tavniot& TavniotR;
typedef const Tavniot& TavniotCR;
//class Tavniot: public Array2<Tavnit> {
//	void find_next_match (CStr theword, StringTemp& thebase, Index& i) {
//		for (; i<len(); ++i) {
//			if (at(i).match(theword,thebase)==OK) break;
//		}
//	}
//};			

/* 
Here is a  usage example for this macro:
	Index i; 
	LOOP_MATCHING_TAVNIOT (i,thetavniot,theword,thebase)     // note: NO BRACES!
		cout << thebase << " " << curtavnit.sug();           // you can use 'curtavnit' as a reference to thetavniot[i]
	}														 // note: BRACES REQUIRED!
		
*/
#define LOOP_MATCHING_TAVNIOT(i,thetavniot,theword,thebase)  \
	for(i=0; i<thetavniot.len(); ++i) {\
		TavnitR curtavnit = thetavniot[i];	\
		if (curtavnit.match(theword,thebase)!=OK) continue; 


#define LOOP_BASE_MATCHING_TAVNIOT(i,thetavniot,theword,thebase,ref_morphinfo)  \
	for(i=0; i<thetavniot.len(); ++i) {\
		TavnitR curtavnit = thetavniot[i];	\
		if (curtavnit.match(theword,thebase)!=OK) continue;\
		if (strcmp(thebase.str,ref_morphinfo.baseword() )!=OK) continue;



#endif
