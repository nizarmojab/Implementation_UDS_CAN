#ifndef INPUT_OUTPUT_CONTROL_FUNCTIONAL_UNIT_H_
#define INPUT_OUTPUT_CONTROL_FUNCTIONAL_UNIT_H_

#include <stdint.h>
#include <stdbool.h>
#include "uds_services.h"

// Taille maximale des données pour le contrôle des entrées/sorties
#define MAX_IO_CONTROL_DATA_SIZE 8

// Identifiants de Service UDS
#define UDS_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER 0x2F
#define UDS_RESPONSE_INPUT_OUTPUT_CONTROL 0x6F

// Paramètres de contrôle d'entrée/sortie
#define IOCP_SHORT_TERM_ADJUSTMENT 0x03
#define IOCP_RETURN_CONTROL_TO_ECU 0x00
#define IOCP_FREEZE_CURRENT_STATE 0x02

// Structure pour la requête InputOutputControlByIdentifier
typedef struct {
    uint8_t SID;                  // Identifiant de service
    uint16_t dataIdentifier;       // Identifiant de données (DID)
    uint8_t controlOptionRecord;   // Option de contrôle (IOCP)
    uint8_t controlState[MAX_IO_CONTROL_DATA_SIZE];  // État de contrôle (maximum 8 octets)
} IOControlRequest_t;

// Structure pour la réponse InputOutputControlByIdentifier
typedef struct {
    uint8_t SID;                   // Identifiant de service de réponse
    uint16_t dataIdentifier;        // Identifiant de données (DID)
    uint8_t controlStatusRecord[MAX_IO_CONTROL_DATA_SIZE];  // Statut du contrôle (maximum 8 octets)
} IOControlResponse_t;

// Prototypes des fonctions de contrôle d'entrée/sortie
void uds_input_output_control_by_identifier(IOControlRequest_t *request, IOControlResponse_t *response);
void send_positive_response_input_output_control_by_identifier(uint16_t dataIdentifier, uint8_t controlStatus, uint8_t *controlState);
void send_negative_response_input_output_control_by_identifier(uint8_t nrc);
void handle_input_output_control_response(IOControlResponse_t *response);


#endif /* INPUT_OUTPUT_CONTROL_FUNCTIONAL_UNIT_H_ */
