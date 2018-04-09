#include "string.hpp"
#include <fstream>

Array<String> StringFuncs::split(const String& s, char delim)
{
	Array<String> elems;
        
    const char* cstr = s.c_str();
    unsigned int strLength = (unsigned int)s.length();
    unsigned int start = 0;
    unsigned int end = 0;
        
    while(end <= strLength) {
        while(end <= strLength) {
            if(cstr[end] == delim) {
                break;
			}
            end++;
        }
            
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
        
    return elems;
}

String StringFuncs::getFilePath(const String& fileName)
{
	const char* cstr = fileName.c_str();
	unsigned int strLength = (unsigned int)fileName.length();
	unsigned int end = strLength - 1;
	
	while(end != 0) {
		if(cstr[end] == '/') {
			break;
		}
		end--;
	}

	if(end == 0) {
		return fileName;
	} else {
		unsigned int start = 0;
		end = end + 1;
		return fileName.substr(start, end - start);
	}
}

bool StringFuncs::loadTextFileWithIncludes(String& output, const String& fileName,
		const String& includeKeyword)
{
	std::ifstream file;
	file.open(fileName.c_str());

	String filePath = getFilePath(fileName);
	std::stringstream ss;
	String line;

	if(file.is_open()) {
		while(file.good()) {
			getline(file, line);
			
			if(line.find(includeKeyword) == String::npos) {
				ss << line << "\n";
			} else {
				String includeFileName = StringFuncs::split(line, ' ')[1];
				includeFileName =
					includeFileName.substr(1,includeFileName.length() - 2);

				String toAppend;
				loadTextFileWithIncludes(toAppend, filePath + includeFileName,
						includeKeyword);
				ss << toAppend << "\n";
			}
		}
	} else {
		DEBUG_LOG(LOG_TYPE_IO, LOG_ERROR, "Unable to load shader: %s",
				fileName.c_str());
		return false;
	}

	output = ss.str();
	return true;
}

