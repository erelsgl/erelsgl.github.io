/* morphtav -- tavniot morfologiot */


#include <morph/morphtav.h>
#include <cpplib/trie0.hxx>

Tavniot tavniot_jayakut_ecem;
Tavniot tavniot_ribui_ecem;
Tavniot tavniot_toar;
Tavniot tavniot_poal_avar;
Tavniot tavniot_beinony;
Tavniot txiliot_poal_atid[NUM_OF_BINYANIM];
Tavniot hemjeky_poal_atid_ZY[NUM_OF_BINYANIM];  // ZAKAR YAXID
Tavniot hemjeky_poal_atid_NY[NUM_OF_BINYANIM];  // NQEVA YAXID
Tavniot hemjeky_poal_atid_ZR[NUM_OF_BINYANIM];  // ZAKAR rabim
Tavniot hemjeky_poal_atid_NR[NUM_OF_BINYANIM];  // NQEVA rabim

//Tavnit MIUXD_maqor_gizrat_l_h    ("?WT","?H",POAL,0);
Tavnit MIUXD_maqor_gizrat_p_i    ("?T","I?",POAL,16);
Tavnit MIUXD_ciwui_binyan_hifiil_3 ("abc","HabIc",POAL,61);
Tavnit MIUXD_ciwui_binyan_hifiil_2 ("bc","HbIc",POAL,61);


/****************** matching ***********************************/


bool ends_with (StringTempCR theword,char c) {
	return theword.at_end(0)==c;
}

bool ends_with (TavnitCR thetavnit, char c) {
	CStr theword = thetavnit.base();    Size thelen=thetavnit.baselen();
	return theword[thelen-1]==c;
}




bool btavnit_hifiil (StringTempCR theword) {
	return	(theword.len==5 && theword[0]=='H' && theword[3]=='I' && theword[2]!='I' && theword[4]!='I') || 
			(theword.len==4 && theword[0]=='H' && theword[2]=='I' && theword[1]!='I' && theword[3]!='I');
}

bool btavnit_hifiil (TavnitCR thetavnit) {
	CStr theword = thetavnit.base();   Size thelen=thetavnit.baselen();
	return	(thelen==5 && theword[0]=='H' && theword[3]=='I' && theword[2]!='I' && theword[4]!='I') || 
			(thelen==4 && theword[0]=='H' && theword[2]=='I' && theword[1]!='I' && theword[3]!='I');
}





/****************** IO ***********************************/

status read_tavniot_jayakut (istream& tavniot_infile) {
	DOr(read(tavniot_infile,tavniot_jayakut_ecem,Format("P T")));	
	return OK;

}


status read_the_tavniot (CStr thepath) {
	ifstream tavniot_infile;
	StringTemp thefilename = concat_path_to_filename(thepath,"milon/tavniot.ma");
	cerr << "reading tavniot file " << thefilename << endl;
	DOr(open(thefilename.str,tavniot_infile));
	skip_comments(tavniot_infile,'%');
	cerr << "\treading tavniot-jayakut:" << endl;
	DOr(read_tavniot_jayakut(tavniot_infile));
	skip_comments(tavniot_infile,'%');
	cerr << "\treading tavniot-ribui-ecem:" << endl;
	DOr(read(tavniot_infile,tavniot_ribui_ecem,Format("P T")));
	skip_comments(tavniot_infile,'%');
	cerr << "\treading tavniot-toar:" << endl;
	DOr(read(tavniot_infile,tavniot_toar,Format("P T")));
	skip_comments(tavniot_infile,'%');
	cerr << "\treading tavniot-poal-avar:" << endl;
	DOr(read(tavniot_infile,tavniot_poal_avar,Format("P T")));
	skip_comments(tavniot_infile,'%');
	cerr << "\treading tavniot-beinoni:" << endl;
	DOr(read (tavniot_infile,tavniot_beinony,Format("P T")));
	cerr << "\treading tavniot-poal-atid:" << endl;
	for (Index i=0; i<NUM_OF_BINYANIM; ++i) {
		skip_comments(tavniot_infile,'%');
		DOr(read (tavniot_infile,txiliot_poal_atid[i],Format("P T")));
		skip_comments(tavniot_infile,'%');
		DOr(read (tavniot_infile,hemjeky_poal_atid_ZY[i],Format("P T")));
		skip_comments(tavniot_infile,'%');
		DOr(read (tavniot_infile,hemjeky_poal_atid_NY[i],Format("P T")));
		skip_comments(tavniot_infile,'%');
		DOr(read (tavniot_infile,hemjeky_poal_atid_ZR[i],Format("P T")));
		skip_comments(tavniot_infile,'%');
		DOr(read (tavniot_infile,hemjeky_poal_atid_NR[i],Format("P T")));
	}
	tavniot_infile.close();
	return OK;
}

void write_the_tavniot (CStr thepath) {
	ofstream out;
	StringTemp thefilename = concat_path_to_filename(thepath,"milon/tavniot.ma");
	DO(open(thefilename.str,out,ios::trunc));
	out<<endl; writeln(out,tavniot_jayakut_ecem,Format("P\nT"));
	out<<endl; writeln(out,tavniot_ribui_ecem,Format("P\nT"));
	out<<endl; writeln(out,tavniot_toar,Format("P\nT"));
	out<<endl; writeln(out,tavniot_poal_avar,Format("P\nT"));
	out<<endl; writeln(out,tavniot_beinony,Format("P\nT"));
	for (Index i=0; i<NUM_OF_BINYANIM; ++i) {
		out<<endl; writeln(out,txiliot_poal_atid[i],Format("P\nT"));
		out<<endl; writeln(out,hemjeky_poal_atid_ZY[i],Format("P\nT"));
		out<<endl; writeln(out,hemjeky_poal_atid_NY[i],Format("P\nT"));
		out<<endl; writeln(out,hemjeky_poal_atid_ZR[i],Format("P\nT"));
		out<<endl; writeln(out,hemjeky_poal_atid_NR[i],Format("P\nT"));
	}
	out.close();
}




