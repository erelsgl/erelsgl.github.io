/*
 * File morph/similar.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/similar.cpp
 */

/* similar.cpp */

#include <morph/similar.h>


void bdoq_dimion (MorphInfoBasicCR a, MorphInfoBasicCR b,  Dimion& thedimion) {
	if (a.heleqdiber()!=b.heleqdiber())  { thedimion=0; return; } 
	switch (a.heleqdiber()) {
	case ECEM: thedimion = 
		(1 + (a.yidua()==b.yidua())) *
		(1 + (a.mispar()==b.mispar())) *
		(1 + (a.smikut()==b.smikut())) *
		(1 + (a.jiabud()==b.jiabud())) *
		(1 + (a.otiotyaxas()==b.otiotyaxas())) * 
		(1 + (a.guf_siomet()==b.guf_siomet()));
		break;
	case TOAR: thedimion = 
		(1 + (a.yidua()==b.yidua())) *
		(1 + (a.smikut()==b.smikut())) * 
		(1 + 3*(a.jiabud()==b.jiabud())) *
		(1 + 3*(a.otiotyaxas()==b.otiotyaxas()));
		break;
	case MISPAR: thedimion =
		(1 + (a.yidua()==b.yidua())) *
		(1 + (a.mispar()==b.mispar())) * 
		(1 + (a.guf_siomet()==b.guf_siomet())) * 
		(1 + (a.smikut()==b.smikut())) * 
		(1 + (a.jiabud()==b.jiabud())) *
		(1 + (a.otiotyaxas()==b.otiotyaxas()));
		break;
	case POAL: case POAL_EZER: 
		if (a.beinony()||b.beinony()) thedimion =
			(1 + 3*(a.zman()==b.zman())) *
			(1 + 3*(a.guf()==b.guf())) * 
			(1 + 3*(a.jiabud()==b.jiabud()));
		else thedimion =
			(1 + 7*(a.zman()==b.zman())) * 
			(1 + 7*(a.guf()==b.guf()));
		break;
	case MILAT_YAXAS: thedimion =
		(1 + 7*(a.jiabud()==b.jiabud())) * 
		(1 + 7*(a.guf_siomet()==b.guf_siomet()));
		break;
	case MILAT_GUF: thedimion =
		(1 + (a.yidua()==b.yidua())) *
		(1 + 3*(a.guf()==b.guf())) * 
		(1 + 3*(a.jiabud()==b.jiabud())) *
		(1 + (a.otiotyaxas()==b.otiotyaxas()));
		break;
	case JEM_PRATI: thedimion = 
		(1 + 7*(a.otiotyaxas()==b.otiotyaxas())) * 
		(1 + 7*(a.jiabud()==b.jiabud()));
		break;
	default: thedimion = 
		1+63*(a.jiabud()==b.jiabud());
		break;
	}
	if (thedimion==1) thedimion=0;
}


void bdoq_dimion_hatama (MorphInfoBasicCR a, MorphInfoBasicCR b,  Dimion& thedimion) {
	if (a.heleqdiber()!=b.heleqdiber())  { thedimion=0; return; } 
	switch (a.heleqdiber()) {
	case ECEM: case TOAR: case MISPAR: thedimion = 
		(1 + 3*(a.meyuda()==b.meyuda())) *
		(1 + 3*(identical_meen(a.meen(),b.meen()))) * 
		(1 + 3*(identical_mispar(a.mispar(),b.mispar())));
		break;
	case POAL: case POAL_EZER: case MILAT_GUF: thedimion = 
		(1 + 3*(identical_guf(a.guf(),b.guf()))) *
		(1 + 3*(identical_meen(a.meen(),b.meen()))) * 
		(1 + 3*(identical_mispar(a.mispar(),b.mispar())));
		break;
	case MILAT_YAXAS: thedimion =
		(1 + 3*(identical_guf(a.guf_siomet(),b.guf_siomet()))) *
		(1 + 3*(identical_meen(a.meen_siomet(),b.meen_siomet()))) * 
		(1 + 3*(identical_mispar(a.mispar_siomet(),b.mispar_siomet())));
		break;
	case JEM_PRATI: thedimion = 
		(1 + 63*(identical_meen(a.meen(),b.meen())));
		break;
	default: thedimion = ZEHUT; 
		break;
	}
	if (thedimion==1) thedimion=0;
}


