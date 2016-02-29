/*
 * File morph/convlang.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/convlang.cpp
 */

/* convlang.cpp */
 
#include <morph/convlang.h>

/*******************************************************************************/
/*********      Hebrew-English letter translation     **************************/
/*******************************************************************************/

uchar heb2eng_tab[] = "ABGDHWZX@IKKLMMNNS&PPCCQR$T";
CStr hebrew_english_letters = "ABGDHWZX@IKKLMMNNS&PPCCQR$T";
bool is_hebrew_english_letter (char c) { return contains(hebrew_english_letters,c); }

uchar heb2eng(uchar c) { return  (ALEPH<=c&&c<=TAV)? heb2eng_tab[c-ALEPH]: c; }
uchar eng2heb(uchar c) { Index i=charindex(c,CStr(heb2eng_tab)); if (i<27) return ALEPH+i; else return c; }

void heb2eng (StringTemp& s) 
	{for (Index i=0; i<s.len; ++i) s[i]=heb2eng(s[i]); } 

void eng2heb (StringTemp& s) {
	for (Index i=0; i<s.len; ++i) {
		if (i+1<s.len && is_hebrew_english_letter(s[i+1]))       s[i]=to_thilit(eng2heb(s[i]));
		else if (i>0 && is_heb(s[i-1]))     s[i]=to_sofit(eng2heb(s[i]));
		else s[i]=to_thilit(eng2heb(s[i]));
	}
}



/*******************************************************************************/
/*********      Fonemic-English letter translation     **************************/
/*******************************************************************************/



bool is_tnuaa (char c) {
	return charinstring(c,"aeiou");
}

bool is_icur (char c) {
	return (c!='\0' && charinstring(c,"^bgdhwzx@yklmns&pcqr$t-"));
}


bool fonemic2eng_special (CStr source, StringTemp& target) {
	if ( compare(source,"^im")==0 ) {
		target.append("AM"); return true;
	}
	if ( compare(source,"min-")==0 ) {
		target.append("MN"); return true;
	}
	if ( compare(source,"mis'")==0 ) {
		target.append("MS'"); return true;
	}
	if ( compare(source,"koll")==0 ) {
		target.append("KL"); return true;
	}
	return false;
}


void fonemic2eng (StringTempCR source, StringTemp& target) {
	char c, prev, next, prev2, next2;
	target.truncate();
	if (fonemic2eng_special(source.str,target))  return;
	for (Index i=0; i<source.len; ++i) {
		c=source[i];
		prev=(i>0? source[i-1]: '\0');             next=source[i+1];
		prev2=(i>1? source[i-2]: '\0');            next2=(i<source.len-2? source[i+2]: '\0');
		if (prev=='-' || prev=='=') prev='\0';
		if (prev2=='-' || prev2=='=') prev2='\0';
		if (c==prev && c!='i') continue;					// dagge$ xazaq
		switch (c) {
		case '-':  
			if (fonemic2eng_special(source.str+1+i,target))  return;
			continue;
		case 'a':
			if (next=='\0')  { target.append('H');	}						 // xawa ==> XWWH
			continue;
		case 'e':
			if (next=='\0')  { target.append('H');	}						 // xawa ==> XWWH
			continue;
		case 'w':
			if ((next=='e' || next=='a') && next2!='w' && prev!='\0')   { target.append('W'); }     // ha$we ==> H$WWH     $wa^ ==> $WW^     haway ==> HWWII
			else if ((prev=='e' || prev=='a' || prev=='i') && is_icur(next)) { target.append('W'); }   // rewx ==> RWWX  dezawu ==> DZWW
			break;
		case 'i':
			if (is_icur(next2) && next!=next2 && next!='i') continue;
			if (next=='-') continue;
			if (next=='y') continue; 		// &ibriyim ==> &BRIIM;     ^otiyot ==> ^WTIWT
			break;
		case 'y': 
			if      (prev=='a'  && is_icur(next))  target.append('I');     // qayam ==> QIIM, hanxaya ==> HNXIH
			else if (prev!='\0' && next=='a' && next2!='\0')  target.append('I');     // syag ==> SIIG, yad ==> ID
			c='i';
			break;
		case 'o': 
			c='w';
			if (next=='^') continue;  // zo^t ==> Z^T
			break;
		case 'u':
			c='w';
			break;
		case '=':
			if (i<source.len-1)    c='-';
			else continue;
			break;
		case 'j': c='$'; break;
		case 'f': c='p'; break;
		case 'v': c='b'; break;
		case '^': c='a'; break;
		};
		c=toupper(c);
		DOx(target.append(c));
	}
}



/****************************************************************************************************/
/********                     CONVLANG SELF TEST                                           **********/
/****************************************************************************************************/

// The test has moved to the file:
//	convlang.test.cpp
