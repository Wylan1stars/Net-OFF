import os
import time
import json
from datetime import datetime
import logging
import sys

# Ruta del archivo de configuración y log
APP_DIR = os.path.dirname(os.path.abspath(sys.executable))
CONFIG_FILE = os.path.join(APP_DIR, "config.json")
LOG_FILE = os.path.join(APP_DIR, "app.log")

# Configuración del log
logging.basicConfig(filename=LOG_FILE, level=logging.DEBUG, format="%(asctime)s - %(levelname)s - %(message)s")

def obtener_hora_apagado():
    """Lee la hora de apagado desde el archivo JSON."""
    try:
        if os.path.exists(CONFIG_FILE):
            with open(CONFIG_FILE, "r") as f:
                data = json.load(f)
                hora_apagado = data.get("hora_apagado")
                logging.info(f"Hora de apagado obtenida: {hora_apagado}")
                return hora_apagado
        else:
            logging.warning("El archivo config.json no existe.")
    except Exception as e:
        logging.error(f"Error al leer el archivo config.json: {e}")
    return None

def apagar_equipo():
    """Ejecuta el comando para apagar el equipo."""
    try:
        logging.info("Ejecutando comando de apagado...")
        if os.name == "nt":  # Windows
            os.system("shutdown /s /t 1")
        else:  # Linux / Mac
            os.system("sudo shutdown now")
        logging.info("Comando de apagado ejecutado.")
    except Exception as e:
        logging.error(f"Error al intentar apagar el equipo: {e}")

def main():
    """Bucle principal que verifica la hora constantemente."""
    hora_apagado = obtener_hora_apagado()
    if not hora_apagado:
        logging.error("No se ha configurado una hora de apagado.")
        return

    logging.info(f"Esperando a que la hora actual sea: {hora_apagado}")
    while True:
        hora_actual = datetime.now().strftime("%H:%M")
        logging.debug(f"Hora actual: {hora_actual}")
        if hora_actual == hora_apagado:
            logging.info("¡Hora alcanzada! Apagando el equipo...")
            apagar_equipo()
            break
        time.sleep(30)  # Esperar 30 segundos antes de volver a verificar

if __name__ == "__main__":
    main()
