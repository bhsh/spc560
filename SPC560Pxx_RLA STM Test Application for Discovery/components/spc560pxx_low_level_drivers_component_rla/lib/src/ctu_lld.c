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
 * @file    ctu_lld.c
 * @brief   SPC5xx CTU low level driver code.
 *
 * @addtogroup CTU
 * @{
 */

#include "ctu_lld.h"

#if (LLD_USE_CTU == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   CTUD1 driver identifier.
 */
#if (SPC5_CTU_USE_CTU0 == TRUE) || defined(__DOXYGEN__)
CTUDriver CTUD1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (SPC5_CTU_USE_CTU0 == TRUE)
#if !defined(SPC5_CT0_ERR_I_HANDLER)
#error "SPC5_CT0_ERR_I_HANDLER not defined"
#endif
/**
 * @brief   CTU0 Error interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_CT0_ERR_I_HANDLER) {

  IRQ_PROLOGUE();

  /* Clear error flags */
  CTUD1.ctup_tagp->CTUEFR.R = (uint16_t)(SPC5_CTUEFR_ET_OE | SPC5_CTUEFR_T1_OE |
                              SPC5_CTUEFR_T0_OE | SPC5_CTUEFR_ADC_OE |
                              SPC5_CTUEFR_TGS_OSM | SPC5_CTUEFR_MRS_O |
                              SPC5_CTUEFR_ICE | SPC5_CTUEFR_SM_TO | 
                              SPC5_CTUEFR_MRS_RE);

  /* Execute the callback function */
  if (CTUD1.config->ie_cb != NULL) {
    CTUD1.config->ie_cb(&CTUD1);
  }

  IRQ_EPILOGUE();
}

#if !defined(SPC5_CT0_MRS_I_HANDLER)
#error "SPC5_CT0_MRS_I_HANDLER not defined"
#endif
/**
 * @brief   CTU0 MRS interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_CT0_MRS_I_HANDLER) {

  IRQ_PROLOGUE();

  /* Clear MRS interrupt flag */
  CTUD1.ctup_tagp->CTUIFR.B.MRS = 1U;

  /* Execute the callback function */
  if (CTUD1.config->mrs_i_cb != NULL) {
    CTUD1.config->mrs_i_cb(&CTUD1);
  }

  IRQ_EPILOGUE();
}

#if !defined(SPC5_CT0_T0_I_HANDLER)
#error "SPC5_CT0_T0_I_HANDLER not defined"
#endif
/**
 * @brief   CTU0 Trigger 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_CT0_T0_I_HANDLER) {

  IRQ_PROLOGUE();

  /* Clear T0 interrupt flag */
  CTUD1.ctup_tagp->CTUIFR.B.T0 = 1U;

  /* Execute the callback function */
  if (CTUD1.config->t_i_cb[0] != NULL) {
    CTUD1.config->t_i_cb[0](&CTUD1);
  }

  IRQ_EPILOGUE();
}

#if !defined(SPC5_CT0_T1_I_HANDLER)
#error "SPC5_CT0_T1_I_HANDLER not defined"
#endif
/**
 * @brief   CTU0 Trigger 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_CT0_T1_I_HANDLER) {

  IRQ_PROLOGUE();

  /* Clear T1 interrupt flag */
  CTUD1.ctup_tagp->CTUIFR.B.T1 = 1U;

  /* Execute the callback function */
  if (CTUD1.config->t_i_cb[1] != NULL) {
    CTUD1.config->t_i_cb[1](&CTUD1);
  }

  IRQ_EPILOGUE();
}

#if !defined(SPC5_CT0_T2_I_HANDLER)
#error "SPC5_CT0_T2_I_HANDLER not defined"
#endif
/**
 * @brief   CTU0 Trigger 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_CT0_T2_I_HANDLER) {

  IRQ_PROLOGUE();

  /* Clear T2 interrupt flag */
  CTUD1.ctup_tagp->CTUIFR.B.T2 = 1U;

  /* Execute the callback function */
  if (CTUD1.config->t_i_cb[2] != NULL) {
    CTUD1.config->t_i_cb[2](&CTUD1);
  }

  IRQ_EPILOGUE();
}

#if !defined(SPC5_CT0_T3_I_HANDLER)
#error "SPC5_CT0_T3_I_HANDLER not defined"
#endif
/**
 * @brief   CTU0 Trigger 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_CT0_T3_I_HANDLER) {

  IRQ_PROLOGUE();

  /* Clear T3 interrupt flag */
  CTUD1.ctup_tagp->CTUIFR.B.T3 = 1U;

  /* Execute the callback function */
  if (CTUD1.config->t_i_cb[3] != NULL) {
    CTUD1.config->t_i_cb[3](&CTUD1);
  }

  IRQ_EPILOGUE();
}

