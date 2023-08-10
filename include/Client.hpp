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
		void					setReceivingComplete();
		void					buildRequest();

		const std::string&		httpProtocol() const;
		const std::string& 		method() const;
		const std::string& 		path() const;
		const std::string&		directory() const;
		const int& 				contentLength() const;
		const std::string& 		contentType() const;
		std::string&			buffer();
		std::string&			pathToSendFile();
		
		
		clientState				state;
		std::string				sendPath;
		
		bool					requestHeadComplete();
		bool					requestBodyComplete();
		bool					receivingComplete();
		bool					responseFileSelected();
		
	private:
		std::string		_buffer;
		Request*		_request;
		
		int				_socketfd;
		int				_pollStructIndex;
		sockaddr_in		_clientAddress;

		std::string		_directory;

		bool			_receivingComplete;
		bool			_requestHeadComplete;
		bool			_requestBodyComplete;
		bool			_responseFileSelected;
		
		size_t			_bodyBytesHandled;
		bool			_sendingComplete;
};

#endif