void bdoq_dimion_sof (MorphInfoBasicCR a, MorphInfoBasicCR b,  Dimion& thedimion) {
	if (a.heleqdiber()!=b.heleqdiber())  { thedimion=0; return; } 
	if (!identical_smikut(a.smikut(),b.smikut()))  { thedimion=0; return; } 
	if (a.has_siomet() != b.has_siomet())  { thedimion=0; return; } 
	switch (a.heleqdiber()) {
	case ECEM: case MISPAR: thedimion = 4 * 
		(1 + 3*(a.meyuda()==b.meyuda())) *
		(1 + 3*(a.guf_siomet()==b.guf_siomet()));
		break;
	case TOAR: thedimion = 8 *
		(1 + 7*(a.meyuda()==b.meyuda()));
		break;
	case POAL: case POAL_EZER: thedimion =
		(1 + 7*(a.zman()==b.zman())) * 
		(1 + 7*(a.guf()==b.guf()));
		break;
	case MILAT_YAXAS: thedimion = 8 *
		(1 + 7*(a.guf_siomet()==b.guf_siomet()));
		break;
	case MILAT_GUF: thedimion = 8 *
		(1 + 7*(a.guf()==b.guf()));
		break;
	default: thedimion = 64;
		break;
	}
	if (thedimion==1) thedimion=0;
}


void bdoq_dimion_hatama_sof (MorphInfoBasicCR a, MorphInfoBasicCR b,  Dimion& thedimion) {
	if (a.heleqdiber()!=b.heleqdiber())  { thedimion=0; return; } 
	if (!identical_smikut(a.smikut(),b.smikut()))  { thedimion=0; return; } 
	if (a.has_siomet() != b.has_siomet())  { thedimion=0; return; } 
	switch (a.heleqdiber()) {
	case ECEM: case TOAR: case MISPAR: thedimion = 
		(1 + 3*(a.meyuda()==b.meyuda())) *
		(1 + 3*(identical_meen(a.meen(),b.meen()))) * 
		(1 + 3*(identical_mispar(a.mispar(),b.mispar())));
		break;
	case POAL: case POAL_EZER: case MILAT_GUF: thedimion = 
		(1 + 3*(identical_guf(a.guf(),b.guf()))) *
		(1 + 3*(identical_meen(a.meen(),b.meen()))) * 
		(1 + 3*(identical_mispar(a.mispar(),b.mispar())));
		break;
	case MILAT_YAXAS: thedimion =
		(1 + 3*(identical_guf(a.guf_siomet(),b.guf_siomet()))) *
		(1 + 3*(identical_meen(a.meen_siomet(),b.meen_siomet()))) * 
		(1 + 3*(identical_mispar(a.mispar_siomet(),b.mispar_siomet())));
		break;
	case JEM_PRATI: thedimion = 
		(1 + 63*(identical_meen(a.meen(),b.meen())));
		break;
	default: thedimion = ZEHUT; 
		break;
	}
	if (thedimion==1) thedimion=0;
}






void bdoq_dimion_baseword (MorphInfoCR a, MorphInfoCR b,  Dimion& thedimion) {
	thedimion =	ZEHUT*(a.heleqdiber()==b.heleqdiber() && 
				identical(a.baseword(),b.baseword())) ;
}

void bdoq_dimion (MorphInfoCR a, MorphInfoCR b, Dimion& sbw, Dimion& sast) {
	bdoq_dimion (a,b,sast);
	bdoq_dimion_baseword (a,b,sbw);
}



Dimion dimion (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	Dimion d;  bdoq_dimion (a,b,d);  return d; }

Dimion dimion_hatama (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	Dimion d;  bdoq_dimion_hatama (a,b,d);  return d; }

Dimion dimion_sof (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	Dimion d;  bdoq_dimion_sof (a,b,d);  return d; }

Dimion dimion_hatama_sof (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	Dimion d;  bdoq_dimion_hatama_sof (a,b,d);  return d; }


bool are_SA_ST (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	return dimion(a,b)==ZEHUT; }


bool are_SBW (MorphInfoCR a, MorphInfoCR b) {
	return 	a.heleqdiber()==b.heleqdiber() && 
			identical(a.baseword(),b.baseword());
}

bool are_SBW_SA_ST (MorphInfoCR a, MorphInfoCR b) {
	if ( !identical ( a.baseword(), b.baseword() ) ) return false;  // milat basis lo^ zeha
	else return are_SA_ST ( a , b );
}





