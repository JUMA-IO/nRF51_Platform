#ifndef _BOARD_CONFIG_UICR_H_
#define _BOARD_CONFIG_UICR_H_

#include "nrf51.h"

#define BOARD_CONFIG_VALUE_ENABLE				(0x5AA55AA5UL)
#define BOARD_CONFIG_VALUE_DISABLE			(0xFFFFFFFFUL)

typedef struct _user_information_config_t_
{
	/*************************/
  uint32_t CLENR0;
	uint32_t PBPCONF;
	uint64_t XTALFREQ;
	uint32_t FWID;
	uint32_t BOOTLOADERADDR;
	/**************************/
	uint32_t NRFFW[14];
	uint32_t NRFHW[12];
	/**************************/
	uint32_t LFCLKSRC_EN;
	uint32_t LFCLKSRC;
	uint32_t CUSTOMER[28];
	/**************************/
}user_information_config_t;


enum LFCLKSRC
{
  LFCLKSRC_SYNTH_250_PPM,                       /**< LFCLK Synthesized from HFCLK.                    */
  LFCLKSRC_XTAL_500_PPM,                        /**< LFCLK crystal oscillator 500 PPM accuracy.       */
  LFCLKSRC_XTAL_250_PPM,                        /**< LFCLK crystal oscillator 250 PPM accuracy.       */
  LFCLKSRC_XTAL_150_PPM,                        /**< LFCLK crystal oscillator 150 PPM accuracy.       */
  LFCLKSRC_XTAL_100_PPM,                        /**< LFCLK crystal oscillator 100 PPM accuracy.       */
  LFCLKSRC_XTAL_75_PPM,                         /**< LFCLK crystal oscillator 75 PPM accuracy.        */
  LFCLKSRC_XTAL_50_PPM,                         /**< LFCLK crystal oscillator 50 PPM accuracy.        */
  LFCLKSRC_XTAL_30_PPM,                         /**< LFCLK crystal oscillator 30 PPM accuracy.        */
  LFCLKSRC_XTAL_20_PPM,                         /**< LFCLK crystal oscillator 20 PPM accuracy.        */
  LFCLKSRC_RC_250_PPM_250MS_CALIBRATION,        /**< LFCLK RC oscillator, 250ms  calibration interval.*/
  LFCLKSRC_RC_250_PPM_500MS_CALIBRATION,        /**< LFCLK RC oscillator, 500ms  calibration interval.*/
  LFCLKSRC_RC_250_PPM_1000MS_CALIBRATION,       /**< LFCLK RC oscillator, 1000ms calibration interval.*/
  LFCLKSRC_RC_250_PPM_2000MS_CALIBRATION,       /**< LFCLK RC oscillator, 2000ms calibration interval.*/
  LFCLKSRC_RC_250_PPM_4000MS_CALIBRATION,       /**< LFCLK RC oscillator, 4000ms calibration interval.*/
  LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION,       /**< LFCLK RC oscillator, 8000ms calibration interval.*/
  LFCLKSRC_RC_250_PPM_TEMP_1000MS_CALIBRATION,  /**< LFCLK RC oscillator. Temperature checked every 1000ms, if changed above a threshold, a calibration is done.*/
  LFCLKSRC_RC_250_PPM_TEMP_2000MS_CALIBRATION,  /**< LFCLK RC oscillator. Temperature checked every 2000ms, if changed above a threshold, a calibration is done.*/
  LFCLKSRC_RC_250_PPM_TEMP_4000MS_CALIBRATION,  /**< LFCLK RC oscillator. Temperature checked every 4000ms, if changed above a threshold, a calibration is done.*/
  LFCLKSRC_RC_250_PPM_TEMP_8000MS_CALIBRATION,  /**< LFCLK RC oscillator. Temperature checked every 8000ms, if changed above a threshold, a calibration is done.*/
  LFCLKSRC_RC_250_PPM_TEMP_16000MS_CALIBRATION, /**< LFCLK RC oscillator. Temperature checked every 16000ms, if changed above a threshold, a calibration is done.*/
};



#endif //_BOARD_CONFIG_UICR_H_
