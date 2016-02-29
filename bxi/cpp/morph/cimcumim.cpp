/*
 * File morph/cimcumim.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/cimcumim.cpp
 */

/* cimcumim.cpp -- cimcumim &abur mnattex taxbiri */

#include <morph/mip2.h>
#include <morph/sntncinf.h>
#include <morph/cimcumim.h>
#include <morph/hajlamot.h>
#include <math.h>

bool lapoal_yej_nose (MorphInfoCR the_poal) {
	if (the_poal.nismak()) return true;
	if (the_poal.meen()==BOTH&&the_poal.mispar()==BOTH&&the_poal.guf()==BOTH) return true;
	return false;
}

bool lapoal_yej_musa_yajir (MorphInfoCR the_poal) {
	return (strinstring("-AT",the_poal.baseword()));
}

double ciyun_cimcumi (MorphInfoCR the_poal) {
	CStr bw = the_poal.baseword();    Size bwlen = strlen(bw);
	Index i = charindex ('+',bw);
	if (i<bwlen && isdigit(bw[i+1]))
		return (bw[i+1]-'0');
	i = charindex ('-',bw);
	if (i<bwlen && isdigit(bw[i+1]))
		return (-(bw[i+1]-'0'));
	return 0;
}

bool laecem_yej_tmura (MorphInfoCR the_ecem) {
	if (strinstring("()",the_ecem.baseword())) return true;
	return false;
}

bool laecem_yej_toar (MorphInfoCR the_ecem) {
	if (strinstring("-:",the_ecem.baseword())) return true;
	return false;
}

bool laecem_yej_jnei_tearim (MorphInfoCR the_ecem) {
	if (strinstring("-:-:",the_ecem.baseword())) return true;
	return false;
}

bool milat_yaxas_jelo_ykola_lhajlim_ecem (MorphInfoCR the_mila) {
	CStr hamila = the_mila.baseword();
	return identical(hamila,"AT")||identical(hamila,"AIN")||identical(hamila,"LBD");
}

bool milat_yaxas_jelo_ykola_lhajlim_poal (MorphInfoCR the_mila) {
	CStr hamila = the_mila.baseword();
	return identical(hamila,"$L")||identical(hamila,"AIN")||identical(hamila,"LBD");
}

bool milat_yaxas_jelo_ykola_lhajlim_poal (MorphInfoCR the_mila, MorphInfoCR the_poal) {
	CStr hamila = the_mila.baseword();
	if (identical(hamila,"$L")||identical(hamila,"AIN")||identical(hamila,"LBD")) return true;
	if (identical(hamila,"AT")&&lapoal_yej_musa_yajir(the_poal)) return true;
	return false;
}



bool maqaf (MorphInfoCR the_mila) {
	return the_mila.baseword()[0]=='-';
}

bool sfarot (MorphInfoCR the_mila) {
	return the_mila.baseword()[1]=='#';
}


/******************************   XoqCimcum   ***********************************************/
/******************************   XoqCimcum   ***********************************************/
/******************************   XoqCimcum   ***********************************************/




bool XoqCimcum::match (SentenceInfoCR ha_mijpat, Index ha_hatxala) const {
	if (ha_hatxala+ork()-1 >= ha_mijpat.count()) return false;
	LOOPVECTOR(;,reija,t)  if (!reija[t].match(ha_mijpat[ha_hatxala+t])) return false;
	MorphInfoPatternCR w=reija[0];
	MorphInfoCR 
		the_w=ha_mijpat[ha_hatxala+0],  
		the_x=(ork()>=2? ha_mijpat[ha_hatxala+1]: miNQUDA),
		the_y=(ork()>=3? ha_mijpat[ha_hatxala+2]: miNQUDA);

	/* miqrim myuxadim */
	switch (sug) {
	case 3: case 6: // POAL TOARPOAL  ^o  TOAR TOARPOAL
		if (the_x.hu("LA")) return false; break;
	case 22: // MILATYAXAS ECEM
		if (the_w.hu("AT") && !the_x.meyuda())  return false; break;
	//case 24: case 25: case 26: case 27:  // OTYAXAS-xxx
	//	if (the_w.otiotyaxas()==NO_OY) return false; break;
	case 32:       // MILATYAXAS W MILATYAXAS
		if (!identical(the_w.baseword(),the_x.baseword())) return false; break;
	case 33:   {   // MILATYAXAS AW MILATYAXAS
		MorphInfoCR the_y = ha_mijpat[ha_hatxala+2];
		if (!identical(the_w.baseword(),the_y.baseword())) return false; break;}
	//case 42: // POAL ECEM [musa^ ya$ir]
	//	if (the_x.meyuda())  return false; break;
	case 41: // POAL MILATYAXAS
		if (milat_yaxas_jelo_ykola_lhajlim_poal(the_x,the_w)) return false; break;
	case 42: // POAL ECEM[musa-yajir]
		if (lapoal_yej_musa_yajir(the_w)) return false; break;
	case 43: // MILATYAXAS POAL 
		if (milat_yaxas_jelo_ykola_lhajlim_poal(the_w,the_x)) return false; break;
	case 44: // MILATYAXAS , POAL 
		if (the_w.hu("AT")||milat_yaxas_jelo_ykola_lhajlim_poal(the_w,the_y)) return false; break;
	case 53: // ECEM MILATYAXAS
		if (milat_yaxas_jelo_ykola_lhajlim_ecem(the_x)) return false; break;
	case 91: // ECEM [nose] POAL 
		if (lapoal_yej_nose(the_x)) return false; break;
	case 92: // POAL ECEM [nose]
		if (lapoal_yej_nose(the_w)) return false; break;
	}
	if ( laecem_yej_tmura(the_w) && (sug==10||sug==11||sug==15||sug==18||sug==52||sug==53) )  return false; 
		// axrei tmura lo^ yabo^u:   to^ar,   milat-gup, tmura, tmura,   $ell,    cerup-yaxs.
	if ( laecem_yej_jnei_tearim(the_w) && (sug==10)) return false; 
		// axrei 2 tearim lo^ yabo^ &od to^ar
	

	if (ork()==2) {          // b-xuqqim m-^ork 2 carik libdoq hat^ama bein w l x
		MorphInfoPatternCR x=reija[1];
		if (!yej_hatama(w,x,the_w,the_x)) return false;
	}
	else if (ork()==3) {     // b-xuqqim m-^ork 3 carik libdoq hat^ama bein w l-y ^o bein w l-x
		MorphInfoPatternCR x=reija[1], y=reija[2];
		if (sug==103) {      // POALEZER  nose^  $EMPOAL
			if (!yej_hatama(w,x,the_w,the_x)) return false;
		}
		else {
			if (!yej_hatama(w,y,the_w,the_y)) return false;
		}
	}
	return true;
}


