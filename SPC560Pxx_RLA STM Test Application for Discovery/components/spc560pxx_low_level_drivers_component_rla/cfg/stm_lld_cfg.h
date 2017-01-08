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
 * @file    stm_lld_cfg.h
 * @brief   STM Driver configuration macros and structures.
 *
 * @addtogroup STM
 * @{
 */

#ifndef _STM_LLD_CFG_H_
#define _STM_LLD_CFG_H_

#include "spc5_lld.h"
#include "stm_lld.h"

#if (LLD_USE_STM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data strustmres and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* List of the STMConfig strucutres defined in stm_lld_cfg.c.*/
extern const STM_CH_Config stm0_config[SPC5_STM0_CHANNELS];

#ifdef __cplusplus
extern "C" {
#endif
/*Callback Prototypes*/
  void cb_stm0_ch0(void);
  void cb_stm0_ch1(void);
  void cb_stm0_ch2(void);
  void cb_stm0_ch3(void);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_STM */

#endif /* _STM_LLD_CFG_H_ */

/** @} */
