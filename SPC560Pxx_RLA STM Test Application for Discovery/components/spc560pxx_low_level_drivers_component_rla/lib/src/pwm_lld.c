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
 * @file    pwm_lld.c
 * @brief   SPC5xx PWM low level driver code.
 *
 * @addtogroup PWM
 * @{
 */

#include "pwm_lld.h"

#if (LLD_USE_PWM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PWMD1 driver identifier.
 */
#if (SPC5_PWM_USE_SMOD0 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD1;
#endif

/**
 * @brief   PWMD2 driver identifier.
 */
#if (SPC5_PWM_USE_SMOD1 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE) || defined(__DOXYGEN__)
PWMDriver PWMD2;
#endif

/**
 * @brief   PWMD3 driver identifier.
 */
#if (SPC5_PWM_USE_SMOD2 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE) || defined(__DOXYGEN__)
PWMDriver PWMD3;
#endif

/**
 * @brief   PWMD4 driver identifier.
 */
#if (SPC5_PWM_USE_SMOD3 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE) || defined(__DOXYGEN__)
PWMDriver PWMD4;
#endif

/**
 * @brief   PWMD5 driver identifier.
 */
#if (SPC5_PWM_USE_SMOD4 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD5;
#endif

/**
 * @brief   PWMD6 driver identifier.
 */
#if (SPC5_PWM_USE_SMOD5 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE) || defined(__DOXYGEN__)
PWMDriver PWMD6;
#endif

/**
 * @brief   PWMD7 driver identifier.
 */
#if (SPC5_PWM_USE_SMOD6 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE) || defined(__DOXYGEN__)
PWMDriver PWMD7;
#endif

/**
 * @brief   PWMD8 driver identifier.
 */
#if (SPC5_PWM_USE_SMOD7 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE) || defined(__DOXYGEN__)
PWMDriver PWMD8;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Number of active FlexPWM0 submodules.
 * @note    When SPC5_PWM0_USE_SYNC_SMOD is TRUE if the FlexPWM0 submodules are
 *          active, flexpwm_active_submodules0 = 1.
 */
#if (SPC5_PWM_USE_SMOD0 == TRUE) || (SPC5_PWM_USE_SMOD1 == TRUE) || \
    (SPC5_PWM_USE_SMOD2 == TRUE) || (SPC5_PWM_USE_SMOD3 == TRUE)
static uint32_t flexpwm_active_submodules0;
#endif

/**
 * @brief   Number of active FlexPWM1 submodules.
 * @note    When SPC5_PWM1_USE_SYNC_SMOD is TRUE if the FlexPWM1 submodules are
 *          active, flexpwm_active_submodules1 = 1.
 */
#if (SPC5_PWM_USE_SMOD4 == TRUE) || (SPC5_PWM_USE_SMOD5 == TRUE) || \
    (SPC5_PWM_USE_SMOD6 == TRUE) || (SPC5_PWM_USE_SMOD7 == TRUE)
static uint32_t flexpwm_active_submodules1;
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Configures and activates the PWM peripheral submodule.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] sid       PWM submodule identifier
 *
 * @notapi
 */
static void pwm_lld_start_submodule(PWMDriver *pwmp, uint8_t sid) {
  pwmcnt_t pwmperiod;
  uint32_t psc;
  uint8_t cha;
  uint8_t chb;

/* Retrieve the channel a/b of the submodule sid when the channel 
   synchronization is enabled.*/
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  cha = 0U;
  chb = 1U;
#elif (SPC5_PWM0_USE_SYNC_SMOD == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD1 == pwmp) {    
    cha = (uint8_t)(2U * sid);
    chb = (uint8_t)(2U * sid + 1U);
  } else {
    cha = 0U;
    chb = 1U;
  }
#elif (SPC5_PWM0_USE_SYNC_SMOD == FALSE && SPC5_PWM1_USE_SYNC_SMOD == TRUE)
  if (&PWMD5 == pwmp) {
    cha = (uint8_t)(2U * sid);
    chb = (uint8_t)(2U * sid + 1U);
  } else {
    cha = 0U;
    chb = 1U;
  }
#else
  cha = (uint8_t)(2U * sid);
  chb = (uint8_t)(2U * sid + 1U);
