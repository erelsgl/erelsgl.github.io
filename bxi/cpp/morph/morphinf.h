/*
 * File morph/morphinf.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphinf.h
 */

/* morphinf.h --   morphological info structures: MorphInfoBasic, MorphInfo */

#ifndef __MORPHINF_H
#define __MORPHINF_H

#include <morph/morphtyp.h>
#include <cpplib/stringt.h>

/*
	This unit defines 2 types:  MorphInfoBasic and MorphInfo.

	MorphInfoBasic encapsulates the morphological information about a single word, 
		(MEEN, MISPAR, GUF, etc...) except the lexical-value.

	MorphInfo inherits MorphInfoBasic, and it includes the lexical-value too.

	NOTE: the lexical-value is called the "baseword" of the MorphInfo.
*/

/***************************************************************************************/
/*      MorphInfoBasic                                                                 */
/***************************************************************************************/




/* 
	'txiliot' is the number of letters before the baseword (such as: W B K L M H $).
	'meen_siomet', 'mispar_siomet' and 'guf_siomet' are the meen, mispar and guf of the 
		suffix ("kinnuy-$ayakut $ell $em-&ecm" ^o "siomt $ell millat-yaxas").
*/

class MorphInfoBasic;
typedef MorphInfoBasic& MorphInfoBasicR;
typedef const MorphInfoBasic& MorphInfoBasicCR;
	
class MorphInfoBasic        {         // morphological information about one Hebrew word
	protected:
		int myvav: 1;
		Jiabud myjiabud: 3;
		OtiotYaxas myotiotyaxas: 3;
		Yidua myyidua: 2;
		HeleqDiber	myheleqdiber: 4;            // 23 bits

		Meen		mymeen: 2;		
		Mispar		mymispar: 2;	
		Guf			myguf: 3;		
		Smikut		mysmikut: 2;	
		Zman		myzman: 3;		

		Meen        mymeen_siomet: 2;
		Mispar		mymispar_siomet: 2;
		Guf			myguf_siomet: 3;


	public:
		/* queries */
		uint txiliot() const {   // num of txiliot
			return (
				(myvav? 1: 0) +
				(myjiabud==JIABUD_JE? 1: myjiabud==JIABUD_KJE? 2: myjiabud==JIABUD_LKJE? 3: myjiabud==JIABUD_JEKJE? 3: myjiabud==JIABUD_HA? 1: myjiabud==JIABUD_JEHA? 2: 0) +
				(myotiotyaxas==NO_OY? 0: 1) +
				(myyidua==KNMEYUDA? (myotiotyaxas==NO_OY||myotiotyaxas==OY_MEM? 1: 0): 0)
			);
		}
		Yidua		yidua() const { return myyidua; }
		Jiabud		jiabud() const { return myjiabud; }
		OtiotYaxas	otiotyaxas() const { return myotiotyaxas; }
		bool		vav() const { return myvav!=0; }
		bool		has_vav_o_yaxas_o_jiabud() const {
			return myvav || (myotiotyaxas!=NO_OY) || (myjiabud!=NO_JIABUD); }
		HeleqDiber	heleqdiber() const {return myheleqdiber;}		
		Meen		meen() const {return mymeen;}				
		Mispar		mispar() const {return mymispar;}			
		Guf			guf() const {return myguf;}				
		Smikut		smikut() const {return mysmikut;}			
		Zman		zman() const {return myzman;}				
		Meen        meen_siomet() const {return mymeen_siomet;}		
		Mispar		mispar_siomet() const {return mymispar_siomet;}	
		Guf			guf_siomet() const {return myguf_siomet;}		
		bool		has_siomet() const { return myguf_siomet!=NONE; }

