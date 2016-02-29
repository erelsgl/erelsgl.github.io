/*
 * File morph/morphui.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphui.cpp
 */

/* morphui.cpp */

#include <iomanip.h>
#include <iostream.h>
#include <morph/morphui.h>
#include <morph/morphopt.h>
#include <cpplib/ui.h>

HeleqDiber input_hd(CStr prompt) {
	for (int i=-8, j=0; i<=7; ++i) {
		HeleqDiber hd = HeleqDiber(i);
		if (hd2char(hd) == '*') continue;
		cout << setiosflags(ios::right) << setw(2) << (i+8) << ". " 
			 << setiosflags(ios::left) << setw(15) << hd2str(hd);
		++j;
		if (j%4==0)  cout<<endl;
	}
	cout << endl;
	return HeleqDiber ( -8 + input_int (prompt,0,15) );
}


Meen input_meen (CStr prompt) { 
	return char2meen ( prompt_from (prompt,"ZNB*") ); }

Mispar input_mispar (CStr prompt) { 
	return char2mispar ( prompt_from (prompt,"YRB*") ); }

Smikut input_smikut (CStr prompt) { 
	return char2smikut ( prompt_from (prompt,"FSB*") ); }

Guf input_guf (CStr prompt) { 
	return char2guf ( prompt_from (prompt,"123A*") ); }

Zman input_zman (CStr prompt) { 
	return char2zman ( prompt_from (prompt,"VHTCMS*") ); }



/* noten la-mi$tamme$ liktob nittux bsisi (bli &erk milloni) */
void input_morphinfo_basic (MorphInfoBasic& theinfo) {
	theinfo.setheleqdiber ( input_hd("XLQ DIBR") );
	switch (theinfo.heleqdiber()) {
		case ECEM: case MISPAR: 
			theinfo.setecem ( input_meen("  MIN"), input_mispar("  MSPR"), input_smikut("  SMIKWT") );   cout<<endl;
			theinfo.setsiomet ( input_meen("  MIN-SIWMT"), input_mispar("  MSPR-SIWMT"), input_guf("  GWP-SIWMT") );   
			break;
		case TOAR:
			theinfo.setecem ( input_meen("  MIN"), input_mispar("  MSPR"), input_smikut("  SMIKWT") );   cout<<endl;
			break;
		case POAL: case POAL_EZER:
			theinfo.setpoal ( input_meen("  MIN"), input_mispar("  MSPR"), input_guf("  GWP"), input_zman("  ZMN")  );
			break;
		case JEM_PRATI:
			theinfo.setmeen (input_meen("  MIN"));
			break;
		case MILAT_GUF:
			theinfo.setpoal ( input_meen("  MIN"), input_mispar("  MSPR"), input_guf("  GWP"), NO_ZMAN );
			break;
		case MILAT_YAXAS:
			theinfo.setsiomet ( input_meen("  MIN-SIWMT"), input_mispar("  MSPR-SIWMT"), input_guf("  GWP-SIWMT") );
			break;
		default: ;
	}
	cout<<endl;
}


/* noten la-mi$tamme$ libxor ^et ha-pirruq ha-nakon $ell "hamila" l- txilliot+bsis */
Index select_txiliot_option (MorphOptionsCR theoptions, CStr hamila) {
	LOOPVECTOR(;,theoptions,i)  {
		cout << "      " << i << ". ";
		if (theoptions[i].txiliot() > 0) {
			for (Index j=0; j < theoptions[i].txiliot(); ++j)
				cout << hamila[j];
			cout << "-" << theoptions[i].baseword() << endl;
		}
		else 
			cout << hamila << endl;
	}
	return Index (input_int("       MHW HNITUX HNKWN?",0,theoptions.count()-1));
}



/* noten la-mi$tamme$ liktob ^et ha-nittux $ell "hamila" */
void input_morphinfo (CStr hamila, MorphInfo& theinfo) {
	extern MorphOptions natax_txiliot(CStr);
	MorphOptions the_txiliot_options = natax_txiliot(hamila);
	MorphInfo& selected_option = 
		the_txiliot_options.count()>1? 
			the_txiliot_options [ select_txiliot_option (the_txiliot_options,hamila) ]:
			the_txiliot_options [0];
	CStr hamila_bli_txiliot = selected_option.baseword();
	uint the_num_of_txiliot = strlen(hamila) - strlen(hamila_bli_txiliot);

	theinfo.settxiliot (selected_option);
	input_morphinfo_basic (theinfo);
	HeleqDiber hd=theinfo.heleqdiber();
	if (hd==ECEM || hd==POAL || hd==POAL_EZER || hd==TOAR || hd==MILAT_GUF || hd==MILAT_YAXAS) {
		StringTemp thebaseword = input_string ("MHW H&RK HMILWNI?",20,hamila_bli_txiliot);
		theinfo.setbaseword (thebaseword.str);
	}
	else
		theinfo.setbaseword (hamila_bli_txiliot);
}	



#ifdef TEST_MORPHUI

void main(void) {
}

#endif

