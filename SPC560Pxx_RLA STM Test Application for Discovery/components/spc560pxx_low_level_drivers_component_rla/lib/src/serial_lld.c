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
 * @file    serial_lld.c
 * @brief   SPC5xx SERIAL low level driver code.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "serial_lld.h"

#if (LLD_USE_SERIAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   LIINFlex-0 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX0 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/**
 * @brief   LIINFlex-1 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX1 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/**
 * @brief   LIINFlex-2 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX2 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD3;
#endif

/**
 * @brief   LIINFlex-3 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX3 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD4;
#endif

/**
 * @brief   LIINFlex-4 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX4 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD5;
#endif

/**
 * @brief   LIINFlex-5 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX5 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD6;
#endif

/**
 * @brief   LIINFlex-6 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX6 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD7;
#endif

/**
 * @brief   LIINFlex-7 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX7 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD8;
#endif

/**
 * @brief   LIINFlex-8 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX8 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD9;
#endif

/**
 * @brief   LIINFlex-9 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX9 == TRUE)|| defined(__DOXYGEN__)
SerialDriver SD10;
#endif

/**
 * @brief   LIINFlex-14 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX14 == TRUE)|| defined(__DOXYGEN__)
SerialDriver SD15;
#endif

/**
 * @brief   LIINFlex-15 serial driver identifier.
 */
#if (SPC5_SERIAL_USE_LINFLEX15 == TRUE)|| defined(__DOXYGEN__)
SerialDriver SD16;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const SerialConfig default_config = {SERIAL_DEFAULT_BITRATE,
SD_MODE_8BITS_PARITY_NONE};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   LINFlex initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void spc5_linflex_init(SerialDriver *sdp, const SerialConfig *config) {
  uint32_t div;
  volatile struct spc5_linflex *linflexp = sdp->linflexp;

  /* Enters the configuration mode.*/
  linflexp->LINCR1.R = 1; /* INIT bit.                */

  /* Configures the LINFlex in UART mode with all the required
   parameters.*/
  linflexp->UARTCR.R = SPC5_UARTCR_UART; /* UART mode FIRST.         */
  linflexp->UARTCR.R = (uint16_t)(
      SPC5_UARTCR_UART | SPC5_UARTCR_RXEN | (uint16_t)config->mode);
  div = SPC5_LINFLEX0_CLK / config->speed;
  linflexp->LINFBRR.R = (uint16_t)(div & 15U); /* Fractional divider.      */
  linflexp->LINIBRR.R = (uint16_t)(div >> 4); /* Integer divider.         */
  linflexp->UARTSR.R = 0xFFFF; /* Clearing UARTSR register.*/
  linflexp->LINIER.R = (uint16_t)(SPC5_LINIER_DTIE | SPC5_LINIER_DRIE |
  SPC5_LINIER_BOIE | SPC5_LINIER_FEIE |
  SPC5_LINIER_SZIE); /* Interrupts enabled.      */

  /* Leaves the configuration mode.*/
  linflexp->LINCR1.R = 0;
}

/**
 * @brief   LINFlex de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] linflexp  pointer to a LINFlex I/O block
 */
static void spc5_linflex_deinit(volatile struct spc5_linflex *linflexp) {

  /* Enters the configuration mode.*/
  linflexp->LINCR1.R = 1; /* INIT bit.                */

  /* Resets the LINFlex registers.*/
  linflexp->LINFBRR.R = 0; /* Fractional divider.      */
  linflexp->LINIBRR.R = 0; /* Integer divider.         */
  linflexp->UARTSR.R = 0xFFFF; /* Clearing UARTSR register.*/
  linflexp->UARTCR.R = SPC5_UARTCR_UART;
  linflexp->LINIER.R = 0; /* Interrupts disabled.     */

  /* Leaves the configuration mode.*/
  linflexp->LINCR1.R = 0;
}

/**
 * @brief   Common RXI IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void spc5xx_serve_rxi_interrupt(SerialDriver *sdp) {
  uint16_t sr = sdp->linflexp->UARTSR.R;
  uint16_t currpointer;

  sdp->linflexp->UARTSR.R = (uint16_t)(SPC5_UARTSR_NF | SPC5_UARTSR_DRF |
  SPC5_UARTSR_PE0);
  irqIsrDisableFromISR();

  if ((sr & SPC5_UARTSR_DRF) == SPC5_UARTSR_DRF) {
    currpointer = (uint16_t)(sdp->ib_pointer + 1U);
    currpointer %= SERIAL_BUFFERS_SIZE;
    /*check read buffer full If full, discard received data */
    if (currpointer != sdp->eofread_pointer) {
      sdp->ib[sdp->ib_pointer++] = (uint8_t)sdp->linflexp->BDRM.B.DATA4;
      sdp->ib_pointer %= SERIAL_BUFFERS_SIZE;
    }
    sdp->linflexp->UARTSR.R = (uint16_t)SPC5_UARTSR_RMB;
  }

  irqIsrEnableFromISR();
}

