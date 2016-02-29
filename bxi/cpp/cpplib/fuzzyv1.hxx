/*
 * File cpplib/fuzzyv1.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/fuzzyv1.hxx
 */

/* fuzzyv1.hxx -- fuzzy set as vector-1 */

#ifndef __FUZZYV1_HXX
#define __FUZZYV1_HXX

#include <cpplib/pair.hxx>
#include <cpplib/vector1.hxx>
#include <cpplib/array2.hxx>
#include <cpplib/mathtemp.h>
#include <cpplib/sikui.h>


#define TEMPLATE template<class Item>
#define SET FuzzySetAsVector1<Item>
#define SETCR const SET&



typedef Array2<Sikui> ArrayOfSikuiim;
typedef const ArrayOfSikuiim& ArrayOfSikuiimCR;
inline double realsum (ArrayOfSikuiimCR thesikuiim) { Sikui s; sum(thesikuiim,s); return realsikui(s); }
DEFINE_INLINE_IO_OPERATORS(ArrayOfSikuiim)


inline void atxel (ArrayOfSikuiim& thesikuiim, uint thecount) {
	thesikuiim.truncate(thecount);
	zero(thesikuiim);
}




TEMPLATE class FuzzySetAsVector1: public Vector1 < KeyData<Sikui,Item> > {
	public:
		typedef const Item& ItemCR;
		/*********************** queries ******************************/

		Sikui sikui (Index i) const { return i<count()? at(i).key(): 0; }
		double realsikui (Index i) const { return ::realsikui(sikui(i)); }
		Sikui& sikui (Index i)  { return at(i).key(); }
		ItemCR info (Index i) const { return at(i).data(); }
		Item& info (Index i) { return at(i).data(); }


		/************ finding options with properties *****************/

		// This macro is used to find an analysis with certain properties. For example:
		//	       Index i=0;  FINDITEMWITHSIKUI(theset,i, isdigit(item) )
		//    will set i to the index of the first item that is a digit (in a fuzzy-set of chars).
		//		(if there is none, i will be equal to theset.count() ).
		#define FINDITEMWITHSIKUI(theset,theindex,thecond)\
			for(;theindex<theset.count();++theindex) {\
				ItemCR item=theset.info(theindex);  if(thecond) break; }

		#define CONTAINSITEMWITHSIKUI(thecond)\
   Index i;\
			for(i=0;i<count();++i) {\
				ItemCR item=info(i);  if(thecond) break; }\
			return i<count();

		#define XAJEV_SIKUI_KOLEL(theproperty) { \
			Sikui sum=0;  \
			for (Index i=0; i<count(); ++i) {\
				ItemCR item=info(i);\
				if(theproperty)  sum += sikui(i); \
			}\
			return sum;\
		}

		
		//bool contains (ItemCR theitem) const {
		//	return sikui(theitem)>0; }
		bool contains (ItemCR theinfo) const { CONTAINSITEMWITHSIKUI(identical(item,theinfo)); }

		Sikui sikui (ItemCR theinfo) const { // This is actually the 'member-function' of the fuzzy-set
			XAJEV_SIKUI_KOLEL ( identical(item,theinfo) );  }

		Index indexof (ItemCR theinfo) const { Index i=0; FINDITEMWITHSIKUI((*this),i, identical(item,theinfo) ); return i; }

		Index index_with_greatest_sikui () const { 
   Index result=0;
			for (Index i=1; i<count(); ++i)
				if (sikui(i)>sikui(result))  result=i;
			return result;
		}

		ItemCR info_with_greatest_sikui() const {
			assert (!isempty());
			Index m = index_with_greatest_sikui();
			return info(m);
		}

		Sikui greatest_sikui() const {
			assert (!isempty());
			Index m = index_with_greatest_sikui();
			return sikui(m);
		}

		status get_info_with_greatest_sikui (Item& theinfo, Sikui& thesikui) const {
			if (isempty()) return EEMPTY;
			Index m = index_with_greatest_sikui();
			Item inf = info(m);
			theinfo = inf;
			thesikui = sikui(m);
			return OK;
		}
		/********************* editing the sikuiim *******************/

		Sikui skum_sikuiim() const { 
			Sikui s=0;  for (Index i=0; i<count(); ++i)  s+=sikui(i);  
			return s; 
		}
		Sikui skum;                                  // the sum of all the sikuiim
		void xajev_skum() { skum=skum_sikuiim(); }

		void set_sikui_axid () {                     // gives an equal sikui to each option (1/count).
			Sikui the_sikui_axid = ::sikui(1./count());
			for (Index i=0; i<count(); ++i) 
				sikui(i) = the_sikui_axid;
		}

		void zero() {
			for (Index i=0; i<count(); ++i) 
				sikui(i) = 0;
		}

		void normalize() {                           // set the sikuiim so that the sum will be 1
			xajev_skum(); 		assert (0<skum&&skum<10000*SIKUI1);
			if ( uint(abs(SIKUI1-skum)) < count() ) return;  // the difference from '1' is to small to normalize (it is a round-off error)
				for (Index i=0; i<count(); ++i) 
					sikui(i) = (Sikui)(sikui(i)*(SIKUI1R/skum));
			//multiply ( SIKUI1R / skum );
		}


		status set (Index to_whom, Sikui how_much) {
			if (to_whom>=count()) return ERANGE;
			sikui(to_whom) = max(how_much,1);
			normalize();
			return OK;
		}

		status add_bli_nirmul (Index to_whom, Sikui how_much) {
			if (to_whom>=count()) return ERANGE;
			sikui(to_whom) = max ( sikui(to_whom)+how_much , 1l );
			return OK;
		}

		status add (Index to_whom, Sikui how_much) {
			DOr (add_bli_nirmul(to_whom,how_much));
			normalize();
			return OK;
		}


		status set (ArrayOfSikuiimCR thesikuiim) {   // copy the sikuiim from the given array.
			if ( thesikuiim.count() != count() )  return ERANGE;
			LOOPVECTOR (;,thesikuiim,i)  sikui(i) = max(thesikuiim[i],1l);
			normalize();
			return OK;
		}

		status add_bli_nirmul (ArrayOfSikuiimCR thesikuiim, double weight=1) { // add sikuiim from the given array.
			if ( thesikuiim.count() != count() )  return ERANGE;
			//#pragma warning ( disable: 4244 )
			LOOPVECTOR (;,thesikuiim,i)  {
				sikui(i) = max ( Sikui(sikui(i)+weight*thesikuiim[i]) , 1 );
			}
			return OK;
		}

		status add (ArrayOfSikuiimCR thesikuiim, double weight=1) { // add sikuiim from the given array.
			DOr (add_bli_nirmul(thesikuiim,weight));
			normalize();
			return OK;
		}


		status add_only_the_nonzero (ArrayOfSikuiimCR thesikuiim, double weight=1) {
			//This version changes only the options for which the sikui added is nonzero.
			//It will not touch the other options. It is used to change the relative sikuyim in a subset of options. 
			if ( thesikuiim.count() != count() || weight==0)  return ERANGE;
			Sikui sum_before_adding=0, sum_after_adding=0;
			// a. Calculate the sum before adding. This sum must remain the same after adding + normalization.
			// b. Add the sikuyim.
			// c. Calculate the sum after adding, and normalize so that it will be equal to the sum before adding.
			LOOPVECTOR (;,thesikuiim,i)  {
				if (thesikuiim[i]!=0)  {
					sum_before_adding += sikui(i);
					sikui(i) = max ( Sikui(sikui(i)+weight*thesikuiim[i]) , 1 );
					sum_after_adding += sikui(i);
				}
			}
			if (sum_after_adding==0) return OK;
			// d. normalize.
			{LOOPVECTOR (;,thesikuiim,i)  
			//#pragma warning ( disable: 4244 )
				if (thesikuiim[i]!=0)  sikui(i) = sikui(i)*sum_before_adding/sum_after_adding;		
			}
			return OK;
		}


		status add_bli_nirmul (SETCR theotherset, double weight=1) { // add sikuiim from the given array.
			if ( theotherset.count() != count() )  return ERANGE;
			//#pragma warning ( disable: 4244 )
			LOOPVECTOR (;,theotherset,i)  {
				sikui(i) = max ( Sikui(sikui(i)+weight*theotherset.sikui(i)) , 1l );
			}
			return OK;
		}


		/************************* sidur ************************/

		void sader_lfi_sikui_bseder_yored () {
		// implemented by maximum-sort.
			if (count()<=1) return;      
			for (Index i=0; i<count()-1; ++i)	{
				// mca^ ^et ha-^indeqs (ha- >=i) &im ha-sikui ha-gadol bioter.
    Index curmax=i;
				for (Index j=i+1; j<count(); ++j)
					if (sikui(j)>sikui(curmax)) curmax = j;
				// ha&ber ^et ha-maximum la-maqom $ello
				if (curmax!=i)  swap (myinfo.p[i],myinfo.p[curmax]);
			}
		}


		void sader_indexim_lfi_sikui_bseder_yored (Array2<Index>& the_indexes) const {
		// implemented by maximum-sort.
			the_indexes.truncate(count());
			// ^atxel ^et ha-^indeqsim
			LOOPVECTOR(;,the_indexes,k) the_indexes[k]=k;
			if (count()<=1) return;      
			for (Index i=0; i<count()-1; ++i)	{
				// mca^ ^et ha-^indeqs (ha- >=i) &im ha-sikui ha-gadol bioter.
    Index curmax=i;
				for (Index j=i+1; j<count(); ++j)
					if (sikui(the_indexes[j])>sikui(the_indexes[curmax])) curmax = j;
				// ha&ber ^et ha-maximum la-maqom $ello
				if (curmax!=i)  swap (the_indexes[i],the_indexes[curmax]);
			}
		}



		/************************* editing the options ************************/


		/******** constructors *************/
		FuzzySetAsVector1 (Size thesize=0): 
			Vector1<Data> (thesize) {}
};




