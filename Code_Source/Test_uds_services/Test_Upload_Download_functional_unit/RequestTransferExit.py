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

# Fonction pour tester RequestTransferExit avec différents paramètres
def test_request_transfer_exit(data, description=""):
    print(f"\nTest: {description}")
    command = bytes([0x37]) + data  # SID 0x37 pour RequestTransferExit
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

# Fonction principale pour effectuer tous les tests
def test_transfer_exit_service():
    # Test avec des paramètres valides
    print("Tests de réponses positives avec paramètres valides :")
    valid_data = bytes([0x00, 0x01, 0x02])  # Exemple de données valides pour terminer le transfert
    test_request_transfer_exit(valid_data, "Paramètres valides pour terminer un transfert en cours")

    # Test de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte :")
    invalid_length_data = bytes([0x00])  # Exemple de longueur de message incorrecte
    test_request_transfer_exit(invalid_length_data, "Longueur de message incorrecte")

    # Test de séquence de requêtes incorrecte
    print("\nTests de séquence de requêtes incorrecte (aucun transfert en cours) :")
    test_request_transfer_exit(valid_data, "Pas de transfert en cours")

    # Test de paramètres hors de portée
    print("\nTests pour paramètres hors de portée :")
    out_of_range_data = bytes([0xFF, 0xFF, 0xFF])  # Paramètres simulant des valeurs hors portée
    test_request_transfer_exit(out_of_range_data, "Paramètres de requête hors de portée")

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour RequestTransferExit...")
        test_transfer_exit_service()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