#endif

  /* Clears Status Register.*/
  pwmp->flexpwmp->SUB[sid].STS.R = 0xFFFF;

  /* Clears LDOK and initializes the registers.*/
  pwmp->flexpwmp->MCTRL.B.CLDOK |= 1U << sid;

  /* Setting PWM clock frequency and submodule prescaler.*/
  psc = (uint32_t)SPC5_FLEXPWM0_CLK / pwmp->config->frequency;

  switch (psc) {
  case 1:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = SPC5_FLEXPWM_PSC_1;
    break;
  case 2:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = SPC5_FLEXPWM_PSC_2;
    break;
  case 4:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = SPC5_FLEXPWM_PSC_4;
    break;
  case 8:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = SPC5_FLEXPWM_PSC_8;
    break;
  case 16:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = SPC5_FLEXPWM_PSC_16;
    break;
  case 32:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = SPC5_FLEXPWM_PSC_32;
    break;
  case 64:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = SPC5_FLEXPWM_PSC_64;
    break;
  case 128:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = SPC5_FLEXPWM_PSC_128;
    break;
  default:
    ;
  }

  /* Disables PWM FAULT function. */
  pwmp->flexpwmp->SUB[sid].DISMAP.R = 0;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP = 1U;

  /* Sets PWM period.*/
  pwmperiod = pwmp->period;
  pwmp->flexpwmp->SUB[sid].INIT.R = ~(pwmperiod / 2U) + 1U;
  pwmp->flexpwmp->SUB[sid].VAL[0].R = 0;
  pwmp->flexpwmp->SUB[sid].VAL[1].R = pwmperiod / 2U;

  /* Sets the submodule channels.*/
  switch (pwmp->config->mode & PWM_ALIGN_MASK) {
  case PWM_ALIGN_EDGE:
    /* Setting reloads.*/
    pwmp->flexpwmp->SUB[sid].CTRL.B.HALF = 0;
    pwmp->flexpwmp->SUB[sid].CTRL.B.FULL = 1;

    /* Setting active front of PWM channels.*/
    pwmp->flexpwmp->SUB[sid].VAL[2].R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[sid].VAL[4].R = ~(pwmperiod / 2U) + 1U;
    break;
  case PWM_ALIGN_CENTER:
    /* Setting reloads.*/
    pwmp->flexpwmp->SUB[sid].CTRL.B.HALF = 1;
    pwmp->flexpwmp->SUB[sid].CTRL.B.FULL = 0;
    break;
  default:
    ;
  }

  /* Polarities setup.*/
  switch (pwmp->config->channels[cha].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    pwmp->flexpwmp->SUB[sid].OCTRL.B.POLA = 1;

    /* Enables CHA mask and CHA.*/
    pwmp->flexpwmp->MASK.B.MASKA |= 1U << sid;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN |= 1U << sid;

    break;
  case PWM_OUTPUT_ACTIVE_HIGH:
    pwmp->flexpwmp->SUB[sid].OCTRL.B.POLA = 0;

    /* Enables CHA mask and CHA.*/
    pwmp->flexpwmp->MASK.B.MASKA |= 1U << sid;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN |= 1U << sid;

    break;
  case PWM_OUTPUT_DISABLED:
    /* Enables CHA mask.*/
    pwmp->flexpwmp->MASK.B.MASKA |= 1U << sid;

    break;
  default:
    ;
  }
  switch (pwmp->config->channels[chb].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    pwmp->flexpwmp->SUB[sid].OCTRL.B.POLB = 1;

    /* Enables CHB mask and CHB.*/
    pwmp->flexpwmp->MASK.B.MASKB |= 1U << sid;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN |= 1U << sid;

    break;
  case PWM_OUTPUT_ACTIVE_HIGH:
    pwmp->flexpwmp->SUB[sid].OCTRL.B.POLB = 0;

    /* Enables CHB mask and CHB.*/
    pwmp->flexpwmp->MASK.B.MASKB |= 1U << sid;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN |= 1U << sid;

    break;
  case PWM_OUTPUT_DISABLED:
    /* Enables CHB mask.*/
    pwmp->flexpwmp->MASK.B.MASKB |= 1U << sid;

    break;
  default:
    ;
  }

  /* Complementary output setup.*/
  switch (pwmp->config->channels[cha].mode & PWM_COMPLEMENT_OUT_MASK) {
  case PWM_COMPLEMENT_OUT_ACTIVE_LOW:
    pwmp->flexpwmp->SUB[sid].OCTRL.B.POLA = 1;
    pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP = 0U;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN |= 1U << sid;
    break;
  case PWM_COMPLEMENT_OUT_ACTIVE_HIGH:
    pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP = 0U;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN |= 1U << sid;
    break;
  default:
    ;
  }

  switch (pwmp->config->channels[chb].mode & PWM_COMPLEMENT_OUT_MASK) {
  case PWM_COMPLEMENT_OUT_ACTIVE_LOW:
    pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP = 0U;
    pwmp->flexpwmp->MCTRL.B.IPOL &= ~ (1U << sid);
    pwmp->flexpwmp->SUB[sid].OCTRL.B.POLB = 1;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN |= 1U << sid;
    break;
  case PWM_COMPLEMENT_OUT_ACTIVE_HIGH:
    pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP = 0U;
    pwmp->flexpwmp->MCTRL.B.IPOL |= 1U << sid;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN |= 1U << sid;
    break;
  default:
    ;
  }

  /* Set to 0 the deadtime when the channels are complementary */
  if (pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP == 0U) {
    pwmp->flexpwmp->SUB[sid].DTCNT0.R = 0x0;
    pwmp->flexpwmp->SUB[sid].DTCNT1.R = 0x0;
  }

  /* Sets the INIT and MASK registers.*/
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FRCEN = 1U;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FORCE = 1U;

  /* Sets as counter init of submodules 1-3 the master sync from the
     submodule 0.*/
#if (SPC5_PWM0_USE_SYNC_SMOD == TRUE)
  if (&PWMD1 == pwmp && sid != 0U) {
    pwmp->flexpwmp->SUB[sid].CTRL2.B.INIT_SEL = 2U;
  }
#endif
#if (SPC5_PWM1_USE_SYNC_SMOD == TRUE)
  if (&PWMD5 == pwmp && sid != 0U) {
    pwmp->flexpwmp->SUB[sid].CTRL2.B.INIT_SEL = 2U;
  }	
#endif

  /* Updates SMOD registers and starts SMOD.*/
  pwmp->flexpwmp->MCTRL.B.LDOK |= 1U << sid;
  pwmp->flexpwmp->MCTRL.B.RUN |= 1U << sid;
}

/**
 * @brief   Enables a PWM channel of a submodule.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 * @param[in] width     PWM pulse width as clock pulses number
 * @param[in] sid       PWM submodule id
 *
 * @notapi
 */
static void pwm_lld_enable_submodule_channel(PWMDriver *pwmp,
                                  pwmchannel_t channel,
                                  pwmcnt_t width, uint8_t sid) {
  pwmcnt_t pwmperiod;
  pwmperiod = pwmp->period;

  /* Clears LDOK.*/
  pwmp->flexpwmp->MCTRL.B.CLDOK |= 1U << sid;

  /* Activates the width interrupt.*/
  if ((channel % 2U) == 0U) {
    if (pwmp->config->channels[channel].callback != NULL) {
      if ((pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE & 0x08U) == 0U) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE |= 0x08U;
      }
    }

    /* Sets the channel width.*/
    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      if (width >= (pwmperiod / 2U)) {
        pwmp->flexpwmp->SUB[sid].VAL[3].R = width - (pwmperiod / 2U);
      } else {
        pwmp->flexpwmp->SUB[sid].VAL[3].R = ~((pwmperiod / 2U) - width) + 1U;
      }
      break;
    case PWM_ALIGN_CENTER:
      pwmp->flexpwmp->SUB[sid].VAL[3].R = width / 2U;
      pwmp->flexpwmp->SUB[sid].VAL[2].R = ~(width / 2U) + 1U;
      break;
    default:
      ;
    }

    /* Removes the channel mask if it is necessary.*/
    if ((pwmp->flexpwmp->MASK.B.MASKA & (1U << sid)) == (1U << sid)) {
      pwmp->flexpwmp->MASK.B.MASKA &= ~ (1U << sid);
    }

    if ((pwmp->config->channels[channel].mode & PWM_COMPLEMENT_OUT_MASK) != 0U) {
      pwmp->flexpwmp->MASK.B.MASKB &= ~ (1U << sid);
    }
  }
  /* Activates the width interrupt.*/
  else {
    if (pwmp->config->channels[channel].callback != NULL) {
      if ((pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE & 0x20U) == 0U) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE |= 0x20U;
      }
    }
    /* Sets the channel width.*/
    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      if (width >= (pwmperiod / 2U)) {
        pwmp->flexpwmp->SUB[sid].VAL[3].R = width - (pwmperiod / 2U);
      } else {
        pwmp->flexpwmp->SUB[sid].VAL[5].R = ~((pwmperiod / 2U) - width) + 1U;
      }
      break;
    case PWM_ALIGN_CENTER:
      pwmp->flexpwmp->SUB[sid].VAL[5].R = width / 2U;
      pwmp->flexpwmp->SUB[sid].VAL[4].R = ~(width / 2U) + 1U;
      break;
    default:
      ;
    }

    /* Removes the channel mask if it is necessary.*/
    if ((pwmp->flexpwmp->MASK.B.MASKB & (1U << sid)) == (1U << sid)) {
      pwmp->flexpwmp->MASK.B.MASKB &= ~ (1U << sid);
    }

    if ((pwmp->config->channels[channel].mode & PWM_COMPLEMENT_OUT_MASK) != 0U) {
      pwmp->flexpwmp->MASK.B.MASKA &= ~ (1U << sid);
    }
  }

  /* Activates the periodic interrupt. If channel synchronization is enabled,
     only the periodic interrupt of submodule 0 is activeted. */
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (pwmp->flexpwmp->SUB[sid].INTEN.B.RIE != 1U) {
    if (pwmp->config->callback != NULL) {
      pwmp->flexpwmp->SUB[sid].INTEN.B.RIE = 1;
    }
  }
