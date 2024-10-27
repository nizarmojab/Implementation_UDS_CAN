/*
 * main.c
 *
 *      Author: Nizar Mojab
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "main.h"
#include "uds_services.h"

/* Private function prototypes -----------------------------------------------*/
void GPIO_Init(void);
void Error_handler(void);
void UART2_Init(void);
void CAN1_Init(void);
void CAN_Filter_Config(void);
void SystemClock_Config_HSE(uint8_t clock_freq);

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
CAN_HandleTypeDef hcan1;
CAN_RxHeaderTypeDef RxHeader;

int main(void)
{
    HAL_Init();
    SystemClock_Config_HSE(SYS_CLOCK_FREQ_84_MHZ);
    GPIO_Init();
    UART2_Init();
    CAN1_Init();
    CAN_Filter_Config();

    // Envoyer un message de test via UART
    const char *test_msg = "UART Initialized Successfully!\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)test_msg, strlen(test_msg), HAL_MAX_DELAY);

    // Activer les notifications pour les interruptions CAN
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
    {
        Error_handler();
    }

    // Démarrer le module CAN
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        Error_handler();
    }

    while (1)
    {
        // L'application principale peut effectuer d'autres tâches si nécessaire
    }

    return 0;
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config_HSE(uint8_t clock_freq)
{
    RCC_OscInitTypeDef Osc_Init;
    RCC_ClkInitTypeDef Clock_Init;
    uint8_t flash_latency = 0;

    Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    Osc_Init.HSEState = RCC_HSE_ON;
    Osc_Init.PLL.PLLState = RCC_PLL_ON;
    Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    switch (clock_freq)
    {
        case SYS_CLOCK_FREQ_84_MHZ:
            Osc_Init.PLL.PLLM = 4;
            Osc_Init.PLL.PLLN = 84;
            Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
            flash_latency = 2;
            break;

        // Ajoutez d'autres cas si nécessaire

        default:
            return;
    }

    if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
    {
        Error_handler();
    }

    if (HAL_RCC_ClockConfig(&Clock_Init, flash_latency) != HAL_OK)
    {
        Error_handler();
    }

    // Configure le systick timer
    uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
    HAL_SYSTICK_Config(hclk_freq / 1000);
}

/**
 * @brief CAN Initialization Function
 * @param None
 * @retval None
 */
void CAN1_Init(void)
{
    hcan1.Instance = CAN1;
    hcan1.Init.Mode = CAN_MODE_NORMAL;
    hcan1.Init.AutoBusOff = ENABLE;
    hcan1.Init.AutoRetransmission = ENABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    // Settings related to CAN bit timings
    hcan1.Init.Prescaler = 3;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_11TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;

    if (HAL_CAN_Init(&hcan1) != HAL_OK)
    {
        Error_handler();
    }
}

/**
 * @brief Configures the CAN filter.
 * @retval None
 */
void CAN_Filter_Config(void)
{
    CAN_FilterTypeDef can1_filter_init;

    can1_filter_init.FilterActivation = ENABLE;
    can1_filter_init.FilterBank = 0;
    can1_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
    can1_filter_init.FilterIdHigh = 0x0000;
    can1_filter_init.FilterIdLow = 0x0000;
    can1_filter_init.FilterMaskIdHigh = 0X01C0;
    can1_filter_init.FilterMaskIdLow = 0x0000;
    can1_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
    can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;

    if (HAL_CAN_ConfigFilter(&hcan1, &can1_filter_init) != HAL_OK)
    {
        Error_handler();
    }
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef ledgpio;
    ledgpio.Pin = GPIO_PIN_5;
    ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
    ledgpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &ledgpio);

    // Configurez d'autres GPIO si nécessaire
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
void UART2_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_handler(); // Gestion d'erreur si l'initialisation échoue
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_handler(void)
{
    while (1)
    {
        // Boucle infinie en cas d'erreur
    }
}

/* Function to send messages via UART */
void UART2_Send(const char* message)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}

/* Interrupt Callbacks */

