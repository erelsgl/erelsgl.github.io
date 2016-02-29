/*
 * File cpplib/sentence.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/sentence.h
 */

/* sentence.h -- Sentence -- a string that represents a sentence, divided into words */
/*
*/


#ifndef __SENTENCE_H
#define __SENTENCE_H

#include <cpplib/stringt.h>
#include <cpplib/array2.hxx>
#include <cpplib/range.hxx>

typedef Range<uint> RangeOfUints;
typedef Array2<RangeOfUints> ArrayOfRangeOfUints;


struct Sentence;
typedef const Sentence& SentenceCR;

struct Sentence: public StringTemp {
protected:
	ArrayOfRangeOfUints word_ranges;
	void update_word_ranges () {
		word_ranges.truncate();
		for(Index cur_word=0, cur_char=0 ; ; ++cur_word) {
			uint cur_word_start, cur_word_end;
			while (isspace(str[cur_char]) && cur_char<len) cur_char++;
			if (cur_char==len) break;
			cur_word_start = cur_char;
			while (!isspace(str[cur_char]) && cur_char<len) cur_char++;
			cur_word_end = cur_char;
			word_ranges.append_empty();
			word_ranges[cur_word].set (cur_word_start,cur_word_end);
		}
	}

public:
	Sentence (Size thesize=0): StringTemp(thesize) {}
	Sentence (CStr thestr): StringTemp(thestr)  { update_word_ranges(); }
	Sentence (Sentence& thestr): StringTemp(thestr) { update_word_ranges(); }
	void steal (Sentence& thestr) { StringTemp::steal(thestr); word_ranges.steal(thestr.word_ranges); }
	void set (CStr thestr) { StringTemp::set(thestr); update_word_ranges(); }
	void operator= (Sentence& thestr) { steal(thestr); }
	void free () { StringTemp::free(); ::free(word_ranges); }
	~Sentence() { free(); }
	uint wordcount() const { return word_ranges.count(); }

	void insert_EOS_after_each_word () {
		for (Index w=0; w<wordcount(); ++w)
			str [ word_ranges[w].high() ]  =  '\0';
	}

	uint word_length (Index wordindex) const {
		return word_ranges[wordindex].high() - word_ranges[wordindex].low(); 
	}


	void get_word (Index wordindex, StringTemp& theword) const {
		if ( wordindex>=wordcount() )  { theword.overwrite("."); return; }
		const RangeOfUints& therange = word_ranges[wordindex];
		theword.put (0,str, therange.low(),therange.high() );
	}

	StringTemp word (Index wordindex) const {
		if ( wordindex>=wordcount() )  return StringTemp(".");
		StringTemp theword (word_length(wordindex)+1);
		get_word (wordindex,theword);
		return theword;
	}


	void get_space_before_word (Index wordindex, StringTemp& thespace) const {
		if ( wordindex>wordcount() ) 
			thespace.overwrite("");
		else if ( wordindex==wordcount() )	// space after last word
			thespace.put(0,str, word_ranges[wordindex-1].high(), len);
		else if ( wordindex==0 )  // space before first word
			thespace.put(0,str, 0, word_ranges[wordindex].low() );
		else
			thespace.put(0,str, word_ranges[wordindex-1].high(), word_ranges[wordindex].low() );
	}

	StringTemp space_before_word (Index wordindex) const {
		StringTemp theword (8);
		get_space_before_word (wordindex,theword);
		return theword;
	}

	void get_space_after_word (Index wordindex, StringTemp& thespace) const { get_space_before_word(wordindex+1,thespace); }
	StringTemp space_after_word (Index wordindex) const { return space_before_word(wordindex+1); }

	CStr at_word (Index wordindex) const {
		if ( wordindex>=wordcount() )
			return ".";
		return str  +  word_ranges[wordindex].low();
	}


	status readline (istream& in, CStr thestoppers=".!?") {          
		for(;;) {     // the reading is stopped by '.' followed by a space (or EOF).
			DOr(StringTemp::readline(in,thestoppers));
			if ( isspace(in.peek()) || in.eof() ) break;
		}
		append(" ."); 
		update_word_ranges(); 
		return OK; 
	}

	void duplicate (SentenceCR from) {
		::duplicate ( (StringTemp&)(*this) , (StringTempCR)from );
		::duplicate ( word_ranges , from.word_ranges );
	}
   
};

inline void duplicate (Sentence& to, SentenceCR from) {
	to.duplicate(from);
}

inline void free (Sentence& s) {
	s.free();
}







#endif

