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
 * @file    ivor.s
 * @brief   Kernel ISRs.
 *
 * @addtogroup PPC_CORE
 * @{
 */

/*
 * Imports the configuration headers.
 */
#define _FROM_ASM_
#include "core.h"
#include "irq_cfg.h"
#include "checkCompiler.h"

#if !defined(__DOXYGEN__)
        /*
         * INTC registers address.
         */
#if defined(_SPC570Sxx_)
        .equ  INTC_IACKR, 0xFC040020
        .equ  INTC_EOIR,  0xFC040030
#elif defined(_SPC574Kxx_)
        .equ  INTC_IACKR, 0xFC040028
        .equ  INTC_EOIR,  0xFC040038
#else
        .equ  INTC_IACKR, 0xfff48010
        .equ  INTC_EOIR,  0xfff48018
#endif /* _SPC570Sxx_ || _SPC574Kxx_ */

        .section    .handlers, "axv"
_VLE

#if (IRQ_ENABLE_RLA_IVORS == TRUE)

#if (CORE_SUPPORTS_DECREMENTER == 1)
        /*
         * _IVOR10 handler (Book-E decrementer).
         */
        .align      4
        .globl      _IVOR10
        .type       _IVOR10, @function
_IVOR10:
        /* Creation of the external stack frame (extctx structure).*/
        e_stwu      sp, -80(sp)             /* Size of the extctx structure.*/

        e_stmvsrrw  8(sp)                   /* Saves PC, MSR.               */
        e_stmvsprw  16(sp)                  /* Saves CR, LR, CTR, XER.      */
        e_stmvgprw  32(sp)                  /* Saves GPR0, GPR3...GPR12.    */

        /* Reset DIE bit in TSR register.*/
        e_lis       %r3, 0x0800             /* DIS bit mask.                */
        mtspr       336, %r3                /* TSR register.                */

#ifndef _SPC570Sxx_
        /* Restoring pre-IRQ MSR register value.*/
        mfSRR1      %r0

        /* No preemption, keeping EE disabled.*/
        se_bclri    %r0, 16                 /* EE = bit 16.                 */
        mtMSR       %r0
#endif

        e_b           _ivor_exit
#endif /* CORE_SUPPORTS_DECREMENTER */

        /*
         * _IVOR4 handler (Book-E external interrupt).
         */
        .align      4
        .globl      _IVOR4
        .type       _IVOR4, @function
_IVOR4:
        /* Creation of the external stack frame (extctx structure).*/
        e_stwu      sp, -80(sp)             /* Size of the extctx structure.*/

        e_stmvsrrw  8(sp)                   /* Saves PC, MSR.               */
        e_stmvsprw  16(sp)                  /* Saves CR, LR, CTR, XER.      */
        e_stmvgprw  32(sp)                  /* Saves GPR0, GPR3...GPR12.    */

        /* Software vector address from the INTC register.*/
        e_lis       %r3, HI(INTC_IACKR)
        e_or2i      %r3, LO(INTC_IACKR)     /* IACKR register address.      */
        se_lwz      %r3, 0(%r3)             /* IACKR register value.        */
        se_lwz      %r3, 0(%r3)
        se_mtCTR    %r3                     /* Software handler address.    */

        /* Restoring pre-IRQ MSR register value.*/
        mfSRR1      %r0
        /* No preemption, keeping EE disabled.*/
        se_bclri    %r0, 16                 /* EE = bit 16.                 */
        mtMSR       %r0

#if (IRQ_ENABLE_IRQ_PREEMPTION == TRUE)
        /* Allows preemption while executing the software handler.*/
        wrteei      1
#endif /* IRQ_ENABLE_IRQ_PREEMPTION */

        /* Executes the software handler.*/
        se_bctrl

#if (IRQ_ENABLE_IRQ_PREEMPTION == TRUE)
        /* Prevents preemption again.*/
        wrteei      0
#endif /* IRQ_ENABLE_IRQ_PREEMPTION */

        /* Informs the INTC that the interrupt has been served.*/
        mbar        0
        e_lis       %r3, HI(INTC_EOIR)
        e_or2i      %r3, LO(INTC_EOIR)
        se_stw      %r3, 0(%r3)             /* Writing any value should do. */

        /* Context restore.*/
        .globl      _ivor_exit
_ivor_exit:

        e_lmvgprw   32(sp)                  /* Restores GPR0, GPR3...GPR12. */
        e_lmvsprw   16(sp)                  /* Restores CR, LR, CTR, XER.   */
        e_lmvsrrw   8(sp)                   /* Restores PC, MSR.            */
        e_addi      sp, sp, 80              /* Back to the previous frame.  */
        se_rfi

#endif /* IRQ_ENABLE_RLA_IVORS */

#endif /* !defined(__DOXYGEN__) */

/** @} */
