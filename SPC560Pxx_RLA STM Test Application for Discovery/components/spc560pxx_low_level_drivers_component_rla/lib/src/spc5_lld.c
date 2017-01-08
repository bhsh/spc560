/*
    SPC5 RLA - Copyright (C) 2015 STMicroelectronics

    Licensed under the Apache License, Version 2.0 (the "License").
    You may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    spc5_lld.c
 * @brief   SPC5xx subsystem low level driver code.
 *
 * @addtogroup SPC5_LLD
 * @{
 */

#include "spc5_lld.h"
#include "spc5_lld_cfg.h"

#if (LLD_USE_PIT == TRUE)
#include "pit_lld_cfg.h"
#endif

#if (LLD_USE_PIT == TRUE) || defined(__DOXYGEN__)
/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   System time.
 */
static systime_t systime;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   System Time Initialization 
 * @note  System time is used only if PIT is enabled. PIT0 is used as System Timer
 * @notapi
 */
static void sysTimeInit(void)
{
  
  /* System timer initialization.*/
  systime = 0;
  
  /* PIT channel 0 initialization for system ticks */
  pit_lld_start(&PITD, pit_config);

  /* PIT channel 0 start for system ticks */
  pit_lld_channel_start(&PITD,PIT_CHANNEL_CH0);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   system tyme counter increment (callback function for PIT timer)
 *
 * @isr
 */
void sysTimeCounter(void) {

  systime++;
  
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/
 /**
 * @brief   SPC5xx subsystem low level driver initialization.
 *
 * @api
 */
void spc5LLDInit(void) {
  
  /* EDMA initialization.*/
  edmaInit();

#if (LLD_USE_PIT == TRUE) || defined(__DOXYGEN__)
  pit_lld_init();
  /* System Time initialization is only possbile when pit is enabled */
  sysTimeInit();
#endif 
#if (LLD_USE_ICU == TRUE) || defined(__DOXYGEN__)
  icu_lld_init();
#endif
#if (LLD_USE_PWM == TRUE)|| defined(__DOXYGEN__)
  pwm_lld_init();
#endif  
#if (LLD_USE_CAN  == TRUE) || defined(__DOXYGEN__)
  can_lld_init();
#endif
#if (LLD_USE_SERIAL == TRUE) || defined(__DOXYGEN__)
  sd_lld_init();
#endif
#if (LLD_USE_SPI == TRUE) || defined(__DOXYGEN__)
  spi_lld_init();
#endif
#if (LLD_USE_ADC == TRUE) || defined(__DOXYGEN__)
  adc_lld_init();
#endif
#if (LLD_USE_CTU == TRUE) || defined(__DOXYGEN__)
  ctu_lld_init();
#endif
#if (LLD_USE_STM == TRUE) || defined(__DOXYGEN__)
  stm_lld_init();
#endif
} 

#if (LLD_USE_PIT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Delays for the specified number of milliseconds.
 *
 * @param[in] msec      time in milliseconds
 *
 * @notapi
 */
#ifdef __ghs__
#pragma ghs ZO
#endif
void sysTimeWaitMilliseconds(systime_t msec)
{
  systime_t current;
  
  irqIsrDisable();
  current = systime;
  while (systime < current + msec) {
    irqIsrEnable();
    irqIsrDisable();
  }
  irqIsrEnable();
}
#ifdef __ghs__
#pragma ghs O
#endif

/**
 * @brief   Returns the system time in milliseconds.
 *
 * @return              the system time
 *
 * @notapi
 */
systime_t sysTimeGetTimeMilliseconds(void)
{
  return (systime_t)systime;
}
#endif
/** @} */
