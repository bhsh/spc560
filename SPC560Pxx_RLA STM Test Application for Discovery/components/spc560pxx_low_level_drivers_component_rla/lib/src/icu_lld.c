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
 * @file    icu_lld.c
 * @brief   SPC5xx ICU low level driver code.
 *
 * @addtogroup ICU
 * @{
 */

#include "icu_lld.h"

#if (LLD_USE_ICU == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   ICUD1 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD0 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD1;
#endif

/**
 * @brief   ICUD2 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD1 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD2;
#endif

/**
 * @brief   ICUD3 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD2 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD3;
#endif

/**
 * @brief   ICUD4 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD3 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD4;
#endif

/**
 * @brief   ICUD5 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD4 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD5;
#endif

/**
 * @brief   ICUD6 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD5 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD6;
#endif

/**
 * @brief   ICUD7 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD6 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD7;
#endif

/**
 * @brief   ICUD8 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD7 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD8;
#endif

/**
 * @brief   ICUD9 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD8 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD9;
#endif

/**
 * @brief   ICUD10 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD9 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD10;
#endif

/**
 * @brief   ICUD11 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD10 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD11;
#endif

/**
 * @brief   ICUD12 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD11 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD12;
#endif

/**
 * @brief   ICUD13 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD12 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD13;
#endif

/**
 * @brief   ICUD14 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD13 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD14;
#endif

/**
 * @brief   ICUD15 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD14 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD15;
#endif

/**
 * @brief   ICUD16 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD15 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD16;
#endif

/**
 * @brief   ICUD17 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD16 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD17;
#endif

/**
 * @brief   ICUD18 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD17 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD18;
#endif

/**
 * @brief   ICUD19 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD18 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD19;
#endif

/**
 * @brief   ICUD20 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD19 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD20;
#endif

/**
 * @brief   ICUD21 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD20 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD21;
#endif

/**
 * @brief   ICUD22 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD21 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD22;
#endif

/**
 * @brief   ICUD23 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD22 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD23;
#endif

/**
 * @brief   ICUD24 driver identifier.
 */
#if (SPC5_ICU_USE_SMOD23 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD24;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Number of active eTimer Submodules.
 */
#if (SPC5_ICU_USE_ETIMER0 == TRUE)
static uint32_t icu_active_submodules0;
#endif
#if (SPC5_ICU_USE_ETIMER1 == TRUE)
static uint32_t icu_active_submodules1;
#endif
#if (SPC5_ICU_USE_ETIMER2 == TRUE)
static uint32_t icu_active_submodules2;
#endif
#if (SPC5_ICU_USE_ETIMER3 == TRUE)
static uint32_t icu_active_submodules3;
#endif

/**
 * @brief   Width and Period registers.
 */
static uint16_t width;
static uint16_t period;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 */
static void icu_lld_serve_interrupt(ICUDriver *icup) {
  uint16_t intdma;
  uint16_t sr;

  intdma = icup->etimerp->CHANNEL[icup->smod_number].INTDMA.R;
  sr = icup->etimerp->CHANNEL[icup->smod_number].STS.R & intdma;

#if (ICU_SKIP_FIRST_CAPTURE == TRUE)
  if ((sr & 0x0008U) != 0U) { /* TOF */
    icup->etimerp->CHANNEL[icup->smod_number].STS.B.TOF = 1U;
    _icu_isr_invoke_overflow_cb(icup);
  }
  if ((sr & 0x0040U) != 0U) { /* ICF1 */
    if (icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.CNTMODE ==
        SPC5_ETIMER_CNTMODE_RFE_SIHA) {
      icup->etimerp->CHANNEL[icup->smod_number].STS.B.ICF1 = 1U;
      icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.CNTMODE =
          SPC5_ETIMER_CNTMODE_RE;
    }
    else {
      icup->etimerp->CHANNEL[icup->smod_number].STS.B.ICF1 = 1U;
      if (icup->etimerp->CHANNEL[icup->smod_number].CTRL3.B.C1FCNT == 2U) {
        period = icup->etimerp->CHANNEL[icup->smod_number].CAPT1.R;
        period = icup->etimerp->CHANNEL[icup->smod_number].CAPT1.R;
      } else {
        period = icup->etimerp->CHANNEL[icup->smod_number].CAPT1.R;
      }
      _icu_isr_invoke_period_cb(icup);
    }
  }
  else if ((sr & 0x0080U) != 0U) { /* ICF2 */
    if (icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.CNTMODE ==
        SPC5_ETIMER_CNTMODE_RFE_SIHA) {
      icup->etimerp->CHANNEL[icup->smod_number].STS.B.ICF2 = 1U;
      icup->etimerp->CHANNEL[icup->smod_number].CNTR.R = 0;
    }
    else {
      icup->etimerp->CHANNEL[icup->smod_number].STS.B.ICF2 = 1U;
      if (icup->etimerp->CHANNEL[icup->smod_number].CTRL3.B.C2FCNT == 2U) {
        width = icup->etimerp->CHANNEL[icup->smod_number].CAPT2.R;
        width = icup->etimerp->CHANNEL[icup->smod_number].CAPT2.R;
      } else {
        width = icup->etimerp->CHANNEL[icup->smod_number].CAPT2.R;
      }
      _icu_isr_invoke_width_cb(icup);
    }
  } else {
    ; /*close if...else if as per MISRA rules*/
  }
#else
  if ((sr & 0x0008U) != 0U) { /* TOF */
    icup->etimerp->CHANNEL[icup->smod_number].STS.B.TOF = 1U;
    _icu_isr_invoke_overflow_cb(icup);
  }
  if ((sr & 0x0040U) != 0U) { /* ICF1 */
    icup->etimerp->CHANNEL[icup->smod_number].STS.B.ICF1 = 1U;
    if (icup->etimerp->CHANNEL[icup->smod_number].CTRL3.B.C1FCNT == 2U) {
      period = icup->etimerp->CHANNEL[icup->smod_number].CAPT1.R;
      period = icup->etimerp->CHANNEL[icup->smod_number].CAPT1.R;
    } else {
      period = icup->etimerp->CHANNEL[icup->smod_number].CAPT1.R;
    }
    _icu_isr_invoke_period_cb(icup);
  }
  else if ((sr & 0x0080U) != 0U) { /* ICF2 */
    icup->etimerp->CHANNEL[icup->smod_number].STS.B.ICF2 = 1U;
    if (icup->etimerp->CHANNEL[icup->smod_number].CTRL3.B.C2FCNT == 2U) {
      width = icup->etimerp->CHANNEL[icup->smod_number].CAPT2.R;
      width = icup->etimerp->CHANNEL[icup->smod_number].CAPT2.R;
    } else {
      width = icup->etimerp->CHANNEL[icup->smod_number].CAPT2.R;
    }
    _icu_isr_invoke_width_cb(icup);
  } else {
    ; /*close if...else if as per MISRA rules*/
  }
#endif /* ICU_SKIP_FIRST_CAPTURE */
}

/**
 * @brief   eTimer SubModules initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p ICUDriver object
 * @param[in] config    the architecture-dependent ICU driver configuration
 */
static void spc5_icu_smod_init(ICUDriver *icup) {
  uint32_t psc = (icup->clock / icup->config->frequency);

  /* Set primary source and clock prescaler.*/
  switch (psc) {
  case 1:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.PRISRC =
        SPC5_ETIMER_IP_BUS_CLK_DIV_1;
    break;
  case 2:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.PRISRC =
        SPC5_ETIMER_IP_BUS_CLK_DIV_2;
    break;
  case 4:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.PRISRC =
        SPC5_ETIMER_IP_BUS_CLK_DIV_4;
    break;
  case 8:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.PRISRC =
        SPC5_ETIMER_IP_BUS_CLK_DIV_8;
    break;
  case 16:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.PRISRC =
        SPC5_ETIMER_IP_BUS_CLK_DIV_16;
    break;
  case 32:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.PRISRC =
        SPC5_ETIMER_IP_BUS_CLK_DIV_32;
    break;
  case 64:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.PRISRC =
        SPC5_ETIMER_IP_BUS_CLK_DIV_64;
    break;
  case 128:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.PRISRC =
        SPC5_ETIMER_IP_BUS_CLK_DIV_128;
    break;
  default:
    /* Nothing to do. */
    break;
  }

  /* Set control registers.*/
  icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.ONCE = 0U;
  icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.LENGTH = 0U;
  icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.DIR = 0U;
  icup->etimerp->CHANNEL[icup->smod_number].CTRL2.B.PIPS = 0U;

  /* Set secondary source.*/
  switch (icup->smod_number) {
  case 0:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.SECSRC =
        SPC5_ETIMER_COUNTER_0_INPUT_PIN;
    break;
  case 1:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.SECSRC =
        SPC5_ETIMER_COUNTER_1_INPUT_PIN;
    break;
  case 2:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.SECSRC =
        SPC5_ETIMER_COUNTER_2_INPUT_PIN;
    break;
  case 3:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.SECSRC =
        SPC5_ETIMER_COUNTER_3_INPUT_PIN;
    break;
  case 4:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.SECSRC =
        SPC5_ETIMER_COUNTER_4_INPUT_PIN;
    break;
  case 5:
    icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.SECSRC =
        SPC5_ETIMER_COUNTER_5_INPUT_PIN;
    break;
  default:
    /* Nothing to do. */
    break;
  }

  /* Set secondary source polarity.*/
  if (icup->config->mode == ICU_INPUT_ACTIVE_HIGH) {
    icup->etimerp->CHANNEL[icup->smod_number].CTRL2.B.SIPS = 0U;
  }
  else {
    icup->etimerp->CHANNEL[icup->smod_number].CTRL2.B.SIPS = 1U;
  }

  /* Direct pointers to the capture registers in order to make reading
     data faster from within callbacks.*/
  icup->pccrp = &period;
  icup->wccrp = &width;

  /* Enable channel.*/
  icup->etimerp->ENBL.B.ENBL |= (uint16_t)(1UL << (icup->smod_number));
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (SPC5_ICU_USE_SMOD0 == TRUE)
#if !defined(SPC5_ETIMER0_TC0IR_HANDLER)
#error "SPC5_ETIMER0_TC0IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_TC0IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD1);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD0 */

#if (SPC5_ICU_USE_SMOD1 == TRUE)
#if !defined(SPC5_ETIMER0_TC1IR_HANDLER)
#error "SPC5_ETIMER0_TC1IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_TC1IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD2);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD1 */

#if (SPC5_ICU_USE_SMOD2 == TRUE)
#if !defined(SPC5_ETIMER0_TC2IR_HANDLER)
#error "SPC5_ETIMER0_TC2IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_TC2IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD3);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD2 */

#if (SPC5_ICU_USE_SMOD3 == TRUE)
#if !defined(SPC5_ETIMER0_TC3IR_HANDLER)
#error "SPC5_ETIMER0_TC3IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_TC3IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD4);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD3 */

#if (SPC5_ICU_USE_SMOD4 == TRUE)
#if !defined(SPC5_ETIMER0_TC4IR_HANDLER)
#error "SPC5_ETIMER0_TC4IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_TC4IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD5);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD4 */

#if (SPC5_ICU_USE_SMOD5 == TRUE)
#if !defined(SPC5_ETIMER0_TC5IR_HANDLER)
#error "SPC5_ETIMER0_TC5IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_TC5IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD6);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD5 */

#if (SPC5_ICU_USE_SMOD6 == TRUE)
#if !defined(SPC5_ETIMER1_TC0IR_HANDLER)
#error "SPC5_ETIMER1_TC0IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_TC0IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD7);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD6 */

#if (SPC5_ICU_USE_SMOD7 == TRUE)
#if !defined(SPC5_ETIMER1_TC1IR_HANDLER)
#error "SPC5_ETIMER1_TC1IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_TC1IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD8);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD7 */

#if (SPC5_ICU_USE_SMOD8 == TRUE)
#if !defined(SPC5_ETIMER1_TC2IR_HANDLER)
#error "SPC5_ETIMER1_TC2IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_TC2IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD9);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD8 */

#if (SPC5_ICU_USE_SMOD9 == TRUE)
#if !defined(SPC5_ETIMER1_TC3IR_HANDLER)
#error "SPC5_ETIMER1_TC3IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_TC3IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD10);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD9 */

#if (SPC5_ICU_USE_SMOD10 == TRUE)
#if !defined(SPC5_ETIMER1_TC4IR_HANDLER)
#error "SPC5_ETIMER1_TC4IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_TC4IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD11);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD10 */

#if (SPC5_ICU_USE_SMOD11 == TRUE)
#if !defined(SPC5_ETIMER1_TC5IR_HANDLER)
#error "SPC5_ETIMER1_TC5IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_TC5IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD12);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD11 */

#if (SPC5_ICU_USE_SMOD12 == TRUE)
#if !defined(SPC5_ETIMER2_TC0IR_HANDLER)
#error "SPC5_ETIMER2_TC0IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_TC0IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD13);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD12 */

#if (SPC5_ICU_USE_SMOD13 == TRUE)
#if !defined(SPC5_ETIMER2_TC1IR_HANDLER)
#error "SPC5_ETIMER2_TC1IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_TC1IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD14);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD13 */

#if (SPC5_ICU_USE_SMOD14 == TRUE)
#if !defined(SPC5_ETIMER2_TC2IR_HANDLER)
#error "SPC5_ETIMER2_TC2IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_TC2IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD15);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD14 */

#if (SPC5_ICU_USE_SMOD15 == TRUE)
#if !defined(SPC5_ETIMER2_TC3IR_HANDLER)
#error "SPC5_ETIMER2_TC3IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_TC3IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD16);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD15 */

#if (SPC5_ICU_USE_SMOD16 == TRUE)
#if !defined(SPC5_ETIMER2_TC4IR_HANDLER)
#error "SPC5_ETIMER2_TC4IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_TC4IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD17);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD16 */

#if (SPC5_ICU_USE_SMOD17 == TRUE)
#if !defined(SPC5_ETIMER2_TC5IR_HANDLER)
#error "SPC5_ETIMER2_TC5IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_TC5IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD18);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD17 */

#if (SPC5_ICU_USE_SMOD18 == TRUE)
#if !defined(SPC5_ETIMER3_TC0IR_HANDLER)
#error "SPC5_ETIMER3_TC0IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_TC0IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD19);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD18 */

#if (SPC5_ICU_USE_SMOD19 == TRUE)
#if !defined(SPC5_ETIMER3_TC1IR_HANDLER)
#error "SPC5_ETIMER3_TC1IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_TC1IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD20);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD19 */

#if (SPC5_ICU_USE_SMOD20 == TRUE)
#if !defined(SPC5_ETIMER3_TC2IR_HANDLER)
#error "SPC5_ETIMER3_TC2IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_TC2IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD21);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD20 */

#if (SPC5_ICU_USE_SMOD21 == TRUE)
#if !defined(SPC5_ETIMER3_TC3IR_HANDLER)
#error "SPC5_ETIMER3_TC3IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_TC3IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD22);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD21 */

#if (SPC5_ICU_USE_SMOD22 == TRUE)
#if !defined(SPC5_ETIMER3_TC4IR_HANDLER)
#error "SPC5_ETIMER3_TC4IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_TC4IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD23);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD22 */

#if (SPC5_ICU_USE_SMOD23 == TRUE)
#if !defined(SPC5_ETIMER3_TC5IR_HANDLER)
#error "SPC5_ETIMER3_TC5IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_TC5IR_HANDLER) {

  IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD24);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD23 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ICU driver initialization.
 *
 * @init
 */
void icu_lld_init(void) {

  /* Submodules initially all not in use.*/
#if(SPC5_ICU_USE_ETIMER0 == TRUE)
  icu_active_submodules0 = 0;
#endif
#if(SPC5_ICU_USE_ETIMER1 == TRUE)
  icu_active_submodules1 = 0;
#endif
#if(SPC5_ICU_USE_ETIMER2 == TRUE)
  icu_active_submodules2 = 0;
#endif
#if(SPC5_ICU_USE_ETIMER3 == TRUE)
  icu_active_submodules3 = 0;
#endif

  /* Reset width and period registers.*/
  width = 0;
  period = 0;

#if (SPC5_ICU_USE_SMOD0 == TRUE)
  /* Driver initialization.*/
  ICUD1.config = NULL;
  ICUD1.etimerp = &SPC5_ETIMER_0;
  ICUD1.smod_number = 0U;
  ICUD1.clock = SPC5_ETIMER0_CLK;
#endif

#if (SPC5_ICU_USE_SMOD1 == TRUE)
  /* Driver initialization.*/
  ICUD2.config = NULL;
  ICUD2.etimerp = &SPC5_ETIMER_0;
  ICUD2.smod_number = 1U;
  ICUD2.clock = SPC5_ETIMER0_CLK;
#endif

#if (SPC5_ICU_USE_SMOD2 == TRUE)
  /* Driver initialization.*/
  ICUD3.config = NULL;
  ICUD3.etimerp = &SPC5_ETIMER_0;
  ICUD3.smod_number = 2U;
  ICUD3.clock = SPC5_ETIMER0_CLK;
#endif

#if (SPC5_ICU_USE_SMOD3 == TRUE)
  /* Driver initialization.*/
  ICUD4.config = NULL;
  ICUD4.etimerp = &SPC5_ETIMER_0;
  ICUD4.smod_number = 3U;
  ICUD4.clock = SPC5_ETIMER0_CLK;
#endif

#if (SPC5_ICU_USE_SMOD4 == TRUE)
  /* Driver initialization.*/
  ICUD5.config = NULL;
  ICUD5.etimerp = &SPC5_ETIMER_0;
  ICUD5.smod_number = 4U;
  ICUD5.clock = SPC5_ETIMER0_CLK;
#endif

#if (SPC5_ICU_USE_SMOD5 == TRUE)
  /* Driver initialization.*/
  ICUD6.config = NULL;
  ICUD6.etimerp = &SPC5_ETIMER_0;
  ICUD6.smod_number = 5U;
  ICUD6.clock = SPC5_ETIMER0_CLK;
#endif

#if (SPC5_ICU_USE_SMOD6 == TRUE)
  /* Driver initialization.*/
  ICUD7.config = NULL;
  ICUD7.etimerp = &SPC5_ETIMER_1;
  ICUD7.smod_number = 0U;
  ICUD7.clock = SPC5_ETIMER1_CLK;
#endif

#if (SPC5_ICU_USE_SMOD7 == TRUE)
  /* Driver initialization.*/
  ICUD8.config = NULL;
  ICUD8.etimerp = &SPC5_ETIMER_1;
  ICUD8.smod_number = 1U;
  ICUD8.clock = SPC5_ETIMER1_CLK;
#endif

#if (SPC5_ICU_USE_SMOD8 == TRUE)
  /* Driver initialization.*/
  ICUD9.config = NULL;
  ICUD9.etimerp = &SPC5_ETIMER_1;
  ICUD9.smod_number = 2U;
  ICUD9.clock = SPC5_ETIMER1_CLK;
#endif

#if (SPC5_ICU_USE_SMOD9 == TRUE)
  /* Driver initialization.*/
  ICUD10.config = NULL;
  ICUD10.etimerp = &SPC5_ETIMER_1;
  ICUD10.smod_number = 3U;
  ICUD10.clock = SPC5_ETIMER1_CLK;
#endif

#if (SPC5_ICU_USE_SMOD10 == TRUE)
  /* Driver initialization.*/
  ICUD11.config = NULL;
  ICUD11.etimerp = &SPC5_ETIMER_1;
  ICUD11.smod_number = 4U;
  ICUD11.clock = SPC5_ETIMER1_CLK;
#endif

#if (SPC5_ICU_USE_SMOD11 == TRUE)
  /* Driver initialization.*/
  ICUD12.config = NULL;
  ICUD12.etimerp = &SPC5_ETIMER_1;
  ICUD12.smod_number = 5U;
  ICUD12.clock = SPC5_ETIMER1_CLK;
#endif

#if (SPC5_ICU_USE_SMOD12 == TRUE)
  /* Driver initialization.*/
  ICUD13.config = NULL;
  ICUD13.etimerp = &SPC5_ETIMER_2;
  ICUD13.smod_number = 0U;
  ICUD13.clock = SPC5_ETIMER2_CLK;
#endif

#if (SPC5_ICU_USE_SMOD13 == TRUE)
  /* Driver initialization.*/
  ICUD14.config = NULL;
  ICUD14.etimerp = &SPC5_ETIMER_2;
  ICUD14.smod_number = 1U;
  ICUD14.clock = SPC5_ETIMER2_CLK;
#endif

#if (SPC5_ICU_USE_SMOD14 == TRUE)
  /* Driver initialization.*/
  ICUD15.config = NULL;
  ICUD15.etimerp = &SPC5_ETIMER_2;
  ICUD15.smod_number = 2U;
  ICUD15.clock = SPC5_ETIMER2_CLK;
#endif

#if (SPC5_ICU_USE_SMOD15 == TRUE)
  /* Driver initialization.*/
  ICUD16.config = NULL;
  ICUD16.etimerp = &SPC5_ETIMER_2;
  ICUD16.smod_number = 3U;
  ICUD16.clock = SPC5_ETIMER2_CLK;
#endif

#if (SPC5_ICU_USE_SMOD16 == TRUE)
  /* Driver initialization.*/
  ICUD17.config = NULL;
  ICUD17.etimerp = &SPC5_ETIMER_2;
  ICUD17.smod_number = 4U;
  ICUD17.clock = SPC5_ETIMER2_CLK;
#endif

#if (SPC5_ICU_USE_SMOD17 == TRUE)
  /* Driver initialization.*/
  ICUD18.config = NULL;
  ICUD18.etimerp = &SPC5_ETIMER_2;
  ICUD18.smod_number = 5U;
  ICUD18.clock = SPC5_ETIMER2_CLK;
#endif

#if (SPC5_ICU_USE_SMOD18 == TRUE)
  /* Driver initialization.*/
  ICUD19.config = NULL;
  ICUD19.etimerp = &SPC5_ETIMER_3;
  ICUD19.smod_number = 0U;
  ICUD19.clock = SPC5_ETIMER3_CLK;
#endif

#if (SPC5_ICU_USE_SMOD19 == TRUE)
  /* Driver initialization.*/
  ICUD20.config = NULL;
  ICUD20.etimerp = &SPC5_ETIMER_3;
  ICUD20.smod_number = 1U;
  ICUD20.clock = SPC5_ETIMER3_CLK;
#endif

#if (SPC5_ICU_USE_SMOD20 == TRUE)
  /* Driver initialization.*/
  ICUD21.config = NULL;
  ICUD21.etimerp = &SPC5_ETIMER_3;
  ICUD21.smod_number = 2U;
  ICUD21.clock = SPC5_ETIMER3_CLK;
#endif

#if (SPC5_ICU_USE_SMOD21 == TRUE)
  /* Driver initialization.*/
  ICUD22.config = NULL;
  ICUD22.etimerp = &SPC5_ETIMER_3;
  ICUD22.smod_number = 3U;
  ICUD22.clock = SPC5_ETIMER3_CLK;
#endif

#if (SPC5_ICU_USE_SMOD22 == TRUE)
  /* Driver initialization.*/
  ICUD23.config = NULL;
  ICUD23.etimerp = &SPC5_ETIMER_3;
  ICUD23.smod_number = 4U;
  ICUD23.clock = SPC5_ETIMER3_CLK;
#endif

#if (SPC5_ICU_USE_SMOD23 == TRUE)
  /* Driver initialization.*/
  ICUD24.config = NULL;
  ICUD24.etimerp = &SPC5_ETIMER_3;
  ICUD24.smod_number = 5U;
  ICUD24.clock = SPC5_ETIMER3_CLK;
#endif

#if (SPC5_ICU_USE_ETIMER0 == TRUE)

  INTC.PSR[SPC5_ETIMER0_TC0IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_TC1IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_TC2IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_TC3IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_TC4IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_TC5IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_WTIF_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_RCF_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;

#endif

#if (SPC5_ICU_USE_ETIMER1 == TRUE)

  INTC.PSR[SPC5_ETIMER1_TC0IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_TC1IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_TC2IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_TC3IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_TC4IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_TC5IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_RCF_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;

#endif

#if (SPC5_ICU_USE_ETIMER2 == TRUE)

  INTC_PSR(SPC5_ETIMER2_TC0IR_NUMBER) = SPC5_ICU_ETIMER2_PRIORITY;
  INTC_PSR(SPC5_ETIMER2_TC1IR_NUMBER) = SPC5_ICU_ETIMER2_PRIORITY;
  INTC_PSR(SPC5_ETIMER2_TC2IR_NUMBER) = SPC5_ICU_ETIMER2_PRIORITY;
  INTC_PSR(SPC5_ETIMER2_TC3IR_NUMBER) = SPC5_ICU_ETIMER2_PRIORITY;
  INTC_PSR(SPC5_ETIMER2_TC4IR_NUMBER) = SPC5_ICU_ETIMER2_PRIORITY;
  INTC_PSR(SPC5_ETIMER2_TC5IR_NUMBER) = SPC5_ICU_ETIMER2_PRIORITY;
  INTC_PSR(SPC5_ETIMER2_RCF_NUMBER) = SPC5_ICU_ETIMER2_PRIORITY;

#endif

#if (SPC5_ICU_USE_ETIMER3 == TRUE)

  INTC_PSR(SPC5_ETIMER3_TC0IR_NUMBER) = SPC5_ICU_ETIMER3_PRIORITY;
  INTC_PSR(SPC5_ETIMER3_TC1IR_NUMBER) = SPC5_ICU_ETIMER3_PRIORITY;
  INTC_PSR(SPC5_ETIMER3_TC2IR_NUMBER) = SPC5_ICU_ETIMER3_PRIORITY;
  INTC_PSR(SPC5_ETIMER3_TC3IR_NUMBER) = SPC5_ICU_ETIMER3_PRIORITY;
  INTC_PSR(SPC5_ETIMER3_TC4IR_NUMBER) = SPC5_ICU_ETIMER3_PRIORITY;
  INTC_PSR(SPC5_ETIMER3_TC5IR_NUMBER) = SPC5_ICU_ETIMER3_PRIORITY;
  INTC_PSR(SPC5_ETIMER3_RCF_NUMBER) = SPC5_ICU_ETIMER3_PRIORITY;

#endif
}

/**
 * @brief   Configures and activates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @param[in] config    pointer to the @p ICUConfig object
 *
 * @api
 */
void icu_lld_start(ICUDriver *icup, const ICUConfig *config) {

  irqIsrDisable();
  
  icup->config = config;
  
#if (SPC5_ICU_USE_SMOD0 == TRUE)
  if (&ICUD1 == icup) {
    icu_active_submodules0++;
  }
#endif
#if (SPC5_ICU_USE_SMOD1 == TRUE)
  if (&ICUD2 == icup) {
    icu_active_submodules0++;
  }
#endif
#if (SPC5_ICU_USE_SMOD2 == TRUE)
  if (&ICUD3 == icup) {
    icu_active_submodules0++;
  }
#endif
#if (SPC5_ICU_USE_SMOD3 == TRUE)
  if (&ICUD4 == icup) {
    icu_active_submodules0++;
  }
#endif
#if (SPC5_ICU_USE_SMOD4 == TRUE)
  if (&ICUD5 == icup) {
    icu_active_submodules0++;
  }
#endif
#if (SPC5_ICU_USE_SMOD5 == TRUE)
  if (&ICUD6 == icup) {
    icu_active_submodules0++;
  }
#endif
#if (SPC5_ICU_USE_SMOD6 == TRUE)
  if (&ICUD7 == icup) {
    icu_active_submodules1++;
  }
#endif
#if (SPC5_ICU_USE_SMOD7 == TRUE)
  if (&ICUD8 == icup) {
    icu_active_submodules1++;
  }
#endif
#if (SPC5_ICU_USE_SMOD8 == TRUE)
  if (&ICUD9 == icup) {
    icu_active_submodules1++;
  }
#endif
#if (SPC5_ICU_USE_SMOD9 == TRUE)
  if (&ICUD10 == icup) {
    icu_active_submodules1++;
  }
#endif
#if (SPC5_ICU_USE_SMOD10 == TRUE)
  if (&ICUD11 == icup) {
    icu_active_submodules1++;
  }
#endif
#if (SPC5_ICU_USE_SMOD11 == TRUE)
  if (&ICUD12 == icup) {
    icu_active_submodules1++;
  }
#endif
#if (SPC5_ICU_USE_SMOD12 == TRUE)
  if (&ICUD13 == icup) {
    icu_active_submodules2++;
  }
#endif
#if (SPC5_ICU_USE_SMOD13 == TRUE)
  if (&ICUD14 == icup) {
    icu_active_submodules2++;
  }
#endif
#if (SPC5_ICU_USE_SMOD14 == TRUE)
  if (&ICUD15 == icup) {
    icu_active_submodules2++;
  }
#endif
#if (SPC5_ICU_USE_SMOD15 == TRUE)
  if (&ICUD16 == icup) {
    icu_active_submodules2++;
  }
#endif
#if (SPC5_ICU_USE_SMOD16 == TRUE)
  if (&ICUD17 == icup) {
    icu_active_submodules2++;
  }
#endif
#if (SPC5_ICU_USE_SMOD17 == TRUE)
  if (&ICUD18 == icup) {
    icu_active_submodules2++;
  }
#endif
#if (SPC5_ICU_USE_SMOD18 == TRUE)
  if (&ICUD19 == icup) {
    icu_active_submodules3++;
  }
#endif
#if (SPC5_ICU_USE_SMOD19 == TRUE)
  if (&ICUD20 == icup) {
    icu_active_submodules3++;
  }
#endif
#if (SPC5_ICU_USE_SMOD20 == TRUE)
  if (&ICUD21 == icup) {
    icu_active_submodules3++;
  }
#endif
#if (SPC5_ICU_USE_SMOD21 == TRUE)
  if (&ICUD22 == icup) {
    icu_active_submodules3++;
  }
#endif
#if (SPC5_ICU_USE_SMOD22 == TRUE)
  if (&ICUD23 == icup) {
    icu_active_submodules3++;
  }
#endif
#if (SPC5_ICU_USE_SMOD23 == TRUE)
  if (&ICUD24 == icup) {
    icu_active_submodules3++;
  }
#endif

  /* Set eTimer0 Clock.*/
#if (SPC5_ICU_USE_ETIMER0 == TRUE)

  /* If this is the first Submodule activated then the eTimer0 is enabled.*/
  if (icu_active_submodules0 == 1U) {
    SPCSetPeripheralClockMode(SPC5_ETIMER0_PCTL,
                                 SPC5_ICU_ETIMER0_START_PCTL);
  }
#endif

  /* Set eTimer1 Clock.*/
#if (SPC5_ICU_USE_ETIMER1 == TRUE)
  /* If this is the first Submodule activated then the eTimer1 is enabled.*/
  if (icu_active_submodules1 == 1U) {
    SPCSetPeripheralClockMode(SPC5_ETIMER1_PCTL,
                                 SPC5_ICU_ETIMER1_START_PCTL);
  }
#endif

  /* Set eTimer2 Clock.*/
#if (SPC5_ICU_USE_ETIMER2 == TRUE)
  /* If this is the first Submodule activated then the eTimer2 is enabled.*/
  if (icu_active_submodules2 == 1U) {
    SPCSetPeripheralClockMode(SPC5_ETIMER2_PCTL,
                                 SPC5_ICU_ETIMER2_START_PCTL);
  }
#endif

  /* Set eTimer3 Clock.*/
#if (SPC5_ICU_USE_ETIMER3 == TRUE)
  /* If this is the first Submodule activated then the eTimer3 is enabled.*/
  if (icu_active_submodules3 == 1U) {
    SPCSetPeripheralClockMode(SPC5_ETIMER3_PCTL,
                                 SPC5_ICU_ETIMER3_START_PCTL);
  }
#endif

  /* Timer disabled.*/
  icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.CNTMODE =
      SPC5_ETIMER_CNTMODE_NO_OPERATION;

  /* Clear pending IRQs (if any).*/
  icup->etimerp->CHANNEL[icup->smod_number].STS.R = 0xFFFF;

  /* All IRQs and DMA requests disabled.*/
  icup->etimerp->CHANNEL[icup->smod_number].INTDMA.R = 0U;

  /* Compare Load 1 and Compare Load 2 disabled.*/
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.CLC1 = 0U;
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.CLC2 = 0U;

  /* Capture 1 and Capture 2 disabled.*/
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.CPT1MODE =
      SPC5_ETIMER_CPT1MODE_DISABLED;
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.CPT2MODE =
      SPC5_ETIMER_CPT2MODE_DISABLED;

  /* Counter reset to zero.*/
  icup->etimerp->CHANNEL[icup->smod_number].CNTR.R = 0U;

  /* Configuration.*/
  spc5_icu_smod_init(icup);
  
  irqIsrEnable();
}

/**
 * @brief   Deactivates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icu_lld_stop(ICUDriver *icup) {

  irqIsrDisable();

#if (SPC5_ICU_USE_SMOD0 == TRUE)
  if (&ICUD1 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFEU;
    icu_active_submodules0--;
  }
#endif
#if (SPC5_ICU_USE_SMOD1 == TRUE)
  if (&ICUD2 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFDU;
    icu_active_submodules0--;
  }
#endif
#if (SPC5_ICU_USE_SMOD2 == TRUE)
  if (&ICUD3 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFBU;
    icu_active_submodules0--;
  }
#endif
#if (SPC5_ICU_USE_SMOD3 == TRUE)
  if (&ICUD4 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xF7U;
    icu_active_submodules0--;
  }
#endif
#if (SPC5_ICU_USE_SMOD4 == TRUE)
  if (&ICUD5 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xEFU;
    icu_active_submodules0--;
  }
#endif
#if (SPC5_ICU_USE_SMOD5 == TRUE)
  if (&ICUD6 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xDFU;
    icu_active_submodules0--;
  }
#endif
#if (SPC5_ICU_USE_SMOD6 == TRUE)
  if (&ICUD7 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFEU;
    icu_active_submodules1--;
  }
#endif
#if (SPC5_ICU_USE_SMOD7 == TRUE)
  if (&ICUD8 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFDU;
    icu_active_submodules1--;
  }
#endif
#if (SPC5_ICU_USE_SMOD8 == TRUE)
  if (&ICUD9 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFBU;
    icu_active_submodules1--;
  }
#endif
#if (SPC5_ICU_USE_SMOD9 == TRUE)
  if (&ICUD10 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xF7U;
    icu_active_submodules1--;
  }
#endif
#if (SPC5_ICU_USE_SMOD10 == TRUE)
  if (&ICUD11 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xEFU;
    icu_active_submodules1--;
  }
#endif
#if (SPC5_ICU_USE_SMOD11 == TRUE)
  if (&ICUD12 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xDFU;
    icu_active_submodules1--;
  }
#endif
#if (SPC5_ICU_USE_SMOD12 == TRUE)
  if (&ICUD13 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFEU;
    icu_active_submodules2--;
  }
#endif
#if (SPC5_ICU_USE_SMOD13 == TRUE)
  if (&ICUD14 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFDU;
    icu_active_submodules2--;
  }
#endif
#if (SPC5_ICU_USE_SMOD14 == TRUE)
  if (&ICUD15 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFBU;
    icu_active_submodules2--;
  }
#endif
#if (SPC5_ICU_USE_SMOD15 == TRUE)
  if (&ICUD16 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xF7U;
    icu_active_submodules2--;
  }
#endif
#if (SPC5_ICU_USE_SMOD16 == TRUE)
  if (&ICUD17 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xEFU;
    icu_active_submodules2--;
  }
#endif
#if (SPC5_ICU_USE_SMOD17 == TRUE)
  if (&ICUD18 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xDFU;
    icu_active_submodules2--;
  }
#endif
#if (SPC5_ICU_USE_SMOD18 == TRUE)
  if (&ICUD19 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFEU;
    icu_active_submodules3--;
  }
#endif
#if (SPC5_ICU_USE_SMOD19 == TRUE)
  if (&ICUD20 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFDU;
    icu_active_submodules3--;
  }
#endif
#if (SPC5_ICU_USE_SMOD20 == TRUE)
  if (&ICUD21 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xFBU;
    icu_active_submodules3--;
  }
#endif
#if (SPC5_ICU_USE_SMOD21 == TRUE)
  if (&ICUD22 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xF7U;
    icu_active_submodules3--;
  }
#endif
#if (SPC5_ICU_USE_SMOD22 == TRUE)
  if (&ICUD23 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xEFU;
    icu_active_submodules3--;
  }
#endif
#if (SPC5_ICU_USE_SMOD23 == TRUE)
  if (&ICUD24 == icup) {
    /* Disable channel.*/
    icup->etimerp->ENBL.B.ENBL &= 0xDFU;
    icu_active_submodules3--;
  }
#endif
  /* eTimer0 clock deactivation.*/
#if (SPC5_ICU_USE_ETIMER0 == TRUE)
  /* If it is the last active submodules then the eTimer0 is disabled.*/
  if (icu_active_submodules0 == 0U) {
    if (icup->etimerp->ENBL.B.ENBL == 0U) {
      SPCSetPeripheralClockMode(SPC5_ETIMER0_PCTL,
                                   SPC5_ICU_ETIMER0_STOP_PCTL);
    }
  }
#endif

  /* eTimer1 clock deactivation.*/
#if (SPC5_ICU_USE_ETIMER1 == TRUE)
  /* If it is the last active submodules then the eTimer1 is disabled.*/
  if (icu_active_submodules1 == 0U) {
    if (icup->etimerp->ENBL.B.ENBL == 0U) {
      SPCSetPeripheralClockMode(SPC5_ETIMER1_PCTL,
                                   SPC5_ICU_ETIMER1_STOP_PCTL);
    }
  }
#endif

  /* eTimer2 clock deactivation.*/
#if (SPC5_ICU_USE_ETIMER2 == TRUE)
  /* If it is the last active submodules then the eTimer2 is disabled.*/
  if (icu_active_submodules2 == 0U) {
    if (icup->etimerp->ENBL.B.ENBL == 0U) {
      SPCSetPeripheralClockMode(SPC5_ETIMER2_PCTL,
                                   SPC5_ICU_ETIMER2_STOP_PCTL);
    }
  }
#endif

  /* eTimer3 clock deactivation.*/
#if (SPC5_ICU_USE_ETIMER3 == TRUE)
  /* If it is the last active submodules then the eTimer3 is disabled.*/
  if (icu_active_submodules3 == 0U) {
    if (icup->etimerp->ENBL.B.ENBL == 0U) {
      SPCSetPeripheralClockMode(SPC5_ETIMER3_PCTL,
                                   SPC5_ICU_ETIMER3_STOP_PCTL);
    }
  }
#endif

  irqIsrEnable();
}

/**
 * @brief   Enables the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icu_lld_enable(ICUDriver *icup) {

  irqIsrDisable();

  /* Clear pending IRQs (if any).*/
  icup->etimerp->CHANNEL[icup->smod_number].STS.R = 0xFFFFU;

  /* Set Capture 1 and Capture 2 Mode.*/
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.CPT1MODE =
      SPC5_ETIMER_CPT1MODE_RISING_EDGE;
  icup->etimerp->CHANNEL[icup->smod_number].CTRL3.B.ROC =
      SPC5_ETIMER_ROC_REL_ON_CAP1;
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.CPT2MODE =
      SPC5_ETIMER_CPT2MODE_FALLING_EDGE;

  /* Active interrupts.*/
  if (icup->config->period_cb != NULL || icup->config->width_cb != NULL) {
    icup->etimerp->CHANNEL[icup->smod_number].INTDMA.B.ICF1IE = 1U;
    icup->etimerp->CHANNEL[icup->smod_number].INTDMA.B.ICF2IE = 1U;
  }
  if (icup->config->overflow_cb != NULL) {
    icup->etimerp->CHANNEL[icup->smod_number].INTDMA.B.TOFIE = 1U;
  }

  /* Set Capture FIFO Water Mark.*/
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.CFWM = 0U;

  /* Enable Counter.*/
#if (ICU_SKIP_FIRST_CAPTURE == TRUE)
  icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.CNTMODE =
      SPC5_ETIMER_CNTMODE_RFE_SIHA;
#else
  icup->etimerp->CHANNEL[icup->smod_number].CTRL.B.CNTMODE =
      SPC5_ETIMER_CNTMODE_RE;
#endif /* ICU_SKIP_FIRST_CAPTURE */

  /* Enable Capture process.*/
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.ARM = 1U;
  
  irqIsrEnable();
}

/**
 * @brief   Disables the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icu_lld_disable(ICUDriver *icup) {

  irqIsrDisable();

  /* Disable Capture process.*/
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.ARM = 0U;

  /* Clear pending IRQs (if any).*/
  icup->etimerp->CHANNEL[icup->smod_number].STS.R = 0xFFFFU;

  /* Set Capture 1 and Capture 2 Mode to Disabled.*/
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.CPT1MODE =
      SPC5_ETIMER_CPT1MODE_DISABLED;
  icup->etimerp->CHANNEL[icup->smod_number].CCCTRL.B.CPT2MODE =
      SPC5_ETIMER_CPT2MODE_DISABLED;

  /* Disable interrupts.*/
  if (icup->config->period_cb != NULL || icup->config->width_cb != NULL) {
    icup->etimerp->CHANNEL[icup->smod_number].INTDMA.B.ICF1IE = 0U;
    icup->etimerp->CHANNEL[icup->smod_number].INTDMA.B.ICF2IE = 0U;
  }
  if (icup->config->overflow_cb != NULL) {
    icup->etimerp->CHANNEL[icup->smod_number].INTDMA.B.TOFIE = 0U;
  }

  irqIsrEnable();
}

#endif /* LLD_USE_ICU */

/** @} */
