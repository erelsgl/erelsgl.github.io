/*
 * File cpplib/matrix2.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/matrix2.hxx
 */

/* matrix2.hxx -- a 2-dim matrix implemented as a 1-dim vector. */



#ifndef __MATRIX2_HXX

#define __MATRIX2_HXX



#include <cpplib/klli1.h>

#include <cpplib/io2.h>



#define TEMPLATE template <class Data>

#define MATRIX Matrix2<Data>

#define MATRIXCR const Matrix2<Data>&



/**************************************************************************

**************************************************************************/





/**************************************************************************/

/**********        Matrix2                            ***********************/

/**************************************************************************/





TEMPLATE class Matrix2 {

protected:

	Pointer1<Data> myinfo;

	Size my_rowcount, my_colcount;



public:		

	typedef Data Data;

	/***********  construction and destruction **********/



	Matrix2 (Size the_rowcount=0, Size the_colcount=0)  { 

		my_rowcount = the_rowcount;  my_colcount = the_colcount;

		DOx(myinfo.alloc(the_rowcount*the_colcount));  

	}

	~Matrix2() { /* free();*/  } // nothing is done automatically!



	void free() { myinfo.free(); }

	void remove_all() { myinfo.free(); }





	status realloc(Size the_rowcount, Size the_colcount) { 

		my_rowcount = the_rowcount;  my_colcount = the_colcount;

		Data* old_elements; 

		DOr (myinfo.realloc (the_rowcount*the_colcount,old_elements));

		vecfree(old_elements);	

		return OK;

	}



	status reset(Size the_rowcount, Size the_colcount) { return realloc(the_rowcount, the_colcount); }





	/**************      assignment functions     ***********************/

	status share (MATRIXCR from) { return myinfo.share(from.myinfo); }

	status steal (MATRIX& from)  { return myinfo.steal(from.myinfo); }

	void operator= (MATRIXCR from) { share(from); }



	/****************              query              *******************/

	Index rowcount() const { return my_rowcount; }

	Index colcount() const { return my_colcount; }

	bool isempty() const { return myinfo.p==NULL; }



	DataR at (Index row, Index col)		{ return myinfo[row*my_colcount + col]; }

	DataCR at (Index row, Index col) const	{ return myinfo[row*my_colcount + col]; }

	



	bool operator==(int ZERO) const { return isempty(); }

	void operator= (int ZERO) { free(); }


	void duplicate (MATRIXCR from) {
		my_rowcount=from.my_rowcount; 
        my_colcount=from.my_colcount;
		duplicate (myinfo, from.myinfo);
    }
};



TEMPLATE void duplicate (MATRIX& to, MATRIXCR from) {
	to.duplicate(from);
}

TEMPLATE void free (MATRIX& v) {
	v.free();
}







#define LOOPMATRIXROWS(init,thematrix,therow)  init;  \

	for(Index therow=0; therow<thematrix.rowcount(); ++therow)  



#define LOOPMATRIXCOLS(init,thematrix,thecol)  init;  \

	for(Index thecol=0; thecol<thematrix.colcount(); ++thecol)  



#define LOOPMATRIX(init,thematrix,therow,thecol)  init;  \

	for(Index therow=0; therow<thematrix.rowcount(); ++therow)  \

		for(Index thecol=0; thecol<thematrix.colcount(); ++therow)









/************************************************************************************************/

/************  	IO operations      **************************************************************/

/************************************************************************************************/





/* [BEWARE! OUTDATED DOCUMENTATION!]

	The output format should contain at least one char, which is the item-separator.

	If the format is preceded by 'P', it says that the output will be surrounded

		by parenthesis.

	If the format is preceded by '*', it will be skipped.

	examples: (the vector  contains a,b,c and d)

		"P "           ==> [4: a b c d]

		"P,"           ==> [4: a,b,c,d]

		" "  (or "* ") ==> a b c d

		","  (or "*,") ==> a,b,c,d

*/

TEMPLATE void write (ostream& out, MATRIXCR themat, Format format="") {

//	if (format[0]=='P')   {         // Parenthesized format: [n: a1 a2 ... an]

//		format = format.next();

		out << '[';

		out << themat.rowcount() << " " << themat.colcount() << ": " << endl;

		LOOPMATRIXROWS(;,themat,row) {

			LOOPMATRIXCOLS(;,themat,col) {

				write (out,themat.at(row,col),format);

				out <<"|";

			}

			out << endl;

		}

		out << endl << "]";

//	}

//	else {                          // Simple format: a1 a2 ... an

//		if (format[0]=='*' || format[0]=='L')  format=format.next();

//		write_vector (out,thevec, str(format[0]),format.next() );

//	}

}



//TEMPLATE void write (ostream& out, ARRAYCR thevec) { write (out,thevec,""); }



TEMPLATE status read (istream& in, MATRIX& themat, Format format="P") {

//	if (format[0]=='P')   {         // Parenthesized format: [n: a1 a2 ... an]

//		format = format.next();

		DOr(testchar(in,'['));

		Size therowcount=0, thecolcount=0;

		DOr(read(in,therowcount));  DOr(read(in,thecolcount));

		DOr(testchar(in,':'));

		DOr(themat.reset(therowcount,thecolcount));

		Data curdata;

		LOOPMATRIXROWS(;,themat,row) {

			LOOPMATRIXCOLS(;,themat,col) {

				DOr(read(in,curdata));

				themat.at(row,col) = curdata;

				DOr(testchar(in,'|'));

			}

		}

		DOr(testchar(in,']'));

//	}

//	else if (format[0]=='L')    {   // line format: read myinfo.p until end-of-line

//		format = format.next();

//		thevec.truncate();

//		return read_container (in,thevec, format[0], '\n', format.next()); 

//	}

//	else {                          // Simple format: a1 a2 ... an (until the EOF or another error).

//		if (format[0]=='*')  format=format.next();

//		thevec.truncate();

//		return read_container (in,thevec, format[0], '\0', format.next()); 

//	}

}





#undef TEMPLATE

#undef MATRIX

#undef MATRIXCR

#undef CURSOR 

#undef CURSORCR

#endif



