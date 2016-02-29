/*
 * File morph/addlex.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/addlex.cpp
 */

/* addlex.cpp -- generate lexicon */

#include <morph/addlex.h>
#include <morph/lexicon.h>
#include <morph/morphopt.h>
#include <morph/morphtav.h>
#include <cpplib/stringt.h>

/*************************************************************************************/
/********     Inverse morphological analysis           *******************************/
/*************************************************************************************/

	/************* sug-ecem ***************************************/

	uint sug_ecem (StringTemp theword, MorphInfoCR ref_info) {
		StringTemp thebase(30);
		Index i;  LOOP_BASE_MATCHING_TAVNIOT(i,tavniot_ribui_ecem, theword.str,thebase, ref_info) 
			if ( identical_smikut(curtavnit.smikut(),ref_info.smikut()) )
				return curtavnit.sug(); 
		}
		return 0;
	}


  
    /************ sug-poal-beinony *****************************************************/

	uint sug_beinony (StringTempCR theword, MorphInfoCR ref_info) {
		uint sug_beinony_bli_siomot (StringTempCR theword, MorphInfoCR ref_info);
		static StringTemp myword1 (30);
		uint oldsug=0, newsug=0;
		Index i;  LOOP_MATCHING_TAVNIOT(i,tavniot_toar, theword.str,myword1) 
			if ( ends_with(myword1,'H') && !ends_with(curtavnit,'H') ) continue;  // if it ends with 'H' -- it can't have siomet jayakut other than sug-7.
			if ( !identical_meen(curtavnit.meen(),ref_info.meen()) || !identical_mispar(curtavnit.mispar(),ref_info.mispar()) ) continue;
			oldsug=newsug; newsug=sug_beinony_bli_siomot(myword1,ref_info);
			if (newsug!=0 && oldsug!=0) return 0;      // ambiguity -- can't learn!
			if (newsug==0 && oldsug!=0) swap (newsug,oldsug);
		}
		return newsug;//!=0? newsug: 15;
	}


	uint sug_beinony_bli_siomot (StringTempCR theword, MorphInfoCR ref_info) {
		static StringTemp myword2 (30);
		uint oldsug=0, newsug=0;
		Index i;  LOOP_BASE_MATCHING_TAVNIOT(i,tavniot_beinony, theword.str,myword2, ref_info) 
			if ( !identical_zman (curtavnit.zman(),ref_info.zman()) ) continue;
			oldsug=newsug;  newsug=curtavnit.sug();
			if (newsug!=0 && oldsug!=0) return 0;      // ambiguity -- can't learn!
			if (newsug==0 && oldsug!=0) swap (newsug,oldsug);
		}
		return newsug;
	}



    /************ sug-poal-atid *****************************************************/

	uint sug_poal_atid (StringTempCR theword, MorphInfoCR ref_info) {
		uint sug_txiliot_poal_atid (uint, StringTempCR, MorphInfoCR);
		uint oldsug=0, newsug=0;
		for (Index binyan=0; binyan<NUM_OF_BINYANIM; binyan++)  {
			oldsug=newsug;	newsug=sug_txiliot_poal_atid (binyan,theword,ref_info);
			if (newsug!=0 && oldsug!=0) {    // ambiguity -- can't learn!
				if (newsug==13) return oldsug;
				if (oldsug==13) return newsug;
				return 0;
			}
			if (newsug==0 && oldsug!=0) swap (newsug,oldsug);
		}
		return newsug;//!=0? newsug: 99;
	}

	uint sug_txiliot_poal_atid (uint binyan, StringTempCR theword, MorphInfoCR ref_info) {
		static StringTemp myword1 (30);
		uint sug_hemjeky_poal_atid (TavniotR, StringTempCR, MorphInfoCR);
		uint oldsug=0, newsug=0;
		Index i;  LOOP_MATCHING_TAVNIOT(i,txiliot_poal_atid[binyan], theword.str,myword1) 
			Guf guftav = curtavnit.guf();  Zman zmantav = curtavnit.zman();
			if ( zmantav!=ref_info.zman() ) continue;
			oldsug=newsug;
			if ( zmantav == MAQOR ) 
				newsug = sug_hemjeky_poal_atid (hemjeky_poal_atid_ZY[binyan],myword1,ref_info);
			else if (guftav==GUF1) {
				if ( guftav!=ref_info.guf() ) continue;
				if ( curtavnit.mispar()!=ref_info.mispar() ) continue;
				newsug = sug_hemjeky_poal_atid (hemjeky_poal_atid_ZY[binyan],myword1,ref_info);
			}
			else if (guftav==GUF3) {
				if ( guftav!=ref_info.guf() ) continue;
				if ( curtavnit.meen()!=ref_info.meen() ) continue;
				if (ref_info.meen()==ZAKAR && ref_info.mispar()==YAXID)
					newsug = sug_hemjeky_poal_atid (hemjeky_poal_atid_ZY[binyan],myword1,ref_info);
				if (ref_info.meen()==ZAKAR && ref_info.mispar()==RABIM)
					newsug = sug_hemjeky_poal_atid (hemjeky_poal_atid_ZR[binyan],myword1,ref_info);
			}
			else {
				if (ref_info.meen()==ZAKAR && ref_info.mispar()==YAXID)
					newsug = sug_hemjeky_poal_atid (hemjeky_poal_atid_ZY[binyan],myword1,ref_info);
				else if (ref_info.meen()==NQEVA && ref_info.mispar()==YAXID && ref_info.guf()==GUF3)
					newsug = sug_hemjeky_poal_atid (hemjeky_poal_atid_ZY[binyan],myword1,ref_info);
				else if (ref_info.meen()==ZAKAR && ref_info.mispar()==RABIM)
					newsug = sug_hemjeky_poal_atid (hemjeky_poal_atid_ZR[binyan],myword1,ref_info);
				else if (ref_info.meen()==NQEVA && ref_info.mispar()==YAXID)
					newsug = sug_hemjeky_poal_atid (hemjeky_poal_atid_NY[binyan],myword1,ref_info);
				else if (ref_info.meen()==NQEVA && ref_info.mispar()==RABIM)
					newsug = sug_hemjeky_poal_atid (hemjeky_poal_atid_NR[binyan],myword1,ref_info);
			}
			if (newsug!=0 && oldsug!=0) return 0;      // ambiguity -- can't learn!
			if (newsug==0 && oldsug!=0) swap (newsug,oldsug);
		}	
		return newsug;
	}


	uint sug_hemjeky_poal_atid (TavniotR thetavniot, StringTempCR theword, MorphInfoCR ref_info) {
		static StringTemp myword2 (30);
		uint oldsug=0, newsug=0;
		Index j; LOOP_BASE_MATCHING_TAVNIOT(j,thetavniot,theword.str,myword2, ref_info)
			if ( ends_with(myword2,'H') && !ends_with(curtavnit,'H') )  continue;
			oldsug=newsug;  newsug=curtavnit.sug();
			if (newsug!=0 && oldsug!=0) return 0;      // ambiguity -- can't learn!
			if (newsug==0 && oldsug!=0) swap (newsug,oldsug);
		}
		return newsug;
	}


