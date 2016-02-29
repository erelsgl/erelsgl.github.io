/*
 * File morph/mip.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/mip.cpp
 */

/* mip.cpp */


#include <morph/mip.h>
#include <cpplib/stringt.h>

#ifdef UNIX
#include <strstream.h>
#else
#include <strstrea.h>
#endif

MorphInfoPattern MIP_KOLEL_HAKOL;


bool MorphInfoPattern::match (MorphInfoCR theinfo) const {  // does the info matches the pattern?
	if (!heleqdiber_xajuv) return true;
	#define BDOQ(tkuna) if (##tkuna##_xajuv && !identical_##tkuna##(theinfo.##tkuna##(),tkuna##())) return false;
	#define BDOQ_S(tkuna) if (##tkuna##_siomet_xajuv && !identical_##tkuna##(theinfo.##tkuna##_siomet(),tkuna##_siomet())) return false;
	BDOQ(baseword)	BDOQ(heleqdiber)	
	BDOQ(vav)		BDOQ(jiabud)	BDOQ(otiotyaxas)	BDOQ(yidua)
	BDOQ(meen)		BDOQ(mispar)	BDOQ(guf)			BDOQ(smikut)	BDOQ(zman)
	BDOQ_S(meen)	BDOQ_S(mispar)	BDOQ_S(guf)
	return true;
	#undef BDOQ
	#undef BDOQ_S
}




/*----- operations -----*/
void MorphInfoPattern::xituk (MorphInfoPatternCR a, MorphInfoPatternCR b) { 
	// puts in this MIP the properties that are common to both a and b.
	#define XITUK(tkuna)\
		if (a.##tkuna##_xajuv && b.##tkuna##_xajuv && identical_##tkuna##(a.##tkuna##(),b.##tkuna##()) ) {\
			set##tkuna## (a.##tkuna##()); \
			##tkuna##_xajuv = true;\
		}
	#define XITUK_S(tkuna)\
		if (a.##tkuna##_siomet_xajuv && b.##tkuna##_siomet_xajuv && identical_##tkuna##(a.##tkuna##_siomet(),b.##tkuna##_siomet()) ) {\
			set##tkuna##_siomet (a.##tkuna##_siomet()); \
			##tkuna##_siomet_xajuv = true;\
		}
	init();
	XITUK(baseword)	XITUK(heleqdiber)	
	XITUK(vav)		XITUK(jiabud)	XITUK(otiotyaxas)	XITUK(yidua)
	XITUK(meen)		XITUK(mispar)	XITUK(guf)			XITUK(smikut)	XITUK(zman)
	XITUK_S(meen)	XITUK_S(mispar)	XITUK_S(guf)
	#undef XITUK
	#undef XITUK_S
}

void MorphInfoPattern::xituk (MorphInfoCR a) {
	// intersects this MIP with a.
	#define XITUK(tkuna) if (!identical_##tkuna##(tkuna##(),a.##tkuna##()))  ##tkuna##_xajuv=false; 
	#define XITUK_S(tkuna) if (!identical_##tkuna##(tkuna##_siomet(),a.##tkuna##_siomet()))  ##tkuna##_siomet_xajuv=false; 
	XITUK(baseword)	XITUK(heleqdiber)	
	XITUK(vav)		XITUK(jiabud)	XITUK(otiotyaxas)	XITUK(yidua)
	XITUK(meen)		XITUK(mispar)	XITUK(guf)			XITUK(smikut)	XITUK(zman)
	XITUK_S(meen)	XITUK_S(mispar)	XITUK_S(guf)
	#undef XITUK
	#undef XITUK_S
}


