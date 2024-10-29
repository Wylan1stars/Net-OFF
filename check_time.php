<?php

// Datos de conexión a la base de datos
define ('DB_HOST', 'localhost');
define ('DB_USER', 'lperconti_no');
define ('DB_PASS', 'Noff2024!!');
define ('DB_NAME', 'lperconti_netoff');

class Conexion extends mysqli {
    public $enlace;

    function __construct() {
        $this->enlace = mysqli_connect(DB_HOST, DB_USER, DB_PASS, DB_NAME);
        if ($this->enlace->connect_error) {
            die("Conexión fallida: " . $this->enlace->connect_error);
        }
    }

    function __destruct() {
        mysqli_close($this->enlace);
    }
}

class Horario {
    private $conn;

    function __construct($conn) {
        $this->conn = $conn;
    }

    function verificarHorario() {
        $sql = "SELECT hora_inicio, hora_fin FROM horario WHERE id = 1"; // Cambia 'horario' e 'id' según tu estructura
        $result = $this->conn->query($sql);

        if ($result->num_rows > 0) {
            $row = $result->fetch_assoc();
            $hora_inicio = $row["hora_inicio"];
            $hora_fin = $row["hora_fin"];

            // Obtener la hora actual del servidor
            $hora_actual = date("H:i:s");

            // Comparar la hora actual con el horario permitido
            if ($hora_actual >= $hora_inicio && $hora_actual <= $hora_fin) {
                return json_encode(array("apagado" => false));
            } else {
                return json_encode(array("apagado" => true));
            }
        } else {
            return json_encode(array("error" => "No se encontraron horarios"));
        }
    }
}

// Crear conexión y verificar horario
$objConexion = new Conexion();
$horario = new Horario($objConexion->enlace);

// Mostrar el resultado en JSON
echo $horario->verificarHorario();

?>
