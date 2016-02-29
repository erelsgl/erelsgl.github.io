/*
 * File morph/lexicon.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/lexicon.cpp
 */

/* lexicon.cpp */

#include <morph/lexicon.h>
#include <morph/lexinf.h>
#include <morph/morphopt.h>

bool lexicon_is_creative = false;


/********************************************************************/
/*************     The lexicon structures          ******************/
/********************************************************************/

#include <cpplib/trie0.hxx>

Trie0<LexicalInfoECEM> lexicon_ecem[2];
Trie0<LexicalInfoPOAL> lexicon_poal[2];
Trie0<LexicalInfoTOAR> lexicon_toar[1];
Trie0<LexicalInfoMILIT> lexicon_milit[1];        // includes: milat-xibur, toar-poal, milit. 
Trie0<LexicalInfoJEMPRATI> lexicon_jem_prati[1];

Trie0<MorphInfo>	lexicon_milot_yaxas, lexicon_milot_guf, 
					lexicon_misparim, lexicon_pealei_ezer;

Trie0<MorphInfo>	lexicon_acmim_miuxdim, lexicon_pealim_miuxdim_avar, 
					lexicon_pealim_miuxdim_howe, lexicon_pealim_miuxdim_atid, 
					lexicon_tearim_miuxdim;

Trie0<MorphOptions>	lexicon_yocei_dofen;



/********************************************************************/
/*						queries										*/
/********************************************************************/

#define GET_ITEM(curinfop,lexicon,theword) { curinfop=lexicon.itemp(theword);   if(curinfop==NULL || *curinfop==0) return false; }
#define GET_PTR(pointer,info) { if(pointer!=NULL)  *pointer = info; }


bool lexicon_contains_ecem (CStr theword, uint* thesugribui, Meen* themeen) {
	LexicalInfoECEM* curinfop; GET_ITEM(curinfop,lexicon_ecem[0],theword); 
	GET_PTR(thesugribui,curinfop->sug_ribui);
	GET_PTR(themeen,curinfop->meen);
	return true;
}	

bool lexicon_contains_ecem (CStr theword, uint sug_ribui, Meen* themeen) {
	if (sug_ribui==0) return lexicon_contains_ecem (theword,(uint*)NULL,themeen);
	LexicalInfoECEM* curinfop;			 GET_ITEM(curinfop,lexicon_ecem[0],theword); 
	if (curinfop->sug_ribui!=sug_ribui)  GET_ITEM(curinfop,lexicon_ecem[1],theword);
	if (curinfop->sug_ribui!=sug_ribui)  return false; 
	GET_PTR(themeen,curinfop->meen);
	//else if (curinfop->sug1==0 && lexicon_is_creative)		return true;
	return true;
}



bool lexicon_contains_poal (CStr theword, uint* thesughowe, uint* thesugatid) {
	LexicalInfoPOAL* curinfop; GET_ITEM(curinfop,lexicon_poal[0],theword);
	GET_PTR(thesughowe,curinfop->sug_howe);
	GET_PTR(thesugatid,curinfop->sug_atid);
	return true;
}

bool lexicon_contains_poal_1 (CStr theword, uint sug_howe) {
	if (sug_howe==0) return lexicon_contains_poal (theword,NULL,NULL);
	LexicalInfoPOAL* curinfop;			 GET_ITEM(curinfop,lexicon_poal[0],theword); 
	if (curinfop->sug_howe!=sug_howe)    GET_ITEM(curinfop,lexicon_poal[1],theword);
	if (curinfop->sug_howe!=sug_howe)  return false; 
	//else if (curinfop->sug1==0 && lexicon_is_creative)		return true;
	return true;
}

