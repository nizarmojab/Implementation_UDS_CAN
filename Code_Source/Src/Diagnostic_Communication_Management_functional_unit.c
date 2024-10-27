#include "uds_services.c"
#include "Diagnostic_Communication_Management_functional_unit.h"


/****************************************Diagnostic Session Control************************************************************/
// Déclaration de la structure UDS_Session
UDS_Session uds_session = {UDS_SESSION_DEFAULT, false};

/**
 * @brief Contrôle de session diagnostique
 * @param session_type : Type de session de diagnostic (Default, Programming, Extended, Safety)
 */
void uds_diagnostic_session_control(uint8_t session_type) {
    uint8_t message_length = 2; // Exemple de longueur de message UDS

    if (!is_service_allowed(UDS_DIAGNOSTIC_SESSION_CONTROL)) {
            send_negative_response_diagnostic_control(UDS_DIAGNOSTIC_SESSION_CONTROL, NRC_CONDITIONS_NOT_CORRECT);
            return;
        }
    if (message_length != 2) {
        send_negative_response_diagnostic_control(UDS_DIAGNOSTIC_SESSION_CONTROL, NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    switch (session_type) {
        case UDS_SESSION_DEFAULT:
            if (uds_session.current_session == UDS_SESSION_DEFAULT) {
                // Réinitialiser la session par défaut
                reset_default_session();
                send_positive_response_diagnostic_control(UDS_DIAGNOSTIC_SESSION_CONTROL);
            } else {
                // Passage de la session non par défaut à la session par défaut
                reset_events();
                deactivate_extended_services();
                uds_session.current_session = UDS_SESSION_DEFAULT;
                send_positive_response_diagnostic_control(UDS_DIAGNOSTIC_SESSION_CONTROL);
            }
            break;

        case UDS_SESSION_PROGRAMMING:
            // Passage à la session de programmation
            reset_events();
            uds_session.current_session = UDS_SESSION_PROGRAMMING;
            send_positive_response_diagnostic_control(UDS_DIAGNOSTIC_SESSION_CONTROL);
            break;

        case UDS_SESSION_EXTENDED_DIAGNOSTIC:
            // Passage à la session de diagnostic étendu
            reset_events();
            uds_session.current_session = UDS_SESSION_EXTENDED_DIAGNOSTIC;
            send_positive_response_diagnostic_control(UDS_DIAGNOSTIC_SESSION_CONTROL);
            break;

        case UDS_SESSION_SAFETY_SYSTEM:
            // Passage à la session de sécurité
            reset_events();
            uds_session.current_session = UDS_SESSION_SAFETY_SYSTEM;
            send_positive_response_diagnostic_control(UDS_DIAGNOSTIC_SESSION_CONTROL);
            break;

        default:
            // Réponse négative pour sous-fonction non supportée
            send_negative_response_diagnostic_control(UDS_DIAGNOSTIC_SESSION_CONTROL, NRC_SUB_FUNCTION_NOT_SUPPORTED);
            break;
    }
}


/**
 * @brief Réinitialisation de la session par défaut
 */
void reset_default_session() {
    // Réinitialise les paramètres de la session par défaut
    uds_session.security_access_granted = false;
    // Réinitialisation des événements et autres paramètres spécifiques
    reset_events();
    deactivate_extended_services();
}

/**
 * @brief Réinitialise les événements actifs (ex: ResponseOnEvent)
 */
void reset_events() {
    // Arrête tous les événements actifs déclenchés dans les sessions non par défaut
}

/**
 * @brief Désactive les services non supportés dans la session par défaut
 */
void deactivate_extended_services() {
    // Désactivation des services étendus comme CommunicationControl, ResponseOnEvent, etc.
}

/**
 * @brief Vérifie si un service est autorisé dans la session actuelle
 * @param service_id : Identifiant du service UDS
 * @return true si le service est autorisé, false sinon
 */
bool is_service_allowed(uint8_t service_id) {
    switch (service_id) {
        case UDS_DIAGNOSTIC_SESSION_CONTROL:
        case UDS_ECU_RESET:
        case UDS_TESTER_PRESENT:
        case UDS_CLEAR_DIAGNOSTIC_INFORMATION:
        case UDS_READ_DTC_INFORMATION:
            // Ces services sont toujours autorisés dans les deux sessions
            return true;

        case UDS_SECURITY_ACCESS:
        case UDS_COMMUNICATION_CONTROL:
        case UDS_ACCESS_TIMING_PARAMETER:
        case UDS_SECURED_DATA_TRANSMISSION:
        case UDS_CONTROL_DTC_SETTING:
        case UDS_RESPONSE_ON_EVENT:
        case UDS_LINK_CONTROL:
        case UDS_READ_MEMORY_BY_ADDRESS:
        case UDS_WRITE_MEMORY_BY_ADDRESS:
        case UDS_REQUEST_DOWNLOAD:
        case UDS_REQUEST_UPLOAD:
        case UDS_TRANSFER_DATA:
        case UDS_REQUEST_TRANSFER_EXIT:
        case UDS_REQUEST_FILE_TRANSFER:
            // Ces services ne sont autorisés que dans les sessions non par défaut
            return uds_session.current_session != UDS_SESSION_DEFAULT;

        case UDS_READ_DATA_BY_IDENTIFIER:
        case UDS_WRITE_DATA_BY_IDENTIFIER:
        case UDS_READ_SCALING_DATA_BY_IDENTIFIER:
        case UDS_DYNAMICAL_DEFINE_DATA_IDENTIFIER:
            // Ces services nécessitent l'accès sécurité, donc non autorisés en session par défaut
            return uds_session.current_session != UDS_SESSION_DEFAULT && uds_session.security_access_granted;

        case UDS_ROUTINE_CONTROL:
            // RoutineControl nécessite aussi un accès sécurité et une session non par défaut
            return uds_session.current_session != UDS_SESSION_DEFAULT && uds_session.security_access_granted;

        default:
            // Pour les autres services non répertoriés
            return false;
    }
}


/**
 * @brief Envoie une réponse positive au client
 * @param service_id : Identifiant du service pour lequel la réponse est envoyée
 */
void send_positive_response_diagnostic_control(uint8_t service_id) {
    uint8_t response[8] = {0};

    // Réponse SID pour indiquer un succès (ajout de 0x40 au service ID)
    response[0] = service_id + 0x40; // Par exemple, pour DiagnosticSessionControl, ce sera 0x50

    // Sub-function : Type de session dans la réponse (default, extended, etc.)
    response[1] = uds_session.current_session;

    // Session Parameter Record : données supplémentaires (P2Server_max et P2*Server_max par exemple)
    // Simulons des valeurs pour ces timings (en ms)
    response[2] = 0x00; // P2Server_max high byte
    response[3] = 0x32; // P2Server_max low byte (50 ms par exemple)
    response[4] = 0x01; // P2*Server_max high byte
    response[5] = 0xF4; // P2*Server_max low byte (500 ms par exemple)

    // Longueur des données de réponse (6 octets dans ce cas)
    send_can_message(response, 6);
}

/**
 * @brief Envoie une réponse négative au client
 * @param service_id : Identifiant du service pour lequel la réponse est envoyée
 * @param nrc : Code de réponse négative (NRC)
 */
void send_negative_response_diagnostic_control(uint8_t service_id, uint8_t nrc) {
    uint8_t response[3] = {0};

    response[0] = UDS_NEGATIVE_RESPONSE; // Réponse négative générique
    response[1] = service_id; // Service concerné
    response[2] = nrc; // Code NRC (Negative Response Code)

    send_can_message(response, 3);
}

/**********************************************ECU RESET***********************************************************************/
/**
 * @brief ECU Reset Service
 * @param resetType : Type of reset requested by the client (e.g., hard reset, soft reset, etc.)
 */
void uds_ecu_reset(uint8_t resetType) {
    // Vérification de la validité du resetType
    if (resetType > UDS_RESET_TYPE_DISABLE_RAPID_POWER_SHUTDOWN) {
        send_negative_response_ecu_reset(UDS_ECU_RESET, NRC_SUB_FUNCTION_NOT_SUPPORTED);
        return;
    }

    // Construire la réponse positive (SID = 0x51)
    uint8_t response[3] = {0x51, resetType, 0xFF}; // 0xFF pour "powerDownTime" si applicable
    send_can_message(response, 3);  // Envoyer la réponse avant l'exécution du reset

    // Exécution du reset en fonction du resetType
    switch (resetType) {
        case UDS_RESET_TYPE_HARD_RESET:
            hard_reset();  // Fonction qui effectue une réinitialisation matérielle
            break;
        case UDS_RESET_TYPE_SOFT_RESET:
            soft_reset();  // Fonction qui effectue une réinitialisation logicielle
            break;
        case UDS_RESET_TYPE_ENABLE_RAPID_POWER_SHUTDOWN:
            // Activer la fonction d'arrêt rapide de l'alimentation
            enable_rapid_power_shutdown();
            break;
        case UDS_RESET_TYPE_DISABLE_RAPID_POWER_SHUTDOWN:
            // Désactiver la fonction d'arrêt rapide de l'alimentation
            disable_rapid_power_shutdown();
            break;
        default:
            // En cas de type de reset non supporté
            send_negative_response_ecu_reset(UDS_ECU_RESET, NRC_SUB_FUNCTION_NOT_SUPPORTED);
            return;
    }

    // Après le reset, revenir à la session par défaut
    uds_session.current_session = UDS_SESSION_DEFAULT;
}

void hard_reset() {
    // Utilisation de la fonction NVIC pour déclencher une réinitialisation complète
    NVIC_SystemReset();
}
void soft_reset() {
    // Exemple d'un soft reset qui pourrait redémarrer l'application sans réinitialiser les registres matériels
    // Vous pouvez définir ici votre propre logique pour réinitialiser uniquement les parties nécessaires
    // par exemple, réinitialiser certains périphériques ou redémarrer des tâches sans réinitialiser l'ensemble du système.

    // Redémarrer la boucle principale ou réinitialiser certains périphériques
}
void enable_rapid_power_shutdown() {
    // Entrer dans un mode basse consommation
    // Exemple avec le mode Stop de la STM32
    HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}
void disable_rapid_power_shutdown() {

}


/**
 * @brief Sends a positive response for the ECUReset service
 * @param resetType : Type of reset that was requested by the client
 * @param powerDownTime : Time in seconds the ECU will stay in power-down mode (only for rapid power shutdown)
 */
void send_positive_response_ecu_reset(uint8_t resetType, uint8_t powerDownTime) {
    uint8_t response[3] = {0};

    // Response SID (0x51 for ECUReset Response)
    response[0] = 0x51;

    // Sub-function echoing the resetType (received from the client request)
    response[1] = resetType;

    // PowerDownTime is only included when the resetType is 'Enable Rapid Power Shutdown' (0x04)
    if (resetType == UDS_RESET_TYPE_ENABLE_RAPID_POWER_SHUTDOWN) {
        response[2] = powerDownTime;  // 0x00 to 0xFE for seconds, 0xFF for not available
        send_can_message(response, 3); // Send a message with 3 bytes
    } else {
        // If powerDownTime is not applicable, only send the first two bytes
        send_can_message(response, 2); // Send a message with 2 bytes
    }
}

/**
 * @brief Envoie une réponse négative pour le service ECUReset
 * @param service_id : Identifiant du service ECUReset (0x11)
 * @param nrc : Code de réponse négative (NRC)
 */
void send_negative_response_ecu_reset(uint8_t service_id, uint8_t nrc) {
    uint8_t response[3] = {0};

    // Remplir le message de réponse négative
    response[0] = UDS_NEGATIVE_RESPONSE;         // Réponse négative générique
    response[1] = service_id;   // Service concerné (ECUReset 0x11)
    response[2] = nrc;          // NRC spécifique

    // Traitement des différents NRC en fonction des conditions
    switch (nrc) {
        case NRC_SUB_FUNCTION_NOT_SUPPORTED:
            // Code 0x12 : Sous-fonction non prise en charge
            response[2] = NRC_SUB_FUNCTION_NOT_SUPPORTED;
            break;
        case NRC_INCORRECT_MESSAGE_LENGTH:
            // Code 0x13 : Longueur ou format du message incorrect
            response[2] = NRC_INCORRECT_MESSAGE_LENGTH;
            break;
        case NRC_CONDITIONS_NOT_CORRECT:
            // Code 0x22 : Conditions non respectées pour l'ECU Reset
            response[2] = NRC_CONDITIONS_NOT_CORRECT;
            break;
        case NRC_SECURITY_ACCESS_DENIED:
            // Code 0x33 : Accès de sécurité refusé
            response[2] = NRC_SECURITY_ACCESS_DENIED;
            break;
        default:
            // Autres codes de réponse négative non définis
            response[2] = 0x7F; // Code générique d'erreur si non reconnu
            break;
    }

    // Envoyer la réponse CAN avec le code NRC approprié
    send_can_message(response, 3);
}

/**********************************************Security access***********************************************************************/
/**
 * @brief Service Security Access (0x27)
 * @param sub_function : Sous-fonction (0x01 pour demande de seed, 0x02 pour vérification de clé)
 * @param data : Pointeur vers les données envoyées (key ou autres paramètres)
 * @param data_length : Longueur des données envoyées
 */
// Définition des variables de seed et key
uint8_t security_seed_lvl1 = 0x5A; // Seed prédéfini pour niveau 1
uint8_t security_key_lvl1 = 0xA5;  // Clé prédéfinie pour le seed de niveau 1
uint8_t security_seed_lvl2 = 0x9B; // Seed prédéfini pour niveau 2
uint8_t security_key_lvl2 = 0xB9;  // Clé prédéfinie pour le seed de niveau 2

void uds_security_access(uint8_t sub_function, uint8_t *data, uint8_t data_length) {
    switch (sub_function) {
        case UDS_SECURITY_ACCESS_REQUEST_SEED_LVL1:
            // Demande de seed pour le niveau 1
            send_seed(UDS_SECURITY_ACCESS_REQUEST_SEED_LVL1);
            send_positive_response_security_access(UDS_SECURITY_ACCESS_REQUEST_SEED_LVL1);
            break;

        case UDS_SECURITY_ACCESS_SEND_KEY_LVL1:
            // Vérifier la longueur des données envoyées
            if (data_length != 1) {
                send_negative_response_security_access(sub_function, NRC_INCORRECT_MESSAGE_LENGTH);
                return;
            }
            // Vérifier la clé envoyée
            if (verify_key(UDS_SECURITY_ACCESS_SEND_KEY_LVL1, data)) {
                send_positive_response_security_access(UDS_SECURITY_ACCESS_SEND_KEY_LVL1);
            } else {
                send_negative_response_security_access(sub_function, NRC_INVALID_KEY);
            }
            break;

        case UDS_SECURITY_ACCESS_REQUEST_SEED_LVL2:
            // Demande de seed pour le niveau 2
            send_seed(UDS_SECURITY_ACCESS_REQUEST_SEED_LVL2);
            send_positive_response_security_access(UDS_SECURITY_ACCESS_REQUEST_SEED_LVL2);
            break;

        case UDS_SECURITY_ACCESS_SEND_KEY_LVL2:
            // Vérifier la longueur des données envoyées
            if (data_length != 1) {
                send_negative_response_security_access(sub_function, NRC_INCORRECT_MESSAGE_LENGTH);
                return;
            }
            // Vérifier la clé envoyée
            if (verify_key(UDS_SECURITY_ACCESS_SEND_KEY_LVL2, data)) {
                send_positive_response_security_access(UDS_SECURITY_ACCESS_SEND_KEY_LVL2);
            } else {
                send_negative_response_security_access(sub_function, NRC_INVALID_KEY);
            }
            break;

        default:
            // Sous-fonction non supportée
            send_negative_response_security_access(sub_function, NRC_SUB_FUNCTION_NOT_SUPPORTED);
            break;
    }
}


/**
 * @brief Envoie le seed pour le niveau de sécurité donné
 * @param level : Niveau de sécurité (ex: 0x01 pour niveau 1, 0x03 pour niveau 2)
 */
void send_seed(uint8_t level) {
    uint8_t response[3] = {0};

    response[0] = UDS_SECURITY_ACCESS + 0x40;  // Réponse positive
    response[1] = level;  // Niveau de sécurité demandé

    // Seed spécifique au niveau de sécurité
    if (level == UDS_SECURITY_ACCESS_REQUEST_SEED_LVL1) {
        response[2] = security_seed_lvl1;
    } else if (level == UDS_SECURITY_ACCESS_REQUEST_SEED_LVL2) {
        response[2] = security_seed_lvl2;
    }

    send_can_message(response, 3);  // Envoyer le seed via CAN
}

/**
 * @brief Vérifie la clé envoyée par le client
 * @param level : Niveau de sécurité (ex: 0x02 pour niveau 1, 0x04 pour niveau 2)
 * @param key : Clé envoyée par le client
 */
bool verify_key(uint8_t level, uint8_t* key) {
    // Vérification simple de la clé
    if (level == UDS_SECURITY_ACCESS_SEND_KEY_LVL1 && *key == security_key_lvl1) {
        uds_session.security_access_granted = true;  // Accorder l'accès de sécurité
        return true;
    } else if (level == UDS_SECURITY_ACCESS_SEND_KEY_LVL2 && *key == security_key_lvl2) {
        uds_session.security_access_granted = true;  // Accorder l'accès de sécurité
        return true;
    } else {
        return false;
    }
}



/**
 * @brief Envoie une réponse positive pour le service Security Access
 * @param sub_function : Sous-fonction (0x01 pour demande de seed, 0x02 pour vérification de clé)
 */
void send_positive_response_security_access(uint8_t sub_function) {
    uint8_t response[4] = {0};

    // Réponse SID pour indiquer un succès (ajout de 0x40 au service ID)
    response[0] = UDS_SECURITY_ACCESS + 0x40;  // Réponse positive (0x27 + 0x40 = 0x67)
    response[1] = sub_function;  // Type d'accès sécurisé (écho de la sous-fonction)

    // Si c'est une demande de seed, on inclut la graine dans la réponse
    if (sub_function == UDS_SECURITY_ACCESS_REQUEST_SEED_LVL1) {
        response[2] = security_seed_lvl1;  // Graine de sécurité niveau 1
    } else if (sub_function == UDS_SECURITY_ACCESS_REQUEST_SEED_LVL2) {
        response[2] = security_seed_lvl2;  // Graine de sécurité niveau 2
    }

    // Longueur du message : 2 octets pour la sous-fonction et la graine (si présente)
    uint8_t response_length = (sub_function == UDS_SECURITY_ACCESS_REQUEST_SEED_LVL1 ||
                               sub_function == UDS_SECURITY_ACCESS_REQUEST_SEED_LVL2) ? 3 : 2;

    // Envoyer le message CAN avec la réponse positive
    send_can_message(response, response_length);
}

/**
 * @brief Envoie une réponse négative pour le service Security Access
 * @param sub_function : Sous-fonction concernée
 * @param nrc : Code de réponse négative (NRC)
 */
void send_negative_response_security_access(uint8_t sub_function, uint8_t nrc) {
    uint8_t response[3] = {0};

    response[0] = UDS_NEGATIVE_RESPONSE;         // Indique une réponse négative
    response[1] = UDS_SECURITY_ACCESS;  // Identifiant du service
    response[2] = nrc;          // NRC (code de réponse négative)

    send_can_message(response, 3);  // Envoyer la réponse via CAN
}

/************************************************CommunicationControl************************************************************/
// Implémentation du service Communication Control (0x28)
void uds_communication_control(uint8_t sub_function) {
    switch (sub_function) {
        case UDS_COMM_CONTROL_ENABLE_RX_AND_TX:
            // Activer Rx et Tx
            if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
                send_negative_response_communication_control(NRC_CONDITIONS_NOT_CORRECT);
                return;
            }
            send_positive_response_communication_control(sub_function);
            break;

        case UDS_COMM_CONTROL_ENABLE_RX_AND_DISABLE_TX:
            // Activer uniquement Rx, désactiver Tx
            if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
                send_negative_response_communication_control(NRC_CONDITIONS_NOT_CORRECT);
                return;
            }
            if (HAL_CAN_DeactivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
                send_negative_response_communication_control(NRC_CONDITIONS_NOT_CORRECT);
                return;
            }
            send_positive_response_communication_control(sub_function);
            break;

        case UDS_COMM_CONTROL_DISABLE_RX_AND_ENABLE_TX:
            // Désactiver Rx, activer Tx
            if (HAL_CAN_DeactivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
                send_negative_response_communication_control(NRC_CONDITIONS_NOT_CORRECT);
                return;
            }
            if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
                send_negative_response_communication_control(NRC_CONDITIONS_NOT_CORRECT);
                return;
            }
            send_positive_response_communication_control(sub_function);
            break;

        case UDS_COMM_CONTROL_DISABLE_RX_AND_TX:
            // Désactiver à la fois Rx et Tx
            if (HAL_CAN_DeactivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
                send_negative_response_communication_control(NRC_CONDITIONS_NOT_CORRECT);
                return;
            }
            send_positive_response_communication_control(sub_function);
            break;

        case UDS_COMM_CONTROL_ENABLE_RX_AND_TX_WITH_ENHANCED_INFO:
            // Activer Rx et Tx avec informations améliorées
            if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
                send_negative_response_communication_control(NRC_CONDITIONS_NOT_CORRECT);
                return;
            }
            // Ajoutez la logique pour les informations améliorées ici
            send_positive_response_communication_control(sub_function);
            break;

        case UDS_COMM_CONTROL_ENABLE_RX_WITH_ENHANCED_INFO:
            // Activer Rx avec informations améliorées
            if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
                send_negative_response_communication_control(NRC_CONDITIONS_NOT_CORRECT);
                return;
            }
            // Ajoutez la logique pour les informations améliorées ici
            send_positive_response_communication_control(sub_function);
            break;

        default:
            // Sous-fonction non supportée
            send_negative_response_communication_control(NRC_SUB_FUNCTION_NOT_SUPPORTED);
            break;
    }
}


