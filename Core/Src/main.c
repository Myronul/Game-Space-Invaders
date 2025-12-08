/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "st7735.h"
#include "GFX_FUNCTIONS.h"
#include "fonts.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef struct Entitate
{
	int16_t x;
	int16_t y;

}Entitate;


bool ok = 0;
bool start = 0;
bool intrerupere = 0;
bool trigger = 0;

uint8_t latime=128;
uint8_t lungime=128;

uint8_t apasat12=0;//sus
uint8_t apasat13=0;//jos
uint8_t apasat14=0;//dreapta
uint8_t apasat15=0;//stanga

uint16_t timp=0;
uint16_t timp2=0;

Entitate entitate[20];//entitate[0] va fi userul -> racheta
uint8_t contorEntitati=0;

Entitate gloanteNava[300];
uint8_t contorGloanteNava = 0;

Entitate gloanteInamici[300];
uint8_t contorGloanteInamici = 0;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USB_PCD_Init(void);
/* USER CODE BEGIN PFP */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim2)
{
	timp++;//timerul se apeleaza de 100 de ori pe secunda deci
		  //o secunda trece cand timp ajunge la 100 analog 50 pentru 500 ms
		 //Nu este cazul pentru Overflow deoarece variabila se incrementeaza doar de 100 de ori pe secunda
		//Pentru 30 de secunde brb am avea 3000 maximul pe 16 biti unsigned fiind 65.536-1

	timp2++;

	if(timp2==6000)
	{
		timp2 = 0;
	}
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void initializare_ecran_start(int *speed)
{

	ST7735_FillScreenFast(ST7735_BLACK);

	ST7735_WriteString(0, 8, "  Space Invaders!", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	ST7735_WriteString(20, 32, "Dificultate: ", Font_7x10, ST7735_WHITE, ST7735_BLACK);

	ST7735_WriteString( 7, lungime/2, "Mica", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	ST7735_WriteString( latime-35 , lungime/2, "Mare", Font_7x10, ST7735_WHITE, ST7735_BLACK);

	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)!=1)//cat timp nu am apasat pe trigger si am selectat viteza, putem alege
	{

		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)==1)//am apasat stanga->selectam mica
		{
			ST7735_WriteString(7 , lungime/2, "Mica", Font_7x10, ST7735_BLUE, ST7735_BLACK);
			ST7735_WriteString(latime-35 , lungime/2, "Mare", Font_7x10, ST7735_WHITE, ST7735_BLACK);

			*speed=25;
		}

		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)==1)//am apasat dreapta->selectam mare
		{
			ST7735_WriteString(7, lungime/2, "Mica", Font_7x10, ST7735_WHITE, ST7735_BLACK);
			ST7735_WriteString(latime-35, lungime/2, "Mare", Font_7x10, ST7735_BLUE, ST7735_BLACK);

			*speed=5;

		}

	}


	ST7735_FillScreenFast(ST7735_BLACK);

}

void ecran_game_over()
{
	ST7735_FillScreenFast(ST7735_BLACK);
	ST7735_WriteString(0, 64, "    Game Over!", Font_7x10, ST7735_WHITE, ST7735_BLACK);
}

void ecran_joc_castigat()
{
	ST7735_FillScreenFast(ST7735_BLACK);
	ST7735_WriteString(0, 64, "   Level Passed!", Font_7x10, ST7735_WHITE, ST7735_BLACK);
}

