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
 * @file    pal.h
 * @brief   SPC5xx SIUL low level driver header.
 *
 * @addtogroup PAL
 * @{
 */

#ifndef _PAL_H_
#define _PAL_H_

#include "platform.h"
#include "clock.h"

/**
 * @name    Logic level constants
 * @{
 */
/**
 * @brief   Logical low state.
 */
#define PAL_LOW                         0U

/**
 * @brief   Logical high state.
 */
#define PAL_HIGH                        1U
/** @} */

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

#undef PAL_MODE_RESET
#undef PAL_MODE_UNCONNECTED
#undef PAL_MODE_INPUT
#undef PAL_MODE_INPUT_PULLUP
#undef PAL_MODE_INPUT_PULLDOWN
#undef PAL_MODE_INPUT_ANALOG
#undef PAL_MODE_OUTPUT_PUSHPULL
#undef PAL_MODE_OUTPUT_OPENDRAIN

/**
 * @name    SIUL-specific PAL modes
 * @{
 */
#define PAL_SPC5_SMC                (1UL << 14)
#define PAL_SPC5_APC                (1UL << 13)
#define PAL_SPC5_PA_MASK            (7UL << 10)
#define PAL_SPC5_PA(n)              ((uint32_t)(n) << 10)
#define PAL_SPC5_OBE                (1UL << 9)
#define PAL_SPC5_IBE                (1UL << 8)
#define PAL_SPC5_ODE                (1UL << 5)
#define PAL_SPC5_SRC                (1UL << 2)
#define PAL_SPC5_WPE                (1UL << 1)
#define PAL_SPC5_WPS                (1UL << 0)
/** @} */

/**
 * @name    Pads mode constants
 * @{
 */
/**
 * @brief   After reset state.
 */
#define PAL_MODE_RESET                  0

/**
 * @brief   Safe state for <b>unconnected</b> pads.
 */
#define PAL_MODE_UNCONNECTED            (PAL_SPC5_WPE | PAL_SPC5_WPS)

/**
 * @brief   Regular input high-Z pad.
 */
#define PAL_MODE_INPUT                  (PAL_SPC5_IBE)

/**
 * @brief   Input pad with weak pull up resistor.
 */
#define PAL_MODE_INPUT_PULLUP           (PAL_SPC5_IBE | PAL_SPC5_WPE |      \
                                         PAL_SPC5_WPS)

/**
 * @brief   Input pad with weak pull down resistor.
 */
#define PAL_MODE_INPUT_PULLDOWN         (PAL_SPC5_IBE | PAL_SPC5_WPE)

/**
 * @brief   Analog input mode.
 */
#define PAL_MODE_INPUT_ANALOG           PAL_SPC5_APC

/**
 * @brief   Push-pull output pad.
 */
#define PAL_MODE_OUTPUT_PUSHPULL        (PAL_SPC5_IBE | PAL_SPC5_OBE)

/**
 * @brief   Open-drain output pad.
 */
#define PAL_MODE_OUTPUT_OPENDRAIN       (PAL_SPC5_IBE | PAL_SPC5_OBE |      \
                                         PAL_SPC5_ODE)

/**
 * @brief   Alternate "n" output pad.
 * @note    Both the IBE and OBE bits are specified in this mask, the OBE
 *          bit is not required for some PCRs but in that case it is
 *          ignored.
 */
#define PAL_MODE_OUTPUT_ALTERNATE(n)    (PAL_SPC5_IBE | PAL_SPC5_OBE |      \
                                         PAL_SPC5_PA(n))
/** @} */

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 16

/**
 * @brief   Whole port mask.
 * @brief   This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFFFF)

/**
 * @brief   Digital I/O port sized unsigned type.
 */
typedef uint16_t ioportmask_t;

/**
 * @brief   Digital I/O modes.
 */
typedef uint16_t iomode_t;

/**
 * @brief   Port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */
typedef uint32_t ioportid_t;

/**
 * @brief   SIUL register initializer type.
 */
typedef struct {
  int32_t                   pcr_index;
  uint8_t                   gpdo_value;
  iomode_t                  pcr_value;
} spc_siu_init_t;

/**
 * @brief   Generic I/O ports static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialized the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  iomode_t                  default_mode;
  const spc_siu_init_t      *inits;
  const uint8_t             *padsels;
} PALConfig;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief   I/O port A identifier.
 */
#define PORT_A          0U

/**
 * @brief   I/O port B identifier.
 */
#define PORT_B          1U

/**
 * @brief   I/O port C identifier.
 */
#define PORT_C          2U

/**
 * @brief   I/O port D identifier.
 */
#define PORT_D          3U

/**
 * @brief   I/O port E identifier.
 */
#define PORT_E          4U

/**
 * @brief   I/O port F identifier.
 */
#define PORT_F          5U

/**
 * @brief   I/O port G identifier.
 */
#define PORT_G          6U

/**
 * @brief   I/O port H identifier.
 */
#define PORT_H          7U

/**
 * @brief   I/O port I identifier.
 */
#define PORT_I          8U

/**
 * @brief   I/O port J identifier.
 */
#define PORT_J          9U

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   Port bit helper macro.
 * @note    Overrides the one in @p pal.h.
 *
 * @param[in] n         bit position within the port
 *
 * @return              The bit mask.
 */
#define PAL_PORT_BIT(n) ((ioportmask_t)(0x8000U >> (n)))

/**
 * @brief   Reads the physical I/O port states.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 *
 * @notapi
 */
#define pal_readport(port) (((volatile uint16_t *)SIU.PGPDI)[port])

/**
 * @brief   Reads the output latch.
 * @details The purpose of this function is to read back the latched output
 *          value.
 *
 * @param[in] port      port identifier
 * @return              The latched logical states.
 *
 * @notapi
 */
#define pal_readlatch(port) (((volatile uint16_t *)SIU.PGPDO)[port])

/**
 * @brief   Writes a bits mask on a I/O port.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @notapi
 */
#define pal_writeport(port, bits)                                       \
  (((volatile uint16_t *)SIU.PGPDO)[port] = (bits))

/**
 * @brief   Reads a logical state from an I/O pad.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @return              The logical state.
 * @retval PAL_LOW      low logical state.
 * @retval PAL_HIGH     high logical state.
 *
 * @notapi
 */
#define pal_readpad(port, pad)                                          \
  (SIU.GPDI[((port) * 16U) + (pad)].R)

/**
 * @brief   Writes a logical state on an output pad.
 * @note    This function is not meant to be invoked directly by the
 *          application  code.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] bit       logical value, the value must be @p PAL_LOW or
 *                      @p PAL_HIGH
 *
 * @notapi
 */
#define pal_writepad(port, pad, bit)                                    \
  (SIU.GPDO[((port) * 16U) + (pad)].R = (bit))

/**
 * @brief   Sets a pad logical state to @p PAL_HIGH.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_setpad(port, pad)                                           \
  (SIU.GPDO[((port) * 16U) + (pad)].R = 1)

/**
 * @brief   Clears a pad logical state to @p PAL_LOW.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_clearpad(port, pad)                                         \
  (SIU.GPDO[((port) * 16U) + (pad)].R = 0)

/**
 * @brief   Toggles a pad logical state.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_togglepad(port, pad)                                        \
  (SIU.GPDO[((port) * 16U) + (pad)].R = ~SIU.GPDO[((port) * 16U) + (pad)].R)


#ifdef __cplusplus
extern "C" {
#endif
  void pal_init(const PALConfig *config);
#ifdef __cplusplus
}
#endif

#endif /* _PAL_H_ */

/** @} */
