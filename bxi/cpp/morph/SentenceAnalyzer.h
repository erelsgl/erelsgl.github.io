/*
 * File morph/SentenceAnalyzer.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/SentenceAnalyzer.h
 */

/* SentenceAnalyzer.h 
			[cf. JAMES ALLEN: NATURAL LANGUAGE UNDERSTANDING, 2nd ed., 1995, & 3.4]
*/

#ifndef __SENTENCE_ANALYZER
#define __SENTENCE_ANALYZER

#define TEST_SNTNCANL

#include <morph/PartialSentences.h>
#include <morph/ArrayOfActiveArcs.h>
#include <morph/ArrayOfConstituents.h>
#include <cpplib/queue2.hxx>

class SentenceAnalyzer {
protected:
	
	Size the_wordcount;

	Array2<SentenceInfo>* my_final_options_p;
	#define my_final_options (*my_final_options_p)

	Queue2<Constituent> the_complete_constituents;      // the "agenda"
	ArrayOfActiveArcs  the_active_arcs;
	ArrayOfConstituents  the_old_constituents;

	//PartialSentences the_partial_sentences;
	PartialSentences the_partial_sentences;

	status start (Size the_original_wordcount, Array2<SentenceInfo>& the_options) {
		the_wordcount = the_original_wordcount;
		DOr(the_partial_sentences.reset (the_wordcount,the_wordcount) );
		DOr(the_active_arcs.reset (the_wordcount));
		DOr(the_old_constituents.reset (the_wordcount));
		::free (the_options);
		my_final_options_p = &the_options;
		return OK;
	}

	status extend_the_new_active_arc_and_append_it (ActiveArc& a_new_active_arc, ConstituentCR thecons) {
		a_new_active_arc.extend_by(thecons);
		if (!a_new_active_arc.is_complete()) {
			ActiveArc to_append;  ::duplicate(to_append, a_new_active_arc);
			DOr(the_active_arcs.append(to_append)); 
			return try_to_extend_arc_by_old_constituents(to_append);
		}
		else {
			Constituent the_new_cons;
			if (a_new_active_arc.can_become_a_complete_constituent(the_new_cons)) {
				// enqueue the new constituent
				LOOPVECTOR(;,the_complete_constituents,i) {
					Constituent& cur_constituent = the_complete_constituents.at(i);
					if (identical(cur_constituent,the_new_cons) )  {
						if (the_new_cons.ciyun_sofi() > cur_constituent.ciyun_sofi())
							duplicate (cur_constituent, the_new_cons);
						else
							free (the_new_cons);
						return OK;
					}
				}
				return the_complete_constituents.enqueue(the_new_cons);
			}
			else
				return OK;
		}
	}


	status try_to_extend_arc_by_old_constituents (ActiveArcCR thearc) {
		const SimpleArrayOfConstituents& cur_constituents = the_old_constituents.constituents_that_maybe_can_extend(thearc);
		LOOPVECTOR(;,cur_constituents,c) {
			if (thearc.can_be_extended_by(cur_constituents[c])) {
				ActiveArc a_new_active_arc;
				::duplicate(a_new_active_arc,thearc);
				DOr (extend_the_new_active_arc_and_append_it(a_new_active_arc, cur_constituents[c]));
			}
		}
		return OK;
	}


	status add_new_active_arcs (ConstituentCR cur_constituent) {
		LOOPVECTOR (;,hacimcumim,x) {   
			XoqCimcumCR cur_xoq = hacimcumim[x]; 
			ActiveArc a_new_active_arc;
			a_new_active_arc.set (cur_xoq,cur_constituent.from,cur_constituent.from);
			if (a_new_active_arc.can_be_extended_by (cur_constituent)) {
				DOr (extend_the_new_active_arc_and_append_it(a_new_active_arc, cur_constituent));
			}
		}
		return OK;
	}

	status update_active_arcs (ConstituentCR cur_constituent) {
		const SimpleArrayOfActiveArcs& cur_arcs = the_active_arcs.arcs_that_maybe_can_be_extended_by(cur_constituent);
		LOOPVECTOR (;,cur_arcs,a) {  
			if (cur_arcs[a].can_be_extended_by(cur_constituent)) {
				ActiveArc a_new_active_arc;
				::duplicate(a_new_active_arc,cur_arcs[a]);
				DOr (extend_the_new_active_arc_and_append_it(a_new_active_arc, cur_constituent));
			}
		}
		return OK;
	}


