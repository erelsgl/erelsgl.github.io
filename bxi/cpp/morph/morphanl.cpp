/*
 * File morph/morphanl.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphanl.cpp
 */

/* morphanl.cpp -- morphological analyzer implementation */
/* see the file: morphanl.test.cpp for a demonstration of the morphological analyzer. */

#include <morph/morphanl.h>
#include <morph/lexicon.h>
#include <morph/morphtav.h>
#include <cpplib/stringt.h>



/********************************************************************************/
/*************       global variables             *******************************/
/********************************************************************************/

HeleqDiber default_heleq_diber = JEM_PRATI;	// the heleq-diber of unknown words

bool the_analyzer_is_initialized = false;
bool I_used_the_default_nitux = false;

bool last_analysis_used_the_default() { return I_used_the_default_nitux; }


/********************************************************************************/
/*************       Morph Analyzer               *******************************/
/********************************************************************************/

struct MorphAnalyzer {
	
	MorphOptions* the_final_options_p;        // contains all the possible analyses for the current word
	#define the_final_options (*the_final_options_p)


	MorphAnalyzer () {}

	/*-----------------------------------------------------------------*/
	/*------------------     status variables             -------------*/
	/*-----------------------------------------------------------------*/

	StringTemp myword0, myword1, myword2;
	MorphInfo myinfo1, myinfo2;    

	HeleqDiber hd;
	uint num_of_txiliot;
	bool vav;
	Yidua yidua;
	Jiabud jiabud;
	OtiotYaxas otiotyaxas;


	bool natax_only_txiliot;


	/*-----------------------------------------------------------------*/
	/*------------------     natax -- the main routine  -------------*/
	/*-----------------------------------------------------------------*/

	void natax (CStr w, bool only_txiliot, MorphOptions& the_options) {
		the_options.remove_all();
		the_final_options_p = &the_options;
		I_used_the_default_nitux = false;

		myword0.set(w);
		myword1.reset(myword0.len+3);  
		myword2.reset(myword0.len+3);  
		myinfo1.init();
		myinfo2.init();
		num_of_txiliot = 0;
		hd=NO_HD;
		vav=false;
		yidua=NONE;
		jiabud=NO_JIABUD;
		otiotyaxas=NO_OY;
		natax_only_txiliot = only_txiliot;

		if (the_options.isempty()) ntx_symbols();
		if (the_options.isempty()) ntx_txiliot(); 
		if (the_options.isempty()) ntx_sfarot(); 
		if (default_heleq_diber!=NO_HD) {
			if (the_options.isempty()) {
				natax_only_txiliot = true;
				ntx_txiliot(); 
				I_used_the_default_nitux = true;
			}
		}
	}


	/*-----------------------------------------------------------------*/
	/*--------  addoption -- a utility routine            -------------*/
	/*-----------------------------------------------------------------*/

	void addoption(CStr thebase, MorphInfo theinfo) {
		if (natax_only_txiliot) {
			theinfo.setvav(vav);  
			theinfo.setjiabud(jiabud);
			theinfo.setotiotyaxas(otiotyaxas);
			theinfo.setyidua (hd==JEM_PRATI? NONE: yidua==NONE? LOMEYUDA: yidua);
			theinfo.setbaseword (thebase);
			the_final_options.append(theinfo);
			return;
		}


		/* paccel nittuxim 'kfulim' */
		if (theinfo.meen()==BOTH && theinfo.guf()==GUF2) {
			theinfo.setmeen(ZAKAR); addoption(thebase,theinfo);
			theinfo.setmeen(NQEVA); addoption(thebase,theinfo);
			return;
		}	
		if (theinfo.smikut()==BOTH) {
			theinfo.setsmikut(NIFRAD); addoption(thebase,theinfo);
			theinfo.setsmikut(NISMAK); addoption(thebase,theinfo);
			return;
		}
		if (yidua==BOTH) {
			yidua=KNMEYUDA; addoption(thebase,theinfo);
			yidua=LOMEYUDA; addoption(thebase,theinfo);
			yidua=BOTH;
			return;
		}


		/* ba@@el nittuxim lo^ ^ep$ariyim */
		if (theinfo.meyuda_diqduqit() && (hd==POAL || hd==POAL_EZER || yidua==KNMEYUDA))  return;      // &ecem myuda& and po&al can't be nismak or have kinui-guf

		/* qva& ^et ha-txiliot */
		theinfo.setvav(vav);  
		theinfo.setjiabud(jiabud);

		if (yej_otiotyaxas(hd)) 
			theinfo.setotiotyaxas(otiotyaxas);
		else
			theinfo.setotiotyaxas(NO_OY);

		if (yej_yidua(hd))  
			theinfo.setyidua (yidua==NONE? LOMEYUDA: yidua);
		else if ( (hd==POAL || hd==POAL_EZER) && theinfo.beinony() ) {
			if (yidua==KNMEYUDA) 
				theinfo.setjiabud (jiabud==NO_JIABUD? JIABUD_HA: JIABUD_JEHA);
		}
		else {
			if (yidua==KNMEYUDA) return;
			theinfo.setyidua (NONE);
		}

		theinfo.setbaseword (thebase);
		theinfo.setheleqdiber (hd);
		the_final_options.append(theinfo);
	}



