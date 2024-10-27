import serial
import time

# Configuration de la connexion s�rie
SERIAL_PORT = 'COM3'  # Remplacez par le port correspondant � votre STM32
BAUD_RATE = 115200
TIMEOUT = 1

# Initialisation de la connexion s�rie
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la r�ponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)  # Pause pour laisser le STM32 traiter la commande
    response = ser.read(ser.in_waiting or 1)
    return response

# Tests de sc�nario positifs pour ReadDataByIdentifier
def test_read_single_did():
    print("Test: Lecture d'un DID unique")
    command = bytes([0x22, 0xF1, 0x90])  # SID 0x22, DID 0xF190 (exemple pour le num�ro de s�rie)
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse 0x62 et donn�es associ�es au DID 0xF190

def test_read_multiple_dids():
    print("Test: Lecture de plusieurs DIDs")
    command = bytes([0x22, 0x01, 0x0A, 0x01, 0x10])  # SID 0x22, DID 0x010A et DID 0x0110
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse 0x62 et donn�es associ�es aux DIDs 0x010A et 0x0110

# Tests de sc�narios de r�ponse n�gative (NRC)
def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    command = bytes([0x22])  # SID 0x22 sans DID
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x13 (Incorrect message length or invalid format)

def test_response_too_long():
    print("Test: R�ponse trop longue")
    command = bytes([0x22] + [0x01, 0x0A] * 33)  # Trop de DIDs dans une seule requ�te
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x14 (Response too long)

def test_conditions_not_correct():
    print("Test: Conditions incorrectes")
    # Simulation d'une condition o� le service n'est pas disponible
    command = bytes([0x22, 0x02, 0x22])  # DID exemple o� la condition n'est pas correcte
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x22 (Conditions not correct)

def test_request_out_of_range():
    print("Test: DID hors de port�e")
    command = bytes([0x22, 0xFF, 0xFF])  # DID inexistant
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x31 (Request out of range)

def test_security_access_denied():
    print("Test: Acc�s s�curit� requis et refus�")
    command = bytes([0x22, 0x01, 0x23])  # DID n�cessitant un acc�s de s�curit� sans acc�s autoris�
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x33 (Security access denied)

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour ReadDataByIdentifier...")
        
        # Tests de sc�nario positif
        test_read_single_did()
        test_read_multiple_dids()
        
        # Tests de sc�narios de r�ponse n�gative (NRC)
        test_incorrect_message_length()
        test_response_too_long()
        test_conditions_not_correct()
        test_request_out_of_range()
        test_security_access_denied()
        
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
