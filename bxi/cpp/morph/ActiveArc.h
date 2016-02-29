/*
 * File morph/ActiveArc.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/ActiveArc.h
 */

/* ActiveArc.h -- an auxiliary structure for SentenceAnalyzer */

#ifndef __ACTIVEARC_H
#define __ACTIVEARC_H


#include <morph/morphinf.h>
#include <morph/cimcumim.h>
#include <morph/sntncinc.h>
#include <cpplib/vector1.hxx>

struct ActiveArc {         
	const XoqCimcum* xoq_p;
	XoqCimcumCR xoq() const { return *xoq_p; }

	SentenceInfoWithCiyun the_partial_sentence;
	Index from, to;
	bool contains (Index word) const { return from<=word && word<to; }

	//double ciyun_cimcumi;


	ActiveArc (): the_partial_sentence(), my_option_indices(0) {}
	void set (XoqCimcumCR thexoq, Index thefrom, Index theto) { 
		xoq_p=&thexoq; from=thefrom; to=theto; 
		the_partial_sentence.truncate(); 
	}
	ActiveArc (XoqCimcumCR thexoq, Index thefrom, Index theto): the_partial_sentence(), my_option_indices(0) { 
		set(thexoq,thefrom,theto); 
	}

	bool is_complete () const {
		return the_partial_sentence.count()==xoq().ork();        // ha-nquda nimce^t b-sop ha-xoqq.
	}

	bool match (MorphInfoCR theword) const {
		return xoq().match(the_partial_sentence.count(), theword);
	}

	bool can_be_extended_by (ConstituentCR thecons) const {
		return (to==thecons.from && match(thecons.word) );
	}

	status extend_by (ConstituentCR thecons)  {
		to = thecons.to;
		DOr(the_partial_sentence.append(
			thecons.word, thecons.ciyun_cimcumi(), thecons.ciyun_curani()));
		DOr(my_option_indices.append(thecons.my_option_indices));
		return OK;
	}

	bool can_become_a_complete_constituent (Constituent& thecons) {
		double haciyun_hacimcumi;
		if (!xoq().match(the_partial_sentence,0, thecons.word, &haciyun_hacimcumi)) return false;
		thecons.from = from;  thecons.to = to;
		thecons.set_ciyun_cimcumi (haciyun_hacimcumi + the_partial_sentence.ciyun_cimcumi());
		thecons.set_ciyun_curani (the_partial_sentence.ciyun_curani());
		//free(thecons.my_option_indices);
		duplicate(thecons.my_option_indices, my_option_indices);
		return true;
	}

	bool isvalid() const { return the_partial_sentence.isvalid(); }

	Vector1<Index> my_option_indices;  // The indices of the options chosen for each word, from "from" to "to".
};
typedef const ActiveArc& ActiveArcCR;
inline void duplicate (ActiveArc& target, ActiveArcCR source) {
	target.xoq_p = source.xoq_p;
	target.to = source.to;  target.from=source.from;
	::duplicate(target.the_partial_sentence, source.the_partial_sentence);
	::duplicate(target.my_option_indices, source.my_option_indices);
}

inline void free (ActiveArc& a) 
{  
	::free(a.the_partial_sentence);   
	::free(a.my_option_indices);
}

bool sharing (ActiveArcCR a, ActiveArcCR b) { return sharing(a.the_partial_sentence,b.the_partial_sentence); }



#endif