#elif (SPC5_PWM0_USE_SYNC_SMOD == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (!(&PWMD1 == pwmp && sid != 0U)) {
    if (pwmp->flexpwmp->SUB[sid].INTEN.B.RIE != 1U) {
      if (pwmp->config->callback != NULL) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.RIE = 1;
      }
    }
  }
#elif (SPC5_PWM0_USE_SYNC_SMOD == FALSE && SPC5_PWM1_USE_SYNC_SMOD == TRUE)
  if (!(&PWMD5 == pwmp && sid != 0U)) {
    if (pwmp->flexpwmp->SUB[sid].INTEN.B.RIE != 1U) {
      if (pwmp->config->callback != NULL) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.RIE = 1;
      }
    }
  }
#else /* SPC5_PWM0_USE_SYNC_SMOD == TRUE && SPC5_PWM1_USE_SYNC_SMOD == TRUE */
  if ((&PWMD1 == pwmp || &PWMD5 == pwmp) && sid == 0U) {
    if (pwmp->flexpwmp->SUB[sid].INTEN.B.RIE != 1U) {
      if (pwmp->config->callback != NULL) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.RIE = 1;
      }
    }
  }
#endif

  /* Sets the MASK registers.*/
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FRCEN = 1U;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FORCE = 1U;

  /* Forces reload of the VALUE registers.*/
  pwmp->flexpwmp->MCTRL.B.LDOK |= 1U << sid;
}

/**
 * @brief   Disables a PWM channel of a submodule.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 * @param[in] sid       PWM submodule id
 *
 * @notapi
 */
static void pwm_lld_disable_submodule_channel(PWMDriver *pwmp,
                                   pwmchannel_t channel,
                                   uint8_t sid) {
  uint8_t maska, maskb;
  
  pwmp->flexpwmp->MCTRL.B.CLDOK |= 1U << sid;

  /* Disables the width interrupt.*/
  if ((channel % 2U) == 0U) {
    if (pwmp->config->channels[channel].callback != NULL) {
      if ((pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE & 0x08U) != 0U) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE &= 0x37U;
      }
    }

    /* Activates the channel mask.*/
    if ((pwmp->config->channels[channel].mode & PWM_COMPLEMENT_OUT_MASK) != 0U) {
      pwmp->flexpwmp->MASK.B.MASKA |= 1U << sid;
      pwmp->flexpwmp->MASK.B.MASKB |= 1U << sid;
    }
    else {
      pwmp->flexpwmp->MASK.B.MASKA |= 1U << sid;
    }
  }
  /* Disables the width interrupt.*/
  else {
    if (pwmp->config->channels[channel].callback != NULL) {
      if ((pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE & 0x20U) != 0U) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE &= 0x1FU;
      }
    }

    /* Activates the channel mask.*/
    if ((pwmp->config->channels[channel].mode & PWM_COMPLEMENT_OUT_MASK) != 0U) {
      pwmp->flexpwmp->MASK.B.MASKA |= 1U << sid;
      pwmp->flexpwmp->MASK.B.MASKB |= 1U << sid;
    }
    else {
      pwmp->flexpwmp->MASK.B.MASKB |= 1U << sid;
	}
  }

  /* Sets the MASK registers.*/
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FRCEN = 1U;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FORCE = 1U;

  maska = (uint8_t)pwmp->flexpwmp->MASK.B.MASKA;
  maskb = (uint8_t)pwmp->flexpwmp->MASK.B.MASKB;

  if (((maska & (1U << sid)) != 0U) && ((maskb & (1U << sid)) == 1U)) {
    /* Disables RIE interrupt to prevent reload interrupt.*/
    pwmp->flexpwmp->SUB[sid].INTEN.B.RIE = 0U;

    /* Clears the reload flag.*/
    pwmp->flexpwmp->SUB[sid].STS.B.RF = 1U;
  }

  pwmp->flexpwmp->MCTRL.B.LDOK |= (1U << sid);
}

/**
 * @brief   Common SMOD0...SMOD7 IRQ handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] sid       PWM submodule id
 */
static void pwm_lld_serve_interrupt(PWMDriver *pwmp, uint8_t sid) {
  uint16_t sr, inten;
  uint8_t cha;
  uint8_t chb;

/* Retrieve the channel a/b of the submodule sid when the channel
   synchronization is enabled.*/
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  cha = 0U;
  chb = 1U;
#elif (SPC5_PWM0_USE_SYNC_SMOD == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD1 == pwmp) {    
    cha = (uint8_t)(2U * sid);
    chb = (uint8_t)(2U * sid + 1U);
  } else {
    cha = 0U;
    chb = 1U;
  }
#elif (SPC5_PWM0_USE_SYNC_SMOD == FALSE && SPC5_PWM1_USE_SYNC_SMOD == TRUE)
  if (&PWMD5 == pwmp) {
    cha = (uint8_t)(2U * sid);
    chb = (uint8_t)(2U * sid + 1U);
  } else {
    cha = 0U;
    chb = 1U;
  }
#else
  cha = (uint8_t)(2U * sid);
  chb = (uint8_t)(2U * sid + 1U);
#endif

  inten = pwmp->flexpwmp->SUB[sid].INTEN.R;
  sr = pwmp->flexpwmp->SUB[sid].STS.R & inten;
  if ((sr & SPC5_FLEXPWM_STS_CMPF3) != 0U) {
    pwmp->flexpwmp->SUB[sid].STS.B.CMPF |= 0x08U;
    pwmp->config->channels[cha].callback(pwmp);
  }
  if ((sr & SPC5_FLEXPWM_STS_CMPF5) != 0U) {
    pwmp->flexpwmp->SUB[sid].STS.B.CMPF |= 0x20U;
    pwmp->config->channels[chb].callback(pwmp);
  }
  if ((sr & SPC5_FLEXPWM_STS_RF) != 0U) {
    pwmp->flexpwmp->SUB[sid].STS.B.RF = 1U;
    pwmp->config->callback(pwmp);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (SPC5_PWM_USE_SMOD0 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_FLEXPWM0_RF0_HANDLER)
#error "SPC5_FLEXPWM0_RF0_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD0 RF0 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM0_RF0_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD1, 0);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_FLEXPWM0_COF0_HANDLER)
#error "SPC5_FLEXPWM0_COF0_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD0 COF0 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM0_COF0_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD1, 0);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_PWM_USE_SMOD1 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
#if !defined(SPC5_FLEXPWM0_RF1_HANDLER)
#error "SPC5_FLEXPWM0_RF1_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD1 RF1 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM0_RF1_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD2, 1);

  IRQ_EPILOGUE();
}
#endif

