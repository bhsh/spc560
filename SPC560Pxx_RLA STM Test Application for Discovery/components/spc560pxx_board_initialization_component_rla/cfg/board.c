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

#include "board.h"
#include "clock.h"

/* Initial setup of all defined pads, the list is terminated by a {-1, 0, 0}.*/
static const spc_siu_init_t spc_siu_init[] = {
  {(int32_t)PCR(PORT_A, Led_D11),    PAL_LOW,    (iomode_t)(PAL_MODE_OUTPUT_PUSHPULL)},
  {(int32_t)PCR(PORT_A, Led_D12),    PAL_LOW,    (iomode_t)(PAL_MODE_OUTPUT_PUSHPULL)},
  {(int32_t)PCR(PORT_G, Led_D13),    PAL_LOW,    (iomode_t)(PAL_MODE_OUTPUT_PUSHPULL)},
  {(int32_t)PCR(PORT_B, PIN_LIN0TXD), PAL_LOW,    (iomode_t)(PAL_MODE_OUTPUT_ALTERNATE(1))},
  {(int32_t)PCR(PORT_B, PIN_LIN0RXD), PAL_LOW,    (iomode_t)(PAL_MODE_INPUT)},
  {-1, 0, 0}
};

/* Initialization array for the PSMI registers.*/
static const uint8_t spc_padsels_init[SPC5_SIUL_NUM_PADSELS] = {
  0,   0,   0,   0,   0,   0,   0,   0,   
  0,   0,   0,   0,   0,   0,   0,   0,   
  0,   0,   0,   0,   0,   0,   0,   0,   
  0,   0,   0,   0,   0,   0,   0,   0,   
  0,   0,   0,   0,   
};

/**
 * @brief   PAL setup.
 */
static const PALConfig pal_default_config = {
  (iomode_t)(PAL_MODE_UNCONNECTED),
  spc_siu_init,
  spc_padsels_init
};

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  pal_init(&pal_default_config);
}
