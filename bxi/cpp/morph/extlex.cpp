/*
 * File morph/extlex.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/extlex.cpp
 */

/* extlex.cpp -- lexicon extension utility */

#include <morph/addlex.h>
#include <morph/morphopt.h>
#include <morph/morphtav.h>
#include <morph/lexicon.h>
#include <morph/morphanl.h>

#include <cpplib/stringt.h>
#include <cpplib/cmdline1.h>

void extend_the_lexicon(istream& in) {
	MorphOptions cur_options;
	StringTemp cur_fullword (50);
	for(;;) {
		skip(in,"\n\t: ");
		DOEOFx(cur_fullword.readline (in,":"));
		if (cur_fullword.len==0) continue;   // empty word.
		skip(in," \t");
		DOx(read (in, cur_options, Format("L")));
		for (Index i=0; i<cur_options.count(); ++i) {
			MorphInfo curoption = cur_options[i];
			cout << " " << curoption;
			extend_lexicon (cur_fullword.str, curoption);
			MorphOptions new_options = natax (cur_fullword.str);
			if ( !contains(new_options,curoption) ) {
				cout << "[m]";              // miuxd
				CStr curfullword = curoption.fullword_bli_txiliot(cur_fullword.str);
				curoption.settxiliot (false,NO_JIABUD,NO_OY,LOMEYUDA);
				add_miuxd_to_lexicon (curfullword,curoption);
			}
		}
	}
}


void main (int argc, char* argv[]) {
	set_synopsis ("EXTLEX analysis.NTX path-to-current path-to-new [/t path-to-tavniot]");
	parse_command_line (argc,argv,3,3,"t","");
	initialize_the_analyzer (option('t')==NULL? "e:\\erel\\morph": option('t'),
							 variable(1) );
	ifstream in;
	open_infile(variable(0),in);
	extend_the_lexicon(in);  cout<<endl;
	write_the_lexicon (variable(2));
}

