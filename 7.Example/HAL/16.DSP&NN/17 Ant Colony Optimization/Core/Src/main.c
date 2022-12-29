/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define NOA 10        // ���ϵĸ�����
#define ILIMIT 50     // ѭ������
#define Q 3           // ��Ϣ�ظ��µĳ���
#define RHO 0.8       // �ӷ�����
#define STEPS 10      // ·�̵Ĳ���
#define EPSILON 0.15  // ȷ���ж�ѡ��������
#define SEED 32768    // �������Seed

void   printp(double pheromone[2][STEPS]);                                              // ��ʾ
void   printmSTEPS(int mSTEPS[NOA][STEPS]);                                             // ��ʾ���ϵ��ж�
void   walk(int cost[2][STEPS], double pheromone[2][STEPS], int mSTEPS[NOA][STEPS]);    // ʹ���ϲ���
void   update(int cost[2][STEPS], double pheromone[2][STEPS], int mSTEPS[NOA][STEPS]);  // ������Ϣ��
double rand1(void);                                                                     // ����0��1ʵ�����������
int    rand01(void);                                                                    // ����0��1���������

// ������Ϣ��
void update(int cost[2][STEPS], double pheromone[2][STEPS], int mSTEPS[NOA][STEPS]) {
    int    m;   // ���ϸ���ĺ���
    int    lm;  // ���ϲ��еľ���
    int    i, j;
    double sum_lm = 0;  // ���ϲ��е��������

    // ��Ϣ�صĻӷ�
    for (i = 0; i < 2; ++i)
        for (j = 0; j < STEPS; ++j)
            pheromone[i][j] *= RHO;

    // ���ϵĸ���
    for (m = 0; m < NOA; ++m) {
        // ����m���ƶ�����lm�ļ���
        lm = 0;
        for (i = 0; i < STEPS; ++i)
            lm += cost[mSTEPS[m][i]][i];

        // ��Ϣ�صĸ���
        for (i = 0; i < STEPS; ++i)
            pheromone[mSTEPS[m][i]][i] += Q * (1.0 / lm);
        sum_lm += lm;
    }
    // ���ϲ��е�ƽ����������
    printf("%lf\n", sum_lm / NOA);
}

// ʹ���ϲ���
void walk(int cost[2][STEPS], double pheromone[2][STEPS], int mSTEPS[NOA][STEPS]) {
    int m;  // ���ϸ���ĺ���
    int s;  // ��������������λ��

    for (m = 0; m < NOA; ++m) {
        for (s = 0; s < STEPS; ++s) {
            // ���ڦ�-greedy���ж�ѡ��
            if ((rand1() < EPSILON) || (abs(pheromone[0][s] - pheromone[1][s]) < 1e-9)) {
                // ����ж�
                mSTEPS[m][s] = rand01();
            } else {  // ������Ϣ�ص�ѡ��
                if (pheromone[0][s] > pheromone[1][s])
                    mSTEPS[m][s] = 0;
                else
                    mSTEPS[m][s] = 1;
            }
        }
    }
    // ������ϵ��ж�
    printmSTEPS(mSTEPS);
}

// ��ʾ���ϵ��ж�
void printmSTEPS(int mSTEPS[NOA][STEPS]) {
    int i, j;

    printf("mSTEPS\r\n");
    for (i = 0; i < NOA; ++i) {
        for (j = 0; j < STEPS; ++j)
            printf("%d ", mSTEPS[i][j]);
        printf("\r\n");
    }
}

// ��ʾ��Ϣ��
void printp(double pheromone[2][STEPS]) {
    int i, j;

    for (i = 0; i < 2; ++i) {
        for (j = 0; j < STEPS; ++j)
            printf("%4.2lf ", pheromone[i][j]);
        printf("\r\n");
    }
}

// ����0��1��ʵ�����������
double rand1(void) {
    // ������ļ���
    return (double)rand() / RAND_MAX;
}

// ����0��1���������
int rand01(void) {
    int rnd;
    // ȥ������������ֵ
    while ((rnd = rand()) == RAND_MAX) {}
    // ���������
    return (int)((double)rnd / RAND_MAX * 2);
}

void dsp_test(void) {
    // �����Ĵ��ۣ����룩
    int cost[2][STEPS] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                          {5, 5, 5, 5, 5, 5, 5, 5, 5, 5}};

    double pheromone[2][STEPS] = {0};  // ��������Ϣ����
    int    mSTEPS[NOA][STEPS];         // �����߹��Ĺ���
    int    i;                          // ѭ�������Ŀ���

    // ������ĳ�ʼ��
    srand(SEED);

    // ���Ż�������
    for (i = 0; i < ILIMIT; ++i) {
        // ��Ϣ�ص�״̬�����
        printf("%d:\r\n", i);
        printp(pheromone);
        // ʹ���ϲ���
        walk(cost, pheromone, mSTEPS);
        // ������Ϣ��
        update(cost, pheromone, mSTEPS);
    }
    // ��Ϣ��״̬�����
    printf("%d:\r\n", i);
    printp(pheromone);
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* USER CODE BEGIN 1 */

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
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    /* USER CODE BEGIN 2 */
    printf("Strat\r\n");
    dsp_test();
    printf("End\r\n");
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
