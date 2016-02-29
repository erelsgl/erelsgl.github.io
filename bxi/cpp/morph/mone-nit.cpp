/*
 * File morph/mone-nit.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/mone-nit.cpp
 */

/* mone-nit.cpp -- mone nittuxim */

#include <morph/mone-nit.h>



	void MoneNituxim::zero_all() {
		for (Index i=0; i<16; ++i)  mycounters[i].zero_all(); 
		my_skum = 0;
	}
	status MoneNituxim::setcount (MorphInfoBasicCR theinfo, Count thecount) {
		MIBCounter& thecounter = counter(theinfo.heleqdiber());
		return thecounter.setcount(theinfo,thecount);
	}
	status MoneNituxim::add (MorphInfoBasicCR theinfo, Sikui thesikui)   {    // add the sikui to all entries 'similar' to the info.
		MIBCounter& thecounter = counter(theinfo.heleqdiber());
		for (MIBCounter::Cursor c(thecounter); c.isvalid(); ++c) {
			MorphInfoBasicCR curdata = c.data();
			Dimion d = dimion(theinfo,curdata);
			c.count() += thesikui*d / mispar_milim_domot(curdata);
			my_skum += thesikui*d / mispar_milim_domot(curdata);
		}
		return OK;
	}
	status MoneNituxim::add_exact (MorphInfoBasicCR theinfo, Sikui thesikui)   {    // add the sikui to all entries 'similar' to the info.
		MIBCounter& thecounter = counter(theinfo.heleqdiber());
		DOr(thecounter.add(theinfo,thesikui));
		my_skum += thesikui;
		return true;
	}
	Count MoneNituxim::count (MorphInfoBasicCR theinfo) const  {
		return counter(theinfo.heleqdiber()).count(theinfo); }
	double MoneNituxim::relative_count (MorphInfoBasicCR theinfo) const {
		return double(counter(theinfo.heleqdiber()).count(theinfo)) / double(my_skum); }




void write (ostream& out, MoneNituximCR theinfo) { 
	out << "{" << theinfo.my_skum << endl;
	for (Index i=0; i<16; ++i)
		write(out, theinfo.mycounters[i], Format("t:")); 
	out << "}" << endl;
}

status read (istream& in, MoneNituxim& theinfo) {
	DOr(testchar(in,'{'));
	DOr(read(in,theinfo.my_skum));
	char the_stopper='}', the_keydata_separator=':', the_line_separator='\0';
	Format keyformat="", dataformat="";
	if (testchar(in,the_stopper)==OK) return OK; 
	for (Index i=0;;++i) {
		KeyData <MorphInfoBasic,Count> theinput;
		DOr ( read_keydata (in, theinput, the_keydata_separator, keyformat, dataformat ) );
		DOr ( theinfo.setcount ( theinput.key(), theinput.data() ) );
		if (testchar(in,the_stopper)==OK) return OK; 
		DOr ( testchar(in,the_line_separator) );
	}
	return OK;
}

void duplicate (MoneNituxim& to, MoneNituximCR from) {
	for (Index i=0; i<16; ++i)
		duplicate (to.mycounters[i], from.mycounters[i]);
}

