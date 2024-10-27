#ifndef DATA_TRANSMISSION_FUNCTIONAL_UNIT_H_
#define DATA_TRANSMISSION_FUNCTIONAL_UNIT_H_
#include "uds_services.h"
#include <stdint.h>
#include <stdbool.h>


extern void send_can_message(uint8_t *message, uint8_t length);
// extern void send_uart_message(uint8_t *message, uint8_t length);
// Définitions des tailles
#define MAX_DATA_SIZE 64
#define MAX_PERIODIC_PIDS 16
#define MAX_DYNAMIC_DIDS 10
#define MAX_DATA_IDENTIFIERS 10

// Identifiants de Service UDS pour cette unité fonctionnelle
#define UDS_READ_DATA_BY_IDENTIFIER                0x22
#define UDS_READ_MEMORY_BY_ADDRESS                 0x23
#define UDS_READ_SCALING_DATA_BY_IDENTIFIER        0x24
#define UDS_READ_DATA_BY_PERIODIC_IDENTIFIER       0x2A
#define UDS_DYNAMICAL_DEFINE_DATA_IDENTIFIER       0x2C
#define UDS_WRITE_DATA_BY_IDENTIFIER               0x2E
#define UDS_WRITE_MEMORY_BY_ADDRESS                0x3D

// Sous-fonctions pour DynamicallyDefineDataIdentifier
#define UDS_DDDI_DEFINE_BY_IDENTIFIER              0x01
#define UDS_DDDI_DEFINE_BY_MEMORY_ADDRESS          0x02
#define UDS_DDDI_CLEAR_DYNAMIC_IDENTIFIER          0x03

// Définition des transmissionModes
#define UDS_TRANSMISSION_MODE_SLOW      0x01
#define UDS_TRANSMISSION_MODE_MEDIUM    0x02
#define UDS_TRANSMISSION_MODE_FAST      0x03
#define UDS_TRANSMISSION_MODE_STOP      0x04

// Structure pour stocker les informations sur les PIDs périodiques
typedef struct {
    uint8_t periodicDataIdentifier;  // Identifiant PID
    uint8_t transmissionMode;        // Mode de transmission (slow, medium, fast)
    bool isActive;                   // Indicateur d'activité
} PeriodicPIDInfo;

// Structure pour les Data Identifiers
typedef struct {
    uint16_t dataIdentifier;
    uint8_t dataRecord[MAX_DATA_SIZE];  // Données associées à l'identifiant
    uint8_t dataLength;                 // Longueur des données
    bool isSecured;                     // Indicateur de protection (si sécurisé)
    bool isWritable;                    // Indicateur si l'identifiant est accessible en écriture
} DataIdentifier;

// Structures pour DynamicallyDefineDataIdentifier
typedef struct {
    uint16_t dynamicDataIdentifier;
    uint16_t sourceDataIdentifier;
    uint8_t positionInSource;
    uint8_t memorySize;
} DynamicallyDefinedIdentifier;

extern PeriodicPIDInfo periodic_pid_list[MAX_PERIODIC_PIDS];

// Prototypes des fonctions UDS pour cette unité fonctionnelle

void uds_read_data_by_identifier(uint8_t* data, uint8_t data_length);
bool is_security_required_for_did(uint16_t did);
void send_positive_response_read_data_by_identifier(uint8_t* dataIdentifiers, uint8_t* dataRecords, uint8_t number_of_dids);
void send_negative_response_read_data_by_identifier(uint8_t nrc);

void uds_read_memory_by_address(uint8_t* data, uint8_t data_length);
void send_positive_response_read_memory_by_address(uint8_t* dataRecord, uint8_t data_length);
void send_negative_response_read_memory_by_address(uint8_t nrc);

void uds_read_data_by_periodic_identifier(uint8_t* data, uint8_t data_length);
void send_positive_response_read_data_by_periodic_identifier(void);
void send_negative_response_read_data_by_periodic_identifier(uint8_t nrc);
void start_periodic_transmission(uint8_t pid, uint8_t mode);
void stop_periodic_transmission(uint8_t pid);
void stop_all_periodic_transmissions(void);
bool is_security_granted_for_pid(uint8_t pid);
bool is_pid_supported_in_session(uint8_t pid);

void uds_dynamically_define_data_identifier(uint8_t sub_function, uint8_t *data, uint8_t data_length);
void define_by_identifier(uint8_t *data, uint8_t data_length);
void define_by_memory_address(uint8_t *data, uint8_t data_length);
void clear_dynamically_defined_data_identifier(uint16_t dynamicDataIdentifier);
void send_positive_response_dynamically_define_data_identifier(uint8_t sub_function, uint16_t dynamic_did);
void send_negative_response_dynamically_define_data_identifier(uint8_t sub_function, uint8_t nrc);
bool is_dynamic_identifier_supported(uint16_t did);

void uds_write_data_by_identifier(uint8_t* data, uint8_t data_length);
void send_positive_response_write_data_by_identifier(uint16_t dataIdentifier);
void send_negative_response_write_data_by_identifier(uint8_t nrc);
bool write_data_to_identifier(uint16_t dataIdentifier, uint8_t* dataRecord, uint8_t dataLength);
bool is_data_identifier_supported_for_write(uint16_t dataIdentifier);
bool are_conditions_correct_for_did(uint16_t dataIdentifier);

#endif /* DATA_TRANSMISSION_FUNCTIONAL_UNIT_H_ */
