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
 * @file    irq_cfg.h
 * @brief   IRQ configuration macros and structures.
 *
 * @addtogroup IRQ
 * @{
 */

#ifndef _IRQ_CFG_H_
#define _IRQ_CFG_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Common constants
 * @{
 */
#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE                       0U
#endif

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE                        1U
#endif

/**
 * @name    ISR vectors identifiers
 * @{
 */
/** @} */

/**
 * @name    ISR vectors names overrides
 * @{
 */
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

#if !defined(IRQ_ENABLE_IRQ_PREEMPTION)
#define IRQ_ENABLE_IRQ_PREEMPTION          FALSE
#endif

#if !defined(IRQ_ENABLE_RLA_IVORS)
#define IRQ_ENABLE_RLA_IVORS          TRUE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void irq_cfg_init(void);
#ifdef __cplusplus
}
#endif
#endif /* !defined(_FROM_ASM_) */

#endif /* _IRQ_CFG_H_ */

/** @} */
