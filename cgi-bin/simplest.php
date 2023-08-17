<!DOCTYPE html>
<head>
<title>PHP Test</title>
</head>
<body>
<?php 
$url = getenv('FULL_URL');
$url_components = parse_url($url);
parse_str($url_components['query'], $params);
echo 'HIIII' .$params['name'];
foreach (getenv() as $key => $value) {
	echo $key . ' - ' . $value . '
	';
}
if (isset($_GET['name'])) {
	$name = htmlspecialchars($_GET['name']);
	echo '<p>Hello $name!</p>';
} else {
	echo '<p>Hello       !</p>';
}
?>
<?php
echo "Content-Type: text/plain\r\n\r\n";

function printSuperglobal($name, $array) {
    echo "Superglobal: $name\n";
    foreach ($array as $key => $value) {
        if (is_array($value)) {
            echo "  $key:\n";
            foreach ($value as $subkey => $subvalue) {
                echo "    [$subkey] => $subvalue\n";
            }
        } else {
            echo "  $key => $value\n";
        }
    }
    echo "\n";
}

printSuperglobal('$_GET', $_GET);
printSuperglobal('$_POST', $_POST);
printSuperglobal('$_COOKIE', $_COOKIE);
printSuperglobal('$_FILES', $_FILES);
printSuperglobal('$_SERVER', $_SERVER);
printSuperglobal('$_ENV', $_ENV);
printSuperglobal('$_REQUEST', $_REQUEST);
printSuperglobal('$_SESSION', $_SESSION);
?>
</body>
</html>
