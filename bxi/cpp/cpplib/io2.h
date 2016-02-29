/*
 * File cpplib/io2.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/io2.h
 */

/* io2.h -- some useful template IO routines */
/*
	This unit defines the following template functions:
		void write_vector (the_output, the_vector, the_separator, the_data_format);
		status read_vector (the_input, the_vector, the_separator, the_stopper, the_data_format);
			// read elements (seperated by the_separator) until the_stopper is read. 
		void  write_container (the_input, the_container, the_separator, Format the_data_format);
		status  read_container (the_input, the_container, the_separator, the_stopper, the_data_format);
		void write_pair (the_output, the_key, the_separator, the_data, the_key_format, the_data_format);
		status read_pair (the_input, the_key, the_separator, the_data, the_key_format, the_data_format);
*/

#ifndef __IO2_H
#define __IO2_H

#include <cpplib/io.h>
#include <cpplib/stdtempl.h>


#define LOG(logfile,output) { logfile<<output;  cerr<<output;  }

/* write_vector:
	writes thevector's items into out. separates them by theseparator (use theseparator='\0' for no separator).
*/
template<class Vector>  void  write_vector (ostream& out, const Vector& thevector, 
											CStr theseparator, Format thedataformat) {
	if (thevector.isempty()) return;
	if (theseparator[0]=='\n') {
		LOOPVECTOR (;,thevector,i) {
			out << theseparator;
			write (out,thevector[i],thedataformat);
		}
	}
	else {
		LOOPVECTOR (;,thevector,i) {
			write (out,thevector[i],thedataformat);
			if ( i+1 < thevector.count() )     out << theseparator;
		}
	}
}



/* read_vector:
	reads items from in into thevector, separated by theseparator, until thestopper is read. 
		(use theseparator='\0' for no separator).
	thevector should be large enough to contain all the items read!!
*/
template<class Vector>  status  read_vector (istream& in, Vector& thevector, 
											 char theseparator, char thestopper, Format thedataformat) {
	char thesep = (theseparator==' ' || theseparator=='\n')? '\0': theseparator;  
	Index themaxsize = thevector.size();
	if (testchar(in,thestopper)==OK) { thevector.truncate(0);  return OK; }
	for (Index i=0; i<themaxsize; ++i) {
		DOr ( read (in, thevector[i], thedataformat ) );
		if (testchar(in,thestopper)==OK) { thevector.truncate(i+1);  return OK; }
		if ( i+1 < themaxsize ) DOr (testchar(in,thesep));
	}
	return testchar(in,thestopper);
}


/* write_container:
	writes thecont's items into out. separates them by theseparator (use theseparator='\0' for no separator).
*/
template<class Container>  void  write_container (
 istream& in, Container& thecont,
 CStr theseparator, Format thedataformat) {
	if (thecont.isempty()) return;
	typename Container::Cursor i (thecont);
	write (out, i.item(), thedataformat);
	for (i.next(); i.isvalid(); i.next() ) {
		out << theseparator;
		write (out,i.item(),thedataformat);
	}
}


/* read_container:
	read items from in into thecont (which must be a container of type Data). The items are separated by
		theseparator. The items are read until thestopper is seen in the input.
*/
template<class Container>  status  read_container (istream& in, Container& thecont, 
						            			   char theseparator, char thestopper, Format thedataformat) {
	if (theseparator==' ' || theseparator=='\n')  theseparator='\0';
	if (thestopper && testchar(in,thestopper)==OK) return OK; 
	for (Index i=0;;++i) {
		typename Container::Data theinput;
		DOr ( read (in, theinput, thedataformat ) );
		DOr (thecont.append(theinput));
		if (testchar(in,thestopper)==OK) return OK; 
		DOr (testchar(in,theseparator));
	}
}



/* write_pair
*/

template<class Key,class Data> void write_pair (ostream& out, const Key& thekey, CStr theseparator, const Data& thedata,
												Format keyformat, Format dataformat) {
	write (out, thekey, keyformat);
	out << theseparator;
	write (out, thedata, dataformat);
}

/* read_pair
*/

template<class Key,class Data>  status read_pair (istream& in, Key& thekey, char theseparator, Data& thedata, Format keyformat, Format dataformat) {
	DOr ( read (in, thekey, keyformat) );
	DOr ( testchar(in,theseparator) );
	DOr ( read (in, thedata, dataformat) );
	return OK;
}









#endif
