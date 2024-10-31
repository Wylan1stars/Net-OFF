<?php
// Configuración de conexión a la base de datos MySQL
$host = "localhost"; // Cambia a la IP o dominio del servidor MySQL
$db = "lperconti_netoff"; // Cambia al nombre de tu base de datos
$user = "lperconti_no@web.tecnica4berazategui.edu.ar"; // Cambia al usuario de tu base de datos
$password = "Noff2024!!"; // Cambia a la contraseña de tu base de datos

// Establecer conexión
$conn = new mysqli($host, $user, $password, $db);

// Comprobar conexión
if ($conn->connect_error) {
    die("Error de conexión a la base de datos: " . $conn->connect_error);
}

// Verificar si se recibe el ID del laboratorio en la solicitud
if (isset($_GET['laboratorio_id'])) {
    $laboratorio_id = intval($_GET['laboratorio_id']);

    // Consulta SQL para obtener el horario de inicio y apagado
    $sql = "SELECT hora_inicio, hora_apagado FROM horarios_laboratorio WHERE laboratorio_id = ?";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("i", $laboratorio_id);
    $stmt->execute();
    $result = $stmt->get_result();

    // Verificar si se encontraron resultados
    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        $response = array(
            "hora_inicio" => $row["hora_inicio"],
            "hora_apagado" => $row["hora_apagado"]
        );
    } else {
        // Respuesta en caso de que el laboratorio no exista en la base de datos
        $response = array("error" => "Laboratorio no encontrado");
    }

    // Liberar el resultado y cerrar la conexión
    $stmt->close();
} else {
    // Respuesta en caso de que no se proporcione un ID válido
    $response = array("error" => "ID de laboratorio no proporcionado");
}

$conn->close();

// Devolver la respuesta en formato JSON
header('Content-Type: application/json');
echo json_encode($response);
?>