// Envoie une réponse positive pour Communication Control
void send_positive_response_communication_control(uint8_t sub_function) {
    // Tableau de réponse contenant 2 octets
    uint8_t response[2];

    // Octet #1 : SID de réponse (0x68 = 0x28 + 0x40)
    response[0] = UDS_COMMUNICATION_CONTROL + 0x40;

    // Octet #2 : Echo de la sous-fonction envoyée par le client
    response[1] = sub_function;

    // Envoi du message CAN
    send_can_message(response, 2);
}

// Envoie une réponse négative pour Communication Control
void send_negative_response_communication_control(uint8_t nrc) {
    // Tableau de réponse contenant 3 octets
    uint8_t response[3];

    // Octet #1 : 0x7F indiquant une réponse négative
    response[0] = UDS_NEGATIVE_RESPONSE;

    // Octet #2 : Service ID (SID) pour Communication Control (0x28)
    response[1] = UDS_COMMUNICATION_CONTROL;

    // Octet #3 : Code de réponse négative (NRC)
    response[2] = nrc;

    // Envoi du message CAN
    send_can_message(response, 3);
}

/*****************************************************TesterPresent**************************************************************/
void uds_tester_present(uint8_t sub_function) {
    // Utilisation d'une macro pour vérifier la sous-fonction réservée
    if ((sub_function & UDS_TESTER_PRESENT_SUB_FUNCTION_MASK) != UDS_TESTER_PRESENT_ZERO_SUB_FUNCTION) {
        // Si la sous-fonction est différente de 0x00, elle est non supportée
        send_negative_response_tester_present(NRC_SUB_FUNCTION_NOT_SUPPORTED);
        return;
    }

    // Vérification du suppressPosRspMsgIndicationBit avec une macro
    if ((sub_function & UDS_SUPPRESS_POS_RSP_MSG_INDICATION_BIT) == UDS_SUPPRESS_POS_RSP_MSG_INDICATION_BIT) {
        // Si suppressPosRspMsgIndicationBit est à 1, aucune réponse ne doit être envoyée
        return;
    }

    // Si suppressPosRspMsgIndicationBit est à 0, envoyer une réponse positive
    send_positive_response_tester_present(sub_function);
}

