#include <iostream>
#include <fstream>
#include <cstdio>

int main()
{
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t bytesRead;

	pFile = std::fopen("webserv.conf", "r");
	if (!pFile) {std::cout << "error opening file"; return 1;}

 	fseek(pFile, 0, SEEK_END);

	lSize = ftell(pFile);
	rewind (pFile);

	

	/* size_t fread ( void * ptr, size_t size, size_t count, FILE * stream ); */
	// say we have the size file before reading

	buffer = (char*)malloc(sizeof(lSize));
	bytesRead = fread(buffer,1,lSize,pFile);

	if (bytesRead != lSize)
	{std::cout << "Error reading file\n"; return 1;}

	std::cout << "file content: \n" << buffer << std::endl;
	fclose(pFile);
	free(buffer);
	return 0;

}
