import serial
import time

# Configuration de la connexion s�rie
SERIAL_PORT = 'COM3'  # Modifiez en fonction de votre port s�rie
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

# Fonction pour tester RequestFileTransfer avec diff�rents param�tres
def test_request_file_transfer(data, description=""):
    print(f"\nTest: {description}")
    command = bytes([0x34]) + data  # SID 0x34 pour RequestFileTransfer
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

# Fonction principale pour effectuer tous les tests
def test_file_transfer_service():
    # Test avec des param�tres valides
    print("Tests de r�ponses positives avec param�tres valides :")
    valid_data = bytes([0x01, 0x00, 0x20]) + b'/path/to/file'  # Exemple de mode d'op�ration valide et de chemin
    test_request_file_transfer(valid_data, "Param�tres valides pour un transfert de fichier")

    # Test de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte :")
    invalid_length_data = bytes([0x01, 0x00])  # Longueur incorrecte pour simuler une erreur
    test_request_file_transfer(invalid_length_data, "Longueur de message incorrecte")

    # Test de mode d'op�ration hors de port�e
    print("\nTests de mode d'op�ration hors de port�e :")
    out_of_range_data = bytes([0x06, 0x00, 0x20]) + b'/path/to/file'  # Mode d'op�ration hors de la plage valide
    test_request_file_transfer(out_of_range_data, "Mode d'op�ration hors de port�e")

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour RequestFileTransfer...")
        test_file_transfer_service()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