/**
 * @brief   Common TXI IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void spc5xx_serve_txi_interrupt(SerialDriver *sdp) {

  sdp->linflexp->UARTSR.R = (uint16_t)SPC5_UARTSR_DTF;

  irqIsrDisableFromISR();

  sdp->ob_pointer %= SERIAL_BUFFERS_SIZE;

  if (sdp->ob_pointer == sdp->eofwrite_pointer) {
    sdp->linflexp->UARTCR.B.TXEN = 0;
  }
  else {
    sdp->linflexp->BDRL.B.DATA0 = sdp->ob[sdp->ob_pointer++];
  }

  irqIsrEnableFromISR();
}

/**
 * @brief   Common ERR IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void spc5xx_serve_err_interrupt(SerialDriver *sdp) {

  sdp->linflexp->UARTSR.R = (uint16_t)(SPC5_UARTSR_BOF | SPC5_UARTSR_FEF |
  SPC5_UARTSR_SZF);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (SPC5_SERIAL_USE_LINFLEX0 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX0_RXI_HANDLER)
#error "SPC5_LINFLEX0_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-0 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX0_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD1);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX0_TXI_HANDLER)
#error "SPC5_LINFLEX0_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-0 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX0_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD1);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX0_ERR_HANDLER)
#error "SPC5_LINFLEX0_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-0 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX0_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD1);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX1 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX1_RXI_HANDLER)
#error "SPC5_LINFLEX1_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-1 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX1_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD2);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX1_TXI_HANDLER)
#error "SPC5_LINFLEX1_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-1 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX1_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD2);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX1_ERR_HANDLER)
#error "SPC5_LINFLEX1_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-1 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX1_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD2);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX2 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX2_RXI_HANDLER)
#error "SPC5_LINFLEX2_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-2 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX2_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD3);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX2_TXI_HANDLER)
#error "SPC5_LINFLEX2_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-2 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX2_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD3);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX2_ERR_HANDLER)
#error "SPC5_LINFLEX2_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-2 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX2_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD3);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX3 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX3_RXI_HANDLER)
#error "SPC5_LINFLEX3_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-3 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX3_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD4);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX3_TXI_HANDLER)
#error "SPC5_LINFLEX3_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-3 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX3_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD4);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX3_ERR_HANDLER)
#error "SPC5_LINFLEX3_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-3 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX3_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD4);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX4 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX4_RXI_HANDLER)
#error "SPC5_LINFLEX4_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-4 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX4_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD5);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX4_TXI_HANDLER)
#error "SPC5_LINFLEX4_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-4 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX4_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD5);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX4_ERR_HANDLER)
#error "SPC5_LINFLEX4_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-4 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX4_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD5);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX5 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX5_RXI_HANDLER)
#error "SPC5_LINFLEX5_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-5 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX5_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD6);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX5_TXI_HANDLER)
#error "SPC5_LINFLEX5_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-5 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX5_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD6);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX5_ERR_HANDLER)
#error "SPC5_LINFLEX5_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-5 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX5_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD6);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX6 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX6_RXI_HANDLER)
#error "SPC5_LINFLEX6_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-6 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX6_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD7);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX6_TXI_HANDLER)
#error "SPC5_LINFLEX6_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-6 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX6_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD7);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX6_ERR_HANDLER)
#error "SPC5_LINFLEX6_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-6 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX6_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD7);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX7 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX7_RXI_HANDLER)
#error "SPC5_LINFLEX7_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-7 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX7_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD8);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX7_TXI_HANDLER)
#error "SPC5_LINFLEX7_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-7 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX7_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD8);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX7_ERR_HANDLER)
#error "SPC5_LINFLEX7_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-7 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX7_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD8);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX8 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX8_RXI_HANDLER)
#error "SPC5_LINFLEX8_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-8 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX8_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD9);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX8_TXI_HANDLER)
#error "SPC5_LINFLEX8_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-8 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX8_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD9);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX8_ERR_HANDLER)
#error "SPC5_LINFLEX8_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-8 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX8_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD9);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX9 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX9_RXI_HANDLER)
#error "SPC5_LINFLEX9_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-9 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX9_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD10);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX9_TXI_HANDLER)
#error "SPC5_LINFLEX9_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-9 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX9_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD10);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX9_ERR_HANDLER)
#error "SPC5_LINFLEX9_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-9 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX9_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD10);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX14 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX14_RXI_HANDLER)
#error "SPC5_LINFLEX14_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-14 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX14_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD15);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX14_TXI_HANDLER)
#error "SPC5_LINFLEX14_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-14 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX14_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD15);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX14_ERR_HANDLER)
#error "SPC5_LINFLEX14_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-14 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX14_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD15);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SERIAL_USE_LINFLEX15 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX15_RXI_HANDLER)
#error "SPC5_LINFLEX15_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-15 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX15_RXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD16);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX15_TXI_HANDLER)
#error "SPC5_LINFLEX15_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-15 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX15_TXI_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD16);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX15_ERR_HANDLER)
#error "SPC5_LINFLEX15_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-15 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER( SPC5_LINFLEX15_ERR_HANDLER) {

  IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD16);

  IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @init
 */
