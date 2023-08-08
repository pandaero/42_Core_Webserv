#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

class	Request
{
	public:
		Request();
		Request(std::string);
		
		std::string			headerValue(std::string);
		
		const std::string&	method() const;
		const std::string&	path() const;
		const std::string&	httpProtocol() const;
		const int&			contentLength() const;
		const std::string&	contentType() const;

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
