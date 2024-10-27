import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'  # Remplacez par le port utilisé par votre STM32
BAUD_RATE = 115200
TIMEOUT = 1

# Initialisation de la connexion série
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la réponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)  # Pause pour laisser le STM32 traiter la commande
    response = ser.read(ser.in_waiting or 1)
    return response

# Scénarios positifs pour ReadMemoryByAddress
def test_read_memory_4byte_address():
    print("Test: Lecture mémoire avec adresse sur 4 octets")
    command = bytes([0x23, 0x24, 0x20, 0x48, 0x13, 0x92, 0x01, 0x03])  # Adresse 0x20481392, taille 0x0103
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : Réponse 0x63 avec les données lues en mémoire

def test_read_memory_2byte_address():
    print("Test: Lecture mémoire avec adresse sur 2 octets")
    command = bytes([0x23, 0x12, 0x48, 0x13, 0x05])  # Adresse 0x4813, taille 0x05
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : Réponse 0x63 avec les données lues en mémoire

def test_read_memory_3byte_address():
    print("Test: Lecture mémoire avec adresse sur 3 octets")
    command = bytes([0x23, 0x23, 0x20, 0x48, 0x13, 0x00, 0x03])  # Adresse 0x204813, taille 0x03
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : Réponse 0x63 avec les données lues en mémoire

# Tests pour les codes de réponse négative (NRC)
def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    command = bytes([0x23])  # Message trop court
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x13 (incorrectMessageLengthOrInvalidFormat)

def test_request_out_of_range():
    print("Test: Adresse ou taille hors de portée")
    command = bytes([0x23, 0x24, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x02])  # Adresse invalide
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x31 (requestOutOfRange)

def test_conditions_not_correct():
    print("Test: Conditions non correctes pour la requête")
    command = bytes([0x23, 0x24, 0x20, 0x48, 0x13, 0x92, 0x01, 0x03])  # Cas où la condition n'est pas respectée
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x22 (conditionsNotCorrect)

def test_security_access_denied():
    print("Test: Accès sécurisé requis et refusé")
    command = bytes([0x23, 0x24, 0x20, 0x48, 0x13, 0x92, 0x01, 0x03])  # Zone mémoire sécurisée sans accès
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x33 (SecurityAccessDenied)

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour ReadMemoryByAddress...")
        
        # Scénarios positifs
        test_read_memory_4byte_address()
        test_read_memory_2byte_address()
        test_read_memory_3byte_address()
        
        # Tests de scénarios de réponse négative (NRC)
        test_incorrect_message_length()
        test_request_out_of_range()
        test_conditions_not_correct()
        test_security_access_denied()
        
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
