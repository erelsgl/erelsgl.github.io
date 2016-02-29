/*
 * File morph/fonemtav.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/fonemtav.cpp
 */

/* fonemtav.cpp -- tavniot fonemiot */


#include <cpplib/stringt.h>
#include <morph/tavniot.h>

Tavniot tavniot_txiliot_fonemiot_W;
Tavniot tavniot_txiliot_fonemiot_J;
Tavniot tavniot_txiliot_fonemiot_BKLMH;

uint num_of_txiliot_BKLMH (CStr theword) {
	static StringTemp theword3(30);
	Index i; LOOP_MATCHING_TAVNIOT (i,tavniot_txiliot_fonemiot_BKLMH,theword,theword3)
		return curtavnit.info().txiliot(); }
	return 0;
}

uint num_of_txiliot_J (CStr theword) {
	static StringTemp theword2(30);
	Index i; LOOP_MATCHING_TAVNIOT (i,tavniot_txiliot_fonemiot_J,theword,theword2)
		return curtavnit.info().txiliot() + num_of_txiliot_BKLMH(theword2.str); }
	return num_of_txiliot_BKLMH (theword);
}

uint num_of_txiliot_W (CStr theword) {
	static StringTemp theword1(30);
	Index i; LOOP_MATCHING_TAVNIOT (i,tavniot_txiliot_fonemiot_W,theword,theword1)
		return curtavnit.info().txiliot() + num_of_txiliot_J(theword1.str); }
	return num_of_txiliot_J (theword);
}


uint num_of_txiliot (CStr the_fonemic_word) {	
	if (!charinstring('-',the_fonemic_word)) return 0;
	return num_of_txiliot_W (the_fonemic_word);
}



status read_tavniot_fonemiot (CStr thepath) {
	ifstream tavniot_infile;
	StringTemp thefilename = concat_path_to_filename(thepath,"tavniot.fon");
	DOr(open(thefilename.str,tavniot_infile));
	skip_comments(tavniot_infile,'%');
	DOr(read(tavniot_infile,tavniot_txiliot_fonemiot_W,Format("P T")));
	skip_comments(tavniot_infile,'%');
	DOr(read(tavniot_infile,tavniot_txiliot_fonemiot_J,Format("P T")));
	skip_comments(tavniot_infile,'%');
	DOr(read(tavniot_infile,tavniot_txiliot_fonemiot_BKLMH,Format("P T")));
	tavniot_infile.close();
	return OK;
}


#ifdef TEST_FONEMTAV

void main (void) {
	read_tavniot_fonemiot ("d:\\cpp\\morph");
	cout << num_of_txiliot("wwww") << " ";           //0
	cout << num_of_txiliot("w-www") << " ";          //1
	cout << num_of_txiliot("w-w-www") << " ";          //1
	cout << num_of_txiliot("w-$e-www") << " ";          //2
	cout << num_of_txiliot("$e-w-ww") << " ";          //1
	cout << num_of_txiliot("w$e-www") << " ";          //2
	cout << num_of_txiliot("$ew-ww") << "   ";          //0

	cout << num_of_txiliot("b-wwww") << " ";          //1
	cout << num_of_txiliot("w-b-www") << " ";          //2          
	cout << num_of_txiliot("w-b-w-www") << " ";          //2
	cout << num_of_txiliot("w-b-$e-www") << " ";          //2
	cout << num_of_txiliot("b-w-$e-www") << " ";          //1
	cout << num_of_txiliot("$e-b-w-ww") << " ";          //2
	cout << num_of_txiliot("w-$e-b-www") << " ";          //3
	cout << num_of_txiliot("w$e-b-www") << " ";          //3
	cout << num_of_txiliot("w-$eb-www") << " ";          //3
	cout << num_of_txiliot("w$eb-www") << " ";          //3
	cout << num_of_txiliot("mi-$ew-ww") << " ";          //1
	cout << num_of_txiliot("mi-$e-w-ww") << " ";          //2
}

#endif