/*
status get_lexical_info_from_morph_info (StringTempCR thefullword, MorphInfoCR morphinfo, LexicalInfo& lexinfo) {
	lexinfo.heleqdiber = morphinfo.heleqdiber();
	lexinfo.sugtavnit = lexinfo.sug2 = 0;       // the default;
	switch (morphinfo.heleqdiber()) {
	case JEM_PRATI:
		lexinfo.sug2 = morphinfo.meen();
		break;
	case ECEM:
		lexinfo.sug2 = morphinfo.meen();
		if (morphinfo.mispar()!=RABIM)  break;      // nothing to learn
		if (morphinfo.guf_siomet()!=NONE)  break;  
		if (morphinfo.smikut()==NISMAK) break;     
		lexinfo.sugtavnit = sug_ecem(thefullword.str,morphinfo);
		break;
	case TOAR:
		break;
	case POAL:
		switch (morphinfo.zman()) {
		case HOWE: case HOWE_SAVIL:
			lexinfo.sugtavnit = sug_beinony (thefullword, morphinfo);
			break;
		case ATID: case CIWUI:
			lexinfo.sug2 = sug_poal_atid (thefullword, morphinfo);
			break;
		}	
		break;
	}
	return OK;
}

status get_txiliot_from_morph_info (StringTempCR thefullword, MorphInfoR morphinfo) {
	if (charinstring(thefullword[0],"W$"))  {
		if ( identical(morphinfo.baseword(),thefullword.str+1) ) {
			morphinfo.settxiliot(1);
			return OK;
		}
	}
	if (thefullword[0]=='W' && thefullword[1]=='$')  {
		if ( identical(morphinfo.baseword(),thefullword.str+2) ) {
			morphinfo.settxiliot(2);
			return OK;
		}
	}
	morphinfo.settxiliot(0);
	return OK;
}
*/

status extend_lexicon (CStr thefullword, MorphInfo theanalysis) {
	/* this function should extend the lexicon so that it will know that 'the-analysis' matches 'the-fullword'. */
	HeleqDiber hd = theanalysis.heleqdiber();
	CStr thebaseword = theanalysis.baseword();
	thefullword = theanalysis.fullword_bli_txiliot(thefullword);
	theanalysis.settxiliot(false,NO_JIABUD,NO_OY,LOMEYUDA);
	uint sug1, sug2;
	switch (hd) {
	case MILAT_YAXAS: case MILAT_GUF: case MISPAR: case POAL_EZER:
		if (thefullword[0]==theanalysis.baseword()[0])
			DOr(add_milit_to_lexicon(thefullword,theanalysis));
		break;
	case ECEM:
		if (theanalysis.mispar()==RABIM &&    
			!theanalysis.has_siomet() && 
			theanalysis.smikut()!=NISMAK		) 
			sug1 = sug_ecem(thefullword,theanalysis);
		else
			sug1 = 0;
		add_ecem_to_lexicon (thebaseword,sug1,theanalysis.meen());
		break;
	case TOAR:
		add_toar_to_lexicon (thebaseword);
		break;
	case POAL:
		switch (theanalysis.zman()) {
		case AVAR: 
			add_poal_to_lexicon (thebaseword);
			break;
		case HOWE:  case HOWE_SAVIL:
			sug1 = sug_beinony (thefullword, theanalysis);
			add_poal_1_to_lexicon (thebaseword,sug1);
			break;
		case ATID: case CIWUI: //case MAQOR:
			sug2 = sug_poal_atid (thefullword, theanalysis);
			add_poal_2_to_lexicon (thebaseword,sug2);
			break;
		case MAQOR:
			sug2 = sug_poal_atid(thefullword,theanalysis);
			uint sug1p, sug2p;
			if (lexicon_contains_poal(thebaseword,&sug1p,&sug2p)) {
				if (sug2p==0) 
					add_poal_2_to_lexicon (thebaseword,sug2);
				else if (sug2p!=sug2)
					add_miuxd_to_lexicon (thefullword,theanalysis);
			}
			else 
				add_poal_2_to_lexicon (thebaseword,sug2);
			break;
		}
		break;
	case JEM_PRATI:
		DOr (add_jem_prati_to_lexicon (thebaseword, theanalysis.meen() ));
		break;
	default:
		DOr (add_word_to_lexicon(thebaseword,hd));
	}
	return OK;
}	