bool lexicon_contains_poal_2 (CStr theword, uint sug_atid) {
	if (sug_atid==0) return lexicon_contains_poal (theword,NULL,NULL);
	LexicalInfoPOAL* curinfop;			 GET_ITEM(curinfop,lexicon_poal[0],theword); 
	if (curinfop->sug_atid!=sug_atid)    GET_ITEM(curinfop,lexicon_poal[1],theword);
	if (curinfop->sug_atid!=sug_atid)  return false; 
	//else if (curinfop->sug1==0 && lexicon_is_creative)		return true;
	return true;
}

bool lexicon_contains_jem_prati (CStr theword, Meen* themeen) {
	LexicalInfoJEMPRATI* curinfop; GET_ITEM(curinfop,lexicon_jem_prati[0],theword); 
	GET_PTR(themeen,curinfop->meen);
	return true;
}

bool lexicon_contains_toar (CStr theword, uint sug_toar) {
	LexicalInfoTOAR* curinfop;			 GET_ITEM(curinfop,lexicon_toar[0],theword); 
	if (sug_toar!=0 && curinfop->sug_toar!=sug_toar)  return false; 
	return true;
}


bool lexicon_contains_milit (CStr theword, HeleqDiber* thehd) {
	LexicalInfoMILIT* curinfop; GET_ITEM(curinfop,lexicon_milit[0],theword); 
	GET_PTR(thehd,curinfop->hd);
	return true;
}	

bool lexicon_contains_milit (CStr theword, HeleqDiber hd) {
	LexicalInfoMILIT* curinfop;			 GET_ITEM(curinfop,lexicon_milit[0],theword); 
	if (hd!=NO_HD && curinfop->hd!=hd)  return false; 
	return true;
}


bool lexicon_contains_milat_yaxas (CStr theword, MorphInfo& theinfo) {
	return lexicon_milot_yaxas.contains(theword,theinfo); }

bool lexicon_contains_milat_guf (CStr theword, MorphInfo& theinfo) {
	return lexicon_milot_guf.contains(theword,theinfo); }

bool lexicon_contains_mispar (CStr theword, MorphInfo& theinfo) {
	return lexicon_misparim.contains(theword,theinfo); }

bool lexicon_contains_poal_ezer (CStr theword, MorphInfo& theinfo) {
	return lexicon_pealei_ezer.contains(theword,theinfo); }

bool lexicon_contains_poal_miuxd (CStr theword, Zman thezman, MorphInfo& theinfo) {
	return	thezman==AVAR? lexicon_pealim_miuxdim_avar.contains(theword,theinfo): 
			(thezman==HOWE||thezman==HOWE_SAVIL)? lexicon_pealim_miuxdim_howe.contains(theword,theinfo): 
			(thezman==ATID||thezman==CIWUI||thezman==MAQOR)? lexicon_pealim_miuxdim_atid.contains(theword,theinfo):
			false;
}

bool lexicon_contains_toar_miuxd (CStr theword, MorphInfo& theinfo) {
	return	lexicon_tearim_miuxdim.contains(theword,theinfo); }

bool lexicon_contains_ecem_miuxd (CStr theword, MorphInfo& theinfo) {
	return	lexicon_acmim_miuxdim.contains(theword,theinfo); }


bool lexicon_contains_yoce_dofen (CStr theword, MorphOptions& theoptions) {
	MorphOptions* p = lexicon_yocei_dofen.itemp(theword);
	if (p==NULL) return false;
	if (p->isempty()) return false;
	theoptions.share(*p);
	return true;
}


#undef GET_ITEM
#undef GET_PTR

/********************************************************************/
/*						EDITING										*/
/********************************************************************/

#define GET_ITEM(curinfop,lexicon,theword,theinfo) { curinfop=lexicon.itemp(theword);   if(curinfop==NULL) return lexicon.insert(theword,theinfo);  }

