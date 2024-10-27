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

# Fonction pour tester TransferData avec différents paramètres
def test_transfer_data(block_sequence, description=""):
    print(f"\nTest: {description}")
    command = bytes([0x36, block_sequence])
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

# Fonction principale pour effectuer tous les tests
def test_transfer_data_service():
    # Test avec des paramètres valides (suite de blockSequenceCounter)
    print("Tests de réponses positives avec séquence valide :")
    for i in range(3):  # Envoie une séquence de 3 blocs valides
        test_transfer_data(i + 1, f"Block Sequence {i + 1}")

    # Test de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte :")
    test_transfer_data(0x01, "Longueur incorrecte (message incomplet)")

    # Test d'erreur de séquence de requêtes actives
    print("\nTests de séquence de requêtes incorrecte (pas de téléchargement ou upload actif) :")
    test_transfer_data(0x01, "Pas de requête active")

    # Test de compteur de séquence de bloc incorrect
    print("\nTests de compteur de séquence de bloc incorrect :")
    test_transfer_data(0x05, "Compteur de séquence hors ordre")

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour TransferData...")
        test_transfer_data_service()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
