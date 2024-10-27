import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'  # Remplacez par le port correct de votre STM32
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

# Test de vérification de transition de mode avec un paramètre fixe
def test_verify_mode_transition_fixed():
    print("Test: Vérification de la transition de mode avec paramètre fixe")
    command = bytes([0x87, 0x01, 0x05])  # SID 0x87, sub_function = 0x01, modeIdentifier = 0x05
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente de 0xC7 avec la sous-fonction écho

# Test de vérification de transition de mode avec un paramètre spécifique
def test_verify_mode_transition_specific():
    print("Test: Vérification de la transition de mode avec paramètre spécifique")
    command = bytes([0x87, 0x02, 0x02, 0x49, 0xF0])  # SID 0x87, sub_function = 0x02, linkRecord = 0x0249F0
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente de 0xC7 avec la sous-fonction écho

# Test de transition de mode après vérification
def test_transition_mode():
    print("Test: Transition de mode")
    command = bytes([0x87, 0x03])  # SID 0x87, sub_function = 0x03 pour transition
    response = send_command(command)
    print(f"Réponse : {response.hex()}")  # Attente de 0xC7 avec la sous-fonction écho ou absence de réponse

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour LinkControl...")
        test_verify_mode_transition_fixed()
        test_verify_mode_transition_specific()
        test_transition_mode()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
