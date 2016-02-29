/*
 * File cpplib/wordcnt.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/wordcnt.cpp
 */

/* wordcnt.cpp -- word counter */

#include <cpplib/wordcnt.h>


status WordCounter::count_words_in_file  (istream& in, Index from_wordnum, Index to_wordnum, CStr the_stopper) {
	StringTemp cur_word(30);
	for (Index wordnum=0; wordnum<to_wordnum; ++wordnum) {
		skip_comments (in,'%');
		DOEOF (cur_word.readline(in," \t\n"));
		if (compare(cur_word.str,the_stopper)==0) break;

		if (wordnum<from_wordnum)     // skip words until from_wordnum
			continue;

		DOr(insert(cur_word.str));
	}
	return OK;
}


