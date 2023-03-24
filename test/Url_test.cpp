// Tests for the URL Class

#include "../include/Url.hpp"

#include <iostream>

int	main()
{
	std::vector<std::string>									validStr;
	std::vector<std::vector<std::pair<bool, std::string> > >	expected;
	
	std::vector<std::pair<bool, std::string> >	defaultPairVector(6);
	for (int i = 0; i < 6; ++i)
		defaultPairVector.push_back(std::make_pair(false, ""));

	std::vector<std::pair<bool, std::string> >	reUsePairVector(6);
	
	validStr.push_back("http://www.domain.com");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "www.domain.com";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com:8080/");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "www.domain.com";
		reUsePairVector[PORT].first = true;
		reUsePairVector[PORT].second = "8080";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com:8080/hello.html?key=1#hello");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "www.domain.com";
		reUsePairVector[PORT].first = true;
		reUsePairVector[PORT].second = "8080";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello.html";
		reUsePairVector[PARAMETERS].first = true;
		reUsePairVector[PARAMETERS].second = "key=1";
		reUsePairVector[ANCHOR].first = true;
		reUsePairVector[ANCHOR].second = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com:8080/hello.html#hello");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "www.domain.com";
		reUsePairVector[PORT].first = true;
		reUsePairVector[PORT].second = "8080";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello.html";
		reUsePairVector[ANCHOR].first = true;
		reUsePairVector[ANCHOR].second = "hello";
	}
	expected.push_back(reUsePairVector);

	validStr.push_back("http://www.domain.com:8080/hello.html?key=1");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "www.domain.com";
		reUsePairVector[PORT].first = true;
		reUsePairVector[PORT].second = "8080";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello.html";
		reUsePairVector[PARAMETERS].first = true;
		reUsePairVector[PARAMETERS].second = "key=1";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com:8080/hello.html");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "www.domain.com";
		reUsePairVector[PORT].first = true;
		reUsePairVector[PORT].second = "8080";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com/hello.html");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "www.domain.com";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com/hello.html?key=1");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "www.domain.com";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello.html";
		reUsePairVector[PARAMETERS].first = true;
		reUsePairVector[PARAMETERS].second = "key=1";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com/hello.html#hello");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "www.domain.com";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello.html";
		reUsePairVector[ANCHOR].first = true;
		reUsePairVector[ANCHOR].second = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com/hello.html?key=1#hello");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "www.domain.com";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello.html";
		reUsePairVector[PARAMETERS].first = true;
		reUsePairVector[PARAMETERS].second = "key=1";
		reUsePairVector[ANCHOR].first = true;
		reUsePairVector[ANCHOR].second = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("file:///hello.html");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "file";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("file:///hello/hello.html");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "file";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello/hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("file:///mainfolder/hello/hello.html");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "file";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "mainfolder/hello/hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1:8080/hello/hello.html");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "192.168.0.1";
		reUsePairVector[PORT].first = true;
		reUsePairVector[PORT].second = "8080";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello/hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1:8080/hello/hello.html?key=1#hello");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "192.168.0.1";
		reUsePairVector[PORT].first = true;
		reUsePairVector[PORT].second = "8080";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello/hello.html";
		reUsePairVector[PARAMETERS].first = true;
		reUsePairVector[PARAMETERS].second = "key=1";
		reUsePairVector[ANCHOR].first = true;
		reUsePairVector[ANCHOR].second = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1:8080/hello/hello.html#hello");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "192.168.0.1";
		reUsePairVector[PORT].first = true;
		reUsePairVector[PORT].second = "8080";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello/hello.html";
		reUsePairVector[ANCHOR].first = true;
		reUsePairVector[ANCHOR].second = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1/hello/hello.html");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "192.168.0.1";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello/hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1/hello/hello.html?key=1");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "192.168.0.1";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello/hello.html";
		reUsePairVector[PARAMETERS].first = true;
		reUsePairVector[PARAMETERS].second = "key=1";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1/hello/hello.html?key=1#hello");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "192.168.0.1";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello/hello.html";
		reUsePairVector[PARAMETERS].first = true;
		reUsePairVector[PARAMETERS].second = "key=1";
		reUsePairVector[ANCHOR].first = true;
		reUsePairVector[ANCHOR].second = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1/hello/hello.html#hello");
	{
		reUsePairVector = defaultPairVector;
		reUsePairVector[SCHEME].first = true;
		reUsePairVector[SCHEME].second = "http";
		reUsePairVector[DOMAJN].first = true;
		reUsePairVector[DOMAJN].second = "192.168.0.1";
		reUsePairVector[PATH].first = true;
		reUsePairVector[PATH].second = "hello/hello.html";
		reUsePairVector[ANCHOR].first = true;
		reUsePairVector[ANCHOR].second = "hello";
	}
	expected.push_back(reUsePairVector);
	
	std::cout << "Loaded test cases." << std::endl;

	std::vector<Url>	validUrl;
	try
	{
		for (size_t i = 0; i < validStr.size(); ++i)
			validUrl.push_back(Url(validStr[i]));
	}
	catch (std::exception & exc)
	{
		std::cerr << exc.what() << std::endl;
	}

	std::cout << "Contructed URL class objects." << std::endl;

	std::cout << "----- ----- ----- ----- ----- ----- ----- -----" << std::endl;
	for (size_t i = 0; i < validUrl.size(); ++i)
		for (int j = 0; j < 6; ++j)
			if (validUrl[i].getProperties()[j].first != expected[i][j].first || validUrl[i].getProperties()[j].second != expected[i][j].second)
			{
				std::cout << "Error: parsing: \"" << validStr[i] << "\"\nProperty: ";
				switch (j)
				{
					case 0:
						std::cout << "SCHEME\n";
						break;
					case 1:
						std::cout << "DOMAIN\n";
						break;
					case 2:
						std::cout << "PORT\n";
						break;
					case 3:
						std::cout << "PATH\n";
						break;
					case 4:
						std::cout << "PARAMETERS\n";
						break;
					case 5:
						std::cout << "ANCHOR\n";
						break;
				}
				std::cout << "Result  : " << validUrl[i].getProperties()[j].first << ", " << validUrl[i].getProperties()[j].second;
				std::cout << "\nExpected: " << expected[i][j].first << ", " << expected[i][j].second << std::endl;
				std::cout << "----- ----- ----- ----- ----- ----- ----- -----" << std::endl;
			}

// Invalid

	return (0);
}
