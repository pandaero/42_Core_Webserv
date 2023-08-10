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
		std::string		_statusPage;
		off_t			_bodySize;
		std::string		_filePath;
};

#endif