status add_ecem_to_lexicon (CStr theword, uint sug_ribui, Meen meen) { 
	LexicalInfoECEM theinfo (sug_ribui,meen);
	LexicalInfoECEM* curinfop;	GET_ITEM(curinfop,lexicon_ecem[0],theword,theinfo);
	if (*curinfop!=theinfo)	    GET_ITEM(curinfop,lexicon_ecem[1],theword,theinfo);
	if (*curinfop!=theinfo)	    return EEXIST;  // already exists!
	if (curinfop->sug_ribui==0) curinfop->sug_ribui=sug_ribui;
	if (curinfop->meen==NONE) curinfop->meen=meen;
	return OK;
}

status add_poal_to_lexicon (CStr theword) { 
	LexicalInfoPOAL theinfo (0,0);
	LexicalInfoPOAL* curinfop;	GET_ITEM(curinfop,lexicon_poal[0],theword,theinfo);
	return OK;
}

status add_poal_1_to_lexicon (CStr theword, uint sug_howe) { 
	LexicalInfoPOAL theinfo (sug_howe,0);
	LexicalInfoPOAL* curinfop;				GET_ITEM(curinfop,lexicon_poal[0],theword,theinfo);
	if (dif(curinfop->sug_howe,sug_howe))	GET_ITEM(curinfop,lexicon_poal[1],theword,theinfo);
	if (dif(curinfop->sug_howe,sug_howe))	return EEXIST;  // already exists!
	if (curinfop->sug_howe==0) curinfop->sug_howe=sug_howe;
	return OK;
}

status add_poal_2_to_lexicon (CStr theword, uint sug_atid) { 
	LexicalInfoPOAL theinfo (0,sug_atid);
	LexicalInfoPOAL* curinfop;				GET_ITEM(curinfop,lexicon_poal[0],theword,theinfo);
	if (dif(curinfop->sug_atid,sug_atid))	GET_ITEM(curinfop,lexicon_poal[1],theword,theinfo);
	if (dif(curinfop->sug_atid,sug_atid))	return EEXIST;  // already exists!
	if (curinfop->sug_atid==0) curinfop->sug_atid=sug_atid;
	return OK;
}

status add_poal_to_lexicon (CStr theword, uint sug_howe, uint sug_atid) { 
	LexicalInfoPOAL theinfo (sug_howe,sug_atid);
	LexicalInfoPOAL* curinfop;				GET_ITEM(curinfop,lexicon_poal[0],theword,theinfo);
	if (*curinfop!=theinfo)					GET_ITEM(curinfop,lexicon_poal[1],theword,theinfo);
	if (*curinfop!=theinfo)	return EEXIST;  // already exists!
	if (curinfop->sug_howe==0) curinfop->sug_howe=sug_howe;
	if (curinfop->sug_atid==0) curinfop->sug_atid=sug_atid;
	return OK;
}


status add_toar_to_lexicon (CStr theword, uint sug_toar) {
	LexicalInfoTOAR theinfo (sug_toar);
	LexicalInfoTOAR* curinfop;	GET_ITEM(curinfop,lexicon_toar[0],theword,theinfo);
	if (*curinfop!=theinfo)	return EEXIST;
	if (curinfop->sug_toar==0) curinfop->sug_toar=sug_toar;
	return OK;
}

status add_jem_prati_to_lexicon (CStr theword, Meen meen) {
	LexicalInfoJEMPRATI theinfo (meen);
	LexicalInfoJEMPRATI* curinfop;	GET_ITEM(curinfop,lexicon_jem_prati[0],theword,theinfo);
	if (*curinfop!=theinfo)	return EEXIST;
	if (curinfop->meen==NONE) curinfop->meen=meen;
	return OK;
}

status add_milit_to_lexicon (CStr theword, HeleqDiber hd) {
	LexicalInfoMILIT theinfo (hd);
	LexicalInfoMILIT* curinfop;	GET_ITEM(curinfop,lexicon_milit[0],theword,theinfo);
	if (*curinfop!=theinfo)	return EEXIST;
	if (curinfop->hd==NO_HD) curinfop->hd=hd;
	return OK;
}

