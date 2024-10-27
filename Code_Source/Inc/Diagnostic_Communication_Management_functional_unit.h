#ifndef DIAGNOSTIC_COMMUNICATION_MANAGEMENT_FUNCTIONAL_UNIT_H_
#define DIAGNOSTIC_COMMUNICATION_MANAGEMENT_FUNCTIONAL_UNIT_H_

#include "uds_services.h"
// Définitions des tailles
#define MAX_DATA_SIZE 64
#define MAX_EVENTS 10
#define DEFAULT_ENCRYPTION_KEY {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, \
                                0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}


// Identifiants de Service UDS pour cette unité fonctionnelle
#define UDS_DIAGNOSTIC_SESSION_CONTROL   0x10
#define UDS_ECU_RESET                    0x11
#define UDS_SECURITY_ACCESS              0x27
#define UDS_COMMUNICATION_CONTROL        0x28
#define UDS_TESTER_PRESENT               0x3E
#define UDS_ACCESS_TIMING_PARAMETER      0x83
#define UDS_SECURED_DATA_TRANSMISSION    0x84
#define UDS_CONTROL_DTC_SETTING          0x85
#define UDS_RESPONSE_ON_EVENT            0x86
#define UDS_LINK_CONTROL                 0x87

// Types de session pour DIAGNOSTIC_SESSION_CONTROL
#define UDS_SESSION_DEFAULT                             0x01 // Session par défaut
#define UDS_SESSION_PROGRAMMING                         0x02 // Session de programmation
#define UDS_SESSION_EXTENDED_DIAGNOSTIC                 0x03 // Session de diagnostic étendue
#define UDS_SESSION_SAFETY_SYSTEM                       0x04 // Session du système de sécurité
#define UDS_SESSION_ISO_SAE_RESERVED                    0x00 // Bits réservés par ISO SAE (0x00)
#define UDS_SESSION_VEHICLE_MANUFACTURER_SPECIFIC       0x40 // Session spécifique au constructeur du véhicule
#define UDS_SESSION_SYSTEM_SUPPLIER_SPECIFIC            0x60 // Session spécifique au fournisseur de système


// Sous-fonctions pour ECU Reset
#define UDS_RESET_TYPE_HARD_RESET                       0x01
#define UDS_RESET_TYPE_SOFT_RESET                       0x03
#define UDS_RESET_TYPE_ENABLE_RAPID_POWER_SHUTDOWN      0x04
#define UDS_RESET_TYPE_DISABLE_RAPID_POWER_SHUTDOWN     0x05

// Sous-fonctions pour Security Access
#define UDS_SECURITY_ACCESS_REQUEST_SEED_LVL1           0x01
#define UDS_SECURITY_ACCESS_SEND_KEY_LVL1               0x02
#define UDS_SECURITY_ACCESS_REQUEST_SEED_LVL2           0x03
#define UDS_SECURITY_ACCESS_SEND_KEY_LVL2               0x04

// Sous-fonctions pour Communication Control
#define UDS_COMM_CONTROL_ENABLE_RX_AND_TX                    0x00
#define UDS_COMM_CONTROL_ENABLE_RX_AND_DISABLE_TX            0x01
#define UDS_COMM_CONTROL_DISABLE_RX_AND_ENABLE_TX            0x02
#define UDS_COMM_CONTROL_DISABLE_RX_AND_TX                   0x03
#define UDS_COMM_CONTROL_ENABLE_RX_AND_TX_WITH_ENHANCED_INFO 0x04
#define UDS_COMM_CONTROL_ENABLE_RX_WITH_ENHANCED_INFO        0x05

// Sous-fonctions pour Tester Present
#define UDS_TESTER_PRESENT_ZERO_SUB_FUNCTION    0x00 // Sous-fonction 0x00 (autorisé)
#define UDS_TESTER_PRESENT_RESERVED_MIN         0x01 // Sous-fonctions 0x01 à 0x7F (réservé ISO)
#define UDS_TESTER_PRESENT_SUB_FUNCTION_MASK    0x7F // Masque pour vérifier la sous-fonction
#define UDS_SUPPRESS_POS_RSP_MSG_INDICATION_BIT 0x80 // Masque pour suppressPosRspMsgIndicationBit (bit 7)
#define UDS_ZERO_SUB_FUNCTION                   0x00
#define UDS_SUPPRESS_POS_RSP_BIT                0x80

// Sous-fonctions pour Access Timing Parameter
#define UDS_TIMING_PARAMETER_READ        0x01 // Lire les paramètres de temporisation
#define UDS_TIMING_PARAMETER_WRITE       0x02 // Écrire de nouveaux paramètres de temporisation

// Sous-fonctions pour ControlDTCSetting
#define UDS_CONTROL_DTC_SETTING_ON       0x01  // Activer la mise à jour des DTC
#define UDS_CONTROL_DTC_SETTING_OFF      0x02  // Désactiver la mise à jour des DTC

// Sous-fonctions pour ResponseOnEvent
#define ROE_STOP_RESPONSE_ON_EVENT       0x00
#define ROE_ON_DTC_STATUS_CHANGE         0x01
#define ROE_ON_TIMER_INTERRUPT           0x02
#define ROE_START_RESPONSE_ON_EVENT      0x05
#define ROE_CLEAR_RESPONSE_ON_EVENT      0x06

