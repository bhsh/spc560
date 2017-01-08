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
 * @file    spc5_lld.h
 * @brief   SPC5xx subsystem low level driver header.
 *
 * @addtogroup SPC5_LLD
 * @{
 */

#ifndef _SPC5_LLD_H_
#define _SPC5_LLD_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "core.h"
#include "board.h"
#include "clock.h"
#include "irq.h"

#include "typedefs.h"
#include "xpc560p.h"
#include "spc560p_registry.h"

typedef uint32_t systime_t;

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    System timer frequency
 * @{
 */
/**
 * @brief   System timer tick frequency [Hz].
 */
#define SYSTEM_TIMER_FREQUENCY                   1000UL
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Seconds to system ticks.
 * @details Converts from seconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] sec       number of seconds
 * @return              The number of ticks.
 *
 * @api
 */
#define TIMER_S2ST(sec)                                                      \
  ((systime_t)((sec) * SYSTEM_TIMER_FREQUENCY))

/**
 * @brief   Milliseconds to system ticks.
 * @details Converts from milliseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] msec      number of milliseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define TIMER_MS2ST(msec)                                                    \
  ((systime_t)(((((uint32_t)(msec)) * ((uint32_t)SYSTEM_TIMER_FREQUENCY) -   \
                1UL) / 1000UL) + 1UL))

/**
 * @brief   Microseconds to system ticks.
 * @details Converts from microseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] usec      number of microseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define TIMER_US2ST(usec)                                                    \
  ((systime_t)(((((uint32_t)(usec)) * ((uint32_t)SYSTEM_TIMER_FREQUENCY) -   \
                1UL) / 1000000UL) + 1UL))

/**
 * @brief   System ticks to seconds.
 * @details Converts from system ticks number to seconds.
 * @note    The result is rounded downward.
 *
 * @param[in] tick      number of ticks
 * @return              The number of seconds.
 *
 * @api
 */				
#define TIMER_ST2S(tick)                                                     \
  ((uint32_t)(((systime_t)tick) / SYSTEM_TIMER_FREQUENCY))

/**
 * @brief   System ticks to milliseconds.
 * @details Converts from system ticks number to milliseconds.
 * @note    The result is rounded downward.
 *
 * @param[in] tick      number of ticks
 * @return              The number of milliseconds.
 *
 * @api
 */	
#define TIMER_ST2MS(tick)                                                    \
  ((uint32_t)((((systime_t)tick) * 1000UL) / SYSTEM_TIMER_FREQUENCY))

/**
 * @brief   System ticks to microseconds.
 * @details Converts from system ticks number to microseconds.
 * @note    The result is rounded downward.
 *
 * @param[in] tick      number of ticks
 * @return              The number of microseconds.
 *
 * @api
 */	
#define TIMER_ST2US(tick)                                                    \
  ((uint32_t)((((systime_t)tick) * 1000000UL) / SYSTEM_TIMER_FREQUENCY))
  
				
/**
 * @brief   Delays for the specified number of seconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] sec       time in seconds, must be different from zero
 *
 * @api
 */
#define sysWaitSeconds(sec) sysTimeWaitMilliseconds(TIMER_S2ST(sec))

/**
 * @brief   Delays for the specified number of milliseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] msec      time in milliseconds, must be different from zero
 *
 * @api
 */
#define sysWaitMilliseconds(msec) sysTimeWaitMilliseconds(TIMER_MS2ST(msec))

/**
 * @brief   Delays for the specified number of microseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] usec      time in microseconds, must be different from zero
 *
 * @api
 */
#define sysWaitMicroseconds(usec) sysTimeWaitMilliseconds(TIMER_US2ST(usec))


/**
 * @brief   Returns the system time in seconds.
 *
 * @api
 */
#define sysGetSeconds() TIMER_ST2S(sysTimeGetTimeMilliseconds())

/**
 * @brief   Returns the system time in milliseconds.
 *
 * @api
 */
#define sysGetMilliseconds() TIMER_ST2MS(sysTimeGetTimeMilliseconds())

/**
 * @brief   Returns the system time in microseconds.
 *
 * @api
 */
#define sysGetMicroseconds() TIMER_ST2US(sysTimeGetTimeMilliseconds())

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif
  void spc5LLDInit(void);
  void sysTimeWaitMilliseconds(systime_t msec);
  systime_t sysTimeGetTimeMilliseconds(void);
#ifdef __cplusplus
}
#endif

#endif /* _SPC5_LLD_H_ */

/** @} */
