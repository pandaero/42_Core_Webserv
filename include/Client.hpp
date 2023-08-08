#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class	Client
{
	public:
		Client(int);
		~Client();

		sockaddr_in*			sockaddr();
		const int&				socketfd() const;
		const int&				pollStructIndex() const;
		
		void					setSocketfd(int);
		void					buildRequest();

		const std::string&		httpProtocol() const;
		const std::string& 		method() const;
		const std::string& 		path() const;
		const std::string&		directory() const;
		const int& 				contentLength() const;
		const std::string& 		contentType() const;
		std::string&			buffer();
		
		bool					requestHeadComplete();
		bool					requestBodyComplete();
		Request*				_request;
		
	private:
		std::string		_buffer;
		
		int				_clientSocketfd;
		int				_pollStructIndex;
		sockaddr_in		_clientAddress;

		std::string		_directory;
		size_t			_bodyBytesWritten;
		bool			_requestHeadComplete;
		bool			_requestBodyComplete;
};

#endif