void send_positive_response_tester_present(uint8_t sub_function) {
    // Tableau de réponse contenant 2 octets
    uint8_t response[2];

    // Octet #1 : SID de réponse pour TesterPresent (0x3E + 0x40 = 0x7E)
    response[0] = UDS_TESTER_PRESENT + 0x40;

    // Octet #2 : Echo de la sous-fonction (bits 6 à 0) envoyée par le client
    response[1] = sub_function & UDS_TESTER_PRESENT_SUB_FUNCTION_MASK;  // On ne prend que les bits 6 à 0

    // Envoi du message CAN
    send_can_message(response, 2);
}

void send_negative_response_tester_present(uint8_t nrc) {
    // Tableau de réponse contenant 3 octets
    uint8_t response[3];

    // Octet #1 : 0x7F indiquant une réponse négative
    response[0] = UDS_NEGATIVE_RESPONSE;

    // Octet #2 : Service ID (SID) pour Tester Present (0x3E)
    response[1] = UDS_TESTER_PRESENT;

    // Octet #3 : Code de réponse négative (NRC)
    response[2] = nrc;

    // Envoi du message CAN
    send_can_message(response, sizeof(response));
}


/*********************************************************access_timing_parameter**********************************************************/
// Exemple de paramètres de temporisation par défaut
TimingParameters timing_params = {
    .p2_server_max = 0x32,       // 50 ms
    .p2_star_server_max = 0x1F4  // 500 ms
};

