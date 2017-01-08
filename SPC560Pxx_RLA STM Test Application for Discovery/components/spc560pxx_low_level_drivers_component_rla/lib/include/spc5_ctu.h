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
 * @file    spc5_ctu.h
 * @brief   SPC5xx CTU header file.
 *
 * @addtogroup CTU
 * @{
 */

#ifndef _SPC5_CTU_H_
#define _SPC5_CTU_H_

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
 * @brief   SPC5 CTU registers block.
 * @note    Redefined from the SPC5 headers because the non uniform
 *          declaration of the SubModules registers among the various
 *          sub-families.
 */
struct spc5_ctu {
  union {
    vuint32_t R;
    struct {
      vuint32_t I15_FE:1;
      vuint32_t I15_RE:1;
      vuint32_t I14_FE:1;
      vuint32_t I14_RE:1;
      vuint32_t I13_FE:1;
      vuint32_t I13_RE:1;
      vuint32_t I12_FE:1;
      vuint32_t I12_RE:1;
      vuint32_t I11_FE:1;
      vuint32_t I11_RE:1;
      vuint32_t I10_FE:1;
      vuint32_t I10_RE:1;
      vuint32_t I9_FE:1;
      vuint32_t I9_RE:1;
      vuint32_t I8_FE:1;
      vuint32_t I8_RE:1;
      vuint32_t I7_FE:1;
      vuint32_t I7_RE:1;
      vuint32_t I6_FE:1;
      vuint32_t I6_RE:1;
      vuint32_t I5_FE:1;
      vuint32_t I5_RE:1;
      vuint32_t I4_FE:1;
      vuint32_t I4_RE:1;
      vuint32_t I3_FE:1;
      vuint32_t I3_RE:1;
      vuint32_t I2_FE:1;
      vuint32_t I2_RE:1;
      vuint32_t I1_FE:1;
      vuint32_t I1_RE:1;
      vuint32_t I0_FE:1;
      vuint32_t I0_RE:1;
    } B;
  } TGSISR; /* Trigger Generator Subunit Input Selection Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t:7;
      vuint16_t ETTM:1;
      vuint16_t PRES:2;
      vuint16_t MRSSM:5;
      vuint16_t TGSM:1;
    } B;
  } TGSCR; /* Trigger Generator Subunit Control Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t TCRV:16;
    } B;
  } TCR[8]; /* Trigger 0->7 Compare Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t TGSCCV:16;
    } B;
  } TGSCCR; /* TGS Counter Compare Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t TGSCRV:16;
    } B;
  } TGSCRR; /* TGS Counter Reload Register */

  uint16_t CTU_reserved0;

  union {
    vuint32_t R;
    struct {
      vuint32_t:3;
      vuint32_t T3INDEX:5;
      vuint32_t:3;
      vuint32_t T2INDEX:5;
      vuint32_t:3;
      vuint32_t T1INDEX:5;
      vuint32_t:3;
      vuint32_t T0INDEX:5;
    } B;
  } CLCR1; /* Command List Control Register 1 */

  union {
    vuint32_t R;
    struct {
      vuint32_t:3;
      vuint32_t T7INDEX:5;
      vuint32_t:3;
      vuint32_t T6INDEX:5;
      vuint32_t:3;
      vuint32_t T5INDEX:5;
      vuint32_t:3;
      vuint32_t T4INDEX:5;
    } B;
  } CLCR2; /* Command List Control Register 2 */

  union {
    vuint32_t R;
    struct {
      vuint32_t:3;
      vuint32_t T3E:1;
      vuint32_t T3ETE:1;
      vuint32_t T3T1E:1;
      vuint32_t T3T0E:1;
      vuint32_t T3ADCE:1;
      vuint32_t:3;
      vuint32_t T2E:1;
      vuint32_t T2ETE:1;
      vuint32_t T2T1E:1;
      vuint32_t T2T0E:1;
      vuint32_t T2ADCE:1;
      vuint32_t:3;
      vuint32_t T1E:1;
      vuint32_t T1ETE:1;
      vuint32_t T1T1E:1;
      vuint32_t T1T0E:1;
      vuint32_t T1ADCE:1;
      vuint32_t:3;
      vuint32_t T0E:1;
      vuint32_t T0ETE:1;
      vuint32_t T0T1E:1;
      vuint32_t T0T0E:1;
      vuint32_t T0ADCE:1;
    } B;
  } THCR1; /* Trigger Handler Control Register 1 */

  union {
    vuint32_t R;
    struct {
      vuint32_t:3;
      vuint32_t T7E:1;
      vuint32_t T7ETE:1;
      vuint32_t T7T1E:1;
      vuint32_t T7T0E:1;
      vuint32_t T7ADCE:1;
      vuint32_t:3;
      vuint32_t T6E:1;
      vuint32_t T6ETE:1;
      vuint32_t T6T1E:1;
      vuint32_t T6T0E:1;
      vuint32_t T6ADCE:1;
      vuint32_t:3;
      vuint32_t T5E:1;
      vuint32_t T5ETE:1;
      vuint32_t T5T1E:1;
      vuint32_t T5T0E:1;
      vuint32_t T5ADCE:1;
      vuint32_t:3;
      vuint32_t T4E:1;
      vuint32_t T4ETE:1;
      vuint32_t T4T1E:1;
      vuint32_t T4T0E:1;
      vuint32_t T4ADCE:1;
    } B;
  } THCR2; /* Trigger Handler Control Register 2 */

  union {
    vuint16_t R;
    struct {
      vuint16_t CIR:1;
      vuint16_t FC:1;
      vuint16_t CMS:1;
      vuint16_t:1;
      vuint16_t FIFO:2;
      vuint16_t:4;
      vuint16_t SU:1;
      vuint16_t:1;
      vuint16_t CH:4;
    } B;
  } CLR[24]; /* Commands List Register x (double-buffered) (x = 1,...,24) */

  uint16_t CTU_reserved1[8];
		
  union {
    vuint16_t R;
    struct {
      vuint16_t:12;
      vuint16_t DMAEN3:1;
      vuint16_t DMAEN2:1;
      vuint16_t DMAEN1:1;
      vuint16_t DMAEN0:1;
    } B;
  } FDCR; /* FIFO DMA Control Register */

  uint16_t CTU_reserved2;
		   
  union {
    vuint32_t R;
    struct {
      vuint32_t FIFO_OVERRUN_EN7:1;
      vuint32_t FIFO_OVERFLOW_EN7:1;
      vuint32_t FIFO_EMPTY_EN7:1;
      vuint32_t FIFO_FULL_EN7:1;
      vuint32_t FIFO_OVERRUN_EN6:1;
      vuint32_t FIFO_OVERFLOW_EN6:1;
      vuint32_t FIFO_EMPTY_EN6:1;
      vuint32_t FIFO_FULL_EN6:1;
      vuint32_t FIFO_OVERRUN_EN5:1;
      vuint32_t FIFO_OVERFLOW_EN5:1;
      vuint32_t FIFO_EMPTY_EN5:1;
      vuint32_t FIFO_FULL_EN5:1;
      vuint32_t FIFO_OVERRUN_EN4:1;
      vuint32_t FIFO_OVERFLOW_EN4:1;
      vuint32_t FIFO_EMPTY_EN4:1;
      vuint32_t FIFO_FULL_EN4:1;
      vuint32_t FIFO_OVERRUN_EN3:1;
      vuint32_t FIFO_OVERFLOW_EN3:1;
      vuint32_t FIFO_EMPTY_EN3:1;
      vuint32_t FIFO_FULL_EN3:1;
      vuint32_t FIFO_OVERRUN_EN2:1;
      vuint32_t FIFO_OVERFLOW_EN2:1;
      vuint32_t FIFO_EMPTY_EN2:1;
      vuint32_t FIFO_FULL_EN2:1;
      vuint32_t FIFO_OVERRUN_EN1:1;
      vuint32_t FIFO_OVERFLOW_EN1:1;
      vuint32_t FIFO_EMPTY_EN1:1;
      vuint32_t FIFO_FULL_EN1:1;
      vuint32_t FIFO_OVERRUN_EN0:1;
      vuint32_t FIFO_OVERFLOW_EN0:1;
      vuint32_t FIFO_EMPTY_EN0:1;
      vuint32_t FIFO_FULL_EN0:1;
    } B;
  } FCR; /* FIFO Control Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t THRESHOLD3:8;
      vuint32_t THRESHOLD2:8;
      vuint32_t THRESHOLD1:8;
      vuint32_t THRESHOLD0:8;
    } B;
  } FTH1; /* FIFO Threshold Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t THRESHOLD7:8;
      vuint32_t THRESHOLD6:8;
      vuint32_t THRESHOLD5:8;
      vuint32_t THRESHOLD4:8;
    } B;
  } FTH2; /* FIFO Threshold Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t FIFO_OVERRUN7:1;
      vuint32_t FIFO_OVERFLOW7:1;
      vuint32_t FIFO_EMPTY7:1;
      vuint32_t FIFO_FULL7:1;
      vuint32_t FIFO_OVERRUN6:1;
      vuint32_t FIFO_OVERFLOW6:1;
      vuint32_t FIFO_EMPTY6:1;
      vuint32_t FIFO_FULL6:1;
      vuint32_t FIFO_OVERRUN5:1;
      vuint32_t FIFO_OVERFLOW5:1;
      vuint32_t FIFO_EMPTY5:1;
      vuint32_t FIFO_FULL5:1;
      vuint32_t FIFO_OVERRUN4:1;
      vuint32_t FIFO_OVERFLOW4:1;
      vuint32_t FIFO_EMPTY4:1;
      vuint32_t FIFO_FULL4:1;
      vuint32_t FIFO_OVERRUN3:1;
      vuint32_t FIFO_OVERFLOW3:1;
      vuint32_t FIFO_EMPTY3:1;
      vuint32_t FIFO_FULL3:1;
      vuint32_t FIFO_OVERRUN2:1;
      vuint32_t FIFO_OVERFLOW2:1;
      vuint32_t FIFO_EMPTY2:1;
      vuint32_t FIFO_FULL2:1;
      vuint32_t FIFO_OVERRUN1:1;
      vuint32_t FIFO_OVERFLOW1:1;
      vuint32_t FIFO_EMPTY1:1;
      vuint32_t FIFO_FULL1:1;
      vuint32_t FIFO_OVERRUN0:1;
      vuint32_t FIFO_OVERFLOW0:1;
      vuint32_t FIFO_EMPTY0:1;
      vuint32_t FIFO_FULL0:1;
    } B;
  } FST; /* FIFO Status Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t:11;
      vuint32_t NCH:5;
      vuint32_t:6;
      vuint32_t DATA:10;
    } B;
  } FRA[8]; /* FIFO Right Aligned Data */

  union {
    vuint32_t R;
    struct {
      vuint32_t:11;
      vuint32_t NCH:5;
	  vuint32_t:1;
      vuint32_t DATA:10;
      vuint32_t:5;
    } B;
  } FLA[8]; /* FIFO Left Aligned Data */

  union {
    vuint16_t R;
    struct {
      vuint16_t:7;
      vuint16_t ETOE:1;
      vuint16_t T1OE:1;
      vuint16_t T0OE:1;
      vuint16_t ADCOE:1;
      vuint16_t TGSOSM:1;
      vuint16_t MRSO:1;
      vuint16_t ICE:1;
      vuint16_t SMTO:1;
      vuint16_t MRSRE:1;
    } B;
  } CTUEFR; /* Cross Triggering Unit Error Flag Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t:6;
      vuint16_t ADC:1;
      vuint16_t T7:1;
      vuint16_t T6:1;
      vuint16_t T5:1;
      vuint16_t T4:1;
      vuint16_t T3:1;
      vuint16_t T2:1;
      vuint16_t T1:1;
      vuint16_t T0:1;
      vuint16_t MRS:1;
    } B;
  } CTUIFR; /* Cross Triggering Unit Interrupt Flag Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t T7IE:1;
      vuint16_t T6IE:1;
      vuint16_t T5IE:1;
      vuint16_t T4IE:1;
      vuint16_t T3IE:1;
      vuint16_t T2IE:1;
      vuint16_t T1IE:1;
      vuint16_t T0IE:1;
      vuint16_t:5;
      vuint16_t MRSDMAE:1;
      vuint16_t MRSIE:1;
      vuint16_t IEE:1;
    } B;
  } CTUIR; /* Cross Triggering Unit Interrupt Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t:8;
      vuint16_t COTR:8;
    } B;
  } COTR; /* Control On-Time Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t T7SG:1;
      vuint16_t T6SG:1;
      vuint16_t T5SG:1;
      vuint16_t T4SG:1;
      vuint16_t T3SG:1;
      vuint16_t T2SG:1;
      vuint16_t T1SG:1;
      vuint16_t T0SG:1;
      vuint16_t CTUADCRESET:1;
      vuint16_t CTUODIS:1;
      vuint16_t FILTERENABLE:1;
      vuint16_t CGRE:1;
      vuint16_t FGRE:1;
      vuint16_t MRSSG:1;
      vuint16_t GRE:1;
      vuint16_t TGSISRRE:1;
    } B;
  } CTUCR; /* Cross Triggering Unit Control Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t:8;
      vuint16_t FILTERVALUE:8;
    } B;
  } CTUFILTER; /* Cross Triggering Unit Digital Filter */

  union {
    vuint16_t R;
    struct {
      vuint16_t:15;
      vuint16_t MDIS:1;
    } B;
  } CTUPCR; /* Cross Triggering Unit Power Control Register */
};
/*lint +e46 */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    CTU units references
 * @{
 */
#if (SPC5_HAS_CTU0 == TRUE) || defined(__DOXYGEN__)
#define SPC5_CTU_0          (*(volatile struct spc5_ctu *)0xFFE0C000UL)
#endif
/** @} */

#endif /* _SPC5_CTU_H_ */

/** @} */
