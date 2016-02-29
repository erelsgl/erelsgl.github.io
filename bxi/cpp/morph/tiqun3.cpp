/*
 * File morph/tiqun3.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/tiqun3.cpp
 */

/* tiqun3.cpp */

#include <morph/tiqun3.h>
#include <morph/mip2.h>
#include <morph/morphsik.h>
#include <morph/hpmaio.h>


bool equivalent (MorphInfoPatternCR a, MorphInfoPatternCR b) {
	return false;
}

bool ulay_baseword_xajuv (MorphInfoCR a) {
	return  true;
}


/*****************************************************************/
/*****                   Tiqun3                               ****/
/*****************************************************************/




struct Tiqun3 {
	MorphInfoPattern x, y, newx, newy;
	Sikui tosefetx, tosefety;

	// Ma$ma&ut ha-tiqun hi^: 
	//	^im "tosefetx>0", w-ha-nittux ha-nokxi mat^im la-tabnit "x",
	//		mca^ nittux xlupi $e-mat^im la-tabnit "newx", w-hosep lo "tosefetx".
	//	kn"l lgabei y.

	static Array2<Index> x_indexes, y_indexes;          // ^indeqsim msuddarim lpi sikkui $ell "the_options".

	bool match_old (MorphInfoCR the_x, MorphInfoCR the_y) const {
		bool x_match = 
			x.heleqdiber_xajuv? x.match(the_x): newx.heleqdiber_xajuv? !newx.match(the_x): true;
			//(x.heleqdiber_xajuv? x.match(the_x): true) && 
			//(newx.heleqdiber_xajuv? !newx.match(the_x): true);
		bool y_match =  
			y.heleqdiber_xajuv? y.match(the_y): newy.heleqdiber_xajuv? !newy.match(the_y): true;
			//(y.heleqdiber_xajuv? y.match(the_y): true) && 
			//(newy.heleqdiber_xajuv? !newy.match(the_y): true);
		return x_match && y_match;
	}

	bool match_new (MorphInfoCR the_new_x, MorphInfoCR the_new_y) const {
		if (newx.heleqdiber_xajuv && newy.heleqdiber_xajuv)
			return ::match(newx,newy, the_new_x,the_new_y);
		bool x_match = 
			newx.heleqdiber_xajuv? newx.match(the_new_x): x.heleqdiber_xajuv? !x.match(the_new_x): true;
		bool y_match =  
			newy.heleqdiber_xajuv? newy.match(the_new_y): y.heleqdiber_xajuv? !y.match(the_new_y): true;
			//(y.heleqdiber_xajuv? y.match(the_y): true) && 
			//(newy.heleqdiber_xajuv? !newy.match(the_y): true);
		return x_match && y_match;
	}

	bool match (
		MorphInfoCR the_x, MorphInfoCR the_y, 
		MorphOptionsWithSikuiCR the_x_options, MorphOptionsWithSikuiCR the_y_options, 
		Index& the_newx_index, Index& the_newy_index) const 
	{
		if (!match_old(the_x, the_y)) return false;

		if (tosefetx>0) the_x_options.sader_indexim_lfi_sikui_bseder_yored(x_indexes);
		if (tosefety>0) the_y_options.sader_indexim_lfi_sikui_bseder_yored(y_indexes);

		if (tosefetx>0 && tosefety==0) {
			LOOPVECTOR (;,the_x_options,o) {
				MorphInfoCR the_new_x = the_x_options.info(x_indexes[o]);
				//bool match_new_x = 
					//newx.heleqdiber_xajuv? ::match(newx,y, cur_new_x,the_y): !x.match(cur_new_x);
					//(newx.heleqdiber_xajuv? ::match(newx,y, cur_new_x,the_y): true) &&
					//(x.heleqdiber_xajuv? !x.match(cur_new_x): true);
				if (match_new(the_new_x,the_y)) {
					the_newx_index = x_indexes[o]; 
					goto found_x;
				}
			}
			return false;

			found_x:
			if (the_x_options.sikui(the_newx_index)+tosefetx > the_x_options.greatest_sikui())
				return true;
			else return false;
		}

		else if (tosefety>0 && tosefetx==0) {
			LOOPVECTOR (;,the_y_options,o) {
				MorphInfoCR the_new_y = the_y_options.info(y_indexes[o]);
				//bool match_new_y = 
					//newy.heleqdiber_xajuv? ::match(x,newy, the_x,cur_new_y): !y.match(cur_new_y);
					//(newy.heleqdiber_xajuv? ::match(x,newy, the_x,cur_new_y): true) &&
					//(y.heleqdiber_xajuv? !y.match(cur_new_y): true);
				if (match_new(the_x,the_new_y)) {
					the_newy_index = y_indexes[o]; 
					goto found_y;
				}
			}
			return false;

			found_y:
			if (the_y_options.sikui(the_newy_index)+tosefety > the_y_options.greatest_sikui())
				return true;
			else return false;
		}

		else if (tosefetx>0 && tosefety>0) {
			LOOPVECTOR (;,the_x_options,ox) {
				MorphInfoCR the_new_x = the_x_options.info(x_indexes[ox]);
				LOOPVECTOR (;,the_y_options,oy) {
					MorphInfoCR the_new_y = the_y_options.info(y_indexes[oy]);
					if (match_new(the_new_x,the_new_y)) {
						the_newx_index = x_indexes[ox];
						the_newy_index = y_indexes[oy]; 
						goto found_xy;
					}
				}
			}
			return false;

			found_xy:
			if (
				the_x_options.sikui(the_newx_index)+tosefetx > the_x_options.greatest_sikui() && 
				the_y_options.sikui(the_newy_index)+tosefety > the_y_options.greatest_sikui()   ) 
					return true;
			else return false;
		}

		else return false;
	}


