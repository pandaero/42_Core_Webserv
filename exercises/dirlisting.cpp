#include <fstream>
#include <sstream>
#include <iostream>
#include <dirent.h>

int main(void)
{
	    DIR *folder;
    struct dirent *dirElm;
    std::stringstream dirLisStr;

    dirLisStr << "<html><body>Directory Listing:<ul>";
    folder = opendir(".");
    dirElm = readdir(folder);
    std::cout << "Reading folder" << std::endl;
    
    while (dirElm != NULL)
    {
		std::string	compare(dirElm->d_name);
		if (compare == "..")
		{
			dirElm = readdir(folder);
			continue;
		}
		dirLisStr << "<li><a href=\">";
		if (compare == ".")
			dirLisStr << ".<\">.";
		else
			dirLisStr << compare << "<\">" << compare;
		dirLisStr << "</a></li>";
		dirElm = readdir(folder);
    }
    
    closedir(folder);
    std::cout << "Closed folder" << std::endl;
    
    dirLisStr << "</ul></body></html>";
    std::ofstream dirLisFile("file.html");
    dirLisFile << dirLisStr.rdbuf();
    dirLisFile.close();
    
    return 0;
}
