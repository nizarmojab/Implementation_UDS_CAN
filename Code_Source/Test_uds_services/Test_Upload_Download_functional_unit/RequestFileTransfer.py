import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'  # Modifiez en fonction de votre port série
BAUD_RATE = 115200
TIMEOUT = 1

# Initialisation de la connexion série
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la réponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)
    response = ser.read(ser.in_waiting or 1)
    return response

# Fonction pour tester RequestFileTransfer avec différents paramètres
def test_request_file_transfer(data, description=""):
    print(f"\nTest: {description}")
    command = bytes([0x34]) + data  # SID 0x34 pour RequestFileTransfer
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

# Fonction principale pour effectuer tous les tests
def test_file_transfer_service():
    # Test avec des paramètres valides
    print("Tests de réponses positives avec paramètres valides :")
    valid_data = bytes([0x01, 0x00, 0x20]) + b'/path/to/file'  # Exemple de mode d'opération valide et de chemin
    test_request_file_transfer(valid_data, "Paramètres valides pour un transfert de fichier")

    # Test de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte :")
    invalid_length_data = bytes([0x01, 0x00])  # Longueur incorrecte pour simuler une erreur
    test_request_file_transfer(invalid_length_data, "Longueur de message incorrecte")

    # Test de mode d'opération hors de portée
    print("\nTests de mode d'opération hors de portée :")
    out_of_range_data = bytes([0x06, 0x00, 0x20]) + b'/path/to/file'  # Mode d'opération hors de la plage valide
    test_request_file_transfer(out_of_range_data, "Mode d'opération hors de portée")

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour RequestFileTransfer...")
        test_file_transfer_service()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