double XoqCimcum::ciyun_cimcumi (MorphInfoCR the_w, MorphInfoCR the_x, MorphInfoCR the_y) const {
if (sug<41||43<sug) return 0;

	double hasikui_hayaxasi;
//	if (sug==41||sug==42)                   // POAL HA$LAMA
//		hasikui_hayaxasi = mone_hajlamot_pealim.sikui_yaxasi_mtuqan(the_w,the_x);
//	else if (sug==43)
//		hasikui_hayaxasi = mone_hajlamot_pealim.sikui_yaxasi_mtuqan(the_x,the_w);
	if (sug==42 && the_x.meyuda())          // POAL ECEMMYUDA  [bli '^et'!]
		hasikui_hayaxasi = 0.0025;
	else
		hasikui_hayaxasi = 0.1;

	if (hasikui_hayaxasi==0)
		return 0;
	else if (hasikui_hayaxasi<0.05 || 0.2<hasikui_hayaxasi) 
		return log10(hasikui_hayaxasi*10);
	else
		return 0;
}


bool XoqCimcum::match (SentenceInfoCR ha_mijpat, Index ha_hatxala, MorphInfo& ha_tocaa, double* ha_ciyun_p) const {
	if (!match(ha_mijpat,ha_hatxala)) return false;
	MorphInfoPatternCR w=reija[0], x=(ork()>=2? reija[1]: reija[0]);
	MorphInfoCR 
		the_w=ha_mijpat[ha_hatxala+0],  
		the_x=(ork()>=2? ha_mijpat[ha_hatxala+1]: miNQUDA),
		the_y=(ork()>=3? ha_mijpat[ha_hatxala+2]: miNQUDA);
	ha_tocaa = ha_mijpat[ha_hatxala+index_bsis_hatocaa];
	hattel (ha_tocaa, seifa);

	// set siomet of tocaa
	switch (sug) {
	case 13: case 16: {  // NISMAK SOMEK
		MorphInfoCR the_somek  = ha_mijpat[ha_hatxala+ork()-1];
		MorphInfoCR the_nismak = ha_mijpat[ha_hatxala];
		ha_tocaa.setyidua (the_somek.yidua());
		if (sug==16 && the_nismak.mispar()!=BOTH)   // MISPAR ECEM
			ha_tocaa.setmispar (the_nismak.mispar());
		break;}
	case 21:            // MILATYAXAS JEMPRATI
		ha_tocaa.setsiomet(the_x.meen(),YAXID,GUF3); break;
	case 22:            // MILATYAXAS ECEM
		ha_tocaa.setsiomet(the_x.meen(),the_x.mispar(),GUF3); break;
	case 23:            // MILATYAXAS MILATGUF
		ha_tocaa.setsiomet(the_x.meen(),the_x.mispar(),the_x.guf()); break;
	case 25:            // OTYAXAS JEMPRATI
		ha_tocaa.setsiomet(the_w.meen(),YAXID,GUF3); break;
	case 26:            // OTYAXAS ECEM
		ha_tocaa.setsiomet(the_w.meen(),the_w.mispar(),GUF3); break;
	case 27:            // OTYAXAS MILATGUF
		ha_tocaa.setsiomet(the_w.meen(),the_w.mispar(),the_w.guf()); break;
	case 91: case 92: case 103: case 113:       // POAL + nose
		ha_tocaa.setsmikut(NISMAK);             // po&l nismak == po&l &imm no$e^
	}

	// set txiliot of tocaa
	ha_tocaa.setvav(the_w.vav());
	ha_tocaa.setjiabud(the_w.jiabud());
	if (sug==24 || sug==25 || sug==26 || sug==27)    // OTYAXAS-xxx
		ha_tocaa.setotiotyaxas(NO_OY);
	else if (sug==61)        // RQ ====, GM ====, ...
		ha_tocaa.setotiotyaxas(the_x.otiotyaxas());
	else
		ha_tocaa.setotiotyaxas(the_w.otiotyaxas());



	// set baseword of tocaa
	if (sug==24 || sug==25 || sug==26 || sug==27) {   // OTYAXAS-xxx
		ha_tocaa.setbaseword (maxrozet_otiotyaxas(the_w.otiotyaxas()));
	}
	else if (seifa.baseword_xajuv) {
		StringTemp the_new_baseword(50);
		for (Index i=0; i<strlen(seifa.baseword()); ++i) {
			uchar c = seifa.baseword()[i];
			if ('a'<=c && c<'a'+ork()) {
				DOx(the_new_baseword.append(
					ha_mijpat[ha_hatxala+c-'a'].baseword()));
			}
			else {
				DOx(the_new_baseword.append(c));
			}
		}
		if ( (sug==41&&the_x.hu("AT")) || 
			 (sug==43&&the_w.hu("AT"))    )
			 the_new_baseword.append("-AT");
		ha_tocaa.setbaseword(the_new_baseword.str);
	}

	if (ha_ciyun_p!=NULL) 
		*ha_ciyun_p = ciyun_cimcumi (the_w, the_x, the_y);
	return true;
}