	status create_final_options () {
		DOr(the_partial_sentences.create_whole_sentences());  // This line does NOT cause any memory-leak!
		#ifdef TEST_SNTNCANL
		writeln (sntncanllogfile,the_partial_sentences);
		#endif
		::free (my_final_options);

		//::duplicate ( my_final_options, the_partial_sentences.at(0,the_wordcount-1) );
		const SentenceOptionsWithCiyun& the_reduced_options_for_the_whole_sentence = the_partial_sentences.at(0,the_wordcount-1);
		my_final_options.reset(the_reduced_options_for_the_whole_sentence.count());
		LOOPVECTOR(;,the_reduced_options_for_the_whole_sentence,i)
			::duplicate(my_final_options[i], the_reduced_options_for_the_whole_sentence[i]);

		::duplicate ( my_final_option_indices, the_reduced_options_for_the_whole_sentence.my_option_indices);

		sntncanllogfile << "my_final_options: "; 
		writeln (sntncanllogfile,my_final_options,Format("*\n"));  sntncanllogfile<<endl;
		sntncanllogfile << "my_final_option_indices: ";
		writeln (sntncanllogfile,my_final_option_indices,Format("*\n"));  sntncanllogfile<<endl<<endl<<endl;
		return OK;
	}


	status process_all_complete_constituents () {
		while (!the_complete_constituents.isempty()) {
			Constituent cur_constituent = the_complete_constituents.athead();
			sntncanllogfile<<cur_constituent<<" ";
			#ifdef TEST_SA_AUX
			cerr<<cur_constituent<<" ";
			#endif
			DOr(update_active_arcs(cur_constituent));
			DOr(add_new_active_arcs(cur_constituent)); 
			DOr(the_partial_sentences.append(cur_constituent));
			DOr(the_old_constituents.append(cur_constituent));
			the_complete_constituents.dequeue();
		}
		sntncanllogfile<<endl;
		#ifdef TEST_SA_AUX
		cerr<<endl<<endl;
		#endif
		return OK;
	}


public:

	Vector1<Index> my_final_option_indices;

	status natax (SentenceInfoCR the_original_sentence, Array2<SentenceInfo>& the_options) {
		DOr(start(the_original_sentence.count(),the_options));
		LOOPVECTOR (;,the_original_sentence,w)  {
			MorphInfoCR curword = the_original_sentence[w];
			DOr(the_complete_constituents.enqueue (
				Constituent(curword,w,w+1)));
			DOr(process_all_complete_constituents());
		}
		DOr(create_final_options());
		return OK;
	}

	status natax_jenit (Index word_to_replace, MorphInfoCR new_word) {
		the_active_arcs.remove_all_arcs_containing(word_to_replace);
		the_old_constituents.remove_all_constituents_containing(word_to_replace);
		the_partial_sentences.remove_all_partial_sentences_containing(word_to_replace);
		DOr(the_complete_constituents.enqueue (
			Constituent(new_word,word_to_replace,word_to_replace+1)));
		DOr(process_all_complete_constituents());
		DOr(create_final_options());
		return OK;
	}
	friend void duplicate (SentenceAnalyzer& to, const SentenceAnalyzer& from);

	status finish() {
		::free(the_complete_constituents);
		//isvalid(the_active_arcs);
		::free(the_active_arcs);
		::free(the_old_constituents);
		::free(the_partial_sentences);
		return OK;
	}

	~SentenceAnalyzer() { finish(); }

};




void duplicate (SentenceAnalyzer& to, const SentenceAnalyzer& from) {
	to.the_wordcount = from.the_wordcount;
	to.my_final_options_p = from.my_final_options_p;

	//isvalid(from.the_active_arcs);
	::duplicate(to.the_active_arcs, from.the_active_arcs);
	//isvalid(to.the_active_arcs);
	::duplicate(to.the_old_constituents.theconstituents,from.the_old_constituents.theconstituents);
	::duplicate(to.the_partial_sentences,from.the_partial_sentences);
}




/*******************************************************************/
/*********  public routines						      **************/
/*******************************************************************/

bool the_sentence_analyzer_is_initialized = false;

void initialize_the_sentence_analyzer (CStr path_to_cimcumim) {
	assert (OK==qra_cimcumim(path_to_cimcumim) );
	//assert (OK==qra_lexicon_bituyim(path_to_cimcumim) );
	//ktov_lexicon_bituyim(path_to_cimcumim);
	the_sentence_analyzer_is_initialized = true;
}

#endif