void pixel_racheta_jucator()
{
	uint8_t i;
	uint8_t j;
	//incepem de la 8 pixeli fata de jos


	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if((i==0 && j==0) || (i==0 && j==1) || (i==0 && j==2) || (i==0 && j==3) || (i==0 && j==5) || (i==0 && j==6) ||
			   (i==1 && j==3) || (i==1 && j==5) || (i==2 && j==3) || (i==2 && j==5) || (i==2 && j==7) || (i==3 && j==1) ||
			   (i==3 && j==2) || (i==3 && j==3) || (i==3 && j==4) || (i==3 && j==5) || (i==3 && j==6) || (i==3 && j==7) ||
			   (i==4 && j==1) || (i==4 && j==2) || (i==4 && j==3) || (i==4 && j==4) || (i==4 && j==5) || (i==4 && j==6) ||
			   (i==4 && j==7) || (i==5 && j==3) || (i==5 && j==5) || (i==5 && j==7) || (i==6 && j==3) || (i==6 && j==5) ||
			   (i==7 && j==0) || (i==7 && j==1) || (i==7 && j==2) || (i==7 && j==3) || (i==7 && j==5) || (i==7 && j==6) )

			{
				//coloreaza pixelul albastru
				ST7735_DrawPixel(entitate[0].x+i,entitate[0].y+j, ST7735_BLUE);
			}

			else
			{
				//coloreaza negru
				ST7735_DrawPixel(entitate[0].x+i,entitate[0].y+j, ST7735_BLACK);
			}
		}
	}

}


void refresh_glont(uint8_t x, uint8_t y)
{
	uint8_t i,j;

	for(i=0;i<4;i++)
	{
		for(j=0;j<2;j++)
		{
			ST7735_DrawPixel(x+j,y+i, ST7735_BLACK);
		}
	}
}

void pixel_glont(uint8_t x, uint8_t y)
{
	uint8_t i,j;

	for(i=0;i<4;i++)
	{
		for(j=0;j<2;j++)
		{
			ST7735_DrawPixel(x+j,y+i, ST7735_RED);
		}
	}
}



void refresh_gloante()
{
	uint8_t i;

	for(i=1;i<=contorGloanteNava;i++)
	{
		refresh_glont(gloanteNava[i].x,gloanteNava[i].y);
	}

	for(i=1;i<=contorGloanteInamici;i++)
	{
		refresh_glont(gloanteInamici[i].x,gloanteInamici[i].y);
	}
}

void draw_gloante()
{
	uint8_t i;

	for(i=1;i<=contorGloanteNava;i++)
	{
		pixel_glont(gloanteNava[i].x,gloanteNava[i].y);
	}

	for(i=1;i<=contorGloanteInamici;i++)
	{
		pixel_glont(gloanteInamici[i].x,gloanteInamici[i].y);
	}

}



void refresh_entitate(uint8_t x, uint8_t y)
{
	uint8_t i;
	uint8_t j;

	for(i=x;i<x+8;i++)
	{
		for(j=y;j<y+8;j++)
		{
			ST7735_DrawPixel(i,j, ST7735_BLACK);
		}
	}
}

void pixel_adversar(uint8_t x, uint8_t y)
{
	uint8_t i;
	uint8_t j;

	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if( (i==2 && j==1) || (i==3 && j==1) || (i==4 && j==1) || (i==5 && j==1) || (i==1 && j==2) || (i==2 && j==2) ||
				(i==3 && j==2) || (i==4 && j==2) || (i==5 && j==2) || (i==6 && j==2) || (i==1 && j==3) || (i==3 && j==3) ||
				(i==4 && j==3) || (i==6 && j==3) || (i==1 && j==4) || (i==2 && j==4) || (i==3 && j==4) || (i==4 && j==4)
				|| (i==5 && j==4) || (i==6 && j==4) || (i==2 && j==5) || (i==3 && j==5) || (i==4 && j==5) || (i==5 && j==5)
				|| (i==1 && j==6) || (i==3 && j==6) || (i==4 && j==6) || (i==6 && j==6))
			{
				ST7735_DrawPixel(x+i,y+j, ST7735_WHITE);
			}

			else

			{
				ST7735_DrawPixel(x+i,y+j, ST7735_BLACK);
			}
		}
	}
}