uint mispar_milim_domot (MorphInfoBasicCR a) {
	HeleqDiber hd = a.heleqdiber();
	int kofl_min_mspr;
	switch (hd) {
	case ECEM: case MISPAR:   // multiply by 2 for {ZKR, NQVA}
		switch ((a.jiabud()!=NONE) + (a.otiotyaxas()!=NONE) + (a.has_siomet())) {
		case 0: return 2 * (1*8 + 3*4 + 3*2 + 1*1) * 8; ;  
		case 1: return 2 * (1*16 + 4*8 + 6*4 + 4*2 + 1*1) * 4; 
		case 2: return 2 * (1*32 + 5*16 + 10*8 + 10*4 + 5*2 + 1*1) * 2; 
		case 3: return 2 * (1*64 + 6*32 + 15*16 + 20*8 + 15*4 + 6*2 + 1*1) * 1; 
		}
	case TOAR:  // multiply by 4 for {ZKR, NQVA} * {YXID, RBIM}
		switch ((a.jiabud()!=NONE) + (a.otiotyaxas()!=NONE)) {
		case 0: return 4 * (1*4 + 2*2 + 1*1) * 16; ;  
		case 1: return 4 * (1*16 + 2*8+1*4 + 1*4+2*2 + 1*1) * 4; 
		case 2: return 4 * (1*64 + 2*32+2*16 + 1*16+4*8+1*4 + 2*4+2*2 + 1*1) * 1; 
		}
	case POAL: case POAL_EZER:  // multiply by 4 for {ZKR, NQVA} * {YXID, RBIM}
		kofl_min_mspr = 
			a.guf()==NONE? 1:
			a.guf()==GUF1? 2:  // multiply by 2 for {YXID, RBIM}
			4; // multiply by 4 for {ZKR, NQVA} * {YXID, RBIM}
		if (a.beinony()) return kofl_min_mspr * (1*64 + 3*16 + 3*4 + 1*1); 
		else return kofl_min_mspr * (1*64 + 2*8 + 1*1);
	case MILAT_YAXAS: 
		kofl_min_mspr =
			!a.has_siomet()? 1:
			a.guf_siomet()==GUF1? 2:  // multiply by 2 for {YXID, RBIM}
			4; // multiply by 4 for {ZKR, NQVA} * {YXID, RBIM}
		return kofl_min_mspr * (1*64 + 2*8 + 1*1); 
	case MILAT_GUF:
		kofl_min_mspr = 
			a.guf()==NONE? 1:
			a.guf()==GUF1? 2:  // multiply by 2 for {YXID, RBIM}
			4; // multiply by 4 for {ZKR, NQVA} * {YXID, RBIM}
		return kofl_min_mspr * (1*64 + 2*32+2*16 + 1*16+4*8+1*4 + 2*4+2*2 + 1*1);
	default: 
		return 1;
	}
}

#ifdef TEST_SIMILAR

#include <morph/bwcnt.h>
#include <morph/morphanl.h>
#include <morph/anlcnt.h>
#include <morph/lexicon.h>
#include <cpplib/ui.h>

static bool sof;

BasewordCounter the_baseword_counter;
AnalysisCounter the_analysis_counter;

static StringTemp input_word_to_analyze (CStr prompt, MorphOptions& theoptions) {
		StringTemp thefullword = input_string (prompt, 12);
		if (thefullword[0]=='!') {
			lexicon_is_creative = !lexicon_is_creative;
			cout << "lexicon is " << yesno_str(lexicon_is_creative) << "creative" << endl;
		}
		if (thefullword[0]=='~') sof=true;
		if (thefullword[0]=='0')  {
			the_baseword_counter.zero_all();
			the_analysis_counter.zero_all();
			writeln (cout,the_baseword_counter,Format("L"));
			writeln (cout,the_analysis_counter,Format("L  "));
		}
		natax (thefullword.str, theoptions);
		cout << theoptions << endl;
		return thefullword;
}


void main (void) 
{
	MorphOptions theoptions, theoptions2;
	initialize_the_analyzer ("..\\..\\..\\harc","..\\..\\..\\harc");
	for (sof=false; !sof;) {
		StringTemp thefullword1 = input_word_to_analyze ("KTWB MILH LNITUX (~ LSIUM)", theoptions);
		StringTemp thefullword2 = input_word_to_analyze ("KTWB MILH DWMH (~ LSIUM)", theoptions2);
		for (Index i=0; i<theoptions.count(); ++i) {
			MorphInfoCR option1 = theoptions[i];
	//		the_baseword_counter.insert (option1);
	//		the_analysis_counter.insert (option1);
	//		writeln (cout,the_baseword_counter,Format("L"));
	//		writeln (cout,the_analysis_counter,Format("L  "));
			for (Index j=0; j<theoptions2.count(); j++) {
				Dimion sbw,sast;
				bdoq_dimion (option1,theoptions2[j],sbw,sast);
				if (sbw>0||sast>0) {
					cout << option1 << " ~~ " << theoptions2[j] << " :";
					cout << " SBW " << sbw
						 << " SAST " << sast
						 << endl;
				}
			}
		}
	}
}


#endif
