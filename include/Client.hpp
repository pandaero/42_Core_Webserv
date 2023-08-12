#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class	Client
{
	public:
		Client(int);
		~Client();

		void					reset();
		//sockaddr_in*			sockaddr();
		const int&				pollStructIndex() const;
		
		void					buildRequest();

		const std::string&		httpProtocol() const;
		const std::string& 		method() const;
		const std::string& 		path() const;
		const std::string&		directory() const;
		const int& 				contentLength() const;
		const std::string& 		contentType() const;
		
		std::string				sendPath;
		std::streampos			filePosition;
		
		int						fd;
		std::string				buffer;
		
		bool					errorPending;
		bool					requestHeadComplete;
		bool					requestBodyComplete;
		bool					responseFileSelected;
		bool					responseHeadSent;
		
	private:
		Request*				_request;
		
		int						_pollStructIndex;
		//sockaddr_in				_clientAddress;

		std::string				_directory;
};

#endif
