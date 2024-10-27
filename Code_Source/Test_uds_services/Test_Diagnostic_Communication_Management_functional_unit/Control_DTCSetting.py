import serial
import time

# Configuration de la connexion s�rie
SERIAL_PORT = 'COM3'  # Remplacez par le port utilis� par votre STM32
BAUD_RATE = 115200
TIMEOUT = 1  # Timeout de 1 seconde

# Initialisation de la connexion s�rie
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

# Fonction pour envoyer une commande et lire la r�ponse
def send_command(command):
    ser.write(command)
    time.sleep(0.1)  # Pause pour laisser le STM32 traiter la commande
    response = ser.read(ser.in_waiting or 1)  # Lire les donn�es disponibles
    return response

# Test pour activer la mise � jour des DTC (sub_function = 0x01)
def test_dtc_setting_on():
    print("Test: Activation de la mise � jour des DTC (sub_function = 0x01)")
    command = bytes([0x85, 0x01])  # SID 0x85 avec sub_function 0x01
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attente de la r�ponse avec SID 0xC5 (0x85 + 0x40) et sub_function 0x01

# Test pour d�sactiver la mise � jour des DTC (sub_function = 0x02)
def test_dtc_setting_off():
    print("Test: D�sactivation de la mise � jour des DTC (sub_function = 0x02)")
    command = bytes([0x85, 0x02])  # SID 0x85 avec sub_function 0x02
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attente de la r�ponse avec SID 0xC5 (0x85 + 0x40) et sub_function 0x02

# Test pour une sous-fonction non support�e (exemple: sub_function = 0x03)
def test_unsupported_sub_function():
    print("Test: Sous-fonction non support�e (sub_function = 0x03)")
    command = bytes([0x85, 0x03])  # SID 0x85 avec sub_function 0x03 (non support�e)
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attente de la r�ponse avec SID 0x7F et NRC 0x12 (sub-function not supported)

# Test pour les conditions incorrectes (par exemple, hors session autoris�e)
def test_conditions_not_correct():
    print("Test: Conditions incorrectes")
    # Simuler un �tat o� le service n'est pas autoris� (ajuster c�t� STM32 si n�cessaire)
    command = bytes([0x85, 0x01])  # SID 0x85 avec sub_function 0x01
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")
    # Attente de la r�ponse avec SID 0x7F et NRC 0x22 (conditions not correct)

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour ControlDTCSetting...")
        test_dtc_setting_on()
        test_dtc_setting_off()
        test_unsupported_sub_function()
        test_conditions_not_correct()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
