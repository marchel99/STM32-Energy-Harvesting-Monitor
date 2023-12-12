import serial
import csv
from datetime import datetime


port = 'COM8'
baud_rate = 115200
timeout = 1  # Timeout w sekundach dla operacji odczytu


csv_filename = 'output.csv'


def log_data_to_csv(port, baud_rate, timeout, csv_filename):
    with serial.Serial(port, baud_rate, timeout=timeout) as ser, open(csv_filename, 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        csv_writer.writerow(['Time', 'Voltage'])  # Nagłówek CSV
        csvfile.flush()  # Opróżnienie bufora
        print(f"Connected to {port}. Reading data...")
        
        try:
            while True:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8').strip()
                    print(f"Raw data: {line}")  # Logowanie surowych danych dla debugowania
                    if line.startswith('Czas:'):
                        parts = line.split('|')
                        time_str = parts[0].replace('Czas:', '').strip()
                        voltage_str = parts[1].replace('Voltage:', '').strip()
                        csv_writer.writerow([time_str, voltage_str])
                        csvfile.flush()  # Ponowne opróżnienie bufora po zapisie
                        print(f"Logged data: {time_str}, {voltage_str}")
        except KeyboardInterrupt:
            print("Stopped reading data from UART.")
        except Exception as e:
            print(f"An error occurred: {e}")


log_data_to_csv(port, baud_rate, timeout, csv_filename)
