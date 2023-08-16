import cgi
import os

def handle_file_upload():
    target_dir = "uploads/"
    form = cgi.FieldStorage()

    if "file" in form:
        file_item = form["file"]
        target_file = target_dir + file_item.filename

        if file_item.file:
            with open(target_file, "wb") as f:
                f.write(file_item.file.read())

            return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nFile uploaded successfully."

        return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\nSorry, there was an error uploading your file."

    return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\nInvalid file format."

def handle_post_request():
    form = cgi.FieldStorage()

    if "name" in form and "age" in form:
        name = form["name"].value
        age = form["age"].value

        response = (
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
            "<!DOCTYPE html><html><head><title>POST Request Example - Result</title></head><body>"
            "<h1>POST Request Example - Result</h1>"
            f"<p>Hello, {name}! You are {age} years old.</p></body></html>"
        )

        return response

    return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\nMissing name or age parameter."

def main():
    print("Content-Type: text/html\r\n\r\n")
    request_method = os.environ.get("REQUEST_METHOD")

    if request_method == "POST":
        print(handle_file_upload() if "file" in cgi.FieldStorage() else handle_post_request())

if __name__ == "__main__":
    main()