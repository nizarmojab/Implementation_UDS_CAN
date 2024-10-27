import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'  # Modifiez en fonction de votre port
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

# Fonction pour tester chaque sous-fonction
def test_io_control(sub_function, data_identifier, control_state, description=""):
    print(f"\nTest: {description}")
    # Construire la commande avec SID, sous-fonction, identifiant et état de contrôle
    command = bytes([0x2F, sub_function]) + data_identifier.to_bytes(2, 'big') + control_state
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

# Fonction principale pour exécuter tous les tests
def test_input_output_control():
    # Scénarios de tests avec réponses positives
    print("Tests de réponses positives pour chaque option de contrôle:")
    test_io_control(0x01, 0x1234, bytes([0x01]), "Option de contrôle: Ajustement à court terme")
    test_io_control(0x02, 0x1234, bytes([0x00]), "Option de contrôle: Retour au contrôle de l'ECU")
    test_io_control(0x03, 0x1234, bytes([0x00]), "Option de contrôle: Geler l'état actuel")

    # Tests de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte:")
    test_io_control(0x01, 0x1234, b"", "Longueur incorrecte (manque état de contrôle)")

    # Tests pour demande hors de portée (DID non pris en charge)
    print("\nTests de demande hors de portée:")
    test_io_control(0x01, 0xFFFF, bytes([0x01]), "Identifiant de données non pris en charge")

    # Tests pour conditions non correctes
    print("\nTests pour conditions non correctes:")
    test_io_control(0x04, 0x1234, bytes([0x01]), "Option de contrôle non valide")

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour InputOutputControlByIdentifier...")
        test_input_output_control()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
