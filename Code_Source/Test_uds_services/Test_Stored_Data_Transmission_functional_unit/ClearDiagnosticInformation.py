import serial
import time

# Configuration de la connexion s�rie
SERIAL_PORT = 'COM3'  # Modifiez en fonction de votre port COM
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

# Test du sc�nario positif pour ClearDiagnosticInformation
def test_clear_diagnostic_information_positive():
    print("Test: Effacer les informations diagnostiques pour un groupOfDTC support�")
    # Commande pour ClearDiagnosticInformation avec un groupOfDTC support� (exemple : 0x010000)
    command = bytes([0x14, 0x01, 0x00, 0x00])
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse positive (0x54)

# Test de longueur de message incorrecte (NRC 0x13)
def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    # Commande avec une longueur de message incorrecte (exemple : 2 octets)
    command = bytes([0x14, 0x01])
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x13 (incorrectMessageLengthOrInvalidFormat)

# Test de groupe de DTC non support� (NRC 0x31)
def test_request_out_of_range():
    print("Test: groupOfDTC non support�")
    # Commande avec un groupOfDTC non support� (exemple : 0x050000)
    command = bytes([0x14, 0x05, 0x00, 0x00])
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x31 (requestOutOfRange)

# Test de conditions non correctes pour l'effacement (NRC 0x22)
def test_conditions_not_correct():
    print("Test: Conditions incorrectes pour effacement de DTC")
    # Utiliser un groupOfDTC support� mais dans des conditions incorrectes
    command = bytes([0x14, 0x02, 0x00, 0x00])
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x22 (conditionsNotCorrect)

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour ClearDiagnosticInformation...")
        
        # Sc�narios positifs
        test_clear_diagnostic_information_positive()
        
        # Tests de sc�narios de r�ponse n�gative (NRC)
        test_incorrect_message_length()
        test_request_out_of_range()
        test_conditions_not_correct()
        
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