// Fonction principale pour Access Timing Parameter
void uds_access_timing_parameter(uint8_t sub_function, uint8_t* data, uint8_t data_length) {
    if (!is_service_allowed(UDS_ACCESS_TIMING_PARAMETER)) {
        send_negative_response_access_timing(NRC_CONDITIONS_NOT_CORRECT);
        return;
    }

    switch (sub_function) {
        case UDS_TIMING_PARAMETER_READ:
            // Lire les paramètres de temporisation
            if (data_length != 0) {
                send_negative_response_access_timing(NRC_INCORRECT_MESSAGE_LENGTH);
                return;
            }
            read_timing_parameters();
            break;

        case UDS_TIMING_PARAMETER_WRITE:
            // Écrire de nouveaux paramètres de temporisation
            if (data_length != 4) {
                send_negative_response_access_timing(NRC_INCORRECT_MESSAGE_LENGTH);
                return;
            }
            write_timing_parameters(data, data_length);
            break;

        default:
            send_negative_response_access_timing(NRC_SUB_FUNCTION_NOT_SUPPORTED);
            break;
    }
}

// Fonction pour lire les paramètres de temporisation
void read_timing_parameters() {
    uint8_t response[6];

    // SID de réponse (0x83 + 0x40 = 0xC3)
    response[0] = UDS_ACCESS_TIMING_PARAMETER + 0x40;

    // P2Server_max
    response[1] = (timing_params.p2_server_max >> 8) & 0xFF; // High byte
    response[2] = timing_params.p2_server_max & 0xFF;        // Low byte

    // P2*Server_max
    response[3] = (timing_params.p2_star_server_max >> 8) & 0xFF; // High byte
    response[4] = timing_params.p2_star_server_max & 0xFF;        // Low byte

    // Envoyer la réponse CAN
    send_can_message(response, 5);
}

