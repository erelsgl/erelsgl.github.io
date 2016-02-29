/*
 * File morph/ArrayOfConstituents.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/ArrayOfConstituents.h
 */

/* ArrayOfConstituents.h -- an auxiliary structure for SentenceAnalyzer */


#include <morph/Constituent.h>
#include <morph/ActiveArc.h>
#include <cpplib/vector1.hxx>
#include <cpplib/array2.hxx>




typedef Array2<Constituent> SimpleArrayOfConstituents;

// This structure stores the constituents according to the "from" field.
struct ArrayOfConstituents {
	Vector1 < SimpleArrayOfConstituents >  theconstituents;
	static SimpleArrayOfConstituents the_empty_set;
	const SimpleArrayOfConstituents& constituents_that_maybe_can_extend(ActiveArcCR thearc) const {
		if (thearc.to>=theconstituents.count()) return the_empty_set;
		return theconstituents[thearc.to]; 
	}
	status append (ConstituentCR thecons) { return theconstituents[thecons.from].append(thecons); }
	status reset (Size thesize) { return theconstituents.reset(thesize); }
	void remove_all_constituents_containing (Index word) {
		for (Index from=0; from<=word; ++from) {
			SimpleArrayOfConstituents& curconstituents = theconstituents[from];
			LOOPVECTOR(;,curconstituents,a) 
				while (curconstituents[a].contains(word) && a<curconstituents.count())
					curconstituents.removeat(a);
		}
	}
};
void free (ArrayOfConstituents& aoc) { ::free(aoc.theconstituents); }
void duplicate (ArrayOfConstituents& to, const ArrayOfConstituents& from) { ::duplicate(to.theconstituents,from.theconstituents); }

SimpleArrayOfConstituents ArrayOfConstituents::the_empty_set;
