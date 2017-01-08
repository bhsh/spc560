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
 * @file    pwm_lld.h
 * @brief   SPC5xx PWM low level driver header.
 *
 * @addtogroup PWM
 * @{
 */

#ifndef _PWM_LLD_H_
#define _PWM_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_PWM == TRUE) || defined(__DOXYGEN__)

#include "spc5_flexpwm.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    PWM output mode macros
 * @{
 */
/**
 * @brief   Standard output modes mask.
 */
#define PWM_OUTPUT_MASK                     0x0FU

/**
 * @brief   Output not driven, callback only.
 */
#define PWM_OUTPUT_DISABLED                 0x00U

/**
 * @brief   Positive PWM logic, active is logic level one.
 */
#define PWM_OUTPUT_ACTIVE_HIGH              0x01U

/**
 * @brief   Inverse PWM logic, active is logic level zero.
 */
#define PWM_OUTPUT_ACTIVE_LOW               0x02U
/** @} */

/**
 * @name    STS register bits definitions
 * @{
 */
#define SPC5_FLEXPWM_STS_CMPF0              (1UL << 0)
#define SPC5_FLEXPWM_STS_CMPF1              (1UL << 1)
#define SPC5_FLEXPWM_STS_CMPF2              (1UL << 2)
#define SPC5_FLEXPWM_STS_CMPF3              (1UL << 3)
#define SPC5_FLEXPWM_STS_CMPF4              (1UL << 4)
#define SPC5_FLEXPWM_STS_CMPF5              (1UL << 5)
#define SPC5_FLEXPWM_STS_CFX0               (1UL << 6)
#define SPC5_FLEXPWM_STS_CFX1               (1UL << 7)
#define SPC5_FLEXPWM_STS_RF                 (1UL << 12)
#define SPC5_FLEXPWM_STS_REF                (1UL << 13)
#define SPC5_FLEXPWM_STS_RUF                (1UL << 14)
/** @} */

/**
 * @name    PSC values definition
 * @{
 */
#define SPC5_FLEXPWM_PSC_1                  0U
#define SPC5_FLEXPWM_PSC_2                  1U
#define SPC5_FLEXPWM_PSC_4                  2U
#define SPC5_FLEXPWM_PSC_8                  3U
#define SPC5_FLEXPWM_PSC_16                 4U
#define SPC5_FLEXPWM_PSC_32                 5U
#define SPC5_FLEXPWM_PSC_64                 6U
#define SPC5_FLEXPWM_PSC_128                7U
/** @} */

/**
 * @brief   Max number of PWM channels per PWM driver.
 */
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE) && (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
#define PWM_CHANNELS                        2
#else
#define PWM_CHANNELS                        8
#endif

/**
 * @brief   Complementary output modes mask.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_COMPLEMENT_OUT_MASK             0xF0U

/**
 * @brief   Complementary output not driven.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_COMPLEMENT_OUT_DISABLED         0x00U

/**
 * @brief   Complementary output, active is logic level one.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_COMPLEMENT_OUT_ACTIVE_HIGH      0x10U

/**
 * @brief   Complementary output, active is logic level zero.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_COMPLEMENT_OUT_ACTIVE_LOW       0x20U

/**
 * @brief   Alignment mode mask.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_ALIGN_MASK                      0x01U

/**
 * @brief   Edge-Aligned PWM functional mode.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_ALIGN_EDGE                      0x00U

/**
 * @brief   Center-Aligned PWM functional mode.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_ALIGN_CENTER                    0x01U

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   PWMD1 driver enable switch.
 * @details If set to @p TRUE the support for PWMD1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_SMOD0) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD0                  FALSE
#endif

/**
 * @brief   PWMD2 driver enable switch.
 * @details If set to @p TRUE the support for PWMD2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_SMOD1) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD1                  FALSE
#endif

/**
 * @brief   PWMD3 driver enable switch.
 * @details If set to @p TRUE the support for PWMD3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_SMOD2) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD2                  FALSE
#endif

/**
 * @brief   PWMD4 driver enable switch.
 * @details If set to @p TRUE the support for PWMD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_SMOD3) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD3                  FALSE
#endif

/**
 * @brief   FlexPWM0 synchronization enable switch.
 * @details If set to @p TRUE the FlexPWM0 synchronization is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM0_USE_SYNC_SMOD) || defined(__DOXYGEN__)
#define SPC5_PWM0_USE_SYNC_SMOD              FALSE
#endif

/**
 * @brief   PWMD1 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD0_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD0_PRIORITY             7
#endif

/**
 * @brief   PWMD2 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD1_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD1_PRIORITY             7
#endif

/**
 * @brief   PWMD3 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD2_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD2_PRIORITY             7
#endif

/**
 * @brief   PWMD4 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD3_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD3_PRIORITY             7
#endif

/**
 * @brief   FlexPWM-0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_PWM_FLEXPWM0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_PWM_FLEXPWM0_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   FlexPWM-0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_PWM_FLEXPWM0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_PWM_FLEXPWM0_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   PWMD5 driver enable switch.
 * @details If set to @p TRUE the support for PWMD5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_SMOD4) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD4                  FALSE
#endif

/**
 * @brief   PWMD6 driver enable switch.
 * @details If set to @p TRUE the support for PWMD6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_SMOD5) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD5                  FALSE
#endif

/**
 * @brief   PWMD7 driver enable switch.
 * @details If set to @p TRUE the support for PWMD7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_SMOD6) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD6                  FALSE
#endif

/**
 * @brief   PWMD8 driver enable switch.
 * @details If set to @p TRUE the support for PWMD8 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_SMOD7) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD7                  FALSE
#endif

/**
 * @brief   FlexPWM1 synchronization enable switch.
 * @details If set to @p TRUE the FlexPWM1 synchronization is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM1_USE_SYNC_SMOD) || defined(__DOXYGEN__)
#define SPC5_PWM1_USE_SYNC_SMOD              FALSE
#endif

/**
 * @brief   PWMD5 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD4_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD4_PRIORITY             7
#endif

/**
 * @brief   PWMD6 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD5_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD5_PRIORITY             7
#endif

/**
 * @brief   PWMD7 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD6_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD6_PRIORITY             7
#endif

/**
 * @brief   PWMD8 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD7_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD7_PRIORITY             7
#endif

/**
 * @brief   FlexPWM-1 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_PWM_FLEXPWM1_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_PWM_FLEXPWM1_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   FlexPWM-1 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_PWM_FLEXPWM1_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_PWM_FLEXPWM1_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/*===========================================================================*/
/* Configuration checks.                                                     */
/*===========================================================================*/

#if (SPC5_PWM_USE_SMOD0 == TRUE || SPC5_PWM_USE_SMOD1 == TRUE || \
     SPC5_PWM_USE_SMOD2 == TRUE || SPC5_PWM_USE_SMOD3 == TRUE)
#define SPC5_PWM_USE_FLEXPWM0               TRUE
#else
#define SPC5_PWM_USE_FLEXPWM0               FALSE
#endif

#if (SPC5_PWM_USE_SMOD4 == TRUE || SPC5_PWM_USE_SMOD5 == TRUE || \
     SPC5_PWM_USE_SMOD6 == TRUE || SPC5_PWM_USE_SMOD7 == TRUE)
