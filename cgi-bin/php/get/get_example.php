<?php
// get_example.php

echo "<!DOCTYPE html>";
echo "<html>";
echo "<head>";
echo "    <title>GET Request Example - Result</title>";
echo "</head>";
echo "<body>";
echo "    <h1>GET Request Example - Result</h1>";

// Check if name and age parameters are set
if (isset($_GET['name']) && isset($_GET['age'])) {
    $name = $_GET['name'];
    $age = $_GET['age'];
    
    echo "    <p>Hello, $name! You are $age years old.</p>";
} else {
    echo "    <p>Missing name or age parameter.</p>";
}

echo "</body>";
echo "</html>";
?>