void XoqCimcum::atxel_et_index_bsis_hatocaa() {
	/* atxel ^et index_bsis_hatocaa */
	if (sug==101 || sug==103)                   // POALEZER MAQOR
		index_bsis_hatocaa = 0;
	else if (sug==61)                   // RQ ====, GM ====, ...
		index_bsis_hatocaa = 1;
	else {
		index_bsis_hatocaa = 0;
		LOOPVECTOR(;,reija,t)
			if (seifa.heleqdiber()==reija[t].heleqdiber()) {
				index_bsis_hatocaa = t; break; }
	}
}


status XoqCimcum::read  (istream& in) {
	skip_comments(in,'%');
	reija.truncate();
	DOr (::read (in,sug));
	for(;;) {
		MorphInfoPattern cur_mip;
		DOr (::read(in,cur_mip));
		reija.append(cur_mip);
		if (testchar(in,'>')==OK)   break;
	}
	DOr (::read (in,seifa) ); 
	atxel_et_index_bsis_hatocaa();
	return OK;
}


void XoqCimcum::ktov_barur (ostream& out) const {
	out << "(QBWCH " << sug << ")";
	LOOPVECTOR(;,reija,i) {
		out << "   ";
		reija[i].ktov_barur(out, i>0 );
	}
	out << endl << "                  ==>   ";
	MorphInfoPattern seifa_zmanit = seifa;
	seifa_zmanit.baseword_xajuv = false;
	seifa_zmanit.ktov_barur(out, true);
}


Vector1<XoqCimcum> hacimcumim;


status qra_cimcumim (CStr path_to_cimcumim) {
	ifstream cimcumim_infile;
	cerr << "reading cimcumim file " << path_to_cimcumim << endl;
	DOr(open(path_to_cimcumim,cimcumim_infile));
	skip_comments(cimcumim_infile,'%');
	DOr(read(cimcumim_infile, hacimcumim, Format("P\n")));
	cimcumim_infile.close();
	LOOPVECTOR(;,hacimcumim,c) {
		XoqCimcumCR xc = hacimcumim[c];
		if ((xc.sug==0||xc.sug==23) && xc.ork()==2) {
			XoqCimcum new_xc;
			duplicate (new_xc,xc);
			DOr(new_xc.hosef_maqaf());
			DOr(hacimcumim.append(new_xc));
		}
	}
	return OK;
}

void ktov_cimcumim_barur (CStr path_to_cimcumim) {
	ofstream out;
	StringTemp thefilename = concat(path_to_cimcumim, 'b');
	DO(open(thefilename.str,out,ios::trunc));
	hacimcumim[0].ktov_barur(out);
	for(Index c=1; c<hacimcumim.count(); ++c) {
		out<<endl;
		if (hacimcumim[c-1].sug != hacimcumim[c].sug)  out << endl;
		if (hacimcumim[c-1].sug/10 != hacimcumim[c].sug/10)  out << endl;
		hacimcumim[c].ktov_barur(out);
	}
	out.close();
}

