/*
 * File morph/convlang.test.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/convlang.test.cpp
 */

/* convlang.test.cpp */
#include <ctype.h>

****************************************************************************************************/
/********                     CONVLANG SELF TEST                                           **********/
/****************************************************************************************************/


#include <morph/convlang.h>
#include <cpplib/cmdline1.h>
#include <cpplib/convert1.h>


class EngToHebConverter: public Converter {
	status convert_line(ostream& target) { 
		//if (!contains(hebrew_english_letters,myline.at_end(0)))
		//	myline.append('A');
		eng2heb(myline);
		target<<myline; 
		return OK; 
	}
};

void main (int argc, char* argv[]) {
	cout << endl << "CONVLANG 1.0" << endl;
	set_synopsis ("CONVLANG source target");
	parse_command_line (argc,argv, 2,2, "","");

	open_infile (variable(0),global_source);
	open_outfile(variable(1),global_target,0);
	EngToHebConverter theconverter;
	DOx(theconverter.convert());
}
));
}