TEMPLATE double merxaq_hamming (SETCR a, SETCR b) {
	if (a.count()!=b.count()) return double(SIKUI_MAX);
	Sikui sa=a.skum_sikuiim(), sb=b.skum_sikuiim();
	if (sa==0&&sb==0) return 0;                     //a==b: both of them are zeroed.
	else if (sa==0||sb==0) return double(SIKUI_MAX);        //a==0 and b!=0 (or vice versa).
	LOOPVECTOR(double sum=0,a,i) {
		double dif = double(a.sikui(i))/sa - double(b.sikui(i))/sb;
		if (dif>=0) sum+=dif; else sum-=dif;        // sum += |dif|
	}
	return sum;
}

TEMPLATE double merxaq_euclid (SETCR a, SETCR b) {
	if (a.count()!=b.count()) return double(SIKUI_MAX);
	Sikui sa=a.skum_sikuiim(), sb=b.skum_sikuiim();
	if (sa==0&&sb==0) return 0;                     //a==b: both of them are zeroed.
	else if (sa==0||sb==0) return double(SIKUI_MAX);        //a==0 and b!=0 (or vice versa).
	LOOPVECTOR(double sum=0,a,i) {
		double dif = double(a.sikui(i))/sa - double(b.sikui(i))/sb;
		sum += dif*dif;
	}
	return sqrt(sum);
}

