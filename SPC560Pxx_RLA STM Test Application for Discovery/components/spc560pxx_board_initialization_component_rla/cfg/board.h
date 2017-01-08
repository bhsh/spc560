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

#ifndef _BOARD_H_
#define _BOARD_H_

#include "pal.h"

/*
 * Setup for a generic SPC560Pxx board.
 */

/*
 * Board identifiers.
 */
#define BOARD_SPC56P_DISCOVERY
#define BOARD_NAME                  "STMicroelectronics SPC560P Discovery"

/*
 * I/O definitions.
 */
#define Led_D11                     0U
#define Led_D12                     1U
#define Led_D13                     4U
#define PIN_LIN0TXD                 2U
#define PIN_LIN0RXD                 3U

/*
 * Support macros.
 */
#define PCR(port, pin)  (((port) * 16U) + (pin))

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