		bool		meyuda_diqduqit() const { return 
			mysmikut==NISMAK || has_siomet() || 
			myheleqdiber==JEM_PRATI;
		}
		bool		meyuda() const { return meyuda_diqduqit() || myyidua==KNMEYUDA; }
		bool		beinony() const { return myzman==HOWE||myzman==HOWE_SAVIL; }
		bool		poal() const { return myheleqdiber==POAL||myheleqdiber==POAL_EZER; }
		bool		nismak() const { return mysmikut==NISMAK; }
		bool		can_be_somek () const  { return (myheleqdiber==ECEM||myheleqdiber==JEM_PRATI||myheleqdiber==MISPAR)&&(!myvav&&myjiabud==NO_JIABUD&&myotiotyaxas==NO_OY); }
		bool		hu (HeleqDiber hd) const { return myheleqdiber==hd; }


		/* editing */
		void	setyidua(Yidua theyidua)  {myyidua=theyidua;}		
		void	setjiabud(Jiabud thejiabud)  {myjiabud=thejiabud;}		
		void	setotiotyaxas(OtiotYaxas theoy)  {myotiotyaxas=theoy;}		
		void	setvav (bool thevav) {myvav = (thevav? -1: 0);}
		void	setheleqdiber(HeleqDiber thehd)  {myheleqdiber=thehd;}		
		void	setmeen(Meen themeen)  {mymeen=themeen;}				
		void	setmispar(Mispar themispar)  {mymispar=themispar;}			
		void	setguf(Guf theguf)  {myguf=theguf;}				
		void	setsmikut(Smikut thesmikut)  {mysmikut=thesmikut;}	
		void	setzman(Zman thezman)  {myzman=thezman;}			
		void    setmeen_siomet(Meen themeen)  {mymeen_siomet=themeen;}		
		void	setmispar_siomet(Mispar themispar)  {mymispar_siomet=themispar;}	
		void	setguf_siomet(Guf theguf)  {myguf_siomet=theguf;}		

		void	settxiliot(bool thevav, Jiabud thejiabud, OtiotYaxas theotiotyaxas, Yidua theyidua) {
					myvav=thevav? -1: 0;  myjiabud=thejiabud; myotiotyaxas=theotiotyaxas; myyidua=theyidua; }
		void	settxiliot(MorphInfoBasicCR theinfo) {
					settxiliot ((theinfo.myvav!=0), theinfo.myjiabud, theinfo.myotiotyaxas, theinfo.myyidua); }
		void	setecem (Meen themeen, Mispar themispar, Smikut thesmikut) {
					mymeen=themeen; mymispar=themispar; mysmikut=thesmikut; }
		void	setpoal (Meen themeen, Mispar themispar, Guf theguf, Zman thezman) {
					mymeen=themeen; mymispar=themispar; myguf=theguf; myzman=thezman;  }
		void	setiqar (Meen themeen, Mispar themispar, Guf theguf, Smikut thesmikut, Zman thezman) {
					mymeen=themeen; mymispar=themispar; myguf=theguf; myzman=thezman; mysmikut=thesmikut;  }
		void	setsiomet (Meen themeen, Mispar themispar, Guf theguf) {
					mymeen_siomet=themeen; mymispar_siomet=themispar; myguf_siomet=theguf; }

		void	set (MorphInfoBasicCR theinfo) {
					myheleqdiber = theinfo.myheleqdiber;
					setiqar (theinfo.mymeen,theinfo.mymispar,theinfo.myguf,theinfo.mysmikut,theinfo.myzman);
					setsiomet (theinfo.mymeen_siomet,theinfo.mymispar_siomet,theinfo.myguf_siomet); 				}