/**
 * @brief  Rx FIFO 0 message pending callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t rcvd_msg[8];
    uint8_t response[3];
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, rcvd_msg) == HAL_OK)
    {
        // Construction du message à envoyer via UART
        char msg[100];
        sprintf(msg, "Message CAN reçu: ID=0x%lX, Data=[%d, %d, %d, %d, %d, %d, %d, %d]\n",
                (unsigned long)RxHeader.StdId, rcvd_msg[0], rcvd_msg[1], rcvd_msg[2], rcvd_msg[3], rcvd_msg[4], rcvd_msg[5], rcvd_msg[6], rcvd_msg[7]);

        // Envoyer le message UART
        UART2_Send(msg);

        // Identifier le service UDS basé sur le premier octet du message
        switch (rcvd_msg[0]) {
            case UDS_DIAGNOSTIC_SESSION_CONTROL:
                // Appeler la fonction pour le service Diagnostic Session Control
                uds_diagnostic_session_control(rcvd_msg[1]);
                break;

            case UDS_ECU_RESET:
                // Appeler la fonction pour le service ECU Reset avec le resetType
                uds_ecu_reset(rcvd_msg[1]);
                break;
            case UDS_SECURITY_ACCESS:
                // Appeler directement la fonction pour le service Security Access
                uds_security_access(rcvd_msg[1], &rcvd_msg[2], RxHeader.DLC - 2);
                break;
            case UDS_COMMUNICATION_CONTROL:
                uds_communication_control(rcvd_msg[1]);
                break;
            case UDS_TESTER_PRESENT:
                // Appeler la fonction pour le service TesterPresent
                uds_tester_present(rcvd_msg[1]);
                break;
            case UDS_ACCESS_TIMING_PARAMETER:
                // Appeler la fonction pour le service Access Timing Parameter
                uds_access_timing_parameter(rcvd_msg[1], &rcvd_msg[2], RxHeader.DLC - 2);
                break;
            case UDS_SECURED_DATA_TRANSMISSION:
                // Appeler la fonction pour le service Secured Data Transmission
                uds_secured_data_transmission(&rcvd_msg[1], RxHeader.DLC - 1);
                break;
            case UDS_CONTROL_DTC_SETTING:
                // Appeler la fonction pour le service ControlDTCSetting
                uds_control_dtc_setting(rcvd_msg[1]);
                break;
            case UDS_RESPONSE_ON_EVENT:
                // Appeler la fonction pour le service ResponseOnEvent
                uds_response_on_event(rcvd_msg[1], &rcvd_msg[2], RxHeader.DLC - 2);
                break;
            case UDS_LINK_CONTROL:
                // Appeler la fonction pour le service LinkControl
                uds_link_control(rcvd_msg[1], &rcvd_msg[2], RxHeader.DLC - 2);
                break;
            case UDS_READ_DATA_BY_IDENTIFIER:
                // Appeler la fonction pour le service ReadDataByIdentifier
                uds_read_data_by_identifier(&rcvd_msg[1], RxHeader.DLC - 1);
                break;
            case UDS_READ_DATA_BY_PERIODIC_IDENTIFIER:
                // Appeler la fonction pour le service ReadDataByPeriodicIdentifier
                uds_read_data_by_periodic_identifier(&rcvd_msg[1], RxHeader.DLC - 1);
                break;
            case UDS_DYNAMICAL_DEFINE_DATA_IDENTIFIER:
                // Appeler la fonction pour le service DynamicallyDefineDataIdentifier
                uds_dynamically_define_data_identifier(rcvd_msg[1], &rcvd_msg[2], RxHeader.DLC - 2);
                break;
            case UDS_WRITE_DATA_BY_IDENTIFIER:
                // Appeler la fonction pour le service WriteDataByIdentifier
                uds_write_data_by_identifier(&rcvd_msg[1], RxHeader.DLC - 1);
                break;
            case UDS_CLEAR_DIAGNOSTIC_INFORMATION:
                // Appeler la fonction pour le service ClearDiagnosticInformation
                uds_clear_diagnostic_information(&rcvd_msg[1], RxHeader.DLC - 1);
                break;
            case UDS_READ_DTC_INFORMATION:
                // Appeler la fonction pour gérer le service ReadDTCInformation
                uds_read_dtc_information(rcvd_msg[1], &rcvd_msg[2], RxHeader.DLC - 2);
                break;
            case UDS_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER:
                uds_input_output_control_by_identifier((IOControlRequest_t *)&rcvd_msg[1], NULL);
                break;
            case UDS_ROUTINE_CONTROL:
                uds_routine_control((RoutineControlRequest_t *)&rcvd_msg[1], NULL);
                break;
            case UDS_REQUEST_DOWNLOAD:
                uds_request_download((RequestDownload_t *)&rcvd_msg[1]);
                break;
            case UDS_REQUEST_UPLOAD:
                uds_request_upload((RequestUpload_t *)&rcvd_msg[1]);
                break;
            case UDS_TRANSFER_DATA:
                uds_transfer_data((RequestTransferData_t *)&rcvd_msg[1]);
                break;
            case UDS_REQUEST_TRANSFER_EXIT:
                uds_request_transfer_exit((RequestTransferExit_t *)&rcvd_msg[1], NULL);
                break;
            case UDS_REQUEST_FILE_TRANSFER:
                uds_request_file_transfer((RequestFileTransfer_t *)&rcvd_msg[1]);
                 break;

            default:
              // Remplir le message de réponse négative pour un service non supporté
              response[0] = UDS_NEGATIVE_RESPONSE;         // Réponse négative générique
              response[1] = rcvd_msg[0];  // Service non supporté
              response[2] = NRC_SERVICE_NOT_SUPPORTED;         // Code NRC (ServiceNotSupported)

              // Envoyer le message de réponse négative via CAN
              send_can_message(response, 3);
             break;
        }
    }
}

/**
 * @brief  Transmission Mailbox 0 complete callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    char msg[50];
    sprintf(msg, "Message Transmitted:M0\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}