void sd_lld_init(void) {

#if (SPC5_SERIAL_USE_LINFLEX0 == TRUE)
  SD1.linflexp = &SPC5_LINFLEX0;
  INTC_PSR (SPC5_LINFLEX0_RXI_NUMBER) = SPC5_SERIAL_LINFLEX0_PRIORITY;
  INTC_PSR (SPC5_LINFLEX0_TXI_NUMBER) = SPC5_SERIAL_LINFLEX0_PRIORITY;
  INTC_PSR (SPC5_LINFLEX0_ERR_NUMBER) = SPC5_SERIAL_LINFLEX0_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX1 == TRUE)
  SD2.linflexp = &SPC5_LINFLEX1;
  INTC_PSR (SPC5_LINFLEX1_RXI_NUMBER) = SPC5_SERIAL_LINFLEX1_PRIORITY;
  INTC_PSR (SPC5_LINFLEX1_TXI_NUMBER) = SPC5_SERIAL_LINFLEX1_PRIORITY;
  INTC_PSR (SPC5_LINFLEX1_ERR_NUMBER) = SPC5_SERIAL_LINFLEX1_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX2 == TRUE)
  SD3.linflexp = &SPC5_LINFLEX2;
  INTC_PSR (SPC5_LINFLEX2_RXI_NUMBER) = SPC5_SERIAL_LINFLEX2_PRIORITY;
  INTC_PSR (SPC5_LINFLEX2_TXI_NUMBER) = SPC5_SERIAL_LINFLEX2_PRIORITY;
  INTC_PSR (SPC5_LINFLEX2_ERR_NUMBER) = SPC5_SERIAL_LINFLEX2_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX3 == TRUE)
  SD4.linflexp = &SPC5_LINFLEX3;
  INTC_PSR (SPC5_LINFLEX3_RXI_NUMBER) = SPC5_SERIAL_LINFLEX3_PRIORITY;
  INTC_PSR (SPC5_LINFLEX3_TXI_NUMBER) = SPC5_SERIAL_LINFLEX3_PRIORITY;
  INTC_PSR (SPC5_LINFLEX3_ERR_NUMBER) = SPC5_SERIAL_LINFLEX3_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX4 == TRUE)
  SD5.linflexp = &SPC5_LINFLEX4;
  INTC_PSR (SPC5_LINFLEX4_RXI_NUMBER) = SPC5_SERIAL_LINFLEX4_PRIORITY;
  INTC_PSR (SPC5_LINFLEX4_TXI_NUMBER) = SPC5_SERIAL_LINFLEX4_PRIORITY;
  INTC_PSR (SPC5_LINFLEX4_ERR_NUMBER) = SPC5_SERIAL_LINFLEX4_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX5 == TRUE)
  SD6.linflexp = &SPC5_LINFLEX5;
  INTC_PSR (SPC5_LINFLEX5_RXI_NUMBER) = SPC5_SERIAL_LINFLEX5_PRIORITY;
  INTC_PSR (SPC5_LINFLEX5_TXI_NUMBER) = SPC5_SERIAL_LINFLEX5_PRIORITY;
  INTC_PSR (SPC5_LINFLEX5_ERR_NUMBER) = SPC5_SERIAL_LINFLEX5_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX6 == TRUE)
  SD7.linflexp = &SPC5_LINFLEX6;
  INTC_PSR (SPC5_LINFLEX6_RXI_NUMBER) = SPC5_SERIAL_LINFLEX6_PRIORITY;
  INTC_PSR (SPC5_LINFLEX6_TXI_NUMBER) = SPC5_SERIAL_LINFLEX6_PRIORITY;
  INTC_PSR (SPC5_LINFLEX6_ERR_NUMBER) = SPC5_SERIAL_LINFLEX6_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX7 == TRUE)
  SD8.linflexp = &SPC5_LINFLEX7;
  INTC_PSR (SPC5_LINFLEX7_RXI_NUMBER) = SPC5_SERIAL_LINFLEX7_PRIORITY;
  INTC_PSR (SPC5_LINFLEX7_TXI_NUMBER) = SPC5_SERIAL_LINFLEX7_PRIORITY;
  INTC_PSR (SPC5_LINFLEX7_ERR_NUMBER) = SPC5_SERIAL_LINFLEX7_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX8 == TRUE)
  SD9.linflexp = &SPC5_LINFLEX8;
  INTC_PSR (SPC5_LINFLEX8_RXI_NUMBER) = SPC5_SERIAL_LINFLEX8_PRIORITY;
  INTC_PSR (SPC5_LINFLEX8_TXI_NUMBER) = SPC5_SERIAL_LINFLEX8_PRIORITY;
  INTC_PSR (SPC5_LINFLEX8_ERR_NUMBER) = SPC5_SERIAL_LINFLEX8_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX9 == TRUE)
  SD10.linflexp = &SPC5_LINFLEX9;
  INTC_PSR (SPC5_LINFLEX9_RXI_NUMBER) = SPC5_SERIAL_LINFLEX9_PRIORITY;
  INTC_PSR (SPC5_LINFLEX9_TXI_NUMBER) = SPC5_SERIAL_LINFLEX9_PRIORITY;
  INTC_PSR (SPC5_LINFLEX9_ERR_NUMBER) = SPC5_SERIAL_LINFLEX9_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX14 == TRUE)
  SD15.linflexp = &SPC5_LINFLEX14;
  INTC_PSR (SPC5_LINFLEX14_RXI_NUMBER) = SPC5_SERIAL_LINFLEX14_PRIORITY;
  INTC_PSR (SPC5_LINFLEX14_TXI_NUMBER) = SPC5_SERIAL_LINFLEX14_PRIORITY;
  INTC_PSR (SPC5_LINFLEX14_ERR_NUMBER) = SPC5_SERIAL_LINFLEX14_PRIORITY;
