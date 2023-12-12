import serial
import csv
from datetime import datetime

# Ustawienia portu szeregowego
port = 'COM8'
baud_rate = 115200
timeout = 1  # Timeout w sekundach dla operacji odczytu

# Ścieżka do pliku CSV
csv_filename = 'output.csv'

# Funkcja do odczytu danych z UART i zapisu do pliku CSV
def log_data_to_csv(port, baud_rate, timeout, csv_filename):
    with serial.Serial(port, baud_rate, timeout=timeout) as ser, open(csv_filename, 'w', newline='') as csvfile:
        # Utworzenie obiektu writer do zapisu danych CSV
        csv_writer = csv.writer(csvfile)
        # Zapis nagłówka CSV
        csv_writer.writerow(['Time', 'Voltage'])
        
        print(f"Connected to {port}. Reading data...")
        
        try:
            while True:
                if ser.in_waiting > 0:
                    # Odczyt linii (zakładamy, że każda linia kończy się znakiem nowej linii)
                    line = ser.readline().decode('utf-8').strip()
                    # Parsowanie danych
                    if line.startswith('Czas:'):
                        # Usunięcie etykiet i podział danych na czas i napięcie
                        parts = line.split('|')
                        time_str = parts[0].replace('Czas:', '').strip()
                        voltage_str = parts[1].replace('Voltage:', '').strip()
                        # Zapis danych do CSV
                        csv_writer.writerow([time_str, voltage_str])
                        print(f"Logged data: {time_str}, {voltage_str}")
        except KeyboardInterrupt:
            print("Stopped reading data from UART.")

# Wywołanie funkcji
log_data_to_csv(port, baud_rate, timeout, csv_filename)
