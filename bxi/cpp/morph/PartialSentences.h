/*
 * File morph/PartialSentences.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/PartialSentences.h
 */

/* PartialSentences.h -- auxiliary structures for SentenceAnalyzer */


#ifndef __PARTIAL_SENTENCES
#define __PARTIAL_SENTENCES

#include <morph/sntncinc.h>
#include <morph/Constituent.h>
#include <cpplib/matrix1.hxx>


extern ofstream sntncanllogfile;


/*--------------------- SentenceOptionsWithCiyun -------------------------*/
/*--------------------- SentenceOptionsWithCiyun -------------------------*/
// This structure contains sentence-options which all have the same ciyun_sofi().

class SentenceOptionsWithCiyun;
typedef const SentenceOptionsWithCiyun& SentenceOptionsWithCiyunCR;
class SentenceOptionsWithCiyun: public Array2<SentenceInfoWithCiyun>  {
	double my_ciyun_sofi;
public:
	Vector1<Index> my_option_indices;  // The indices of the options chosen for each word, from "from" to "to".
	static SentenceInfoWithCiyun a_new_option;
	SentenceOptionsWithCiyun():  Array2<SentenceInfoWithCiyun>(0), my_ciyun_sofi(MIN_CIYUN), my_option_indices() {}
	void remove_all() {
		LOOPTHISVECTOR(;,i)  ::free(at(i));
		truncate();
		my_option_indices.truncate();
		my_ciyun_sofi=MIN_CIYUN;
	}


	status append (SentenceInfoWithCiyunCR the_new_option, const Vector1<Index>& the_option_indices) {
		double the_new_ciyun_sofi = the_new_option.ciyun_sofi();
		if (the_new_ciyun_sofi > my_ciyun_sofi) {  // The new option is better, so throw away all current options...
			remove_all();
			SentenceInfoWithCiyun to_append;  ::duplicate(to_append,the_new_option);
			my_ciyun_sofi = the_new_ciyun_sofi;
			DOr(my_option_indices.append(the_option_indices));
			return Array2<SentenceInfoWithCiyun>::append(to_append);
		}
		else if (the_new_ciyun_sofi == my_ciyun_sofi) { // The new option is equivalent, so append it.
			if (!contains(*this,the_new_option)) {
				SentenceInfoWithCiyun to_append;  ::duplicate(to_append,the_new_option);
				if (my_option_indices.isempty()) {
					DOr(my_option_indices.append(the_option_indices));
				}
				return Array2<SentenceInfoWithCiyun>::append(to_append);
			}
			else return OK;
		}
		else {                            // The new option is worse, so do nothing.
			return OK;
		}
	}

	status append (MorphInfo the_word, double the_ciyun_cimcumi, double the_ciyun_curani, const Vector1<Index>& the_option_indices) {         // append a single-word option
		a_new_option.truncate();
		DOr(a_new_option.append(the_word, the_ciyun_cimcumi, the_ciyun_curani));
		DOr(append(a_new_option, the_option_indices));
		return OK;
	}

	status append (SentenceInfoWithCiyunCR firsthalf, SentenceInfoWithCiyunCR secondhalf, double the_ciyun_cimcumi=0) {  //  concat the halves and append the result
		static Vector1<Index> empty_indices_vector;
		a_new_option.truncate();
		DOr(a_new_option.append(firsthalf));
		DOr(a_new_option.append(secondhalf));
		return append(a_new_option, empty_indices_vector);
	}

	status append (const Array2<SentenceInfoWithCiyun>& firsthalfoptions, const Array2<SentenceInfoWithCiyun>& secondhalfoptions) {
		LOOPVECTOR(;,firsthalfoptions,i) {
			LOOPVECTOR(;,secondhalfoptions,j) {
				DOr(append(firsthalfoptions[i], secondhalfoptions[j])); }}
		return OK;
	}

	status append (SentenceOptionsWithCiyunCR firsthalfoptions, SentenceOptionsWithCiyunCR secondhalfoptions) {
		double the_new_ciyun_sofi = firsthalfoptions.my_ciyun_sofi + secondhalfoptions.my_ciyun_sofi - MAX_CIYUN;
		if (the_new_ciyun_sofi < my_ciyun_sofi)  return OK;
		else {
			DOr (append ( (const Array2<SentenceInfoWithCiyun>&)firsthalfoptions, (const Array2<SentenceInfoWithCiyun>&)secondhalfoptions ));
			if (my_option_indices.isempty()) {
				DOr (my_option_indices.append(firsthalfoptions.my_option_indices));
				DOr (my_option_indices.append(secondhalfoptions.my_option_indices));
			}
			return OK;
		}
	}

	friend void duplicate (SentenceOptionsWithCiyun& to, const SentenceOptionsWithCiyun& from);
};

SentenceInfoWithCiyun SentenceOptionsWithCiyun::a_new_option;

void duplicate (SentenceOptionsWithCiyun& to, const SentenceOptionsWithCiyun& from) { 
	::duplicate( (Array2<SentenceInfoWithCiyun>&)to, (const Array2<SentenceInfoWithCiyun>&)from ); 
	::duplicate( to.my_option_indices, from.my_option_indices ); 
	to.my_ciyun_sofi = from.my_ciyun_sofi;
}

void free (SentenceOptionsWithCiyun& o) { 
	o.remove_all();
	::free ( (Array2<SentenceInfoWithCiyun>)o );
	::free (o.my_option_indices);
}



/*--------------------- PartialSentences -------------------------*/
/*--------------------- PartialSentences -------------------------*/
// Each SentenceOptionsWithCiyun keeps all best-options found so far for 
//	analyzing the part of the sentence between "row" and "col+1". (i.e. row==from, col==to-1)

struct PartialSentences: Matrix1<SentenceOptionsWithCiyun> {

	PartialSentences() {}

	status append (ConstituentCR thecons) {
		return at(thecons.from, thecons.to-1).append(thecons.word, thecons.ciyun_cimcumi(), thecons.ciyun_curani(), thecons.my_option_indices);
	}


	status create_whole_sentences () {
		// Algorithm: create partial-sentences of increasing lengths.
		// PRECONDITION: all partial sentence of length 1 are already in the table 
		//		(they were inserted when the corresponding Constituents were removed from the agenda).
		for (Index PSL=2; PSL<=rowcount(); ++PSL) {  // PSL==Partial Sentence Length
			// INVARIANT: the partial-sentences of length < PSL are already in the table.
			Index from, to;
			for (from=0, to=PSL; to<=colcount(); ++from, ++to)  {
				Data& the_options = at(from,to-1);
				assert(the_options.len() <= the_options.size());
				for (Index middle=from+1; middle<=to-1; ++middle) {
					DataCR firsthalfoptions = at(from,middle-1);
					DataCR secondhalfoptions = at(middle,to-1);
					DOr( the_options.append (firsthalfoptions, secondhalfoptions) );
					assert(the_options.my_option_indices.count() == PSL);
				}
			}
		}
		return OK;
	}


	void remove_all_partial_sentences_containing (Index theword) {
		for (Index from=0; from<=theword; ++from)
			for (Index to=theword+1; to<=colcount(); ++to)
				at(from,to-1).remove_all();
	}

	void remove_all() {
		for (Index r=0; r<rowcount(); ++r)
			for (Index c=0; c<colcount(); ++c)
				::free(at(r,c));
	}

};


#endif