	bool match (
		MorphInfoCR the_x, MorphInfoCR the_y, 
		MorphOptionsWithSikuiCR the_x_options, MorphOptionsWithSikuiCR the_y_options, 
		MorphInfo& the_newx, MorphInfo& the_newy) const 
	{
		Index the_newx_index, the_newy_index;
		if (!match(the_x,the_y,the_x_options,the_y_options,the_newx_index,the_newy_index)) return false;
		if (tosefetx>0) the_newx = the_x_options.info(the_newx_index); 
		else            the_newx = the_x;
		if (tosefety>0) the_newy = the_y_options.info(the_newy_index); 
		else            the_newy = the_y;
		return true;
	}


	bool match (
		MorphInfoCR the_x, MorphInfoCR the_y, 
		MorphOptionsWithSikui& the_x_options, MorphOptionsWithSikui& the_y_options) const 
	{
		Index the_newx_index, the_newy_index;
		if (!match(the_x,the_y,the_x_options,the_y_options,the_newx_index,the_newy_index)) return false;
		if (tosefetx>0) {
			if (newx.heleqdiber_xajuv)
				the_x_options.add_bli_nirmul(the_newx_index, +tosefetx);
			else {
				LOOPVECTOR (;,the_x_options,o) {
					MorphInfoCR cur_new_x = the_x_options.info(x_indexes[o]);
					bool match_new_x = !x.match(cur_new_x);
						//(newx.heleqdiber_xajuv? ::match(newx,y, cur_new_x,the_y): true) &&
						//(x.heleqdiber_xajuv? !x.match(cur_new_x): true);
					if (match_new_x) 
						the_x_options.add_bli_nirmul(x_indexes[o], +tosefetx);
				}
			}
			the_x_options.normalize();
		}
		if (tosefety>0) {
			if (newy.heleqdiber_xajuv)
				the_y_options.add_bli_nirmul(the_newy_index, +tosefety);
			else {
				LOOPVECTOR (;,the_y_options,o) {
					MorphInfoCR cur_new_y = the_y_options.info(y_indexes[o]);
					bool match_new_y = !y.match(cur_new_y);
						//(newy.heleqdiber_xajuv? ::match(x,newy, the_x,cur_new_y): true) &&
						//(y.heleqdiber_xajuv? !y.match(cur_new_y): true);
					if (match_new_y) 
						the_y_options.add_bli_nirmul(y_indexes[o], +tosefety);
				}
			}
			the_y_options.normalize();
		}
		return true;
	}

	bool meyutar () const {
		if (!x.heleqdiber_xajuv && !newx.heleqdiber_xajuv && tosefetx>0) return true;
		if (!y.heleqdiber_xajuv && !newy.heleqdiber_xajuv && tosefety>0) return true;
		return equivalent(x,newx) && equivalent(y,newy);
	}


	void set_klum_lo_xajuv (
		MorphInfoCR the_x, MorphInfoCR the_y, 
		MorphInfoCR the_newx, MorphInfoCR the_newy, 
		Sikui the_tosefetx, Sikui the_tosefety ) 
	{
		x.set_hatxala_lo_xajuva(the_x);  
		y.set_sof_lo_xajuv(the_y);  
		newx.set_hatxala_lo_xajuva(the_newx);
		newy.set_sof_lo_xajuv(the_newy);
		tosefetx = the_tosefetx;
		tosefety = the_tosefety;
	}


	void set_xajivut_baseword(bool xajivut_baseword_x, bool xajivut_baseword_y) {
		x.baseword_xajuv = xajivut_baseword_x;
		y.baseword_xajuv = xajivut_baseword_y;
		newx.baseword_xajuv = xajivut_baseword_x;
		newy.baseword_xajuv = xajivut_baseword_y;
	}


	void set_tosefetx(int t) {
			tosefetx=t;
			if (t==0)
				newx=x;
	}

	void set_tosefety(int t) {
			tosefety=t;
			if (t==0)
				newy=y;
	}

