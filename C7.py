import numpy as np
import matplotlib.pyplot as plt

def read_temperature_matrix(port, baudrate=9600, timeout=5):
    ser = serial.Serial(port, baudrate, timeout=timeout)
    
    # Inicializa la gráfica una vez
    fig, ax, img = initialize_plot()
    
    while True:  # Bucle infinito para lectura continua
        try:
            line = ser.readline().decode('utf-8').strip()
            print(f"Received line: {line}")  # Línea de depuración
            if "Error: Sensor did not ack" in line:
                print("Error detectado, saltando línea")
                continue  # Salta la línea con error y sigue al siguiente ciclo
            if line:
                values = list(map(float, line.split(',')))
                if len(values) == 768:
                    values = [0 if np.isnan(x) else x for x in values]  # Reemplaza NaN por 0
                    matrix = np.array(values).reshape((24, 32))
                    update_plot(matrix, img, fig)  # Actualiza la gráfica con la nueva matriz
                else:
                    print("Error: Número incorrecto de datos recibidos")
            else:
                print("Error: Línea vacía recibida")
        except ValueError as ve:
            print(f"Error de valor: {ve}")
        except Exception as e:
            print(f"Error: {e}")

def initialize_plot():
    """Inicializa la figura y el objeto de la gráfica."""
    fig, ax = plt.subplots(figsize=(10, 8))
    data = np.zeros((24, 32))  # Matriz inicial vacía
    img = ax.imshow(data, cmap='hot', interpolation='nearest')
    plt.colorbar(img, ax=ax, label="Temperatura (°C)")
    ax.set_title("Matriz de Temperatura (MLX90640)")
    ax.set_xlabel("Columnas (pixeles)")
    ax.set_ylabel("Filas (pixeles)")
    plt.ion()  # Modo interactivo
    plt.show()
    return fig, ax, img

def update_plot(data, img, fig):
    """Actualiza los datos de la gráfica existente."""
    img.set_data(data)  # Actualiza los datos de la imagen
    img.set_clim(vmin=np.min(data), vmax=np.max(data))  # Ajusta los límites del color
    fig.canvas.draw()  # Dibuja los cambios
    fig.canvas.flush_events()  # Actualiza la ventana

# Cambia el puerto COM según tu configuración
port = "COM8"
read_temperature_matrix(port)