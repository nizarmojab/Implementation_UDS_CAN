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

# Sc�narios de test pour CommunicationControl (0x28)
def test_enable_rx_and_tx():
    print("Test: Enable Rx and Tx")
    command = bytes([0x28, 0x00, 0x01])  # Service 0x28, sub-function 0x00
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_enable_rx_and_disable_tx():
    print("Test: Enable Rx and Disable Tx")
    command = bytes([0x28, 0x01, 0x02])  # Service 0x28, sub-function 0x01
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_disable_rx_and_enable_tx():
    print("Test: Disable Rx and Enable Tx")
    command = bytes([0x28, 0x02, 0x01])  # Service 0x28, sub-function 0x02
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_disable_rx_and_tx():
    print("Test: Disable Rx and Tx")
    command = bytes([0x28, 0x03, 0x01])  # Service 0x28, sub-function 0x03
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_enable_rx_and_disable_tx_with_enhanced_info():
    print("Test: Enable Rx and Disable Tx with Enhanced Info")
    command = bytes([0x28, 0x04, 0x01, 0x00, 0x0A])  # Service 0x28, sub-function 0x04
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_enable_rx_and_tx_with_enhanced_info():
    print("Test: Enable Rx and Tx with Enhanced Info")
    command = bytes([0x28, 0x05, 0x01, 0x00, 0x0A])  # Service 0x28, sub-function 0x05
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

# Sc�narios de test pour les erreurs NRC
def test_invalid_sub_function():
    print("Test: Invalid Sub-function")
    command = bytes([0x28, 0x07])  # Sous-fonction non support�e
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_conditions_not_correct():
    print("Test: Conditions Not Correct")
    command = bytes([0x28, 0x01])  # Exemple de condition incorrecte
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

def test_request_out_of_range():
    print("Test: Request Out of Range")
    command = bytes([0x28, 0x01, 0xFF, 0x00, 0x0A])  # communicationType hors de port�e
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour Communication Control...")
        test_enable_rx_and_tx()
        test_enable_rx_and_disable_tx()
        test_disable_rx_and_enable_tx()
        test_disable_rx_and_tx()
        test_enable_rx_and_disable_tx_with_enhanced_info()
        test_enable_rx_and_tx_with_enhanced_info()
        test_invalid_sub_function()
        test_conditions_not_correct()
        test_request_out_of_range()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