#define SPC5_PWM_USE_FLEXPWM1               TRUE
#else
#define SPC5_PWM_USE_FLEXPWM1               FALSE
#endif

#if (SPC5_HAS_FLEXPWM0 == FALSE && SPC5_PWM_USE_FLEXPWM0 == TRUE)
#error "FlexPWM0 not present in the selected device"
#endif

#if (SPC5_HAS_FLEXPWM1 == FALSE && SPC5_PWM_USE_FLEXPWM1 == TRUE)
#error "FlexPWM1 not present in the selected device"
#endif

#if (SPC5_PWM_USE_FLEXPWM0 == FALSE && SPC5_PWM_USE_FLEXPWM1 == FALSE)
#error "PWM driver activated but no PWM peripheral assigned"
#endif

#if (SPC5_PWM0_USE_SYNC_SMOD == TRUE && SPC5_PWM_USE_SMOD0 == FALSE)
#error "FlexPWM0 sm0 must be activated when FlexPWM0 submodule synchronization is activated"
#endif

#if (SPC5_PWM1_USE_SYNC_SMOD == TRUE && SPC5_PWM_USE_SMOD4 == FALSE)
#error "FlexPWM1 sm0 must be activated when FlexPWM1 submodule synchronization is activated"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing a PWM driver.
 */
typedef struct PWMDriver PWMDriver;

/**
 * @brief   PWM notification callback type.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 */
typedef void (*pwmcallback_t)(PWMDriver *pwmp);

/**
 * @brief   PWM mode type.
 */
typedef uint32_t pwmmode_t;

/**
 * @brief   PWM channel type.
 */
typedef uint8_t pwmchannel_t;

/**
 * @brief   PWM counter type.
 */
typedef uint16_t pwmcnt_t;

/**
 * @brief   PWM driver channel configuration structure.
 */
typedef struct {
  /**
   * @brief Channel active logic level.
   */
  pwmmode_t                 mode;
  /**
   * @brief Channel callback pointer.
   * @note  This callback is invoked on the channel compare event. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             callback;
  /* End of the mandatory fields.*/
} PWMChannelConfig;

/**
 * @brief   PWM driver configuration structure.
 */
typedef struct {
  /**
   * @brief Timer clock in Hz.
   */
  uint32_t                  frequency;
  /**
   * @brief PWM period in ticks.
   */
  pwmcnt_t                  period;
  /**
   * @brief Periodic callback pointer.
   * @note  This callback is invoked on PWM counter reset. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             callback;
  /**
   * @brief Channels configurations.
   */
  PWMChannelConfig          channels[PWM_CHANNELS];
  /* End of the mandatory fields.*/
  /**
   * @brief PWM functional mode.
   */
  pwmmode_t                 mode;
} PWMConfig;

/**
 * @brief   Structure representing a PWM driver.
 */