// Sous-fonctions pour LinkControl
#define UDS_LINK_CONTROL_VERIFY_MODE_TRANSITION_FIXED     0x01
#define UDS_LINK_CONTROL_VERIFY_MODE_TRANSITION_SPECIFIC  0x02
#define UDS_LINK_CONTROL_TRANSITION_MODE                  0x03

// Structure de session UDS
typedef struct {
    uint8_t current_session;
    bool security_access_granted;
} UDS_Session;

// Structure pour les paramètres de temporisation
typedef struct {
    uint16_t p2_server_max;
    uint16_t p2_star_server_max;
} TimingParameters;

// Structures de données pour la transmission sécurisée
typedef struct {
    uint8_t encrypted_data[MAX_DATA_SIZE];
    uint8_t data_length;
} SecuredDataPacket;

typedef struct {
    uint8_t session_id;
    uint8_t encryption_key[16];  // Clé pour le chiffrement/déchiffrement
} SecuritySession;

// Structure d'événement pour ResponseOnEvent
typedef struct {
    uint8_t eventType;
    uint8_t serviceToRespondTo;
    bool isActive;
    uint32_t eventWindowTime; // Temps pendant lequel l'événement est actif
} Event;

// Structure pour le service LinkControl
typedef struct {
    uint8_t modeIdentifier;
    uint8_t linkRecord[3]; // Pour stocker les paramètres spécifiques si nécessaire
} LinkControl;


// Déclaration des variables de seed et key
extern uint8_t security_seed_lvl1;
extern uint8_t security_key_lvl1;
extern uint8_t security_seed_lvl2;
extern uint8_t security_key_lvl2;

// Déclaration des DIDs supportés
#define SUPPORTED_DID_1 0x1001
#define SUPPORTED_DID_2 0x1002
#define SUPPORTED_DID_3 0x1003

// Initialisation de la session
extern UDS_Session uds_session;
extern SecuritySession current_session;
extern Event events[MAX_EVENTS];
extern LinkControl link_control;


bool is_service_allowed(uint8_t service_id);
void uds_diagnostic_session_control(uint8_t session_type);
void send_negative_response_diagnostic_control(uint8_t service_id, uint8_t nrc);
void send_positive_response_diagnostic_control(uint8_t service_id);
void reset_default_session(void);
void reset_events(void);
void deactivate_extended_services(void);

void uds_ecu_reset(uint8_t resetType);
void send_negative_response_ecu_reset(uint8_t service_id, uint8_t nrc);
void enable_rapid_power_shutdown(void);
void disable_rapid_power_shutdown(void);
void hard_reset();
void soft_reset();
void send_negative_response_ecu_reset(uint8_t service_id, uint8_t nrc);


void uds_security_access(uint8_t sub_function, uint8_t *data, uint8_t data_length);
void send_seed(uint8_t level);
bool verify_key(uint8_t level, uint8_t* key);
void send_positive_response_security_access(uint8_t sub_function);
void send_negative_response_security_access(uint8_t sub_function, uint8_t nrc);

void uds_communication_control(uint8_t sub_function);
void send_positive_response_communication_control(uint8_t sub_function);
void send_negative_response_communication_control(uint8_t nrc);

void uds_tester_present(uint8_t sub_function);
void send_positive_response_tester_present(uint8_t sub_function);
void send_negative_response_tester_present(uint8_t nrc);

void uds_access_timing_parameter(uint8_t sub_function, uint8_t* data, uint8_t data_length);
void send_positive_response_access_timing(uint8_t sub_function);
void send_negative_response_access_timing(uint8_t nrc);
void read_timing_parameters(void);
void write_timing_parameters(uint8_t* data, uint8_t data_length);


void uds_secured_data_transmission(uint8_t* data, uint8_t data_length);
void encrypt_data(uint8_t* data, uint8_t length, uint8_t* encrypted_data, uint8_t* key);
void decrypt_data(uint8_t* encrypted_data, uint8_t length, uint8_t* decrypted_data, uint8_t* key);
void secured_data_send(uint8_t* encrypted_data, uint8_t length);
void send_positive_response_secured_data_transmission(uint8_t* securityDataResponseRecord, uint8_t data_length);
void send_negative_response_secured_data_transmission(uint8_t nrc);

void uds_control_dtc_setting(uint8_t sub_function);
void send_positive_response_control_dtc_setting(uint8_t sub_function);
void send_negative_response_control_dtc_setting(uint8_t nrc);

void uds_response_on_event(uint8_t sub_function, uint8_t* data, uint8_t data_length);
void stop_response_on_event();
void on_dtc_status_change(uint8_t* data, uint8_t data_length);
void on_timer_interrupt(uint8_t* data, uint8_t data_length);
void start_response_on_event();
void clear_response_on_event();
void send_positive_response_roe(uint8_t sub_function, uint8_t eventType, uint8_t numberOfIdentifiedEvents, uint8_t eventWindowTime);
void send_negative_response_roe(uint8_t sub_function, uint8_t nrc);


void uds_link_control(uint8_t sub_function, uint8_t* data, uint8_t data_length);
void send_positive_response_link_control(uint8_t sub_function);
void send_negative_response_link_control(uint8_t sub_function, uint8_t nrc);

#endif /* DIAGNOSTIC_COMMUNICATION_MANAGEMENT_FUNCTIONAL_UNIT_H_ */
