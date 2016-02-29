/* morphtav .h-- tavniot morfologiot */

#ifndef __MORPHTAV_H
#define __MORPHTAV_H

#include <morph/tavniot.h>


bool ends_with (StringTempCR theword, char c);
bool ends_with (TavnitCR thetavnit, char c);
bool btavnit_hifiil (StringTempCR theword);
bool btavnit_hifiil (TavnitCR thetavnit);


#define NUM_OF_BINYANIM 6

extern Tavniot tavniot_jayakut_ecem;
extern Tavniot tavniot_ribui_ecem;
extern Tavniot tavniot_toar;
extern Tavniot tavniot_poal_avar;
extern Tavniot tavniot_beinony;
extern Tavniot txiliot_poal_atid[NUM_OF_BINYANIM];
extern Tavniot hemjeky_poal_atid_ZY[NUM_OF_BINYANIM];  // ZAKAR YAXID
extern Tavniot hemjeky_poal_atid_NY[NUM_OF_BINYANIM];  // neqeva YAXID
extern Tavniot hemjeky_poal_atid_ZR[NUM_OF_BINYANIM];  // ZAKAR rabim
extern Tavniot hemjeky_poal_atid_NR[NUM_OF_BINYANIM];  // neqeva rabim
//extern Tavnit MIUXD_maqor_gizrat_l_h    ;
extern Tavnit MIUXD_maqor_gizrat_p_i    ;
extern Tavnit MIUXD_ciwui_binyan_hifiil_3 ;
extern Tavnit MIUXD_ciwui_binyan_hifiil_2 ;



status read_the_tavniot (CStr thepath);
void write_the_tavniot (CStr thepath);

#endif
