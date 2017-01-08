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
 * @file    eirq.c
 * @brief   SPC5xx EIRQ code.
 *
 * @addtogroup EIRQ
 * @{
 */

#include "eirq.h"
#include "eirq_cfg.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/
void manage_eirq_interrupt(uint8_t eirqnum);

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/	
/**
 * @brief  manage external interrupt occurred and call related callback
 *
 * @param[in] eirqnum number of External Interrupt Occurred
 */
void manage_eirq_interrupt(uint8_t eirqnum){
	uint8_t i;
	i=0U;
	while (eirqconf[i].eirqNumber != -1) {
	  if (eirqnum == (uint8_t)eirqconf[i].eirqNumber){
		  /* call related callback */
	    if (eirqconf[i].callback != NULL){
	    	eirqconf[i].callback();
		}
	  }
	  i++;
	}
}
/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/
/**
 * @brief  EIRQ 0-7 interrupt handler.
 *
 * @isr
 */
#if (SPC5_SIUL_EIRQ_HAS_0_7_HANDLER == TRUE)
IRQ_HANDLER(SPC5_SIUL_EIRQ_0_7_HANDLER){
  uint8_t i;

  IRQ_PROLOGUE();

  irqIsrDisableFromISR();

  for (i=0U;i<8U;i++){
	/*check eirq occured */
    if ((SIU.ISR.R >> i) == 1U){
    	/*call management function*/
    	manage_eirq_interrupt(i);
    	/*reset interrupt flag*/
    	SIU.ISR.R |= (1UL<<i);
    }
  }	  

  irqIsrEnableFromISR();

  IRQ_EPILOGUE();
}
#endif

/**
 * @brief  EIRQ 8-15 interrupt handler.
 *
 * @isr
 */
#if (SPC5_SIUL_EIRQ_HAS_8_15_HANDLER == TRUE)
IRQ_HANDLER(SPC5_SIUL_EIRQ_8_15_HANDLER){
  uint8_t i;

  IRQ_PROLOGUE();

  irqIsrDisableFromISR();

  for (i=8U;i<16U;i++){
	/*check eirq occured */
    if ((SIU.ISR.R >> i) == 1U){
    	/*call management function*/
    	manage_eirq_interrupt(i);
    	/*reset interrupt flag*/
    	SIU.ISR.R |= (1UL<<i);
    }
  }	  

  irqIsrEnableFromISR();

  IRQ_EPILOGUE();
}
#endif

/**
 * @brief  EIRQ 16-23 interrupt handler.
 *
 * @isr
 */
#if (SPC5_SIUL_EIRQ_HAS_16_23_HANDLER == TRUE)
IRQ_HANDLER(SPC5_SIUL_EIRQ_16_23_HANDLER){
  uint8_t i;

  IRQ_PROLOGUE();

  irqIsrDisableFromISR();

  for (i=16U;i<24U;i++){
	/*check eirq occured */
    if ((SIU.ISR.R >> i) == 1U){
    	/*call management function*/
    	manage_eirq_interrupt(i);
    	/*reset interrupt flag*/
    	SIU.ISR.R |= (1UL<<i);
    }
  }	  

  irqIsrEnableFromISR();

  IRQ_EPILOGUE();
}
#endif

/**
 * @brief  EIRQ 24-31 interrupt handler.
 *
 * @isr
 */
#if (SPC5_SIUL_EIRQ_HAS_24_31_HANDLER == TRUE)
IRQ_HANDLER(SPC5_SIUL_EIRQ_24_31_HANDLER){
  uint8_t i;

  IRQ_PROLOGUE();

  irqIsrDisableFromISR();

  for (i=24U;i<32U;i++){
	/*check eirq occured */
    if ((SIU.ISR.R >> i) == 1U){
    	/*call management function*/
    	manage_eirq_interrupt(i);
    	/*reset interrupt flag*/
    	SIU.ISR.R |= (1UL<<i);
    }
  }	  

  irqIsrEnableFromISR();

  IRQ_EPILOGUE();
}
#endif


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Eirq initialization.
 *
 * @init
 */
void eirqInit(void) {
  uint8_t i;
  uint8_t eirqnum;

  /* Priority settings */
#if (SPC5_SIUL_EIRQ_HAS_0_7_HANDLER == TRUE)
  INTC_PSR(SPC5_SIUL_EIRQ_0_7_INT_NUMBER) = SPC5_EIRQ_0_7_IRQ_PRIORITY;
#endif
#if (SPC5_SIUL_EIRQ_HAS_8_15_HANDLER == TRUE)
  INTC_PSR(SPC5_SIUL_EIRQ_8_15_INT_NUMBER) = SPC5_EIRQ_8_15_IRQ_PRIORITY;
#endif
#if (SPC5_SIUL_EIRQ_HAS_16_23_HANDLER == TRUE)
  INTC_PSR(SPC5_SIUL_EIRQ_16_23_INT_NUMBER) = SPC5_EIRQ_16_23_IRQ_PRIORITY;
#endif
#if (SPC5_SIUL_EIRQ_HAS_24_31_HANDLER == TRUE)
  INTC_PSR(SPC5_SIUL_EIRQ_24_31_INT_NUMBER) = SPC5_EIRQ_24_31_IRQ_PRIORITY;
#endif
  /* Registers Configuration */
  i = 0;
  while (eirqconf[i].eirqNumber != -1) {
	eirqnum = (uint8_t)eirqconf[i].eirqNumber;
	/* set rising edge event flag*/
    if (eirqconf[i].risingEdge == TRUE){
      SIU.IREER.R |= (1UL << eirqnum);
    }
    /* set falling edge event flag*/
    if (eirqconf[i].fallingEdge == TRUE){
	  SIU.IFEER.R |= (1UL << eirqnum);
	}
	/* set antiglith filter */
    if (eirqconf[i].filterEnable == TRUE){
	  SIU.IFER.R |= (1UL << eirqnum);
	}
	/* enable eirq interrupt */
    SIU.IRER.R |= (1UL << eirqnum);
	i++;
  }
}


/** @} */