#endif

#if (SPC5_SERIAL_USE_LINFLEX15 == TRUE)
  SD16.linflexp = &SPC5_LINFLEX15;
  INTC_PSR (SPC5_LINFLEX15_RXI_NUMBER) = SPC5_SERIAL_LINFLEX15_PRIORITY;
  INTC_PSR (SPC5_LINFLEX15_TXI_NUMBER) = SPC5_SERIAL_LINFLEX15_PRIORITY;
  INTC_PSR (SPC5_LINFLEX15_ERR_NUMBER) = SPC5_SERIAL_LINFLEX15_PRIORITY;
#endif

}

/**
 * @brief   Low level serial driver configuration and start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @api
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  if (config == NULL) {
    config = &default_config;
  }
#if (SPC5_SERIAL_USE_LINFLEX0 == TRUE)
  if (&SD1 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX0_PCTL,
    SPC5_SERIAL_LINFLEX0_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX1 == TRUE)
  if (&SD2 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX1_PCTL,
    SPC5_SERIAL_LINFLEX1_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX2 == TRUE)
  if (&SD3 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX2_PCTL,
    SPC5_SERIAL_LINFLEX2_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX3 == TRUE)
  if (&SD4 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX3_PCTL,
    SPC5_SERIAL_LINFLEX3_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX4 == TRUE)
  if (&SD5 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX4_PCTL,
    SPC5_SERIAL_LINFLEX4_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX5 == TRUE)
  if (&SD6 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX5_PCTL,
    SPC5_SERIAL_LINFLEX5_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX6 == TRUE)
  if (&SD7 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX6_PCTL,
    SPC5_SERIAL_LINFLEX6_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX7 == TRUE)
  if (&SD8 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX7_PCTL,
    SPC5_SERIAL_LINFLEX7_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX8 == TRUE)
  if (&SD9 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX8_PCTL,
    SPC5_SERIAL_LINFLEX8_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX9 == TRUE)
  if (&SD10 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX9_PCTL,
    SPC5_SERIAL_LINFLEX9_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX14 == TRUE)
  if (&SD15 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX14_PCTL,
    SPC5_SERIAL_LINFLEX14_START_PCTL);
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX15 == TRUE)
  if (&SD16 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX15_PCTL,
    SPC5_SERIAL_LINFLEX15_START_PCTL);
  }
#endif

  spc5_linflex_init(sdp, config);
  sdp->ib_pointer = 0;
  sdp->ob_pointer = 0;
  sdp->eofwrite_pointer = 0;
  sdp->eofread_pointer = 0;
}

/**
 * @brief   Low level serial driver stop.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @api
 */
