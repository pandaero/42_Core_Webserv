#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

/*
This class is just a glorified struct. It is only instantiated as a private member
of the Server class. Therefore everything is public.
*/
class	Client
{
	public:
		Client(int);
		~Client();

		void					reset();
		void					parseRequest();
		
		// infrastructure vars
		int						fd;
		int						pollStructIndex;
		std::string				buffer;
		
		// request vars
		std::string				httpProtocol;
		std::string		 		method;
		std::string		 		path;
		std::string				directory;
		strMap					headers;
		int						contentLength;
		std::string		 		contentType;

		// response vars
		std::string				sendPath;
		std::streampos			filePosition;
		
		// status bools
		bool					errorPending;
		bool					requestHeadComplete;
		bool					requestBodyComplete;
		bool					responseFileSelected;
		bool					responseHeadSent;
		
	private:
		strMap					createHeaderMap(std::string&, std::string, std::string, std::string);
};

#endif
