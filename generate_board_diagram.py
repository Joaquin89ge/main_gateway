import re

# Ruta al archivo de configuración
default_config_path = "src/config.h"
default_output_path = "docs/diagramas_arquitectura/conexiones_placa.mmd"

# Diccionario para mapear módulos a pines
pin_map = {
    "LORA_CS": "RFM95_CS",
    "LORA_INT": "RFM95_INT",
    "RTC_IO": "RTC_DAT",
    "RTC_SCLK": "RTC_CLK",
    "RTC_CE": "RTC_RST",
    "DHT22": "DHT_PIN",
    "GPS_RX": "GPS_RX",
    "GPS_TX": "GPS_TX"
}


def extract_pins(config_path):
    pins = {}
    with open(config_path, "r", encoding="utf-8") as f:
        for line in f:
            for key, macro in pin_map.items():
                match = re.match(rf"#define\s+{macro}\s+(\w+)", line)
                if match:
                    pins[key] = match.group(1)
    return pins


def generate_mermaid(pins, output_path):
    with open(output_path, "w", encoding="utf-8") as f:
        f.write("---\ntitle: Diagrama de Conexiones de la Placa\n---\n\n")
        f.write("%% Diagrama generado automáticamente desde config.h\n")
        f.write("graph TD\n")
        f.write("    ESP[ESP8266 / ESP32]\n")
        f.write("    LORA[LoRa SX1278]\n")
        f.write("    RTC[RTC DS1302]\n")
        f.write("    DHT[DHT22 Sensor]\n")
        f.write("    GPS[GPS Neo-6M]\n")
        f.write("    POWER[Fuente de Alimentación]\n")
        f.write("\n")
        f.write("    POWER --> ESP\n")
        f.write(f"    ESP -- SPI (CS={pins.get('LORA_CS', '?')}) --> LORA\n")
        f.write(f"    ESP -- INT (INT={pins.get('LORA_INT', '?')}) --> LORA\n")
        f.write(
            f"    ESP -- 3-wire (IO={pins.get('RTC_IO', '?')}, SCLK={pins.get('RTC_SCLK', '?')}, CE={pins.get('RTC_CE', '?')}) --> RTC\n")
        f.write(f"    ESP -- GPIO (DHT={pins.get('DHT22', '?')}) --> DHT\n")
        f.write(
            f"    ESP -- UART (RX={pins.get('GPS_RX', '?')}, TX={pins.get('GPS_TX', '?')}) --> GPS\n")
        f.write("    LORA -- Antena --> ANT[Antena 433MHz]\n")


if __name__ == "__main__":
    pins = extract_pins(default_config_path)
    generate_mermaid(pins, default_output_path)
    print(f"Diagrama generado correctamente en: {default_output_path}")