#if !defined(SPC5_FLEXPWM0_COF1_HANDLER)
#error "SPC5_FLEXPWM0_COF1_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD1 COF1 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM0_COF1_HANDLER) {

  IRQ_PROLOGUE();

#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  pwm_lld_serve_interrupt(&PWMD2, 1);
#else
  pwm_lld_serve_interrupt(&PWMD1, 1);
#endif

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_PWM_USE_SMOD2 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
#if !defined(SPC5_FLEXPWM0_RF2_HANDLER)
#error "SPC5_FLEXPWM0_RF2_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD2 RF2 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM0_RF2_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD3, 2);

  IRQ_EPILOGUE();
}
#endif

#if !defined(SPC5_FLEXPWM0_COF2_HANDLER)
#error "SPC5_FLEXPWM0_COF2_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD2 COF2 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM0_COF2_HANDLER) {

  IRQ_PROLOGUE();

#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  pwm_lld_serve_interrupt(&PWMD3, 2);
#else
  pwm_lld_serve_interrupt(&PWMD1, 2);
#endif

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_PWM_USE_SMOD3 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
#if !defined(SPC5_FLEXPWM0_RF3_HANDLER)
#error "SPC5_FLEXPWM0_RF3_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD1 RF3 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM0_RF3_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD4, 3);

  IRQ_EPILOGUE();
}
#endif

#if !defined(SPC5_FLEXPWM0_COF3_HANDLER)
#error "SPC5_FLEXPWM0_COF3_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD1 COF3 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM0_COF3_HANDLER) {

  IRQ_PROLOGUE();

#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  pwm_lld_serve_interrupt(&PWMD4, 3);
#else
  pwm_lld_serve_interrupt(&PWMD1, 3);
#endif

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_PWM_USE_SMOD4 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_FLEXPWM1_RF0_HANDLER)
#error "SPC5_FLEXPWM0_RF1_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM1-SMOD0 RF0 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM1_RF0_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD5, 0);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_FLEXPWM1_COF0_HANDLER)
#error "SPC5_FLEXPWM1_COF0_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM1-SMOD0 COF0 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM1_COF0_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD5, 0);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_PWM_USE_SMOD5 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
#if !defined(SPC5_FLEXPWM1_RF1_HANDLER)
#error "SPC5_FLEXPWM1_RF1_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM1-SMOD1 RF1 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM1_RF1_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD6, 1);

  IRQ_EPILOGUE();
}
#endif

#if !defined(SPC5_FLEXPWM1_COF1_HANDLER)
#error "SPC5_FLEXPWM1_COF1_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM1-SMOD1 COF1 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM1_COF1_HANDLER) {

  IRQ_PROLOGUE();

#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  pwm_lld_serve_interrupt(&PWMD6, 1);
#else
  pwm_lld_serve_interrupt(&PWMD5, 1);
#endif

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_PWM_USE_SMOD6 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
#if !defined(SPC5_FLEXPWM1_RF2_HANDLER)
#error "SPC5_FLEXPWM1_RF2_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM1-SMOD2 RF2 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM1_RF2_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD7, 2);

  IRQ_EPILOGUE();
}
#endif

#if !defined(SPC5_FLEXPWM1_COF2_HANDLER)
#error "SPC5_FLEXPWM1_COF2_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM1-SMOD2 COF2 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM1_COF2_HANDLER) {

  IRQ_PROLOGUE();

#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  pwm_lld_serve_interrupt(&PWMD7, 2);
#else
  pwm_lld_serve_interrupt(&PWMD5, 2);
#endif

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_PWM_USE_SMOD7 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
#if !defined(SPC5_FLEXPWM1_RF3_HANDLER)
#error "SPC5_FLEXPWM1_RF3_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM1-SMOD3 RF3 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM1_RF3_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD8, 3);

  IRQ_EPILOGUE();
}
#endif

#if !defined(SPC5_FLEXPWM1_COF3_HANDLER)
#error "SPC5_FLEXPWM1_COF3_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM1-SMOD3 COF3 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXPWM1_COF3_HANDLER) {

  IRQ_PROLOGUE();

#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  pwm_lld_serve_interrupt(&PWMD8, 3);
#else
  pwm_lld_serve_interrupt(&PWMD5, 3);
#endif

  IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level PWM driver initialization.
 *
 * @init
 */
void pwm_lld_init(void) {

  /* All FlexPWM submodules are initially not in use.*/
#if (SPC5_PWM_USE_SMOD0 == TRUE) || (SPC5_PWM_USE_SMOD1 == TRUE) || \
    (SPC5_PWM_USE_SMOD2 == TRUE) || (SPC5_PWM_USE_SMOD3 == TRUE)
  flexpwm_active_submodules0 = 0U;
#endif
#if (SPC5_PWM_USE_SMOD4 == TRUE) || (SPC5_PWM_USE_SMOD5 == TRUE) || \
    (SPC5_PWM_USE_SMOD6 == TRUE) || (SPC5_PWM_USE_SMOD7 == TRUE)
  flexpwm_active_submodules1 = 0U;
#endif

#if (SPC5_PWM_USE_SMOD0 == TRUE)
  /* Driver initialization.*/
  PWMD1.config = NULL;
  PWMD1.flexpwmp = &SPC5_FLEXPWM_0;
  INTC.PSR[SPC5_FLEXPWM0_RF0_NUMBER].R = SPC5_PWM_SMOD0_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_COF0_NUMBER].R = SPC5_PWM_SMOD0_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_CAF0_NUMBER].R = SPC5_PWM_SMOD0_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_FFLAG_NUMBER].R = SPC5_PWM_SMOD0_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_REF_NUMBER].R = SPC5_PWM_SMOD0_PRIORITY;
#endif

#if (SPC5_PWM_USE_SMOD1 == TRUE)
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  /* Driver initialization.*/
  PWMD2.config = NULL;
  PWMD2.flexpwmp = &SPC5_FLEXPWM_0;
