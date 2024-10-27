/*
 * Upload_Download_functional_unit.c
 *
 *  Created on: 21 oct. 2024
 *      Author: PC
 */
#include "uds_services.c"
#include "Upload_Download_functional_unit.h"
/*******************************************************Request_download******************************************************/
// Fonction pour traiter la demande de téléchargement
void uds_request_download(RequestDownload_t *request) {
    // 1. Vérification de la longueur minimale
    if (sizeof(*request) < sizeof(request->dataFormatIdentifier) + sizeof(request->addressAndLengthFormatIdentifier) + 4) {
        send_negative_response_request_download(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // 2. Vérification de l'identifiant de format de données
    if (request->dataFormatIdentifier != 0x00) { // Exemple de vérification
        send_negative_response_request_download(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // 3. Vérification de l'identifiant de format d'adresse et de longueur
    if (!is_memory_address_valid(request->memoryAddress) || !is_memory_size_valid(request->memorySize)) {
        send_negative_response_request_download(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // 4. Vérification de la sécurité (si applicable)
    if (is_security_active()) { // Implémentez is_security_active()
        send_negative_response_request_download(NRC_SECURITY_ACCESS_DENIED);
        return;
    }

    // Préparer la réponse positive
    ResponseDownload_t response;
    response.lengthFormatIdentifier = 0x74; // Identifiant de réponse
    response.maxNumberOfBlockLength[0] = 0x00; // Remplacez par la logique appropriée
    response.maxNumberOfBlockLength[1] = 0xFF; // Remplacez par la logique appropriée

    // Envoyer la réponse positive
    send_positive_response_request_download(&response);
}

// Fonction pour envoyer une réponse positive
void send_positive_response_request_download(ResponseDownload_t *response) {
    // Envoi du message via CAN
    send_can_message((uint8_t *)response, sizeof(ResponseDownload_t));
    // send_uart_message((uint8_t *)response, sizeof(ResponseDownload_t));
}

// Fonction pour envoyer une réponse négative
void send_negative_response_request_download(uint8_t nrc) {
    uint8_t response[2];
    response[0] = UDS_RESPONSE_REQUEST_DOWNLOAD; // SID de réponse
    response[1] = nrc; // NRC
    send_can_message(response, sizeof(response));
    //send_uart_message(response, sizeof(response));
}

// Fonction pour vérifier la validité de l'adresse mémoire
bool is_memory_address_valid(uint8_t *address) {
    // Implémentez votre logique pour vérifier la validité de l'adresse
    return true; // Remplacez par la logique appropriée
}

// Fonction pour vérifier la validité de la taille mémoire
bool is_memory_size_valid(uint8_t *size) {
    // Implémentez votre logique pour vérifier la validité de la taille
    return true; // Remplacez par la logique appropriée
}
/*******************************************************RequestUpload******************************************************/

// Fonction pour gérer la requête RequestUpload
void uds_request_upload(RequestUpload_t *request) {
    // 1. Vérification de la longueur du message
    if (sizeof(*request) < sizeof(request->dataFormatIdentifier) + sizeof(request->addressAndLengthFormatIdentifier)) {
        send_negative_response_upload(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // 2. Vérification de l'identifiant et du format de données
    if (request->dataFormatIdentifier != 0x35) { // Exemple de validation
        send_negative_response_upload(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // 3. Vérification de la sécurité
    if (is_security_active()) {
        send_negative_response_upload(NRC_SECURITY_ACCESS_DENIED);
        return;
    }

    // 4. Validation des données dans memoryAddress et memorySize
    // Implémentez votre logique pour vérifier si l'adresse et la taille sont valides

    // 5. Envoyer une réponse positive
    uint8_t maxNumberOfBlockLength[2] = {0x00, 0xFF}; // Exemple
    send_positive_response_upload(maxNumberOfBlockLength);
}

// Fonction pour envoyer une réponse positive
void send_positive_response_upload(uint8_t *maxNumberOfBlockLength) {
    ResponseUpload_t response;
    response.lengthFormatIdentifier = 0x75; // LFID
    response.maxNumberOfBlockLength[0] = maxNumberOfBlockLength[0];
    response.maxNumberOfBlockLength[1] = maxNumberOfBlockLength[1];

    // Envoyer le message via le protocole CAN ou autre méthode appropriée
    send_can_message((uint8_t*)&response, sizeof(response));
    //send_uart_message((uint8_t*)&response, sizeof(response));
}

// Fonction pour envoyer une réponse négative
void send_negative_response_upload(uint8_t nrc) {
    uint8_t response[2];
    response[0] = 0x35; // SID de réponse
    response[1] = nrc;

    // Envoyer le message d'erreur
    send_can_message(response, sizeof(response));
    //send_uart_message(response, sizeof(response));
}

// Exemple de fonction pour vérifier si le niveau de sécurité est actif
bool is_security_active(void) {
    // Logique pour vérifier si la sécurité est active
    return false; // Remplacez par votre logique
}

// Fonction pour valider la requête
bool validate_request_upload(RequestUpload_t *request) {
    // Implémentez la logique de validation
    return true; // Remplacez par votre logique
}

/*******************************************************TransferData******************************************************/
#include "uds_services.h"

// Fonction pour gérer la requête TransferData
void uds_transfer_data(RequestTransferData_t *request) {
    // 1. Vérification de la longueur du message
    if (sizeof(*request) < sizeof(request->blockSequenceCounter)) {
        send_negative_response_transfer_data(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // 2. Vérification si une requête de téléchargement ou d'upload est active
    if (!is_request_download_active() && !is_request_upload_active()) {
        send_negative_response_transfer_data(NRC_REQUEST_SEQUENCE_ERROR);
        return;
    }

    // 3. Vérification de la validité du blockSequenceCounter
    static uint8_t lastBlockSequenceCounter = 0;
    if (request->blockSequenceCounter != (lastBlockSequenceCounter + 1) % 256) {
        send_negative_response_transfer_data(NRC_WRONG_BLOCK_SEQUENCE_COUNTER);
        return;
    }

    // 4. Traitement des données transférées
    // Si c'est un téléchargement, écrivez les données dans la mémoire
    // Si c'est un upload, lisez les données à partir de la mémoire
    // Exemple (à adapter selon vos besoins) :
    // write_data_to_memory(request->transferRequestParameterRecord);

    // 5. Mettez à jour le dernier blockSequenceCounter utilisé
    lastBlockSequenceCounter = request->blockSequenceCounter;

    // 6. Envoyer une réponse positive
    send_positive_response_transfer_data(request->blockSequenceCounter);
}

// Fonction pour envoyer une réponse positive
#include <string.h> // Inclure pour memcpy

void send_positive_response_transfer_data(uint8_t blockSequenceCounter) {
    ResponseTransferData_t response;
    response.blockSequenceCounter = blockSequenceCounter;

    // Remplir response.transferResponseParameterRecord si nécessaire
    // Exemple :
    // response.transferResponseParameterRecord[0] = ...;

    // Calculez la taille totale du message à envoyer
    size_t responseSize = sizeof(response.blockSequenceCounter) +
                          sizeof(response.transferResponseParameterRecord); // Ajoutez la taille de tous les membres

    // Envoyer le message via le protocole CAN ou autre méthode appropriée
    send_can_message((uint8_t*)&response, responseSize);
    // send_uart_message((uint8_t*)&response, responseSize);
}

// Fonction pour envoyer une réponse négative
void send_negative_response_transfer_data(uint8_t nrc) {
    uint8_t response[2];
    response[0] = 0x36; // SID de réponse
    response[1] = nrc;

    // Envoyer le message d'erreur
    send_can_message(response, sizeof(response));
    //send_uart_message(response, sizeof(response));
}

// Exemple de vérification d'activité des requêtes
bool is_request_download_active(void) {
    // Logique pour vérifier si une requête de téléchargement est active
    return false; // Remplacez par votre logique
}

bool is_request_upload_active(void) {
    // Logique pour vérifier si une requête d'upload est active
    return false; // Remplacez par votre logique
}

// Fonction pour valider la requête de transfert de données
bool validate_transfer_data_request(RequestTransferData_t *request) {
    // Implémentez la logique de validation
    return true; // Remplacez par votre logique
}

/*******************************************************RequestTransferExit*************************************************/
// Fonction pour gérer la requête RequestTransferExit
void uds_request_transfer_exit(RequestTransferExit_t *request, ResponseTransferExit_t *response) {
    // 1. Vérification de la longueur du message
    if (sizeof(*request) < sizeof(request->transferRequestParameterRecord)) {
        send_negative_response_transfer_exit(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // 2. Vérification de l'état de la séquence de demande
    if (!is_transfer_in_progress()) {
        send_negative_response_transfer_exit(NRC_REQUEST_SEQUENCE_ERROR);
        return;
    }

    // 3. Validation des données dans transferRequestParameterRecord
    if (!validate_transfer_request_parameters(request)) {
        send_negative_response_transfer_exit(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // 4. Logique pour finaliser la requête de transfert
    // (Ajoutez ici la logique nécessaire pour finaliser le transfert de données)

    // 5. Préparation de la réponse positive
    memset(response, 0, sizeof(ResponseTransferExit_t)); // Réinitialiser la réponse
    // Remplir response.transferResponseParameterRecord si nécessaire
    // Exemple : response.transferResponseParameterRecord[0] = ...;

    // Envoyer la réponse positive
    send_positive_response_transfer_exit(response);
}

// Fonction pour envoyer une réponse positive
void send_positive_response_transfer_exit(ResponseTransferExit_t *response) {
    // Envoi du message via le protocole CAN ou autre méthode appropriée
    size_t response_size = sizeof(ResponseTransferExit_t); // Taille de la réponse
    send_can_message((uint8_t*)response, response_size);
}

// Fonction pour envoyer une réponse négative
void send_negative_response_transfer_exit(uint8_t nrc) {
    // Envoi d'un message d'erreur par CAN
    uint8_t response[2];
    response[0] = UDS_RESPONSE_TRANSFER_EXIT; // SID de réponse
    response[1] = nrc; // NRC

    send_can_message(response, sizeof(response));
    //send_uart_message(response, sizeof(response));
}

// Fonction pour vérifier si un transfert est en cours
bool is_transfer_in_progress(void) {
    // Implémentez votre logique pour vérifier si un transfert est actif
    return true; // Remplacez ceci par la logique appropriée
}

// Fonction pour valider les paramètres de la requête de transfert
bool validate_transfer_request_parameters(RequestTransferExit_t *request) {
    // Implémentez votre logique pour valider les paramètres
    return true; // Remplacez ceci par la logique appropriée
}

/*******************************************************RequestFileTransfer*************************************************/
// Fonction pour traiter la requête RequestFileTransfer
void uds_request_file_transfer(RequestFileTransfer_t *request) {
    // 1. Vérification de la longueur du message
    if (request->filePathAndNameLength > sizeof(request->filePathAndName)) {
        send_negative_response_file_transfer(0x13); // Longueur incorrecte
        return;
    }

    // 2. Vérification du mode d'opération
    if (request->modeOfOperation < 0x01 || request->modeOfOperation > 0x05) {
        send_negative_response_file_transfer(0x31); // Mode hors plage
        return;
    }

    // 3. Vérifications supplémentaires selon le mode d'opération
    // (Ajoutez ici des vérifications spécifiques si nécessaire)

    // 4. Envoi de la réponse positive
    ResponseFileTransfer_t response;
    response.SID = 0x78; // Identifiant de réponse
    response.modeOfOperation = request->modeOfOperation;
    response.lengthFormatIdentifier = 0x00; // À définir selon le contexte
    response.maxNumberOfBlockLength[0] = 0xFF; // Exemple de longueur maximale
    response.maxNumberOfBlockLength[1] = 0xFF; // Exemple de longueur maximale
    response.dataFormatIdentifier = request->dataFormatIdentifier;

    // Remplir les longueurs de fichier si nécessaire
    // Exemple : response.fileSizeOrDirInfoParameterLength = ...;

    // Appeler la fonction pour envoyer la réponse positive
    send_positive_response_file_transfer(&response);
}

// Fonction pour envoyer une réponse positive
void send_positive_response_file_transfer(ResponseFileTransfer_t *response) {
    // Envoi du message via le protocole CAN ou autre méthode appropriée
    send_can_message((uint8_t*)response, sizeof(ResponseFileTransfer_t));
    //send_uart_message((uint8_t*)response, sizeof(ResponseFileTransfer_t));
}


// Fonction pour envoyer une réponse négative
void send_negative_response_file_transfer(uint8_t nrc) {
    uint8_t response[2];
    response[0] = 0x78; // Identifiant de réponse
    response[1] = nrc; // NRC

    // Envoi d'un message d'erreur par CAN
    send_can_message(response, sizeof(response));
    // send_uart_message(response, sizeof(response));
}



