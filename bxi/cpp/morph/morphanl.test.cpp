/*
 * File morph/morphanl.test.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphanl.test.cpp
 */

/* morphanl.test.cpp -- a main program that demonstrates the morphological analyzer. */
/* 	(the morphological analyzer is implemented in morphanl.cpp */


#include <morph/morphanl.h>
#include <morph/lexicon.h>
#include <morph/morphtav.h>
#include <cpplib/stringt.h>
#include <cpplib/cmdline1.h>
#include <cpplib/ui.h>
#include <fstream.h>



int main (int argc, char* argv[]) {
	set_synopsis ("MORPHANL [/f file-to-analyze] [/w word-to-analyze] [/l path-to-lexicon]");
	parse_command_line (argc,argv,0,0,"fwlt","");
	CStr path_to_lexicon = option('l')==NULL? ".": option('l');
	initialize_the_analyzer (path_to_lexicon, path_to_lexicon);

	MorphOptions theoptions;

	/********** single-word test ***************/
	if (option('w')!=NULL) {
		natax (option('w'),theoptions);
		cout << option('w') << " == " << theoptions << endl;
	}

	/********** batch test (read words from a file) ***************/
	else if (option('f')!=NULL) {		
		ifstream input;
		cerr << "opening input file " << option('f') << endl;
		open_infile (option('f'),input);
		StringTemp cur_word (20);
		for (;;) {
			skip_comments (input,'%');
			DOEOFx(cur_word.readline(input," \n"));
			natax(cur_word.str, theoptions);  
			if (theoptions.count()>0)
				cout<<cur_word<<"== "<<theoptions<<endl; 
		}
		input.close();
	}

	/************ interactive analysis *************************/
	else for (;;) {				
		StringTemp thefullword = input_string ("KTWB MILH LNITUX (KTWB '~' KDI LSIIM)", 16);
		if (thefullword[0]=='!') {
			lexicon_is_creative = !lexicon_is_creative;
			cout << "lexicon is " << yesno_str(lexicon_is_creative) << "creative" << endl;
		}
		if (thefullword[0]=='~') break;
		natax (thefullword.str, theoptions);
		for (Index i=0; i<theoptions.count(); ++i) {
			theoptions[i].ktov_barur (cout,thefullword.str);
			cout << " (" << theoptions[i] << ") " << endl;
		}
	}

	return 0;
}
