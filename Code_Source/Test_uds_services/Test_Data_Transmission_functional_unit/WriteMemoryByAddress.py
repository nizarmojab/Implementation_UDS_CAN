import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'  # Modifier selon le port utilisé
BAUD_RATE = 115200
TIMEOUT = 1

# Initialisation de la connexion série
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la réponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)
    response = ser.read(ser.in_waiting or 1)
    return response

# Test d'un DID supporté avec une écriture réussie
def test_write_supported_did():
    print("Test : Ecriture dans un DID supporté")
    command = bytes([0x2E, 0x01, 0x02, 0xAB, 0xCD])  # Exemple de commande pour SUPPORTED_DID_1
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : Réponse positive 0x6E 0x01 0x02

# Test d'un DID non supporté pour l'écriture
def test_write_unsupported_did():
    print("Test : Ecriture dans un DID non supporté")
    command = bytes([0x2E, 0xFF, 0xFF, 0x12, 0x34])  # DID non supporté
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x31 (Request Out Of Range)

# Test avec des conditions de session non correctes
def test_incorrect_session():
    print("Test : Conditions de session non correctes")
    command = bytes([0x2E, 0x01, 0x02, 0x56, 0x78])  # SUPPORTED_DID_1 mais session incorrecte
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x22 (Conditions Not Correct)

# Test avec un accès sécurisé requis, mais non accordé
def test_security_access_required():
    print("Test : Accès sécurisé requis mais non accordé")
    command = bytes([0x2E, 0x01, 0x02, 0x9A, 0xBC])  # SUPPORTED_DID_1 nécessitant un accès sécurisé
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x33 (Security Access Denied)

# Test d'un message avec longueur incorrecte
def test_incorrect_message_length():
    print("Test : Longueur de message incorrecte")
    command = bytes([0x2E, 0x01])  # Commande incomplète
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x13 (Incorrect Message Length Or Invalid Format)

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour WriteDataByIdentifier...")
        
        # Scénarios de réponse positive
        test_write_supported_did()
        
        # Tests de scénarios de réponse négative (NRC)
        test_write_unsupported_did()
        test_incorrect_session()
        test_security_access_required()
        test_incorrect_message_length()
        
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