void draw_adversari()
{
	uint8_t i;

	for(i=1;i<=contorEntitati;i++)
		{
			pixel_adversar(entitate[i].x,entitate[i].y);
		}
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//Functie de intreupere de sistem, cand unul din butoanele directionale vor fi apasate
	//aceasta functie va fi apelata si parcursa

    if(timp >= 15)
    {
        intrerupere = 1;

        if(GPIO_Pin == GPIO_PIN_11)
        {
        	trigger = 1;
        }

        if(GPIO_Pin == GPIO_PIN_12)
        {
        	//SUS

            apasat12 = 1;
            apasat13 = 0;
            apasat14 = 0;
            apasat15 = 0;

        }

        if(GPIO_Pin == GPIO_PIN_13)
        {
        	//JOS
            apasat12 = 0;
            apasat13 = 1;
            apasat14 = 0;
            apasat15 = 0;

        }

        if(GPIO_Pin == GPIO_PIN_14)
        {
        	//DREAPTA

            apasat12 = 0;
            apasat13 = 0;
            apasat14 = 1;
            apasat15 = 0;

        }

        if(GPIO_Pin == GPIO_PIN_15)
        {
        	//STANGA

            apasat12 = 0;
            apasat13 = 0;
            apasat14 = 0;
            apasat15 = 1;

        }

        timp = 0;

    }
}

void deplasare_jucator()
{

	if(apasat12 == 1)
	{
		//sus

        if((entitate[0].y >= 32))
        {
            refresh_entitate(entitate[0].x, entitate[0].y);
            entitate[0].y = entitate[0].y - 4;
            pixel_racheta_jucator();
        }

        apasat12 = 0;

	}


	if(apasat13 == 1)
	{
		//jos

        if((entitate[0].y != 120))
        {
            refresh_entitate(entitate[0].x, entitate[0].y);
            entitate[0].y = entitate[0].y + 4;
            pixel_racheta_jucator();
        }

        apasat13 = 0;

	}

	if(apasat14 == 1)
	{
		//dreapta

        if((entitate[0].x != 120))
        {
            refresh_entitate(entitate[0].x, entitate[0].y);
            entitate[0].x = entitate[0].x + 4;
            pixel_racheta_jucator();
        }

        apasat14 = 0;

	}

	if(apasat15 == 1)
	{
		//stanga

        if((entitate[0].x != 0))
        {
            refresh_entitate(entitate[0].x, entitate[0].y);
            entitate[0].x = entitate[0].x - 4;
            pixel_racheta_jucator();
        }

        apasat15 = 0;

	}


}


void initializare_adversar_nivel1()
{

	contorEntitati=4;

	entitate[1].x=8;
	entitate[1].y=8;

	entitate[2].x=40;
	entitate[2].y=8;

	entitate[3].x=72;
	entitate[3].y=8;

	entitate[4].x=104;
	entitate[4].y=8;

	draw_adversari();
}


