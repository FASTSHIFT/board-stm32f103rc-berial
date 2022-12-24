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

// ��Visual Studio�Ļ����Ա�֤
#define _CRT_SECURE_NO_WARNINGS

// ����GA����������������Ž�

// ���ų����Ķ���

#define MAXVALUE 100                    // �����ͼ�ֵ�����ֵ
#define N 30                            // �������
#define WEIGHTLIMIT (N * MAXVALUE / 4)  // ��������
#define POOLSIZE 30                     // Ⱦɫ�����
#define LASTG 50                        // ��ֹ���
#define MRATE 0.01                      // ͻ��ĸ���
#define SEED 32767                      // �������seed
#define YES 1                           // yes��Ӧ������ֵ
#define NO 0                            // no��Ӧ������ֵ

// ����ĳ�ʼ��
void initparcel();

// ������Ӧ��
int evalfit(int gene[]);

// ����
void mating(int pool[POOLSIZE][N], int ngpool[POOLSIZE * 2][N]);

// ͻ��
void mutation(int ngpool[POOLSIZE * 2][N]);

// ������
void printp(int pool[POOLSIZE][N]);

// ���ɳ�ʼȾɫ�弯��
void initpool(int pool[POOLSIZE][N]);

// ����n֮�µ������
int rndn(int l);
// ��ֵ�ķ�ת
int notval(int v);

// ѡ�񸸴�
int selectp(int roulette[POOLSIZE], int totalfitness);

// �����ض�Ⱦɫ��Ľ���
void crossing(int m[], int p[], int c1[], int c2[]);

void selectng(int ngpool[POOLSIZE * 2][N], int pool[POOLSIZE][N]);  // ѡ����һ��

// �������� weight,value
int parcel[N][2] = {
    {65, 27},
    {39, 82},
    {9, 85},
    {72, 71},
    {87, 91},
    {91, 28},
    {34, 92},
    {58, 79},
    {3, 27},
    {12, 82},
    {92, 15},
    {39, 49},
    {83, 54},
    {76, 43},
    {6, 26},
    {77, 2},
    {68, 6},
    {24, 60},
    {60, 47},
    {6, 40},
    {91, 58},
    {44, 68},
    {50, 33},
    {91, 92},
    {57, 62},
    {97, 49},
    {96, 68},
    {39, 77},
    {89, 6},
    {24, 97}};  // ����

// ����ĳ�ʼ��
void initparcel() {
    // int i = 0;
    // while ((i < N) && (scanf("%d %d", &parcel[i][0], &parcel[i][1]) != EOF)) ++i;
}

// ѡ����һ��
void selectng(int ngpool[POOLSIZE * 2][N], int pool[POOLSIZE][N]) {
    int i, j, c;                 // ѭ�����Ʋ���
    int totalfitness = 0;        // ��Ӧ�ȵ��ܼ�ֵ
    int roulette[POOLSIZE * 2];  // �����Ӧ��
    int ball;                    // ��ѡ��λ�õ���ֵ��
    int acc = 0;                 // ��Ӧ�ȵ��ۻ�ֵ

    // ѭ������ѡ��
    for (i = 0; i < POOLSIZE; ++i) {
        // ��������
        totalfitness = 0;
        for (c = 0; c < POOLSIZE * 2; ++c) {
            roulette[c] = evalfit(ngpool[c]);
            // ������Ӧ�ȵ��ܼ�ֵ
            totalfitness += roulette[c];
        }
        // ѡ��һ��Ⱦɫ��
        ball = rndn(totalfitness);
        acc  = 0;
        for (c = 0; c < POOLSIZE * 2; ++c) {
            acc += roulette[c];     // �ۻ�����ֵ
            if (acc > ball) break;  // ��Ӧ�Ļ���
        }

        // Ⱦɫ��ĸ���
        for (j = 0; j < N; ++j)
            pool[i][j] = ngpool[c][j];
    }
}

// ������ѡ��
int selectp(int roulette[POOLSIZE], int totalfitness) {
    int i;        // ѭ���Ŀ��Ʊ���
    int ball;     // ��ѡ��λ�õ���ֵ��
    int acc = 0;  // ����ֵ���ۻ�ֵ

    ball = rndn(totalfitness);
    for (i = 0; i < POOLSIZE; ++i) {
        acc += roulette[i];     // ����ֵ���ۻ�
        if (acc > ball) break;  // ��Ӧ�Ļ���
    }
    return i;
}

