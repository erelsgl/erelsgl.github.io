/*
 * File morph/lexinf.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/lexinf.h
 */

/* lexinf.h -- structure LexicalInfo, to hold information about words in lexicon */

#ifndef __LEXINF_H
#define __LEXINF_H

#include <morph/morphtyp.h>
#include <cpplib/io.h>

/***************************************************************************************/
/*      LexicalInfo																       */
/***************************************************************************************/

inline bool dif(uint a, uint b) { return (a!=0&&b!=0&&a!=b); }

struct LexicalInfoECEM { 
	uint sug_ribui: 4;
	Meen meen: 2;
	LexicalInfoECEM (uint thesugribui=0, uint themeen=NONE) { sug_ribui=thesugribui; meen=themeen; }
	void operator= (uint ZERO)  { sug_ribui=0; meen=NONE; }  
	bool operator== (uint ZERO) const  { return (sug_ribui==0&&meen==NONE); }  
	bool operator!= (LexicalInfoECEM i) const  { return dif(meen,i.meen) || dif(sug_ribui,i.sug_ribui); }  
};
typedef const LexicalInfoECEM& LexicalInfoECEMCR;
inline void write (ostream& out, LexicalInfoECEMCR theinfo, Format format="")
	{	out << hd2char(ECEM) << theinfo.sug_ribui << ':' << meen2char(theinfo.meen); }
inline status read (istream& in, LexicalInfoECEM& theinfo, Format format="") 
	{	skip (in," \t\n"); HeleqDiber hd = char2hd(in.get());
		short s; DOr (read(in,s)); theinfo.sug_ribui=s;
		DOr(testchar(in,':'));
		theinfo.meen = char2meen(in.get());  
		return OK; }

struct LexicalInfoPOAL { 
	uint sug_howe: 4;
	uint sug_atid: 8;
	LexicalInfoPOAL (uint thesughowe=0, uint thesugatid=NONE) { sug_howe=thesughowe; sug_atid=thesugatid; }
	void operator= (uint ZERO)  { sug_howe=sug_atid=0; }  
	bool operator== (uint ZERO) const  { return (sug_howe==0&&sug_atid==0); }  
	bool operator!= (LexicalInfoPOAL i) const  { return dif(sug_howe,i.sug_howe)||dif(sug_atid,i.sug_atid); }
};
typedef const LexicalInfoPOAL& LexicalInfoPOALCR;
inline void write (ostream& out, LexicalInfoPOALCR theinfo, Format format="")
	{	out << hd2char(POAL) << theinfo.sug_howe << ':' << theinfo.sug_atid; }
inline status read (istream& in, LexicalInfoPOAL& theinfo, Format format="") 
	{	skip (in," \t\n"); HeleqDiber hd = char2hd(in.get());
		short s; 
		DOr (read(in,s)); theinfo.sug_howe=s;
		DOr(testchar(in,':'));
		DOr (read(in,s)); theinfo.sug_atid=s;
		return OK; }

struct LexicalInfoTOAR { 
	uint sug_toar: 2;
	LexicalInfoTOAR (uint thesugtoar=0) { sug_toar=thesugtoar; }
	void operator= (uint ZERO)  { sug_toar=0; }  
	bool operator== (uint ZERO) const  { return (sug_toar==0); }  
	bool operator!= (LexicalInfoTOAR i) const  { return dif(sug_toar,i.sug_toar); }  
};
typedef const LexicalInfoTOAR& LexicalInfoTOARCR;
inline void write (ostream& out, LexicalInfoTOARCR theinfo, Format format="")
	{	out << hd2char(TOAR) << theinfo.sug_toar; }
inline status read (istream& in, LexicalInfoTOAR& theinfo, Format format="") 
	{	skip (in," \t\n"); HeleqDiber hd = char2hd(in.get());
		short s; DOr (read(in,s)); theinfo.sug_toar=s;
		return OK;  }

struct LexicalInfoJEMPRATI { 
	Meen meen: 2;
	LexicalInfoJEMPRATI (uint themeen=NONE) { meen=themeen; }
	void operator= (uint ZERO)  { meen=NONE; }  
	bool operator== (uint ZERO) const  { return (meen==NONE); }  
	bool operator!= (LexicalInfoJEMPRATI i) const  { return dif(meen,i.meen); }  
};
typedef const LexicalInfoJEMPRATI& LexicalInfoJEMPRATICR;
inline void write (ostream& out, LexicalInfoJEMPRATICR theinfo, Format format="")
	{	out << hd2char(JEM_PRATI) << meen2char(theinfo.meen); }
inline status read (istream& in, LexicalInfoJEMPRATI& theinfo, Format format="") 
	{	skip (in," \t\n"); HeleqDiber hd = char2hd(in.get());
		theinfo.meen = char2meen(in.get());  
		return OK; }

struct LexicalInfoMILIT { 
	HeleqDiber hd;
	LexicalInfoMILIT (HeleqDiber thehd=NO_HD) { hd=thehd; }
	LexicalInfoMILIT (uint ZERO) { hd=NO_HD; }
	void operator= (uint ZERO)  { hd=NO_HD; }  
	bool operator== (uint ZERO) const  { return (hd==NO_HD); }  
	bool operator!= (LexicalInfoMILIT i) const  { return dif(hd,i.hd); }
};
typedef const LexicalInfoMILIT& LexicalInfoMILITCR;
inline void write (ostream& out, LexicalInfoMILITCR theinfo, Format format="")
	{	out << hd2char(theinfo.hd); }
inline status read (istream& in, LexicalInfoMILIT& theinfo, Format format="") 
	{	skip (in," \t\n"); theinfo.hd = char2hd(in.get());
		return OK; }

#endif

