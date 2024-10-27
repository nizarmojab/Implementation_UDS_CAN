#include "Data_Transmission_functional_unit.h"
#include "uds_services.c"




/****************************************************ReadDataByIdentifier*******************************************************/
uint8_t data_record_1[] = {0x12, 0x34};
uint8_t data_record_2[] = {0x56, 0x78};
uint8_t data_record_3[] = {0x9A, 0xBC};

/**
 * @brief Implémente le service ReadDataByIdentifier (0x22)
 * @param data : Pointeur vers les données de la requête (contenant les dataIdentifiers)
 * @param data_length : Longueur des données
 */
void uds_read_data_by_identifier(uint8_t* data, uint8_t data_length) {
    // Vérification de la longueur minimale et modulo 2 de la requête
    if (data_length < 2 || data_length % 2 != 0) {
        send_negative_response_read_data_by_identifier(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // Vérification de la longueur maximale de la requête
    if (data_length > MAX_DATA_SIZE) {
        send_negative_response_read_data_by_identifier(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    uint8_t response[MAX_DATA_SIZE] = {0};  // Réponse maximale de 64 octets
    uint8_t response_index = 0;  // Index de remplissage de la réponse
    bool did_supported = false;  // Indicateur pour vérifier si au moins un DID est supporté

    response[response_index++] = UDS_READ_DATA_BY_IDENTIFIER + 0x40;  // SID de réponse 0x62

    // Boucle pour chaque dataIdentifier
    for (uint8_t i = 0; i < data_length; i += 2) {
        uint16_t did = (data[i] << 8) | data[i + 1];  // Récupération du DID

        // Vérification si le service est supporté pour chaque DID dans la session active
        if (!is_service_allowed(UDS_READ_DATA_BY_IDENTIFIER)) {
            send_negative_response_read_data_by_identifier(NRC_CONDITIONS_NOT_CORRECT);
            return;
        }

        // Vérification des conditions de sécurité pour le DID
        if (is_security_required_for_did(did) && !uds_session.security_access_granted) {
            send_negative_response_read_data_by_identifier(NRC_SECURITY_ACCESS_DENIED);
            return;
        }

        switch (did) {
            case SUPPORTED_DID_1:
                // Ajout du DID et des données associées à la réponse
                response[response_index++] = data[i];       // MSB du DID
                response[response_index++] = data[i + 1];   // LSB du DID
                memcpy(&response[response_index], data_record_1, sizeof(data_record_1));
                response_index += sizeof(data_record_1);
                did_supported = true;
                break;

            case SUPPORTED_DID_2:
                response[response_index++] = data[i];       // MSB du DID
                response[response_index++] = data[i + 1];   // LSB du DID
                memcpy(&response[response_index], data_record_2, sizeof(data_record_2));
                response_index += sizeof(data_record_2);
                did_supported = true;
                break;

            case SUPPORTED_DID_3:
                response[response_index++] = data[i];       // MSB du DID
                response[response_index++] = data[i + 1];   // LSB du DID
                memcpy(&response[response_index], data_record_3, sizeof(data_record_3));
                response_index += sizeof(data_record_3);
                did_supported = true;
                break;

            default:
                // Si le DID n'est pas supporté, continuer la boucle sans répondre immédiatement
                continue;
        }

        // Vérifier si la taille de la réponse dépasse la limite du protocole de transport
        if (response_index > MAX_DATA_SIZE) {
            send_negative_response_read_data_by_identifier(NRC_RESPONSE_TOO_LONG);
            return;
        }
    }

    // Si aucun DID n'est supporté, renvoyer une réponse négative
    if (!did_supported) {
        send_negative_response_read_data_by_identifier(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // Envoyer la réponse positive avec tous les DIDs traités
    send_can_message(response, response_index);
    // send_uart_message(response, response_index);
}

bool is_security_required_for_did(uint16_t did) {
    // Ajoutez ici la logique pour vérifier si un DID nécessite un accès de sécurité
    return false;  // Par défaut, supposons que les DIDs ne nécessitent pas de sécurité
}

/**
 * @brief Envoie une réponse positive pour le service ReadDataByIdentifier (0x22)
 * @param dataIdentifiers : Tableau des DIDs (identifiants de données)
 * @param dataRecords : Tableau des enregistrements de données associés aux DIDs
 * @param number_of_dids : Nombre de DIDs dans la réponse
 */
void send_positive_response_read_data_by_identifier(uint8_t* dataIdentifiers, uint8_t* dataRecords, uint8_t number_of_dids) {
    uint8_t response[MAX_DATA_SIZE] = {0};
    uint8_t index = 0;

    response[index++] = UDS_READ_DATA_BY_IDENTIFIER + 0x40;  // SID de réponse 0x62

    // Ajouter les DIDs et leurs enregistrements associés
    for (uint8_t i = 0; i < number_of_dids; i++) {
        response[index++] = dataIdentifiers[2 * i];     // MSB du DID
        response[index++] = dataIdentifiers[2 * i + 1]; // LSB du DID
        response[index++] = dataRecords[2 * i];         // Valeur de l'enregistrement
        response[index++] = dataRecords[2 * i + 1];     // Valeur de l'enregistrement
    }

    // Envoyer la réponse via CAN
    send_can_message(response, index);
    // send_uart_message(response, index);
}

/**
 * @brief Envoie une réponse négative pour le service ReadDataByIdentifier (0x22)
 * @param nrc : Code de réponse négative (NRC)
 */
void send_negative_response_read_data_by_identifier(uint8_t nrc) {
    uint8_t response[3] = {0};

    response[0] = UDS_NEGATIVE_RESPONSE;  // SID pour réponse négative 0x7F
    response[1] = UDS_READ_DATA_BY_IDENTIFIER;  // SID du service 0x22
    response[2] = nrc;  // Code de réponse négative (NRC)

    send_can_message(response, 3);
    //send_uart_message(response, 3);
}

/*********************************************************ReadMemoryByAddress********************************************************/
// Simule une mémoire pour la lecture
uint8_t memory[1024] = {
    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB,
    0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD,
};

// Implémente le service ReadMemoryByAddress (0x23)
void uds_read_memory_by_address(uint8_t* data, uint8_t data_length) {
    // Vérifier la longueur minimale
    if (data_length < 4) {
        send_negative_response_read_memory_by_address(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    uint8_t addressAndLengthFormatIdentifier = data[0];
    uint8_t address_length = addressAndLengthFormatIdentifier & 0x0F; // Bas nibble
    uint8_t size_length = (addressAndLengthFormatIdentifier >> 4) & 0x0F; // Haut nibble

    // Vérifier si l'ALFID est valide
    if (address_length < 1 || address_length > 4 || size_length < 1 || size_length > 4) {
        send_negative_response_read_memory_by_address(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // Calculer l'adresse mémoire à partir des octets fournis
    uint32_t memory_address = 0;
    for (int i = 0; i < address_length; i++) {
        memory_address = (memory_address << 8) | data[1 + i];
    }

    // Calculer la taille mémoire à partir des octets fournis
    uint32_t memory_size = 0;
    for (int i = 0; i < size_length; i++) {
        memory_size = (memory_size << 8) | data[1 + address_length + i];
    }

    // Vérification de la plage d'adresses et de la taille
    if (memory_address + memory_size > sizeof(memory)) {
        send_negative_response_read_memory_by_address(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // Vérifier les conditions de sécurité
    if (!uds_session.security_access_granted) {
        send_negative_response_read_memory_by_address(NRC_SECURITY_ACCESS_DENIED);
        return;
    }

    // Lire la mémoire et envoyer la réponse positive
    send_positive_response_read_memory_by_address(&memory[memory_address], memory_size);
}

// Fonction pour envoyer une réponse positive avec les données lues
void send_positive_response_read_memory_by_address(uint8_t* dataRecord, uint8_t data_length) {
    uint8_t response[MAX_DATA_SIZE] = {0};
    uint8_t index = 0;

    // Ajouter le SID 0x63 pour la réponse positive
    response[index++] = UDS_READ_MEMORY_BY_ADDRESS + 0x40;

    // Ajouter les données lues
    memcpy(&response[index], dataRecord, data_length);
    index += data_length;

    // Envoyer la réponse via CAN
    send_can_message(response, index);
    //send_uart_message(response, index);
}

// Fonction pour envoyer une réponse négative
void send_negative_response_read_memory_by_address(uint8_t nrc) {
    uint8_t response[3] = {0};

    response[0] = UDS_NEGATIVE_RESPONSE;  // SID pour réponse négative 0x7F
    response[1] = UDS_READ_MEMORY_BY_ADDRESS;  // SID du service 0x23
    response[2] = nrc;  // Code de réponse négative (NRC)

    send_can_message(response, 3);
    //send_uart_message(response, 3);

}
/****************************************************ReadDataByPeriodicIdentifier***********************************************/
// Initialisation de la liste des PIDs périodiques
PeriodicPIDInfo periodic_pid_list[MAX_PERIODIC_PIDS];

// Fonction principale pour gérer la requête ReadDataByPeriodicIdentifier
void uds_read_data_by_periodic_identifier(uint8_t* data, uint8_t data_length) {
    // Vérification de la longueur minimale du message
    if (data_length < 2) {
        send_negative_response_read_data_by_periodic_identifier(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    uint8_t transmissionMode = data[1]; // Byte 2 : Mode de transmission
    if (transmissionMode < UDS_TRANSMISSION_MODE_SLOW || transmissionMode > UDS_TRANSMISSION_MODE_STOP) {
        send_negative_response_read_data_by_periodic_identifier(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // Vérification de la longueur pour chaque mode
    if (transmissionMode == UDS_TRANSMISSION_MODE_STOP && data_length < 2) {
        send_negative_response_read_data_by_periodic_identifier(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    } else if (transmissionMode != UDS_TRANSMISSION_MODE_STOP && data_length < 3) {
        send_negative_response_read_data_by_periodic_identifier(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // Boucle pour traiter tous les periodicDataIdentifiers (PIDs) fournis
    for (uint8_t i = 2; i < data_length; i++) {
        uint8_t periodicDataIdentifier = data[i];

        // Vérifier si le PID est valide dans la session active
        if (!is_pid_supported_in_session(periodicDataIdentifier)) {
            send_negative_response_read_data_by_periodic_identifier(NRC_REQUEST_OUT_OF_RANGE);
            return;
        }

        // Vérifier la sécurité si nécessaire
        if (!is_security_granted_for_pid(periodicDataIdentifier)) {
            send_negative_response_read_data_by_periodic_identifier(NRC_SECURITY_ACCESS_DENIED);
            return;
        }

        // Traitement du mode stopSending
        if (transmissionMode == UDS_TRANSMISSION_MODE_STOP) {
            stop_periodic_transmission(periodicDataIdentifier);
        } else {
            // Démarrer la transmission périodique pour ce PID
            start_periodic_transmission(periodicDataIdentifier, transmissionMode);
        }
    }

    // Envoyer la réponse initiale positive
    send_positive_response_read_data_by_periodic_identifier();
}

// Fonction pour démarrer la transmission périodique
void start_periodic_transmission(uint8_t pid, uint8_t mode) {
    // Parcourir la liste des PIDs périodiques pour trouver un emplacement libre
    for (int i = 0; i < MAX_PERIODIC_PIDS; i++) {
        if (!periodic_pid_list[i].isActive) {
            // Initialiser les paramètres pour ce PID
            periodic_pid_list[i].periodicDataIdentifier = pid;
            periodic_pid_list[i].transmissionMode = mode;
            periodic_pid_list[i].isActive = true;
            return;
        }
    }

    // Si aucun emplacement n'est libre, envoyer une réponse négative
    send_negative_response_read_data_by_periodic_identifier(NRC_CONDITIONS_NOT_CORRECT);
}

// Fonction pour arrêter la transmission périodique pour un PID donné
void stop_periodic_transmission(uint8_t pid) {
    for (int i = 0; i < MAX_PERIODIC_PIDS; i++) {
        if (periodic_pid_list[i].periodicDataIdentifier == pid && periodic_pid_list[i].isActive) {
            periodic_pid_list[i].isActive = false;
            return;
        }
    }
}

// Fonction pour arrêter toutes les transmissions périodiques
void stop_all_periodic_transmissions(void) {
    for (int i = 0; i < MAX_PERIODIC_PIDS; i++) {
        periodic_pid_list[i].isActive = false;
    }
}

// Fonction pour envoyer une réponse positive initiale
void send_positive_response_read_data_by_periodic_identifier(void) {
    uint8_t response[2];
    response[0] = UDS_READ_DATA_BY_PERIODIC_IDENTIFIER + 0x40; // Réponse positive SID
    response[1] = 0x00; // No data required in the positive response

    // Envoyer la réponse via CAN
    send_can_message(response, 2);
    //send_uart_message(response, 2);
}

// Fonction pour envoyer une réponse négative
void send_negative_response_read_data_by_periodic_identifier(uint8_t nrc) {
    uint8_t response[3];
    response[0] = UDS_NEGATIVE_RESPONSE; // SID for negative response
    response[1] = UDS_READ_DATA_BY_PERIODIC_IDENTIFIER; // SID du service
    response[2] = nrc; // NRC code

    send_can_message(response, 3);
    //send_uart_message(response, 3);
}

// Vérification si un PID est supporté dans la session active
bool is_pid_supported_in_session(uint8_t pid) {
    // Vérification des PIDs supportés (cette logique peut être modifiée en fonction de la session)
    if (pid == 0xE3 || pid == 0x24 || pid == 0x01 || pid == 0x02) {
        return true;
    }
    return false;
}

// Vérification des autorisations de sécurité pour un PID
bool is_security_granted_for_pid(uint8_t pid) {
    // Simuler une vérification de sécurité. Modifier cette fonction selon les besoins.
    // Par exemple, vérifier si le pid nécessite une session sécurisée.
    return uds_session.security_access_granted;
}

/*************************************************DynamicallyDefineDataIdentifier***********************************************/
// Stockage des identifiants de données dynamiques
static DynamicallyDefinedIdentifier dynamic_did_list[MAX_DYNAMIC_DIDS];
static uint8_t dynamic_did_count = 0;

// Fonction pour gérer le service DynamicallyDefineDataIdentifier (0x2C)
void uds_dynamically_define_data_identifier(uint8_t sub_function, uint8_t *data, uint8_t data_length) {
    // Vérifier la sous-fonction demandée
    switch (sub_function) {
        case UDS_DDDI_DEFINE_BY_IDENTIFIER:
            define_by_identifier(data, data_length);
            break;

        case UDS_DDDI_DEFINE_BY_MEMORY_ADDRESS:
            define_by_memory_address(data, data_length);
            break;

        case UDS_DDDI_CLEAR_DYNAMIC_IDENTIFIER:
            // Convertir les deux premiers octets en DID dynamique (uint16_t)
            if (data_length < 2) {
                send_negative_response_dynamically_define_data_identifier(sub_function, NRC_INCORRECT_MESSAGE_LENGTH);
                return;
            }
            uint16_t did_to_clear = (data[0] << 8) | data[1];
            clear_dynamically_defined_data_identifier(did_to_clear);
            break;

        default:
            // Envoyer une réponse négative pour une sous-fonction non supportée
            send_negative_response_dynamically_define_data_identifier(sub_function, NRC_SUB_FUNCTION_NOT_SUPPORTED);
            break;
    }
}

// Fonction pour définir un DID dynamique par identifiant (0x01)
void define_by_identifier(uint8_t *data, uint8_t data_length) {
    if (data_length < 8) {
        // Longueur de message incorrecte
        send_negative_response_dynamically_define_data_identifier(UDS_DDDI_DEFINE_BY_IDENTIFIER, NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    if (dynamic_did_count >= MAX_DYNAMIC_DIDS) {
        // Dépassement du nombre maximum d'identifiants dynamiques
        send_negative_response_dynamically_define_data_identifier(UDS_DDDI_DEFINE_BY_IDENTIFIER, NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // Extraire les données de la requête
    uint16_t dynamic_did = (data[0] << 8) | data[1];   // DID dynamique
    uint16_t source_did = (data[2] << 8) | data[3];    // DID source
    uint8_t position_in_source = data[4];              // Position dans le DID source
    uint8_t memory_size = data[5];                     // Taille des données

    // Vérification si le DID est déjà défini
    if (is_dynamic_identifier_supported(dynamic_did)) {
        send_negative_response_dynamically_define_data_identifier(UDS_DDDI_DEFINE_BY_IDENTIFIER, NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // Enregistrer le nouveau DID dynamique
    dynamic_did_list[dynamic_did_count].dynamicDataIdentifier = dynamic_did;
    dynamic_did_list[dynamic_did_count].sourceDataIdentifier = source_did;
    dynamic_did_list[dynamic_did_count].positionInSource = position_in_source;
    dynamic_did_list[dynamic_did_count].memorySize = memory_size;

    dynamic_did_count++;

    // Envoyer une réponse positive
    send_positive_response_dynamically_define_data_identifier(UDS_DDDI_DEFINE_BY_IDENTIFIER, dynamic_did);
}

// Fonction pour définir un DID dynamique par adresse mémoire (0x02)
void define_by_memory_address(uint8_t *data, uint8_t data_length) {
    if (data_length < 10) {
        send_negative_response_dynamically_define_data_identifier(UDS_DDDI_DEFINE_BY_MEMORY_ADDRESS, NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    if (dynamic_did_count >= MAX_DYNAMIC_DIDS) {
        send_negative_response_dynamically_define_data_identifier(UDS_DDDI_DEFINE_BY_MEMORY_ADDRESS, NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // Extraire les informations de la requête
    uint16_t dynamic_did = (data[0] << 8) | data[1];   // DID dynamique
    uint8_t address_format = data[2];                  // Format d'adresse et de taille

    // Validation simple basée sur address_format (Exemple)
    if (address_format != 0x14) {
        send_negative_response_dynamically_define_data_identifier(UDS_DDDI_DEFINE_BY_MEMORY_ADDRESS, NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    uint32_t memory_address = (data[3] << 24) | (data[4] << 16) | (data[5] << 8) | data[6]; // Adresse mémoire
    uint8_t memory_size = data[7];                     // Taille des données

    // Enregistrer le DID dynamique
    dynamic_did_list[dynamic_did_count].dynamicDataIdentifier = dynamic_did;
    dynamic_did_list[dynamic_did_count].positionInSource = memory_address; // Stocker l'adresse comme position
    dynamic_did_list[dynamic_did_count].memorySize = memory_size;

    dynamic_did_count++;

    // Envoyer une réponse positive
    send_positive_response_dynamically_define_data_identifier(UDS_DDDI_DEFINE_BY_MEMORY_ADDRESS, dynamic_did);
}


// Fonction pour effacer un DID dynamique (0x03)
void clear_dynamically_defined_data_identifier(uint16_t dynamicDataIdentifier) {
    // Rechercher et supprimer le DID dynamique
    for (uint8_t i = 0; i < dynamic_did_count; i++) {
        if (dynamic_did_list[i].dynamicDataIdentifier == dynamicDataIdentifier) {
            // Effacer l'entrée en décalant les autres éléments
            dynamic_did_list[i] = dynamic_did_list[dynamic_did_count - 1]; // Remplacer par la dernière entrée
            dynamic_did_count--;
            send_positive_response_dynamically_define_data_identifier(UDS_DDDI_CLEAR_DYNAMIC_IDENTIFIER, dynamicDataIdentifier);
            return;
        }
    }

    // Si le DID n'existe pas, envoyer une réponse négative
    send_negative_response_dynamically_define_data_identifier(UDS_DDDI_CLEAR_DYNAMIC_IDENTIFIER, NRC_REQUEST_OUT_OF_RANGE);
}

// Fonction pour vérifier si un DID dynamique est déjà défini
bool is_dynamic_identifier_supported(uint16_t did) {
    for (uint8_t i = 0; i < dynamic_did_count; i++) {
        if (dynamic_did_list[i].dynamicDataIdentifier == did) {
            return true;
        }
    }
    return false;
}

// Fonction pour envoyer une réponse positive
void send_positive_response_dynamically_define_data_identifier(uint8_t sub_function, uint16_t dynamic_did) {
    uint8_t response[4];
    response[0] = UDS_DYNAMICAL_DEFINE_DATA_IDENTIFIER + 0x40; // SID pour la réponse positive
    response[1] = sub_function;
    response[2] = (dynamic_did >> 8) & 0xFF; // DID MSB
    response[3] = dynamic_did & 0xFF;        // DID LSB
    send_can_message(response, 4);
    //send_uart_message(response, 4);
}

// Fonction pour envoyer une réponse négative
void send_negative_response_dynamically_define_data_identifier(uint8_t sub_function, uint8_t nrc) {
    uint8_t response[3];
    response[0] = UDS_NEGATIVE_RESPONSE; // Réponse négative
    response[1] = UDS_DYNAMICAL_DEFINE_DATA_IDENTIFIER; // SID du service
    response[2] = nrc; // Code NRC
    send_can_message(response, 3);
    //send_uart_message(response, 3);
}
/************************************************* WriteMemoryByAddress*********************************************************/

// Stockage fictif pour les DIDs, à remplacer par la logique réelle de votre système
uint8_t did_storage[MAX_DATA_SIZE] = {0};

// Fonction principale pour gérer le service WriteDataByIdentifier (0x2E)
void uds_write_data_by_identifier(uint8_t* data, uint8_t data_length) {
    // Vérification de la longueur minimale du message (4 octets : SID + DID + au moins 1 octet de données)
    if (data_length < 4) {
        send_negative_response_write_data_by_identifier(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // Extraire le dataIdentifier (DID) de la requête
    uint16_t dataIdentifier = (data[0] << 8) | data[1];

    // Vérifier si le DID est supporté et writable dans la session actuelle
    if (!is_data_identifier_supported_for_write(dataIdentifier)) {
        send_negative_response_write_data_by_identifier(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // Vérification de la sécurité (si le DID nécessite un accès sécurisé)
    if (is_security_required_for_did(dataIdentifier) && !uds_session.security_access_granted) {
        send_negative_response_write_data_by_identifier(NRC_SECURITY_ACCESS_DENIED);
        return;
    }

    // Vérifier les conditions d'écriture (par ex. session, état de l'ECU)
    if (!are_conditions_correct_for_did(dataIdentifier)) {
        send_negative_response_write_data_by_identifier(NRC_CONDITIONS_NOT_CORRECT);
        return;
    }

    // Extraire le dataRecord à partir du message
    uint8_t* dataRecord = &data[2];
    uint8_t dataRecordLength = data_length - 2;

    // Effectuer l'écriture des données dans le DID spécifié
    if (!write_data_to_identifier(dataIdentifier, dataRecord, dataRecordLength)) {
        send_negative_response_write_data_by_identifier(NRC_GENERAL_PROGRAMMING_FAILURE);
        return;
    }

    // Envoyer une réponse positive après écriture réussie
    send_positive_response_write_data_by_identifier(dataIdentifier);
}

// Fonction pour vérifier si un DID est supporté pour l'écriture
bool is_data_identifier_supported_for_write(uint16_t dataIdentifier) {
    // Implémentez la logique ici pour vérifier si le DID est writable
    switch (dataIdentifier) {
        case SUPPORTED_DID_1:
        case SUPPORTED_DID_2:
        case SUPPORTED_DID_3:
            return true;  // Ces DIDs supportent l'écriture
        default:
            return false; // DID non supporté
    }
}



// Fonction pour écrire des données dans le DID spécifié
bool write_data_to_identifier(uint16_t dataIdentifier, uint8_t* dataRecord, uint8_t dataLength) {
    // Implémentez la logique pour écrire les données dans la mémoire associée au DID
    // Exemple simple : écrire dans une zone de mémoire fictive
    switch (dataIdentifier) {
        case SUPPORTED_DID_1:
            memcpy(&did_storage[0], dataRecord, dataLength);
            return true;
        case SUPPORTED_DID_2:
            memcpy(&did_storage[16], dataRecord, dataLength);
            return true;
        case SUPPORTED_DID_3:
            memcpy(&did_storage[32], dataRecord, dataLength);
            return true;
        default:
            return false;
    }
}

// Fonction pour envoyer une réponse positive après écriture réussie
void send_positive_response_write_data_by_identifier(uint16_t dataIdentifier) {
    uint8_t response[3];
    response[0] = UDS_WRITE_DATA_BY_IDENTIFIER + 0x40; // Réponse positive SID (0x2E + 0x40 = 0x6E)
    response[1] = (dataIdentifier >> 8) & 0xFF;       // Octet MSB du dataIdentifier
    response[2] = dataIdentifier & 0xFF;              // Octet LSB du dataIdentifier
    send_can_message(response, 3);
    //send_uart_message(response, 3);
}

// Fonction pour envoyer une réponse négative pour WriteDataByIdentifier
void send_negative_response_write_data_by_identifier(uint8_t nrc) {
    uint8_t response[3];
    response[0] = UDS_NEGATIVE_RESPONSE;   // SID pour une réponse négative
    response[1] = UDS_WRITE_DATA_BY_IDENTIFIER; // SID du service WriteDataByIdentifier (0x2E)
    response[2] = nrc;                     // Code NRC
    send_can_message(response, 3);
    //send_uart_message(response, 3);
}

// Fonction pour vérifier les conditions spécifiques (exemple : état de la session)
bool are_conditions_correct_for_did(uint16_t dataIdentifier) {
    // Implémentez des vérifications supplémentaires si nécessaire
    // Exemple : autorisation seulement dans une session étendue
    if (uds_session.current_session == 0) {  // Session par défaut
        return false;  // Conditions non remplies si la session est par défaut
    }
    return true;
}

void send_can_message(uint8_t *message, uint8_t length) {
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;

    TxHeader.DLC = length;
    TxHeader.StdId = 0x7E0; // Identifiant standard UDS pour l'ECU
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;

    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, message, &TxMailbox) != HAL_OK) {
        // Gérer l'erreur d'envoi
        Error_handler();
    }
}

/*
 * void send_uart_message(uint8_t *message, uint8_t length) {
    if (HAL_UART_Transmit(&huart1, message, length, HAL_MAX_DELAY) != HAL_OK) {
        // Gérer l'erreur d'envoi
        Error_handler();
    }
}
 *
 */