void hattel (MorphInfo& theinfo, MorphInfoPatternCR thepattern) {
	#define HATTEL(tkuna) if (thepattern.##tkuna##_xajuv)  theinfo.set##tkuna##(thepattern.##tkuna##()); 
	HATTEL(baseword)	HATTEL(heleqdiber)
	HATTEL(vav)		HATTEL(jiabud)	HATTEL(otiotyaxas)	HATTEL(yidua)
	HATTEL(meen)		HATTEL(mispar)	HATTEL(guf)			HATTEL(smikut)	HATTEL(zman)
	HATTEL(meen_siomet)				HATTEL(mispar_siomet)					
	if (thepattern.guf_siomet()!=BOTH) { HATTEL(guf_siomet) }
	#undef HATTEL
}
/*
void MorphInfoPattern::kasse (MorphInfo& a) const {
	// mkasse ^et "a" b-koll ha-tkunot ha-xa$ubot $ell "this".
	#define KASSE(tkuna) if (##tkuna##_xajuv)  a.set##tkuna##(my##tkuna##); 
	KASSE(baseword)	KASSE(heleqdiber)	
	KASSE(vav)		KASSE(jiabud)	KASSE(otiotyaxas)	KASSE(yidua)
	KASSE(meen)		KASSE(mispar)	KASSE(guf)			KASSE(smikut)	KASSE(zman)
	KASSE(meen_siomet)				KASSE(mispar_siomet)					KASSE(guf_siomet)
	#undef KASSE
}
*/


/***************************************************************************************/
/*      IO for MorphInfoPattern                                                        */
/***************************************************************************************/


void MorphInfoPattern::klum_lo_xajuv () {
	baseword_xajuv= heleqdiber_xajuv= vav_xajuv=
	jiabud_xajuv= otiotyaxas_xajuv= yidua_xajuv=
	meen_xajuv= mispar_xajuv= guf_xajuv=
	smikut_xajuv= zman_xajuv= meen_siomet_xajuv=
	mispar_siomet_xajuv= guf_siomet_xajuv= false;
	yidua_carik_lhatim= meen_carik_lhatim= 
	mispar_carik_lhatim= guf_carik_lhatim= false;
}


void MorphInfoPattern::init () {
	MorphInfo::init();
	klum_lo_xajuv();
}

void MorphInfoPattern::set_hakol_xajuv(MorphInfoCR theinfo) {
	MorphInfo::operator=(theinfo);
	baseword_xajuv= heleqdiber_xajuv= vav_xajuv=
	jiabud_xajuv= otiotyaxas_xajuv= yidua_xajuv=
	meen_xajuv= mispar_xajuv= guf_xajuv=
	smikut_xajuv= zman_xajuv= meen_siomet_xajuv=
	mispar_siomet_xajuv= guf_siomet_xajuv= true;
	yidua_carik_lhatim= meen_carik_lhatim= 
	mispar_carik_lhatim= guf_carik_lhatim= false;
}

void MorphInfoPattern::set(MorphInfoCR theinfo) {
	MorphInfo::operator=(theinfo);
	if (myjiabud!=NO_JIABUD)  myjiabud=ANY_JIABUD;
	if (myotiotyaxas!=NO_OY)  myotiotyaxas=ANY_OY;
	if (myguf_siomet!=NONE)   myguf_siomet=BOTH;
	baseword_xajuv= heleqdiber_xajuv= vav_xajuv=
	jiabud_xajuv= otiotyaxas_xajuv= yidua_xajuv=
	meen_xajuv= mispar_xajuv= guf_xajuv=
	smikut_xajuv= zman_xajuv= meen_siomet_xajuv=
	mispar_siomet_xajuv= guf_siomet_xajuv= true;
	yidua_carik_lhatim= meen_carik_lhatim= 
	mispar_carik_lhatim= guf_carik_lhatim= false;
}