void initializare_nivel_1()
{
	//primul nivel va fi cu 4 adversari fiecare divizat la o distanta pe aceeasi linie 64,112

	entitate[0].x=64;
	entitate[0].y=112;

	pixel_racheta_jucator();
	initializare_adversar_nivel1();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------

void verifica_coliziune_nava_inamica()
{
	uint8_t i,j,k;

	for(i=1;i<=contorGloanteNava;i++)
	{
		for(j=1;j<=contorEntitati;j++)
		{
			if((gloanteNava[i].x >= entitate[j].x) && (gloanteNava[i].x <= entitate[j].x +7) &&
			   (gloanteNava[i].y <= entitate[j].y + 7))
			{
				//de pe ecran
				refresh_entitate(entitate[j].x, entitate[j].y);

				//stergem entitatea din array

				for(k=j;k<contorEntitati;k++)
				{
					entitate[k] = entitate[k+1];
				}

				contorEntitati--;
			}

			else
			{
				continue;
			}
		}
	}
}

bool verifica_coliziune_jucator()
{
	uint8_t i;

	for(i=1;i<=contorGloanteInamici;i++)
	{
		if((gloanteInamici[i].x >= entitate[0].x) && (gloanteInamici[i].x <= entitate[0].x + 7) &&
		   (gloanteInamici[i].y + 4 >= entitate[0].y) && (gloanteInamici[i].y + 4 <= entitate[0].y + 7))
		{
			ST7735_FillScreenFast(ST7735_BLACK);
			return 1;
		}
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------

bool verifica_joc_castigat()
{
	if(contorEntitati == 0) //entitatea 0 este jucatorul
	{

		ST7735_FillScreenFast(ST7735_BLACK);
		return 1;

	}

	else
	{
		return 0;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

void spawn_gloante_inamice()
{
	uint8_t rand_inamic=0;


	if(timp2>=100)
	{
		rand_inamic = (rand()%contorEntitati) + 1;

		contorGloanteInamici++;

		gloanteInamici[contorGloanteInamici].x = entitate[rand_inamic].x + 3;
		gloanteInamici[contorGloanteInamici].y = entitate[rand_inamic].y + 8;

		timp2=0;
	}

}


void verifica_trigger()
{
	//verificam daca am apasat pe butonul de tras gloante

	if(trigger == 1)
	{
		contorGloanteNava++;

		gloanteNava[contorGloanteNava].x = entitate[0].x + 3;
		gloanteNava[contorGloanteNava].y = entitate[0].y - 8;

		trigger = 0;

	}

}


void deplasare_gloante()
{
	uint8_t i;
	for(i=1;i<=contorGloanteInamici;i++)
	{
		gloanteInamici[i].y = gloanteInamici[i].y + 1;
	}


	for(i=1;i<=contorGloanteNava;i++)
	{
		gloanteNava[i].y = gloanteNava[i].y - 1;
	}

}

void verifica_coordonate_gloante()
{
	/*functie ce verifica ca coorodnatele gloantelor inamice sa nu iasa din raza mapei de pixeli
	a ecranului de 128x128x
	*/

	uint8_t i;
	uint8_t j;

	for(i=1;i<=contorGloanteInamici;i++)
	{
		if(gloanteInamici[i].y >= 128)
		{

			for(j=i;j<contorGloanteInamici;j++)
			{
				gloanteInamici[j]=gloanteInamici[j+1];
			}

			contorGloanteInamici--;
		}

		else
		{
			continue;
		}
	}


	for(i=1;i<=contorGloanteNava;i++)
	{
		if(gloanteNava[i].y <= (-4))
		{

			for(j=i;j<contorGloanteNava;j++)
			{
				gloanteNava[j]=gloanteNava[j+1];
			}

			contorGloanteNava--;
		}

		else
		{
			continue;
		}
	}



}

//----------------------------------------------------------------------------------------------------------------------------------------------------

void logica_coliziuni()
{
	verifica_coliziune_nava_inamica();
}

void logica_gloante()
{


	if(start==0) //pentru initializare
	{
		spawn_gloante_inamice();
		draw_gloante();
		start = 1;
		return;
	}

	logica_coliziuni();

	refresh_gloante();
	deplasare_gloante();
	spawn_gloante_inamice();
	verifica_trigger();
	draw_gloante();

	verifica_coordonate_gloante();

}

void fizica_jucator()
{

	//aici vor fi scrise logicile pentru impuscaturi si game over, scor etc
	deplasare_jucator();
	logica_gloante();

}

void motor_joc()
{
	if(ok==0)
	{
		initializare_nivel_1();
		ok=1;
	}

	fizica_jucator();
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  int speed = 25;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USB_PCD_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start_IT(&htim2);//pornim counterul-ceasul

  ST7735_Init();

  ST7735_FillScreenFast(ST7735_BLACK);

  initializare_ecran_start(&speed);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  motor_joc();
	  srand(HAL_GetTick());
	  HAL_Delay(speed);

	  if(verifica_coliziune_jucator() == 1)
	  {
		  ecran_game_over();
		  while(1);
	  }

	  if( verifica_joc_castigat() == 1)
	  {
		  ecran_joc_castigat();
		  while(1);
	  }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7199;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
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
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB4 PB5
                           PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB11 PB12 PB13 PB14
                           PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
