#include "stm32f10x.h"
#include "systick.h"

volatile static uint32_t tick = 0;

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    /* setup systick timer for (SysTick_Freq) Hz interrupts */
    if (SysTick_Config(SystemCoreClock / SysTick_Freq)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x05U);
}

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay(uint32_t count)
{
		uint32_t utick;
    utick = count + get_tick();

    while(get_tick() < utick){
    }
}

/*!
    \brief      delay decrement
    \param[in]  none
    \param[out] none
    \retval     none
*/
void delay_decrement(void)
{
		tick++;
}

/*!
    \brief      get systick tick
    \param[in]  none
    \param[out] none
    \retval     none
*/

uint32_t get_tick(void)
{
	return tick;
}

void systick_DisableIRQ(void)
{
  /* Disable SysTick Interrupt */
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void systick_EnableIRQ(void)
{
  /* Enable SysTick Interrupt */
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void systick_Disable(void)
{
	/* Disable SysTick */
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void systick_Enable(void)
{
	/* Enable SysTick */
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void systick_deinit(void)
{
	SysTick->CTRL  = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
}
