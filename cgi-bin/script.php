<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_FILES['file'])) {
        // Handle file upload (same code as before)
        $targetDir = "uploads/";
        $targetFile = $targetDir . basename($_FILES['file']['name']);

        if (getimagesize($_FILES['file']['tmp_name']) !== false) {
            if (move_uploaded_file($_FILES['file']['tmp_name'], $targetFile)) {
                $response = "HTTP/1.1 200 OK\r\n";
                $response .= "Content-Type: text/html\r\n";
                $response .= "\r\n";
                $response .= "File uploaded successfully.";

                echo $response;
            } else {
                $response = "HTTP/1.1 500 Internal Server Error\r\n";
                $response .= "Content-Type: text/html\r\n";
                $response .= "\r\n";
                $response .= "Sorry, there was an error uploading your file.";

                echo $response;
            }
        } else {
            $response = "HTTP/1.1 400 Bad Request\r\n";
            $response .= "Content-Type: text/html\r\n";
            $response .= "\r\n";
            $response .= "Invalid file format.";

            echo $response;
        }
    } else {
        // Handle POST request (form submission)
        if (isset($_POST['name']) && isset($_POST['age'])) {
            $name = $_POST['name'];
            $age = $_POST['age'];

            $response = "HTTP/1.1 200 OK\r\n";
            $response .= "Content-Type: text/html\r\n";
            $response .= "\r\n";
            $response .= "<!DOCTYPE html>";
            $response .= "<html>";
            $response .= "<head>";
            $response .= "    <title>POST Request Example - Result</title>";
            $response .= "</head>";
            $response .= "<body>";
            $response .= "    <h1>POST Request Example - Result</h1>";
            $response .= "    <p>Hello, $name! You are $age years old.</p>";
            $response .= "</body>";
            $response .= "</html>";

            echo $response;
        } else {
            $response = "HTTP/1.1 400 Bad Request\r\n";
            $response .= "Content-Type: text/html\r\n";
            $response .= "\r\n";
            $response .= "Missing name or age parameter.";

            echo $response;
        }
    }
}
?>
