#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

class	Request
{
	public:
		Request();
		Request(std::string);
		
		std::string			headerValue(std::string);
		
		std::string	path();
		std::string	httpProtocol();
		int			contentLength();
		std::string	contentType();
		std::string	method();

	private:
		strMap		createHeaderMap(std::string&, std::string, std::string, std::string);

		std::string	_method;
		std::string	_path;
		std::string	_protocol;
		int			_contentLength;
		std::string	_contentType;
		strMap		_headers;
};
#endif
