/*
 * Routine_functional_unit.c
 *
 *  Created on: 21 oct. 2024
 *      Author: PC
 */
#include "Routine_functional_unit.h"
#include "uds_services.c"


/*******************************************************RoutineControl******************************************************/
// Fonction pour gérer la commande RoutineControl
void uds_routine_control(RoutineControlRequest_t *request, RoutineControlResponse_t *response) {
    // 1. Vérification de la longueur minimale du message
    if (sizeof(*request) < (sizeof(request->subFunction) + sizeof(request->routineIdentifier))) {
        send_negative_response_routine_control(NRC_INCORRECT_MESSAGE_LENGTH);
        return;
    }

    // 2. Vérification du Routine Identifier (RID)
    if (!is_routine_supported(request->routineIdentifier)) {
        send_negative_response_routine_control(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // 3. Vérification de la Sécurité du RID
    if (!is_security_granted_for_routine(request->routineIdentifier)) {
        send_negative_response_routine_control(NRC_SECURITY_ACCESS_DENIED);
        return;
    }

    // 4. Vérification de la Sous-Fonction
    if (!is_sub_function_supported(request->subFunction)) {
        send_negative_response_routine_control(NRC_SUB_FUNCTION_NOT_SUPPORTED);
        return;
    }

    // 5. Vérification de la Longueur Totale
    // (Ajoutez ici si nécessaire, selon les données supplémentaires)

    // 6. Validation des Données dans routineControlOptionRecord
    if (!validate_routine_control_option(request->routineControlOption))  {
        send_negative_response_routine_control(NRC_REQUEST_OUT_OF_RANGE);
        return;
    }

    // 7. Vérifications Supplémentaires (conditions spécifiques)
    // (Ajoutez ici si nécessaire)

    // Traitement des sous-fonctions
    switch (request->subFunction) {
        case 0x01: // Start Routine
            // Logique pour démarrer la routine
            // start_routine(request->routineIdentifier);
            break;

        case 0x02: // Stop Routine
            // Logique pour arrêter la routine
            // stop_routine(request->routineIdentifier);
            break;

        case 0x03: // Request Routine Results
            // Logique pour demander les résultats de la routine
            // request_routine_results(request->routineIdentifier);
            break;

        default:
            send_negative_response_routine_control(NRC_SUB_FUNCTION_NOT_SUPPORTED);
            return;
    }

    // Préparer la réponse positive
    response->SID = UDS_RESPONSE_ROUTINE_CONTROL;
    response->routineControlType = request->subFunction;
    response->routineIdentifier = request->routineIdentifier;
    // Remplir routineInfo et routineStatusRecord selon la logique de la routine
    // response->routineInfo = get_routine_info(request->routineIdentifier);

    // Envoyer la réponse positive
    send_positive_response_routine_control(response);
}


// Fonction pour envoyer une réponse positive
void send_positive_response_routine_control(RoutineControlResponse_t *response) {
    // Envoi du message via CAN
    send_can_message((uint8_t *)response, sizeof(RoutineControlResponse_t));
    // send_uart_message((uint8_t *)response, sizeof(RoutineControlResponse_t));
}

// Fonction pour envoyer une réponse négative
void send_negative_response_routine_control(uint8_t nrc) {
    uint8_t response[2];
    response[0] = UDS_RESPONSE_ROUTINE_CONTROL; // SID de réponse
    response[1] = nrc; // NRC

    send_can_message(response, sizeof(response));
    // send_uart_message(response, sizeof(response));
}

// Fonction pour vérifier si le routineIdentifier est supporté
bool is_routine_supported(uint16_t routineIdentifier) {
    // Implémentez votre logique pour vérifier si le routineIdentifier est supporté
    return true; // Remplacez ceci par la logique appropriée
}

// Fonction pour vérifier la sécurité pour le routineIdentifier
bool is_security_granted_for_routine(uint16_t routineIdentifier) {
    // Implémentez votre logique pour vérifier si la sécurité est accordée pour ce routineIdentifier
    return true; // Remplacez ceci par la logique appropriée
}

// Fonction pour vérifier si la sous-fonction est supportée
bool is_sub_function_supported(uint8_t subFunction) {
    // Implémentez votre logique pour vérifier si la sous-fonction est supportée
    return true; // Remplacez ceci par la logique appropriée
}

// Fonction pour valider les données dans routineControlOptionRecord
bool validate_routine_control_option(uint8_t option) {
    // Implémentez votre logique pour valider l'option
    return true; // Modifier selon votre logique
}
