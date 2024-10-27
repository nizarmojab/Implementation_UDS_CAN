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

# Fonction pour tester chaque sous-fonction de RoutineControl
def test_routine_control(sub_function, routine_identifier, routine_option, description=""):
    print(f"\nTest: {description}")
    # Construire la commande avec SID, sous-fonction, routine identifier et option
    command = bytes([0x31, sub_function]) + routine_identifier.to_bytes(2, 'big') + bytes([routine_option])
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

# Fonction principale pour exécuter tous les tests
def test_routine_control_service():
    # Scénarios de tests avec réponses positives pour chaque sous-fonction
    print("Tests de réponses positives pour chaque sous-fonction:")
    test_routine_control(0x01, 0x0101, 0x00, "Démarrer la routine")
    test_routine_control(0x02, 0x0101, 0x00, "Arrêter la routine")
    test_routine_control(0x03, 0x0101, 0x00, "Demander les résultats de la routine")

    # Test de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte:")
    test_routine_control(0x01, 0x0101, None, "Longueur incorrecte (option manquante)")

    # Test pour demande hors de portée (RID non pris en charge)
    print("\nTests de demande hors de portée:")
    test_routine_control(0x01, 0xFFFF, 0x00, "Routine Identifier non pris en charge")

    # Test pour accès refusé pour des raisons de sécurité
    print("\nTests de sécurité non accordée:")
    test_routine_control(0x01, 0x0202, 0x00, "Accès non autorisé pour cette routine")

    # Test pour sous-fonction non supportée
    print("\nTests de sous-fonction non supportée:")
    test_routine_control(0x04, 0x0101, 0x00, "Sous-fonction non supportée")

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour RoutineControl...")
        test_routine_control_service()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
