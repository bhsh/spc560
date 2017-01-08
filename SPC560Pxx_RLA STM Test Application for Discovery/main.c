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

/* Inclusion of the main header files of all the imported components in the
   order specified in the application wizard. The file is generated
   automatically.*/
#include "components.h"
#include "stm_lld_cfg.h"

/*stm0 channel 0 callback*/
void cb_stm0_ch0 (void){
  uint8_t message[]= "STM0 Channel 0 expired!\r\n";
  while (sd_lld_write(&SD1,message,(uint16_t)(sizeof(message)/sizeof(message[0]))) == SERIAL_MSG_WAIT){}
}

/*stm0 channel 1 callback*/
void cb_stm0_ch1 (void){
  uint8_t message[]= "STM0 Channel 1 expired!\r\n";
  while (sd_lld_write(&SD1,message,(uint16_t)(sizeof(message)/sizeof(message[0]))) == SERIAL_MSG_WAIT){}
}

/*stm0 channel 2 callback*/
void cb_stm0_ch2 (void){
  uint8_t message[]= "STM0 Channel 2 expired!\r\n";
  while (sd_lld_write(&SD1,message,(uint16_t)(sizeof(message)/sizeof(message[0]))) == SERIAL_MSG_WAIT){}
}

/*stm0 channel 3 callback*/
void cb_stm0_ch3 (void){
  uint8_t message[]= "STM0 Channel 3 expired!\r\n";
  while (sd_lld_write(&SD1,message,(uint16_t)(sizeof(message)/sizeof(message[0]))) == SERIAL_MSG_WAIT){}
}

/*
 * Application entry point.
 */
int main(void) {

  /* Initialization of all the imported components in the order specified in
     the application wizard. The function is generated automatically.*/
  componentsInit();

  /* Enable Interrupts */
  irqIsrEnable();

  /* Activates STM0 Driver */
  stm_lld_start(&STMD1,stm0_config);

  /* Enable channels and start global counter */
  stm_lld_enable(&STMD1);

  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
  sd_lld_start(&SD1, NULL);

  /* Application main loop.*/
  for ( ; ; ) {
      pal_lld_togglepad(PORT_A,Led_D11);
      sysWaitMilliseconds(200);
      pal_lld_togglepad(PORT_A,Led_D12);
      sysWaitMilliseconds(200);
      pal_lld_togglepad(PORT_G,Led_D13);
      sysWaitMilliseconds(200);
  }
}
