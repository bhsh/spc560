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
 * @file    spc5_stm.h
 * @brief   SPC5xx STM header file.
 *
 * @addtogroup STM
 * @{
 */

#ifndef _SPC5_STM_H_
#define _SPC5_STM_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/
/* PPC register definitions not compatible with MISRA rules on bit-fields 
   (Rules 6.1). */
/*lint -e46 */
/**
 * @brief   SPC5 STM registers block.
 * @note    declaration of the SubModules registers among the various
 *          
 */
  struct STM_CH_tag{

    union { /* STM Channel Control 0..3 */
        vuint32_t R;
        struct {
            vuint32_t :31;
            vuint32_t CEN:1;
        } B;
    } CCR;

    union { /* STM Channel Interrupt 0..3 */
        vuint32_t R;
        struct {
            vuint32_t :31;
            vuint32_t CIF:1;
        } B;
    } CIR;

    union { /* STM Channel Compare 0..3 */
        vuint32_t R;
        struct {
            vuint32_t CMP:32;
        } B;
    } CMP;

    vuint8_t STM_CHANNEL_reserved0[4]; /* Reserved 4 bytes between ch reg's */

  }; /* end of STM_CHANNEL_tag */


struct spc5_stm{

    union { /* STM Control (Base+0x0000) */
        vuint32_t R;
        struct {
            vuint32_t :16;
            vuint32_t CPS:8;
            vuint32_t :6;
            vuint32_t FRZ:1;
            vuint32_t TEN:1;
        } B;
    } CR;

    union { /* STM Count (Base+0x0004) */
        vuint32_t R;
    } CNT;

    vuint8_t STM_reserved1[8]; /* Reserved 8 bytes (Base+0x0008-0x000F) */

    struct STM_CH_tag CH[4]; /*STM Channels 0..3 (Base+0x0010-0x0048) */

};
/*lint +e46 */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    STM units references
 * @{
 */
#if (SPC5_HAS_STM0 == TRUE)
#if defined (_SPC570Sxx_) || defined(_SPC574Kxx_)
#define SPC5_STM0         (*(volatile struct spc5_stm *) 0xFC068000UL)
#else
#define SPC5_STM0         (*(volatile struct spc5_stm *) 0xFFF3C000UL)
#endif
#endif

#if (SPC5_HAS_STM2 == TRUE)
#define SPC5_STM2         (*(volatile struct spc5_stm *) 0xFC070000UL)
#endif	

/** @} */

#endif /* _SPC5_STM_H_ */

/** @} */
