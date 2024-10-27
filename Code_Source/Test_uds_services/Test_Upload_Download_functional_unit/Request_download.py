import serial
import time

# Configuration de la connexion s�rie
SERIAL_PORT = 'COM3'  # Modifiez en fonction de votre port
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

# Fonction pour tester RequestDownload avec diff�rents param�tres
def test_request_download(data_format, address_format, memory_address, memory_size, description=""):
    print(f"\nTest: {description}")
    command = bytes([0x34, data_format, address_format]) + memory_address + memory_size
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

# Fonction principale pour effectuer tous les tests
def test_request_download_service():
    # Sc�narios de tests avec r�ponses positives pour RequestDownload
    print("Tests de r�ponses positives :")
    test_request_download(0x00, 0x44, b'\x00\x10\x00\x00', b'\x00\x00\x10', "Param�tres valides pour le t�l�chargement")

    # Test de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte :")
    test_request_download(0x00, 0x44, b'\x00\x10\x00', b'\x00\x10', "Longueur incorrecte (adresse incompl�te)")

    # Test avec une adresse m�moire invalide
    print("\nTests d'adresse m�moire invalide :")
    test_request_download(0x00, 0x44, b'\xFF\xFF\xFF\xFF', b'\x00\x10', "Adresse m�moire non valide")

    # Test avec une taille m�moire invalide
    print("\nTests de taille m�moire invalide :")
    test_request_download(0x00, 0x44, b'\x00\x10\x00\x00', b'\xFF\xFF\xFF', "Taille m�moire non valide")

    # Test avec acc�s refus� pour des raisons de s�curit�
    print("\nTests de s�curit� active :")
    test_request_download(0x00, 0x44, b'\x00\x10\x00\x00', b'\x00\x10', "Acc�s refus� pour des raisons de s�curit�")

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour RequestDownload...")
        test_request_download_service()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
