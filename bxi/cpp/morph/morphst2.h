/* morphst.h -- morphological options trie with sikuiim */

#ifndef __MORPHST_H
#define __MORPHST_H

#include <morph/morphsik.h>
#include <cpplib/trie0.hxx>


/*        ***** USAGE EXAMPLE *****
#include <morph/morphanl.h>       
...

	MorphOptionsWithSikuiTrie yourtrie;
//initializing the trie, by analyzing each word in a file and assigning equal probability to each analysis:
	ifstream your_input_stream ("words.txt");
	initialize_the_analyzer (path_to_tavniot,path_to_lexicon);
	yourtrie.analyze_words_in_file (your_input_stream);

//saving the trie in a file:
	ofstream your_output_stream("file.txt");
	write (your_output_stream, yourtrie );
	yout_output_stream.close();
//restoring the trie from a file:
	ifstream your_input_stream("file.txt");
	read (your_input_stream, yourtrie );
	yout_input_stream.close();
*/



/* class MorphOptionsTrie is used to hold all the possible morphological options for *
 * all words.																   */
class MorphOptionsWithSikuiTrie:  public Trie0<MorphOptionsWithSikui>  {
public:
	status analyze_words_in_file (istream& in, Index from_wordnum=0, Index to_wordnum=0x7ffffff, CStr the_word_that_will_stop_the_analysis="\0x26");
		// analyze each word, assign equal probability to each option, and insert it to the trie
	status read_analyses_from_file (istream& in);
		// read the analyses of each word, assign equal probability to each option, and insert it to the trie
	MorphOptionsWithSikui analysis (CStr hamila);
		// tries to find the word-analysis in the trie, and if not found -- analyzes it and inserts it to the trie.
	void set_sikui_axid ();
		// set_sikui_axid for all the words
	status insert (CStr hamila, MorphOptionsWithSikui theoptions);
	status insert (CStr hamila, Index thecorrectindex);
	status insert (CStr hamila, MorphInfoCR hanitux);
	status add (CStr hamila, MorphOptionsWithSikui thecorrectoptions);
	Sikui sikui (CStr hamila, Index theindex) const;
};

typedef const MorphOptionsWithSikuiTrie&  MorphOptionsWithSikuiTrieCR;


// Format("P") puts parenthesis arround the trie. Format(" ") omits the parenthesis.
void write (ostream& out, MorphOptionsWithSikuiTrieCR theinfo, Format format="P");
inline status read  (istream& in, MorphOptionsWithSikuiTrie& theinfo) { return read (in,theinfo,Format("T  ")); }


// koll ha-funqciot $e-mod_dot merxaq manixot $e-ha-mibnim mekilim bdiuq ^otan millim,
//	w-ha-hebdelim beineihem hem raqq ba-sikkuyim.
double merxaq_hamming (MorphOptionsWithSikuiTrieCR a, MorphOptionsWithSikuiTrieCR b);
double merxaq_euclid (MorphOptionsWithSikuiTrieCR a, MorphOptionsWithSikuiTrieCR b);
double merxaq_maximum (MorphOptionsWithSikuiTrieCR a, MorphOptionsWithSikuiTrieCR b);
double merxaq_maximum_mjuqlal (MorphOptionsWithSikuiTrieCR a, MorphOptionsWithSikuiTrieCR b);
double merxaq_hamming__measured_to_reference (MorphOptionsWithSikuiTrieCR measured, MorphOptionsWithSikuiTrieCR reference);
double merxaq_euclid__measured_to_reference (MorphOptionsWithSikuiTrieCR measured, MorphOptionsWithSikuiTrieCR reference);
double merxaq_maximum__measured_to_reference (MorphOptionsWithSikuiTrieCR measured, MorphOptionsWithSikuiTrieCR reference);
double merxaq_maximum_mjuqlal__measured_to_reference (MorphOptionsWithSikuiTrieCR measured, MorphOptionsWithSikuiTrieCR reference);

#endif