#if !defined(SPC5_CT0_T4_I_HANDLER)
#error "SPC5_CT0_T4_I_HANDLER not defined"
#endif
/**
 * @brief   CTU0 Trigger 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_CT0_T4_I_HANDLER) {

  IRQ_PROLOGUE();

  /* Clear T4 interrupt flag */
  CTUD1.ctup_tagp->CTUIFR.B.T4 = 1U;

  /* Execute the callback function */
  if (CTUD1.config->t_i_cb[4] != NULL) {
    CTUD1.config->t_i_cb[4](&CTUD1);
  }

  IRQ_EPILOGUE();
}

#if !defined(SPC5_CT0_T5_I_HANDLER)
#error "SPC5_CT0_T5_I_HANDLER not defined"
#endif
/**
 * @brief   CTU0 Trigger 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_CT0_T5_I_HANDLER) {

  IRQ_PROLOGUE();

  /* Clear T5 interrupt flag */
  CTUD1.ctup_tagp->CTUIFR.B.T5 = 1U;

  /* Execute the callback function */
  if (CTUD1.config->t_i_cb[5] != NULL) {
    CTUD1.config->t_i_cb[5](&CTUD1);
  }

  IRQ_EPILOGUE();
}

#if !defined(SPC5_CT0_T6_I_HANDLER)
#error "SPC5_CT0_T6_I_HANDLER not defined"
#endif
/**
 * @brief   CTU0 Trigger 6 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_CT0_T6_I_HANDLER) {

  IRQ_PROLOGUE();

  /* Clear T6 interrupt flag */
  CTUD1.ctup_tagp->CTUIFR.B.T6 = 1U;

  /* Execute the callback function */
  if (CTUD1.config->t_i_cb[6] != NULL) {
    CTUD1.config->t_i_cb[6](&CTUD1);
  }

  IRQ_EPILOGUE();
}

#if !defined(SPC5_CT0_T7_I_HANDLER)
#error "SPC5_CT0_T7_I_HANDLER not defined"
#endif
/**
 * @brief   CTU0 Trigger 7 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_CT0_T7_I_HANDLER) {

  IRQ_PROLOGUE();

  /* Clear T7 interrupt flag */
  CTUD1.ctup_tagp->CTUIFR.B.T7 = 1U;

  /* Execute the callback function */
  if (CTUD1.config->t_i_cb[7] != NULL) {
    CTUD1.config->t_i_cb[7](&CTUD1);
  }

  IRQ_EPILOGUE();
}
#endif /* SPC5_CTU_USE_CTU0 */


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level CTU driver initialization.
 *
 * @notapi
 */
void ctu_lld_init(void) {

#if (SPC5_CTU_USE_CTU0 == TRUE)
  /* Driver initialization.*/
  CTUD1.config = NULL;
  CTUD1.ctup_tagp = &SPC5_CTU_0;

  INTC.PSR[SPC5_CT0_MRS_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_T0_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_T1_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_T2_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_T3_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_T4_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_T5_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_T6_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_T7_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_FIFO1_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_FIFO2_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_FIFO3_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_FIFO4_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_ADC_I_NUMBER].R = SPC5_CTU0_PRIORITY;
  INTC.PSR[SPC5_CT0_ERR_I_NUMBER].R = SPC5_CTU0_PRIORITY;
#endif
}

/**
 * @brief   Configures and activates the CTU peripheral.
 *
 * @param[in] ctup      pointer to a @p CTUDriver object
 * @param[in] config    pointer to a @p CTUConfig object 
 *
 * @notapi
 */
