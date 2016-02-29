/*
 * File cpplib/histgram.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/histgram.h
 */

/* histgram.h -- Histogram */

#ifndef __HISTGRAM_H
#define __HISTGRAM_H


/*
	This unit defines a Histogram counter.
	When you initialize the Histogram, you set the size of the bins.
	Then you add data (numbers) to the Histogram, and it counts how many data
		there is in each bin.
	For example:

	HistogramCounter myhist (1000);  // 
	myhist.add(0);    // bin #1
	myhist.add(5);    // bin #1
	myhist.add(995);  // bin #1
	myhist.add(999);  // bin #1
	myhist.add(1000); // bin #2

    cout << myhist.count(0);      // prints 4
	cout << myhist.count(500);    // prints 4
	cout << myhist.count(1000);   // prints 1
	cout << myhist.count(2000);   // prints 0

*/

#include <cpplib/vector1.hxx>
#include <cpplib/map.hxx>
#include <cpplib/countc.hxx>
#include <cpplib/stattemp.h>
#include <cpplib/sgt.h>
#include <math.h>

class HistogramCounter;
typedef const HistogramCounter& HistogramCounterCR;

class HistogramCounter {

	double my_bin_size;
	int bin_num (double the_data) const { return (int)(the_data/my_bin_size); }
	CounterVector1Sorted<int> my_bin_counter;

	// fixed frequencies-of-frequencies, as calculated by the Good-Turing formula
	Map1<int,double> my_goodturing_counts;

public:
	/***** construct and destruct *******/
	HistogramCounter(double the_bin_size):  my_bin_counter(), my_bin_size(the_bin_size) { }  		


	/************ queries **************************/
	Count count (double the_data) const { return my_bin_counter.count(bin_num(the_data)); }
	Count count (int the_data) const { return my_bin_counter.count(bin_num(the_data)); }

	Count contains (double the_data) const { return my_bin_counter.contains(bin_num(the_data)); }
	Count contains (int the_data) const { return my_bin_counter.contains(bin_num(the_data)); }

	status setcount (double the_data, uint thecount=0) { return my_bin_counter.setcount(bin_num(the_data), thecount); }
	status setcount (int the_data, uint thecount=0) { return my_bin_counter.setcount(bin_num(the_data), thecount); }

	status insert (double the_data, uint howmany=1) { return my_bin_counter.insert(bin_num(the_data), howmany); }
	status insert (int the_data, uint howmany=1) { return my_bin_counter.insert(bin_num(the_data), howmany); }

	status add (double the_data, uint howmany) { return insert(the_data, howmany); }
	status add (int the_data, uint howmany) { return insert(the_data, howmany); }

	status append (double the_data) { return insert(the_data); }
	status append (int the_data) { return insert(the_data); }

	status remove_one (double the_data, uint howmany=1) { return my_bin_counter.remove_one(bin_num(the_data), howmany); }
	status remove_one (int the_data, uint howmany=1) { return my_bin_counter.remove_one(bin_num(the_data), howmany); }

	status remove (double the_data) { return my_bin_counter.remove(bin_num(the_data)); }
	status remove (int the_data) { return my_bin_counter.remove(bin_num(the_data)); }

	const CounterVector1Sorted<int>& bincounter()  const { return my_bin_counter; }
	CounterVector1Sorted<int>& bincounter()              { return my_bin_counter; }

	void calculate_goodturing_counts(int the_number_of_unseen_species) {
		assert(!my_bin_counter.isempty());
		SimpleGoodTuringEstimator(my_bin_counter).get_fixed_frequencies(my_goodturing_counts,the_number_of_unseen_species); 
	}

	void calculate_goodturing_counts() {
		assert(!my_bin_counter.isempty());
		SimpleGoodTuringEstimator(my_bin_counter).get_fixed_frequencies(my_goodturing_counts); 
	}

	double goodturing_count (double the_data) const { return my_goodturing_counts.item(bin_num(the_data)) * my_bin_size; }
	double goodturing_count (long the_data) const { return my_goodturing_counts.item(bin_num(the_data)) * my_bin_size; }
	double goodturing_count (int the_data) const { return my_goodturing_counts.item(bin_num(the_data)) * my_bin_size; }
};

void write (ostream& out, HistogramCounterCR thehc, Format format) {
	write (out,thehc.bincounter(),format); }

void write (ostream& out, HistogramCounterCR thehc) { write (out,thehc.bincounter() ); }

status read (istream& in, HistogramCounter& thehc, Format format) { 
	return read (in, thehc.bincounter(), format); }

status read (istream& in, HistogramCounter& thehc) { return read (in,thehc.bincounter() ); }



#endif
