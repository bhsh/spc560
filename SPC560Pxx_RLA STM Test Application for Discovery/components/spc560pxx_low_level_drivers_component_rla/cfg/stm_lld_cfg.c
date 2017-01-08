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
 * @file    stm_lld_cfg.c
 * @brief   STM Driver configuration code.
 *
 * @addtogroup STM
 * @{
 */

#include "stm_lld_cfg.h"

#if (LLD_USE_STM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
const STM_CH_Config stm0_config[SPC5_STM0_CHANNELS] ={
  {TRUE,1000000,cb_stm0_ch0}
  ,
  {TRUE,2000000,cb_stm0_ch1}
  ,
  {TRUE,3000000,cb_stm0_ch2}
  ,
  {TRUE,4000000,cb_stm0_ch3}
};


/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

#endif /* LLD_USE_STM */

/** @} */