// Fonction pour écrire de nouveaux paramètres de temporisation
void write_timing_parameters(uint8_t* data, uint8_t data_length) {
    // Mise à jour des paramètres de temporisation avec les données reçues
    timing_params.p2_server_max = (data[0] << 8) | data[1];        // P2Server_max
    timing_params.p2_star_server_max = (data[2] << 8) | data[3];   // P2*Server_max

    // Envoyer une réponse positive
    send_positive_response_access_timing(UDS_TIMING_PARAMETER_WRITE);
}

void send_positive_response_access_timing(uint8_t sub_function) {
    uint8_t response[2];

    // SID de réponse pour AccessTimingParameter (0x83 + 0x40 = 0xC3)
    response[0] = UDS_ACCESS_TIMING_PARAMETER + 0x40;

    // Écho de la sous-fonction (bits 6 à 0)
    response[1] = sub_function & 0x7F;

    // Envoyer la réponse CAN
    send_can_message(response, 2);
}


void send_negative_response_access_timing(uint8_t nrc) {
    uint8_t response[3];

    // Octet #1 : 0x7F indiquant une réponse négative
    response[0] = UDS_NEGATIVE_RESPONSE;

    // Octet #2 : Service ID (SID) pour Access Timing Parameter (0x83)
    response[1] = UDS_ACCESS_TIMING_PARAMETER;

    // Octet #3 : Code de réponse négative (NRC)
    response[2] = nrc;

    // Envoyer le message CAN
    send_can_message(response, 3);
}

