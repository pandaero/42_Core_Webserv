// Tests for the URL Class

#include "../include/Url.hpp"

#include <iostream>
#include <iomanip>

int	main()
{
	std::vector<std::string>				validStr;
	std::vector<std::vector<std::string> >	expected;
	
	std::vector<std::string>	defaultVector(6);
	for (int i = 0; i < 6; ++i)
		defaultVector.push_back("");

	std::vector<std::string>	reUsePairVector(6);
	
	validStr.push_back("http://www.domain.com");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "www.domain.com";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com:8080/");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "www.domain.com";
		reUsePairVector[PORT] = "8080";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com:8080/hello.html?key=1#hello");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "www.domain.com";
		reUsePairVector[PORT] = "8080";
		reUsePairVector[PATH] = "hello.html";
		reUsePairVector[PARAMETERS] = "key=1";
		reUsePairVector[ANCHOR] = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com:8080/hello.html#hello");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "www.domain.com";
		reUsePairVector[PORT] = "8080";
		reUsePairVector[PATH] = "hello.html";
		reUsePairVector[ANCHOR] = "hello";
	}
	expected.push_back(reUsePairVector);

	validStr.push_back("http://www.domain.com:8080/hello.html?key=1");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "www.domain.com";
		reUsePairVector[PORT] = "8080";
		reUsePairVector[PATH] = "hello.html";
		reUsePairVector[PARAMETERS] = "key=1";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com:8080/hello.html");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "www.domain.com";
		reUsePairVector[PORT] = "8080";
		reUsePairVector[PATH] = "hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com/hello.html");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "www.domain.com";
		reUsePairVector[PATH] = "hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com/hello.html?key=1");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "www.domain.com";
		reUsePairVector[PATH] = "hello.html";
		reUsePairVector[PARAMETERS] = "key=1";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com/hello.html#hello");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "www.domain.com";
		reUsePairVector[PATH] = "hello.html";
		reUsePairVector[ANCHOR] = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://www.domain.com/hello.html?key=1#hello");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "www.domain.com";
		reUsePairVector[PATH] = "hello.html";
		reUsePairVector[PARAMETERS] = "key=1";
		reUsePairVector[ANCHOR] = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("file:///hello.html");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "file";
		reUsePairVector[PATH] = "hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("file:///hello/hello.html");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "file";
		reUsePairVector[PATH] = "hello/hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("file:///mainfolder/hello/hello.html");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "file";
		reUsePairVector[PATH] = "mainfolder/hello/hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1:8080/hello/hello.html");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "192.168.0.1";
		reUsePairVector[PORT] = "8080";
		reUsePairVector[PATH] = "hello/hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1:8080/hello/hello.html?key=1#hello");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "192.168.0.1";
		reUsePairVector[PORT] = "8080";
		reUsePairVector[PATH] = "hello/hello.html";
		reUsePairVector[PARAMETERS] = "key=1";
		reUsePairVector[ANCHOR] = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1:8080/hello/hello.html#hello");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "192.168.0.1";
		reUsePairVector[PORT] = "8080";
		reUsePairVector[PATH] = "hello/hello.html";
		reUsePairVector[ANCHOR] = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1/hello/hello.html");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "192.168.0.1";
		reUsePairVector[PATH] = "hello/hello.html";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1/hello/hello.html?key=1");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "192.168.0.1";
		reUsePairVector[PATH] = "hello/hello.html";
		reUsePairVector[PARAMETERS] = "key=1";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1/hello/hello.html?key=1#hello");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "192.168.0.1";
		reUsePairVector[PATH] = "hello/hello.html";
		reUsePairVector[PARAMETERS] = "key=1";
		reUsePairVector[ANCHOR] = "hello";
	}
	expected.push_back(reUsePairVector);
	validStr.push_back("http://192.168.0.1/hello/hello.html#hello");
	{
		reUsePairVector = defaultVector;
		reUsePairVector[SCHEME] = "http";
		reUsePairVector[DOMAJN] = "192.168.0.1";
		reUsePairVector[PATH] = "hello/hello.html";
		reUsePairVector[ANCHOR] = "hello";
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
			if (validUrl[i].getProperties()[j] != expected[i][j] || validUrl[i].getProperties()[j] != expected[i][j])
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
				std::cout << "Result  : " << validUrl[i].getProperties()[j];
				std::cout << "\nExpected: " << expected[i][j] << std::endl;
				std::cout << "----- ----- ----- ----- ----- ----- ----- -----" << std::endl;
			}
	std::cout << "Performed all valid tests." << std::endl;
	std::cout << "----- ----- ----- ----- ----- ----- ----- -----" << std::endl;

// Invalid
	std::vector<std::string>	inValidStr;
	inValidStr.push_back("http//://192.168.0.1/hello/hello.html#hello");
	inValidStr.push_back("http:/192.16");
	inValidStr.push_back("http:192.168.0.1/hello/hello.html#hello");

	bool				passed = false;	
	std::vector<Url>	inValidUrl;
	for (size_t i = 0; i < inValidStr.size(); ++i)
	{
		std::cout << "\"" << std::setw(50) << std::left << inValidStr[i] + "\"" << " - ";
		try
		{
			Url	trying(inValidStr[i]);
			inValidUrl.push_back(trying);
		}
		catch(const std::exception& exc)
		{
			std::cout << "PASSED.";
			passed = true;
		}
		if (!passed)
			std::cout << "FAILED.";
		std::cout << "\n";
	}
	std::cout << "----- ----- ----- ----- ----- ----- ----- -----" << std::endl;
	std::cout << "Performed all invalid tests." << std::endl;
	std::cout << "----- ----- ----- ----- ----- ----- ----- -----" << std::endl;
	return (0);
}
