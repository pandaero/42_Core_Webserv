
#include "../include/Config.hpp"

void setField(Server& newServer, std::string key, std::string value)
{
	if (key == SERVERNAME)
		newServer.setName(value);
	else if (key == HOST)
		newServer.setHost(value);
	else if (key == PORT)
		newServer.setPort(value);
	else if (key == CLIMAXBODY)
		newServer.setClientMaxBody(value);
	else if (key == GET)
		newServer.setGet(true);

}

void parseConfigFile(const char* path)
{
	std::ifstream	infile(path);
	std::string		line;
	std::string		key;
	std::string		value;
	std::vector<Server>	serverList;
	
	if (!infile.is_open())
		throw std::range_error("Could not open config file.");		
	while (std::getline(infile, line))
	{
		if (trim(line) == "<server>")
		{
			Server newServer;

			while (std::getline(infile, line) && trim(line) != "</server>")
			{
				key = splitEraseStr(line, ":");
				value = trim(line);
				setField(newServer, key, value);
				//std::cout << "key: " << key << "\tvalue: " << value << std::endl;
			}
			serverList.push_back(newServer);
		}
	}
	
	
		/* size_t delim = line.find(',');
		if (delim == std::string::npos)
			continue;
		if (!date.parseDate(line.substr(0, delim)))
			continue;
		if (!parsePrice(line.substr(delim + 1, line.size()), &price))
			continue;
		_btc_price.insert(std::make_pair(date, price)); */
	infile.close();
}


