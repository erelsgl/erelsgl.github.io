/*
 * File morph/morphinf.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphinf.cpp
 */

/* morphinf.cpp */


#include <morph/morphinf.h>
#include <cpplib/stringt.h>
#ifdef UNIX
#include <strstream.h>
#else
#include <strstrea.h>
#endif

StringTemp concat_bw (MorphInfoCR a, MorphInfoCR b) {
	return concat (a.baseword(),' ',b.baseword());
}


/***************************************************************************************/
/*      IO for MorphInfoBasic                                                          */
/***************************************************************************************/


void MorphInfoBasic::ktov_cafuf (ostream& out)  const{
	#define WRITE(tkuna)  if (yej_##tkuna##(myheleqdiber))  out<<##tkuna##2char(##tkuna##())
	out << vav2char(vav()) <<  hd2char(heleqdiber());
	out << jiabud2char(jiabud());
	WRITE(otiotyaxas); WRITE(yidua); 
	WRITE(meen); WRITE(mispar); WRITE(smikut); WRITE(guf); WRITE(zman);
	if (yej_siomet(myheleqdiber))  out << meen2char(meen_siomet()) << mispar2char(mispar_siomet()) << guf2char(guf_siomet());
	#undef WRITE
}
 

void MorphInfoBasic::ktov_meforat (ostream& out)  const{
	#define WRITE(tkuna)  if (yej_##tkuna##(myheleqdiber))  out<<##tkuna##2str(##tkuna##())<<" "
	out << hd2str(heleqdiber()) << " ";
	WRITE(yidua);
	WRITE(zman); WRITE(meen); WRITE(mispar); WRITE(smikut); WRITE(guf); 
	if (yej_siomet(myheleqdiber)) 
		if (has_siomet())
			out << " (SIOMT: " << meen2str(meen_siomet()) << " " << mispar2str(mispar_siomet()) << " " << guf2str(guf_siomet()) << ")";
	#undef WRITE
}


void write (ostream& out, MorphInfoBasicCR theinfo, Format format) {
	theinfo.ktov_cafuf (out);
}

status read (istream& in, MorphInfoBasicR theinfo, Format format) {
	#define READ(tkuna)\
		if (yej_##tkuna##(theinfo.heleqdiber()))  {\
			char c=in.get();\
			theinfo.set##tkuna##(char2##tkuna##(c));\
		}

	theinfo.init();
	theinfo.setvav(char2vav(in.get()));
	theinfo.setheleqdiber(char2hd(in.get()));
	theinfo.setjiabud(char2jiabud( in.get() ));
	READ(otiotyaxas); READ(yidua);
	READ(meen); READ(mispar); READ(smikut); READ(guf); READ(zman);
	if (yej_siomet(theinfo.heleqdiber())) {
		char c1=in.get(); char c2=in.get(); char c3=in.get(); 
		theinfo.setsiomet ( char2meen(c1), char2mispar(c2), char2guf(c3) );
	}
	if (theinfo.poal())
		theinfo.setsmikut(NONE);
	return OK;
}


bool identical_txiliot (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	return	
			identical_vav(a.vav(),b.vav()) &&
			identical_jiabud(a.jiabud(),b.jiabud()) &&
			identical_otiotyaxas(a.otiotyaxas(),b.otiotyaxas()) &&
			identical_yidua(a.yidua(),b.yidua()); }
bool identical_ecem (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	return	identical_meen(a.meen(),b.meen()) && 
			identical_mispar(a.mispar(),b.mispar()) && 
			identical_smikut(a.smikut(),b.smikut()); }
bool identical_siomet (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	return	identical_meen(a.meen_siomet(),b.meen_siomet()) && 
			identical_mispar(a.mispar_siomet(),b.mispar_siomet()) && 
			identical_guf(a.guf_siomet(),b.guf_siomet()); }
