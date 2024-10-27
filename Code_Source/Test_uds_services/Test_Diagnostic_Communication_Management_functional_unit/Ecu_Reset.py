import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'
BAUD_RATE = 115200
TIMEOUT = 1

ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la réponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)
    response = ser.read(ser.in_waiting or 1)
    return response

# Tests de scénarios
def test_hard_reset():
    print("Test: Hard Reset")
    command = bytes([0x11, 0x01])
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_soft_reset():
    print("Test: Soft Reset")
    command = bytes([0x11, 0x02])
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_enable_rapid_power_shutdown():
    print("Test: Enable Rapid Power Shutdown")
    command = bytes([0x11, 0x03])
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_disable_rapid_power_shutdown():
    print("Test: Disable Rapid Power Shutdown")
    command = bytes([0x11, 0x04])
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_unsupported_sub_function():
    print("Test: Sous-fonction non supportée")
    command = bytes([0x11, 0x05])
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_incorrect_message_length():
    print("Test: Longueur de message incorrecte")
    command = bytes([0x11])
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_conditions_not_correct():
    print("Test: Conditions non correctes")
    command = bytes([0x11, 0x01])  # Commande dans une session non autorisée
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

def test_security_access_denied():
    print("Test: Accès sécurité refusé")
    command = bytes([0x11, 0x02])  # Commande nécessitant un accès sécurisé
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour ECU Reset...")
        test_hard_reset()
        test_soft_reset()
        test_enable_rapid_power_shutdown()
        test_disable_rapid_power_shutdown()
        test_unsupported_sub_function()
        test_incorrect_message_length()
        test_conditions_not_correct()
        test_security_access_denied()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