	/*-----------------------------------------------------------------*/
	/*--------  ntx_txiliot and utility routines          -------------*/
	/*-----------------------------------------------------------------*/

	#define ntx_WITHOUT_TXILIT_VAV(thelen,thecheck) \
			if (myword0.len-thelen>1 || isdigit(myword0[1])) {\
				vav = true; \
				myword0.removefirst_fast(thelen); num_of_txiliot+=thelen; \
				thecheck(); \
				myword0.restorefirst(thelen);  num_of_txiliot-=thelen;\
				vav = false; }

	#define ntx_WITHOUT_TXILIT_JIABUD(thelen,thecheck,thejiabud) \
			if (myword0.len-thelen>1 || isdigit(myword0[1])) {\
				jiabud=thejiabud;     \
				myword0.removefirst_fast(thelen); num_of_txiliot+=thelen; \
				thecheck(); \
				myword0.restorefirst(thelen);  num_of_txiliot-=thelen;\
				jiabud=NO_JIABUD;     }

	#define ntx_WITHOUT_TXILIT_YAXAS_w_YIDUA(thelen,thecheck,theoy,theyidua) \
			if (myword0.len-thelen>1 || isdigit(myword0[1])) {\
				otiotyaxas=theoy;    yidua = theyidua; \
				myword0.removefirst_fast(thelen); num_of_txiliot+=thelen; \
				thecheck(); \
				myword0.restorefirst(thelen);  num_of_txiliot-=thelen;\
				otiotyaxas=NO_OY;     yidua = NONE; }
		
	void ntx_txiliot () {
		switch(myword0[0]) {
			case 'W':	ntx_txiliot_vav(); break;
			case 'B': case 'M': case 'H': ntx_txiliot_yaxas_w_yidua(); break;
			case 'L': case 'K': case '$': ntx_txiliot_jiabud(); break;
			default:    ntx_bhnxa_jein_txiliot(); break;
		}
	}
 
	void ntx_txiliot_vav () {   // vav ha-xibur
		if (myword0[0]=='W')  { 
			ntx_bhnxa_jein_txiliot();
			ntx_WITHOUT_TXILIT_VAV(1,ntx_txiliot_jiabud);
		} 
		else ntx_txiliot_jiabud ();
	}

	void ntx_txiliot_jiabud () {   // $e- k$e- lk$e- $e-k$e-
		if (myword0.starts_with("$K$",3)) {
			ntx_bhnxa_jein_txiliot();
			ntx_WITHOUT_TXILIT_JIABUD(1,ntx_txiliot_yaxas_w_yidua,JIABUD_JE);
			ntx_WITHOUT_TXILIT_JIABUD(3,ntx_txiliot_yaxas_w_yidua,JIABUD_JEKJE);
		}
		else if (myword0.starts_with('$'))  { 
			ntx_bhnxa_jein_txiliot();
			ntx_WITHOUT_TXILIT_JIABUD(1,ntx_txiliot_yaxas_w_yidua,JIABUD_JE);
		}
		else if (myword0.starts_with("K$")) {
			ntx_txiliot_yaxas_w_yidua();
			ntx_WITHOUT_TXILIT_JIABUD(2,ntx_txiliot_yaxas_w_yidua,JIABUD_KJE);
		}
		else if (myword0.starts_with("LK$",3)) {
			ntx_txiliot_yaxas_w_yidua();
			ntx_WITHOUT_TXILIT_JIABUD(3,ntx_txiliot_yaxas_w_yidua,JIABUD_LKJE);
		}
		else ntx_txiliot_yaxas_w_yidua();
	}