	void meen_carik_lhatim () { newx.meen_carik_lhatim=newy.meen_carik_lhatim=true; }
	void mispar_carik_lhatim () { newx.mispar_carik_lhatim=newy.mispar_carik_lhatim=true; }
	void guf_carik_lhatim () { newx.guf_carik_lhatim=newy.guf_carik_lhatim=true; }
	void yidua_carik_lhatim () { newx.yidua_carik_lhatim=newy.yidua_carik_lhatim=true; }


	void ktov_cafuf (ostream& out) const {
		out 
			<< x << " " << y << " => " 
			<< tosefetx << " " << newx << " " << tosefety << " " << newy;
	}

	void ktov_barur (ostream& out, bool ktov_tosafot=false) const {
		if (x.heleqdiber_xajuv) x.ktov_barur (out,false);
		else if (newx.heleqdiber_xajuv) {  out << "LA-";  newx.ktov_barur (out,false); }
		out << "   ";

		if (y.heleqdiber_xajuv) y.ktov_barur (out,false);
		else if (newy.heleqdiber_xajuv) {  out << "LA-";  newy.ktov_barur (out,false); }
		out << "   ==>   ";

		if (newx.heleqdiber_xajuv) newx.ktov_barur (out,false);
		else if (x.heleqdiber_xajuv) {  out << "LA-";  x.ktov_barur (out,false); }
		if (ktov_tosafot && tosefetx>0)  out << "(+" << tosefetx << ")";
		out << "   ";

		if (newy.heleqdiber_xajuv) newy.ktov_barur (out,false);
		else if (y.heleqdiber_xajuv) {  out << "LA-";  y.ktov_barur (out,false); }
		if (ktov_tosafot && tosefety>0)  out << "(+" << tosefety << ")";
	}

	status read (istream& in) {
		skip_comments(in,'%');
		DOr (::read (in,x) );
		DOr (::read (in,y) );
		DOr (testchar(in,'='));
		DOr (testchar (in,'>'));
		DOr (::read (in,tosefetx));
		DOr (::read (in,newx));
		DOr (::read (in,tosefety));
		DOr (::read (in,newy));
		
		return OK;
	}


	Tiqun3(): x(), y(), newx(), newy() { tosefetx=tosefety=0; }
};
typedef const Tiqun3& Tiqun3CR;

Array2<Index> Tiqun3::x_indexes(20), Tiqun3::y_indexes(20);          // ^indeqsim msuddarim lpi sikkui $ell "the_options".


void write (ostream& out, Tiqun3CR theinfo, Format format="") { theinfo.ktov_cafuf(out); }
status read  (istream& in, Tiqun3& theinfo, Format format="") { return theinfo.read (in); }

inline void duplicate (Tiqun3& to, const Tiqun3& from) { to=from; }
inline void free (Tiqun3& m) { };

DEFINE_INLINE_IO_OPERATORS(Tiqun3)






/*****************************************************************/
/*****                   Tiqunim3                             ****/
/*****************************************************************/

class Tiqunim3:  public Array2 <Tiqun3>  {
	static MorphInfo basicx; 
	static MorphInfo basicy; 
	static MorphInfo basicnewx; 
	static MorphInfo basicnewy; 
	static HeleqDiber xhd; 
	static HeleqDiber yhd; 
	static HeleqDiber newxhd;
	static HeleqDiber newyhd;
	//static bool baseword_x_xajuv, baseword_y_xajuv;


	void yacer_tiqunim_0 (Tiqun3 thetiqun) {
		if (thetiqun.meyutar()) return;
		append(thetiqun);
	}

	void yacer_tiqunim__xajivut (Tiqun3 thetiqun) {
		yacer_tiqunim_0 (thetiqun);
	}

