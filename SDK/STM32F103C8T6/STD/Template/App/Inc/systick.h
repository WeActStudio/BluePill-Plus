#ifndef SYS_TICK_H
#define SYS_TICK_H

#include <stdint.h>

#define SysTick_Freq  (100U)  // Hz
#define SysTick_Tick  (1000U / SysTick_Freq)

/* configure systick */
void systick_config(void);
/* delay a time in milliseconds */
void delay(uint32_t count);
/* delay decrement */
void delay_decrement(void);
/* get system tick */
uint32_t get_tick(void);
/* suspend system tick */
void systick_DisableIRQ(void);
/* resume system tick */
void systick_EnableIRQ(void);

void systick_Disable(void);
void systick_Enable(void);
void systick_deinit(void);

#endif /* SYS_TICK_H */
