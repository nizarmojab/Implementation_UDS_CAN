#ifndef STORED_DATA_TRANSMISSION_FUNCTIONAL_UNIT_H_
#define STORED_DATA_TRANSMISSION_FUNCTIONAL_UNIT_H_

#include <stdint.h>
#include <stdbool.h>
#include "uds_services.h"

// Définition des tailles
#define MAX_DTC_EXT_DATA_RECORD_SIZE 64
#define MAX_DTC_COUNT 255
#define DTC_STATUS_MASK 0xFF

#define UDS_CLEAR_DIAGNOSTIC_INFORMATION 0x14
#define UDS_READ_DTC_INFORMATION         0x19

// Définition des identifiants de service
#define UDS_CLEAR_DIAGNOSTIC_INFORMATION_REQUEST 0x14
#define UDS_CLEAR_DIAGNOSTIC_INFORMATION_RESPONSE 0x54

#define DTC_STATUS_PERMANENT 0x10
#define MAX_RESPONSE_LENGTH 64

#define DTC_TEST_FAILED 0x01     // Indique un DTC échoué
#define DTC_CONFIRMED 0x02       // Indique un DTC confirmé

// Sous-fonctions pour le service ReadDTCInformation (0x19)
#define REPORT_NUMBER_OF_DTC_BY_STATUS_MASK        0x01
#define REPORT_DTC_BY_STATUS_MASK                  0x02
#define REPORT_DTC_SNAPSHOT_IDENTIFICATION         0x03
#define REPORT_DTC_SNAPSHOT_RECORD_BY_DTC_NUMBER   0x04
#define REPORT_DTC_STORED_DATA_BY_RECORD_NUMBER    0x05
#define REPORT_DTC_EXT_DATA_RECORD_BY_DTC_NUMBER   0x06
#define REPORT_NUMBER_OF_DTC_BY_SEVERITY_MASK      0x07
#define REPORT_DTC_BY_SEVERITY_MASK_RECORD         0x08
#define REPORT_SEVERITY_INFORMATION_OF_DTC         0x09
#define REPORT_SUPPORTED_DTC                       0x0A
#define REPORT_FIRST_TEST_FAILED_DTC               0x0B
#define REPORT_FIRST_CONFIRMED_DTC                 0x0C
#define REPORT_MOST_RECENT_TEST_FAILED_DTC         0x0D
#define REPORT_MOST_RECENT_CONFIRMED_DTC           0x0E
#define REPORT_MIRROR_MEMORY_DTC_BY_STATUS_MASK    0x0F
#define REPORT_MIRROR_MEMORY_DTC_EXT_DATA_RECORD   0x10
#define REPORT_NUMBER_OF_MIRROR_MEMORY_DTC_BY_STATUS_MASK 0x11
#define REPORT_NUMBER_OF_EMISSIONS_OBD_DTC_BY_STATUS_MASK 0x12
#define REPORT_EMISSIONS_OBD_DTC_BY_STATUS_MASK    0x13
#define REPORT_DTC_FAULT_DETECTION_COUNTER         0x14
#define REPORT_DTC_WITH_PERMANENT_STATUS           0x15
#define REPORT_DTC_EXT_DATA_RECORD_BY_RECORD_NUMBER 0x16
#define REPORT_USER_DEF_MEMORY_DTC_BY_STATUS_MASK  0x17
#define REPORT_USER_DEF_MEMORY_DTC_SNAPSHOT_RECORD 0x18
#define REPORT_USER_DEF_MEMORY_DTC_EXT_DATA_RECORD 0x19
#define REPORT_WWH_OBD_DTC_BY_MASK_RECORD          0x42
#define REPORT_WWH_OBD_DTC_WITH_PERMANENT_STATUS   0x55



