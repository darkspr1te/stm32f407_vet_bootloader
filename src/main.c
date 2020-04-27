// MKS TFT 3.5" V1.0 Bootloader By darkspr1te & iz3man 
// reads mkstft35.bin from sdcard and flashes to 0xc0000 



#include "main.h"
#include "stm32f407xx.h"
#include "mkstft35.h"
#include "lcd.h"
#include "usart.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

void SystemClock_Config(void);
 void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
 void MX_USART3_UART_Init(void);
 void MX_USART6_UART_Init(void);
void test_stuff(void);

int main(void)
{
  HAL_Init();
 // HAL_MspInit();
  SystemClock_Config();
  MX_GPIO_Init();
  for (int x=0;x<50;x++)
  {
    HAL_GPIO_WritePin(SCH_BACK_LIGHT_GPIO_Port,SCH_BACK_LIGHT_Pin,GPIO_PIN_SET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(SCH_BACK_LIGHT_GPIO_Port,SCH_BACK_LIGHT_Pin,GPIO_PIN_RESET);
    HAL_Delay(50);
  }
  
  MX_USART1_UART_Init();//TX,RX
  MX_USART3_UART_Init();//WIFI TX RX
  MX_USART6_UART_Init();//AUX-1 ?
//To-do 
//Spi init 
//eprom init 
//flash write 
  init_lcd();

HAL_GPIO_WritePin(SCH_BACK_LIGHT_GPIO_Port,SCH_BACK_LIGHT_Pin,GPIO_PIN_SET);
  #ifdef DEBUG
    printf("\n\r\n\r\n\rBooting\n\r");
    printf("Software version: %s\r\n",SOFTWARE_VERSION);
    printf("Board Build: \"%s\"\r\n",HARDWARE);
    printf("Build epoch %d\n\r",LAST_BUILD_TIME);
    printf("%s\n\r",LOADER_VARIANT);
  #endif


// Ok we blinked the screen now lets boot
//
//NVIC_SetVectorTable(NVIC_VectTab_FLASH,APP_ADDRESS);
 Jump_To_App();

}

void test_stuff(void)
{
  
  //do nothing for now, just testing so blink the LCD backlight 
  //schematics say PA1 is backlight, code says it's PD12 WTF?
  // so we blink a differing number of times so user can tell me which pins
  // alternate method is we choose a pin and blink it, if it works we carry on if not we swap pins and carry on.
  for (int count=0;count<6;count++)
    {
      HAL_GPIO_WritePin(BLACK_LIGHT_GPIO_Port,BLACK_LIGHT_Pin,GPIO_PIN_SET);
      HAL_Delay(100);
      HAL_GPIO_WritePin(BLACK_LIGHT_GPIO_Port,BLACK_LIGHT_Pin,GPIO_PIN_RESET);
      HAL_Delay(100);
    }
  for (int count=0;count<3;count++)
    {
      HAL_Delay(100);
      HAL_GPIO_WritePin(SCH_BACK_LIGHT_GPIO_Port, SCH_BACK_LIGHT_Pin,GPIO_PIN_SET);
      HAL_Delay(100);
      HAL_GPIO_WritePin(SCH_BACK_LIGHT_GPIO_Port,SCH_BACK_LIGHT_Pin,GPIO_PIN_RESET);
    }
  
}
//move vector table 
inline void moveVectorTable(uint32_t Offset)
{
    // __disable_irq();
    SCB->VTOR = FLASH_BASE | Offset;
}


//goto application
 static uint8_t go_to(uint32_t myfunc)
{
	uint8_t ret = 0;
	void(*ptr)(void);
	if((*(volatile uint32_t *)myfunc & 0x2ffe0000) == 0x20000000)
	{

		__set_MSP((*(volatile uint32_t *)myfunc));
		ptr = (void(*)(void))(*(__IO uint32_t*)(myfunc+4));

		ptr();
	}
	return ret;      
}
//Jump to application

 void Jump_To_App(void)
 {
     // f_mount(NULL, SPISD_Path, 1);
    //  HAL_SPI_MspDeInit(&hspi1);
     // HAL_TIM_Base_MspDeInit(&htim2);
  while (1);
      __HAL_RCC_GPIOA_CLK_DISABLE();
      __HAL_RCC_GPIOB_CLK_DISABLE();
      __HAL_RCC_GPIOC_CLK_DISABLE();
      __HAL_RCC_GPIOD_CLK_DISABLE();
      __HAL_RCC_GPIOE_CLK_DISABLE();

      HAL_DeInit();

      // Disabling SysTick interrupt
      SysTick->CTRL = 0;
      moveVectorTable(MAIN_PR_OFFSET);
      // Setting initial value to stack pointer
      __set_MSP(*mcuFirstPageAddr);
      // booting really

      Callable resetHandler = (Callable) (*(mcuFirstPageAddr + 1) );
      resetHandler();
 }

//Configure clocks 
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);                                
} 

/** NVIC Configuration
*/
 void MX_NVIC_Init(void)
{
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* TIM4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM4_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
  /* TIM5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(TIM5_IRQn);
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART1_UART_Init(void)
{


  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }


}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART3_UART_Init(void)
{


  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }


}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
 void MX_USART6_UART_Init(void)
{


  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }


}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
 void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
//still have to config the ports before any will function 
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIOA, X_STEP_Pin, GPIO_PIN_SET); 


  GPIO_InitStruct.Pin = T_CS_Pin|T_PEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2;      
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


//according to schematics PA1 is backlight signal
  GPIO_InitStruct.Pin = SCH_BACK_LIGHT_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SCH_BACK_LIGHT_GPIO_Port, &GPIO_InitStruct);
//according to oem source code , ahem, "blacklight is on PD12"
  GPIO_InitStruct.Pin = BLACK_LIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(BLACK_LIGHT_GPIO_Port, &GPIO_InitStruct);

  HAL_GPIO_WritePin(SCH_BACK_LIGHT_GPIO_Port,SCH_BACK_LIGHT_Pin,GPIO_PIN_SET);
////


  GPIO_InitStruct.Pin = F_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  GPIO_InitStruct.Pin = PB12_Pin ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SDIO_CD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDIO_CD_GPIO_Port, &GPIO_InitStruct);


  HAL_GPIO_WritePin(GPIOC, T_CS_Pin|T_PEN_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB, PB12_Pin |F_CS_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB,PB12_Pin, GPIO_PIN_RESET);

 
  HAL_GPIO_WritePin(SDIO_CD_GPIO_Port, SDIO_CD_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);

GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_15;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_8;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_13;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_13;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}


void Error_Handler(void)
{

}
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  /* Check the parameters */
  assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
  assert_param(IS_NVIC_OFFSET(Offset));  
   
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
