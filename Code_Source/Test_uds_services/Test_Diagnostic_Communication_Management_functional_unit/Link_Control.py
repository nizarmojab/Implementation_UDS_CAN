import serial
import time

# Configuration de la connexion s�rie
SERIAL_PORT = 'COM3'  # Remplacez par le port correct de votre STM32
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

# Test de v�rification de transition de mode avec un param�tre fixe
def test_verify_mode_transition_fixed():
    print("Test: V�rification de la transition de mode avec param�tre fixe")
    command = bytes([0x87, 0x01, 0x05])  # SID 0x87, sub_function = 0x01, modeIdentifier = 0x05
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente de 0xC7 avec la sous-fonction �cho

# Test de v�rification de transition de mode avec un param�tre sp�cifique
def test_verify_mode_transition_specific():
    print("Test: V�rification de la transition de mode avec param�tre sp�cifique")
    command = bytes([0x87, 0x02, 0x02, 0x49, 0xF0])  # SID 0x87, sub_function = 0x02, linkRecord = 0x0249F0
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente de 0xC7 avec la sous-fonction �cho

# Test de transition de mode apr�s v�rification
def test_transition_mode():
    print("Test: Transition de mode")
    command = bytes([0x87, 0x03])  # SID 0x87, sub_function = 0x03 pour transition
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente de 0xC7 avec la sous-fonction �cho ou absence de r�ponse

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour LinkControl...")
        test_verify_mode_transition_fixed()
        test_verify_mode_transition_specific()
        test_transition_mode()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