	void yacer_tiqunim__hatama_bguf (Tiqun3 thetiqun) {
		yacer_tiqunim__xajivut (thetiqun);
		if (basicnewx.guf()==NONE || basicnewy.guf()==NONE) return;
		if (identical_guf(basicnewx,basicnewy)) {  // nasse lhosif hat^ama b-guf
			thetiqun.guf_carik_lhatim();
			yacer_tiqunim__xajivut (thetiqun);
		}
	}
	void yacer_tiqunim__hatama_bmispar (Tiqun3 thetiqun) {
		yacer_tiqunim__hatama_bguf (thetiqun);
		if (basicnewx.mispar()==NONE || basicnewy.mispar()==NONE) return;
		if (identical_mispar(basicnewx,basicnewy)) {  // nasse lhosif hat^ama b-mispar
			thetiqun.mispar_carik_lhatim();
			yacer_tiqunim__hatama_bguf (thetiqun);
		}
	}
	void yacer_tiqunim__hatama_bmeen (Tiqun3 thetiqun) {
		yacer_tiqunim__hatama_bmispar (thetiqun);
		if (basicnewx.meen()==NONE || basicnewy.meen()==NONE) return;
		if (identical_meen(basicnewx,basicnewy)) {  // nasse lhosif hat^ama b-meen
			thetiqun.meen_carik_lhatim();
			yacer_tiqunim__hatama_bmispar (thetiqun);
		}
	}
	void yacer_tiqunim__hatama_byidua (Tiqun3 thetiqun) {
		if (thetiqun.newx.smikut()==NISMAK) {
			yacer_tiqunim_0(thetiqun);
			return;
		}
		yacer_tiqunim__hatama_bmeen (thetiqun);
		if (basicnewx.yidua()==NONE || basicnewy.yidua()==NONE) return;
		if (identical_yidua(basicnewx,basicnewy)) {  // nasse lhosif hat^ama b-yidua
			thetiqun.yidua_carik_lhatim();
			yacer_tiqunim__hatama_bmeen (thetiqun);
		}
	}
	void yacer_tiqunim_jel_x_wjel_y (Tiqun3 thetiqun) {
		thetiqun.x.heleqdiber_xajuv=false; thetiqun.newx.heleqdiber_xajuv=true;
		thetiqun.y.heleqdiber_xajuv=false; thetiqun.newy.heleqdiber_xajuv=true;
		yacer_tiqunim__hatama_byidua (thetiqun);
		thetiqun.y.heleqdiber_xajuv=true; thetiqun.newy.heleqdiber_xajuv=false;
		yacer_tiqunim_0(thetiqun);

		thetiqun.x.heleqdiber_xajuv=true; thetiqun.newx.heleqdiber_xajuv=false;
		thetiqun.y.heleqdiber_xajuv=true; thetiqun.newy.heleqdiber_xajuv=false;
		yacer_tiqunim_0(thetiqun);
		thetiqun.y.heleqdiber_xajuv=false; thetiqun.newy.heleqdiber_xajuv=true;
		yacer_tiqunim_0(thetiqun);
	}


	void yacer_tiqunim_raq_jel_y (Tiqun3 thetiqun) {
		thetiqun.x.heleqdiber_xajuv=true; thetiqun.newx.heleqdiber_xajuv=true;
		thetiqun.y.heleqdiber_xajuv=false; thetiqun.newy.heleqdiber_xajuv=true;
		yacer_tiqunim__hatama_byidua (thetiqun);
		thetiqun.y.heleqdiber_xajuv=true; thetiqun.newy.heleqdiber_xajuv=false;
		yacer_tiqunim_0(thetiqun);

		thetiqun.x.heleqdiber_xajuv=false; thetiqun.newx.heleqdiber_xajuv=false;
		thetiqun.y.heleqdiber_xajuv=true; thetiqun.newy.heleqdiber_xajuv=false;
		yacer_tiqunim_0(thetiqun);
		thetiqun.y.heleqdiber_xajuv=false; thetiqun.newy.heleqdiber_xajuv=true;
		yacer_tiqunim_0(thetiqun);
	}


	void yacer_tiqunim_raq_jel_x (Tiqun3 thetiqun) {
		thetiqun.y.heleqdiber_xajuv=true; thetiqun.newy.heleqdiber_xajuv=true;
		thetiqun.x.heleqdiber_xajuv=false; thetiqun.newx.heleqdiber_xajuv=true;
		yacer_tiqunim__hatama_byidua (thetiqun);
		thetiqun.x.heleqdiber_xajuv=true; thetiqun.newx.heleqdiber_xajuv=false;
		yacer_tiqunim_0(thetiqun);

		thetiqun.y.heleqdiber_xajuv=false; thetiqun.newy.heleqdiber_xajuv=false;
		thetiqun.x.heleqdiber_xajuv=true; thetiqun.newx.heleqdiber_xajuv=false;
		yacer_tiqunim_0(thetiqun);
		thetiqun.x.heleqdiber_xajuv=false; thetiqun.newx.heleqdiber_xajuv=true;
		yacer_tiqunim_0(thetiqun);
	}


	void yacer_tiqunim_3 (Tiqun3 thetiqun) {
		if (thetiqun.tosefetx>0 && thetiqun.tosefety==0) 
			yacer_tiqunim_raq_jel_x(thetiqun);
		else if (thetiqun.tosefetx==0 && thetiqun.tosefety>0) 
			yacer_tiqunim_raq_jel_y(thetiqun);
		else {
			yacer_tiqunim_jel_x_wjel_y(thetiqun);

			Tiqun3 thetiqun_raq_jel_x = thetiqun;
			thetiqun_raq_jel_x.set_tosefety(0);
			yacer_tiqunim_raq_jel_x(thetiqun_raq_jel_x);

			Tiqun3 thetiqun_raq_jel_y = thetiqun;
			thetiqun_raq_jel_y.set_tosefetx(0);
			yacer_tiqunim_raq_jel_y (thetiqun_raq_jel_y);
		}
	}



public:
	void yacer_tiqunim (Tiqun3CR the_basic_tiqun) {
		truncate();
		basicx = the_basic_tiqun.x;
		basicy = the_basic_tiqun.y;
		basicnewx = the_basic_tiqun.newx;
		basicnewy = the_basic_tiqun.newy;
		xhd = the_basic_tiqun.x.heleqdiber();
		yhd = the_basic_tiqun.y.heleqdiber();
		newxhd = the_basic_tiqun.newx.heleqdiber();
		newyhd = the_basic_tiqun.newy.heleqdiber();
		yacer_tiqunim_3 (the_basic_tiqun);
	}
};


