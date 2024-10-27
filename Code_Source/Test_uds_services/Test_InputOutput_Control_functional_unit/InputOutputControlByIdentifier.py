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

# Fonction pour tester chaque sous-fonction
def test_io_control(sub_function, data_identifier, control_state, description=""):
    print(f"\nTest: {description}")
    # Construire la commande avec SID, sous-fonction, identifiant et �tat de contr�le
    command = bytes([0x2F, sub_function]) + data_identifier.to_bytes(2, 'big') + control_state
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")

# Fonction principale pour ex�cuter tous les tests
def test_input_output_control():
    # Sc�narios de tests avec r�ponses positives
    print("Tests de r�ponses positives pour chaque option de contr�le:")
    test_io_control(0x01, 0x1234, bytes([0x01]), "Option de contr�le: Ajustement � court terme")
    test_io_control(0x02, 0x1234, bytes([0x00]), "Option de contr�le: Retour au contr�le de l'ECU")
    test_io_control(0x03, 0x1234, bytes([0x00]), "Option de contr�le: Geler l'�tat actuel")

    # Tests de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte:")
    test_io_control(0x01, 0x1234, b"", "Longueur incorrecte (manque �tat de contr�le)")

    # Tests pour demande hors de port�e (DID non pris en charge)
    print("\nTests de demande hors de port�e:")
    test_io_control(0x01, 0xFFFF, bytes([0x01]), "Identifiant de donn�es non pris en charge")

    # Tests pour conditions non correctes
    print("\nTests pour conditions non correctes:")
    test_io_control(0x04, 0x1234, bytes([0x01]), "Option de contr�le non valide")

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour InputOutputControlByIdentifier...")
        test_input_output_control()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
