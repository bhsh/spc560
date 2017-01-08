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
 * @file    ctu_lld.h
 * @brief   SPC5xx CTU low level driver header.
 *
 * @addtogroup CTU
 * @{
 */

#ifndef _CTU_LLD_H_
#define _CTU_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_CTU == TRUE) || defined(__DOXYGEN__)

#include "spc5_ctu.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Number of Trigger Compare Registers.
 */
#define NUM_OF_TCR                          8U
/** @} */

/**
 * @brief   Minimum value of the TGS Counter Reload Register.
 */
#define SPC5_CTU_MIN_TGSCRR_VALUE           0x8000U
/** @} */

/**
 * @brief   Maximum value of the TGS Counter Compare Register.
 */
#define SPC5_CTU_MAX_TGSCCR_VALUE           0x7FFFU
/** @} */

/**
 * @name    CTUEFR register bits definitions
 * @{
 */
#define SPC5_CTUEFR_MRS_RE                  (1UL << 0)
#define SPC5_CTUEFR_SM_TO                   (1UL << 1)
#define SPC5_CTUEFR_ICE                     (1UL << 2)
#define SPC5_CTUEFR_MRS_O                   (1UL << 3)
#define SPC5_CTUEFR_TGS_OSM                 (1UL << 4)
#define SPC5_CTUEFR_ADC_OE                  (1UL << 5)
#define SPC5_CTUEFR_T0_OE                   (1UL << 6)
#define SPC5_CTUEFR_T1_OE                   (1UL << 7)
#define SPC5_CTUEFR_ET_OE                   (1UL << 8)
/** @} */

/**
 * @name    CTUIFR register bits definitions
 * @{
 */
#define SPC5_CTUIFR_MRS_I                   (1UL << 0)
#define SPC5_CTUIFR_T0_I                    (1UL << 1)
#define SPC5_CTUIFR_T1_I                    (1UL << 2)
#define SPC5_CTUIFR_T2_I                    (1UL << 3)
#define SPC5_CTUIFR_T3_I                    (1UL << 4)
#define SPC5_CTUIFR_T4_I                    (1UL << 5)
#define SPC5_CTUIFR_T5_I                    (1UL << 6)
#define SPC5_CTUIFR_T6_I                    (1UL << 7)
#define SPC5_CTUIFR_T7_I                    (1UL << 8)
#define SPC5_CTUIFR_ADC_I                   (1UL << 9)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   CTU-0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_CTU_0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_CTU0_START_PCTL                (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   CTU-0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_CTU_0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_CTU0_STOP_PCTL                 (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/*===========================================================================*/
/* Configuration checks.                                                     */
/*===========================================================================*/

#if (SPC5_HAS_CTU0 == FALSE) && (SPC5_CTU_USE_CTU0 == TRUE)
#error "CTU0 not present in the selected device"
#endif

#if (SPC5_CTU_USE_CTU0 == FALSE)
#error "CTU driver activated but no CTU peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing a CTU driver.
 */
typedef struct CTUDriver CTUDriver;

/**
 * @brief   CTU interrupt notification callback type.
 *
 * @param[in] ctup      pointer to a @p CTUDriver object
 */
typedef void (*ctucallback_t)(CTUDriver *ctup);

/**
 * @brief   CTU driver configuration structure.
 */
typedef struct {
  /**
   * @brief TGS Counter Prescaler value.
   */
  uint8_t                   prescaler;
  /**
   * @brief Toggle mode for external triggers.
   */
  uint8_t                   toggle_mode;
  /**
   * @brief Triggers Generator Sub-unit Selection Register configurations.
   */
  uint8_t                   tgsisr[32];
  /**
   * @brief Triggers Compare Registers configurations.
   */
  uint16_t                  tcr[NUM_OF_TCR];
  /**
   * @brief Triggers Handler Control Register 1.
   */
  uint8_t                   thcr1[32];
  /**
   * @brief Triggers Handler Control Register 2.
   */
  uint8_t                   thcr2[32];
  /**
   * @brief Triggers Handler Control Register 2.
   */
  uint8_t                   ctuir[16];  
  /**
   * @brief   Callback for Error interrupt.
   */
  ctucallback_t ie_cb;
  /**
   * @brief   Callback for MRS interrupt.
   */
  ctucallback_t mrs_i_cb;  
  /**
   * @brief   Callbacks for Trigger[0:7] interrupts.
   */
  ctucallback_t t_i_cb[8];
} CTUConfig;

/**
 * @brief   Structure representing a CTU driver.
 */
struct CTUDriver {
  /**
   * @brief Current driver configuration data.
   */
  const CTUConfig           *config;
  /**
   * @brief Pointer to the volatile CTU registers block.
   */
  volatile struct spc5_ctu  *ctup_tagp;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

IRQ_HANDLER(SPC5_CT0_ERR_I_HANDLER);
IRQ_HANDLER(SPC5_CT0_MRS_I_HANDLER);
IRQ_HANDLER(SPC5_CT0_T0_I_HANDLER);
IRQ_HANDLER(SPC5_CT0_T1_I_HANDLER);
IRQ_HANDLER(SPC5_CT0_T2_I_HANDLER);
IRQ_HANDLER(SPC5_CT0_T3_I_HANDLER);
IRQ_HANDLER(SPC5_CT0_T4_I_HANDLER);
IRQ_HANDLER(SPC5_CT0_T5_I_HANDLER);
IRQ_HANDLER(SPC5_CT0_T6_I_HANDLER);
IRQ_HANDLER(SPC5_CT0_T7_I_HANDLER);

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (SPC5_CTU_USE_CTU0 == TRUE) && !defined(__DOXYGEN__)
extern CTUDriver CTUD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void ctu_lld_init(void);
  void ctu_lld_start(CTUDriver *ctup, const CTUConfig *config);
  void ctu_lld_stop(CTUDriver *ctup);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_CTU */

#endif /* _CTU_LLD_H_ */

/** @} */
