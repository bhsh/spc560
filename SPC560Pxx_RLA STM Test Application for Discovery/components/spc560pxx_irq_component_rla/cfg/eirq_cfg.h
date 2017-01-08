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
 * @file    eirq_cfg.h
 * @brief   External Interrupt configuration and management.
 *
 * @addtogroup EIRQ
 * @{
 */ 
 
#ifndef _EIRQ_CFG_H_
#define _EIRQ_CFG_H_

#include "eirq.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
extern const eirq_config eirqconf[];
/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/
#define SPC5_EIRQ_0_7_IRQ_PRIORITY            INTC_PSR_ENABLE(INTC_PSR_CORE0,8)
#define SPC5_EIRQ_8_15_IRQ_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_CORE0,8)
#define SPC5_EIRQ_16_23_IRQ_PRIORITY          INTC_PSR_ENABLE(INTC_PSR_CORE0,8)
#define SPC5_EIRQ_24_31_IRQ_PRIORITY          INTC_PSR_ENABLE(INTC_PSR_CORE0,8)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#endif /* _EIRQ_CFG_H_ */

/** @} */