bool identical_poal (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	return	identical_meen(a.meen(),b.meen()) && 
			identical_mispar(a.mispar(),b.mispar()) && 
			identical_guf(a.guf(),b.guf()) &&
			(a.nismak()==b.nismak())         &&
			identical_zman(a.zman(),b.zman()); }


bool identical (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	if (a.heleqdiber()!=b.heleqdiber()) return false;
	if (!identical_txiliot(a,b)) return false;
	switch (a.heleqdiber()) {
		case ECEM: case TOAR: case MISPAR: 
			return ( identical_ecem(a,b) && identical_siomet(a,b) );
		case POAL: case POAL_EZER: 
			return ( identical_poal(a,b)  );
		case JEM_PRATI:
			return identical_meen(a.meen(),b.meen());
		case MILAT_GUF:
			return ( identical_poal(a,b) );
		case MILAT_YAXAS:
			return ( identical_siomet(a,b) );
		default: return true;
	}
}


void MorphInfoBasic::set (char* theinfo) {
	istrstream in (theinfo);
	read (in, *this);
}



MorphInfoBasic mibXIBUR (MILAT_XIBUR);
MorphInfoBasic mibYAXAS (MILAT_YAXAS);

MorphInfoBasic MorphInfoBasic::mila_rijona_info() const {
	if (myvav) return mibXIBUR;
	else if (myjiabud!=NO_JIABUD) return mibXIBUR;
	else if (myotiotyaxas!=NO_OY) return mibYAXAS;
	else return *this;
}





/***************************************************************************************/
/*      IO for MorphInfo                                                               */
/***************************************************************************************/


void MorphInfo::ktov_cafuf (ostream& out)  const{
	out << mybaseword;
	if (poal()&&nismak())           // po&l nismak == po&l &imm no$e^
		out<<"-O";
	MorphInfoBasic::ktov_cafuf(out);
}

void MorphInfo::ktov_meforat (ostream& out)  const{
	out << mybaseword << ", ";
	MorphInfoBasic::ktov_meforat(out);
}

void MorphInfo::ktov_barur (ostream& out, CStr thefullword)  const{
	Index i;	
	for (i=0; i<uint(txiliot()); ++i)   // write the txiliot
		out << thefullword[i];
	if ( myyidua==KNMEYUDA && 
		 (myotiotyaxas==OY_BET||myotiotyaxas==OY_KAF||myotiotyaxas==OY_LAMED) )
			out << "H";
	if (i>0) out << "-";
	thefullword=thefullword+i;
	switch (myheleqdiber) {
		case ECEM: 
			if (mysmikut==NISMAK) 
				out << "H-" << mybaseword << maxrozet_ribui() << "-" << "$L";
			else if (has_siomet())
				out << "H-" << mybaseword << maxrozet_ribui() << "-" << "$L" << maxrozet_siomet_jayakut();
			else 
				out << mybaseword << maxrozet_ribui();
			break;
		case TOAR: 
			out<<"%";
			if (mysmikut==NISMAK) 
				out << thefullword << "-";
			else
				out << thefullword;
			break;
		case MISPAR:
			out<<"#";
			if (mysmikut==NISMAK) 
				out << thefullword << "-";
			else if (has_siomet()) 
				out << mybaseword << "-" << maxrozet_siomet_jayakut();
			else 
				out << thefullword;
			break;
		case POAL: case POAL_EZER: 
			switch (myzman) {
				case AVAR: case ATID:
					out << maxrozet_kinui_guf () << "-" << thefullword; break;
				case HOWE: case HOWE_SAVIL:
					if (myguf==NONE)
						out << thefullword;
					else
						out << maxrozet_kinui_guf () << "-" << "&K$W-" << thefullword; 
					break;
				case CIWUI:
					out << thefullword << "!";  break;
				case MAQOR:
					out << "|" << thefullword << "|";  break;
			}
			if (myheleqdiber==POAL_EZER) out<<'z';
			break;
		case MILAT_YAXAS:
			if (!has_siomet())
				out << "^" << thefullword;
			else 
				out << "^" << mybaseword << "-" << maxrozet_siomet_jayakut();
			break;
		case JEM_PRATI:
			out << "'" << thefullword << "'";
			break;
		default: 
			out << thefullword;
			if (strlen(thefullword)>1) out << hd2char(myheleqdiber);
			break;
	}	
}
	



