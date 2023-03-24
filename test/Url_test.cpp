// Tests for the URL Class

#include "../include/Url.hpp"

#include <iostream>

int	main()
{
	std::vector<std::string>									valid;
	std::vector<std::vector<std::pair<bool, std::string> > >	validIn;
	
	std::pair<bool, std::string>				reUsePair;
	std::vector<std::pair<bool, std::string> >	reUsePairVector(6);
	
	valid.push_back("http://www.domain.com:8080/hello.html?key=1#hello");
	{
		reUsePair.first = true;
		reUsePair.second = "http";
		reUsePairVector[SCHEME] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "www.domain.com";
		reUsePairVector[DOMAIN] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "8080";
		reUsePairVector[PORT] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "hello.html";
		reUsePairVector[PATH] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "key=1";
		reUsePairVector[PARAMETERS] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "hello";
		reUsePairVector[ANCHOR] = reUsePair;
	}
	validIn.push_back(reUsePairVector);
	valid.push_back("http://www.domain.com:8080/hello.html#hello");
	{
		reUsePair.first = true;
		reUsePair.second = "http";
		reUsePairVector[SCHEME] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "www.domain.com";
		reUsePairVector[DOMAIN] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "8080";
		reUsePairVector[PORT] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "hello.html";
		reUsePairVector[PATH] = reUsePair;
		reUsePair.first = false;
		reUsePair.second = "";
		reUsePairVector[PARAMETERS] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "hello";
		reUsePairVector[ANCHOR] = reUsePair;
	}
	validIn.push_back(reUsePairVector);

	valid.push_back("http://www.domain.com:8080/hello.html?key=1");
	{
		reUsePair.first = true;
		reUsePair.second = "http";
		reUsePairVector[SCHEME] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "www.domain.com";
		reUsePairVector[DOMAIN] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "8080";
		reUsePairVector[PORT] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "hello.html";
		reUsePairVector[PATH] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "key=1";
		reUsePairVector[PARAMETERS] = reUsePair;
		reUsePair.first = false;
		reUsePair.second = "";
		reUsePairVector[ANCHOR] = reUsePair;
	}
	validIn.push_back(reUsePairVector);

	valid.push_back("http://www.domain.com:8080/hello.html");
	{
		reUsePair.first = true;
		reUsePair.second = "http";
		reUsePairVector[SCHEME] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "www.domain.com";
		reUsePairVector[DOMAIN] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "8080";
		reUsePairVector[PORT] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "hello.html";
		reUsePairVector[PATH] = reUsePair;
		reUsePair.first = false;
		reUsePair.second = "";
		reUsePairVector[PARAMETERS] = reUsePair;
		reUsePair.first = false;
		reUsePair.second = "";
		reUsePairVector[ANCHOR] = reUsePair;
	}
	validIn.push_back(reUsePairVector);

	valid.push_back("http://www.domain.com:8080/");
	{
		reUsePair.first = true;
		reUsePair.second = "http";
		reUsePairVector[SCHEME] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "www.domain.com";
		reUsePairVector[DOMAIN] = reUsePair;
		reUsePair.first = true;
		reUsePair.second = "8080";
		reUsePairVector[PORT] = reUsePair;
		reUsePair.first = false;
		reUsePair.second = "";
		reUsePairVector[PATH] = reUsePair;
		reUsePair.first = false;
		reUsePair.second = "";
		reUsePairVector[PARAMETERS] = reUsePair;
		reUsePair.first = false;
		reUsePair.second = "";
		reUsePairVector[ANCHOR] = reUsePair;
	}
	validIn.push_back(reUsePairVector);

	// valid.push_back("http://www.domain.com/hello.html");
	// valid.push_back("http://www.domain.com/hello.html?key=1");
	// valid.push_back("http://www.domain.com/hello.html#hello");
	// valid.push_back("http://www.domain.com/hello.html?key=1#hello");
	// valid.push_back("file:///hello.html");
	// valid.push_back("file:///hello/hello.html");
	// valid.push_back("http://192.168.0.1:8080/hello/hello.html");
	// valid.push_back("http://192.168.0.1:8080/hello/hello.html?key=1#hello");
	// valid.push_back("http://192.168.0.1:8080/hello/hello.html#hello");
	// valid.push_back("http://192.168.0.1/hello/hello.html");
	// valid.push_back("http://192.168.0.1/hello/hello.html?key=1");
	// valid.push_back("http://192.168.0.1/hello/hello.html?key=1#hello");
	// valid.push_back("http://192.168.0.1/hello/hello.html#hello");
	
	std::cout << "Loaded test cases." << std::endl;

	std::vector<Url>	validUrl;
	try
	{
		for (size_t i = 0; i < valid.size(); ++i)
		{
			std::cout << "String: \"" << valid[i] << "\"" << std::endl;
			validUrl.push_back(Url(valid[i]));
		}
	}
	catch (std::exception & exc)
	{
		std::cerr << exc.what() << std::endl;
	}

	std::cout << "Contructed URL class objects" << std::endl;

	std::cout << "ValidIn: " << validIn[0][0].first << ", " << validIn[0][0].second << std::endl;
	std::cout << "ValidUrl: " << validUrl[0].getProperties()[1].first << ", " << validUrl[0].getProperties()[1].second << std::endl;

	// std::vector<std::pair<bool, std::string> >	prop = validUrl[0].getProperties();
	// std::cout << "Bool: " << validUrl[0].getProperties()[0].first; //<< "Value: " << prop[0].second << std::endl;
	std::cout << "First: " << (validUrl[0].getProperties())[0].first << std::endl;
	for (size_t i = 0; i < validUrl.size(); ++i)
		for (int j = 0; j < 6; ++j)
			if (validUrl[i].getProperties()[j].first != validIn[i][j].first || validUrl[i].getProperties()[j].second != validIn[i][j].second)
			{
				std::cout << "Error: parsing: \"" << valid[i] << "\"\nResult  : " << validUrl[i].getProperties()[j].first << ", " << validUrl[i].getProperties()[j].second;
				std::cout << "\nExpected: " << validIn[i][j].first << ", " << validIn[i][j].second << std::endl;
			}

// Invalid

	return (0);
}
