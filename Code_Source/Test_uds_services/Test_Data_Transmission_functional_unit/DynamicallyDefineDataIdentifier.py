import serial
import time

# Configuration de la connexion s�rie
SERIAL_PORT = 'COM3'  # Modifiez en fonction du port COM utilis�
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

# Test pour la sous-fonction DefineByIdentifier (0x01)
def test_define_by_identifier():
    print("Test: D�finir DID dynamique par identifiant")
    command = bytes([0x2C, 0x01, 0x12, 0x34, 0x56, 0x78, 0x00, 0x02])
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse positive 0x6C 0x01 0x12 0x34

# Test pour la sous-fonction DefineByMemoryAddress (0x02)
def test_define_by_memory_address():
    print("Test: D�finir DID dynamique par adresse m�moire")
    command = bytes([0x2C, 0x02, 0x12, 0x34, 0x14, 0x00, 0x00, 0x10, 0x00, 0x04])
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse positive 0x6C 0x02 0x12 0x34

# Test pour la sous-fonction ClearDynamicIdentifier (0x03)
def test_clear_dynamic_identifier():
    print("Test: Effacer DID dynamique")
    command = bytes([0x2C, 0x03, 0x12, 0x34])
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : R�ponse positive 0x6C 0x03 0x12 0x34

# Test pour les codes de r�ponse n�gative (NRC)
def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    command = bytes([0x2C, 0x01, 0x12])  # Commande incompl�te
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x13 (incorrectMessageLengthOrInvalidFormat)

def test_sub_function_not_supported():
    print("Test: Sous-fonction non support�e")
    command = bytes([0x2C, 0x04, 0x12, 0x34])  # Sous-fonction invalide
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x12 (subFunctionNotSupported)

def test_request_out_of_range():
    print("Test: DID d�j� d�fini ou nombre max de DIDs atteint")
    command = bytes([0x2C, 0x01, 0x12, 0x34, 0x56, 0x78, 0x00, 0x02])  # DID d�j� d�fini ou d�passement
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attendu : NRC 0x31 (requestOutOfRange)

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour DynamicallyDefineDataIdentifier...")
        
        # Sc�narios positifs
        test_define_by_identifier()
        test_define_by_memory_address()
        test_clear_dynamic_identifier()
        
        # Tests de sc�narios de r�ponse n�gative (NRC)
        test_incorrect_message_length()
        test_sub_function_not_supported()
        test_request_out_of_range()
        
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
