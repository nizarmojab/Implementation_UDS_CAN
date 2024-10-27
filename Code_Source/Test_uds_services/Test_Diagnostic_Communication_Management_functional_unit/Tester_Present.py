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

# Sc�narios de test pour TesterPresent (0x3E)
def test_tester_present_standard():
    print("Test: Tester Present avec sous-fonction 0x00 (suppressPosRspMsgIndicationBit = FALSE)")
    command = bytes([0x3E, 0x00])  # SID 0x3E, sous-fonction 0x00
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse positive 0x7E 0x00

def test_tester_present_suppress_response():
    print("Test: Tester Present avec suppressPosRspMsgIndicationBit = TRUE")
    command = bytes([0x3E, 0x80])  # SID 0x3E, sous-fonction 0x00 + suppressPosRspMsgIndicationBit
    response = send_command(command)
    if not response:
        print("Aucune r�ponse (comme attendu avec suppressPosRspMsgIndicationBit = TRUE)")
    else:
        print(f"R�ponse inattendue : {response.hex()}")

# Sc�narios de test pour les erreurs NRC
def test_invalid_sub_function():
    print("Test: Invalid Sub-function (NRC 0x12)")
    command = bytes([0x3E, 0x01])  # Sous-fonction non support�e
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse n�gative 0x7F 0x3E 0x12

def test_incorrect_message_length():
    print("Test: Incorrect Message Length (NRC 0x13)")
    command = bytes([0x3E])  # Message incomplet (moins de 2 octets)
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse n�gative 0x7F 0x3E 0x13

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour TesterPresent...")
        test_tester_present_standard()
        test_tester_present_suppress_response()
        test_invalid_sub_function()
        test_incorrect_message_length()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
