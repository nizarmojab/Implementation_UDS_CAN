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

# Fonction pour tester RequestUpload avec diff�rents param�tres
def test_request_upload(data_format, address_format, description=""):
    print(f"\nTest: {description}")
    command = bytes([0x35, data_format, address_format])
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

# Fonction principale pour effectuer tous les tests
def test_request_upload_service():
    # Test avec des param�tres valides
    print("Tests de r�ponses positives :")
    test_request_upload(0x35, 0x44, "Param�tres valides pour l'upload")

    # Test de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte :")
    test_request_upload(0x35, 0x00, "Longueur incorrecte (adresse manquante)")

    # Test avec identifiant de format non valide
    print("\nTests de demande hors plage :")
    test_request_upload(0x00, 0x44, "Format de donn�es non valide")

    # Test de s�curit� active
    print("\nTests de s�curit� active :")
    test_request_upload(0x35, 0x44, "Acc�s refus� pour des raisons de s�curit�")

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour RequestUpload...")
        test_request_upload_service()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
