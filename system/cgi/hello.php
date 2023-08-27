#!/usr/bin/php-cgi
<?php

// get query string from env
$query_string = getenv('QUERY_STRING');

// parse the query string into key value pairs
parse_str($query_string, $query_vars);

// extract value for specific keys into vars
$first_name = $query_vars['first_name'] ?? '';
$last_name = $query_vars['last_name'] ?? '';

$html_content = <<<HTML
<!DOCTYPE html>
<html>

<head>
	<title>PHP CGI Example</title>
	<link rel="stylesheet" type="text/css" href="/styles.css"/>
</head>

<body>
	<h1>PHP CGI:<br>GET with query string</h1>
	<p>Hello $first_name $last_name!</p>
</body>

</html>
HTML;

echo $html_content;

?>
