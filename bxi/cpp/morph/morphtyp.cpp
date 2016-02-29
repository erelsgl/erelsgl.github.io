/*
 * File morph/morphtyp.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphtyp.cpp
 */

/* morphtyp.cpp */

#include <morph/morphtyp.h>

char hd2char_tab[] = "xmtzp****PETMygj";
char hd2char(HeleqDiber hd) { return hd2char_tab[hd+8]; }
CStr hd2str_tab[] = {"MILT~XIBWR","MILIT","TWAR~PW&L","PW&L~&ZR","$M~PR@I","*","*","*","*","PW&L","&CM","TWAR","MSPR","MILT~IXS","MILT~GWP","MILT~$ALH"};
CStr hd2str(HeleqDiber hd) { return hd2str_tab[hd+8]; }
HeleqDiber char2hd (char c) { return 
	c=='P'?POAL: c=='E'?ECEM: c=='T'?TOAR: c=='M'?MISPAR:
	c=='y'?MILAT_YAXAS: c=='g'?MILAT_GUF: c=='j'?MILAT_JEELA: c=='x'?MILAT_XIBUR:
	c=='m'?MILIT: c=='t'?TOAR_POAL: c=='z'?POAL_EZER: c=='p'?JEM_PRATI: NO_HD; }

char vav2char(bool y) { return y? '1': '*'; }
bool char2vav(char c) { return (c=='1'); }

char jiabud2char_tab[] = "XHAS*JKL";
char jiabud2char(Jiabud j) { return jiabud2char_tab[j+4]; }
CStr jiabud2str_tab[] = {"$K$","H","$","$H","*","$","K$","LK$"};
CStr jiabud2str (Jiabud j) { return jiabud2str_tab[j+4]; }
Jiabud char2jiabud(char c) { return c=='A'?ANY_JIABUD: c=='X'?JIABUD_JEKJE: c=='H'?JIABUD_HA: c=='J'?JIABUD_JE: c=='K'?JIABUD_KJE: c=='L'?JIABUD_LKJE: c=='S'?JIABUD_JEHA: NO_JIABUD;}

char otiotyaxas2char_tab[] = "Ak*bml";
char otiotyaxas2char(OtiotYaxas y) { return otiotyaxas2char_tab[y+2]; }
CStr otiotyaxas2str_tab[] = {"BKLM","K","*","B","M","L"};
CStr otiotyaxas2str (OtiotYaxas y) { return otiotyaxas2str_tab[y+2]; }
OtiotYaxas char2otiotyaxas(char c) {return c=='A'?ANY_OY: c=='k'?OY_KAF: c=='b'?OY_BET: c=='m'?OY_MEM: c=='l'?OY_LAMED: NO_OY;}

char yidua2char_tab[] = "BK*L";
char yidua2char(Yidua y) { return yidua2char_tab[y+2]; }
CStr yidua2str_tab[] = {"","MIWD&","*","LA~MIWD&"};
CStr yidua2str(Yidua y) { return yidua2str_tab[y+2]; }
Yidua char2yidua(char c) {return c=='L'?LOMEYUDA: c=='K'?KNMEYUDA: c=='B'?BOTH: NONE;}

char meen2char_tab[] = "BN*Z";
char meen2char(Meen y) { return meen2char_tab[y+2]; }
CStr meen2str_tab[] = {"ZW\"N","NQBH","*","ZKR"};
CStr meen2str(Meen y) { return meen2str_tab[y+2]; }
Meen char2meen (char c) { return c=='Z'?ZAKAR: c=='N'?NQEVA: c=='B'?BOTH: NONE; }

char mispar2char_tab[] = "BR*Y";
char mispar2char(Mispar y) { return mispar2char_tab[y+2]; }
CStr mispar2str_tab[] = {"","RBIM","*","IXID"};
CStr mispar2str(Mispar y) { return mispar2str_tab[y+2]; }
Mispar char2mispar (char c) { return c=='Y'?YAXID: c=='R'?RABIM: c=='B'?BOTH: NONE; }

char smikut2char_tab[] = "BS*F";
char smikut2char(Smikut y) { return smikut2char_tab[y+2]; }
CStr smikut2str_tab[] = {"","NSMK","*","NPRD"};
CStr smikut2str(Smikut y) { return smikut2str_tab[y+2]; }
Smikut char2smikut (char c) { return c=='F'?NIFRAD: c=='S'?NISMAK: c=='B'?BOTH: NONE; }

