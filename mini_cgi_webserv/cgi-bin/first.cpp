#include <iostream>

int main() {

    std::cout << "HTTP/1.1 200 OK" << std::endl;
    std::cout << "Content-Type: text/plain" << std::endl;
    std::cout << "Content-Lenght: 28" << std::endl;
    std::cout << std::endl;
    std::cout << "Hello from the first CGI!!!!" << std::endl;
}