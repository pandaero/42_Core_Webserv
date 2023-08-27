#!/usr/bin/python3

# Import modules for CGI handling 
import cgi, cgitb, os

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')
last_name  = form.getvalue('last_name')

request_method = os.environ.get("REQUEST_METHOD", "N/A")

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

print(html_content.format(request_method, first_name, last_name))