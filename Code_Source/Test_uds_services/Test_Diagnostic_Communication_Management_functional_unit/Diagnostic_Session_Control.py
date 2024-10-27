import serial
import time

# Configuration de la connexion s�rie
SERIAL_PORT = 'COM3'  # Remplacez par le port utilis� par votre STM32
BAUD_RATE = 115200
TIMEOUT = 1  # Timeout de 1 seconde

# Initialisation de la connexion s�rie
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la r�ponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)  # Pause pour laisser le STM32 traiter la commande
    response = ser.read(ser.in_waiting or 1)  # Lire les donn�es disponibles
    return response

# Sc�narios de test
def test_default_session():
    print("Test: Default Session (dans la session par d�faut)")
    command = bytes([0x10, 0x01])  # Service 0x10, sous-fonction 0x01
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_switch_to_programming_session():
    print("Test: Passer � la session de programmation")
    command = bytes([0x10, 0x02])  # Service 0x10, sous-fonction 0x02
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_switch_to_extended_session():
    print("Test: Passer � la session de diagnostic �tendu")
    command = bytes([0x10, 0x03])  # Service 0x10, sous-fonction 0x03
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_unsupported_sub_function():
    print("Test: Sous-fonction non support�e")
    command = bytes([0x10, 0x05])  # Sous-fonction non d�finie
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    command = bytes([0x10])  # Message incomplet (moins de 2 octets)
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_conditions_not_correct():
    print("Test: Conditions non correctes (service non autoris�)")
    command = bytes([0x27, 0x01])  # Service non autoris� en session par d�faut
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour Diagnostic Session Control...")
        test_default_session()
        test_switch_to_programming_session()
        test_switch_to_extended_session()
        test_unsupported_sub_function()
        test_incorrect_message_length()
        test_conditions_not_correct()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