struct PWMDriver {
  /**
   * @brief Current driver configuration data.
   */
  const PWMConfig           *config;
  /**
   * @brief Current PWM period in ticks.
   */
  pwmcnt_t                  period;
#if defined(PWM_DRIVER_EXT_FIELDS)
  PWM_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the volatile FlexPWM registers block.
   */
  volatile struct spc5_flexpwm *flexpwmp;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    PWM duty cycle conversion
 * @{
 */
/**
 * @brief   Converts from fraction to pulse width.
 * @note    Be careful with rounding errors, this is integer math not magic.
 *          You can specify tenths of thousandth but make sure you have the
 *          proper hardware resolution by carefully choosing the clock source
 *          and prescaler settings, see @p PWM_COMPUTE_PSC.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] denominator denominator of the fraction
 * @param[in] numerator numerator of the fraction
 * @return              The pulse width to be passed to @p pwm_lld_enable_channel().
 *
 * @api
 */
#define PWM_FRACTION_TO_WIDTH(pwmp, denominator, numerator)                 \
  ((pwmcnt_t)((((pwmcnt_t)(pwmp)->period) *                                 \
               (pwmcnt_t)(numerator)) / (pwmcnt_t)(denominator)))

/**
 * @brief   Converts from degrees to pulse width.
 * @note    Be careful with rounding errors, this is integer math not magic.
 *          You can specify hundredths of degrees but make sure you have the
 *          proper hardware resolution by carefully choosing the clock source
 *          and prescaler settings, see @p PWM_COMPUTE_PSC.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] degrees   degrees as an integer between 0 and 36000
 * @return              The pulse width to be passed to @p pwm_lld_enable_channel().
 *
 * @api
 */
#define PWM_DEGREES_TO_WIDTH(pwmp, degrees)                                 \
  PWM_FRACTION_TO_WIDTH(pwmp, 36000, degrees)

/**
 * @brief   Converts from percentage to pulse width.
 * @note    Be careful with rounding errors, this is integer math not magic.
 *          You can specify tenths of thousandth but make sure you have the
 *          proper hardware resolution by carefully choosing the clock source
 *          and prescaler settings, see @p PWM_COMPUTE_PSC.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] percentage percentage as an integer between 0 and 10000
 * @return              The pulse width to be passed to @p pwm_lld_enable_channel().
 *
 * @api
 */
#define PWM_PERCENTAGE_TO_WIDTH(pwmp, percentage)                           \
  PWM_FRACTION_TO_WIDTH(pwmp, 10000, percentage)
/** @} */

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

#if (SPC5_PWM_USE_SMOD0 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_FLEXPWM0_RF0_HANDLER) ;
IRQ_HANDLER(SPC5_FLEXPWM0_COF0_HANDLER);
#endif

#if (SPC5_PWM_USE_SMOD1 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
IRQ_HANDLER(SPC5_FLEXPWM0_RF1_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXPWM0_COF1_HANDLER);
#endif

#if (SPC5_PWM_USE_SMOD2 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
IRQ_HANDLER(SPC5_FLEXPWM0_RF2_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXPWM0_COF2_HANDLER);
#endif

#if (SPC5_PWM_USE_SMOD3 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM0_USE_SYNC_SMOD == FALSE)
IRQ_HANDLER(SPC5_FLEXPWM0_RF3_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXPWM0_COF3_HANDLER);
#endif

#if (SPC5_PWM_USE_SMOD4 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_FLEXPWM1_RF0_HANDLER);
IRQ_HANDLER(SPC5_FLEXPWM1_COF0_HANDLER);
#endif

#if (SPC5_PWM_USE_SMOD5 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
IRQ_HANDLER(SPC5_FLEXPWM1_RF1_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXPWM1_COF1_HANDLER);
#endif

#if (SPC5_PWM_USE_SMOD6 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
IRQ_HANDLER(SPC5_FLEXPWM1_RF2_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXPWM1_COF2_HANDLER);
#endif

#if (SPC5_PWM_USE_SMOD7 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_PWM1_USE_SYNC_SMOD == FALSE)
IRQ_HANDLER(SPC5_FLEXPWM1_RF3_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXPWM1_COF3_HANDLER);
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (SPC5_PWM_USE_SMOD0 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD1;
#endif

#if (SPC5_PWM_USE_SMOD1 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD2;
#endif

#if (SPC5_PWM_USE_SMOD2 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD3;
#endif

#if (SPC5_PWM_USE_SMOD3 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD4;
#endif

#if (SPC5_PWM_USE_SMOD4 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD5;
#endif

#if (SPC5_PWM_USE_SMOD5 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD6;
#endif

#if (SPC5_PWM_USE_SMOD6 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD7;
#endif

#if (SPC5_PWM_USE_SMOD7 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD8;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void pwm_lld_init(void);
  void pwm_lld_start(PWMDriver *pwmp, const PWMConfig *config);
  void pwm_lld_stop(PWMDriver *pwmp);
  void pwm_lld_enable_channel(PWMDriver *pwmp,
                              pwmchannel_t channel,
                              pwmcnt_t width);
  void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel);
  void pwm_lld_change_period(PWMDriver *pwmp, pwmcnt_t period);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_PWM */

#endif /* _PWM_LLD_H_ */

/** @} */