TEMPLATE double merxaq_maximum (SETCR a, SETCR b) {
	if (a.count()!=b.count()) return double(SIKUI_MAX);
	Index ma = a.index_with_greatest_sikui();
	Index mb = b.index_with_greatest_sikui();
	if (ma!=mb) return 1;
	else return 0;
}

TEMPLATE double merxaq_maximum_mjuqlal (SETCR a, SETCR b) {
	if (a.count()!=b.count()) return double(SIKUI_MAX);
	double sa=realsikui(a.skum_sikuiim()), sb=realsikui(b.skum_sikuiim());
	if (sa==0&&sb==0) return 0;                     //a==b: both of them are zeroed.
	else if (sa==0||sb==0) return double(SIKUI_MAX);        //a==0 and b!=0 (or vice versa).
	Index ma = a.index_with_greatest_sikui();
	Index mb = b.index_with_greatest_sikui();
	return 
		max(realsikui(b.sikui(mb))/sb-realsikui(a.sikui(mb))/sa,0.) +
		max(realsikui(a.sikui(ma))/sa-realsikui(b.sikui(ma))/sb,0.) ;
}





/********************************************************/
/************        TEST FUZZY-SET     ***********************/
/********************************************************/

template<class SetType> void test_fuzzy_set(SetType& theset, CStr thesetname) {
	cout << endl << thesetname << " TEST: " << endl;
	EXEC(theset.remove_all());  writeln(cout,theset); //
	EXEC(theset.append('b')); writeln(cout,theset);   // b
	EXEC(theset.append('a')); writeln(cout,theset);   // b a
	EXEC(theset.append('c')); writeln(cout,theset);   // b a c
	EXEC(theset.append('b')); writeln(cout,theset);   // b a c
	EXEC(theset.remove('a')); writeln(cout,theset);   // b c
	EXEC(theset.remove('b')); writeln(cout,theset);   // c
	EXEC(theset.append('a')); writeln(cout,theset);   // c a
	EXEC(theset.remove_all()); writeln(cout,theset);  // 
}	



#undef SET
#undef SETCR
#undef TEMPLATE

#endif
