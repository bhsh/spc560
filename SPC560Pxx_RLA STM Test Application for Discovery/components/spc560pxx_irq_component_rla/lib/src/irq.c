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
 * @file    irq.c
 * @brief   IRQ module code.
 *
 * @addtogroup IRQ
 * @{
 */

#include "irq.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   IRQ module initialization.
 *
 * @api
 */
void irqInit(void) {
  uint8_t i;

  irq_cfg_init();
  
#if CORE_SUPPORTS_IVORS
  /* The core supports IVOR registers, the kernel requires IVOR4 and IVOR10
     and the initialization is performed here.*/
#ifdef __ghs__
  asm ("e_li        %r3, %lo(_IVOR4)    \t\n"
       "mtIVOR4     %r3                 \t\n"
       "e_li        %r3, %lo(_IVOR10)   \t\n"
       "mtIVOR10    %r3");
#else
  asm volatile ("e_li        %%r3, _IVOR4@l       \t\n"
                "mtIVOR4     %%r3                 \t\n"
                "e_li        %%r3, _IVOR10@l      \t\n"
                "mtIVOR10    %%r3" : : : "memory");
#endif
#endif

  /* Method of accessing memory-mapped registers at fixed addresses does not 
     comply with MISRA C because it involves converting an integer to a 
	 pointer. */
  /*lint -e9033 */
  /* INTC initialization, software vector mode, 4 bytes vectors, starting
     at priority 0.*/
     
  INTC_BCR = 0;

  for (i = 0; i < CORE_NUMBER; i++) {
#if defined(_SPC574Kxx_)
	if(i == 1U)
	{
		continue;
	}
#endif
    INTC_CPR(i)   = 0;
    INTC_IACKR(i) = (uint32_t)_vectors;
  }

  /*lint +e9033 */
}

/**
 * @brief   System halt 
 *
 * @api
 */

#if !defined(__DOXYGEN__)
__attribute__((weak, noreturn))
#endif
void irqSysHalt(void) {

  irqIsrDisable();
  for ( ; ; ) {
  }
}

/** @} */