#endif
  INTC.PSR[SPC5_FLEXPWM0_RF1_NUMBER].R = SPC5_PWM_SMOD1_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_COF1_NUMBER].R = SPC5_PWM_SMOD1_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_CAF1_NUMBER].R = SPC5_PWM_SMOD1_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_FFLAG_NUMBER].R = SPC5_PWM_SMOD1_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_REF_NUMBER].R = SPC5_PWM_SMOD1_PRIORITY;
#endif

#if (SPC5_PWM_USE_SMOD2 == TRUE)
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  /* Driver initialization.*/
  PWMD3.config = NULL;
  PWMD3.flexpwmp = &SPC5_FLEXPWM_0;
#endif
  INTC.PSR[SPC5_FLEXPWM0_RF2_NUMBER].R = SPC5_PWM_SMOD2_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_COF2_NUMBER].R = SPC5_PWM_SMOD2_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_CAF2_NUMBER].R = SPC5_PWM_SMOD2_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_FFLAG_NUMBER].R = SPC5_PWM_SMOD2_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_REF_NUMBER].R = SPC5_PWM_SMOD2_PRIORITY;
#endif

#if (SPC5_PWM_USE_SMOD3 == TRUE)
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  /* Driver initialization.*/
  PWMD4.config = NULL;
  PWMD4.flexpwmp = &SPC5_FLEXPWM_0;
#endif
  INTC.PSR[SPC5_FLEXPWM0_RF3_NUMBER].R = SPC5_PWM_SMOD3_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_COF3_NUMBER].R = SPC5_PWM_SMOD3_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_CAF3_NUMBER].R = SPC5_PWM_SMOD3_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_FFLAG_NUMBER].R = SPC5_PWM_SMOD3_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_REF_NUMBER].R = SPC5_PWM_SMOD3_PRIORITY;
#endif

#if (SPC5_PWM_USE_SMOD4 == TRUE)
  /* Driver initialization.*/
  PWMD5.config = NULL;
  PWMD5.flexpwmp = &SPC5_FLEXPWM_1;
  INTC.PSR[SPC5_FLEXPWM1_RF0_NUMBER].R = SPC5_PWM_SMOD4_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_COF0_NUMBER].R = SPC5_PWM_SMOD4_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_CAF0_NUMBER].R = SPC5_PWM_SMOD4_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_FFLAG_NUMBER].R = SPC5_PWM_SMOD4_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_REF_NUMBER].R = SPC5_PWM_SMOD4_PRIORITY;
#endif

#if (SPC5_PWM_USE_SMOD5 == TRUE)
#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  /* Driver initialization.*/
  PWMD6.config = NULL;
  PWMD6.flexpwmp = &SPC5_FLEXPWM_1;
#endif
  INTC.PSR[SPC5_FLEXPWM1_RF1_NUMBER].R = SPC5_PWM_SMOD5_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_COF1_NUMBER].R = SPC5_PWM_SMOD5_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_CAF1_NUMBER].R = SPC5_PWM_SMOD5_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_FFLAG_NUMBER].R = SPC5_PWM_SMOD5_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_REF_NUMBER].R = SPC5_PWM_SMOD5_PRIORITY;
#endif

#if (SPC5_PWM_USE_SMOD6 == TRUE)
#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  /* Driver initialization.*/
  PWMD7.config = NULL;
  PWMD7.flexpwmp = &SPC5_FLEXPWM_1;
#endif
  INTC.PSR[SPC5_FLEXPWM1_RF2_NUMBER].R = SPC5_PWM_SMOD6_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_COF2_NUMBER].R = SPC5_PWM_SMOD6_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_CAF2_NUMBER].R = SPC5_PWM_SMOD6_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_FFLAG_NUMBER].R = SPC5_PWM_SMOD6_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_REF_NUMBER].R = SPC5_PWM_SMOD6_PRIORITY;
#endif

#if (SPC5_PWM_USE_SMOD7 == TRUE)
#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  /* Driver initialization.*/
  PWMD8.config = NULL;
  PWMD8.flexpwmp = &SPC5_FLEXPWM_1;
#endif
  INTC.PSR[SPC5_FLEXPWM1_RF3_NUMBER].R = SPC5_PWM_SMOD7_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_COF3_NUMBER].R = SPC5_PWM_SMOD7_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_CAF3_NUMBER].R = SPC5_PWM_SMOD7_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_FFLAG_NUMBER].R = SPC5_PWM_SMOD7_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM1_REF_NUMBER].R = SPC5_PWM_SMOD7_PRIORITY;
#endif
}

/**
 * @brief   Configures and activates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] config    pointer to a @p PWMConfig object 
 *
 * @api
 */
void pwm_lld_start(PWMDriver *pwmp, const PWMConfig *config) {

  irqIsrDisable();
  
  pwmp->config = config;
  pwmp->period = config->period;

#if (SPC5_PWM_USE_SMOD0 == TRUE)
  if (&PWMD1 == pwmp) {
    flexpwm_active_submodules0++;
  }
#endif /* SPC5_PWM_USE_SMOD0 */

#if (SPC5_PWM_USE_SMOD1 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD2 == pwmp) {
    flexpwm_active_submodules0++;
  }
#endif /* (SPC5_PWM_USE_SMOD1 && !(SPC5_PWM0_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD2 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD3 == pwmp) {
    flexpwm_active_submodules0++;
  }
#endif /* (SPC5_PWM_USE_SMOD2 && !(SPC5_PWM0_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD3 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD4 == pwmp) {
    flexpwm_active_submodules0++;
  }
#endif /* (SPC5_PWM_USE_SMOD3 && !(SPC5_PWM0_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD4 == TRUE)
  if (&PWMD5 == pwmp) {
    flexpwm_active_submodules1++;
  }
#endif /* SPC5_PWM_USE_SMOD4 */

#if (SPC5_PWM_USE_SMOD5 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD6 == pwmp) {
    flexpwm_active_submodules1++;
  }
#endif /* (SPC5_PWM_USE_SMOD5 && !(SPC5_PWM1_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD6 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD7 == pwmp) {
    flexpwm_active_submodules1++;
  }
#endif /* (SPC5_PWM_USE_SMOD6 && !(SPC5_PWM1_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD7 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD8 == pwmp) {
    flexpwm_active_submodules1++;
  }
#endif /* (SPC5_PWM_USE_SMOD7 && !(SPC5_PWM1_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_FLEXPWM0 == TRUE)
  /* If this is the first FlexPWM0 submodule activated, sets Peripheral Clock.*/
  if (flexpwm_active_submodules0 == 1U) {
    SPCSetPeripheralClockMode(SPC5_FLEXPWM0_PCTL,
        SPC5_PWM_FLEXPWM0_START_PCTL);
  }
#endif

#if (SPC5_PWM_USE_FLEXPWM1 == TRUE)
  /* If this is the first FlexPWM1 submodule activated, sets Peripheral Clock.*/
  if (flexpwm_active_submodules1 == 1U) {
    SPCSetPeripheralClockMode(SPC5_FLEXPWM1_PCTL,
        SPC5_PWM_FLEXPWM1_START_PCTL);
  }
