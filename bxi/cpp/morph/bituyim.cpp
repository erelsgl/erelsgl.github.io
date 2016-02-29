/*
 * File morph/bituyim.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/bituyim.cpp
 */

/* bituyim.cpp */

#include <math.h>

#include <morph/morphanl.h>
#include <morph/hpmaio.h>
#include <morph/corpus.h>
#include <morph/alghist.h>

#include <morph/bituy.h>
#include <morph/hajlamot.h>

#include <cpplib/countc.hxx>
#include <cpplib/map.hxx>
#include <cpplib/hashtab.hxx>
#include <cpplib/trie0.hxx>



bool siman_pisuq (MorphInfoCR a) {
	if (strlen(a.baseword())>1) return false;
	char c = a.baseword()[0];
	return !contains("ABGDHWZX@IKLMNS&PCQR$T",c);
}


Corpus thetext;


/***************************************************************************/
/***********        Bi@uyim                                         ********/
/***************************************************************************/



/*
Mone<Bituy> mone_bituyim_bw_bw;
Mone<Bituy> mone_bituyim_bw_txiliot;
MoneMilimAxreiMila mone_mm;

void sfor_bituyim (ArrayOfMOWS& the_sentence_options) {
				Sikui the_sikui = product (options_x.sikui(ox), options_y.sikui(oy), 1);		
				mone_bituyim_bw_bw.add (Bituy_bw_bw(infox,infoy), the_sikui);
				mone_bituyim_bw_txiliot.add (Bituy_bw_txiliot(infox,infoy), the_sikui);
				mone_mm.add (infox,infoy,the_sikui);
			} }
	}
}	
*/


#define Mone CounterHashTable

Mone<Bituy> mone_bituyei_poal;
Mone<Bituy> mone_bituyei_ecem;
Mone<Bituy> mone_bituyei_smikut;
Mone<Bituy> mone_bituyim_bw;
Mone<Bituy> mone_bituyim_hd;




uint ms_maamarim;


void sfor_bituy (MorphInfoCR x, MorphInfoCR y, Sikui hasikui) {
	MorphInfo yy = y.mila_rijona_info();
	HeleqDiber xhd=x.heleqdiber(), yhd=yy.heleqdiber();
	if (xhd==TOAR_POAL&&(yhd==POAL||yhd==POAL_EZER)) return;
	if (yhd==TOAR_POAL&&(xhd==POAL||xhd==POAL_EZER)) return;
	if (xhd==ECEM && yhd==TOAR) return;       
	if (xhd==MILAT_YAXAS && yhd==ECEM) return;       
	if (xhd==JEM_PRATI) return;
	Bituy b;
	b.x().set_baseword(x);
	if (x.smikut()==NISMAK&&y.can_be_somek()) {
		b.y().set_baseword(yy);
		mone_bituyei_smikut.add(b,hasikui);		
	}
	else if (yhd==MILAT_YAXAS||yhd==MILAT_XIBUR) {
		b.y().set_baseword(yy);
		if (xhd==POAL)	mone_bituyei_poal.add(b,hasikui);
		else if (xhd==ECEM)	mone_bituyei_ecem.add(b,hasikui);
		else mone_bituyim_bw.add(b,hasikui);
	}
	else {
		b.y().set_heleqdiber(yhd);
		if (xhd==POAL)	mone_bituyei_poal.add(b,hasikui);
		else if (xhd==ECEM)	mone_bituyei_ecem.add(b,hasikui);
		else mone_bituyim_hd.add(b,hasikui);
	}
}