	void ntx_txiliot_yaxas_w_yidua () {	// b- k- l- mi- mi-ha- ha-
		if (myword0.starts_with("MH",2)) {
			ntx_bhnxa_jein_txiliot();
			ntx_WITHOUT_TXILIT_YAXAS_w_YIDUA(1,ntx_bhnxa_jein_txiliot,OY_MEM,NONE);
			ntx_WITHOUT_TXILIT_YAXAS_w_YIDUA(2,ntx_bhnxa_jein_txiliot,OY_MEM,KNMEYUDA);
		}
		else if (myword0.starts_with('M'))  { 
			ntx_bhnxa_jein_txiliot();
			ntx_WITHOUT_TXILIT_YAXAS_w_YIDUA(1,ntx_bhnxa_jein_txiliot,OY_MEM,NONE);
		}
		else if (myword0.starts_with('L'))  { 
			ntx_bhnxa_jein_txiliot();
			ntx_WITHOUT_TXILIT_YAXAS_w_YIDUA(1,ntx_bhnxa_jein_txiliot,OY_LAMED,BOTH);
		}
		else if (myword0.starts_with('K'))  { 
			ntx_bhnxa_jein_txiliot();
			ntx_WITHOUT_TXILIT_YAXAS_w_YIDUA(1,ntx_bhnxa_jein_txiliot,OY_KAF,BOTH);
		}
		else if (myword0.starts_with('B'))  { 
			ntx_bhnxa_jein_txiliot();
			ntx_WITHOUT_TXILIT_YAXAS_w_YIDUA(1,ntx_bhnxa_jein_txiliot,OY_BET,BOTH);
		}
		else if (myword0.starts_with('H'))  { 
			ntx_bhnxa_jein_txiliot();
			ntx_WITHOUT_TXILIT_YAXAS_w_YIDUA(1,ntx_bhnxa_jein_txiliot,NO_OY,KNMEYUDA);
		}
		else ntx_bhnxa_jein_txiliot();
	}




	/*-----------------------------------------------------------------*/
	/*--------  ntx_bhnxa_jein_txiliot                   -------------*/
	/*-----------------------------------------------------------------*/

	void ntx_bhnxa_jein_txiliot () {
		if (natax_only_txiliot) {
			myinfo1.setheleqdiber(default_heleq_diber);
			if (default_heleq_diber==JEM_PRATI) {
				if (ends_with(myword0,'H')) myinfo1.setmeen(NQEVA);
				else myinfo1.setmeen(BOTH);
			}
			else
				myinfo1.setmeen(BOTH);
			addoption(myword0.str,myinfo1);
			return;
		}
		bool maqaf = false, vav_kfula=false;
		if ( (myword0[0]=='-' || myword0[0]=='"') && num_of_txiliot>0 && myword0.len>1) {
			myword0.removefirst_fast(1);
			maqaf=true;
		}
		if (myword0.starts_with("WW") && num_of_txiliot>0 && vav==false)  {
			myword0.removefirst_fast(1);
			vav_kfula=true;
		}
		if (otiotyaxas==NO_OY && yidua!=KNMEYUDA) {  // bdoq yoce^ dofen
			MorphOptions ydo;         // yoce^-dofen options
			if (lexicon_contains_yoce_dofen (myword0.str,ydo)) {
				LOOPVECTOR(;,ydo,i) {
					myinfo1.set(ydo[i]);
					hd=ydo[i].heleqdiber();  yidua=ydo[i].yidua();  otiotyaxas=ydo[i].otiotyaxas();
					addoption(ydo[i].baseword(),myinfo1);
				}
				goto sof;
			}
		}
		ntx_bhnxa_jze_MISPAR();   if (word_contains_digit(myword0))  goto sof;
		ntx_bhnxa_jze_ECEM();
		ntx_bhnxa_jze_TOAR();
		ntx_bhnxa_jze_MILAT_GUF();
		if (yidua!=KNMEYUDA) {
			ntx_bhnxa_jze_JEM_PRATI();
		}
		if (otiotyaxas==NO_OY) {
			ntx_bhnxa_jze_POAL_HOWE();
			ntx_bhnxa_jze_POAL_EZER();
		}
		if (otiotyaxas==NO_OY && yidua!=KNMEYUDA) {
			ntx_bhnxa_jze_POAL_AVAR();
			ntx_bhnxa_jze_POAL_ATID();
			ntx_bhnxa_jze_MILAT_YAXAS();
			ntx_bhnxa_jze_hd_pjut (MILAT_XIBUR);
			ntx_bhnxa_jze_hd_pjut (MILAT_JEELA);
			ntx_bhnxa_jze_hd_pjut (MILIT);
			ntx_bhnxa_jze_hd_pjut (TOAR_POAL);
		}

	sof:
		if (maqaf) 	myword0.restorefirst(1);
		if (vav_kfula) 	myword0.restorefirst(1);
	}


