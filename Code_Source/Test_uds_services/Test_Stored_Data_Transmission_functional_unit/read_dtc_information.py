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
def test_subfunction(sub_function, data):
    print(f"\nTest de la sous-fonction {sub_function:#02x} avec données : {data.hex()}")
    command = bytes([0x19, sub_function]) + data
    response = send_command(command)
    print(f"Réponse : {response.hex()}")

# Fonction principale pour effectuer tous les tests
def test_read_dtc_information():
    # Dictionnaire des sous-fonctions et leurs données de test (données d'exemple)
    sub_functions = {
        0x01: bytes([0xFF]),                            # REPORT_NUMBER_OF_DTC_BY_STATUS_MASK
        0x02: bytes([0xFF]),                            # REPORT_DTC_BY_STATUS_MASK
        0x03: b"",                                      # REPORT_DTC_SNAPSHOT_IDENTIFICATION
        0x04: bytes([0x00, 0x01, 0x00, 0x01]),          # REPORT_DTC_SNAPSHOT_RECORD_BY_DTC_NUMBER
        0x05: bytes([0x01, 0x02]),                      # REPORT_DTC_STORED_DATA_BY_RECORD_NUMBER
        0x06: bytes([0x00, 0x00, 0x01, 0xFF]),          # REPORT_DTC_EXT_DATA_RECORD_BY_DTC_NUMBER
        0x07: bytes([0x01, 0x00]),                      # REPORT_NUMBER_OF_DTC_BY_SEVERITY_MASK
        0x08: bytes([0xFF, 0xFF]),                      # REPORT_DTC_BY_SEVERITY_MASK_RECORD
        0x09: bytes([0x01, 0x01, 0x01]),                # REPORT_SEVERITY_INFORMATION_OF_DTC
        0x0A: b"",                                      # REPORT_SUPPORTED_DTC
        0x0B: b"",                                      # REPORT_FIRST_TEST_FAILED_DTC
        0x0C: b"",                                      # REPORT_FIRST_CONFIRMED_DTC
        0x0D: b"",                                      # REPORT_MOST_RECENT_TEST_FAILED_DTC
        0x0E: b"",                                      # REPORT_MOST_RECENT_CONFIRMED_DTC
        0x0F: bytes([0xFF]),                            # REPORT_MIRROR_MEMORY_DTC_BY_STATUS_MASK
        0x10: bytes([0x00, 0x01, 0x00, 0x02]),          # REPORT_MIRROR_MEMORY_DTC_EXT_DATA_RECORD
        0x11: bytes([0xFF]),                            # REPORT_NUMBER_OF_MIRROR_MEMORY_DTC_BY_STATUS_MASK
        0x12: bytes([0x01]),                            # REPORT_NUMBER_OF_EMISSIONS_OBD_DTC_BY_STATUS_MASK
        0x13: bytes([0xFF]),                            # REPORT_EMISSIONS_OBD_DTC_BY_STATUS_MASK
        0x14: b"",                                      # REPORT_DTC_FAULT_DETECTION_COUNTER
        0x15: b"",                                      # REPORT_DTC_WITH_PERMANENT_STATUS
        0x16: bytes([0x00, 0x03]),                      # REPORT_DTC_EXT_DATA_RECORD_BY_RECORD_NUMBER
        0x17: bytes([0xFF]),                            # REPORT_USER_DEF_MEMORY_DTC_BY_STATUS_MASK
        0x18: bytes([0x00, 0x01, 0x00, 0x04]),          # REPORT_USER_DEF_MEMORY_DTC_SNAPSHOT_RECORD
        0x19: bytes([0x00, 0x02, 0x00, 0x03]),          # REPORT_USER_DEF_MEMORY_DTC_EXT_DATA_RECORD
        0x1A: bytes([0x01]),                            # REPORT_WWH_OBD_DTC_BY_MASK_RECORD
        0x1B: b""                                       # REPORT_WWH_OBD_DTC_WITH_PERMANENT_STATUS
    }

    # Test de réponses positives pour chaque sous-fonction
    print("Tests de réponses positives pour chaque sous-fonction :")
    for sub_function, data in sub_functions.items():
        test_subfunction(sub_function, data)

    # Test de réponses négatives pour longueur de message incorrecte
    print("\nTests de longueur de message incorrecte :")
    for sub_function, data in sub_functions.items():
        incorrect_data = data[:-1]  # Retirer un octet pour simuler une erreur de longueur
        test_subfunction(sub_function, incorrect_data)

    # Test de réponses négatives pour demande hors de portée
    print("\nTests pour demande hors de portée :")
    out_of_range_data = bytes([0xFF, 0xFF, 0xFF, 0xFF])  # Valeurs hors des DTC connus
    for sub_function in [0x04, 0x05, 0x06, 0x10, 0x16, 0x18, 0x19]:  # Sous-fonctions qui acceptent des identifiants
        test_subfunction(sub_function, out_of_range_data)

    # Test de conditions non correctes
    print("\nTests pour conditions non correctes :")
    for sub_function in [0x03, 0x09, 0x0A, 0x14, 0x15]:  # Sous-fonctions pouvant être hors conditions
        test_subfunction(sub_function, b"")

# Exécution des tests
if __name__ == "__main__":
    try:
        print("Début des tests UDS pour ReadDTCInformation...")
        test_read_dtc_information()
    except Exception as e:
        print(f"Erreur de communication : {e}")
    finally:
        ser.close()