#endif

#if (SPC5_PWM_USE_SMOD0 == TRUE)
  if (&PWMD1 == pwmp) {
    pwm_lld_start_submodule(pwmp, 0);
#if (SPC5_PWM0_USE_SYNC_SMOD == TRUE)
    pwm_lld_start_submodule(pwmp, 1);
    pwm_lld_start_submodule(pwmp, 2);
    pwm_lld_start_submodule(pwmp, 3);
#endif
  }
#endif
#if (SPC5_PWM_USE_SMOD1 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD2 == pwmp) {
    pwm_lld_start_submodule(pwmp, 1);
  }
#endif
#if (SPC5_PWM_USE_SMOD2 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD3 == pwmp) {
    pwm_lld_start_submodule(pwmp, 2);
  }
#endif
#if (SPC5_PWM_USE_SMOD3 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD4 == pwmp) {
    pwm_lld_start_submodule(pwmp, 3);
  }
#endif
#if (SPC5_PWM_USE_SMOD4 == TRUE)
  if (&PWMD5 == pwmp) {
    pwm_lld_start_submodule(pwmp, 0);
#if (SPC5_PWM1_USE_SYNC_SMOD == TRUE)
    pwm_lld_start_submodule(pwmp, 1);
    pwm_lld_start_submodule(pwmp, 2);
    pwm_lld_start_submodule(pwmp, 3);
#endif
  }
#endif
#if (SPC5_PWM_USE_SMOD5 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD6 == pwmp) {
    pwm_lld_start_submodule(pwmp, 1);
  }
#endif
#if (SPC5_PWM_USE_SMOD6 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD7 == pwmp) {
    pwm_lld_start_submodule(pwmp, 2);
  }
#endif
#if (SPC5_PWM_USE_SMOD7 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD8 == pwmp) {
    pwm_lld_start_submodule(pwmp, 3);
  }
#endif
  irqIsrEnable();
}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @api
 */
void pwm_lld_stop(PWMDriver *pwmp) {

  irqIsrDisable();

#if (SPC5_PWM_USE_SMOD0 == TRUE)
  if (&PWMD1 == pwmp) {
    flexpwm_active_submodules0--;
  }
#endif /* SPC5_PWM_USE_SMOD0 */

#if (SPC5_PWM_USE_SMOD1 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD2 == pwmp) {
    flexpwm_active_submodules0--;
  }
#endif /* (SPC5_PWM_USE_SMOD1 && !(SPC5_PWM0_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD2 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD3 == pwmp) {
    flexpwm_active_submodules0--;
  }
#endif /* (SPC5_PWM_USE_SMOD2 && !(SPC5_PWM0_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD3 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD4 == pwmp) {
    flexpwm_active_submodules0--;
  }
#endif /* (SPC5_PWM_USE_SMOD3 && !(SPC5_PWM0_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD4 == TRUE)
  if (&PWMD5 == pwmp) {
    flexpwm_active_submodules1--;
  }
#endif /* SPC5_PWM_USE_SMOD4 */

#if (SPC5_PWM_USE_SMOD5 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD6 == pwmp) {
    flexpwm_active_submodules1--;
  }
#endif /* (SPC5_PWM_USE_SMOD5 && !(SPC5_PWM1_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD6 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD7 == pwmp) {
    flexpwm_active_submodules1--;
  }
#endif /* (SPC5_PWM_USE_SMOD6 && !(SPC5_PWM1_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD7 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD8 == pwmp) {
    flexpwm_active_submodules1--;
  }
#endif /* (SPC5_PWM_USE_SMOD7 && !(SPC5_PWM1_USE_SYNC_SMOD)) */

#if (SPC5_PWM_USE_SMOD0 == TRUE)
  if (&PWMD1 == pwmp) {
    /* FlexPWM0 SMOD 0 stop.*/
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 1U;
    pwmp->flexpwmp->SUB[0].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[0].STS.R = 0xFFFFU;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0xEU;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0xEU;

    pwmp->flexpwmp->MCTRL.B.RUN &= 0xEU;
#if (SPC5_PWM0_USE_SYNC_SMOD == TRUE)
    /* FlexPWM0 SMOD 1-3 stop.*/
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 0xEU;
    pwmp->flexpwmp->SUB[1].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[1].STS.R = 0xFFFFU;
    pwmp->flexpwmp->SUB[2].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[2].STS.R = 0xFFFFU;
    pwmp->flexpwmp->SUB[3].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[3].STS.R = 0xFFFFU;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0x1U;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0x1U;

    pwmp->flexpwmp->MCTRL.B.RUN &= 0x1U;
#endif
  }
#endif
#if (SPC5_PWM_USE_SMOD1 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD2 == pwmp) {
    /* FlexPWM0 SMOD 1 stop.*/
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 2U;
    pwmp->flexpwmp->SUB[1].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[1].STS.R = 0xFFFFU;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0xDU;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0xDU;

    pwmp->flexpwmp->MCTRL.B.RUN &= 0xDU;
  }
#endif
#if (SPC5_PWM_USE_SMOD2 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD3 == pwmp) {
    /* FlexPWM0 SMOD 2 stop.*/
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 4U;
    pwmp->flexpwmp->SUB[2].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[2].STS.R = 0xFFFFU;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0xBU;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0xBU;

    pwmp->flexpwmp->MCTRL.B.RUN &= 0xBU;
  }
#endif
#if (SPC5_PWM_USE_SMOD3 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD4 == pwmp) {
    /* FlexPWM0 SMOD 3 stop.*/
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 8U;
    pwmp->flexpwmp->SUB[3].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[3].STS.R = 0xFFFFU;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0x7U;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0x7U;

      pwmp->flexpwmp->MCTRL.B.RUN &= 0x7U;
    }
#endif
#if (SPC5_PWM_USE_SMOD4 == TRUE)
  if (&PWMD5 == pwmp) {
    /* FlexPWM1 SMOD 0 stop.*/
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 1U;
    pwmp->flexpwmp->SUB[0].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[0].STS.R = 0xFFFFU;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0xEU;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0xEU;

    pwmp->flexpwmp->MCTRL.B.RUN &= 0xEU;
#if (SPC5_PWM1_USE_SYNC_SMOD == TRUE)
    /* FlexPWM1 SMOD 1-3 stop.*/
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 0xEU;
    pwmp->flexpwmp->SUB[1].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[1].STS.R = 0xFFFFU;
    pwmp->flexpwmp->SUB[2].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[2].STS.R = 0xFFFFU;
    pwmp->flexpwmp->SUB[3].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[3].STS.R = 0xFFFFU;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0x1U;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0x1U;

    pwmp->flexpwmp->MCTRL.B.RUN &= 0x1U;
#endif
  }