status add_milit_to_lexicon (CStr theword, MorphInfoCR theinfo) {
	return	theinfo.hu(MILAT_YAXAS)?	lexicon_milot_yaxas.insert(theword,theinfo):
			theinfo.hu(MILAT_GUF)?	lexicon_milot_guf.insert(theword,theinfo):
			theinfo.hu(MISPAR)?		lexicon_misparim.insert(theword,theinfo):
			theinfo.hu(POAL_EZER)?	lexicon_pealei_ezer.insert(theword,theinfo):
			add_milit_to_lexicon (theword,theinfo.heleqdiber());
}


status add_miuxd_to_lexicon (CStr theword, MorphInfoCR theinfo) {
	HeleqDiber thehd = theinfo.heleqdiber();
	if (thehd==ECEM) return lexicon_acmim_miuxdim.insert(theword,theinfo);
	else if (thehd==TOAR) return lexicon_tearim_miuxdim.insert(theword,theinfo);
	else {
		Zman thezman = theinfo.zman();
		return	thezman==AVAR? lexicon_pealim_miuxdim_avar.insert(theword,theinfo): 
				(thezman==HOWE||thezman==HOWE_SAVIL)? lexicon_pealim_miuxdim_howe.insert(theword,theinfo): 
				(thezman==ATID||thezman==CIWUI||thezman==MAQOR)? lexicon_pealim_miuxdim_atid.insert(theword,theinfo):
				OK;
	}
}

status add_yoce_dofen (CStr theword, MorphOptions theoptions) 
{
	return lexicon_yocei_dofen.insert(theword,theoptions);
}



/********************************************************************/
/*						yocei-dofen									*/
/********************************************************************/

Trie0<bool> acamim_im_ntiat_jayakut_miuxedet;
Trie0<bool> pealim_im_ntiat_maqor_miuxedet;
Trie0<bool> tearim_im_ntia_miuxedet;

bool laecem_yej_ntiat_jayakut_miuxedet (CStr the_ecem) {
	return acamim_im_ntiat_jayakut_miuxedet.item(the_ecem); }
bool lapoal_yej_ntiat_maqor_miuxedet (CStr the_poal) {
	return pealim_im_ntiat_maqor_miuxedet.item(the_poal); }
bool latoar_yej_ntia_miuxedet (CStr the_toar) {
	return tearim_im_ntia_miuxedet.item(the_toar); }

void atxel_acamim_im_ntiat_jayakut_miuexedet () {
	for (Trie0<MorphInfo>::Cursor c(lexicon_acmim_miuxdim); c.isvalid(); ++c) {
		MorphInfoCR theinfo = c.data();
		if ( theinfo.mispar()==YAXID )
			acamim_im_ntiat_jayakut_miuxedet.insert (theinfo.baseword(),true);
	}
	acamim_im_ntiat_jayakut_miuxedet.insert ("$NIM",true);  // miqre miuxad.
}

void atxel_pealim_im_ntiat_maqor_miuexedet () {
	for (Trie0<MorphInfo>::Cursor c(lexicon_pealim_miuxdim_atid); c.isvalid(); ++c) {
		MorphInfoCR theinfo = c.data();
		if ( theinfo.zman()==MAQOR )
			pealim_im_ntiat_maqor_miuxedet.insert (theinfo.baseword(),true);
	}
}


/********************************************************************/
/*						IO - NEW									*/
/********************************************************************/

#define write_lexicon_1(out,lexicon) writeln (out,lexicon[0],Format("T:L "));

#define write_lexicon_2(out,lexicon) {\
	out << '{' << endl;\
	write (out,lexicon[0],Format("t "));\
	write (out,lexicon[1],Format("t "));\
	out << '}' << endl;\
}




