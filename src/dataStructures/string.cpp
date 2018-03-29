#include "string.hpp"

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

