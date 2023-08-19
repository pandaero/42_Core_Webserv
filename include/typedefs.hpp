#ifndef TYPEDEFS_HPP
# define TYPEDEFS_HPP

// forward declarations
class	Client;
class	Server;
class	ServerConfig;
class	ConfigFile;

typedef struct
{
	bool			get;
	bool			post;
	bool			delete_;
	std::string		dir_listing;
	std::string		http_redir;
	std::string		upload_dir;
	std::string		std_file;
}	s_locInfo;

typedef std::map<std::string, std::string> 				strMap;
typedef std::map<std::string, std::string>::iterator	strMap_it;
typedef std::vector<std::string>						strVec;
typedef std::vector<std::string>::iterator				strVec_it;
typedef std::map<std::string, s_locInfo>				strLocMap;
typedef std::map<std::string, s_locInfo>::iterator		strLocMap_it;
typedef	std::map<int, std::string>						intStrMap;
typedef	std::map<int, std::string>::iterator			intStrMap_it;
typedef std::vector<Client>								clientVec;
typedef std::vector<Client>::iterator					clientVec_it;

#endif
