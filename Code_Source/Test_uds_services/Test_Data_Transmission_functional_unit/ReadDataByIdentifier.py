import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'  # Remplacez par le port correspondant à votre STM32
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

# Tests de scénario positifs pour ReadDataByIdentifier
def test_read_single_did():
    print("Test: Lecture d'un DID unique")
    command = bytes([0x22, 0xF1, 0x90])  # SID 0x22, DID 0xF190 (exemple pour le numéro de série)
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : Réponse 0x62 et données associées au DID 0xF190

def test_read_multiple_dids():
    print("Test: Lecture de plusieurs DIDs")
    command = bytes([0x22, 0x01, 0x0A, 0x01, 0x10])  # SID 0x22, DID 0x010A et DID 0x0110
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : Réponse 0x62 et données associées aux DIDs 0x010A et 0x0110

# Tests de scénarios de réponse négative (NRC)
def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    command = bytes([0x22])  # SID 0x22 sans DID
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x13 (Incorrect message length or invalid format)

def test_response_too_long():
    print("Test: Réponse trop longue")
    command = bytes([0x22] + [0x01, 0x0A] * 33)  # Trop de DIDs dans une seule requête
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x14 (Response too long)

def test_conditions_not_correct():
    print("Test: Conditions incorrectes")
    # Simulation d'une condition où le service n'est pas disponible
    command = bytes([0x22, 0x02, 0x22])  # DID exemple où la condition n'est pas correcte
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x22 (Conditions not correct)

def test_request_out_of_range():
    print("Test: DID hors de portée")
    command = bytes([0x22, 0xFF, 0xFF])  # DID inexistant
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x31 (Request out of range)

def test_security_access_denied():
    print("Test: Accès sécurité requis et refusé")
    command = bytes([0x22, 0x01, 0x23])  # DID nécessitant un accès de sécurité sans accès autorisé
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x33 (Security access denied)

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour ReadDataByIdentifier...")
        
        # Tests de scénario positif
        test_read_single_did()
        test_read_multiple_dids()
        
        # Tests de scénarios de réponse négative (NRC)
        test_incorrect_message_length()
        test_response_too_long()
        test_conditions_not_correct()
        test_request_out_of_range()
        test_security_access_denied()
        
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