status read_lexicon_ecem (istream& in) {
	StringTemp curword (30);
	DOr(testchar(in,'{'));
	for (Index i=0;;++i) {
		LexicalInfoECEM curdata;        
		if (testchar(in,'}')==OK) { return OK; }
		in>>ws;
		DOr (curword.readline(in," ")); 
		DOr ( read (in, curdata ) );
		if (OK!=add_ecem_to_lexicon (curword.str,curdata.sug_ribui,curdata.meen)) {
			int i=0;
		}
	}
	DOr(testchar(in,'}'));
	return OK;
}

status read_lexicon_poal (istream& in) {
	StringTemp curword (30);
	DOr(testchar(in,'{'));
	for (Index i=0;;++i) {
		LexicalInfoPOAL curdata;        
		if (testchar(in,'}')==OK) { return OK; }
		in>>ws;
		DOr (curword.readline(in," ")); 
		DOr ( read (in, curdata ) );
		if (OK!=add_poal_to_lexicon (curword.str,curdata.sug_howe,curdata.sug_atid)) {
			int i=0;
		}
	}
	DOr(testchar(in,'}'));
	return OK;
}


void   write_the_lexicon (CStr thepath, Format format) {
	ofstream out;
	StringTemp thefilename;

	thefilename = concat_path_to_filename(thepath,"milon/lex0e.ma");
	cerr << "writing lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,out,ios::trunc));
	out<<"\n%\n%\n% LEXICON ECEM\n%\n%\n";
	write_lexicon_2(out,lexicon_ecem);

	thefilename = concat_path_to_filename(thepath,"milon/lex0p.ma");
	cerr << "writing lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,out,ios::trunc));
	out<<"\n%\n%\n% LEXICON POAL\n%\n%\n";
	write_lexicon_2(out,lexicon_poal);
	out<<"\n%\n%\n% LEXICON TOAR\n%\n%\n";
	write_lexicon_1(out,lexicon_toar);
	out<<"\n%\n%\n% LEXICON JEM-PRATI\n%\n%\n";
	write_lexicon_1(out,lexicon_jem_prati);
	out<<"\n%\n%\n% LEXICON MILIT\n%\n%\n";
	write_lexicon_1(out,lexicon_milit);

	thefilename = concat_path_to_filename(thepath,"milon/lex10.ma");
	cerr << "writing lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,out,ios::trunc));
	out<<"\n%\n%\n% LEXICON MILOT YAXAS\n%\n%\n";
	writeln (out,lexicon_milot_yaxas,Format("T:L "));
	out<<"\n%\n%\n% LEXICON MILOT GUF\n%\n%\n";
	writeln (out,lexicon_milot_guf,Format("T:L "));
	out<<"\n%\n%\n% LEXICON MISPARIM\n%\n%\n";
	writeln (out,lexicon_misparim,Format("T:L "));
	out<<"\n%\n%\n% LEXICON P&ALEI-&EZER\n%\n%\n";
	writeln (out,lexicon_pealei_ezer,Format("T:L "));

	thefilename = concat_path_to_filename(thepath,"milon/lexmi.ma");
	cerr << "writing lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,out,ios::trunc));
	out<<"\n%\n%\n% LEXICON &ACAMIM MIUXADIM\n%\n%\n";
	writeln (out,lexicon_acmim_miuxdim,Format("T:L "));
	out<<"\n%\n%\n% LEXICON P&ALIM MIUXADIM -- AVAR\n%\n%\n";
	writeln (out,lexicon_pealim_miuxdim_avar,Format("T:L "));
	out<<"\n%\n%\n% LEXICON P&ALIM MIUXADIM -- HOWE/HOWE-SAVIL\n%\n%\n";
	writeln (out,lexicon_pealim_miuxdim_howe,Format("T:L "));
	out<<"\n%\n%\n% LEXICON P&ALIM MIUXADIM -- ATID/CIWUI/MAQOR\n%\n%\n";
	writeln (out,lexicon_pealim_miuxdim_atid,Format("T:L "));
	out<<"\n%\n%\n% LEXICON T^ARIM MIUXADIM\n%\n%\n";
	writeln (out,lexicon_tearim_miuxdim,Format("T:L "));

	thefilename = concat_path_to_filename(thepath,"milon/lexyd.ma");
	cerr << "writing lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,out,ios::trunc));
	out<<"\n%\n%\n% LEXICON YOC^EI DOFEN\n%\n%\n";
	write (out,lexicon_yocei_dofen,Format("T:L "));

	out.close();
}


