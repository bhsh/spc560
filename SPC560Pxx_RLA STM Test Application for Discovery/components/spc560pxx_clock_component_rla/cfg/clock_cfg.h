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

#ifndef _CLOCK_CFG_H_
#define _CLOCK_CFG_H_

/*
 * Oscillator frequency.
 */
#define SPC5_XOSC_CLK                       12000000U

/*
 * Clock parameter settings.
 */
#define SPC5_NO_INIT                        FALSE
#define SPC5_ALLOW_OVERCLOCK                FALSE
#define SPC5_DISABLE_WATCHDOG               TRUE
#define SPC5_FMPLL0_IDF_VALUE               3U
#define SPC5_FMPLL0_NDIV_VALUE              64U
#define SPC5_FMPLL0_ODF                     SPC5_FMPLL_ODF_DIV4
#define SPC5_FMPLL0_OPTIONS                 (0U)
#define SPC5_FMPLL0_MR_INIT                 (0U)
#define SPC5_FMPLL1_IDF_VALUE               1U
#define SPC5_FMPLL1_NDIV_VALUE              40U
#define SPC5_FMPLL1_ODF                     SPC5_FMPLL_ODF_DIV4
#define SPC5_FMPLL1_OPTIONS                 (0U)
#define SPC5_FMPLL1_MR_INIT                 (0U)
#define SPC5_CLKOUT_SRC                     SPC5_CGM_OCDS_SELCTL_FMPLL0
#define SPC5_CLKOUT_DIV_VALUE               2U
#define SPC5_AUX0CLK_SRC                    SPC5_CGM_SS_FMPLL0
#define SPC5_MCONTROL_DIVIDER_VALUE         2U
#define SPC5_FMPLL1_CLK_DIVIDER_VALUE       2U
#define SPC5_AUX2CLK_SRC                    SPC5_CGM_SS_FMPLL0
#define SPC5_SP_CLK_DIVIDER_VALUE           2U
#define SPC5_AUX3CLK_SRC                    SPC5_CGM_SS_FMPLL0
#define SPC5_FR_CLK_DIVIDER_VALUE           2U
#define SPC5_ME_ME_BITS                     (0UL | SPC5_ME_ME_RESET | SPC5_ME_ME_SAFE | SPC5_ME_ME_DRUN | SPC5_ME_ME_RUN0 | SPC5_ME_ME_RUN1 | SPC5_ME_ME_RUN2 | SPC5_ME_ME_RUN3 | SPC5_ME_ME_HALT0 | SPC5_ME_ME_STOP0)
#define SPC5_ME_SAFE_MC_BITS                (0UL | SPC5_ME_MC_PDO | SPC5_ME_MC_MVRON | SPC5_ME_MC_DFLAON_NORMAL | SPC5_ME_MC_CFLAON_NORMAL | SPC5_ME_MC_IRCON | SPC5_ME_MC_SYSCLK_IRC)
#define SPC5_ME_DRUN_MC_BITS                (0UL | SPC5_ME_MC_MVRON | SPC5_ME_MC_DFLAON_NORMAL | SPC5_ME_MC_CFLAON_NORMAL | SPC5_ME_MC_PLL0ON | SPC5_ME_MC_XOSC0ON | SPC5_ME_MC_IRCON | SPC5_ME_MC_SYSCLK_FMPLL0)
#define SPC5_ME_RUN0_MC_BITS                (0UL | SPC5_ME_MC_MVRON | SPC5_ME_MC_DFLAON_NORMAL | SPC5_ME_MC_CFLAON_NORMAL | SPC5_ME_MC_PLL1ON | SPC5_ME_MC_PLL0ON | SPC5_ME_MC_XOSC0ON | SPC5_ME_MC_IRCON | SPC5_ME_MC_SYSCLK_FMPLL0)
#define SPC5_ME_RUN1_MC_BITS                (0UL | SPC5_ME_MC_MVRON | SPC5_ME_MC_DFLAON_NORMAL | SPC5_ME_MC_CFLAON_NORMAL | SPC5_ME_MC_IRCON | SPC5_ME_MC_SYSCLK_IRC)
#define SPC5_ME_RUN2_MC_BITS                (0UL | SPC5_ME_MC_MVRON | SPC5_ME_MC_DFLAON_NORMAL | SPC5_ME_MC_CFLAON_NORMAL | SPC5_ME_MC_IRCON | SPC5_ME_MC_SYSCLK_IRC)
#define SPC5_ME_RUN3_MC_BITS                (0UL | SPC5_ME_MC_MVRON | SPC5_ME_MC_DFLAON_NORMAL | SPC5_ME_MC_CFLAON_NORMAL | SPC5_ME_MC_IRCON | SPC5_ME_MC_SYSCLK_IRC)
#define SPC5_ME_HALT0_MC_BITS               (0UL | SPC5_ME_MC_MVRON | SPC5_ME_MC_DFLAON_LP | SPC5_ME_MC_CFLAON_LP | SPC5_ME_MC_IRCON | SPC5_ME_MC_SYSCLK_IRC)
#define SPC5_ME_STOP0_MC_BITS               (0UL | SPC5_ME_MC_MVRON | SPC5_ME_MC_DFLAON_PD | SPC5_ME_MC_CFLAON_PD | SPC5_ME_MC_IRCON | SPC5_ME_MC_SYSCLK_IRC)
#define SPC5_ME_RUN_PC3_BITS                (0UL)
#define SPC5_ME_RUN_PC4_BITS                (0UL)
#define SPC5_ME_RUN_PC5_BITS                (0UL)
#define SPC5_ME_RUN_PC6_BITS                (0UL)
#define SPC5_ME_RUN_PC7_BITS                (0UL)
#define SPC5_ME_LP_PC4_BITS                 (0UL)
#define SPC5_ME_LP_PC5_BITS                 (0UL)
#define SPC5_ME_LP_PC6_BITS                 (0UL)
#define SPC5_ME_LP_PC7_BITS                 (0UL)
#define SPC5_FINAL_RUNMODE                  SPC5_RUNMODE_RUN0
#define SPC5_CLOCK_FAILURE_HOOK()           irqSysHalt()

#endif /* _CLOCK_CFG_H_ */
