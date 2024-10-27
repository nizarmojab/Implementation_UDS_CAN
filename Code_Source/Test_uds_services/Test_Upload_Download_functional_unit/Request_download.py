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

# Fonction pour tester RequestDownload avec différents paramètres
def test_request_download(data_format, address_format, memory_address, memory_size, description=""):
    print(f"\nTest: {description}")
    command = bytes([0x34, data_format, address_format]) + memory_address + memory_size
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

# Fonction principale pour effectuer tous les tests
def test_request_download_service():
    # Scénarios de tests avec réponses positives pour RequestDownload
    print("Tests de réponses positives :")
    test_request_download(0x00, 0x44, b'\x00\x10\x00\x00', b'\x00\x00\x10', "Paramètres valides pour le téléchargement")

    # Test de longueur de message incorrecte
    print("\nTests de longueur de message incorrecte :")
    test_request_download(0x00, 0x44, b'\x00\x10\x00', b'\x00\x10', "Longueur incorrecte (adresse incomplète)")

    # Test avec une adresse mémoire invalide
    print("\nTests d'adresse mémoire invalide :")
    test_request_download(0x00, 0x44, b'\xFF\xFF\xFF\xFF', b'\x00\x10', "Adresse mémoire non valide")

    # Test avec une taille mémoire invalide
    print("\nTests de taille mémoire invalide :")
    test_request_download(0x00, 0x44, b'\x00\x10\x00\x00', b'\xFF\xFF\xFF', "Taille mémoire non valide")

    # Test avec accès refusé pour des raisons de sécurité
    print("\nTests de sécurité active :")
    test_request_download(0x00, 0x44, b'\x00\x10\x00\x00', b'\x00\x10', "Accès refusé pour des raisons de sécurité")

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour RequestDownload...")
        test_request_download_service()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
