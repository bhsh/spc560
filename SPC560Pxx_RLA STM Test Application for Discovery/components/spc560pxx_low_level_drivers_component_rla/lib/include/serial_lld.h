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
 * @file    serial_lld.h
 * @brief   SPC5xx SERIAL low level driver header.
 *
 * @addtogroup SERIAL
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_SERIAL == TRUE) || defined(__DOXYGEN__)

#include "spc5_linflex.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Serial driver allowable modes
 * @{
 */
#define SD_MODE_8BITS_PARITY_NONE           (SPC5_UARTCR_WL)
#define SD_MODE_8BITS_PARITY_EVEN           (SPC5_UARTCR_WL |               \
                                             SPC5_UARTCR_PCE)
#define SD_MODE_8BITS_PARITY_ODD            (SPC5_UARTCR_WL |               \
                                             SPC5_UARTCR_PCE |              \
                                             SPC5_UARTCR_OP)
/** @} */

/**
 * @brief  serialmsg_t  constant
 * @{
 */
#define SERIAL_MSG_OK        			(1UL << 1)	/**< @brief message OK.    		        */
#define SERIAL_MSG_WAIT				    (1UL << 2)  /**< @brief ob or ib is non empty. wait */
#define SERIAL_MSG_ERROR				(1UL << 3)	/**< @brief ob is non empty. wait       */
#define SERIAL_MSG_ERROR_LEN			(1UL << 4)	/**< @brief buffer len not allowed      */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   LINFlex-0 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-0 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX0) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX0            FALSE
#endif

/**
 * @brief   LINFlex-1 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-1 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX1) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX1            FALSE
#endif

/**
 * @brief   LINFlex-2 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-2 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX2) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX2            FALSE
#endif

/**
 * @brief   LINFlex-3 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-3 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX3) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX3            FALSE
#endif

/**
 * @brief   LINFlex-4 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-4 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX4) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX4            FALSE
#endif

/**
 * @brief   LINFlex-5 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-5 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX5) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX5            FALSE
#endif

/**
 * @brief   LINFlex-6 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-6 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX6) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX6            FALSE
#endif

/**
 * @brief   LINFlex-7 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-7 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX7) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX7            FALSE
#endif

/**
 * @brief   LINFlex-8 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-8 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX8) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX8            FALSE
#endif

/**
 * @brief   LINFlex-9 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-9 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX9) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX9            FALSE
#endif

/**
 * @brief   LINFlex-14 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-14 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX14) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX14           FALSE
#endif

/**
 * @brief   LINFlex-15 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-15 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX15) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX15           FALSE
#endif

/**
 * @brief   LINFlex-0 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX0_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX0_PRIORITY       INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-1 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX1_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX1_PRIORITY       INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-2 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX2_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX2_PRIORITY       INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-3 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX3_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX3_PRIORITY       INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-4 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX4_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX4_PRIORITY       INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-5 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX5_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX5_PRIORITY       INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-6 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX6_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX6_PRIORITY       INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-7 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX7_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX7_PRIORITY       INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-8 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX8_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX8_PRIORITY       INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-9 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX9_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX9_PRIORITY       INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-14 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX14_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX14_PRIORITY      INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-15 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX15_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX15_PRIORITY      INTC_PSR_ENABLE(INTC_PSR_CORE0, 8)
#endif

/**
 * @brief   LINFlex-0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. 
 */
#if !defined(SPC5_SERIAL_LINFLEX0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX0_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. 
 */
#if !defined(SPC5_SERIAL_LINFLEX0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX0_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-1 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. 
 */
#if !defined(SPC5_SERIAL_LINFLEX1_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX1_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-1 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX1_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX1_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-2 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX2_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX2_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-2 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX2_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX2_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-3 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. 
 */
#if !defined(SPC5_SERIAL_LINFLEX3_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX3_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-3 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX3_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX3_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-4 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX4_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX4_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-4 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX4_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX4_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-5 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX5_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX5_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-5 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX5_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX5_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-6 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX6_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX6_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-6 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX6_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX6_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-7 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX7_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX7_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-7 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX7_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX7_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-8 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX8_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX8_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-8 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX8_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX8_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-9 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX9_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX9_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-9 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX9_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX9_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-14 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX14_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX14_START_PCTL    (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-14 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX14_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX14_STOP_PCTL     (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-15 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX15_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX15_START_PCTL    (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-15 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SERIAL_LINFLEX15_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX15_STOP_PCTL     (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SPC5_SERIAL_USE_LINFLEX0 == TRUE) && (SPC5_HAS_LINFLEX0 == FALSE)
#error "LINFlex-0 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX1 == TRUE) && (SPC5_HAS_LINFLEX1 == FALSE)
#error "LINFlex-1 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX2 == TRUE) && (SPC5_HAS_LINFLEX2 == FALSE)
#error "LINFlex-2 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX3 == TRUE) && (SPC5_HAS_LINFLEX3 == FALSE)
#error "LINFlex-3 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX4 == TRUE) && (SPC5_HAS_LINFLEX4 == FALSE)
#error "LINFlex-4 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX5 == TRUE) && (SPC5_HAS_LINFLEX5 == FALSE)
#error "LINFlex-5 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX6 == TRUE) && (SPC5_HAS_LINFLEX6 == FALSE)
#error "LINFlex-6 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX7 == TRUE) && (SPC5_HAS_LINFLEX7 == FALSE)
#error "LINFlex-7 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX8 == TRUE) && (SPC5_HAS_LINFLEX8 == FALSE)
#error "LINFlex-8 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX9 == TRUE) && (SPC5_HAS_LINFLEX9 == FALSE)
#error "LINFlex-9 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX14 == TRUE) && (SPC5_HAS_LINFLEX14 == FALSE)
#error "LINFlex-14 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX15 == TRUE) && (SPC5_HAS_LINFLEX15 == FALSE)
#error "LINFlex-15 not present in the selected device"
#endif

