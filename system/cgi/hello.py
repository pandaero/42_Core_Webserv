#!/usr/bin/python3

import os, sys, cgi

form = cgi.FieldStorage() # python will automatically read from stdin if no param is given
first_name = form.getvalue('first_name')
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