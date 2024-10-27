import serial
import time

# Configuration de la connexion série
SERIAL_PORT = 'COM3'  # Remplacez par le port utilisé par votre STM32
BAUD_RATE = 115200
TIMEOUT = 1  # Timeout de 1 seconde

# Initialisation de la connexion série
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la réponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)  # Pause pour laisser le STM32 traiter la commande
    response = ser.read(ser.in_waiting or 1)  # Lire les données disponibles
    return response

# Test pour activer la mise à jour des DTC (sub_function = 0x01)
def test_dtc_setting_on():
    print("Test: Activation de la mise à jour des DTC (sub_function = 0x01)")
    command = bytes([0x85, 0x01])  # SID 0x85 avec sub_function 0x01
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attente de la réponse avec SID 0xC5 (0x85 + 0x40) et sub_function 0x01

# Test pour désactiver la mise à jour des DTC (sub_function = 0x02)
def test_dtc_setting_off():
    print("Test: Désactivation de la mise à jour des DTC (sub_function = 0x02)")
    command = bytes([0x85, 0x02])  # SID 0x85 avec sub_function 0x02
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attente de la réponse avec SID 0xC5 (0x85 + 0x40) et sub_function 0x02

# Test pour une sous-fonction non supportée (exemple: sub_function = 0x03)
def test_unsupported_sub_function():
    print("Test: Sous-fonction non supportée (sub_function = 0x03)")
    command = bytes([0x85, 0x03])  # SID 0x85 avec sub_function 0x03 (non supportée)
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attente de la réponse avec SID 0x7F et NRC 0x12 (sub-function not supported)

# Test pour les conditions incorrectes (par exemple, hors session autorisée)
def test_conditions_not_correct():
    print("Test: Conditions incorrectes")
    # Simuler un état où le service n'est pas autorisé (ajuster côté STM32 si nécessaire)
    command = bytes([0x85, 0x01])  # SID 0x85 avec sub_function 0x01
    response = send_command(command)
    print(f"Réponse : {response.hex()}")
    # Attente de la réponse avec SID 0x7F et NRC 0x22 (conditions not correct)

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour ControlDTCSetting...")
        test_dtc_setting_on()
        test_dtc_setting_off()
        test_unsupported_sub_function()
        test_conditions_not_correct()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
