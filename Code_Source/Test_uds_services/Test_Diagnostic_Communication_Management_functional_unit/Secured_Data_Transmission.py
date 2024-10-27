import serial
import time
import os

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

# Chiffrement basique (remplacez par votre fonction de chiffrement si n�cessaire)
def encrypt_data(data, key):
    return bytes([data[i] ^ key[i % len(key)] for i in range(len(data))])

# D�finition de la cl� de chiffrement (exemple)
ENCRYPTION_KEY = bytes([0x1A, 0x2B, 0x3C, 0x4D] * 4)  # Cl� r�p�t�e pour 16 octets

# Sc�narios de test pour SecuredDataTransmission (0x84)
def test_secured_data_transmission_valid():
    print("Test: Transmission de donn�es s�curis�es valides")
    data = bytes([0x12, 0x34, 0x56, 0x78])  # Donn�es d'exemple
    encrypted_data = encrypt_data(data, ENCRYPTION_KEY)
    command = bytes([0x84]) + encrypted_data  # SID 0x84 suivi des donn�es chiffr�es
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse avec le SID 0xC4 (0x84 + 0x40)

def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte (NRC 0x13)")
    command = bytes([0x84])  # Message incomplet (pas de donn�es)
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse n�gative avec NRC 0x13

def test_invalid_format():
    print("Test: Format de message invalide")
    data = bytes([0xAA, 0xBB])  # Donn�es incorrectes (trop courtes)
    encrypted_data = encrypt_data(data, ENCRYPTION_KEY)
    command = bytes([0x84]) + encrypted_data  # SID 0x84 suivi de donn�es chiffr�es invalides
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente d'une r�ponse n�gative avec NRC 0x13

def test_data_transmission_with_invalid_key():
    print("Test: Transmission de donn�es avec cl� incorrecte")
    data = bytes([0x12, 0x34, 0x56, 0x78])  # Donn�es d'exemple
    # Utilisation d'une cl� incorrecte pour tester le d�cryptage
    invalid_key = bytes([0xFF] * 16)
    encrypted_data = encrypt_data(data, invalid_key)
    command = bytes([0x84]) + encrypted_data  # SID 0x84 suivi de donn�es chiffr�es avec une cl� incorrecte
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # La r�ponse devrait signaler une erreur de d�cryptage si appliqu�e

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour SecuredDataTransmission...")
        test_secured_data_transmission_valid()
        test_incorrect_message_length()
        test_invalid_format()
        test_data_transmission_with_invalid_key()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