/***********************************************SecuredDataTransmission**********************************************************/
/**
 * @brief Implémente le service de transmission de données sécurisées (0x84)
 * @param data : Données à transmettre
 * @param data_length : Longueur des données
 */
SecuritySession current_session = {
    .session_id = UDS_SESSION_DEFAULT,
    .encryption_key = DEFAULT_ENCRYPTION_KEY
};

void uds_secured_data_transmission(uint8_t* data, uint8_t data_length) {
    // Vérification si le service est autorisé dans la session actuelle
    if (!is_service_allowed(UDS_SECURED_DATA_TRANSMISSION)) {
        send_negative_response_secured_data_transmission(NRC_CONDITIONS_NOT_CORRECT);
        return;
    }

    // Vérification de la longueur du message
    if (data_length > MAX_DATA_SIZE) {
        send_negative_response_secured_data_transmission(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // Chiffrement des données
    uint8_t encrypted_data[MAX_DATA_SIZE];
    encrypt_data(data, data_length, encrypted_data, current_session.encryption_key);

    // Envoi des données chiffrées
    secured_data_send(encrypted_data, data_length);

    // Données de réponse (à adapter selon vos besoins)
    uint8_t securityDataResponseRecord[MAX_DATA_SIZE];
    // Remplir le tableau securityDataResponseRecord avec des données appropriées

    // Réponse positive après transmission
    send_positive_response_secured_data_transmission(securityDataResponseRecord, data_length);
}


/**
 * @brief Chiffre les données à l'aide de la clé spécifiée
 * @param data : Données à chiffrer
 * @param length : Longueur des données
 * @param encrypted_data : Tableau où stocker les données chiffrées
 * @param key : Clé de chiffrement
 */
void encrypt_data(uint8_t* data, uint8_t length, uint8_t* encrypted_data, uint8_t* key) {
    // Exemple basique de chiffrement XOR (vous devez remplacer par un chiffrement plus robuste comme AES)
    for (uint8_t i = 0; i < length; i++) {
        encrypted_data[i] = data[i] ^ key[i % 16];
    }
}

/**
 * @brief Déchiffre les données à l'aide de la clé spécifiée
 * @param encrypted_data : Données chiffrées à déchiffrer
 * @param length : Longueur des données chiffrées
 * @param decrypted_data : Tableau où stocker les données déchiffrées
 * @param key : Clé de déchiffrement
 */
void decrypt_data(uint8_t* encrypted_data, uint8_t length, uint8_t* decrypted_data, uint8_t* key) {
    // Exemple basique de déchiffrement XOR (vous devez remplacer par un chiffrement plus robuste comme AES)
    for (uint8_t i = 0; i < length; i++) {
        decrypted_data[i] = encrypted_data[i] ^ key[i % 16];
    }
}

/**
 * @brief Envoie les données chiffrées via le bus CAN
 * @param encrypted_data : Données chiffrées à envoyer
 * @param length : Longueur des données chiffrées
 */
void secured_data_send(uint8_t* encrypted_data, uint8_t length) {
    // Vous pouvez ajuster les en-têtes CAN ici si nécessaire
    send_can_message(encrypted_data, length);
}

void send_positive_response_secured_data_transmission(uint8_t* securityDataResponseRecord, uint8_t data_length) {
    uint8_t response[MAX_DATA_SIZE + 1]; // +1 pour l'octet de SID

    // Byte 1: Secured Data Transmission Response SID (0xC4)
    response[0] = UDS_SECURED_DATA_TRANSMISSION + 0x40; // 0xC4

    // Bytes 2 to n: securityDataResponseRecord[]
    for (uint8_t i = 0; i < data_length; i++) {
        response[i + 1] = securityDataResponseRecord[i]; // Ajouter les paramètres de réponse
    }

    // Envoyer la réponse via CAN
    send_can_message(response, data_length + 1); // La longueur totale inclut SID + securityDataResponseRecord
}

void send_negative_response_secured_data_transmission(uint8_t nrc) {
    uint8_t response[3];

    // Byte 1: Negative Response SID (0x7F)
    response[0] = UDS_NEGATIVE_RESPONSE;

    // Byte 2: Secured Data Transmission Service ID (0x84)
    response[1] = UDS_SECURED_DATA_TRANSMISSION;

    // Byte 3: NRC (Code de réponse négative)
    response[2] = nrc;

    // Envoyer la réponse via CAN
    send_can_message(response, 3);
}
/***********************************************ControlDTCSetting****************************************************************/
// Drapeaux pour gérer l'état de mise à jour des bits DTC
bool dtc_update_enabled = true; // Par défaut, les bits DTC sont mis à jour

/**
 * @brief Service ControlDTCSetting (0x85)
 * @param sub_function : Sous-fonction (0x01 pour activer, 0x02 pour désactiver)
 */
void uds_control_dtc_setting(uint8_t sub_function) {
    // Vérification si le service est autorisé dans la session actuelle
    if (!is_service_allowed(UDS_CONTROL_DTC_SETTING)) {
        send_negative_response_control_dtc_setting(NRC_CONDITIONS_NOT_CORRECT);
        return;
    }

    // Vérifier si la sous-fonction est valide (0x01 ou 0x02)
    if (sub_function != UDS_CONTROL_DTC_SETTING_ON && sub_function != UDS_CONTROL_DTC_SETTING_OFF) {
        send_negative_response_control_dtc_setting(NRC_SUB_FUNCTION_NOT_SUPPORTED);
        return;
    }

    // Vérifier et traiter la sous-fonction
    if (sub_function == UDS_CONTROL_DTC_SETTING_ON) {
        if (!dtc_update_enabled) {
            dtc_update_enabled = true; // Activer la mise à jour des DTC
        }
        // Envoyer une réponse positive même si déjà activé
        send_positive_response_control_dtc_setting(sub_function);

    } else if (sub_function == UDS_CONTROL_DTC_SETTING_OFF) {
        if (dtc_update_enabled) {
            dtc_update_enabled = false; // Désactiver la mise à jour des DTC
        }
        // Envoyer une réponse positive même si déjà désactivé
        send_positive_response_control_dtc_setting(sub_function);
    }
}

/**
 * @brief Envoie une réponse positive pour le service ControlDTCSetting
 * @param sub_function : Sous-fonction (0x01 pour activer, 0x02 pour désactiver)
 */
void send_positive_response_control_dtc_setting(uint8_t sub_function) {
    uint8_t response[2] = {0};

    // Réponse SID pour indiquer un succès (ajout de 0x40 au service ID)
    response[0] = UDS_CONTROL_DTC_SETTING + 0x40; // 0xC5

    // Sous-fonction : Echo de la sous-fonction envoyée (0x01 ou 0x02)
    response[1] = sub_function;

    // Envoi de la réponse via CAN
    send_can_message(response, 2);
}

/**
 * @brief Envoie une réponse négative pour le service ControlDTCSetting
 * @param nrc : Code de réponse négative (NRC)
 */
void send_negative_response_control_dtc_setting(uint8_t nrc) {
    uint8_t response[3] = {0};

    // Réponse négative générique (0x7F)
    response[0] = UDS_NEGATIVE_RESPONSE;

    // Service concerné (0x85 pour ControlDTCSetting)
    response[1] = UDS_CONTROL_DTC_SETTING;

    // NRC (Code de réponse négative)
    response[2] = nrc;

    // Envoi de la réponse via CAN
    send_can_message(response, 3);
}

/*************************************************ResponseOnEvent****************************************************************/
Event events[MAX_EVENTS]; // Définition de la variable events

void uds_response_on_event(uint8_t sub_function, uint8_t* data, uint8_t data_length) {
    switch (sub_function) {
        case ROE_STOP_RESPONSE_ON_EVENT:
            // Appel de la fonction pour arrêter la réponse sur événement
            stop_response_on_event();
            break;

        case ROE_ON_DTC_STATUS_CHANGE:
            // Gestion de l'événement basé sur le changement de statut DTC
            on_dtc_status_change(data, data_length);
            break;

        case ROE_ON_TIMER_INTERRUPT:
            // Gestion de l'événement basé sur une interruption de timer
            on_timer_interrupt(data, data_length);
            break;

        case ROE_START_RESPONSE_ON_EVENT:
            // Démarrage de la réponse sur événement
            start_response_on_event();
            break;

        case ROE_CLEAR_RESPONSE_ON_EVENT:
            // Nettoyage des événements configurés
            clear_response_on_event();
            break;

        default:
            // Envoi d'une réponse négative si la sous-fonction n'est pas supportée
            send_negative_response_roe(sub_function, NRC_SUB_FUNCTION_NOT_SUPPORTED);
            break;
    }
}

void stop_response_on_event() {
    // Désactiver tous les événements actifs
    for (int i = 0; i < MAX_EVENTS; i++) {
        events[i].isActive = false;
    }

    // Envoyer une réponse positive avec des valeurs par défaut
    send_positive_response_roe(ROE_STOP_RESPONSE_ON_EVENT, 0x00, 0x00, 0x00);
}

void on_dtc_status_change(uint8_t* data, uint8_t data_length) {
    // Vérifier la longueur des données
    if (data_length != 1) {
        send_negative_response_roe(ROE_ON_DTC_STATUS_CHANGE, NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // Configurer un nouvel événement pour DTC Status Change
    for (int i = 0; i < MAX_EVENTS; i++) {
        if (!events[i].isActive) {
            events[i].eventType = ROE_ON_DTC_STATUS_CHANGE;
            events[i].isActive = true;
            events[i].serviceToRespondTo = UDS_READ_DTC_INFORMATION;
            break;
        }
    }

    // Envoyer une réponse positive
    send_positive_response_roe(ROE_ON_DTC_STATUS_CHANGE, 0x01, 0x01, 0x00);
}

void on_timer_interrupt(uint8_t* data, uint8_t data_length) {
    // Vérifier la longueur des données
    if (data_length != 1) {
        send_negative_response_roe(ROE_ON_TIMER_INTERRUPT, NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // Configurer un événement pour Timer Interrupt
    for (int i = 0; i < MAX_EVENTS; i++) {
        if (!events[i].isActive) {
            events[i].eventType = ROE_ON_TIMER_INTERRUPT;
            events[i].isActive = true;
            events[i].serviceToRespondTo = UDS_READ_DATA_BY_IDENTIFIER;
            break;
        }
    }

    // Envoyer une réponse positive
    send_positive_response_roe(ROE_ON_TIMER_INTERRUPT, 0x01, 0x01, 0x00);
}


void start_response_on_event() {
    // Activer les événements configurés
    for (int i = 0; i < MAX_EVENTS; i++) {
        if (events[i].isActive) {
            // Lancer la logique de gestion des événements
        }
    }

    // Envoyer une réponse positive
    send_positive_response_roe(ROE_START_RESPONSE_ON_EVENT, 0x00, 0x00, 0x00);
}

void clear_response_on_event() {
    // Réinitialiser tous les événements
    for (int i = 0; i < MAX_EVENTS; i++) {
        events[i].isActive = false;
        events[i].eventType = 0;
        events[i].serviceToRespondTo = 0;
    }

    // Envoyer une réponse positive
    send_positive_response_roe(ROE_CLEAR_RESPONSE_ON_EVENT, 0x00, 0x00, 0x00);
}

void send_positive_response_roe(uint8_t sub_function, uint8_t eventType, uint8_t numberOfIdentifiedEvents, uint8_t eventWindowTime) {
    // Initialiser une taille suffisante pour inclure tous les paramètres (3 octets de base + les autres données)
    uint8_t response[6] = {0};

    // Byte 1: ResponseOnEvent Response SID (0x86 + 0x40 = 0xC6)
    response[0] = UDS_RESPONSE_ON_EVENT + 0x40;  // 0xC6

    // Byte 2: eventType (echo de la sous-fonction dans la requête)
    response[1] = eventType;

    // Byte 3: numberOfIdentifiedEvents (0x00 si non applicable ou nombre réel d'événements)
    response[2] = numberOfIdentifiedEvents;

    // Byte 4: eventWindowTime (temps de la fenêtre d'événement)
    response[3] = eventWindowTime;

    // Envoi du message CAN avec la réponse positive (4 octets dans ce cas)
    send_can_message(response, 4);
}

void send_negative_response_roe(uint8_t sub_function, uint8_t nrc) {
    uint8_t response[3] = {0};

    // Byte 1: NRC indicator (0x7F for negative response)
    response[0] = UDS_NEGATIVE_RESPONSE;

    // Byte 2: Service ID (SID of ResponseOnEvent, which is 0x86)
    response[1] = UDS_RESPONSE_ON_EVENT;

    // Byte 3: Negative Response Code (NRC) from Table 108
    response[2] = nrc;

    // Send the negative response via CAN
    send_can_message(response, 3);
}

/****************************************************LinkControl****************************************************************/
LinkControl link_control;

void uds_link_control(uint8_t sub_function, uint8_t* data, uint8_t data_length) {
    switch (sub_function) {
        case UDS_LINK_CONTROL_VERIFY_MODE_TRANSITION_FIXED:
            if (data_length != 1) {
                send_negative_response_link_control(sub_function, NRC_INCORRECT_MESSAGE_LENGTH);
                return;
            }
            link_control.modeIdentifier = data[0];
            send_positive_response_link_control(sub_function);
            break;

        case UDS_LINK_CONTROL_VERIFY_MODE_TRANSITION_SPECIFIC:
            if (data_length != 3) {
                send_negative_response_link_control(sub_function, NRC_INCORRECT_MESSAGE_LENGTH);
                return;
            }
            memcpy(link_control.linkRecord, data, 3);
            send_positive_response_link_control(sub_function);
            break;

        case UDS_LINK_CONTROL_TRANSITION_MODE:
            if (link_control.modeIdentifier == 0) {
                send_negative_response_link_control(sub_function, NRC_REQUEST_SEQUENCE_ERROR);
                return;
            }
            // Effectuer la transition ici en fonction de modeIdentifier ou linkRecord
            send_positive_response_link_control(sub_function);
            break;

        default:
            send_negative_response_link_control(sub_function, NRC_SUB_FUNCTION_NOT_SUPPORTED);
            break;
    }
}

void send_positive_response_link_control(uint8_t sub_function) {
    uint8_t response[2] = {0};

    response[0] = UDS_LINK_CONTROL + 0x40;  // Réponse SID
    response[1] = sub_function;  // Echo de la sous-fonction

    send_can_message(response, 2);
}

void send_negative_response_link_control(uint8_t sub_function, uint8_t nrc) {
    uint8_t response[3] = {0};

    response[0] = UDS_NEGATIVE_RESPONSE; // Réponse négative
    response[1] = UDS_LINK_CONTROL; // Service concerné (LinkControl)
    response[2] = nrc; // NRC (Code de réponse négative)

    send_can_message(response, 3);
}



