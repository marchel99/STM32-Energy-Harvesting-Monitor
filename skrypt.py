import serial
import csv
from datetime import datetime

# Konfiguracja portu szeregowego
port = '/dev/ttyUSB0'  
baud_rate = 115200
timeout = 1  # W sekundach

# Otwarcie portu szeregowego
with serial.Serial(port, baud_rate, timeout=timeout) as ser, open('output.csv', 'w', newline='') as csvfile:
    csv_writer = csv.writer(csvfile)
    
    # Nagłówek CSV
    csv_writer.writerow(['Timestamp', 'Data'])
    
    try:
        while True:
            # Odczyt linii z UART
            line = ser.readline().decode('utf-8').rstrip()
            
            # Zapis do CSV, jeśli linia nie jest pusta
            if line:
                timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                csv_writer.writerow([timestamp, line])
                print(f"{timestamp}, {line}")
                
    except KeyboardInterrupt:
        # Przerwanie pętli przez użytkownika
        print("Odczyt zakończony przez użytkownika.")