MorphInfo Tiqunim3::basicx; 
MorphInfo Tiqunim3::basicy; 
MorphInfo Tiqunim3::basicnewx; 
MorphInfo Tiqunim3::basicnewy; 
HeleqDiber Tiqunim3::xhd; 
HeleqDiber Tiqunim3::yhd; 
HeleqDiber Tiqunim3::newxhd;
HeleqDiber Tiqunim3::newyhd;



Tiqunim3 tiqunim3;

int mspr_tiqunim () { return tiqunim3.count(); }

/*********                 I/O                        **************/
/*********                 I/O                        **************/

void qra_tiqunim (CStr thepath) {
	ifstream tiqunim_infile;
	cerr << "reading tiqunim file " << thepath << endl;
	DOx(open(thepath,tiqunim_infile));
	skip_comments(tiqunim_infile,'%');
	DOx(read(tiqunim_infile,tiqunim3,Format("P\n*")));
	tiqunim_infile.close();
}


void ktov_tiqunim (CStr thepath, CStr kotrt) {
	ofstream out;
	open_outfile_with_messages(thepath,out,ios::trunc);
	out<<endl;
	if (kotrt!=NULL)
		out<<"% "<<kotrt<<endl;
	writeln(out,tiqunim3,Format("P\n"));
	out.close();
}

void ktov_tiqunim_barur (CStr thepath, CStr kotrt) {
	ofstream out;
	StringTemp thefilename = concat(thepath,'b');
	DO(open(thefilename.str,out,ios::trunc));
	if (kotrt!=NULL)
		out<<"% "<<kotrt<<endl;
	LOOPVECTOR(;,tiqunim3,t) {
		tiqunim3[t].ktov_barur(out,true);
		out<<endl;
	}
	out.close();
}



/*****************************************************************/
/*****                   taqqen nittuxim                      ****/
/*****************************************************************/

extern ofstream tiqunimlogfile;

// zohi funqciya pra@it $ell "CorpusImTiqunim", ^az katavti ^otah kak kdei $e-lo^ yir^u ^otah ba-hagdara $ell ha-maxlaqa.
void taqen_nituxim_lfi (CorpusImTiqunim& thetext, Tiqun3CR the_tiqun, Index start_from_word_num=0)  {
	for (Index w=start_from_word_num; w+1<thetext.hanitux_jelanu_laqlt.count(); ++w) {
		MorphInfo&  hanitux_jelanu_lx = thetext.hanitux_jelanu_laqlt[w];
		MorphInfo&  hanitux_jelanu_ly = thetext.hanitux_jelanu_laqlt[w+1];
		MorphOptionsWithSikui& kol_hanituxim_lx = thetext.kol_hanituxim_laqlt[w];
		MorphOptionsWithSikui& kol_hanituxim_ly = thetext.kol_hanituxim_laqlt[w+1];
		//Index index_hanitux_hamtuqan_lx, index_hanitux_hamtuqan_ly;
		if (!the_tiqun.match (
			hanitux_jelanu_lx, hanitux_jelanu_ly, 
			kol_hanituxim_lx, kol_hanituxim_ly)   ) 
				continue;
		hanitux_jelanu_lx = kol_hanituxim_lx.info_with_greatest_sikui();
		hanitux_jelanu_ly = kol_hanituxim_ly.info_with_greatest_sikui();
	}
}
	
// zohi funqciya pra@it $ell "CorpusImTiqunim", ^az katavti ^otah kak kdei $e-lo^ yir^u ^otah ba-hagdara $ell ha-maxlaqa.
void taqen_nituxim_2 (CorpusImTiqunim& thetext) {
	thetext.hajwe_nituxim();  errorlogfile << thetext.ms_tauyot << endl << endl;
	LOOPVECTOR (;,tiqunim3,t) {
		errorlogfile << tiqunim3[t] << endl;
		taqen_nituxim_lfi (thetext, tiqunim3[t]);
		thetext.hajwe_nituxim();  errorlogfile << thetext.ms_tauyot << endl << endl;
	}
}

void CorpusImTiqunim::taqen_nituxim (Index start_from_word_num) {
	LOOPVECTOR (;,tiqunim3,t) 
		taqen_nituxim_lfi (*this, tiqunim3[t],start_from_word_num);
	ntx_qlt_2(start_from_word_num);
}

/********************************************************************************/
/********************           lmad tiqunim         ****************************/
/********************************************************************************/


CorpusImTiqunim textlimud;

/*************************** tiqun_score ********************************************/
/*************************** tiqun_score ********************************************/


Array2<Index> indexei_tauyot;

