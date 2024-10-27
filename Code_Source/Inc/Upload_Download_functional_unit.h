#ifndef UPLOAD_DOWNLOAD_FUNCTIONAL_UNIT_H_
#define UPLOAD_DOWNLOAD_FUNCTIONAL_UNIT_H_

#include <stdint.h>
#include <stdbool.h>
#include "uds_services.h"

// Identifiants des services Upload/Download UDS
#define UDS_REQUEST_DOWNLOAD         0x34
#define UDS_REQUEST_UPLOAD           0x35
#define UDS_TRANSFER_DATA            0x36
#define UDS_REQUEST_TRANSFER_EXIT    0x37
#define UDS_REQUEST_FILE_TRANSFER    0x38
#define UDS_RESPONSE_REQUEST_DOWNLOAD    0x74
#define UDS_RESPONSE_TRANSFER_EXIT       0x77



typedef struct {
    uint8_t dataFormatIdentifier;                // Identifiant de format de données
    uint8_t addressAndLengthFormatIdentifier;    // Identifiant de format d'adresse et de longueur
    uint8_t memoryAddress[4];                     // Adresse mémoire (max 4 octets)
    uint8_t memorySize[4];                        // Taille mémoire (max 4 octets)
} RequestDownload_t;

// Structure pour la réponse de téléchargement
typedef struct {
    uint8_t lengthFormatIdentifier;               // Identifiant de format de longueur
    uint8_t maxNumberOfBlockLength[2];           // Longueur maximale des blocs
} ResponseDownload_t;

// Définition des Structures
typedef struct {
    uint8_t dataFormatIdentifier; // DFI
    uint8_t addressAndLengthFormatIdentifier; // ALFID
    uint8_t memoryAddress[4]; // Peut être ajusté selon la taille
    uint8_t memorySize[4]; // Peut être ajusté selon la taille
} RequestUpload_t;

typedef struct {
    uint8_t lengthFormatIdentifier; // LFID
    uint8_t maxNumberOfBlockLength[2]; // Peut être ajusté selon la taille
} ResponseUpload_t;

typedef struct {
    uint8_t blockSequenceCounter; // BSC
    uint8_t transferRequestParameterRecord[255]; // Paramètres de requête (ajuster selon les besoins)
} RequestTransferData_t;

typedef struct {
    uint8_t blockSequenceCounter; // BSC
    uint8_t transferResponseParameterRecord[255]; // Paramètres de réponse (ajuster selon les besoins)
} ResponseTransferData_t;

// Structure pour la requête RequestTransferExit
typedef struct {
    uint8_t transferRequestParameterRecord[256]; // Taille arbitraire, ajustez selon vos besoins
} RequestTransferExit_t;

// Structure pour la réponse au service
typedef struct {
    uint8_t transferResponseParameterRecord[256]; // Taille arbitraire, ajustez selon vos besoins
} ResponseTransferExit_t;

typedef struct {
    uint8_t modeOfOperation;             // 0x01 - 0x05 : Mode d'opération
    uint16_t filePathAndNameLength;      // Longueur du chemin du fichier
    uint8_t filePathAndName[255];        // Chemin du fichier (jusqu'à 255 octets)
    uint8_t dataFormatIdentifier;         // Identifiant du format de données
    uint16_t fileSizeParameterLength;     // Longueur du paramètre de taille de fichier
    uint8_t fileSizeUncompressed[4];      // Taille non compressée (4 octets)
    uint8_t fileSizeCompressed[4];        // Taille compressée (4 octets)
} RequestFileTransfer_t;


typedef struct {
    uint8_t SID; // Identifiant de service
    uint8_t modeOfOperation; // Écho du mode d'opération
    uint8_t lengthFormatIdentifier; // Longueur du maxNumberOfBlockLength
    uint8_t maxNumberOfBlockLength[2]; // Longueur maximale du bloc
    uint8_t dataFormatIdentifier; // Écho du format de données
    uint16_t fileSizeOrDirInfoParameterLength; // Longueur du paramètre
    uint8_t fileSizeUncompressedOrDirInfoLength[4]; // Taille du fichier
    uint8_t fileSizeCompressed[4]; // Taille compressée
} ResponseFileTransfer_t;


void uds_request_download(RequestDownload_t *request);
void send_positive_response_request_download(ResponseDownload_t *response);
void send_negative_response_request_download(uint8_t nrc);
bool is_memory_address_valid(uint8_t *address);
bool is_memory_size_valid(uint8_t *size);
bool is_security_active(void);

void uds_request_upload(RequestUpload_t *request);
void send_positive_response_upload(uint8_t *maxNumberOfBlockLength);
void send_negative_response_upload(uint8_t nrc);
bool is_security_active(void);
bool validate_request_upload(RequestUpload_t *request);

void uds_transfer_data(RequestTransferData_t *request);
void send_positive_response_transfer_data(uint8_t blockSequenceCounter);
void send_negative_response_transfer_data(uint8_t nrc);
bool is_request_download_active(void);
bool is_request_upload_active(void);
bool validate_transfer_data_request(RequestTransferData_t *request);

void uds_request_transfer_exit(RequestTransferExit_t *request, ResponseTransferExit_t *response);
void send_positive_response_transfer_exit(ResponseTransferExit_t *response);
void send_negative_response_transfer_exit(uint8_t nrc);
bool is_transfer_in_progress(void);
bool validate_transfer_request_parameters(RequestTransferExit_t *request);

void uds_request_file_transfer(RequestFileTransfer_t *request);
void send_positive_response_file_transfer(ResponseFileTransfer_t *response);
void send_negative_response_file_transfer(uint8_t nrc);

void uds_request_download(RequestDownload_t *request);
void send_positive_response_request_download(ResponseDownload_t *response);
void send_negative_response_request_download(uint8_t nrc);
bool is_memory_address_valid(uint8_t *address);
bool is_memory_size_valid(uint8_t *size);
bool is_security_active(void);

void uds_request_upload(RequestUpload_t *request);
void send_positive_response_upload(uint8_t *maxNumberOfBlockLength);
void send_negative_response_upload(uint8_t nrc);
bool is_security_active(void);
bool validate_request_upload(RequestUpload_t *request);

void uds_transfer_data(RequestTransferData_t *request);
void send_positive_response_transfer_data(uint8_t blockSequenceCounter);
void send_negative_response_transfer_data(uint8_t nrc);
bool is_request_download_active(void);
bool is_request_upload_active(void);
bool validate_transfer_data_request(RequestTransferData_t *request);

void uds_request_transfer_exit(RequestTransferExit_t *request, ResponseTransferExit_t *response);
void send_positive_response_transfer_exit(ResponseTransferExit_t *response);
void send_negative_response_transfer_exit(uint8_t nrc);
bool is_transfer_in_progress(void);
bool validate_transfer_request_parameters(RequestTransferExit_t *request);

void uds_request_file_transfer(RequestFileTransfer_t *request);
void send_positive_response_file_transfer(ResponseFileTransfer_t *response);
void send_negative_response_file_transfer(uint8_t nrc);

#endif /* UPLOAD_DOWNLOAD_FUNCTIONAL_UNIT_H_ */
