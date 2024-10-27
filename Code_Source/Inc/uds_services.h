#ifndef UDS_SERVICES_H_
#define UDS_SERVICES_H_

#include <stdint.h>
#include <stdbool.h>

void Error_handler(void);


// Codes de réponse négative (Negative Response Code - NRC)
#define UDS_NEGATIVE_RESPONSE                0x7F
#define NRC_SERVICE_NOT_SUPPORTED            0x11
#define NRC_SUB_FUNCTION_NOT_SUPPORTED       0x12
#define NRC_INCORRECT_MESSAGE_LENGTH         0x13
#define NRC_RESPONSE_TOO_LONG                0x14
#define NRC_CONDITIONS_NOT_CORRECT           0x22
#define NRC_SECURITY_ACCESS_DENIED           0x33
#define NRC_REQUEST_SEQUENCE_ERROR           0x24
#define NRC_REQUEST_OUT_OF_RANGE             0x31
#define NRC_INVALID_KEY                      0x35
#define NRC_EXCEEDED_NUMBER_OF_ATTEMPTS      0x36
#define NRC_REQUIRED_TIME_DELAY_NOT_EXPIRED  0x37
#define NRC_SECURITY_TRANSMISSION_ERROR      0x50
#define NRC_GENERAL_PROGRAMMING_FAILURE      0x72
#define NRC_UPLOAD_DOWNLOAD_NOT_ACCEPTED     0x70
#define NRC_WRONG_BLOCK_SEQUENCE_COUNTER     0x73






#include "Diagnostic_Communication_Management_functional_unit.h"
#include "Data_Transmission_functional_unit.h"
#include "Stored_Data_Transmission_functional_unit.h"
#include "InputOutput_Control_functional_unit.h"
#include "Routine_functional_unit.h"
#include "Upload_Download_functional_unit.h"
#endif /* UDS_SERVICES_H_ */
