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

# Sc�narios de test pour AccessTimingParameter (0x83)
def test_read_extended_timing():
    print("Test: Lire les param�tres de temporisation �tendus (sub-function 0x01)")
    command = bytes([0x83, 0x01])  # SID 0x83, sous-fonction 0x01
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse avec le SID 0xC3

def test_set_timing_to_default():
    print("Test: Param�trer les valeurs par d�faut des param�tres de temporisation (sub-function 0x02)")
    command = bytes([0x83, 0x02])  # SID 0x83, sous-fonction 0x02
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse positive avec le SID 0xC3

def test_read_active_timing_parameters():
    print("Test: Lire les param�tres de temporisation actifs (sub-function 0x03)")
    command = bytes([0x83, 0x03])  # SID 0x83, sous-fonction 0x03
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse avec le SID 0xC3

def test_set_custom_timing_parameters():
    print("Test: D�finir des param�tres de temporisation personnalis�s (sub-function 0x04)")
    # Exemple de timing personnalis�s
    command = bytes([0x83, 0x04, 0x00, 0x32, 0x01, 0xF4])  # SID 0x83, sous-fonction 0x04, suivi des valeurs P2 et P2*
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse avec le SID 0xC3

# Sc�narios de test pour les erreurs NRC
def test_unsupported_sub_function():
    print("Test: Sous-fonction non support�e (NRC 0x12)")
    command = bytes([0x83, 0x05])  # Sous-fonction non d�finie
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse n�gative avec NRC 0x12

def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte (NRC 0x13)")
    command = bytes([0x83])  # Message incomplet (moins de 2 octets)
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse n�gative avec NRC 0x13

def test_conditions_not_correct():
    print("Test: Conditions non correctes (NRC 0x22)")
    # Exemple de condition non respect�e (modifier si n�cessaire pour votre configuration)
    command = bytes([0x83, 0x04, 0xFF, 0xFF])  # Faux timing parameters pour d�clencher NRC 0x22
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse n�gative avec NRC 0x22

def test_request_out_of_range():
    print("Test: Param�tre hors de la plage autoris�e (NRC 0x31)")
    # Envoie de param�tres de timing non valides
    command = bytes([0x83, 0x04, 0xFF, 0xFF, 0xFF, 0xFF])  # Valeurs hors de port�e pour d�clencher NRC 0x31
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse n�gative avec NRC 0x31

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour AccessTimingParameter...")
        test_read_extended_timing()
        test_set_timing_to_default()
        test_read_active_timing_parameters()
        test_set_custom_timing_parameters()
        test_unsupported_sub_function()
        test_incorrect_message_length()
        test_conditions_not_correct()
        test_request_out_of_range()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