		void	fillecem (Meen themeen, Mispar themispar, Smikut thesmikut) {
					if (mymeen==NONE)  mymeen=themeen;
					if (mymispar==NONE)  mymispar=themispar;
					if (mysmikut==NONE)  mysmikut=thesmikut; 				}
		void	fillpoal (Meen themeen, Mispar themispar, Guf theguf, Zman thezman) {
					if (mymeen==NONE)  mymeen=themeen;
					if (mymispar==NONE)  mymispar=themispar;
					if (myguf==NONE)  myguf=theguf;
					if (myzman==NONE)  myzman=thezman;  }
		void	filliqar (Meen themeen, Mispar themispar, Guf theguf, Smikut thesmikut, Zman thezman) {
					fillpoal (themeen,themispar,theguf,thezman);
					if (mysmikut==NONE)  mysmikut=thesmikut; 				}
		void	fillsiomet (Meen themeen, Mispar themispar, Guf theguf) {
					if (mymeen_siomet==NONE)  mymeen_siomet=themeen;
					if (mymispar_siomet==NONE)  mymispar_siomet=themispar;
					if (myguf_siomet==NONE)  myguf_siomet=theguf;            }
		void	fill (MorphInfoBasicCR theinfo) {
					if (myheleqdiber==NONE)  myheleqdiber = theinfo.myheleqdiber;
					filliqar (theinfo.mymeen,theinfo.mymispar,theinfo.myguf,theinfo.mysmikut,theinfo.myzman);
					fillsiomet (theinfo.mymeen_siomet,theinfo.mymispar_siomet,theinfo.myguf_siomet); 				}

		/* init */
		void init (HeleqDiber thehd=NO_HD) { 
			settxiliot(false,NO_JIABUD,NO_OY,NONE); myheleqdiber=thehd; 
			setiqar (NONE,NONE,NONE,NONE,NO_ZMAN);
			setsiomet (NONE,NONE,NONE);
		}
		void set (char* theinfo);  // sets according to a string in the 'cafuf' I/O format
		MorphInfoBasic(HeleqDiber thehd=NO_HD) { init(thehd); }
		MorphInfoBasic(char* theinfo) { set(theinfo); }

		// ye$ 2 curot pel@. nadgim ^otan &al ha-milla XDRKM:
		void ktov_meforat (ostream& out)  const;						// &CM LA-MIWD&, ZKR YXID NFRD ($IIKWT: ZKR RBIM NWKX)
		void ktov_cafuf (ostream& out)  const;							// *E**LZYFZR2

		MorphInfoBasic (int i) { init(NO_HD); }
		bool operator== (int i) const { return myheleqdiber==NO_HD; }

		MorphInfoBasic mila_rijona_info() const;  // The info that matches the first particle of the word
		MorphInfoBasic mila_axrona_info() const { // The info that matches the last particle of the word
			MorphInfoBasic result = *this;
			result.setvav(false);  
			result.setjiabud(NO_JIABUD);
			result.setotiotyaxas(NO_OY);
			return result;
		}


	protected:
		CStr maxrozet_siomet_jayakut ()  const{ return ::maxrozet_siomet_jayakut (mymeen_siomet,mymispar_siomet,myguf_siomet); }
		CStr maxrozet_kinui_guf ()  const{ return ::maxrozet_kinui_guf (mymeen,mymispar,myguf); }
		CStr maxrozet_ribui ()  const{ return ::maxrozet_ribui (mymeen,mymispar); }
};


void write (ostream& out, MorphInfoBasicCR theinfo, Format format="");
status read (istream& in, MorphInfoBasicR theinfo, Format format="");

bool identical (MorphInfoBasicCR a, MorphInfoBasicCR b);
inline bool identical_basic (MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical(a,b); }
inline bool identical_basic_except_txiliot (MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical(a,b); }
inline short compare (MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical(a,b)? 0: 1; }

inline void duplicate (MorphInfoBasic& to, MorphInfoBasicCR from) { to = from; }
inline void free (MorphInfoBasic& ) {  }

DEFINE_INLINE_IO_OPERATORS (MorphInfoBasic)
DEFINE_INLINE_RELATIVE_OPERATORS(MorphInfoBasic)