void ctu_lld_start(CTUDriver *ctup, const CTUConfig *config) {

  uint8_t  i;
  uint32_t tgsisr_conf = 0U;
  uint32_t thcr1_conf  = 0U;
  uint32_t thcr2_conf  = 0U;
  uint16_t ctuir_conf  = 0U;

  irqIsrDisable();
  
  ctup->config = config;

#if SPC5_CTU_USE_CTU0
  /* Set Peripheral Clock.*/
  SPCSetPeripheralClockMode(SPC5_CTU0_PCTL,
                            SPC5_CTU0_START_PCTL);
#endif

  /* Clear the CTU error flags */
  CTUD1.ctup_tagp->CTUEFR.R = (uint16_t)(SPC5_CTUEFR_ET_OE | SPC5_CTUEFR_T1_OE |
                              SPC5_CTUEFR_T0_OE | SPC5_CTUEFR_ADC_OE |
                              SPC5_CTUEFR_TGS_OSM | SPC5_CTUEFR_MRS_O |
                              SPC5_CTUEFR_ICE | SPC5_CTUEFR_SM_TO | 
                              SPC5_CTUEFR_MRS_RE);
  
  /* Clear the CTU interrupt flags */
  ctup->ctup_tagp->CTUIFR.R = (uint16_t)(SPC5_CTUIFR_ADC_I | SPC5_CTUIFR_T7_I |
                              SPC5_CTUIFR_T6_I | SPC5_CTUIFR_T5_I |
                              SPC5_CTUIFR_T4_I | SPC5_CTUIFR_T3_I |
                              SPC5_CTUIFR_T2_I | SPC5_CTUIFR_T1_I |
                              SPC5_CTUIFR_T0_I | SPC5_CTUIFR_MRS_I);
 
  /* Reset the Global Reload Enable (GRE) bit */
  ctup->ctup_tagp->CTUCR.B.CGRE = 1U;
  
  /* Initialize the TGS Counter Reload Register to the minimum value */
  ctup->ctup_tagp->TGSCRR.R = SPC5_CTU_MIN_TGSCRR_VALUE;

  /* Initialize the TGS Counter Compare Register to the maximum value */
  ctup->ctup_tagp->TGSCCR.R = SPC5_CTU_MAX_TGSCCR_VALUE;

  /* Initialize the Triggers Compare Registers */
  for (i =0; i < NUM_OF_TCR; i++) {
    ctup->ctup_tagp->TCR[i].R = (ctup->config->tcr[i] + SPC5_CTU_MIN_TGSCRR_VALUE) & (0xFFFFU);
  }

  /* Enable the CTU Interrupts */
  for (i = 0U; i < 16U; i++) {
	ctuir_conf |= (uint16_t)(((uint32_t)(ctup->config->ctuir[i])) << (15U - ((0x0FU) & i)));
  }
  ctup->ctup_tagp->CTUIR.R = ctuir_conf;
  
  /* Set the TGS Counter Prescaler */
  ctup->ctup_tagp->TGSCR.B.PRES = ctup->config->prescaler - 1U;
  
  /* Set the Trigger Generator Subunit Mode to Triggered Mode (TGS_M = 0) */
  ctup->ctup_tagp->TGSCR.B.TGSM = 0;

  /* Configure the Toggle Mode for external triggers */
  ctup->ctup_tagp->TGSCR.B.ETTM = ctup->config->toggle_mode;

  /* Set the Trigger Handler Control Registers */
  for (i = 0U; i < 32U; i++) {
    thcr1_conf |= (((uint32_t)(ctup->config->thcr1[i])) << (31U - ((0x1FU) & i)));
	thcr2_conf |= (((uint32_t)(ctup->config->thcr2[i])) << (31U - ((0x1FU) & i)));
  }
  ctup->ctup_tagp->THCR1.R = thcr1_conf;
  ctup->ctup_tagp->THCR2.R = thcr2_conf;

  /* Set the Control ON time register */
  ctup->ctup_tagp->COTR.R = 0x0000U;

  /* Update the content of the double buffered registers */
  ctup->ctup_tagp->CTUCR.B.GRE = 1U;

  /* Set the Trigger Generator Sub-unit Input Selection Register */
  for (i = 0U; i < 32U; i++) {
    tgsisr_conf |= (((uint32_t)(ctup->config->tgsisr[i])) << (31U - ((0x1FU) & i)));
  }
  ctup->ctup_tagp->TGSISR.R = tgsisr_conf;

  /* Enable the TGS Input Selection Register Reload */
  ctup->ctup_tagp->CTUCR.B.TGSISRRE = 1;
  /* Wait until the double buffer register are updated */
  while (ctup->ctup_tagp->CTUCR.B.TGSISRRE != 0U) {

  }

  irqIsrEnable();
}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] ctup      pointer to a @p CTUDriver object
 *
 * @api
 */
void ctu_lld_stop(CTUDriver *ctup) {

  uint8_t  i;

  irqIsrDisable();

  /* Clear the TGS Counter Reload Register to the minim value */
  ctup->ctup_tagp->TGSCRR.R = 0x0000U;

  /* Clear the TGS Counter Compare Register to the maximum value */
  ctup->ctup_tagp->TGSCCR.R = 0x0000U;

  /* Clear the Triggers Compare Registers */
  for (i =0; i < NUM_OF_TCR; i++) {
    ctup->ctup_tagp->TCR[i].R = 0x0000U;
  }  
  
  /* Clear the Cross Triggering Unit Interrupt/DMA Register */
  ctup->ctup_tagp->CTUIR.R = 0x0000U;

  /* Clear the Trigger Handler Control Registers */
  ctup->ctup_tagp->THCR1.R = 0x00000000U;
  ctup->ctup_tagp->THCR2.R = 0x00000000U;

  /* Update the content of the double buffered registers */
  ctup->ctup_tagp->CTUCR.B.GRE = 1U;

  /* Clear the Trigger Generator Sub-unit Input Selection Register */
  ctup->ctup_tagp->TGSISR.R = 0x00000000U;;

  /* Enable the TGS Input Selection Register Reload */
  ctup->ctup_tagp->CTUCR.B.TGSISRRE = 1U;
  /* Wait until the double buffer register are updated */
  while (ctup->ctup_tagp->CTUCR.B.TGSISRRE != 0U) {

  }

#if (SPC5_CTU_USE_CTU0 == TRUE)
  /* Disables peripheral clock.*/
  SPCSetPeripheralClockMode(SPC5_CTU0_PCTL,
                            SPC5_CTU0_STOP_PCTL);
#endif

  irqIsrEnable();  
}

#endif /* LLD_USE_CTU */

/** @} */