// Structure pour les enregistrements DTC
typedef struct {
      uint32_t dtcNumber;  // Numéro de DTC
      uint8_t status;      // Statut du DTC
      uint8_t severity;    // Gravité du DTC

      // Pour les snapshots DTC
      uint8_t snapshotRecordNumber;       // Numéro d'enregistrement du snapshot
      uint8_t snapshotData[MAX_DTC_EXT_DATA_RECORD_SIZE];  // Données du snapshot
      uint8_t snapshotDataLength;         // Taille des données du snapshot

      // Pour les données stockées DTC (DTCStoredData)
      uint16_t storedDataRecordNumber;    // Numéro d'enregistrement des données stockées
      uint8_t storedData[MAX_DTC_EXT_DATA_RECORD_SIZE];    // Données stockées
      uint8_t storedDataLength;
      uint8_t isEmissionRelated;  // Ajoutez ce champ si nécessaire
      uint8_t faultDetectionCounter;  // Ajoutez également ce champ si nécessaire
} DTC_Record;

// Prototypes des fonctions UDS pour cette unité fonctionnelle

// ClearDiagnosticInformation
void uds_clear_diagnostic_information(uint8_t* data, uint8_t data_length);
bool is_group_of_dtc_supported(uint32_t groupOfDTC);
bool are_conditions_correct_for_dtc_clear(void);
bool clear_diagnostic_information(uint32_t groupOfDTC);
void send_positive_response_clear_diagnostic_information(void);
void send_negative_response_clear_diagnostic_information(uint8_t nrc);

// ReadDTCInformation
void uds_read_dtc_information(uint8_t sub_function, uint8_t* data, uint8_t data_length);
void send_positive_response_read_dtc_information(uint8_t sub_function, DTC_Record* dtcRecords, uint8_t dtcCount);
void send_negative_response_read_dtc_information(uint8_t sub_function, uint8_t nrc);
void report_number_of_dtc_by_status_mask(uint8_t* data, uint8_t data_length);
void report_dtc_by_status_mask(uint8_t* data, uint8_t data_length);
void report_dtc_snapshot_identification(uint8_t* data, uint8_t data_length);
void report_dtc_snapshot_record_by_dtc_number(uint8_t* data, uint8_t data_length);
void report_dtc_stored_data_by_record_number(uint8_t* data, uint8_t data_length);
void report_dtc_ext_data_record_by_dtc_number(uint8_t* data, uint8_t data_length);
void report_number_of_dtc_by_severity_mask(uint8_t* data, uint8_t data_length);
void report_dtc_by_severity_mask_record(uint8_t* data, uint8_t data_length);
void report_severity_information_of_dtc(uint8_t* data, uint8_t data_length);
void report_supported_dtc(uint8_t* data, uint8_t data_length);
void report_first_test_failed_dtc(uint8_t* data, uint8_t data_length);
void report_first_confirmed_dtc(uint8_t* data, uint8_t data_length);
void report_most_recent_test_failed_dtc(uint8_t* data, uint8_t data_length);
void report_most_recent_confirmed_dtc(uint8_t* data, uint8_t data_length);
void report_mirror_memory_dtc_by_status_mask(uint8_t* data, uint8_t data_length);
void report_mirror_memory_dtc_ext_data_record(uint8_t* data, uint8_t data_length);
void report_number_of_mirror_memory_dtc_by_status_mask(uint8_t* data, uint8_t data_length);
void report_number_of_emissions_obd_dtc_by_status_mask(uint8_t* data, uint8_t data_length);
void report_emissions_obd_dtc_by_status_mask(uint8_t* data, uint8_t data_length);
void report_dtc_fault_detection_counter(uint8_t* data, uint8_t data_length);
void report_dtc_with_permanent_status(uint8_t* data, uint8_t data_length);
void report_dtc_ext_data_record_by_record_number(uint8_t* data, uint8_t data_length);
void report_user_def_memory_dtc_by_status_mask(uint8_t* data, uint8_t data_length);
void report_user_def_memory_dtc_snapshot_record(uint8_t* data, uint8_t data_length);
void report_user_def_memory_dtc_ext_data_record(uint8_t* data, uint8_t data_length);
void report_wwh_obd_dtc_by_mask_record(uint8_t* data, uint8_t data_length);
void report_wwh_obd_dtc_with_permanent_status(uint8_t* data, uint8_t data_length);
uint8_t get_dtc_status_availability_mask(void);
uint8_t get_dtc_format_identifier(void);
uint8_t get_functional_group_identifier(void);
uint8_t get_dtc_severity_availability_mask(void);

#endif /* STORED_DATA_TRANSMISSION_FUNCTIONAL_UNIT_H_ */