#if (SPC5_SERIAL_USE_LINFLEX0 == FALSE) && (SPC5_SERIAL_USE_LINFLEX1 == FALSE) &&               \
    (SPC5_SERIAL_USE_LINFLEX2 == FALSE) && (SPC5_SERIAL_USE_LINFLEX3 == FALSE) &&               \
    (SPC5_SERIAL_USE_LINFLEX4 == FALSE) && (SPC5_SERIAL_USE_LINFLEX5 == FALSE) &&               \
    (SPC5_SERIAL_USE_LINFLEX6 == FALSE) && (SPC5_SERIAL_USE_LINFLEX7 == FALSE) &&               \
    (SPC5_SERIAL_USE_LINFLEX8 == FALSE) && (SPC5_SERIAL_USE_LINFLEX9 == FALSE) &&               \
    (SPC5_SERIAL_USE_LINFLEX14 == FALSE) && (SPC5_SERIAL_USE_LINFLEX15 == FALSE)
#error "SERIAL driver activated but no LINFlex peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Generic Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 * @note    This structure content is architecture dependent, each driver
 *          implementation defines its own version and the custom static
 *          initializers.
 */
typedef struct {
  /**
   * @brief Bit rate.
   */
  uint32_t speed;
  /**
   * @brief Mode flags.
   */
  uint8_t mode;
} SerialConfig;

/**
 * @brief   @p SerialDriver specific data.
 */
typedef struct {
  /**
   * @brief Input Buffer.
   */
  uint8_t ib[SERIAL_BUFFERS_SIZE];
  /**
   * @brief Output Buffer.
   */
  uint8_t ob[SERIAL_BUFFERS_SIZE];
  /**
   * @brief Pointer to Input Buffer.
   */
  uint16_t ib_pointer;
  /**
   * @brief  Pointer to Output Buffer.
   */
  uint16_t ob_pointer;
  /**
   * @brief Pointer latest byte to write in the output buffer.
   */
  uint16_t eofwrite_pointer;
  /**
   * @brief  ointer latest byte read in the output buffer..
   */
  uint16_t eofread_pointer;
  /**
   * @brief Number of bytes received.
   */
  uint16_t ib_len;
  /**
   * @brief Number of bytes to transmit
   */
  uint16_t ob_len;
  /**
   * @brief Pointer to the volatile LINFlex registers block
   */
  volatile struct spc5_linflex *linflexp;
} SerialDriver;

/**
 * @brief   @p type of return message for serial driver
 */
typedef uint32_t serialmsg_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

#if (SPC5_SERIAL_USE_LINFLEX0 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX0_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX0_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX0_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX1 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX1_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX1_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX1_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX2 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX2_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX2_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX2_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX3 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX3_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX3_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX3_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX4 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX4_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX4_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX4_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX5 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX5_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX5_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX5_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX6 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX6_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX6_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX6_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX7 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX7_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX7_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX7_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX8 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX8_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX8_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX8_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX9 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX9_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX9_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX9_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX14 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX14_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX14_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX14_ERR_HANDLER);
#endif
#if (SPC5_SERIAL_USE_LINFLEX15 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX15_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX15_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX15_ERR_HANDLER);
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (SPC5_SERIAL_USE_LINFLEX0 == TRUE) && !defined(__DOXYGEN__)
extern SerialDriver SD1;
#endif
#if (SPC5_SERIAL_USE_LINFLEX1 == TRUE) && !defined(__DOXYGEN__)
extern SerialDriver SD2;
#endif
#if (SPC5_SERIAL_USE_LINFLEX2 == TRUE) && !defined(__DOXYGEN__)
extern SerialDriver SD3;
#endif
#if (SPC5_SERIAL_USE_LINFLEX3 == TRUE) && !defined(__DOXYGEN__)
extern SerialDriver SD4;
#endif
#if (SPC5_SERIAL_USE_LINFLEX4 == TRUE) && !defined(__DOXYGEN__)
extern SerialDriver SD5;
#endif
#if (SPC5_SERIAL_USE_LINFLEX5 == TRUE) && !defined(__DOXYGEN__)
extern SerialDriver SD6;
#endif
#if (SPC5_SERIAL_USE_LINFLEX6 == TRUE) && !defined(__DOXYGEN__)
extern SerialDriver SD7;
#endif
#if (SPC5_SERIAL_USE_LINFLEX7 == TRUE) && !defined(__DOXYGEN__)
extern SerialDriver SD8;
#endif
#if (SPC5_SERIAL_USE_LINFLEX8 == TRUE) && !defined(__DOXYGEN__)
extern SerialDriver SD9;
#endif
#if (SPC5_SERIAL_USE_LINFLEX9 == TRUE)&& !defined(__DOXYGEN__)
extern SerialDriver SD10;
#endif
#if (SPC5_SERIAL_USE_LINFLEX14 == TRUE)&& !defined(__DOXYGEN__)
extern SerialDriver SD15;
#endif
#if (SPC5_SERIAL_USE_LINFLEX15 == TRUE)&& !defined(__DOXYGEN__)
extern SerialDriver SD16;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
  serialmsg_t sd_lld_write(SerialDriver *sdp, uint8_t *buffer, uint16_t len);
  uint16_t sd_lld_read(SerialDriver* sdp, uint8_t* buffer, uint16_t len);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_SERIAL */

#endif /* _SERIAL_LLD_H_ */

/** @} */