void MorphInfoPattern::set_hatxala_lo_xajuva(MorphInfoCR theinfo) {
	MorphInfo::operator=(theinfo);
	if (myjiabud!=NO_JIABUD)  myjiabud=ANY_JIABUD;
	if (myotiotyaxas!=NO_OY)  myotiotyaxas=ANY_OY;
	if (myguf_siomet!=NONE)   myguf_siomet=BOTH;
	heleqdiber_xajuv=true;
	smikut_xajuv=guf_siomet_xajuv=true;
	baseword_xajuv= vav_xajuv= jiabud_xajuv= otiotyaxas_xajuv= yidua_xajuv=
	meen_xajuv= mispar_xajuv= guf_xajuv=
	zman_xajuv= meen_siomet_xajuv= mispar_siomet_xajuv= false;
	yidua_carik_lhatim= meen_carik_lhatim= 
	mispar_carik_lhatim= guf_carik_lhatim= false;
}

void MorphInfoPattern::set_sof_lo_xajuv(MorphInfoCR theinfo) {
	MorphInfo::operator=(theinfo);
	if (myjiabud!=NO_JIABUD)  myjiabud=ANY_JIABUD;
	if (myotiotyaxas!=NO_OY)  myotiotyaxas=ANY_OY;
	if (myguf_siomet!=NONE)   myguf_siomet=BOTH;
	heleqdiber_xajuv=true;
	vav_xajuv=true; jiabud_xajuv=!myvav; otiotyaxas_xajuv=!myvav&&myjiabud==NO_JIABUD;
	baseword_xajuv= yidua_xajuv=
	meen_xajuv= mispar_xajuv= guf_xajuv=
	smikut_xajuv= zman_xajuv= meen_siomet_xajuv=
	mispar_siomet_xajuv= guf_siomet_xajuv= false;
	yidua_carik_lhatim= meen_carik_lhatim= 
	mispar_carik_lhatim= guf_carik_lhatim= false;
}

void MorphInfoPattern::set_klum_lo_xajuv(MorphInfoCR theinfo) {
	MorphInfo::operator=(theinfo);
	if (myjiabud!=NO_JIABUD)  myjiabud=ANY_JIABUD;
	if (myotiotyaxas!=NO_OY)  myotiotyaxas=ANY_OY;
	if (myguf_siomet!=NONE)   myguf_siomet=BOTH;
	baseword_xajuv= heleqdiber_xajuv= vav_xajuv=
	jiabud_xajuv= otiotyaxas_xajuv= yidua_xajuv=
	meen_xajuv= mispar_xajuv= guf_xajuv=
	smikut_xajuv= zman_xajuv= meen_siomet_xajuv=
	mispar_siomet_xajuv= guf_siomet_xajuv= false;
	yidua_carik_lhatim= meen_carik_lhatim= 
	mispar_carik_lhatim= guf_carik_lhatim= false;
}

void MorphInfoPattern::set_baseword (MorphInfoCR theinfo) {
	MorphInfo::setbaseword(theinfo); 
	baseword_xajuv= heleqdiber_xajuv= true;
}

void MorphInfoPattern::set_heleqdiber (MorphInfoCR theinfo) {
	MorphInfo::setheleqdiber(theinfo.heleqdiber()); 
	heleqdiber_xajuv= true;
}

void MorphInfoPattern::set_txiliot (MorphInfoCR theinfo) {
	MorphInfoBasic::settxiliot(theinfo);
	vav_xajuv= jiabud_xajuv= otiotyaxas_xajuv= yidua_xajuv= true;
	MorphInfoBasic::setheleqdiber(theinfo.heleqdiber());
	heleqdiber_xajuv = true;
}

void MorphInfoPattern::set (char* the_pattern) {
	istrstream in (the_pattern);
	read (in);
}




