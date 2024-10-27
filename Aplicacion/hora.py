import tkinter as tk
import json
import os
import subprocess
import logging
import sys

# Ruta del archivo de configuración
APP_DIR = os.path.dirname(os.path.abspath(sys.executable))  # Carpeta del ejecutable
CONFIG_FILE = os.path.join(APP_DIR, "config.json")
APP_EXE = os.path.join(APP_DIR, "app.exe")
LOG_FILE = os.path.join(APP_DIR, "hora.log")

# Configuración del log
logging.basicConfig(filename=LOG_FILE, level=logging.DEBUG)

def guardar_hora():
    """Guarda la hora ingresada en el archivo JSON y lanza app.exe."""
    hora = entry_hora.get()
    try:
        # Validar formato de hora (HH:MM)
        if len(hora) == 5 and hora[2] == ':' and hora[:2].isdigit() and hora[3:].isdigit():
            # Crear el archivo JSON con la hora ingresada
            with open(CONFIG_FILE, "w") as f:
                json.dump({"hora_apagado": hora}, f)
            logging.info(f"Hora guardada correctamente: {hora}")
            lbl_mensaje.config(text="¡Hora guardada correctamente!")

            # Verificar y ejecutar app.exe
            if os.path.exists(APP_EXE):
                logging.info(f"Iniciando {APP_EXE}")
                subprocess.Popen([APP_EXE], shell=True)
                ventana.destroy()  # Cerrar la ventana
            else:
                mensaje = f"Error: {APP_EXE} no encontrado."
                lbl_mensaje.config(text=mensaje)
                logging.error(mensaje)
        else:
            lbl_mensaje.config(text="Formato incorrecto. Usa HH:MM.")
            logging.warning("Formato de hora incorrecto ingresado.")
    except Exception as e:
        lbl_mensaje.config(text=f"Error: {e}")
        logging.error(f"Error al guardar la hora: {e}")

# Crear la ventana Tkinter
ventana = tk.Tk()
ventana.title("Configurar Hora de Apagado")
ventana.geometry("300x150")

# Mostrar el directorio de trabajo (para depuración)
lbl_debug = tk.Label(ventana, text=f"Dir: {APP_DIR}", font=("Arial", 8))
lbl_debug.pack(pady=5)

# Widgets de la ventana
lbl_instruccion = tk.Label(ventana, text="Ingresa la hora de apagado (HH:MM):")
lbl_instruccion.pack(pady=10)

entry_hora = tk.Entry(ventana, width=10)
entry_hora.pack(pady=5)

btn_guardar = tk.Button(ventana, text="Guardar", command=guardar_hora)
btn_guardar.pack(pady=10)

lbl_mensaje = tk.Label(ventana, text="")
lbl_mensaje.pack()

# Iniciar el bucle de la interfaz
ventana.mainloop()
