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
 * @file    intc.h
 * @brief   INTC module header.
 *
 * @addtogroup INTC
 * @{
 */

#ifndef _INTC_H_
#define _INTC_H_

#include <stdint.h>

#include "core.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   INTC base address.
 */
#if defined(_SPC57EMxx_) || defined(_SPC570Sxx_) || defined(_SPC574Kxx_) || defined(__DOXYGEN__)

#if defined(_SPC57EMxx_HSM_)
/* HSM INTC.*/
#define INTC_BASE                   0xA3F48000U
#else
/* Main INTC.*/
#define INTC_BASE                   0xFC040000U
#endif

#elif defined(_SPC58NExx_) || defined(_SPC58ECxx_)

#if defined(_SPC58NExx_HSM_) || defined(_SPC58ECxx_HSM_)
/* HSM INTC.*/
#define INTC_BASE                   0xA3F48000U
#else
/* Core2 INTC_1.*/
#define INTC_BASE                   0xF4044000U
#endif

#else
/* All SPC56 devices.*/
#define INTC_BASE                   0xFFF48000U
#endif

/*
 * @name    INTC PSR register definitions
 * @{
 */
/* TODO    Some platforms support more than 16 priority levels.*/
#define INTC_PSR_PRIO_MASK               0x000F
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    INTC-related macros
 * @{
 */
#if defined(_SPC57EMxx_) || defined(_SPC574Kxx_) || defined(_SPC58NExx_) || defined(_SPC58ECxx_) || \
    defined(__DOXYGEN__)
#define INTC_BCR          (*((volatile uint32_t *)(INTC_BASE + 0U)))
#define INTC_MPROT        (*((volatile uint32_t *)(INTC_BASE + 4U)))
#define INTC_CPR(n)       (*((volatile uint32_t *)(INTC_BASE + 0x10U + ((uint8_t)(n) * sizeof (uint32_t)))))
#define INTC_IACKR(n)     (*((volatile uint32_t *)(INTC_BASE + 0x20U + ((uint8_t)(n) * sizeof (uint32_t)))))
#define INTC_EOIR(n)      (*((volatile uint32_t *)(INTC_BASE + 0x30U + ((uint8_t)(n) * sizeof (uint32_t)))))
#define INTC_PSR(n)       (*((volatile uint16_t *)(INTC_BASE + 0x60U + ((uint16_t)(n) * sizeof (uint16_t)))))

#elif defined(_SPC570Sxx_)
#define INTC_BCR          (*((volatile uint32_t *)(INTC_BASE + 0U)))
#define INTC_CPR(n)       (*((volatile uint32_t *)(INTC_BASE + 0x10U + ((uint8_t)(n) * sizeof (uint32_t)))))
#define INTC_IACKR(n)     (*((volatile uint32_t *)(INTC_BASE + 0x20U + ((uint8_t)(n) * sizeof (uint32_t)))))
#define INTC_EOIR(n)      (*((volatile uint32_t *)(INTC_BASE + 0x30U + ((uint8_t)(n) * sizeof (uint32_t)))))
#define INTC_PSR(n)       (*((volatile uint16_t *)(INTC_BASE + 0x60U + ((uint16_t)(n) * sizeof (uint16_t)))))

#else
#define INTC_BCR          (*((volatile uint32_t *)(INTC_BASE + 0U)))
#define INTC_CPR(n)       (*((volatile uint32_t *)(INTC_BASE + 8U + ((uint8_t)(n) * sizeof (uint32_t)))))
#define INTC_IACKR(n)     (*((volatile uint32_t *)(INTC_BASE + 0x10U + ((uint8_t)(n) * sizeof (uint32_t)))))
#define INTC_EOIR(n)      (*((volatile uint32_t *)(INTC_BASE + 0x18U + ((uint8_t)(n) * sizeof (uint32_t)))))
#define INTC_PSR(n)       (*((volatile uint8_t *)(INTC_BASE + 0x40U + ((uint16_t)(n) * sizeof (uint8_t)))))
#endif
/** @} */

/**
 * @brief   Core selection macros for PSR register.
 */
#if defined(_SPC57EMxx_) || defined(_SPC574Kxx_) || defined(__DOXYGEN__)

#if defined(_SPC57EMxx_HSM_)
/* HSM INTC.*/
#define INTC_PSR_CORE4              0x8000
#else
/* Main INTC.*/
#define INTC_PSR_CORE0              0x8000
#define INTC_PSR_CORE1              0x4000
#define INTC_PSR_CORE2              0x2000
#define INTC_PSR_CORES01            0xC000
#define INTC_PSR_CORES02            0xA000
#define INTC_PSR_CORES12            0x6000
#define INTC_PSR_CORES012           0xE000
#endif

#elif defined(_SPC58NExx_) || defined(_SPC58ECxx_)

#if defined(_SPC58NExx_HSM_) || defined(_SPC58ECxx_HSM_)
/* HSM INTC.*/
#define INTC_PSR_CORE4              0x8000
#else
/* Core 2 INTC_1.*/
#define INTC_PSR_CORE0              0x8000
#define INTC_PSR_CORE1              0x4000
#define INTC_PSR_CORE2              0x2000
#endif

#elif defined(_SPC570Sxx_)
#define INTC_PSR_CORE0              0x8000

#elif defined(_SPC56ECxx_)
#define INTC_PSR_CORE0                0x00
#define INTC_PSR_CORE1                0xC0
#define INTC_PSR_CORES01              0x40

#else
/* Any other SPC56 device.*/
#define INTC_PSR_CORE0                0x00
#endif

/**
 * @brief   PSR register content helper
 */
#if defined(_SPC57EMxx_) || defined(_SPC58NExx_) || defined(_SPC570Sxx_) || defined(_SPC574Kxx_) || defined(__DOXYGEN__)
#define INTC_PSR_ENABLE(cores, prio)    ((uint16_t)(cores) | (uint16_t)(prio))
#else
#define INTC_PSR_ENABLE(cores, prio)    ((uint8_t)(cores) | (uint8_t)(prio))
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* _INTC_H_ */

/** @} */