inline bool identical_meen(MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical_meen(a.meen(),b.meen());  }
inline bool identical_mispar(MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical_mispar(a.mispar(),b.mispar());  }
inline bool identical_smikut(MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical_smikut(a.smikut(),b.smikut());  }
inline bool identical_guf(MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical_guf(a.guf(),b.guf());  }
inline bool identical_zman(MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical_zman(a.zman(),b.zman());  }
inline bool identical_yidua(MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical_yidua(a.yidua(),b.yidua());  }
inline bool identical_otiotyaxas(MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical_otiotyaxas(a.otiotyaxas(),b.otiotyaxas());  }
inline bool identical_jiabud(MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical_jiabud(a.jiabud(),b.jiabud());  }
inline bool identical_vav(MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical_vav(a.vav(),b.vav());  }
inline bool identical_heleqdiber(MorphInfoBasicCR a, MorphInfoBasicCR b) { return identical_heleqdiber(a.heleqdiber(),b.heleqdiber());  }

inline bool identical_mm (MorphInfoBasicCR a, MorphInfoBasicCR b) { return 
	identical_meen(a,b) && identical_mispar(a,b); }

inline bool identical_mmg (MorphInfoBasicCR a, MorphInfoBasicCR b) { return 
	identical_meen(a,b) && identical_mispar(a,b) && identical_guf(a,b) ; }

inline bool identical_mmy (MorphInfoBasicCR a, MorphInfoBasicCR b) { return 
	identical_meen(a,b) && identical_mispar(a,b) && identical_yidua(a,b) ; }

inline bool matimim (MorphInfoBasicCR a, MorphInfoBasicCR b) {
	HeleqDiber ah = a.heleqdiber(), bh = b.heleqdiber();
	if ( (ah==ECEM || ah==TOAR) && (bh==ECEM || bh==TOAR) )  return identical_mmy (a,b);
	if ( (ah==ECEM || ah==TOAR) && (bh==POAL || bh==POAL_EZER) )  return identical_mm (a,b);
	if ( (bh==ECEM || bh==TOAR) && (ah==POAL || ah==POAL_EZER) )  return identical_mm (a,b);
	if ( (ah==MILAT_GUF) && (bh==POAL || bh==POAL_EZER) )  return identical_mmg (a,b);
	if ( (bh==MILAT_GUF) && (ah==POAL || ah==POAL_EZER) )  return identical_mmg (a,b);
	return true;
}


/***************************************************************************************/
/*      MorphInfo                                                                      */
/***************************************************************************************/

#define MAX_BASEWORD_LEN 15
typedef char BaseWord[MAX_BASEWORD_LEN+1];           // 15 is the maximum word-length.


/* 
	'baseword' is the lexical value of the word.
*/

class MorphInfo;
typedef MorphInfo& MorphInfoR;
typedef const MorphInfo& MorphInfoCR;
	
class MorphInfo: public MorphInfoBasic    {
	protected:
//		StringTemp	mybaseword;            // the lexical value (base-word)
		BaseWord	mybaseword;            // the lexical value (base-word)
	public:
		CStr	baseword() const {return mybaseword;}         
		char*	baseword() {return mybaseword;}         
		CStr	fullword_bli_txiliot(CStr thefullword) const {return thefullword+txiliot();}         
		void	setbaseword(CStr thebaseword)  { 
			//mybaseword.set(thebaseword);
			strncpy(mybaseword,thebaseword,MAX_BASEWORD_LEN); mybaseword[MAX_BASEWORD_LEN]='\0'; 
		}         
		void setbaseword (MorphInfoCR theinfo) { setbaseword(theinfo.baseword()); setheleqdiber(theinfo.heleqdiber()); }
		void set (MorphInfoCR theinfo) { MorphInfoBasic::set(theinfo); setbaseword(theinfo.baseword()); }
		void set (char* theinfo);  // sets according to a string in the 'cafuf' I/O format

		bool		hu (HeleqDiber hd) const { return myheleqdiber==hd; }
		bool		hu (CStr bw) const { return identical(mybaseword,bw); }

