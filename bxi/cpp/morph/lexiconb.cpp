/*
 * File morph/lexiconb.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/lexiconb.cpp
 */

/* lexiconb.cpp -- lexicon-bi@utyim */

//#include <morph/lexiconb.h>
#include <morph/cimcumim.h>
#include <cpplib/trie0.hxx>

/********************************************************************/
/*************     Bituy, and the lexicon structures  ***************/
/********************************************************************/


StringTemp key1 (MorphInfoCR x, MorphInfoCR y) {
	return concat3 (x.baseword(),' ',hd2char(y.heleqdiber()) );
}

StringTemp key2 (MorphInfoCR x, MorphInfoCR y) {
	return concat3 (x.baseword(),' ',y.baseword());
}

StringTemp key (MorphInfoPatternCR x, MorphInfoPatternCR y) {
	if (y.baseword_xajuv)  return key2(x,y);
	else	return key1(x,y);
}


struct Bituy: public XoqCimcum {
	static SentenceInfo my_mijpat;

	Bituy(Size thesize=2): XoqCimcum(thesize) {}

	StringTemp key() const { return ::key(reija[0],reija[1]); }

	bool match (MorphInfoCR ha_x, MorphInfoCR ha_y) {
		my_mijpat[0] = ha_x;
		my_mijpat[1] = ha_y;
		my_mijpat.truncate(2);
		return XoqCimcum::match (my_mijpat,0);
	}

	bool match (MorphInfoCR ha_x, MorphInfoCR ha_y, MorphInfo& ha_tocaa) {
		my_mijpat[0] = ha_x;
		my_mijpat[1] = ha_y;
		my_mijpat.truncate(2);
		return XoqCimcum::match (my_mijpat,0, ha_tocaa);
	}

	void operator=(int ZERO) { reija.truncate(); }
	bool operator==(int ZERO) const { return reija.isempty(); }
};
typedef const Bituy& BituyCR;


SentenceInfo Bituy::my_mijpat (2);

Trie0<Bituy> lexicon_bituyim;


/********************************************************************/
/*						queries										*/
/********************************************************************/

/*
bool yej_bituy_jematxil_b (MorphInfoCR x) {  
	Bituy b;  
	if (!lexicon_bituyim.contains(x.baseword(),b))  return false;
	else return b.x().match(x);
}
*/

/*
bool yej_bituy (MorphInfoCR x, MorphInfoCR y, Bituy& b) {
	if (lexicon_bituyim.contains( concat_bw(x,y).str, b ) ) 
		return b.match(x,y);
	else if (lexicon_bituyim.contains(x.baseword(), b))  
		return b.match(x,y);
	else return false;
}
*/

bool yej_bituy (MorphInfoCR x, MorphInfoCR y, MorphInfo& tocaa) {
	Bituy b;  
	if (lexicon_bituyim.contains( key2(x,y).str, b ) ) 
		return b.match(x,y,tocaa);
	else if (lexicon_bituyim.contains( key1(x,y).str, b))  
		return b.match(x,y,tocaa);
	else return false;
}


bool yej_bituy (MorphInfoCR x, MorphInfoCR y) {
	MorphInfo t;  
	return yej_bituy (x,y,t);
}



/********************************************************************/
/*						EDITING										*/
/********************************************************************/

status add_bituy (BituyCR b) { 
	return lexicon_bituyim.insert (b.key().str, b); 
}



/********************************************************************/
/*						IO											*/
/********************************************************************/

void ktov_lexicon_bituyim(ostream& out, Format format="") {
	out << '{' << endl;
	for (Trie0<Bituy>::Cursor c(lexicon_bituyim); c.isvalid(); ++c) {
		writeln (out, c.item());
	}
	out << '}' << endl;
}


status qra_lexicon_bituyim (istream& in, Format format="") {
	DOr(testchar(in,'{'));
	for (Index i=0;;++i) {
		Bituy curdata;        
		if (testchar(in,'}')==OK) { return OK; }
		in.ipfx();
		DOr ( read (in, curdata ) );
		if (OK!=add_bituy (curdata)) {
			int i=0;
		}
	}
	DOr(testchar(in,'}'));
	return OK;
}


void ktov_lexicon_bituyim (CStr thepath) {
	ofstream out;
	StringTemp thefilename;
	thefilename = concat_path_to_filename(thepath,"milon/lex20b.ma");
	cerr << "writing lexicon file " << thefilename << endl;
	DOx(open(thefilename.str,out,0));
	out<<"\n%\n%\n% LEXICON BI@UYIM\n%\n%\n";
	writeln(out, lexicon_bituyim, Format("P "));
	out.close();
}

status qra_lexicon_bituyim (CStr thepath) {
	ifstream in;
	StringTemp thefilename;
	thefilename = concat_path_to_filename(thepath, "milon/lex20.ma");
	cerr << "reading lexicon file " << thefilename << endl;
	DOr(open(thefilename.str,in));
	skip_comments(in,'%');
	DOr(qra_lexicon_bituyim(in));
	in.close();
	return OK;
}


#ifdef TEST_LEXICONB

void main (void) {
	qra_lexicon_bituyim ("c:\\winnt\\profiles\\harc");
	ktov_lexicon_bituyim ("c:\\winnt\\profiles\\cpp");
}

#endif