static int tiqun_score (MorphInfoCR hanitux_haqodem, MorphInfoCR hanitux_hamtuqan, MorphInfoCR hanitux_hanakon) {
	bool haqodem_haya_nakon = identical(hanitux_haqodem,hanitux_hanakon);
	bool hamtuqan_nakon = identical(hanitux_hamtuqan,hanitux_hanakon);
	if (haqodem_haya_nakon && !hamtuqan_nakon)  return -1;      // ha-tiqun qilqel
	if (!haqodem_haya_nakon && hamtuqan_nakon)  return 1;       // ha-tiqun &azar
	return 0;
}

static int tiqun_score (Tiqun3CR the_tiqun, Index w) {
	MorphInfoCR  hanitux_jelanu_lx = textlimud.hanitux_jelanu_laqlt[w];
	MorphInfoCR  hanitux_jelanu_ly = textlimud.hanitux_jelanu_laqlt[w+1];
	MorphInfoCR  hanitux_hanakon_lx = textlimud.hanitux_hanakon_laqlt[w];
	MorphInfoCR  hanitux_hanakon_ly = textlimud.hanitux_hanakon_laqlt[w+1];
	MorphOptionsWithSikuiCR kol_hanituxim_lx = textlimud.kol_hanituxim_laqlt[w];
	MorphOptionsWithSikuiCR kol_hanituxim_ly = textlimud.kol_hanituxim_laqlt[w+1];
	//Index index_hanitux_hamtuqan_lx, index_hanitux_hamtuqan_ly;
	MorphInfo hanitux_hamtuqan_lx, hanitux_hamtuqan_ly;
	if (!the_tiqun.match (
		hanitux_jelanu_lx, hanitux_jelanu_ly, 
		kol_hanituxim_lx, kol_hanituxim_ly,
		hanitux_hamtuqan_lx, hanitux_hamtuqan_ly )   ) 
			return 0;
	return   tiqun_score (hanitux_jelanu_lx,hanitux_hamtuqan_lx,hanitux_hanakon_lx)
	        +tiqun_score (hanitux_jelanu_ly,hanitux_hamtuqan_ly,hanitux_hanakon_ly);
}

static int tiqun_score (Tiqun3CR the_tiqun) {
	int the_score = 0;
	LOOPVECTOR(;,indexei_tauyot,it) {
		Index ww = indexei_tauyot[it];
		if (the_tiqun.tosefety>0) {
			if (ww>0) the_score += tiqun_score(the_tiqun,ww-1); }
		if (the_tiqun.tosefetx>0) {
			if (ww+1<textlimud.godel_haqlt())	the_score += tiqun_score(the_tiqun,ww); }
	}
	if (the_score<=1) return the_score;

	the_score = 0;
	for (Index w=0; w+1<textlimud.godel_haqlt(); ++w) {
		the_score += tiqun_score (the_tiqun,w);
		if (the_score<=-9) return the_score;
	}
	return the_score;
}



/*************************** yacer_tiqunim ********************************************/
/*************************** yacer_tiqunim ********************************************/

Tiqun3 the_best_tiqun;
int the_best_tiqun_score;
int the_previous_best_tiqun_score;
bool baseword_x_xajuv, baseword_y_xajuv;

void yacer_wktov_tiqunim (Tiqun3& hatiqun) {
	static Tiqunim3 hatiqunim;
	hatiqunim.yacer_tiqunim (hatiqun);
	LOOPVECTOR(;,hatiqunim,t) {
		tiqunimlogfile << hatiqunim[t];
		int ts = tiqun_score(hatiqunim[t]);
		tiqunimlogfile << " " << ts << endl;
		if (ts>the_best_tiqun_score) {
			the_best_tiqun_score = ts;
			the_best_tiqun = hatiqunim[t];
			//if (ts>=the_previous_best_tiqun_score) 
			//	throw "stop searching -- I already found the best tiqun";
		}
	}
}




/*************************************  lmad tiqunim ****************************/
/*************************************  lmad tiqunim ****************************/

