/*
 * File morph/ArrayOfActiveArcs.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/ArrayOfActiveArcs.h
 */

/* ArrayOfActiveArcs.h -- an auxiliary structure for the SentenceAnalyzer */


#include <morph/ActiveArc.h>
#include <cpplib/vector1.hxx>
#include <cpplib/array2.hxx>



typedef Array2<ActiveArc> SimpleArrayOfActiveArcs;

// This structure stores the arcs according to the "to" field.
struct ArrayOfActiveArcs {
	Vector1 < SimpleArrayOfActiveArcs >  thearcs;
	static SimpleArrayOfActiveArcs the_empty_set;
	const SimpleArrayOfActiveArcs& arcs_that_maybe_can_be_extended_by(ConstituentCR thecons) const {
		if (thecons.from==0) return the_empty_set;
		return thearcs[thecons.from-1]; 
	}
//	status append (ActiveArcCR thearc) { return thearcs[thearc.to].append(thearc); }
	status append (ActiveArcCR thearc) { 
		SimpleArrayOfActiveArcs& curarcs = thearcs[thearc.to-1];
		if (curarcs.len()>0) {
			ActiveArcCR lastarc = curarcs.last();
			assert (!sharing(thearc, lastarc));
		}
		return curarcs.append(thearc); 
	}
	status reset (Size thesize) { return thearcs.reset(thesize); }
	void remove_all_arcs_containing (Index word) {
		for (Index to=word+1; to<thearcs.count(); ++to) {
			SimpleArrayOfActiveArcs& curarcs = thearcs[to-1];
			LOOPVECTOR(;,curarcs,a) 
				while (curarcs[a].contains(word) && a<curarcs.count())
					curarcs.removeat(a);
		}
	}
};

SimpleArrayOfActiveArcs ArrayOfActiveArcs::the_empty_set;

void isvalid (const SimpleArrayOfActiveArcs& curarcs) { 
	for (Index a=0; a+1<curarcs.count(); ++a) {
		const ActiveArc& curarc = curarcs[a];
		const ActiveArc& nextarc = curarcs[a+1];
		assert (!sharing(curarc,nextarc));
	}
}

void isvalid (const ArrayOfActiveArcs& aoaa) { 
	LOOPVECTOR(;,aoaa.thearcs,to) {
		const SimpleArrayOfActiveArcs& curarcs = aoaa.thearcs[to];
		isvalid(curarcs);
	}
}

void free (ArrayOfActiveArcs& aoaa) { ::free (aoaa.thearcs); }
void duplicate (ArrayOfActiveArcs& to, const ArrayOfActiveArcs& from) { ::duplicate(to.thearcs, from.thearcs); }
/*void duplicate (ArrayOfActiveArcs& to, const ArrayOfActiveArcs& from) { 
	DOx(to.thearcs.reset(from.thearcs.size()));
	LOOPVECTOR(;,from.thearcs,c) {
		const SimpleArrayOfActiveArcs& fromcurarcs = from.thearcs[c];
		SimpleArrayOfActiveArcs&       tocurarcs = to.thearcs[c];
		isvalid (fromcurarcs);
		::duplicate(tocurarcs,fromcurarcs);
		isvalid (tocurarcs);
		//LOOPVECTOR(;,fromcurarcs,a) {
		//	const ActiveArc& fromcurarc = fromcurarcs[a];
		//	ActiveArc&       tocurarc =   tocurarcs[a];
		//	::duplicate (tocurarc,fromcurarc);
		//}
	}
}*/



