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
 * @file    pal.c
 * @brief   SPC5xx SIUL low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "pal.h"

#if defined(SPC5_SIUL_SYSTEM_PINS)
static const unsigned system_pins[] = {SPC5_SIUL_SYSTEM_PINS};
#endif

/**
 * @brief   SPC5xx I/O ports configuration.
 *
 * @param[in] config    the SPC5xx ports configuration
 *
 * @notapi
 */
void pal_init(const PALConfig *config) {
  uint16_t i;

#if defined(SPC5_SIUL_PCTL)
  /* SIUL clock gating if present.*/
  SPCSetPeripheralClockMode(SPC5_SIUL_PCTL,
                            SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2));
#endif

  /* Initialize PCR registers for undefined pads.*/
  for (i = 0; i < SPC5_SIUL_NUM_PCRS; i++) {
#if defined(SPC5_SIUL_SYSTEM_PINS)
    /* Handling the case where some SIU pins are not meant to be reprogrammed,
       for example JTAG pins.*/
    uint16_t j;
    uint16_t pin_check = 0U;
    for (j = 0; j < (sizeof(system_pins)/sizeof(system_pins[0])); j++) {
      if (i == system_pins[j]){
        pin_check = 1U;
        break;
      }
    }
    if (pin_check == 0U) {
      SIU.PCR[i].R = config->default_mode;
    }
#else
    SIU.PCR[i].R = config->default_mode;
#endif
  }

  /* Initialize PADSEL registers.*/
  for (i = 0; i < SPC5_SIUL_NUM_PADSELS; i++){
    SIU.PSMI[i].R = config->padsels[i];
  }

  /* Initialize PCR registers for defined pads.*/
  i = 0;
  while (config->inits[i].pcr_index != -1) {
    SIU.GPDO[config->inits[i].pcr_index].R = config->inits[i].gpdo_value;
    SIU.PCR[config->inits[i].pcr_index].R  = config->inits[i].pcr_value;
    i++;
  }
}

/** @} */
