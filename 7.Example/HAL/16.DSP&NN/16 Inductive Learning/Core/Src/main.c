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

// ����ѧϰ

#define OK 1
#define NG 0
#define SETSIZE 100   // ѧϰ���ݼ��Ĵ�С
#define CNO 10        // ѧϰ���ݵ�λ����10��
#define GENMAX 10000  // ���ɺ�ѡ��Ĵ���
#define SEED 32767    // �������seed

// ����ѧϰ���ݼ�
void readdata(int data[SETSIZE][CNO], int teacher[SETSIZE]);

// ����0��1��2���������
int rand012();

// �����ѡ��ģʽ�ĵ÷֣�0��SETSIZE��
int calcscore(int data[SETSIZE][CNO], int teacher[SETSIZE], int answer[CNO]);

// ��ѡ��ģʽ�ĵ÷֣�0��SETSIZE���ļ���
int calcscore(int data[SETSIZE][CNO], int teacher[SETSIZE], int answer[CNO]) {
    int score = 0;  // �÷֣�0��SETSIZE��
    int point;      // ��һ�µ�λ��(0��CNO)
    int i, j;

    for (i = 0; i < SETSIZE; ++i) {
        // ����һ�³̶�
        point = 0;
        for (j = 0; j < CNO; ++j) {
            if (answer[j] == 2)
                ++point;  // ͨ���
            else if (answer[j] == data[i][j])
                ++point;  // ��һ��
        }

        if ((point == CNO) && (teacher[i] == 1)) {
            ++score;
        } else if ((point != CNO) && (teacher[i] == 0)) {
            ++score;
        }
    }
    return score;
}

// ����ѧϰ���ݼ�
void readdata(int data[SETSIZE][CNO], int teacher[SETSIZE]) {
    int i, j;
    for (i = 0; i < SETSIZE; ++i) {
        for (j = 0; j < CNO; ++j)
            scanf("%d", &data[i][j]);
        scanf("%d", &teacher[i]);
    }
}

// ����0��1��2���������
int rand012() {
    int rnd;

    // ȥ������������ֵ
    while ((rnd = rand()) == RAND_MAX)
        ;
    // ���������
    return (double)rnd / RAND_MAX * 3;
}

// ѧϰ���ݼ�
int data[SETSIZE][CNO] = {
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {0, 1, 0, 0, 0, 1, 1, 0, 1, 0},
    {1, 0, 0, 1, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 1, 1, 0, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {1, 1, 1, 1, 0, 0, 1, 1, 0, 1},
    {0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {0, 0, 1, 1, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 1},
    {0, 1, 0, 0, 1, 0, 0, 0, 1, 1},
    {0, 1, 1, 0, 1, 0, 0, 1, 1, 1},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 1, 1, 0},
    {1, 1, 0, 0, 1, 1, 1, 1, 0, 0},
    {1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 1, 0, 1},
    {1, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 1, 1, 0},
    {1, 1, 1, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0, 1, 0},
    {1, 0, 1, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 1, 1, 0},
    {1, 1, 0, 1, 0, 0, 0, 0, 0, 1},
    {0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 0, 0, 1},
    {1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 1, 0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 0, 1, 0, 1, 1, 0, 1},
    {0, 1, 1, 1, 1, 0, 0, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 1, 0, 0},
    {1, 0, 1, 1, 1, 1, 0, 0, 1, 0},
    {0, 1, 0, 0, 1, 0, 1, 1, 0, 0},
    {1, 1, 1, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 0, 1, 1, 0, 0, 1, 0},
    {0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
    {0, 1, 1, 0, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 0, 0, 1, 1},
    {1, 0, 1, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 1, 0, 0, 1},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 0, 1, 1, 1, 0, 1, 1},
    {0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 1, 1, 1},
    {0, 1, 0, 1, 1, 1, 0, 0, 1, 0},
    {1, 1, 1, 1, 0, 0, 1, 0, 1, 1},
    {1, 1, 1, 0, 0, 1, 0, 1, 1, 1},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 1, 1, 0, 1, 0, 1},
    {0, 0, 0, 1, 1, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 1},
    {0, 1, 0, 1, 0, 1, 0, 1, 1, 0},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 0, 0, 1, 0, 1, 0, 1},
    {0, 1, 0, 0, 0, 0, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 0, 0, 0, 1, 1},
    {0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 0, 1, 0, 1, 1, 0, 0, 1},
    {0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 1, 1, 0, 1, 1, 1},
    {0, 0, 1, 1, 0, 0, 1, 0, 0, 1},
    {0, 0, 1, 1, 1, 1, 1, 0, 1, 1},
    {0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
    {0, 1, 0, 0, 0, 0, 1, 0, 1, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 0, 1, 1},
    {0, 1, 1, 0, 1, 1, 1, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 1, 0, 0},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 1, 1, 1}};

// ��ʦ����
int teacher[SETSIZE] = {1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};

void dsp_test(void) {
    int i, j;
    int score = 0;    // �÷֣�0��SETSIZE��
    int answer[CNO];  // ��ѡ��

    int bestscore = 0;    // �÷ֵ�����ֵ
    int bestanswer[CNO];  // ���������е����Ž�

    srand(SEED);  // ������ĳ�ʼ��

    // ����ѧϰ���ݼ�
    // readdata(data, teacher);

    // ���ɺ�ѡ�Ⲣ���
    for (i = 0; i < GENMAX; ++i) {
        // ���ɺ�ѡ��
        for (j = 0; j < CNO; ++j)
            answer[j] = rand012();

        // ���
        score = calcscore(data, teacher, answer);

        // ���ŵ÷ֵĸ���
        if (score > bestscore) {  // ����ǵ�ĿǰΪֹ������ֵ�����
            for (j = 0; j < CNO; ++j)
                bestanswer[j] = answer[j];
            bestscore = score;
            for (j = 0; j < CNO; ++j)
                printf("%1d ", bestanswer[j]);
            printf(":score=%d\r\n", bestscore);
        }
    }
    // ������Ž�
    printf("\r\n���Ž�\r\n");
    for (j = 0; j < CNO; ++j)
        printf("%1d ", bestanswer[j]);
    printf(":score=%d\r\n", bestscore);
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
