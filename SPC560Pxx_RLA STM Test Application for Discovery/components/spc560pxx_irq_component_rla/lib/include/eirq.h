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
    Portion of the code in this module have been taken from the ChibiOS
    project (ChibiOS - Copyright (C) 2006..2015) licensed under Apache
    2.0 license.
*/

/**
 * @file    eirq.h
 * @brief   SPC5xx EIRQ header.
 *
 * @addtogroup EIRQ
 * @{
 */

#ifndef _EIRQ_H_
#define _EIRQ_H_

#include <stddef.h>
#include "vectors.h"
#include "intc.h"
#include "irq.h"
#include "platform.h"


/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/


//valori di default per macro non definite 


/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   EIRQ notification callback type.
 *
 */
typedef void (*eirqcallback_t)(void);

/**
 * @brief   Structure representing a PIT Channel configuration
 */
typedef struct {
  /**
   * @brief   eirq number.
   */  
  int8_t eirqNumber;
  /**
   * @brief   rising edge trigger event
   */
  uint8_t risingEdge;
  /**
   * @brief    falling edge trigger event
   */
  uint8_t fallingEdge;
  /**
   * @brief    Antiglitch filter enable on PIN
   */
  uint8_t filterEnable;
   /**
   * @brief   callback function.
   */
  eirqcallback_t callback;
}eirq_config;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/
/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/
#if (SPC5_SIUL_EIRQ_HAS_0_7_HANDLER == TRUE)
IRQ_HANDLER(SPC5_SIUL_EIRQ_0_7_HANDLER);
#endif
#if (SPC5_SIUL_EIRQ_HAS_8_15_HANDLER == TRUE)
IRQ_HANDLER(SPC5_SIUL_EIRQ_8_15_HANDLER);
#endif
#if (SPC5_SIUL_EIRQ_HAS_16_23_HANDLER == TRUE)
IRQ_HANDLER(SPC5_SIUL_EIRQ_16_23_HANDLER);
#endif
#if (SPC5_SIUL_EIRQ_HAS_24_31_HANDLER == TRUE)
IRQ_HANDLER(SPC5_SIUL_EIRQ_24_31_HANDLER);
#endif
/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
void eirqInit(void);
#ifdef __cplusplus
}
#endif

#endif
/** @} */