void sfor_bituyim() {
	for(;;) {
		DOEOFx(thetext.qra_qelet(input,ms_maamarim));
		thetext.natax_qelet_1();
		for (Index w=0; w+1<thetext.kol_hanituxim_laqelet.count(); ++w) {
			if (w%100==0) cerr<<w<<" ";
			MorphOptionsWithSikuiCR kol_hanituxim_lx = thetext.kol_hanituxim_laqelet[w];
			MorphOptionsWithSikuiCR kol_hanituxim_ly = thetext.kol_hanituxim_laqelet[w+1];
			LOOPVECTOR (;,kol_hanituxim_lx,ox) { MorphInfo x=kol_hanituxim_lx.info(ox);  if (siman_pisuq(x)) continue;
				LOOPVECTOR (;,kol_hanituxim_ly,oy) { MorphInfo y=kol_hanituxim_ly.info(oy);  if (siman_pisuq(y)) continue;
					//sfor_bituy (x,y, product(kol_hanituxim_lx.sikui(ox),kol_hanituxim_ly.sikui(oy) ) );
					sfor_hajlama (x,y, product(kol_hanituxim_lx.sikui(ox),kol_hanituxim_ly.sikui(oy) ) );
				}
			}
		}
	}
}


void ktov_bituyim (Mone<Bituy>& habituyim, Sikui hasaf) {
	for (Mone<Bituy>::Cursor c(habituyim); c.isvalid(); ++c) {
		Sikui the_count = c.count();
		Bituy the_bituy = c.data();
		if (the_count >= hasaf ) 
			bituyimlogfile <<  "  " << the_bituy << " " << the_count << endl;
	}
}
/*
void ktov_hajlamot (MoneHajlamot& hahajlamot, Sikui hasaf) {
	for (MoneHajlamot::Cursor c1(hahajlamot); c1.isvalid(); ++c1) {
		bituyimlogfile << "  " << c1.key() << ": " << c1.data().sikui_kolel << endl;
		for (MoneHajlamot1::Cursor c2(c1.data());  c2.isvalid(); ++c2) {
			if (c2.count() >= hasaf) 
				bituyimlogfile << "    " << c2.data() << " " << c2.count() << endl;
		}
	}
}
*/

void ktov_bituyim () {
	open_bituyimlogfile(log_path);
	bituyimlogfile << "bi@uyei po&l:" << endl;
	ktov_bituyim (mone_bituyei_poal,sikui(2));
	bituyimlogfile << endl << "bi@uyei &ecm:" << endl;
	ktov_bituyim (mone_bituyei_ecem,sikui(2));
	bituyimlogfile << endl << "bi@uyei smikut:" << endl;
	ktov_bituyim (mone_bituyei_smikut,sikui(2));
	bituyimlogfile << endl << "bi@uyim bw:" << endl;
	ktov_bituyim (mone_bituyim_bw,sikui(2));
	bituyimlogfile << endl << "bi@uyim hd:" << endl;
	ktov_bituyim (mone_bituyim_hd,sikui(2));
	bituyimlogfile << endl << "ha$lamot:" << endl;
	ktov_hajlamot(mone_hajlamot_pealim,sikui(2));
	close_bituyimlogfile();
}
 


#undef Mone




/***************************************************************************/
/***********        The Main Program                                ********/
/***************************************************************************/



void read_the_command_line(int argc, char* argv[]) {
	parse_command_line (argc,argv,1,1,"gzom","st");
	log_path = option('g')==NULL? ".": option('g');
	ms_maamarim = option('m')==NULL? 1: option_as_int('m',0,100);
}

void atxel (CStr corpus, CStr correctoutput) {
	if (swtch('t'))
		atxel_global_database (NULL, filename(corpus,"nt1").str );
	else if (swtch('s'))
		atxel_global_database (NULL, filename(corpus,"nts").str );
	else {      
		atxel_global_database (filename(corpus,"txt").str,NULL);
		ktov_global_database (filename(corpus,"nt0").str); 
		if (correctoutput)       // read correct analysis file for corpus
			qra_nitux_nakon_global (filename(corpus,"txt").str, filename(correctoutput,"to").str);
		open_logfile(2);  xajev_sikuiim_lkol_hamilim_global();  logfile.close();
		ktov_global_database (filename(corpus,correctoutput? "nt1": "nts").str);
	}
}





void main (int argc, char* argv[]) {
	read_the_command_line(argc, argv);
	initialize_the_analyzer(log_path,log_path);
	atxel (variable(0),option('o'));
	thetext.atxel(6000);
	open_infile_with_messages (filename(variable(0),"txt").str, input);
	sfor_bituyim();
	//qra_hajlamot(log_path);
	ktov_bituyim();
	ktov_hajlamot(log_path);
}
