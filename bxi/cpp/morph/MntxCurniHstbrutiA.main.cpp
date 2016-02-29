/*
 * File morph/MntxCurniHstbrutiA.main.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/MntxCurniHstbrutiA.main.cpp
 */

/* MntxCurniHstbrutiA.main.cpp */

/* a program to demonstrate context-dependent morphological analysis in Hebrew */

/* Author: Erel Segal */
/* Email:  erelsgl@cs.technion.ac.il */
/* Web site:  http://www.cs.technion.ac.il/~erelsgl */

/* License: The program and all accompanying files are available under the GPL */
/* (GNU Public License). */
/* The program can be downloaded via the author's web page. */


#include <morph/MntxCurniHstbrutiA.h>
#include <math.h>

#include <morph/lexicon.h>
#include <morph/addlex.h>
#include <morph/morphanl.h>

#include <morph/alghist.h>
#include <morph/tiqun3.h>
//#include <morph/hajlamot.h>

#include <cpplib/argsfile.h>
#include <cpplib/ui.h>
#include <morph/morphui.h>
#include <morph/corpus.h>
#include <cpplib/sentence.h>

#include <cpplib/wordcnt.h>
#include <cpplib/map.hxx>
#include <cpplib/hashtab.hxx>
#include <cpplib/countc.hxx>
#include <morph/hpmaio.h>
#include <morph/hpmamain.h>
#include <morph/hpmainit.h>


/***************************************************************************/
/***********        The Translation Routine                         ********/
/***************************************************************************/

/* Input: a Hebrew string, words seperated by spaces. */
/* Output: a string of Hebrew lexical values, seperated by spaces, in the same order. */
CStr string_to_lexical_values(CStr str) {
	CorpusImTiqunim theCorpus;
	theCorpus.atxl(strlen(str) >> 2);
	theCorpus.qra_mxrozt(str);

	//cerr 
	//	<< "*** $LB 1: $LB HMILA"  << endl
	//	<< "***		QovcQltTocaotJlvMila=" << (qorpus_input_data_from_previous_analysis? qorpus_input_data_from_previous_analysis: "<none>") << endl
	//	<< "***		XlqImunJlvMila=[" << int_argument("TxiltXlqImunJlvMila",0) << "..." << int_argument("SofXlqImunJlvMila",0) << "]" << endl;
	theCorpus.ntx_qlt_1();
	theCorpus.ntx_qlt_2();

	if (string_argument("QovcTiqunim",NULL)) {
	//	cerr 
	//		<< "*** $LB 2: $LB HZUG"  << endl
	//		<< "***		QovcTiqunim=" << string_argument("QovcTiqunim") << endl
	//		<< "***		MsprTiqunim=" << mspr_tiqunim() << endl;
		theCorpus.taqen_nituxim();
	}

	return theCorpus.arkim_milonyim_mhanitux_jelanu();
}


/***************************************************************************/
/***********        The Main Program                                ********/
/***************************************************************************/


int main (int argc, char** argv) {
	ofstream out; open_outfile(argv[2], out);
	ifstream in; open_infile(argv[1], in);
	char* argv1[] = {"mcha.exe", "nitux_mjptim.mca"};
	process_arguments_file(2, argv1);
	initialize_the_analyzer(lex_path,lex_path);

	open_logfile(7); 
	open_lexlogfile(log_path);

	atxl_jlv_mila();
	atxl_jlv_zug();

	/* MAIN PROGRAM */

	//out << string_to_lexical_values("מספר אנשים מגיעים לתאילנד בחופש הגדול") << endl;
	//out << string_to_lexical_values("בג\"ץ אסר על סגירת הטלוויזיה החינוכית") << endl;
	//out << string_to_lexical_values("הערב הוצא צו ביניים האוסר על שרת החינוך לפטר עובדים , לקצץ תקציבים או לבטל שעות שידור ; העותרים טוענים כי כל שינוי בטלוויזיה צריך להיות מעוגן בחקיקה") << endl;
	//out << string_to_lexical_values("אש לעבר ההתנחלות כדים בגזרת ג'נין – אין נפגעים") << endl;
	//out << string_to_lexical_values("פלסטינים ירו הערב אל ההתנחלות כדים מזרחית לג'נין – לא היו נפגעים ולא נגרם נזק . בגלי צה\"ל דווח כי החיילים במקום השיבו אש לעבר מקורות הירי .") << endl;
	//out << string_to_lexical_values("הרמטכ\"ל אמר: \"יש להילחם בטרור!\".") << endl; 

	while (!in.eof()) 
	{
		char line[500];
		in.getline(line, 499);
		//in >> line;
		out << string_to_lexical_values(line) << endl;
	}

	out.close();
	in.close();

	logfile.close();  
	close_lexlogfile();

	return 0;
}