char guf2char_tab[] = "A**123";
char guf2char(Guf y) { return guf2char_tab[y+2]; }
CStr guf2str_tab[] = {"","*","*","MDBR","NWKX","NSTR"};
CStr guf2str(Guf y) { return guf2str_tab[y+2]; }
Guf char2guf (char c) { return c=='1'?GUF1: c=='2'?GUF2: c=='3'?GUF3: c=='A'?BOTH: NONE; }

char zman2char_tab[] = "SCM**VHT";
char zman2char(Zman y) { return zman2char_tab[y+4]; }
CStr zman2str_tab[] = {"HWWH~SBIL","CIWWI","MQWR","*","*","&VR","HWWH","&TID"};
CStr zman2str(Zman y) { return zman2str_tab[y+4]; }
Zman char2zman(char c) {return c=='V'?AVAR: c=='H'?HOWE: c=='T'?ATID: c=='C'?CIWUI: c=='M'?MAQOR: c=='S'?HOWE_SAVIL: NO_ZMAN;}



/************************ maxrozot ************************/


CStr maxrozet_jiabud (Jiabud thejiabud) {
	switch(thejiabud) {
	case JIABUD_JEKJE: //return "$K$";
	case JIABUD_JEHA: //return "$H";
	case JIABUD_JE: return "$";
	case JIABUD_KJE: return "K$";
	case JIABUD_LKJE: return "LK$";
	case JIABUD_HA: return "H";
	default: return "";
	}
}


CStr maxrozet_otiotyaxas (OtiotYaxas theotiotyaxas) {
	switch(theotiotyaxas) {
	case OY_BET: return "B";
	case OY_KAF: return "K";
	case OY_LAMED: return "L";
	case OY_MEM: return "M";
	default: return "";
	}
}


CStr maxrozet_siomet_jayakut (Meen themeen, Mispar themispar, Guf theguf) {
	return 
		theguf==GUF1 && themispar==YAXID ?   "I":
		theguf==GUF1 && themispar==RABIM ?   "NW":
		theguf==GUF2 && themispar==YAXID ?   "K":
		theguf==GUF2 && themeen==ZAKAR && themispar==RABIM ?   "KM":
		theguf==GUF2 && themeen==NQEVA && themispar==RABIM ?   "KN":
		theguf==GUF3 && themeen==ZAKAR && themispar==YAXID ?   "W":
		theguf==GUF3 && themeen==NQEVA && themispar==YAXID ?   "H":
		theguf==GUF3 && themeen==ZAKAR && themispar==RABIM ?   "HM":
	theguf==GUF3 && themeen==NQEVA && themispar==RABIM ?   "HN": "";
}


CStr maxrozet_kinui_guf (Meen themeen, Mispar themispar, Guf theguf) {
	if (theguf==BOTH)  theguf=GUF3;
	if (themeen==BOTH) themeen=ZAKAR;
	return 
		theguf==GUF1 && themispar==YAXID ?   "ANI":
		theguf==GUF1 && themispar==RABIM ?   "ANXNW":
		theguf==GUF2 && themeen==ZAKAR && themispar==YAXID ?   "ATH":
		theguf==GUF2 && themeen==NQEVA && themispar==YAXID ?   "AT":
		theguf==GUF2 && themeen==ZAKAR && themispar==RABIM ?   "ATM":
		theguf==GUF2 && themeen==NQEVA && themispar==RABIM ?   "ATN":
		theguf==GUF3 && themeen==ZAKAR && themispar==YAXID ?   "HWA":
		theguf==GUF3 && themeen==NQEVA && themispar==YAXID ?   "HIA":
		theguf==GUF3 && themeen==ZAKAR && themispar==RABIM ?   "HM":
		theguf==GUF3 && themeen==NQEVA && themispar==RABIM ?   "HN": "";
}


CStr maxrozet_ribui (Meen themeen, Mispar themispar) {
	if (themispar==YAXID)  return "";
	else if (themispar==RABIM) return (themeen==NQEVA? "-WT": "-IM");
	else return "";
}


