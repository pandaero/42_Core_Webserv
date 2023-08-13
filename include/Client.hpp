#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class	Client
{
	public:
		Client(int);
		~Client();

		//sockaddr_in*			sockaddr();
		int						pollStructIndex();
		
		void					buildRequest();

		std::string			httpProtocol();
		std::string 		method();
		std::string 		path();
		std::string			directory();
		int					contentLength();
		std::string 		contentType();
		
		std::string				sendPath;
		std::streampos			filePosition;
		int						statusCode;
		
		int						fd;
		std::string				buffer;
		
		bool					errorPending;
		bool					requestHeadComplete;
		bool					requestBodyComplete;
		bool					responseFileSelected;
		bool					responseHeadSent;
		bool					responseBodySent;
		
	private:
		Request*				_request;
		
		int						_pollStructIndex;
		//sockaddr_in			_clientAddress;

		std::string				_directory;
};

#endif