#endif
#if (SPC5_PWM_USE_SMOD5 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD6 == pwmp) {
    /* FlexPWM1 SMOD 1 stop.*/
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 2U;
    pwmp->flexpwmp->SUB[1].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[1].STS.R = 0xFFFFU;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0xDU;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0xDU;

    pwmp->flexpwmp->MCTRL.B.RUN &= 0xDU;
  }
#endif
#if (SPC5_PWM_USE_SMOD6 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD7 == pwmp) {
    /* FlexPWM1 SMOD 2 stop.*/
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 4U;
    pwmp->flexpwmp->SUB[2].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[2].STS.R = 0xFFFFU;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0xBU;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0xBU;

    pwmp->flexpwmp->MCTRL.B.RUN &= 0xBU;
  }
#endif
#if (SPC5_PWM_USE_SMOD7 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD8 == pwmp) {
    /* FlexPWM1 SMOD 3 stop.*/
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 8U;
    pwmp->flexpwmp->SUB[3].INTEN.R = 0U;
    pwmp->flexpwmp->SUB[3].STS.R = 0xFFFFU;
    pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0x7U;
    pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0x7U;

    pwmp->flexpwmp->MCTRL.B.RUN &= 0x7U;
  }
#endif

#if (SPC5_PWM_USE_FLEXPWM0 == TRUE)
  /* Disables peripheral clock if there is not an activated module.*/
  if (flexpwm_active_submodules0 == 0U) {
    SPCSetPeripheralClockMode(SPC5_FLEXPWM0_PCTL,
                                 SPC5_PWM_FLEXPWM0_STOP_PCTL);
  }
#endif

#if (SPC5_PWM_USE_FLEXPWM1 == TRUE)
  /* Disables peripheral clock if there is not an activated module.*/
  if (flexpwm_active_submodules1 == 0U) {
    SPCSetPeripheralClockMode(SPC5_FLEXPWM1_PCTL,
                                 SPC5_PWM_FLEXPWM1_STOP_PCTL);
  }
#endif

  irqIsrEnable();
}

/**
 * @brief   Enables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwm_lld_start().
 * @post    The channel is active using the specified configuration.
 * @note    The function has effect at the next cycle start.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 * @param[in] width     PWM pulse width as clock pulses number
 *
 * @api
 */
void pwm_lld_enable_channel(PWMDriver *pwmp,
                            pwmchannel_t channel,
                            pwmcnt_t width) {

  irqIsrDisable();

#if (SPC5_PWM_USE_SMOD0 == TRUE)
  if (&PWMD1 == pwmp) {
#if (SPC5_PWM0_USE_SYNC_SMOD == TRUE)
    pwm_lld_enable_submodule_channel(pwmp, channel, width, channel/2U);
#else
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 0U);
#endif
  }
#endif
#if (SPC5_PWM_USE_SMOD1 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD2 == pwmp) {
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 1U);
  }
#endif
#if (SPC5_PWM_USE_SMOD2 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD3 == pwmp) {
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 2U);
  }
#endif
#if (SPC5_PWM_USE_SMOD3 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD4 == pwmp) {
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 3U);
  }
#endif
#if (SPC5_PWM_USE_SMOD4 == TRUE)
  if (&PWMD5 == pwmp) {
#if (SPC5_PWM1_USE_SYNC_SMOD == TRUE)
    pwm_lld_enable_submodule_channel(pwmp, channel, width, channel/2U);
#else
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 0U);
#endif
  }
#endif
#if (SPC5_PWM_USE_SMOD5 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD6 == pwmp) {
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 1U);
  }
#endif
#if (SPC5_PWM_USE_SMOD6 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD7 == pwmp) {
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 2U);
  }
#endif
#if (SPC5_PWM_USE_SMOD7 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD8 == pwmp) {
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 3U);
  }
#endif

  irqIsrEnable();
}

/**
 * @brief   Disables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwm_lld_start().
 * @post    The channel is disabled and its output line returned to the
 *          idle state.
 * @note    The function has effect at the next cycle start.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 *
 * @api
 */
void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel) {

  irqIsrDisable();

#if (SPC5_PWM_USE_SMOD0 == TRUE)
  if (&PWMD1 == pwmp) {
#if (SPC5_PWM0_USE_SYNC_SMOD == TRUE)
    pwm_lld_disable_submodule_channel(pwmp, channel, channel/2U);
#else
    pwm_lld_disable_submodule_channel(pwmp, channel, 0U);
#endif
  }
#endif
#if (SPC5_PWM_USE_SMOD1 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD2 == pwmp) {
    pwm_lld_disable_submodule_channel(pwmp, channel, 1U);
  }
#endif
#if (SPC5_PWM_USE_SMOD2 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD3 == pwmp) {
    pwm_lld_disable_submodule_channel(pwmp, channel, 2U);
  }
#endif
#if (SPC5_PWM_USE_SMOD3 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD4 == pwmp) {
    pwm_lld_disable_submodule_channel(pwmp, channel, 3U);
  }
#endif
#if (SPC5_PWM_USE_SMOD4 == TRUE)
  if (&PWMD5 == pwmp) {
#if (SPC5_PWM1_USE_SYNC_SMOD == TRUE)
    pwm_lld_disable_submodule_channel(pwmp, channel, channel/2U);
#else
    pwm_lld_disable_submodule_channel(pwmp, channel, 0U);
#endif
  }
#endif
#if (SPC5_PWM_USE_SMOD5 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD6 == pwmp) {
    pwm_lld_disable_submodule_channel(pwmp, channel, 1U);
  }
#endif
#if (SPC5_PWM_USE_SMOD6 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD7 == pwmp) {
    pwm_lld_disable_submodule_channel(pwmp, channel, 2U);
  }
#endif
#if (SPC5_PWM_USE_SMOD7 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD8 == pwmp) {
    pwm_lld_disable_submodule_channel(pwmp, channel, 3U);
  }
#endif

  irqIsrEnable();
}

/**
 * @brief   Changes the period the PWM peripheral.
 * @details This function changes the period of a PWM unit that has already
 *          been activated using @p pwm_lld_start().
 * @pre     The PWM unit must have been activated using @p pwm_lld_start().
 * @post    The PWM unit period is changed to the new value.
 * @note    The function has effect at the next cycle start.
 * @note    If a period is specified that is shorter than the pulse width
 *          programmed in one of the channels then the behavior is not
 *          guaranteed.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] period    new cycle time in ticks
 *
 * @api
 */
