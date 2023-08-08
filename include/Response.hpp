#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

class	Response
{
	public:
		Response();
		~Response();
		Response(int, strVec serverNames);
		Response(const Request &, const Server &);

		void			setFile(std::string, const Server &);

		int				send(int, const Server &);
		
		const char*		getStatusPage();

		std::string		getFilePath();
		off_t			getSize();
		
		std::string 	_responseHeader;
		int				_statusCode;
		std::string		_statusMessage;

	private:
		void			build();
		std::string		getHttpMsg(int);
		
		std::string		_statusPage;
		contentType		_contentType;
		off_t			_bodySize;
		std::string		_filePath;
};

#endif
