import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'  # Remplacez par le port utilisé par votre STM32
BAUD_RATE = 115200
TIMEOUT = 1

# Initialisation de la connexion série
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la réponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)  # Pause pour laisser le STM32 traiter la commande
    response = ser.read(ser.in_waiting or 1)
    return response

# Scénarios positifs pour ReadDataByPeriodicIdentifier
def test_periodic_identifier_medium_rate():
    print("Test: Périodique avec transmission à vitesse moyenne")
    command = bytes([0x2A, 0x02, 0xE3, 0x24])  # Mode moyen, PID 0xE3 et 0x24
    response = send_command(command)
    print(f"Réponse initiale : {response.hex()}")
    # Attendu : Réponse 0x6A (SID de réponse positive)
    
    # Simuler des réponses périodiques attendues
    for _ in range(3):
        response_periodic = send_command(b'')
        print(f"Réponse périodique : {response_periodic.hex()}")

def test_periodic_identifier_stop():
    print("Test: Arrêt de la transmission périodique")
    command = bytes([0x2A, 0x04, 0xE3])  # Mode stop, PID 0xE3
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : Réponse 0x6A (SID de réponse positive pour l'arrêt)

# Tests pour les codes de réponse négative (NRC)
def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    command = bytes([0x2A])  # Message trop court
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x13 (incorrectMessageLengthOrInvalidFormat)

def test_request_out_of_range():
    print("Test: PID hors de portée")
    command = bytes([0x2A, 0x02, 0xFF])  # PID non supporté
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x31 (requestOutOfRange)

def test_conditions_not_correct():
    print("Test: Conditions non correctes pour la requête")
    command = bytes([0x2A, 0x02, 0xE3, 0x24])  # Cas de conditions non respectées
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x22 (conditionsNotCorrect)

def test_security_access_denied():
    print("Test: Accès sécurisé requis et refusé")
    command = bytes([0x2A, 0x02, 0xE3])  # PID sécurisé sans accès
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attendu : NRC 0x33 (SecurityAccessDenied)

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour ReadDataByPeriodicIdentifier...")
        
        # Scénarios positifs
        test_periodic_identifier_medium_rate()
        test_periodic_identifier_stop()
        
        # Tests de scénarios de réponse négative (NRC)
        test_incorrect_message_length()
        test_request_out_of_range()
        test_conditions_not_correct()
        test_security_access_denied()
        
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
