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

# Fonction pour tester TransferData avec diff�rents param�tres
def test_transfer_data(block_sequence, description=""):
    print(f"\nTest: {description}")
    command = bytes([0x36, block_sequence])
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

# Fonction principale pour effectuer tous les tests
def test_transfer_data_service():
    # Test avec des param�tres valides (suite de blockSequenceCounter)
    print("Tests de r�ponses positives avec s�quence valide :")
    for i in range(3):  # Envoie une s�quence de 3 blocs valides
        test_transfer_data(i + 1, f"Block Sequence {i + 1}")

    # Test de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte :")
    test_transfer_data(0x01, "Longueur incorrecte (message incomplet)")

    # Test d'erreur de s�quence de requ�tes actives
    print("\nTests de s�quence de requ�tes incorrecte (pas de t�l�chargement ou upload actif) :")
    test_transfer_data(0x01, "Pas de requ�te active")

    # Test de compteur de s�quence de bloc incorrect
    print("\nTests de compteur de s�quence de bloc incorrect :")
    test_transfer_data(0x05, "Compteur de s�quence hors ordre")

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour TransferData...")
        test_transfer_data_service()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
