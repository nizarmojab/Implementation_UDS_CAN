import serial
import time

# Configuration de la connexion s�rie
SERIAL_PORT = 'COM3'  # Modifier selon le port utilis�
BAUD_RATE = 115200
TIMEOUT = 1

# Initialisation de la connexion s�rie
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la r�ponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)
    response = ser.read(ser.in_waiting or 1)
    return response

# Test d'un DID support� avec une �criture r�ussie
def test_write_supported_did():
    print("Test : Ecriture dans un DID support�")
    command = bytes([0x2E, 0x01, 0x02, 0xAB, 0xCD])  # Exemple de commande pour SUPPORTED_DID_1
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse positive 0x6E 0x01 0x02

# Test d'un DID non support� pour l'�criture
def test_write_unsupported_did():
    print("Test : Ecriture dans un DID non support�")
    command = bytes([0x2E, 0xFF, 0xFF, 0x12, 0x34])  # DID non support�
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x31 (Request Out Of Range)

# Test avec des conditions de session non correctes
def test_incorrect_session():
    print("Test : Conditions de session non correctes")
    command = bytes([0x2E, 0x01, 0x02, 0x56, 0x78])  # SUPPORTED_DID_1 mais session incorrecte
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x22 (Conditions Not Correct)

# Test avec un acc�s s�curis� requis, mais non accord�
def test_security_access_required():
    print("Test : Acc�s s�curis� requis mais non accord�")
    command = bytes([0x2E, 0x01, 0x02, 0x9A, 0xBC])  # SUPPORTED_DID_1 n�cessitant un acc�s s�curis�
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x33 (Security Access Denied)

# Test d'un message avec longueur incorrecte
def test_incorrect_message_length():
    print("Test : Longueur de message incorrecte")
    command = bytes([0x2E, 0x01])  # Commande incompl�te
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x13 (Incorrect Message Length Or Invalid Format)

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour WriteDataByIdentifier...")
        
        # Sc�narios de r�ponse positive
        test_write_supported_did()
        
        # Tests de sc�narios de r�ponse n�gative (NRC)
        test_write_unsupported_did()
        test_incorrect_session()
        test_security_access_required()
        test_incorrect_message_length()
        
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
