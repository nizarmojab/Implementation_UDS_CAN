import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'  # Remplacez par le port de votre STM32
BAUD_RATE = 115200
TIMEOUT = 1

# Initialisation de la connexion série
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la réponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)  # Pause pour laisser le STM32 traiter la commande
    response = ser.read(ser.in_waiting or 1)  # Lire les données disponibles
    return response

# Scénarios de test
def test_request_seed_locked():
    print("Test: Demande de seed en état 'Locked'")
    command = bytes([0x27, 0x01])  # Demande de seed, sous-fonction 0x01
    response = send_command(command)
    print(f"Réponse attendue : Seed non nul, reçu : {response.hex()}")

def test_send_key_correct():
    print("Test: Envoi de la clé correcte pour le seed")
    command = bytes([0x27, 0x02, 0xC9, 0xA9])  # Exemple de clé correcte pour un seed
    response = send_command(command)
    print(f"Réponse attendue : Réponse positive, reçu : {response.hex()}")

def test_send_key_incorrect():
    print("Test: Envoi d'une clé incorrecte")
    command = bytes([0x27, 0x02, 0x00, 0x00])  # Clé incorrecte
    response = send_command(command)
    print(f"Réponse attendue : NRC Invalid Key (0x35), reçu : {response.hex()}")

def test_request_seed_unlocked():
    print("Test: Demande de seed en état 'Unlocked'")
    command = bytes([0x27, 0x01])  # Demande de seed, sous-fonction 0x01
    response = send_command(command)
    print(f"Réponse attendue : Seed nul, reçu : {response.hex()}")

def test_send_key_without_seed():
    print("Test: Demande de clé sans demande de seed préalable")
    command = bytes([0x27, 0x02])  # Envoi de clé sans demande de seed
    response = send_command(command)
    print(f"Réponse attendue : NRC Request Sequence Error (0x24), reçu : {response.hex()}")

def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte pour la clé")
    command = bytes([0x27, 0x02, 0xC9])  # Message incomplet pour la clé
    response = send_command(command)
    print(f"Réponse attendue : NRC Incorrect Message Length (0x13), reçu : {response.hex()}")

def test_delay_after_failed_attempts():
    print("Test: Délai requis après plusieurs tentatives infructueuses")
    # Envoyer plusieurs tentatives incorrectes pour déclencher le délai
    for _ in range(3):  # Ajustez le nombre de tentatives en fonction de la configuration du délai
        test_send_key_incorrect()

    # Attendre un certain temps et tester de nouveau la demande de seed
    time.sleep(5)  # Exemple d'attente avant de tester
    command = bytes([0x27, 0x01])  # Demande de seed
    response = send_command(command)
    print(f"Réponse attendue : NRC Required Time Delay Not Expired (0x37), reçu : {response.hex()}")

def test_multiple_security_levels():
    print("Test: Demande de seed pour plusieurs niveaux de sécurité")
    for sub_function in [0x01, 0x03, 0x05]:  # Différents niveaux de sécurité
        command = bytes([0x27, sub_function])  # Demande de seed pour chaque niveau
        response = send_command(command)
        print(f"Niveau {sub_function} - Réponse attendue : Seed pour le niveau, reçu : {response.hex()}")
        # Envoyer la clé correspondante ici pour chaque niveau de sécurité

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour Security Access...")
        test_request_seed_locked()
        test_send_key_correct()
        test_send_key_incorrect()
        test_request_seed_unlocked()
        test_send_key_without_seed()
        test_incorrect_message_length()
        test_delay_after_failed_attempts()
        test_multiple_security_levels()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
