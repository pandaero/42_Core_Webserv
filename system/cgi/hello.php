#!/usr/bin/php-cgi

<?php

$request_method = getenv('REQUEST_METHOD');
$output_file = getenv('OUTPUT_FILE');

if ($request_method === 'GET')
{
	// get query string from env
	$query_string = getenv('QUERY_STRING');
	
	// parse the query string into key value pairs
	parse_str($query_string, $query_vars);
	
	// extract value for specific keys into vars
	$first_name = $query_vars['first_name'] ?? 'none';
	$last_name = $query_vars['last_name'] ?? 'none';
}

if ($request_method === 'POST')
{
	$input_file = getenv('INPUT_FILE');
	$input_data = file_get_contents($input_file);

	parse_str($input_data, $post_vars);

	$first_name = $post_vars['first_name'] ?? 'none';
	$last_name = $post_vars['last_name'] ?? 'none';
}

$html_content = <<<HTML
<!DOCTYPE html>
<html>

<head>
	<title>PHP CGI Example</title>
	<link rel="stylesheet" type="text/css" href="/styles.css"/>
</head>

<body>
	<h1>PHP CGI:<br>$request_method</h1>
	<p>Hello $first_name $last_name!</p>
</body>

</html>
HTML;

file_put_contents($output_file, $html_content);

?>