void sd_lld_stop(SerialDriver *sdp) {

  spc5_linflex_deinit(sdp->linflexp);

#if (SPC5_SERIAL_USE_LINFLEX0 == TRUE)
  if (&SD1 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX0_PCTL,
    SPC5_SERIAL_LINFLEX0_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX1 == TRUE)
  if (&SD2 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX1_PCTL,
    SPC5_SERIAL_LINFLEX1_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX2 == TRUE)
  if (&SD3 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX2_PCTL,
    SPC5_SERIAL_LINFLEX2_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX3 == TRUE)
  if (&SD4 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX3_PCTL,
    SPC5_SERIAL_LINFLEX3_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX4 == TRUE)
  if (&SD5 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX4_PCTL,
    SPC5_SERIAL_LINFLEX4_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX5 == TRUE)
  if (&SD6 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX5_PCTL,
    SPC5_SERIAL_LINFLEX5_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX6 == TRUE)
  if (&SD7 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX6_PCTL,
    SPC5_SERIAL_LINFLEX6_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX7 == TRUE)
  if (&SD8 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX7_PCTL,
    SPC5_SERIAL_LINFLEX7_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX8 == TRUE)
  if (&SD9 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX8_PCTL,
    SPC5_SERIAL_LINFLEX8_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX9 == TRUE)
  if (&SD10 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX9_PCTL,
    SPC5_SERIAL_LINFLEX9_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX14 == TRUE)
  if (&SD15 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX14_PCTL,
    SPC5_SERIAL_LINFLEX14_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_SERIAL_USE_LINFLEX15 == TRUE)
  if (&SD16 == sdp) {
    SPCSetPeripheralClockMode(SPC5_LINFLEX15_PCTL,
    SPC5_SERIAL_LINFLEX15_STOP_PCTL);
    return;
  }
#endif

}

/**
 * @brief   Low level serial driver write
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] buffer	data buffer to be transmitted
 * @param[in] len    	number of bytes to be transmitted
 *
 * @return              operation result
 *
 * @api
 */
#ifdef __ghs__
#pragma ghs ZO
#endif
serialmsg_t sd_lld_write(SerialDriver *sdp, uint8_t *buffer, uint16_t len) {
  uint16_t i;
  uint16_t currpointer;

  /*check if len is correct */
  if (len > SERIAL_BUFFERS_SIZE) {
    return SERIAL_MSG_ERROR_LEN;
  }

  irqIsrDisable();

  /* fill transmission buffer starting for the first useful position */
  for (i = 0; i < len; i++) {
    /*if buffer is overlapped, wait for some bytes to be sent*/
    currpointer = sdp->eofwrite_pointer + 1U;
    while (currpointer == sdp->ob_pointer) {
      irqIsrEnable();
      irqIsrDisable();
    }
    /*fill the output buffer*/
    sdp->ob[sdp->eofwrite_pointer] = buffer[i];
    sdp->eofwrite_pointer++;
    sdp->eofwrite_pointer %= SERIAL_BUFFERS_SIZE;

  }

  /* if transmission not started send the first byte */
  if (sdp->linflexp->UARTCR.B.TXEN == 0U) {
    sdp->linflexp->UARTCR.B.TXEN = 1;
    sdp->linflexp->BDRL.B.DATA0 = sdp->ob[sdp->ob_pointer++];
  }

  irqIsrEnable();

  return SERIAL_MSG_OK;
}
#ifdef __ghs__
#pragma ghs O
#endif
/**
 * @brief   Low level serial driver read
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[out] buffer	data buffer receiver
 * @param[out] len    	number of bytes received
 *
 * @return              The number of bytes effectively read.
 *
 * @api
 */
uint16_t sd_lld_read(SerialDriver* sdp, uint8_t* buffer, uint16_t len) {
  uint16_t i = 0;

  /*check if len is correct. Otherwise return 0 bytes read */
  if (len > SERIAL_BUFFERS_SIZE) {
    return 0;
  }

  while ((sdp->eofread_pointer != sdp->ib_pointer) && (i < len)) {
    buffer[i] = sdp->ib[sdp->eofread_pointer++];
    sdp->eofread_pointer %= SERIAL_BUFFERS_SIZE;
    i++;
  }

  return i;
}

#endif /* LLD_USE_SERIAL */

/** @} */
