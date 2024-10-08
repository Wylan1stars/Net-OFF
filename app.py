from flask import Flask, render_template, jsonify, request
import os
import platform
import json
import uuid
import random
import string
import tkinter as tk
from tkinter import messagebox
from threading import Thread
import time
import subprocess

app = Flask(__name__)

# Archivo donde se almacenarán los dispositivos vinculados
vinculados_file = 'vinculados.json'

# Función para cargar dispositivos vinculados
def cargar_vinculados():
    try:
        with open(vinculados_file, 'r') as f:
            return json.load(f)
    except FileNotFoundError:
        return {}

# Función para guardar dispositivos vinculados
def guardar_vinculados(vinculados):
    with open(vinculados_file, 'w') as f:
        json.dump(vinculados, f)

# Función para generar un código de vinculación aleatorio
def generar_codigo_vinculacion():
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=6))

# Código de vinculación generado aleatoriamente
codigo_vinculacion = generar_codigo_vinculacion()

# Función para verificar el nombre de la red Wi-Fi
def obtener_nombre_wifi():
    sistema = platform.system()
    try:
        if sistema == "Windows":
            # Comando de PowerShell para obtener el nombre de la red Wi-Fi en Windows
            comando = 'netsh wlan show interfaces | findstr /C:" SSID"'
            resultado = subprocess.check_output(comando, shell=True).decode('utf-8').strip()
            return resultado.split(":")[-1].strip()
        elif sistema == "Linux" or sistema == "Darwin":
            # En Linux o macOS, utiliza el comando nmcli
            comando = "nmcli -t -f active,ssid dev wifi | egrep '^yes'"
            resultado = subprocess.check_output(comando, shell=True).decode('utf-8').strip()
            return resultado.split(":")[-1].strip()
    except Exception as e:
        return "No conectado a Wi-Fi"

# Rutas de la aplicación Flask
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/codigo_vinculacion', methods=['GET'])
def obtener_codigo_vinculacion():
    return jsonify({"codigo": codigo_vinculacion}), 200

@app.route('/apagar_pc', methods=['GET'])
def apagar_pc():
    if not verificar_vinculacion(request.args.get('codigo')):
        return jsonify({"error": "Dispositivo no vinculado"}), 403

    sistema = platform.system()
    if sistema == "Windows":
        os.system("shutdown /s /t 1")
    elif sistema == "Linux":
        os.system("shutdown now")
    elif sistema == "Darwin":
        os.system("shutdown -h now")
    else:
        return jsonify({"error": "Sistema operativo no soportado"}), 400
    
    return jsonify({"message": "PC apagada"}), 200

@app.route('/reiniciar_pc', methods=['GET'])
def reiniciar_pc():
    if not verificar_vinculacion(request.args.get('codigo')):
        return jsonify({"error": "Dispositivo no vinculado"}), 403

    sistema = platform.system()
    if sistema == "Windows":
        os.system("shutdown /r /t 1")
    elif sistema == "Linux":
        os.system("reboot")
    elif sistema == "Darwin":
        os.system("sudo reboot")
    else:
        return jsonify({"error": "Sistema operativo no soportado"}), 400
    
    return jsonify({"message": "PC reiniciada"}), 200

@app.route('/vincular', methods=['POST'])
def vincular():
    codigo = request.json.get('codigo')
    if codigo != codigo_vinculacion:
        return jsonify({"error": "Código incorrecto"}), 403
    
    vinculados = cargar_vinculados()
    if codigo in vinculados:
        return jsonify({"message": "Dispositivo ya vinculado"}), 400

    vinculados[codigo] = str(uuid.uuid4())  # Genera un ID único para el dispositivo
    guardar_vinculados(vinculados)
    return jsonify({"message": "Dispositivo vinculado exitosamente"}), 200

def verificar_vinculacion(codigo):
    vinculados = cargar_vinculados()
    return codigo in vinculados

# Función para iniciar la interfaz gráfica con tkinter
def iniciar_interfaz():
    root = tk.Tk()
    root.title("Vinculación de Dispositivo")

    # Función que muestra el estado de la conexión y Wi-Fi
    def verificar_estado():
        while True:
            wifi = obtener_nombre_wifi()
            label_wifi.config(text=f"Red Wi-Fi: {wifi}")
            time.sleep(5)

    def aceptar_vinculacion():
        messagebox.showinfo("Vinculación", f"Dispositivo vinculado correctamente.\nCódigo: {codigo_vinculacion}")

    # Mostrar el código de vinculación
    label_codigo = tk.Label(root, text=f"Código de vinculación: {codigo_vinculacion}", font=("Arial", 14))
    label_codigo.pack(pady=10)

    # Mostrar la red Wi-Fi
    label_wifi = tk.Label(root, text="Red Wi-Fi: Desconocida", font=("Arial", 12))
    label_wifi.pack(pady=10)

    # Botón para aceptar la vinculación
    btn_aceptar = tk.Button(root, text="Aceptar Vinculación", command=aceptar_vinculacion, font=("Arial", 12))
    btn_aceptar.pack(pady=10)

    # Ejecutar la verificación del estado de Wi-Fi en un hilo separado
    Thread(target=verificar_estado).start()

    root.mainloop()

# Ejecutar la interfaz en un hilo separado para no bloquear Flask
if __name__ == '__main__':
    Thread(target=iniciar_interfaz).start()
    app.run(host='0.0.0.0', port=5000)