void write (ostream& out, MorphInfoCR theinfo, Format format) { theinfo.ktov_cafuf(out); }

status read (istream& in, MorphInfoR theinfo, Format format) {
	skip(in," \t\n");
	StringTemp thebaseword(20);    
	DOr(thebaseword.read(in,"0123456789*"));      // read until a sign of the field 'vav' is encountered
	DOr(read (in, (MorphInfoBasicR)theinfo, format) );
	theinfo.setbaseword(thebaseword.str);
	return OK;
}


void MorphInfo::set (char* theinfo) {
	istrstream in (theinfo);
	read (in, *this);
}


/*
			MorphInfo MorphInfo::mila_rijona_info() const
*/

MorphInfo miKLUM;
MorphInfo miW("W",MILAT_XIBUR);
MorphInfo miJE("$",MILAT_XIBUR);
MorphInfo miKJE("K$",MILAT_XIBUR);
MorphInfo miLKJE("LK$",MILAT_XIBUR);
MorphInfo miHA("H",MILAT_XIBUR);
MorphInfo miB("B",MILAT_YAXAS);
MorphInfo miK("K",MILAT_YAXAS);
MorphInfo miL("L",MILAT_YAXAS);
MorphInfo miM("M",MILAT_YAXAS);
MorphInfo miNQUDA(".",MILAT_XIBUR);
MorphInfo miMAQAF("-",MILAT_XIBUR);

MorphInfo jiabud_info (Jiabud thejiabud) {
	switch(thejiabud) {
	case JIABUD_JEKJE:
	case JIABUD_JEHA: 
	case JIABUD_JE: return miJE;
	case JIABUD_KJE: return miKJE;
	case JIABUD_LKJE: return miLKJE;
	case JIABUD_HA: return miHA;
	default: return miKLUM;
	}
}


MorphInfo otiotyaxas_info (OtiotYaxas theotiotyaxas) {
	switch(theotiotyaxas) {
	case OY_BET: return miB;
	case OY_KAF: return miK;
	case OY_LAMED: return miL;
	case OY_MEM: return miM;
	default: return miKLUM;
	}
}



void MorphInfo::operator= (MorphInfoCR theinfo) {
	duplicate (*this, theinfo); }

MorphInfo::MorphInfo (MorphInfoCR theinfo)  {
	duplicate (*this, theinfo); }


CStr MorphInfo::mila_rijona() const {
	if (myvav) return "W";
	else if (myjiabud!=NO_JIABUD) return maxrozet_jiabud(myjiabud);
	else if (myotiotyaxas!=NO_OY) return maxrozet_otiotyaxas(myotiotyaxas);
	else return baseword();
}


MorphInfo MorphInfo::mila_rijona_info() const {
	if (myvav) return miW;
	else if (myjiabud!=NO_JIABUD) return jiabud_info(myjiabud);
	else if (myotiotyaxas!=NO_OY) return otiotyaxas_info(myotiotyaxas);
	else return *this;
}

MorphInfo MorphInfo::mila_axrona_info() const { // The info that matches the last particle of the word
	MorphInfo result = *this;
	result.setvav(false);  
	result.setjiabud(NO_JIABUD);
	result.setotiotyaxas(NO_OY);
	return result;
}




/***************************************************************************************/
/*      Test for MorphInfo                                                             */
/***************************************************************************************/

#ifdef TEST_MORPHINF

