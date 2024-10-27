import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'  # Remplacez par le port utilisé par votre STM32
BAUD_RATE = 115200
TIMEOUT = 1  # Timeout de 1 seconde

# Initialisation de la connexion série
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la réponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)  # Pause pour laisser le STM32 traiter la commande
    response = ser.read(ser.in_waiting or 1)  # Lire les données disponibles
    return response

# Scénarios de test pour AccessTimingParameter (0x83)
def test_read_extended_timing():
    print("Test: Lire les paramètres de temporisation étendus (sub-function 0x01)")
    command = bytes([0x83, 0x01])  # SID 0x83, sous-fonction 0x01
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente d'une réponse avec le SID 0xC3

def test_set_timing_to_default():
    print("Test: Paramétrer les valeurs par défaut des paramètres de temporisation (sub-function 0x02)")
    command = bytes([0x83, 0x02])  # SID 0x83, sous-fonction 0x02
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente d'une réponse positive avec le SID 0xC3

def test_read_active_timing_parameters():
    print("Test: Lire les paramètres de temporisation actifs (sub-function 0x03)")
    command = bytes([0x83, 0x03])  # SID 0x83, sous-fonction 0x03
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente d'une réponse avec le SID 0xC3

def test_set_custom_timing_parameters():
    print("Test: Définir des paramètres de temporisation personnalisés (sub-function 0x04)")
    # Exemple de timing personnalisés
    command = bytes([0x83, 0x04, 0x00, 0x32, 0x01, 0xF4])  # SID 0x83, sous-fonction 0x04, suivi des valeurs P2 et P2*
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente d'une réponse avec le SID 0xC3

# Scénarios de test pour les erreurs NRC
def test_unsupported_sub_function():
    print("Test: Sous-fonction non supportée (NRC 0x12)")
    command = bytes([0x83, 0x05])  # Sous-fonction non définie
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente d'une réponse négative avec NRC 0x12

def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte (NRC 0x13)")
    command = bytes([0x83])  # Message incomplet (moins de 2 octets)
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente d'une réponse négative avec NRC 0x13

def test_conditions_not_correct():
    print("Test: Conditions non correctes (NRC 0x22)")
    # Exemple de condition non respectée (modifier si nécessaire pour votre configuration)
    command = bytes([0x83, 0x04, 0xFF, 0xFF])  # Faux timing parameters pour déclencher NRC 0x22
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente d'une réponse négative avec NRC 0x22

def test_request_out_of_range():
    print("Test: Paramètre hors de la plage autorisée (NRC 0x31)")
    # Envoie de paramètres de timing non valides
    command = bytes([0x83, 0x04, 0xFF, 0xFF, 0xFF, 0xFF])  # Valeurs hors de portée pour déclencher NRC 0x31
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente d'une réponse négative avec NRC 0x31

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour AccessTimingParameter...")
        test_read_extended_timing()
        test_set_timing_to_default()
        test_read_active_timing_parameters()
        test_set_custom_timing_parameters()
        test_unsupported_sub_function()
        test_incorrect_message_length()
        test_conditions_not_correct()
        test_request_out_of_range()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
