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

# Fonction pour tester RequestTransferExit avec diff�rents param�tres
def test_request_transfer_exit(data, description=""):
    print(f"\nTest: {description}")
    command = bytes([0x37]) + data  # SID 0x37 pour RequestTransferExit
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

# Fonction principale pour effectuer tous les tests
def test_transfer_exit_service():
    # Test avec des param�tres valides
    print("Tests de r�ponses positives avec param�tres valides :")
    valid_data = bytes([0x00, 0x01, 0x02])  # Exemple de donn�es valides pour terminer le transfert
    test_request_transfer_exit(valid_data, "Param�tres valides pour terminer un transfert en cours")

    # Test de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte :")
    invalid_length_data = bytes([0x00])  # Exemple de longueur de message incorrecte
    test_request_transfer_exit(invalid_length_data, "Longueur de message incorrecte")

    # Test de s�quence de requ�tes incorrecte
    print("\nTests de s�quence de requ�tes incorrecte (aucun transfert en cours) :")
    test_request_transfer_exit(valid_data, "Pas de transfert en cours")

    # Test de param�tres hors de port�e
    print("\nTests pour param�tres hors de port�e :")
    out_of_range_data = bytes([0xFF, 0xFF, 0xFF])  # Param�tres simulant des valeurs hors port�e
    test_request_transfer_exit(out_of_range_data, "Param�tres de requ�te hors de port�e")

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour RequestTransferExit...")
        test_transfer_exit_service()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