struct Test {
//BaseWord	mybaseword;            // the lexical value (base-word)
//		short       txiliot: 3;	
//		Yidua		myyidua: 3;
		HeleqDiber  myhd:    3;
		int  myhd1:    3;
/*
		HeleqDiber	myheleqdiber: 5;	
		Meen		mymeen: 3;		
		Mispar		mymispar: 3;	
		Guf			myguf: 4;		
		Smikut		mysmikut: 3;	
		Zman		myzman: 4;		
		Binyan		mybinyan: 4;	

		Meen        mymeen_siomet: 3;
		Mispar		mymispar_siomet: 3;
		Guf			myguf_siomet: 4;*/
};	

void main (void) {
	//cout << sizeof Test << endl;
	//cout << sizeof LexicalInfo << endl;
	//cout << sizeof MorphInfoBasic << endl;
	//cout << sizeof MorphInfo << endl;

	MorphInfo info ("XMAH",ECEM); info.settxiliot(false,NO_JIABUD,NO_OY,LOMEYUDA);
	info.setecem (NQEVA,YAXID,NISMAK);   
	info.ktov_barur (cout,"XMAT"); cout<<endl;
	info.setecem (NQEVA,YAXID,NIFRAD);   info.setsiomet (BOTH,YAXID,GUF1);  
	info.ktov_barur (cout,"XMATI"); cout<<endl;
	info.setotiotyaxas(OY_BET);  info.settxiliot(1);  info.setsiomet (ZAKAR,RABIM,GUF3);  
	info.ktov_barur (cout,"BXMATM"); cout<<endl;
	info.setjiabud(JIABUD_KJE);  info.settxiliot(3);  info.setsiomet (NQEVA,RABIM,GUF2);  
	info.ktov_barur (cout,"KJBXMATKN"); cout<<endl;

	info.init ("$MR",POAL); info.settxiliot(false,NO_JIABUD,NO_OY,LOMEYUDA);
	info.setpoal (NQEVA,YAXID,GUF3,AVAR);
	info.ktov_barur (cout,"$MRH");  cout<<endl;
	info.setpoal (ZAKAR,RABIM,GUF2,AVAR);  info.setjiabud(JIABUD_KJE);  info.settxiliot(2);  
	info.ktov_barur (cout,"K$$MRTM");  cout<<endl;
	info.setpoal (BOTH,RABIM,GUF1,AVAR);  info.setvav(true);  info.settxiliot(3);  
	info.ktov_barur (cout,"WK$$MRNW");  cout<<endl;

	info.setpoal (ZAKAR,YAXID,GUF3,ATID); info.settxiliot(false,NO_JIABUD,NO_OY,LOMEYUDA);
	info.ktov_barur (cout,"I$MWR");  cout<<endl;
	info.setpoal (NQEVA,RABIM,GUF2,ATID);  info.setjiabud(JIABUD_JE);  info.settxiliot(1);  
	info.ktov_barur (cout,"$T$MRW");  cout<<endl;
	info.setpoal (BOTH,YAXID,GUF1,ATID);  info.setvav(true);  info.settxiliot(2);  
	info.ktov_barur (cout,"W$A$MWR");  cout<<endl;

	info.settxiliot(false,NO_JIABUD,NO_OY,LOMEYUDA);
	info.setpoal (ZAKAR,YAXID,GUF2,CIWUI); info.settxiliot(1); info.setvav(true);
	info.ktov_barur (cout,"W$MWR");  cout<<endl;
	info.setpoal (ZAKAR,RABIM,GUF2,CIWUI);  info.settxiliot(0);  info.setvav(false);
	info.ktov_barur (cout,"$MRW");  cout<<endl;

	info.init ("B",MILAT_YAXAS); info.settxiliot(false,NO_JIABUD,NO_OY,LOMEYUDA);
	info.setsiomet (ZAKAR,RABIM,GUF2); 
	info.ktov_barur (cout,"BKM"); cout<<endl;
}



#endif