void MorphInfoPattern::ktov_cafuf (ostream& out)  const {
	#define KTOV(tkuna)  if (yej_##tkuna##(myheleqdiber))\
		out<<(##tkuna##_xajuv? ##tkuna##2char(##tkuna##()): DONTCARE)
	#define KTOV2(tkuna) if (yej_##tkuna##(myheleqdiber))\
		out<<(##tkuna##_xajuv? ##tkuna##2char(##tkuna##()): ##tkuna##_carik_lhatim? hatama: DONTCARE)

	if (!heleqdiber_xajuv)  { out << "===="; return; }
	if (baseword_xajuv) out << mybaseword;  else  out << DONTCARE;
	out << (vav_xajuv? vav2char(vav()): DONTCARE)     /* * means 'no vav', 1 means 'vav' */
	    << hd2char(heleqdiber())
	    << (jiabud_xajuv? jiabud2char(jiabud()): DONTCARE);
	char hatama = '9';

	KTOV(otiotyaxas); KTOV2(yidua); 
	KTOV2(meen); KTOV2(mispar); KTOV(smikut); KTOV2(guf); KTOV(zman);
	if (yej_siomet(myheleqdiber))  {
		out		<< (meen_siomet_xajuv? meen2char(meen_siomet()): DONTCARE)
				<< (mispar_siomet_xajuv? mispar2char(mispar_siomet()): DONTCARE)
				<< (guf_siomet_xajuv? guf2char(guf_siomet()): guf_carik_lhatim? hatama: DONTCARE);
	}
	#undef KTOV
	#undef KTOV2
}


void MorphInfoPattern::ktov_barur (ostream& out, bool ktov_hatama)  const {
	#define KTOV(tkuna)  if (yej_##tkuna##(myheleqdiber) && ##tkuna##_xajuv)	out << "-" << ##tkuna##2str(##tkuna##())

	if (!heleqdiber_xajuv)  { out << "=MILH="; return; }    // milla kol$ehi^
	if (vav_xajuv && vav())   out << "W-";
	if (jiabud_xajuv && jiabud()!=NO_JIABUD)  out << jiabud2str(jiabud()) << "-";
	if (otiotyaxas_xajuv && otiotyaxas()!=NO_OY)  out << otiotyaxas2str(otiotyaxas()) << "-";
	if (baseword_xajuv) {
		if (myheleqdiber==MILAT_XIBUR)  { out << "'"<<mybaseword<<"'"; return; }
		else out << "'" << mybaseword << "'[";
	}
	out << hd2str(heleqdiber());
	KTOV(yidua); KTOV(meen); KTOV(mispar); KTOV(smikut); KTOV(guf); KTOV(zman);
	if (guf_siomet_xajuv && guf_siomet()!=NONE)  {
		out << "-&M~SIWMT";
		if (meen_siomet_xajuv && mispar_siomet_xajuv) 
			out << "(" << meen2str(meen_siomet()) 
				<< " " << mispar2str(mispar_siomet()) 
				<< " " << guf2str(guf_siomet()) << ")";
	}
	if (baseword_xajuv)  out << ']';
	//	out		<< (meen_siomet_xajuv? meen2char(meen_siomet()): DONTCARE)
	//			<< (mispar_siomet_xajuv? mispar2char(mispar_siomet()): DONTCARE)
	//			<< (guf_siomet_xajuv? guf2char(guf_siomet()): guf_carik_lhatim? hatama: DONTCARE);
	//}
	if (!ktov_hatama) return;
	if (!yidua_carik_lhatim && !meen_carik_lhatim &&!mispar_carik_lhatim &&!guf_carik_lhatim) return;
	out << " (MTAIM B";
	if (yidua_carik_lhatim)  out << " YIDW&";
	if (meen_carik_lhatim)  out << " MIN";
	if (mispar_carik_lhatim)  out << " MSPR";
	if (guf_carik_lhatim)  out << " GWP";
	out << ")";

	#undef KTOV
}




void write (ostream& out, MorphInfoPatternCR themip, Format format) {
	themip.ktov_cafuf (out);
}

status MorphInfoPattern::read (istream& in) {
	#define xajuv(c) (c!=DONTCARE&&c!='9')
	#define READ(tkuna)\
		if (yej_##tkuna##(myheleqdiber))  {\
			c=in.get();\
			if(xajuv(c)) {\
				set##tkuna##(char2##tkuna##(c));\
				##tkuna##_xajuv=true; \
			}\
		}
	#define READ2(tkuna)\
		if (yej_##tkuna##(myheleqdiber))  {\
			c=in.get();\
			if(xajuv(c)) {\
				set##tkuna##(char2##tkuna##(c));\
				##tkuna##_xajuv=true; \
			}\
			else if (isdigit(c)) {\
				##tkuna##_carik_lhatim=true; \
			}\
		}

	char c;
	init();
	skip(in," \t\n");
	if (in.peek()==DONTCARE) in.get();      // baseword is not xajuv.
	else { 
		StringTemp thebaseword(20);
		DOr(thebaseword.read(in,"*01=" /* DONTCARE=='='! */ )); // read until 'vav' or DONTCARE is encountered
		setbaseword(thebaseword.str);
		baseword_xajuv = true;
	}
	c=in.get(); if(xajuv(c)) { setvav(char2vav(c)); vav_xajuv=true; } 
	c=in.get(); if(xajuv(c)) { setheleqdiber(char2hd(c)); heleqdiber_xajuv=true; }
	c=in.get(); if(xajuv(c)) { setjiabud(char2jiabud(c)); jiabud_xajuv=true; }
	if (!heleqdiber_xajuv) return OK;

	READ(otiotyaxas); READ2(yidua);
	READ2(meen); READ2(mispar); READ(smikut); READ2(guf); READ(zman);
	if (yej_siomet(myheleqdiber)) {
		c=in.get();  if (xajuv(c))  { setmeen_siomet(char2meen(c));  meen_siomet_xajuv=true; }
		c=in.get();  if (xajuv(c))  { setmispar_siomet(char2mispar(c));  mispar_siomet_xajuv=true; }
		c=in.get();  if (xajuv(c))  { setguf_siomet(char2guf(c));  guf_siomet_xajuv=true; }
	}

	return OK;
}


status read (istream& in, MorphInfoPattern& themip, Format format) {
	return themip.read (in);
}







/***************************************************************************************/
/*      Test for MorphInfoPattern                                                      */
/***************************************************************************************/

#ifdef TEST_MIP

void test_xituk() {
	MorphInfo i1 ("KDWR*E**LZRF***"), i2 ("KDWRT1E**KNYF***");
	MorphInfoPattern p1 (i1);
	p1.xituk(i2);
	cout << i1 << "  xituk  " << i2 << "  $awe  " << p1 << endl;
}


void test_siomot() {
	MorphInfoPattern p1 ("=*y*B==");// tabnit $ell millat-yaxs &imm siomt
	MorphInfo i1("B*y****");		// millat-yaxs bli siomt
	MorphInfo i2("B*y*ZY3");		// millat-yaxs &imm siomt
	MorphInfo i3("B*y*NY2");		// millat-yaxs &imm siomt
	cout << i1 << (p1.match(i1)? " matches ": " MISmatches ") << p1 << endl;
	cout << i2 << (p1.match(i2)? " matches ": " MISmatches ") << p1 << endl;
	cout << i3 << (p1.match(i3)? " matches ": " MISmatches ") << p1 << endl;
}


void test_klali() {
	MorphInfoPattern p1 ("====");// tabnit $ell millat-yaxs &imm siomt
	MorphInfo i1("KDWR*E**LZRF***"), i2("KDWRT1E**KNYF***");
	MorphInfo i4("B*y****"), i5("B*y*ZY3"), i6("B*y*NY2");		
	cout << i1 << (p1.match(i1)? " matches ": " MISmatches ") << p1 << endl;
	cout << i2 << (p1.match(i2)? " matches ": " MISmatches ") << p1 << endl;
	cout << i4 << (p1.match(i4)? " matches ": " MISmatches ") << p1 << endl;
	cout << i5 << (p1.match(i5)? " matches ": " MISmatches ") << p1 << endl;
	cout << i6 << (p1.match(i6)? " matches ": " MISmatches ") << p1 << endl;
}


void main (void) {
	test_xituk();
	test_siomot();
	test_klali();
}	


#endif