// ����
void mating(int pool[POOLSIZE][N], int ngpool[POOLSIZE * 2][N]) {
    int i;                   // ѭ���Ŀ��Ʊ���
    int totalfitness = 0;    // ����ֵ���ܼ�ֵ
    int roulette[POOLSIZE];  // �������ֵ
    int mama, papa;          // �����Ļ���ĺ���

    // ��������
    for (i = 0; i < POOLSIZE; ++i) {
        roulette[i] = evalfit(pool[i]);
        // ��������ֵ���ܼ�ֵ
        totalfitness += roulette[i];
    }

    // ѡ��ͽ����ѭ��
    for (i = 0; i < POOLSIZE; ++i) {
        do {  // ������ѡ��
            mama = selectp(roulette, totalfitness);
            papa = selectp(roulette, totalfitness);
        } while (mama == papa);  // ɾ���ظ���

        // �ض�2Ⱦɫ��Ľ���
        crossing(pool[mama], pool[papa], ngpool[i * 2], ngpool[i * 2 + 1]);
    }
}

// �ض�2Ⱦɫ��Ľ���
void crossing(int m[], int p[], int c1[], int c2[]) {
    int j;   // ѭ�����Ʊ���
    int cp;  // ����ĵ�

    // ȷ�������
    cp = rndn(N);

    // ����ǰ�벿��
    for (j = 0; j < cp; ++j) {
        c1[j] = m[j];
        c2[j] = p[j];
    }
    // ���ƺ�벿��
    for (; j < N; ++j) {
        c2[j] = m[j];
        c1[j] = p[j];
    }
}

// ��������ֵ
int evalfit(int g[]) {
    int pos;         // ָ������λ��
    int value  = 0;  // ����ֵ
    int weight = 0;  // ����

    // �����������λ���������������ֵ
    for (pos = 0; pos < N; ++pos) {
        weight += parcel[pos][0] * g[pos];
        value += parcel[pos][1] * g[pos];
    }
    // ��������Ĵ���
    if (weight >= WEIGHTLIMIT) value = 0;
    return value;
}

// ������
void printp(int pool[POOLSIZE][N]) {
    int    i, j;                // ѭ���Ŀ��Ʊ���
    int    fitness;             // ����ֵ
    double totalfitness = 0;    // ����ֵ���ܼ�ֵ
    int    elite, bestfit = 0;  // ��Ӣ����Ĵ����ñ���

    for (i = 0; i < POOLSIZE; ++i) {
        for (j = 0; j < N; ++j)
            printf("%1d", pool[i][j]);
        fitness = evalfit(pool[i]);
        printf("\t%d\r\n", fitness);
        if (fitness > bestfit) {  // ��Ӣ��
            bestfit = fitness;
            elite   = i;
        }
        totalfitness += fitness;
    }
    // �����Ӣ�������ֵ
    printf("elite=%d\tbestfit=%d \t", elite, bestfit);
    // ���ƽ������ֵ
    printf("average=%lf\r\n", totalfitness / POOLSIZE);
}

// ���ɳ�ʼȾɫ�弯��
void initpool(int pool[POOLSIZE][N]) {
    int i, j;  // ѭ�����Ʊ���
    for (i = 0; i < POOLSIZE; ++i)
        for (j = 0; j < N; ++j)
            pool[i][j] = rndn(2);
}

// n���������������
int rndn(int l) {
    int rndno;
    while ((rndno = ((double)rand() / RAND_MAX) * l) == l) {}
    return rndno;
}

// ͻ��
void mutation(int ngpool[POOLSIZE * 2][N]) {
    for (int i = 0; i < POOLSIZE * 2; ++i)
        for (int j = 0; j < N; ++j)
            if ((double)rndn(100) / 100.0 <= MRATE)  // ��ת��ͻ��
                ngpool[i][j] = notval(ngpool[i][j]);
}

// ��ֵ�ķ�ת
int notval(int v) {
    return (v == YES) ? NO : YES;
}

int pool[POOLSIZE][N];        // Ⱦɫ�弯��
int ngpool[POOLSIZE * 2][N];  // ��һ��Ⱦɫ�弯��
int generation;               // ���ڵĴ���

void dsp_test(void) {
    srand(SEED);

    // ����ĳ�ʼ��
    // initparcel() ;

    // ���ɳ�ʼȾɫ�弯��
    initpool(pool);

    // ѭ��ֱ����ֹ�Ӵ�
    for (generation = 0; generation < LASTG; ++generation) {
        printf("[%d]\r\n", generation);  // ����
        mating(pool, ngpool);            // ����
        mutation(ngpool);                // ͻ��
        selectng(ngpool, pool);          // ѡ����һ�Ӵ�
        printp(pool);                    // ������
    }
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
    printf("Start\r\n");
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
