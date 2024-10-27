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

# Test de d�marrage de la r�ponse sur �v�nement (changement de statut DTC)
def test_start_event_on_dtc_status_change():
    print("Test: D�marrage de l'�v�nement sur changement de statut DTC")
    command = bytes([0x86, 0x01, 0x08, 0x01, 0x19, 0x01, 0x01])  # SID 0x86, onDTCStatusChange, 80s window
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente de 0xC6 avec les valeurs �chos

# Test de d�marrage de la r�ponse sur �v�nement (interruption timer)
def test_start_event_on_timer_interrupt():
    print("Test: D�marrage de l'�v�nement sur interruption de timer")
    command = bytes([0x86, 0x02, 0x08, 0x01])  # SID 0x86, onTimerInterrupt avec 80s window
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente de 0xC6 avec les valeurs �chos

# Test pour arr�ter la r�ponse sur �v�nement
def test_stop_response_on_event():
    print("Test: Arr�t de la r�ponse sur �v�nement")
    command = bytes([0x86, 0x00])  # SID 0x86, stopResponseOnEvent
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente de 0xC6 pour stop event

# Test pour nettoyer la r�ponse sur �v�nement
def test_clear_response_on_event():
    print("Test: Nettoyage de la r�ponse sur �v�nement")
    command = bytes([0x86, 0x06])  # SID 0x86, clearResponseOnEvent
    response = send_command(command)
    print(f"R�ponse : {response.hex()}")  # Attente de 0xC6 pour clear event

# Ex�cution des tests
if __name__ == "__main__":
    try:
        print("D�but des tests UDS pour ResponseOnEvent...")
        test_start_event_on_dtc_status_change()
        test_start_event_on_timer_interrupt()
        test_stop_response_on_event()
        test_clear_response_on_event()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
