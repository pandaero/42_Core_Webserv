#!/usr/bin/python3

# Import modules for CGI handling 
import cgi, cgitb, os

request_method = os.environ.get("REQUEST_METHOD", "N/A")
output_file = os.environ.get("OUTPUT_FILE", "")
input_file = os.environ.get("INPUT_FILE", "")

if request_method == "GET":
	form = cgi.FieldStorage()
	first_name = form.getvalue('first_name')
	last_name  = form.getvalue('last_name')

elif request_method == "POST":
    with open(input_file, 'r') as file:
        input_data = file.read()
    input_data = input_data.strip()
    input_pairs = input_data.split('&')
    input_dict = {pair.split('=')[0]: pair.split('=')[1] for pair in input_pairs}
    first_name = input_dict.get('first_name', '')
    last_name = input_dict.get('last_name', '')


html_content = """
<!DOCTYPE html>
<html>

<head>
	<title>Python CGI Example</title>
	<link rel="stylesheet" type="text/css" href="/styles.css"/>
</head>

<body>
	<h1>Python CGI:<br>{}</h1>
	<p>Hello {} {}!</p>
</body>

</html>
"""
with open(output_file, 'w') as file:
        file.write(html_content.format(request_method, first_name, last_name))