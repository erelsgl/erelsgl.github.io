/*
 * File morph/mone-nit.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/mone-nit.h
 */

/* mone-nit.h -- mone nittuxim */
/*
	The structure MoneNittuxim contains an entry for each distinct 
		MorphInfoBasic in the corpus. It counts the 'sikuiim' of all 
		MorphInfoBasic's similar to it.
*/

#ifndef __MONENIT_H
#define __MONENIT_H

#include <cpplib/countc.hxx>
#include <cpplib/map.hxx>
#include <cpplib/vector1.hxx>
#include <cpplib/sikui.h>
#include <morph/morphinf.h>
#include <morph/similar.h>

typedef CounterVector1<MorphInfoBasic> MIBCounter;

class MoneNituxim;
typedef const MoneNituxim& MoneNituximCR;

class MoneNituxim {
	MIBCounter mycounters [16];
	MIBCounter& counter (HeleqDiber hd) { return mycounters[hd+8]; }
	const MIBCounter& counter (HeleqDiber hd) const { return mycounters[hd+8]; }
	Count my_skum;
public:
	void zero_all();
	status setcount (MorphInfoBasicCR theinfo, Count thecount=0);
	status add (MorphInfoBasicCR theinfo, Sikui thesikui=SIKUI1);
	status add_exact (MorphInfoBasicCR theinfo, Sikui thesikui=SIKUI1);
	Count count (MorphInfoBasicCR theinfo) const;
	double relative_count (MorphInfoBasicCR theinfo) const;
	friend void write (ostream& out, MoneNituximCR theinfo);
	friend status read (istream& in, MoneNituxim& theinfo);
	friend void duplicate (MoneNituxim& to, MoneNituximCR from);
	bool isempty() const {
		for (Index i=0; i<16; ++i) 
			if (!mycounters[i].isempty()) return false;
		return true;
	}

	friend class MoneNituximCursor;

	typedef MoneNituximCursor Cursor;
};




class MoneNituximCursor {
protected:
	MoneNituxim* my_mone;
	int my_counter_index;
	MIBCounter::Cursor my_counters_cursor;

	void go_to_next_valid () {
		for (;;) {
			if (my_counters_cursor.isvalid()) break;    // found valid
			++my_counter_index;
			if (my_counter_index>=16) break;            // didn't find valid
			my_counters_cursor.reset(my_mone->mycounters[my_counter_index]);
		}
	}

public:

	/* queries */
	MorphInfoBasic data() const { return my_counters_cursor.data(); }
	Count& count() { return my_counters_cursor.count(); }
	const Count& count() const { return my_counters_cursor.count(); }
	bool isvalid() const { 
		return my_counter_index < 16;
	}


	/* use */
	void reset (MoneNituxim& the_mone) {
		my_mone = &the_mone;
		my_counter_index = 0;
		my_counters_cursor.reset(my_mone->mycounters[my_counter_index]);
		go_to_next_valid();
	}

	MoneNituximCursor (MoneNituxim& the_mone) { reset(the_mone);  }

	void next() { 
		my_counters_cursor.next();
		go_to_next_valid();
	}

	void operator++() { next(); }
};


#endif