#include <morph/tavniot.h>
void atxel_tearim_im_ntia_miuexedet () {	
	extern Tavniot tavniot_toar;
	LOOPVECTOR (;,tavniot_toar,i) {
		Tavnit curtavnit = tavniot_toar[i];
		if (curtavnit.sug()==1)
			tearim_im_ntia_miuxedet.insert(curtavnit.base(),true);
	}
}


void read_the_lexicon (CStr thepath, Format format) {
	ifstream in;
	StringTemp thefilename;

	thefilename = concat_path_to_filename(thepath, "milon/lex0e.ma");
	cerr << "reading lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,in));
	skip_comments(in,'%');
	DOx(read_lexicon_ecem(in));

	thefilename = concat_path_to_filename(thepath, "milon/lex0p.ma");
	cerr << "reading lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,in));
	skip_comments(in,'%');
	DOx(read_lexicon_poal(in));
	skip_comments(in,'%');
	DOx(read(in,lexicon_toar[0],Format("T:L ")));
	skip_comments(in,'%');
	DOx(read(in,lexicon_jem_prati[0],Format("T:L ")));
	skip_comments(in,'%');
	DOx(read(in,lexicon_milit[0],Format("T:L ")));

	thefilename = concat_path_to_filename(thepath,"milon/lex10.ma");  
	cerr << "reading lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,in));
	skip_comments(in,'%');
	DOx(read(in,lexicon_milot_yaxas,Format("T:L ")));
	skip_comments(in,'%');
	DOx(read(in,lexicon_milot_guf,Format("T:L ")));
	skip_comments(in,'%');
	DOx(read(in,lexicon_misparim,Format("T:L ")));
	skip_comments(in,'%');
	DOx(read(in,lexicon_pealei_ezer,Format("T:L ")));

	thefilename = concat_path_to_filename(thepath,"milon/lexmi.ma");  
	cerr << "reading lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,in));
	skip_comments(in,'%');
	DOx(read(in,lexicon_acmim_miuxdim,Format("T:L ")));
	skip_comments(in,'%');
	DOx(read(in,lexicon_pealim_miuxdim_avar,Format("T:L ")));
	skip_comments(in,'%');
	DOx(read(in,lexicon_pealim_miuxdim_howe,Format("T:L ")));
	skip_comments(in,'%');
	DOx(read(in,lexicon_pealim_miuxdim_atid,Format("T:L ")));
	skip_comments(in,'%');
	DOx(read(in,lexicon_tearim_miuxdim,Format("T:L ")));

	thefilename = concat_path_to_filename(thepath,"milon/lexyd.ma");  
	cerr << "reading lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,in));
	skip_comments(in,'%');
	DOx(read(in,lexicon_yocei_dofen,Format("T:P ")));

	in.close();

	atxel_acamim_im_ntiat_jayakut_miuexedet ();
	atxel_pealim_im_ntiat_maqor_miuexedet ();
	atxel_tearim_im_ntia_miuexedet ();
}




#ifdef TEST_LEXICON

void main (void) {
	read_the_lexicon ("c:\\harc");
	write_the_lexicon ("c:\\cpp");
//	cout << bool_str(lexicon_contains_ecem ("QIIM")) << " ";
//	cout << bool_str(lexicon_contains_toar ("QIIM")) << " ";
//	cout << bool_str(lexicon_contains_poal ("QIIM")) << " ";
}

#endif
