/*
 * it.c
 *
 *      Author: Nizar Mojab
 */

#include "main.h"

extern CAN_HandleTypeDef hcan1;


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

/**
  * @brief This function handles CAN_TX interrupts.
  */
void CAN1_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}

/**
  * @brief This function handles CAN_RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}

/**
  * @brief This function handles CAN SCE interrupt.
  */
void CAN1_SCE_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}








