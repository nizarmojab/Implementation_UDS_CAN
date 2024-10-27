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

# Sc�narios positifs pour ReadDataByPeriodicIdentifier
def test_periodic_identifier_medium_rate():
    print("Test: P�riodique avec transmission � vitesse moyenne")
    command = bytes([0x2A, 0x02, 0xE3, 0x24])  # Mode moyen, PID 0xE3 et 0x24
    response = send_command(command)
    print(f"R�ponse initiale : {response.hex()}")
    # Attendu : R�ponse 0x6A (SID de r�ponse positive)
    
    # Simuler des r�ponses p�riodiques attendues
    for _ in range(3):
        response_periodic = send_command(b'')
        print(f"R�ponse p�riodique : {response_periodic.hex()}")

def test_periodic_identifier_stop():
    print("Test: Arr�t de la transmission p�riodique")
    command = bytes([0x2A, 0x04, 0xE3])  # Mode stop, PID 0xE3
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse 0x6A (SID de r�ponse positive pour l'arr�t)

# Tests pour les codes de r�ponse n�gative (NRC)
def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    command = bytes([0x2A])  # Message trop court
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x13 (incorrectMessageLengthOrInvalidFormat)

def test_request_out_of_range():
    print("Test: PID hors de port�e")
    command = bytes([0x2A, 0x02, 0xFF])  # PID non support�
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x31 (requestOutOfRange)

def test_conditions_not_correct():
    print("Test: Conditions non correctes pour la requ�te")
    command = bytes([0x2A, 0x02, 0xE3, 0x24])  # Cas de conditions non respect�es
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x22 (conditionsNotCorrect)

def test_security_access_denied():
    print("Test: Acc�s s�curis� requis et refus�")
    command = bytes([0x2A, 0x02, 0xE3])  # PID s�curis� sans acc�s
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x33 (SecurityAccessDenied)

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour ReadDataByPeriodicIdentifier...")
        
        # Sc�narios positifs
        test_periodic_identifier_medium_rate()
        test_periodic_identifier_stop()
        
        # Tests de sc�narios de r�ponse n�gative (NRC)
        test_incorrect_message_length()
        test_request_out_of_range()
        test_conditions_not_correct()
        test_security_access_denied()
        
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
