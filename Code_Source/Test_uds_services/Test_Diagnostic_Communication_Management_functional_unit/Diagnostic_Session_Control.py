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

# Scénarios de test
def test_default_session():
    print("Test: Default Session (dans la session par défaut)")
    command = bytes([0x10, 0x01])  # Service 0x10, sous-fonction 0x01
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_switch_to_programming_session():
    print("Test: Passer à la session de programmation")
    command = bytes([0x10, 0x02])  # Service 0x10, sous-fonction 0x02
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_switch_to_extended_session():
    print("Test: Passer à la session de diagnostic étendu")
    command = bytes([0x10, 0x03])  # Service 0x10, sous-fonction 0x03
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_unsupported_sub_function():
    print("Test: Sous-fonction non supportée")
    command = bytes([0x10, 0x05])  # Sous-fonction non définie
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    command = bytes([0x10])  # Message incomplet (moins de 2 octets)
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_conditions_not_correct():
    print("Test: Conditions non correctes (service non autorisé)")
    command = bytes([0x27, 0x01])  # Service non autorisé en session par défaut
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour Diagnostic Session Control...")
        test_default_session()
        test_switch_to_programming_session()
        test_switch_to_extended_session()
        test_unsupported_sub_function()
        test_incorrect_message_length()
        test_conditions_not_correct()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
