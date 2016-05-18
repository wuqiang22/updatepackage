#ifndef __CMD5Checksum__  
#define __CMD5Checksum__  

#include <string>
#include <cstring>

class CMD5Checksum
{
public:
	static std::string md5(std::string dat);
	static std::string md5(const void* dat, size_t len);
	static std::string md5file(const char* filename);
	static std::string md5file(FILE* file);
	static std::string md5sum6(std::string dat);
	static std::string md5sum6(const void* dat, size_t len);
};

#endif /* defined(__CMD5Checksum__) */ 