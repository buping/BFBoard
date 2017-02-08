/************************************
FileName:pwm.h//pwm定义
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/

#ifndef _PWM_H
#define _PWM_H

#include "Common.h"

#define Frequency_APB1 63  //APB1频率是36M
#define PWM_1K    999  //1000-1
#define PWM_10K   (6300)
#define PWM_20K   (3000)
#define PWM_50K   (1200)
#define TimerPeriod   PWM_10K
 /* Compute CCR2 value to generate a duty cycle at 50% for channel 1 and 1N */
 #define Pulse_50    (unsigned int) ((unsigned long)( TimerPeriod  /2))
 /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
 #define Pulse_25    (unsigned int) ((unsigned long)  (TimerPeriod  / 4))
 /* Compute CCR2 value to generate a duty cycle at 25%	for channel 3 and 3N */
 #define Pulse_75    (unsigned int) ((unsigned long) (TimerPeriod  / 4*3))
 /* Compute CCR2 value to generate a duty cycle at 12.5%  for channel 4 */
  #define Pulse_100  (unsigned int) ((unsigned long) TimerPeriod)
  #define Pulse_0     (0)
 #define Pulse_10    (unsigned int) ((unsigned long)  (TimerPeriod  / 10))

void initPwm(void);

#endif