void lmad_tiqun () {
	for(Index w=0; w+1<textlimud.godel_haqlt(); ++w) {
		MorphInfoCR hanitux_jelanu_lx = textlimud.hanitux_jelanu_laqlt[w];
		MorphInfoCR hanitux_jelanu_ly = textlimud.hanitux_jelanu_laqlt[w+1];
		MorphInfoCR hanitux_hanakon_lx = textlimud.hanitux_hanakon_laqlt[w];
		MorphInfoCR hanitux_hanakon_ly = textlimud.hanitux_hanakon_laqlt[w+1];
		if (identical(hanitux_jelanu_lx,hanitux_hanakon_lx) && identical(hanitux_jelanu_ly,hanitux_hanakon_ly))  continue;
		tiqunimlogfile 
			<< "M#" << w << " M#" << w+1 
			<< "(" << textlimud.haqlt[w] << " " << textlimud.haqlt[w+1] << "): "
			<< hanitux_jelanu_lx << " " << hanitux_jelanu_ly << "    C\"L    " 
			<< hanitux_hanakon_lx << " " << hanitux_hanakon_ly << endl;
		MorphOptionsWithSikuiCR kol_hanituxim_lx = textlimud.kol_hanituxim_laqlt[w];
		MorphOptionsWithSikuiCR kol_hanituxim_ly = textlimud.kol_hanituxim_laqlt[w+1];
		Sikui sikui_gavoh_bioter_lx = kol_hanituxim_lx.greatest_sikui();
		Sikui sikui_gavoh_bioter_ly = kol_hanituxim_ly.greatest_sikui();
		Sikui sikui_hanitux_hanakon_lx = kol_hanituxim_lx.sikui(hanitux_hanakon_lx);
		if (sikui_hanitux_hanakon_lx==0) continue;       // ha-nittux ha-nakon lo^ nimca^!
		Sikui sikui_hanitux_hanakon_ly = kol_hanituxim_ly.sikui(hanitux_hanakon_ly);
		if (sikui_hanitux_hanakon_ly==0) continue;       // ha-nittux ha-nakon lo^ nimca^!
		Sikui hatosefet_lx = 
			identical(hanitux_jelanu_lx,hanitux_hanakon_lx)? 0:
			sikui_gavoh_bioter_lx - sikui_hanitux_hanakon_lx + 10;
		Sikui hatosefet_ly = 
			identical(hanitux_jelanu_ly,hanitux_hanakon_ly)? 0:
			sikui_gavoh_bioter_ly - sikui_hanitux_hanakon_ly + 10;

		Tiqun3 hatiqun;            
		hatiqun.set_klum_lo_xajuv (
			hanitux_jelanu_lx, hanitux_jelanu_ly, 
			hanitux_hanakon_lx, hanitux_hanakon_ly, 
			hatosefet_lx, hatosefet_ly);
		hatiqun.set_xajivut_baseword(baseword_x_xajuv, baseword_y_xajuv);
		yacer_wktov_tiqunim (hatiqun);
	}
}


/**
 * @param baseword_x_xajuv  ha^im lhityaxxes la-&erek ha-milloni $ell ha-milla ha-ri^$ona b-koll zug.
 * @param baseword_y_xajuv  ha^im lhityaxxes la-&erek ha-milloni $ell ha-milla ha-$niya b-koll zug.
 * @param min_tiqun_score  ha-ciyyun ha-miz&ari $e-carik lihyot l-tiqqun kdei $e-nqabbel ^oto.
 * 	(kkoll $e-"min_tiqun_score" yoter gadol -- yiwwacru paxot tiqqunim).
 */
void lmad_tiqunim (bool baseword_x_xajuv, bool baseword_y_xajuv, int min_tiqun_score) {
	open_tiqunimlogfile(log_path, (baseword_x_xajuv?2:0)+(baseword_y_xajuv?1:0) );

	::baseword_x_xajuv = baseword_x_xajuv;
	::baseword_y_xajuv = baseword_y_xajuv;

	the_previous_best_tiqun_score = textlimud.godel_haqlt();
	open_errorlogfile(log_path);  textlimud.hajwe_nituxim(indexei_tauyot);  errorlogfile.close();
	LOG(tiqunimlogfile,endl << "&K$W I$ B@QS@ " << textlimud.ms_tauyot << " @&WIWT." << endl);
	for(;;) {
		the_best_tiqun_score = 0;
		try {lmad_tiqun();} catch(CStr) {};
		LOG(tiqunimlogfile,endl<<endl<<endl<< "HTIQWN H@WB BIWTR HWA: " << endl << the_best_tiqun << "  (" << "CIWN=" << the_best_tiqun_score << ")" << endl);
		if (the_best_tiqun_score < min_tiqun_score) break;
		tiqunim3.append(the_best_tiqun);
		the_previous_best_tiqun_score = the_best_tiqun_score;
		taqen_nituxim_lfi(textlimud, the_best_tiqun);
		uint ms_tauyot_qodem = textlimud.ms_tauyot;
		open_errorlogfile(log_path);  textlimud.hajwe_nituxim(indexei_tauyot);  errorlogfile.close();
		LOG(tiqunimlogfile,endl << textlimud.ms_tauyot << endl);
		if ((int)(ms_tauyot_qodem-textlimud.ms_tauyot) != the_best_tiqun_score) {
			cerr << "MISMATCH! " << ms_tauyot_qodem-textlimud.ms_tauyot << " != " << the_best_tiqun_score << endl;
		}
	}
	close_tiqunimlogfile();
}