void pwm_lld_change_period(PWMDriver *pwmp, pwmcnt_t period) {

  pwmcnt_t pwmperiod = period;
  
  irqIsrDisable();

  pwmp->period = period;

#if (SPC5_PWM_USE_SMOD0 == TRUE)
  if (&PWMD1 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 1U;

    /* Setting PWM period of submodule 0.*/
    pwmp->flexpwmp->SUB[0].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[0].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[0].VAL[1].R = pwmperiod / 2U;

    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      /* Setting active front of PWM channels of submodule 0.*/
      pwmp->flexpwmp->SUB[0].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[0].VAL[4].R = ~(pwmperiod / 2U) + 1U;
      break;
    default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 1U;
#if (SPC5_PWM0_USE_SYNC_SMOD == TRUE)
    pwmp->flexpwmp->MCTRL.B.RUN &= 0x1U;
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 0xEU;

    /* Setting PWM period of submodules 1-3.*/
    pwmp->flexpwmp->SUB[1].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[1].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[1].VAL[1].R = pwmperiod / 2U;
	pwmp->flexpwmp->SUB[2].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[2].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[2].VAL[1].R = pwmperiod / 2U;
    pwmp->flexpwmp->SUB[3].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[3].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[3].VAL[1].R = pwmperiod / 2U;

    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      /* Setting active front of PWM channels of submodules 1-3.*/
	  pwmp->flexpwmp->SUB[1].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[1].VAL[4].R = ~(pwmperiod / 2U) + 1U;
	  pwmp->flexpwmp->SUB[2].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[2].VAL[4].R = ~(pwmperiod / 2U) + 1U;
	  pwmp->flexpwmp->SUB[3].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[3].VAL[4].R = ~(pwmperiod / 2U) + 1U;
      break;
    default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 0xEU;
    pwmp->flexpwmp->MCTRL.B.RUN |= 0xEU;
#endif
  }
#endif
#if (SPC5_PWM_USE_SMOD1 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD2 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 2U;

    /* Setting PWM period of submodule 1.*/
    pwmp->flexpwmp->SUB[1].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[1].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[1].VAL[1].R = pwmperiod / 2U;

    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:

      /* Setting active front of PWM channels of submodule 1.*/
      pwmp->flexpwmp->SUB[1].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[1].VAL[4].R = ~(pwmperiod / 2U) + 1U;
      break;
    default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 2U;
  }
#endif
#if (SPC5_PWM_USE_SMOD2 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD3 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 4U;

    /* Setting PWM period of submodule 2.*/
    pwmp->flexpwmp->SUB[2].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[2].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[2].VAL[1].R = pwmperiod / 2U;

    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      /* Setting active front of PWM channels of submodule 2.*/
      pwmp->flexpwmp->SUB[2].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[2].VAL[4].R = ~(pwmperiod / 2U) + 1U;
      break;
    default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 4U;
  }
#endif
#if (SPC5_PWM_USE_SMOD3 == TRUE && SPC5_PWM0_USE_SYNC_SMOD == FALSE)
  if (&PWMD4 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 8U;

    /* Setting PWM period of submodule 3.*/
    pwmp->flexpwmp->SUB[3].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[3].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[3].VAL[1].R = pwmperiod / 2U;

    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      /* Setting active front of PWM channels of submodule 3.*/
      pwmp->flexpwmp->SUB[3].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[3].VAL[4].R = ~(pwmperiod / 2U) + 1U;
      break;
    default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 8U;
  }
#endif
#if (SPC5_PWM_USE_SMOD4 == TRUE)
  if (&PWMD5 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 1U;

    /* Setting PWM period of submodule 0.*/
    pwmp->flexpwmp->SUB[0].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[0].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[0].VAL[1].R = pwmperiod / 2U;

    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      /* Setting active front of PWM channels of submodule 0.*/
      pwmp->flexpwmp->SUB[0].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[0].VAL[4].R = ~(pwmperiod / 2U) + 1U;
      break;
    default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 1U;
#if (SPC5_PWM1_USE_SYNC_SMOD == TRUE)
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 0xEU;

    /* Setting PWM period of submodules 1-3.*/
    pwmp->flexpwmp->SUB[1].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[1].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[1].VAL[1].R = pwmperiod / 2U;
	pwmp->flexpwmp->SUB[2].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[2].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[2].VAL[1].R = pwmperiod / 2U;
    pwmp->flexpwmp->SUB[3].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[3].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[3].VAL[1].R = pwmperiod / 2U;

    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      /* Setting active front of PWM channels of submodules 1-3.*/
	  pwmp->flexpwmp->SUB[1].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[1].VAL[4].R = ~(pwmperiod / 2U) + 1U;
	  pwmp->flexpwmp->SUB[2].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[2].VAL[4].R = ~(pwmperiod / 2U) + 1U;
	  pwmp->flexpwmp->SUB[3].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[3].VAL[4].R = ~(pwmperiod / 2U) + 1U;
      break;
    default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 0xEU;
#endif
  }
#endif
#if (SPC5_PWM_USE_SMOD5 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD6 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 2U;

    /* Setting PWM period of submodule 1.*/
    pwmp->flexpwmp->SUB[1].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[1].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[1].VAL[1].R = pwmperiod / 2U;

    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      /* Setting active front of PWM channels of submodule 1.*/
      pwmp->flexpwmp->SUB[1].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[1].VAL[4].R = ~(pwmperiod / 2U) + 1U;
      break;
    default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 2U;
  }
#endif
#if (SPC5_PWM_USE_SMOD6 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD7 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 4U;

    /* Setting PWM period of submodule 2.*/
    pwmp->flexpwmp->SUB[2].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[2].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[2].VAL[1].R = pwmperiod / 2U;

    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      /* Setting active front of PWM channels of submodule 2.*/
      pwmp->flexpwmp->SUB[2].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[2].VAL[4].R = ~(pwmperiod / 2U) + 1U;
      break;
    default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 4U;
  }
#endif
#if (SPC5_PWM_USE_SMOD7 == TRUE && SPC5_PWM1_USE_SYNC_SMOD == FALSE)
  if (&PWMD8 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 8U;

    /* Setting PWM period of submodule 3.*/
    pwmp->flexpwmp->SUB[3].INIT.R = ~(pwmperiod / 2U) + 1U;
    pwmp->flexpwmp->SUB[3].VAL[0].R = 0;
    pwmp->flexpwmp->SUB[3].VAL[1].R = pwmperiod / 2U;

    switch (pwmp->config->mode & PWM_ALIGN_MASK) {
    case PWM_ALIGN_EDGE:
      /* Setting active front of PWM channels of submodule 3.*/
      pwmp->flexpwmp->SUB[3].VAL[2].R = ~(pwmperiod / 2U) + 1U;
      pwmp->flexpwmp->SUB[3].VAL[4].R = ~(pwmperiod / 2U) + 1U;
      break;
    default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 8U;
  }
#endif

  irqIsrEnable();
}

#endif /* LLD_USE_PWM */

/** @} */