		/* init */
		void init(CStr thebaseword,HeleqDiber thehd) { 
			setbaseword(thebaseword);
			MorphInfoBasic::init(thehd); 
		}
		void init (HeleqDiber thehd=NO_HD) {
			setbaseword("");
			MorphInfoBasic::init(thehd);
		}

		void free(){}// { mybaseword.free(); }


		MorphInfo(HeleqDiber thehd=NO_HD) { init(thehd); }
		MorphInfo(CStr thebaseword,HeleqDiber thehd) { init(thebaseword,thehd);  }
		MorphInfo(char* theinfo) { set(theinfo);  }

		void operator= (MorphInfoCR theinfo);
		MorphInfo(MorphInfoCR theinfo);

		bool meyuda_diqduqit() const { return 
			mysmikut==NISMAK || has_siomet() || 
			myheleqdiber==JEM_PRATI || 
			(myheleqdiber==MILAT_GUF && !identical(mybaseword,"ZH")); 
		}

		// ye$ 3 curot pel@. nadgim ^otan &al ha-milla XDRKM:
		void ktov_barur (ostream& out, CStr thefullword)  const;	// H-XDR-$LKM
		void ktov_meforat (ostream& out) const;						// XDR, &CM LA-MIWD&, ZKR YXID NFRD ($IIKWT: ZKR RBIM NWKX)
		void ktov_cafuf (ostream& out) const;						// XDR*E**LZYFZR2

		// "mila_rijona" hi^ ha-millit ha-ri$ona b-nittux ze (lma$al: W (^im ye$ waw ha-xibbur), K$ (^im ^ein W ^abal ye$ txillit "K$E-"),...
		CStr mila_rijona() const;    
		MorphInfo mila_rijona_info() const;
		MorphInfo mila_axrona_info() const; 

		MorphInfo (int i) { myheleqdiber=NO_HD; }
		bool operator== (int i) const { return myheleqdiber==NO_HD; }
};
typedef MorphInfo& MorphInfoR;
typedef const MorphInfo& MorphInfoCR;



void write (ostream& out, MorphInfoCR theinfo, Format format="");
status read (istream& in, MorphInfoR theinfo, Format format="");

inline bool identical (MorphInfoCR a, MorphInfoCR b) { 
	return identical_basic ((MorphInfoBasicCR)a,(MorphInfoBasicCR)b) && identical(a.baseword(),b.baseword()); }
inline bool identical_except_txiliot (MorphInfoCR a, MorphInfoCR b) { 
	return identical_basic_except_txiliot ((MorphInfoBasicCR)a,(MorphInfoBasicCR)b) && identical(a.baseword(),b.baseword()); }
inline bool identical_baseword (MorphInfoCR a, MorphInfoCR b) { 
	return (a.heleqdiber()==b.heleqdiber()) && identical(a.baseword(),b.baseword()); }
inline short compare (MorphInfoCR a, MorphInfoCR b) { return identical(a,b)? 0: 1; }

inline void duplicate (MorphInfo& to, MorphInfoCR from) { 
	duplicate ( (MorphInfoBasic&)to , (MorphInfoBasicCR)from );
	to.setbaseword (from.baseword());
}
inline void free (MorphInfo& a) { a.free(); }


DEFINE_INLINE_IO_OPERATORS (MorphInfo)
DEFINE_INLINE_RELATIVE_OPERATORS (MorphInfo)

StringTemp concat_bw (MorphInfoCR a, MorphInfoCR b);


/* MorphInfo's of particles */
extern MorphInfo miKLUM, miW, miJE, miKJE, miLKJE, miHA, miB, miK, miL, miM, miNQUDA, miMAQAF;

MorphInfo jiabud_info (Jiabud thejiabud);
MorphInfo otiotyaxas_info (OtiotYaxas theotiotyaxas);


#endif


