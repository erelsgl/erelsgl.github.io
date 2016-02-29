/*
 * File morph/hpmainit.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/hpmainit.cpp
 */

/* hpmainit.cpp -- HPMA initialization routines */

#include "hpmainit.h"

#include <cpplib/argsfile.h>
#include <morph/alghist.h>
#include <morph/tiqun3.h>

extern bool plt_cfuf, plt_brur, plt_hstbruyot;     // qorpus.cpp
extern bool lexicon_is_creative;                   // lexicon.cpp
extern bool ktov_et_kl_tocaot_jlv_hmila;           // alghist.cpp
extern CStr log_path;                              // hpmaio.cpp

CStr lex_path;
CStr mamr_lnitux_al_ydi_mjtmj_qlt, mamr_lnitux_al_ydi_mjtmj_plt, mamr_lnitux_al_ydi_tokna_qlt, mamr_lnitux_al_ydi_tokna_plt;
CStr qorpus_lo_mnutx, qorpus_mnutx_qlt, qorpus_mnutx_nitux_nkon, qorpus_input_data_from_previous_analysis, qorpus_output_data_from_this_analysis;


void process_arguments_file (int argc, char* argv[])
{
	if (argc!=2)
		exit_with_error ("wrong command-line format", "", "mcht <arguments-file-name>");

	read_arguments_file(argv[1]);

	lex_path = string_argument("MslulMilon", ".");
	log_path = string_argument("MslulLogim", ".");

	qorpus_lo_mnutx = concat_filename_to_extension(string_argument("QorpusLoMnutx") ,"txt").finalstr();
	qorpus_mnutx_qlt = concat_filename_to_extension(string_argument("QorpusMnutx") ,"txt").finalstr();
	qorpus_mnutx_nitux_nkon = concat_filename_to_extension(string_argument("QorpusMnutx") ,"to").finalstr();

	if (string_argument("MamrLNituxAlYdiMjtmj",NULL)) {
		mamr_lnitux_al_ydi_mjtmj_qlt = concat_filename_to_extension(string_argument("MamrLNituxAlYdiMjtmj") ,"txt").finalstr();
		mamr_lnitux_al_ydi_mjtmj_plt = concat_filename_to_extension(string_argument("MamrLNituxAlYdiMjtmj") ,"out").finalstr();
	}
	else
		mamr_lnitux_al_ydi_mjtmj_qlt = mamr_lnitux_al_ydi_mjtmj_plt = NULL;

	if (string_argument("MamrLNituxAlYdiTokna",NULL)) {
		mamr_lnitux_al_ydi_tokna_qlt = concat_filename_to_extension(string_argument("MamrLNituxAlYdiTokna") ,"txt").finalstr();
		mamr_lnitux_al_ydi_tokna_plt = concat_filename_to_extension(string_argument("MamrLNituxAlYdiTokna") ,"out").finalstr();
	}
	else
		mamr_lnitux_al_ydi_tokna_qlt = mamr_lnitux_al_ydi_tokna_plt = NULL;

	//NT1 file - generated after a "word phase" analysis using an analyzed qorpus.
	//Note that the NT1 file doesn't contain the correct analyses themselves,
	//	so using a ready NT1 file (using -t) will be much less accurate than performing the word phase
	//	each time (using -o <name>).
	if (string_argument("SyomtQovcQltTocaotJlvMila", NULL))
		qorpus_input_data_from_previous_analysis = concat_filename_to_extension(
			string_argument("QorpusLoMnutx"), string_argument("SyomtQovcQltTocaotJlvMila")).finalstr();
	else
		qorpus_input_data_from_previous_analysis = NULL;

	if (string_argument("SyomtQovcPltTocaotJlvMila", NULL))
		qorpus_output_data_from_this_analysis = concat_filename_to_extension(
			string_argument("QorpusLoMnutx"), string_argument("SyomtQovcPltTocaotJlvMila")).finalstr();
	else
		qorpus_output_data_from_this_analysis = NULL;

	plt_cfuf = int_argument("PltCfuf",0)!=0;
	plt_brur = int_argument("PltBrur",0)!=0;
	plt_hstbruyot = int_argument("PltHstbruyot",0)!=0;
	lexicon_is_creative = int_argument("MilonYcirti",0)!=0;
	ktov_et_kl_tocaot_jlv_hmila = int_argument("KtovEtKlTocaotJlvHMila",0)!=0;
}


void atxl_jlv_mila() {
	if (qorpus_input_data_from_previous_analysis)
		atxl_global_database (NULL,qorpus_input_data_from_previous_analysis);  // read previously saved NTS/NT1 file
	else
		atxl_global_database(qorpus_lo_mnutx,NULL);                                // read and analyze a raw TXT file

	if (int_argument("TxiltXlqImunJlvMila") < int_argument("SofXlqImunJlvMila"))
		qra_nitux_nakon_global (qorpus_mnutx_qlt, qorpus_mnutx_nitux_nkon, 
			int_argument("TxiltXlqImunJlvMila"), int_argument("SofXlqImunJlvMila"));

	if (
		!qorpus_input_data_from_previous_analysis || 
		int_argument("TxiltXlqImunJlvMila") < int_argument("SofXlqImunJlvMila") ) 
			xjv_sikuiim_lkol_hmilim_global();

	if (qorpus_output_data_from_this_analysis)
		ktov_global_database (qorpus_output_data_from_this_analysis); // generate NT1 file
}


void atxl_jlv_zug() {
	if (int_argument("TxiltXlqImunJlvZug") < int_argument("SofXlqImunJlvZug")) {
		lmad_tiqunim (qorpus_mnutx_qlt, qorpus_mnutx_nitux_nkon, 
			int_argument("TxiltXlqImunJlvZug"), int_argument("SofXlqImunJlvZug"), int_argument("CyunQtnByotrLTiqun",2));

		CStr kotrt = concat(
			concat("qlt=", qorpus_mnutx_qlt).str, ' ',
			concat("nitux-nkon=", qorpus_mnutx_nitux_nkon).str).finalstr();
		if (string_argument("QovcTiqunim",NULL))
			ktov_tiqunim (string_argument("QovcTiqunim"));
		else
			ktov_tiqunim (concat_path_to_filename(lex_path, "tiqunim3.ma").finalstr(), kotrt);
	}

	if (string_argument("QovcTiqunim",NULL))
		qra_tiqunim(string_argument("QovcTiqunim"));
}