void lmad_tiqunim (CStr input_filename, CStr correct_analysis_filename, Index from_wordnum, Index to_wordnum, int min_tiqun_score) {
	if (from_wordnum >= to_wordnum) {
/*
 * File morph/tiqun3.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/tiqun3.cpp
 */

		cerr << "tiqun3.cpp::lmad_tiqunim: lo^ nilmadim tiqqunim xada$im." << endl;
		return;
	}

	cerr << endl << "limmud tiqqunim: hatxala" << endl;
	textlimud.atxl (2600);
	textlimud.qra_nituxim_nkonim (correct_analysis_filename, from_wordnum, to_wordnum);
	textlimud.qra_qlt (input_filename, from_wordnum, to_wordnum);
	textlimud.ntx_qlt_1();
	textlimud.ntx_qlt_2();

	CStr kotrt = concat(
		concat("qlt=", input_filename).str, ' ',
		concat("nitux-nkon=", correct_analysis_filename).str).finalstr();

	CStr jm_qovc_tiqunim_zmni = concat_path_to_filename (log_path, "tiqunim3temp.ma").finalstr();

	ktov_tiqunim(jm_qovc_tiqunim_zmni, kotrt);
	lmad_tiqunim((bool)false,false, min_tiqun_score); ktov_tiqunim(jm_qovc_tiqunim_zmni, kotrt);
	lmad_tiqunim((bool)false,true, min_tiqun_score); ktov_tiqunim(jm_qovc_tiqunim_zmni, kotrt);
	lmad_tiqunim((bool)true,false, min_tiqun_score); ktov_tiqunim(jm_qovc_tiqunim_zmni, kotrt);
	lmad_tiqunim((bool)true,true, min_tiqun_score); ktov_tiqunim(jm_qovc_tiqunim_zmni, kotrt);
}


#ifdef TEST_TIQUN3
/***************************************************************************/
/**********        TEST TIQUN3                                      ********/
/***************************************************************************/

#include <morph/alghist.h>
#include <morph/morphanl.h>
#include <cpplib/hcs.hxx>

#define LOG_PATH "..\\..\\..\\harc\\"



CorpusImTiqunim textbdiqa; 

/*************************** atxl ********************************************/
/*************************** atxl ********************************************/


void atxl (CStr corpus, CStr correctoutput) {
	if (swtch('t'))
		atxl_global_database (NULL, filename(corpus,"nt1").str );
	else if (swtch('s'))
		atxl_global_database (NULL, filename(corpus,"nts").str );
	else {      
		atxl_global_database (filename(corpus,"txt").str,NULL);
		ktov_global_database (filename(corpus,"nt0").str);
		if (correctoutput)       // read correct analysis file for corpus
			qra_nitux_nakon_global (filename(corpus,"txt").str, filename(correctoutput,"to").str);
		open_logfile(2);  xjv_sikuiim_lkol_hmilim_global();  logfile.close();
		ktov_global_database (filename(corpus,correctoutput? "nt1": "nts").str);
	}
}



void atxl_klali_0 (CStr basicfilename, CStr corpusfilename) {
	initialize_the_analyzer(log_path,log_path);
	atxl_global_database(NULL, LOG_PATH "harc20.nt0");
	qra_mone_nituxim_global(LOG_PATH "harc20.mn1");
	if (corpusfilename!=NULL)
		qra_nitux_nakon_global (filename(corpusfilename,"txt").str,filename(corpusfilename,"to").str);	
	xjv_sikuiim_lkol_hmilim_global();
	//atxl_local_database (filename(basicfilename,"txt").str,NULL);
	//open_logfile(3);  xjv_sikuiim_lkol_hmilim_local(); logfile.close();
	//ktov_local_database (filename(basicfilename,"nts").str);
	qjr_myda_mqomi_lmyda_globli();
}

void atxl_klali(CStr basicfilename) {
	atxl_global_database (NULL, LOG_PATH "harc20.nts");
	atxl_local_database (NULL, filename(basicfilename,"nts").str);
}


void bdoq_tiqunim (CStr input_filename, CStr correct_analysis_filename) {
	textbdiqa.atxl (500);
	textbdiqa.qra_nituxim_nkonim (correct_analysis_filename);
	textbdiqa.qra_qlt (input_filename);
	textbdiqa.ntx_qlt_1();
	textbdiqa.ntx_qlt_2();
	taqen_nituxim_2(textbdiqa);
	LOG(tiqunimlogfile,endl << textbdiqa.ms_tauyot << endl);
}


void main (int argc, char* argv[]) {
	log_path = "..\\..\\..\\harc";
	parse_command_line(argc,argv,1,2,"","qol");
	if (swtch('o')) {
		qra_tiqunim(jm_qovc_tiqunim_zmni);
		ktov_tiqunim_barur(jm_qovc_tiqunim_zmni);
		return;
	}
	if (swtch('l')) {
		atxl_klali_0(variable(0),variable(1));
		open_tiqunimlogfile(log_path);
		lmad_tiqunim(filename(variable(0),"txt").str, filename(variable(0),"to").str);
		tiqunimlogfile.close();
		return;
	}
	if (swtch('q')) {
		qra_tiqunim(log_path);
		atxl_klali_0(variable(0),variable(1));
		open_errorlogfile(log_path);
		bdoq_tiqunim(filename(variable(0),"txt").str, filename(variable(0),"to").str);
		errorlogfile.close();
		return;
	}
}

#endif


