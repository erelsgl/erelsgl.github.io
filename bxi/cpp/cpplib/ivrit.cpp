/*
 * File cpplib/ivrit.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/ivrit.cpp
 */

/* ivrit.cpp -- hebrew support unit */


#include <conio.h>
#include <stdio.h>
#include <cpplib/ivrit.h>
#include <cpplib/klli1.h>


/****************** num to heb *******************************************/

const char units_letter[9] =	  { ALEPH, BET, GIMEL, DALET, HEY, VAV, ZAYIN, HET, TET };
const char tens_letter[9] =	  { YUD, KAF, LAMED, MEM, NUN, SAMEK, AYN, PEY, CADIQ };
const char hundreds_letter[9] = { QUF, REJ, JIN, TAV };

void num_to_heb (int num, StringTempR heb) {
	int n0 = num%10;           // units
	int n01 = num%100;         // units+tens
	int n1 = (num%100)/10;     // tens
	int n2 = (num%1000)/100;   // hundreds

	// sifrat ha-meot
	if (1<=n2&&n2<=4)   {
		heb.append(hundreds_letter[n2-1]);
	}
	else if (5<=n2&&n2<=8) {  
		heb.append(hundreds_letter[n2-5]);
		heb.append(TAV); 
	}
	else if (9<=n2) { 
		heb.append(hundreds_letter[n2-9]);
		heb.append(TAV); 
		heb.append(TAV); 
	}

	// sifrat ha-axadot w-sifrat ha-asarot
	if (n01==15 || n01==16) { 
		heb.append(TET);  
		heb.append(units_letter[n0]);  
	}
	else {
		if (1<=n1)   heb.append(tens_letter[n1-1]);
		if (1<=n0)   heb.append(units_letter[n0-1]);
	}
}

StringTemp num_to_heb (int num, Size thesize)
{
	StringTemp heb(thesize);
	num_to_heb (num,heb);
	return heb;
}



/****************** heb to num *******************************************/

int hebrew_value[27] =  {
   1,2,3,4,5,6,7,8,9, 10,20,20,30,40,40,50,50,60,70,80,80,90,90, 100,200,300,400
};

int heb_to_num (char* heb) {
	int sum;
	for (sum=0; *heb!=0; ++heb)
		if (is_heb(*heb))
			sum+=hebrew_value[*heb-ALEPH];
	return sum;
}


/****************** ivrit btatiq ltini *************************************/

const char _ivrit_ltatiq_ltini[] = "ABGDHWZX@IKKLMMNNS&PPCCQR$T";

char ot_ivrit_ltatiq_ltini (uchar ot_ivrit) {
	if (is_heb(ot_ivrit) )
		return _ivrit_ltatiq_ltini[ot_ivrit - ALEPH];
	else
		return ot_ivrit;
}

uchar ot_ivrit_mtatiq_ltini (char ot_btatiq) {
	Index i = charindex(toupper(ot_btatiq), _ivrit_ltatiq_ltini);
	if (i<MSPR_OTIOT_BIVRIT)
		return ALEPH + i;
	else
		return ot_btatiq;
}




/****************** heb input *************************************/
/*
char key_to_heb [] = {
   32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,'š', 45,'•','.',
   48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,'“', 60, 61, 62, 63,
   64,'™','','','‚','—','‹','’','‰','','‡','Œ','Š','–','Ž','',
  '”','/','˜','ƒ','€','…','„',',','‘','ˆ','†',91 ,92 ,93 ,94 ,95 };

void print_reversed (CStr thestr, Size thelen) {
	for (Index i=0; i<=thelen; i++) {
		gotoxy (79-i,wherey());
		putchar (thestr[thelen-i]);
	}
}

inline void print_reversed (StringTempCR thestr) { print_reversed(thestr.str,thestr.len); }
*/
/* 
StringTemp cgetsheb (int maxlen, CStr first_string)
{
	StringTemp heb (maxlen);
begin:
	heb.overwrite(first_string);
	char c;
	for (;;) {        // loop until ESC or \ or EOL is pressed
		print_reversed (heb);
		c=toupper(getch());
		if (c=='\n'||c=='\r') break;
		if (c=='\\'||c==ESC)  goto begin;  // cancel string
		if (c=='\b') {					 // backspace
			heb.backspace();
			gotoxy (wherex()+1,wherey());
			putchar (' ');
			gotoxy (wherex()-1,wherey());
			continue;
		}
		if (32<=c && c<=95)      c = key_to_heb[c-32];
		putchar (c);
		gotoxy (wherex()-2,wherey());
		heb.append(c);
	}
	return heb;
}
*/


const char thiliot[] = { KAF, MEM, NUN, PEY, CADIQ, 0 };
const char sofiot[] = { KAFSOFIT, MEMSOFIT, NUNSOFIT, PEYSOFIT, CADIQSOFIT, 0 };

char to_sofit   (char ot)
{
	if (is_thilit(ot)) return ot-1;
	else return ot;
}


char to_thilit	(char ot)
{
	if (is_sofit(ot)) return ot+1;
	else return ot;
}


#ifdef TEST_IVRIT

#include <iostream.h>

void main (void)
{

/* Test 1 */
  StringTemp heb;
  char* hebstr;
  cout << endl;
  cout << num_to_heb(1)  << ' '; //cout << heb_to_num(heb) << ' ';
  hebstr = num_to_heb(10).finalstr();
  cout << hebstr << ' '; cout << heb_to_num(hebstr) << ' ';
  cout << num_to_heb(11) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(15) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(20) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(22) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(100) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(103) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(116) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(120) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(121) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(500) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(504) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << num_to_heb(530) << ' '; //cout << heb_to_num(heb) << ' ';
  cout << (hebstr=num_to_heb(595).finalstr()) << ' '; cout << heb_to_num(hebstr) << ' ';
  cout << (hebstr=num_to_heb(920).finalstr()) << ' '; cout << heb_to_num(hebstr) << ' ';

//  cout << "Enter your name in Hebrew: "; cout.flush();
//  cout << endl << "Your name is " << cgetsheb (30) << endl;
}

#endif

#ifdef TRGM_TATIQ

#include <fstream.h>

void main(int argc, char* argv[]) {
	if (argc!=3) {
		exit_with_error ("wrong number of arguments", "", "trgm_tatiq qlt plt");
	}
	ifstream qlt(argv[1]); 
	ofstream plt(argv[2]);
	char tw;
	while (!qlt.eof()) {
		tw = qlt.get();
		plt << to_thilit(ot_ivrit_mtatiq_ltini(tw));
	}
}
#endif

