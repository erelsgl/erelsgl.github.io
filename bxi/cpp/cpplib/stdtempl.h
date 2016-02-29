/*
 * File cpplib/stdtempl.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/stdtempl.h
 */

/* stdtempl.h -- defines standard template functions */
/* 
	This file defines the following functions:
		T min( T, T ) returns the lesser of its arguments                 
		T min( T, T, T ) returns the least of its arguments               
		T max( T, T ) returns the greater of its arguments                
		T max( T, T, T ) returns the greatest of its arguments            
		T average( T, T ) returns the average of its arguments                
		T average( T, T, T ) returns the average of its arguments            
		T range( T minVal, T maxVal, T val ) returns val if val is        
			between minVal and maxVal.  If val is greater than maxVal,    
			returns maxVal.  If val is less than minVal, returns minVal.  
		T swap (T& a, T& b)  swaps a and b.
		T sort2 (T& a, T& b)  swaps a and b if a>b.

	And also the macro:
				LOOPVECTOR(the_initialization,the_vector,the_index)
		which is used to loop over the_vector, using the variable named the_index.
*/

#ifndef __STDTEMPL_H
#define __STDTEMPL_H

template <class T> inline T min( T t1, T t2 )
{
		return t1>t2 ? t2 : t1;
}

template <class T> inline T min( T t1, T t2, T t3 )
{
    return t1>t2 ? (t2>t3 ? t3 : t2) : (t1>t3 ? t3 : t1 );
}

template <class T> inline T max( T t1, T t2 )
{
		return t1>t2 ? t1 : t2;
}

template <class T> inline T max( T t1, T t2, T t3 )
{
		return t1>t2 ? (t1>t3 ? t1 : t3) : (t2>t3 ? t2 : t3);
}

template <class T> inline T average( T t1, T t2 )
{
		return (t1+t2)/2;
}

template <class T> inline T average( T t1, T t2, T t3 )
{
		return (t1+t2+t3)/3;
}

template <class T> inline T range( T minVal, T maxVal, T val )
{
		return min( maxVal, max( minVal, val ) );
}


template <class T> inline void swap (T& a, T& b)
{
	T temp=a;
	a=b;
	b=temp;
}

template <class T> inline void sort2 (T& a, T& b)
{
	if (a>b)  swap (a,b);
}

/*************************** iteration functions ******************************/


// This macro advances the cursor 'c' until the-cond is true.
// For example:
// thecursor.reset(thecontainer);	FINDwithCURSOR(thecursor,c.item()!=0) looks for the first item which is not zero, and puts
//	the cursor there.
#define FINDwithCURSOR(c,thecond)   for(;c.isvalid();c.next())  if (thecond) break

#define FINDinVECTOR(i,thevector,thecond)   for(;i<thevector.count();++i)  if (thecond) break


// This macro loops over a vector. example:
//	LOOPVECTOR(int sum=0,v,i)  sum+=v[i];
// calculates the sum of a vector.
#define LOOPVECTOR(init,thevector,theindex)  init;  for(Index theindex=0; theindex<thevector.count(); ++theindex) 

#define LOOPTHISVECTOR(init,theindex)  init;  for(Index theindex=0; theindex<count(); ++theindex) 

// This macro loops over a container. example:
//	LOOPCONTAINER(int sum=0, thelist, sum+=c.item() ) 
// calculates the sum of a vector.
// Inside 'theaction', use c.item() to denote the current container element.
//#define LOOPCONTAINER (init,thecursor,theaction)  init; { for(thecont::Cursor c(; i<thevector.count(); ++i) theaction; }



#endif  // __STDTEMPL_H


