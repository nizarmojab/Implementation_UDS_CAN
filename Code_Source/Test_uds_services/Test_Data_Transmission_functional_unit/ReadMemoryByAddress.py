import serial
import time

# Configuration de la connexion s�rie
SERIAL_PORT = 'COM3'  # Remplacez par le port utilis� par votre STM32
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

# Sc�narios positifs pour ReadMemoryByAddress
def test_read_memory_4byte_address():
    print("Test: Lecture m�moire avec adresse sur 4 octets")
    command = bytes([0x23, 0x24, 0x20, 0x48, 0x13, 0x92, 0x01, 0x03])  # Adresse 0x20481392, taille 0x0103
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse 0x63 avec les donn�es lues en m�moire

def test_read_memory_2byte_address():
    print("Test: Lecture m�moire avec adresse sur 2 octets")
    command = bytes([0x23, 0x12, 0x48, 0x13, 0x05])  # Adresse 0x4813, taille 0x05
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse 0x63 avec les donn�es lues en m�moire

def test_read_memory_3byte_address():
    print("Test: Lecture m�moire avec adresse sur 3 octets")
    command = bytes([0x23, 0x23, 0x20, 0x48, 0x13, 0x00, 0x03])  # Adresse 0x204813, taille 0x03
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse 0x63 avec les donn�es lues en m�moire

# Tests pour les codes de r�ponse n�gative (NRC)
def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    command = bytes([0x23])  # Message trop court
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x13 (incorrectMessageLengthOrInvalidFormat)

def test_request_out_of_range():
    print("Test: Adresse ou taille hors de port�e")
    command = bytes([0x23, 0x24, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x02])  # Adresse invalide
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x31 (requestOutOfRange)

def test_conditions_not_correct():
    print("Test: Conditions non correctes pour la requ�te")
    command = bytes([0x23, 0x24, 0x20, 0x48, 0x13, 0x92, 0x01, 0x03])  # Cas o� la condition n'est pas respect�e
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x22 (conditionsNotCorrect)

def test_security_access_denied():
    print("Test: Acc�s s�curis� requis et refus�")
    command = bytes([0x23, 0x24, 0x20, 0x48, 0x13, 0x92, 0x01, 0x03])  # Zone m�moire s�curis�e sans acc�s
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x33 (SecurityAccessDenied)

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour ReadMemoryByAddress...")
        
        # Sc�narios positifs
        test_read_memory_4byte_address()
        test_read_memory_2byte_address()
        test_read_memory_3byte_address()
        
        # Tests de sc�narios de r�ponse n�gative (NRC)
        test_incorrect_message_length()
        test_request_out_of_range()
        test_conditions_not_correct()
        test_security_access_denied()
        
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
