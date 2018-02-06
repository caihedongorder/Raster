#include "utils.h"

std::string LoadFileContent(const char* fileName, int & fileSize) {
    std::string fileContent; 
	fileSize = 0;
	FILE* file = fopen(fileName, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int filelen = ftell(file);
		if (filelen > 0)
		{
			char* Content = new char[filelen + 1];
			rewind(file);
			fread(Content, sizeof(char), filelen, file);
			Content[filelen] = 0;
            fileContent = std::string(Content);

            delete [] Content;
		}
		fclose(file);
		file = nullptr;
	}

	return fileContent;
}