	/*-----------------------------------------------------------------*/
	/*-- ntx bhnxa_jze &ecem                                    --------*/
	/*-----------------------------------------------------------------*/

	void ntx_bhnxa_jze_ECEM () {
		myinfo1.init(hd=ECEM);
		if (lexicon_contains_ecem_miuxd(myword0.str,myinfo1)) { addoption(myinfo1.baseword(),myinfo1); }
		ntx_siomot_jayakut(myword0);
	}


	void ntx_siomot_jayakut (StringTempCR theword) {
		Index i;  LOOP_MATCHING_TAVNIOT(i,tavniot_jayakut_ecem, theword.str,myword1) 
			if (myword1.len<2) continue;
			if (yidua==KNMEYUDA && curtavnit.info().meyuda_diqduqit() )  continue;  // mna& yidu& kapul
			if (ends_with(myword1,'H') && !ends_with(curtavnit,'H')  ) continue;  // mna& nittuxim kgon:    ILDHKM (H-ILDH-$LKM)
			if (ends_with(myword1,'I') && !ends_with(curtavnit,'I')  ) continue;  // mna& nittuxim kgon:	AIK (H-AI-$LK)
			if (laecem_yej_ntiat_jayakut_miuxedet(myword1.str) && curtavnit.sug()==0)  continue;
			myinfo1.set ( curtavnit.info() );
			if (yidua==KNMEYUDA && curtavnit.smikut()==BOTH)  myinfo1.setsmikut(NIFRAD);   // ecem meyuda can't be nismak.
			if (myinfo1.mispar()==RABIM)  ntx_siomot_ribui(myword1);
			else ntx_ecem_bli_siomot(myword1);
		}	
	}

	void ntx_siomot_ribui (StringTempCR theword) {
		Meen themeen;
		myinfo2 = myinfo1;  // save
		Index i;  LOOP_MATCHING_TAVNIOT(i,tavniot_ribui_ecem, theword.str,myword2) 
			if (myword2.len<2) continue;
			if (lexicon_contains_ecem(myword2.str,curtavnit.sug(),&themeen)) {
				myinfo1.fill ( curtavnit.info() );
				myinfo1.setmeen (themeen);
				addoption(myword2.str,myinfo1);
				myinfo1 = myinfo2;      // restore
			}
		}	
	}

	void ntx_ecem_bli_siomot (StringTempCR theword) {
		Meen themeen;   uint thesug;
		if (lexicon_contains_ecem(theword.str,&thesug,&themeen)) {
			if (thesug!=4) {            // 4 means: ecem merube (MIM, NI$WAIN,...)
				myinfo1.fillecem(themeen,YAXID,BOTH);
				addoption(theword.str,myinfo1);
			}
		}
	}



	/*-----------------------------------------------------------------*/
	/*-- ntx bhnxa_jze to^ar                                    --------*/
	/*-----------------------------------------------------------------*/

	void ntx_bhnxa_jze_TOAR() {
		myinfo1.init(hd=TOAR);
		if (lexicon_contains_toar_miuxd(myword0.str,myinfo1)) { addoption(myinfo1.baseword(),myinfo1);  }
		ntx_siomot_toar (myword0);
	}
	
	void ntx_siomot_toar (StringTempCR theword) {
		Index i;  LOOP_MATCHING_TAVNIOT(i,tavniot_toar, theword.str,myword1) 
			if (myword1.len<2) continue;
			if (yidua==KNMEYUDA && curtavnit.info().meyuda_diqduqit() )  continue;  // can't be meyuda twice
			if (ends_with(myword1,'H') && !ends_with(curtavnit,'H') ) continue;  // mone& n@iot kgon IPHWT (bmqom IPWT).
			if (ends_with(myword1,'I') && !ends_with(curtavnit,'I') ) continue;  // mone& n@iot kgon XIWNIH (bmqom XIWNIT).
			if (latoar_yej_ntia_miuxedet(myword1.str) && curtavnit.sug()==0)  continue;
			myinfo1.set ( curtavnit.info() );
			ntx_toar_bli_siomot (myword1);
		}
	}

	void ntx_toar_bli_siomot (StringTempCR theword) {
		myinfo2 = myinfo1;  
		if (lexicon_contains_toar(theword.str)) {
			myinfo1.setheleqdiber(TOAR);
			myinfo1.fillecem(ZAKAR,YAXID,BOTH);
			addoption(theword.str,myinfo1);
			myinfo1 = myinfo2;  
		}
	}



	/*-----------------------------------------------------------------*/
	/*-- ntx bhnxa_jze po&al-&avar                              --------*/
	/*-----------------------------------------------------------------*/

	void ntx_bhnxa_jze_POAL_AVAR() {
		myinfo1.init(hd=POAL);  myinfo1.setzman(AVAR);
		if (lexicon_contains_poal_miuxd(myword0.str,AVAR,myinfo1))  { addoption(myinfo1.baseword(),myinfo1); return; }
		ntx_siomot_poal_avar(myword0);
	}


	void ntx_siomot_poal_avar (StringTempCR theword) {
		Index i;  LOOP_MATCHING_TAVNIOT(i,tavniot_poal_avar, theword.str,myword1) 
			if (myword1.len<2) continue;
			if (ends_with(myword1,'I') && !ends_with(curtavnit,'I') ) continue;  // ^im ha-po&l mistayyem b-I, xayabim lnattex ^oto lpi tabnit $ell gizrat L"I.
			if (btavnit_hifiil(myword1) && !btavnit_hifiil(curtavnit) ) continue;  // ^im ha-po&l hu^ b-tabnit hip&il, xayabim lnattex ^oto lpi tabnit $ell binyan hip&il.
			myinfo1.set ( curtavnit.info() );
			ntx_poal_avar_bli_siomot(myword1);
		}	
	}

	void ntx_poal_avar_bli_siomot (StringTempCR theword) {
		if (lexicon_contains_poal(theword.str)) {
			myinfo1.fillpoal(ZAKAR,YAXID,GUF3,AVAR);
			addoption(theword.str,myinfo1);
		}
	}

	


	/*-----------------------------------------------------------------*/
	/*-- ntx bhnxa_jze po&al-howe                               --------*/
	/*-----------------------------------------------------------------*/

	void ntx_bhnxa_jze_POAL_HOWE() {
		myinfo1.init(hd=POAL);
		if (lexicon_contains_poal_miuxd(myword0.str,HOWE,myinfo1)) { addoption(myinfo1.baseword(),myinfo1); return; }  // kolel gam howe-pa&il w-gam howe-savil
		ntx_siomot_poal_howe (myword0);
	}
	
	void ntx_siomot_poal_howe (StringTempCR theword) {
		Index i;  LOOP_MATCHING_TAVNIOT(i,tavniot_toar, theword.str,myword1) 
			if (myword1.len<2) continue;
			if (yidua==KNMEYUDA && curtavnit.info().meyuda_diqduqit() )  continue;  // can't be meyuda twice
			if (ends_with(myword1,'H') && !ends_with(curtavnit,'H') ) continue;  // mone& n@iot kgon MDLIQHWT (bmqom MDLIQWT).
			if (ends_with(myword1,'I') && !ends_with(curtavnit,'I') ) continue;  // mone& n@iot kgon GWBIH (bmqom GWBH).
			if (latoar_yej_ntia_miuxedet(myword1.str) && curtavnit.sug()==0)  continue;
			myinfo1.set ( curtavnit.info() );
			ntx_beinony_bli_siomot (myword1);
		}
	}

	void ntx_beinony_bli_siomot (StringTempCR theword) {
		myinfo2 = myinfo1;
		Index i;  LOOP_MATCHING_TAVNIOT(i,tavniot_beinony, theword.str,myword2) 
			if (myword2.len<2) continue;
			//if (curtavnit.zman()==HOWE_SAVIL)
			if (ends_with(myword1,'I') && !ends_with(curtavnit,'I') ) continue;  // mone& n@iot kgon HRI (bmqom HRH).
			if (lexicon_contains_poal_1(myword2.str,curtavnit.sug())) {
				myinfo1.setheleqdiber(POAL);    
				myinfo1.fill ( curtavnit.info() );
				addoption(myword2.str,myinfo1);
				myinfo1 = myinfo2;  
			}
		}
	}
		


	/*-----------------------------------------------------------------*/
	/*-- ntx bhnxa_jze po&al-&atid/ciwui/maqor                  --------*/
	/*-----------------------------------------------------------------*/

	void ntx_bhnxa_jze_POAL_ATID() {
		hd=POAL;
		if (lexicon_contains_poal_miuxd(myword0.str,ATID,myinfo1))  { addoption(myinfo1.baseword(),myinfo1);  return; }

		for (Index binyan=0; binyan<NUM_OF_BINYANIM; binyan++)  {
			ntx_txiliot_poal_atid (binyan,myword0);
		}
	}



	bool miqre_miuxd (TavnitR tavnit_miuxdt) {
		if (tavnit_miuxdt.match(myword1.str,myword2) == OK && 
			lexicon_contains_poal_2(myword2.str,tavnit_miuxdt.sug())) 
				return true;
		else
				return false;	
	}
	
	void ntx_txiliot_poal_atid (uint binyan, StringTempCR theword) {
		Index i;  LOOP_MATCHING_TAVNIOT(i,txiliot_poal_atid[binyan], theword.str,myword1)
			if (myword1.len<2) continue;
			if (jiabud!=NO_JIABUD && (curtavnit.zman()==CIWUI || curtavnit.zman()==MAQOR)) continue;
			char lastchar = myword1.at_end(0);
			myinfo1.set(curtavnit.info());

			if (myinfo1.zman()==MAQOR) {
				if (ends_with(myword1,'I') && !ends_with(curtavnit,'I') ) continue;
				if (ends_with(myword1,'H') && !ends_with(curtavnit,'H') ) continue;
				ntx_hemjeky_poal_atid (hemjeky_poal_atid_ZY[binyan],myword1);
				if (miqre_miuxd(MIUXD_maqor_gizrat_p_i))  {
					addoption(myword2.str,myinfo1); 
				}
				continue;
			}
			if (myinfo1.guf()==GUF1) {
				ntx_hemjeky_poal_atid (hemjeky_poal_atid_ZY[binyan],myword1);
			}	
			else if (myinfo1.guf()==GUF3) {
				ntx_hemjeky_poal_atid (hemjeky_poal_atid_ZY[binyan],myword1);
				if (lastchar=='W')  ntx_hemjeky_poal_atid (hemjeky_poal_atid_ZR[binyan],myword1);
			}
			else if (myinfo1.guf()==GUF2) {
				ntx_hemjeky_poal_atid (hemjeky_poal_atid_ZY[binyan],myword1);
				if (lastchar=='W')  ntx_hemjeky_poal_atid (hemjeky_poal_atid_ZR[binyan],myword1);
				if (lastchar=='I')  ntx_hemjeky_poal_atid (hemjeky_poal_atid_NY[binyan],myword1);
				if (lastchar=='H')  ntx_hemjeky_poal_atid (hemjeky_poal_atid_NR[binyan],myword1);

				/* miqre miuxad -- ciwui zakar-yaxid $el binyan hif&il */
				if (binyan==3 && myinfo1.zman()==CIWUI) {
					if (miqre_miuxd(MIUXD_ciwui_binyan_hifiil_3) || miqre_miuxd(MIUXD_ciwui_binyan_hifiil_2)) {
						myinfo1.fillpoal (ZAKAR,YAXID,GUF2,CIWUI);
						addoption(myword2.str,myinfo1);
					}
				}
			}
		}
	}


	void ntx_hemjeky_poal_atid (TavniotR thetavniot, StringTempCR theword) {
		myinfo2 = myinfo1;      // save
		Index j; LOOP_MATCHING_TAVNIOT(j,thetavniot,theword.str,myword2)
			if (myword2.len<2) continue;
			if (myword2.ends_with('I') && !ends_with(curtavnit,'I') )  continue; // mna& nittux kgon: &LHI (bmqom &LI)

			/* miqrim miuxadim */
			if (curtavnit.sug()==61 && myinfo1.zman()==CIWUI &&   // miqre miuxad -- ciwui $el binyan hifiil
				curtavnit.meen()==ZAKAR && curtavnit.mispar()==YAXID) continue;
			//if (myword2.ends_with('H') && myinfo1.zman()==MAQOR) continue;   // miqre miuxad -- maqor $el gizrat lamed-hey
			if (myinfo1.zman()==MAQOR && curtavnit.sug()==16) continue;   // miqre miuxad -- maqor $el gizrat pey-yud / pey-lamed ...
			if (myinfo1.zman()==MAQOR && lapoal_yej_ntiat_maqor_miuxedet(myword2.str)) continue; // la-po&l ye$ n@iat maqor miuxedt.

			if (lexicon_contains_poal_2(myword2.str,curtavnit.sug())) {
				if (myinfo1.zman()!=MAQOR)        myinfo1.fill ( curtavnit.info() );
				addoption(myword2.str,myinfo1);	

				/* miqrim miuxdim: Tpqwd(*2), Tpqdi, Tpqdw, Tpdwdnh(*2)								 */
				if (myinfo1.zman()==ATID && myinfo1.guf()==GUF2)	{
					if (myinfo1.meen()==ZAKAR && myinfo1.mispar()==YAXID) {           // Tpqwd
						myinfo1.setmeen(NQEVA);    myinfo1.setguf(GUF3);  addoption(myword2.str,myinfo1); }
					else if (myinfo1.meen()==NQEVA && myinfo1.mispar()==RABIM) {      // TpqwdNH
						myinfo1.setguf(GUF3);  addoption(myword2.str,myinfo1); }
				}
				myinfo1 = myinfo2;       // restore
			}
		}
	}


	/*-----------------------------------------------------------------*/
	/*-- ntx bhnxa_jze $em-pra@i                                --------*/
	/*-----------------------------------------------------------------*/

	void ntx_bhnxa_jze_JEM_PRATI() {
		Meen themeen;
		if (lexicon_contains_jem_prati(myword0.str,&themeen)) {
			myinfo1.init(hd=JEM_PRATI);
			myinfo1.setmeen(themeen);
			addoption(myword0.str,myinfo1);
		}
	}




	/*-----------------------------------------------------------------*/
	/*-- ntx bhnxa_jze milat-yaxas/milat-guf/mispar/po&al-&ezer --------*/
	/*-----------------------------------------------------------------*/

	void ntx_bhnxa_jze_MILAT_YAXAS() {
		hd=MILAT_YAXAS;
		if (lexicon_contains_milat_yaxas(myword0.str,myinfo1))
			addoption(myinfo1.baseword(),myinfo1);
	}

	void ntx_bhnxa_jze_MILAT_GUF() {
		hd=MILAT_GUF;
		if (lexicon_contains_milat_guf(myword0.str,myinfo1)) {
			if (!myinfo1.hu("ZH")  &&   
				yidua==KNMEYUDA  )                    return;    // mna& millim kgon H-MI$HW, H-HHWA, H-ANI
			if (myinfo1.hu("HWA")  &&   
			    otiotyaxas!=NO_OY  )                  return;	// mna& millim kgon L-ATH, B-ANI
			addoption(myinfo1.baseword(),myinfo1);
		}
	}

	void ntx_bhnxa_jze_MISPAR () {
		hd=MISPAR;
		if (myword0[0]=='%' && isdigit(myword0[1])) {
			myinfo1.setsmikut(NIFRAD);
			myinfo1.setmeen(BOTH);
			myinfo1.setmispar(RABIM);
			addoption("M%",myinfo1);
		}
		else if (isdigit(myword0[0])) {
			myinfo1.setsmikut(BOTH);
			myinfo1.setmeen(BOTH);
			if (identical(myword0.str,"1"))
				myinfo1.setmispar(YAXID);
			else 
				myinfo1.setmispar(RABIM);
			addoption("M#",myinfo1);
		}
		else if (lexicon_contains_mispar(myword0.str,myinfo1)) {
			if (yidua==KNMEYUDA && myinfo1.meyuda_diqduqit() ) return; 
			else addoption(myinfo1.baseword(),myinfo1);
		}
	}

	void ntx_bhnxa_jze_POAL_EZER() {
		hd=POAL_EZER;
		if (lexicon_contains_poal_ezer(myword0.str,myinfo1)) {
			if (myinfo1.zman()!=HOWE && myinfo1.zman()!=HOWE_SAVIL && yidua==KNMEYUDA)
				return;
			addoption(myinfo1.baseword(),myinfo1);
		}
	}


	/*-----------------------------------------------------------------*/
	/*--------  ntx_bhnxa_jze_hd_pjut                               --*/
	/*--------		(milat-xibur, milat-$e^ela, to^ar-po&al, milit)  --*/
	/*-----------------------------------------------------------------*/

	void ntx_bhnxa_jze_hd_pjut(HeleqDiber thehd) {
		hd = thehd;
		if (lexicon_contains_milit(myword0.str,thehd))	{
			myinfo1.init(thehd);
			addoption(myword0.str,myinfo1);
		}
	}


	/*-----------------------------------------------------------------*/
	/*--------  ntx_symbols					              -------------*/
	/*-----------------------------------------------------------------*/


	/* ntx_symbols (punctuation, etc) */
	void ntx_symbols() {
		if (charinstring(myword0[0],",;:.!?\"-'()[]{}")) {
			myinfo1.init (hd=MILAT_XIBUR);
			addoption (myword0.str,myinfo1);
		}
	}


	/* ntx_sfarot (punctuation, etc) */
	void ntx_sfarot() {
		if (word_contains_digit(myword0)) {
			myinfo1.init (hd=MISPAR);
			myinfo1.setyidua(LOMEYUDA);
			myinfo1.setmeen(BOTH);
			myinfo1.setmispar(BOTH);
			myinfo1.setsmikut(NIFRAD);
			addoption ("M?",myinfo1);
		}
	}

};


/*******************************************************************/
/*********  public routines						      **************/
/*******************************************************************/



void initialize_the_analyzer (CStr path_to_tavniot, CStr path_to_lexicon) {
	DOx ( read_the_tavniot(path_to_tavniot) );
	read_the_lexicon(path_to_lexicon);
	the_analyzer_is_initialized = true;
}


void natax (CStr theword, MorphOptions& theoptions) {
	assert (the_analyzer_is_initialized);
	MorphAnalyzer myanalyzer;
	myanalyzer.natax(theword,false,theoptions);
}


MorphOptions natax (CStr theword) {
	MorphOptions theoptions;
	natax (theword,theoptions);
	return theoptions;
}


void natax_txiliot (CStr theword, MorphOptions& theoptions) {
	MorphAnalyzer myanalyzer;
	myanalyzer.natax(theword,true,theoptions);
}


MorphOptions natax_txiliot (CStr theword) {
	MorphOptions theoptions;
	natax_txiliot (theword,theoptions);
	return theoptions;
}



/*******************************************************************************************************/
/****************  SELF TEST          ******************************************************************/
/*******************************************************************************************************/


/* The self test has moved to the file: morphanl.test.cpp */ 
