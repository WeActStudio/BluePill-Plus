/* WeAct Studio */
/* GD32F303CCT6 Core Board */
/* Taobao: https://weactstudio.taobao.com */
/* Github: https://github.com/WeActTC */
/* Gitee:  https://gitee.com/WeAct-TC */

#include "usb_iap_core.h"
#include "usbd_hw.h"
#include "systick.h"
#include "gpio.h"

#define KEY_RCU  RCU_GPIOA
#define KEY_PORT GPIOA
#define KEY_PIN GPIO_PIN_0

#define LED_PB2_RCU  RCU_GPIOB
#define LED_PB2_PORT GPIOB
#define LED_PB2_PIN  GPIO_PIN_2

usb_dev usbd_iap;

app_func application;
uint32_t app_address;

static uint8_t bootloader_init(void);
void jump_to_app(uint32_t addr);
void* addrCov2ram(void* addr,uint8_t *ram);
void RamVectorSet(void);
void bootloader_runram(void);
void bootloader_erase(void);

/*!
    \brief      main routine
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
	// Code Size must <= 7176
	
	/* enable the KEY clock */
	rcu_periph_clock_enable(KEY_RCU);
	rcu_periph_clock_enable(RCU_AF);
	
	/* configure button pin as input */
	gpio_init(KEY_PORT, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, KEY_PIN);
    
	if(bootloader_init() != 0)
	{
		gpio_config();
		if (SysTick_Config(SystemCoreClock / 15U)){
			/* capture error */
			while (1){
			}
		}
		/* configure the systick handler priority */
		NVIC_SetPriority(SysTick_IRQn, 0x05U);
		
		while(1)
		{
			gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,(gpio_output_bit_get(LED_PB2_PORT,LED_PB2_PIN)) == SET?RESET:SET);
			pmu_to_sleepmode(WFI_CMD);
		}
	}

	/* KEY must be pressed when power on */
	if(gpio_input_bit_get(KEY_PORT,KEY_PIN) != SET) {	
		jump_to_app(APP_LOADED_ADDR);
	}

	systick_config();

	gpio_config();

	usb_reset();

	/* system clocks configuration */
	rcu_config();

	/* USB device configuration */
	usbd_init(&usbd_iap, &iap_desc, &iap_class);

	/* NVIC configuration */
	nvic_config();

	/* enabled USB pull-up */
	usbd_connect(&usbd_iap);

	uint32_t tick1 = get_tick();

	while(gpio_input_bit_get(KEY_PORT,KEY_PIN) == SET)
	{
		gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,(gpio_output_bit_get(LED_PB2_PORT,LED_PB2_PIN)) == SET?RESET:SET);
		delay_lp(80/SysTick_Tick);
	}
	while (1)
	{
		if(gpio_input_bit_get(KEY_PORT,KEY_PIN) == SET) 
		{
			gpio_bit_reset(LED_PB2_PORT,LED_PB2_PIN);
			while(gpio_input_bit_get(KEY_PORT,KEY_PIN) == SET)
			{
				gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,(gpio_output_bit_get(LED_PB2_PORT,LED_PB2_PIN)) == SET?RESET:SET);
				delay_lp(80/SysTick_Tick);
			}
			delay_lp(100/SysTick_Tick);
			/* goto APP */
			NVIC_SystemReset();
		}
		else
		{
			/* PB2 Breathing Lamp test */
			static uint8_t pwmset;
			static uint16_t time;
			static uint8_t timeflag;
			static uint8_t timecount;
			if((get_tick() - tick1 >= 1 ))
			{
				tick1 = get_tick();
				
				 /* Breathing Lamp */
				if(timeflag == 0)
				{
					time ++;
					if(time >= 1600) timeflag = 1;
				}
				else
				{
					time --;
					if(time == 0) timeflag = 0;
				}
				
				/* Duty Cycle Setting */
				pwmset = time/80;

				/* 20ms Pulse Width */
				if(timecount > 20) timecount = 0;
				else timecount ++;

				if(timecount >= pwmset ) gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,RESET);
				else gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,SET);
			 }
		}
		pmu_to_sleepmode(WFI_CMD);
	}
}

static uint8_t bootloader_init(void)
{
	uint32_t data;
  uint32_t count;
	uint8_t result = 0;
	
	data = (APP_LOADED_ADDR - 16);
  count = (APP_LOADED_ADDR - 4);
	fmc_unlock();
	for(uint8_t i = 0;i<12;i+=4)
	{
		if(*(uint32_t *)(data+i) != *(uint32_t *)(DEVICE_ID+i))
		{
			if(*(uint32_t *)(data+i) == 0xFFFFFFFF)
			{
				fmc_word_program((data+i),*(uint32_t *)(DEVICE_ID+i));
			}
			else
			{
        data = *(uint32_t *)(count);
        if(data > (uint32_t)-16)
        {
          data = data<<1;
          fmc_word_reprogram(count,data);
          result = 0;
          break;
        }
        else
          bootloader_runram();
			}
			result++;
		}
	}
	fmc_lock();
	return result;
}

void jump_to_app(uint32_t addr)
{
	uint32_t app_address;
	void* (*application)(void);
	
	/* test if user code is programmed starting from address */
	if (((*(__IO uint32_t*)addr) & 0x2FFE0000) == 0x20000000)
	{
		app_address = *(__IO uint32_t*) (addr + 4);
		application = (void* (*)(void)) app_address;

		/* initialize user application's stack pointer */
		__set_MSP(*(__IO uint32_t*) addr);
		
		rcu_deinit();
		
		/* jump to user application */
		application();
	}
}

#define RAM_CODE_ADDR 0x2000A000
#if   defined ( __CC_ARM )
uint8_t ramrun[1024*8] __attribute__((at(RAM_CODE_ADDR)));
#else
#define STR(x) #x
#define RAM_CODE_ADDR_AC6(x) STR(x)
uint8_t ramrun[1024*8] __attribute__((section(".ARM.__at_"RAM_CODE_ADDR_AC6(RAM_CODE_ADDR))));

#endif
void* addrCov2ram(void* addr,uint8_t *ram) 
{
	return (void*)((uint32_t)addr - FLASH_BASE + ram);
}

void RamVectorSet(void)
{
	uint32_t* p = (uint32_t*)(RAM_CODE_ADDR+4);	
	uint8_t i;
	
	p[1] = (uint32_t)addrCov2ram((void*)(p[1]),ramrun);
	p[3] = (uint32_t)addrCov2ram((void*)(p[3]),ramrun);	
	
	for(i = 14; i < 75; i++)
	{
		p[i] = (uint32_t)addrCov2ram((void*)(p[i]),ramrun);
	}
}

void bootloader_runram(void)
{
	#include "string.h"
	
	void* (*function)(void);
	
	memcpy((void *)ramrun,(const void *)FLASH_BASE,sizeof(ramrun));
	
	RamVectorSet();
	nvic_vector_table_set(NVIC_VECTTAB_RAM,RAM_CODE_ADDR-NVIC_VECTTAB_RAM);
	function = (void* (*)(void))addrCov2ram(bootloader_erase,ramrun);
	__set_MSP(RAM_CODE_ADDR);
	function();
}

void bootloader_erase(void)
{
	gpio_config();
	gpio_bit_set(LED_PB2_PORT,LED_PB2_PIN);
	fmc_unlock();
	if(FMC_READY == fmc_mass_erase())
    gpio_bit_reset(LED_PB2_PORT,LED_PB2_PIN);
  ob_unlock();
  ob_security_protection_config(FMC_USPC);
  ob_lock();
	fmc_lock();
	while(1)
	{
	}
}


