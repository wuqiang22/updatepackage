#include "ResourceEncrypt.h"
#include <algorithm>

std::string ResourceEncrypt::DEFAULT_ENCRYPT_KEY = "06156415l;km;pjohabsf";
std::string ResourceEncrypt::DEFAULT_ENCRYPT_FLAG = "btpjmzzwq";
//mac android windows不区分文件大小写(mac android 是某些分区)
static const char encrypCharMap[] = "a4so2w5731nve8uprdgfzkij9tc0xl6myhqb";

void checkPath(std::string& path)
{
	transform(path.begin(), path.end(), path.begin(), [](char c)->char{
		if (c == '\\')
			return '/';
		return c;
	});
}

ResourceEncrypt::ResourceEncrypt() :m_isEncrypt(true)
{
	m_encrypMapSize = strlen(encrypCharMap);
	for (int i = 0; i < READ_SIZE; i++)
	{
		m_decrypMap[i] = -1;
	}

	for (int i = 0; i < m_encrypMapSize; i++)
	{
		char value = encrypCharMap[i];
		m_decrypMap[value] = i;
	}

	setEncryptInfo(DEFAULT_ENCRYPT_KEY, DEFAULT_ENCRYPT_FLAG);
}

ResourceEncrypt* ResourceEncrypt::getInstance()
{
	static ResourceEncrypt instance;
	return &instance;
}

ResourceEncrypt::~ResourceEncrypt()
{

}

void ResourceEncrypt::setIsEncrypt(bool isEncrypt)
{
	m_isEncrypt = isEncrypt;
}

bool isNeedCharEncrypt(char source)
{
	return true;
}

void ResourceEncrypt::setEncryptInfo(std::string key, std::string flag)
{
	this->m_encrypt_key = key;
	this->m_encrypt_flag = flag;
	this->m_keySize = key.length();
	this->m_flagSize = flag.length();

	m_encrypMapSize = strlen(encrypCharMap);
	for (int i = 0; i < 255; i++)
	{
		m_decrypMap[i] = -1;
	}

	for (int i = 0; i < m_encrypMapSize; i++)
	{
		char value = encrypCharMap[i];
		m_decrypMap[value] = i;
	}
}


bool ResourceEncrypt::checkIsEncrypt(const char* buffer,size_t size)
{
	if (size < m_flagSize) {
		return false;
	}
	
	return strncmp(buffer, m_encrypt_flag.c_str(), m_flagSize) == 0;
}

void ResourceEncrypt::encryptFile(const char* filepath)
{
	if (!m_isEncrypt)
	{
		return;
	}
	if (this->m_encrypt_key.empty() || this->m_encrypt_flag.empty())
	{
		return;
	}
	std::string tempFilePath = filepath;
	tempFilePath += ".temp";


	FILE* rfp = fopen(filepath, "rb");
	FILE* wfp = fopen(tempFilePath.c_str(), "wb");
	bool isEncrypt = false;
	do{
		if (!rfp || !wfp){
			break;
		}

		size_t  buffersize = m_flagSize > READ_SIZE ? m_flagSize : READ_SIZE;
		char*  buffer = (char*)malloc(buffersize* sizeof(char));

		size_t  count = 0;
		while (feof(rfp) == 0)
		{
			if (count >= m_flagSize) break;
		    size_t read_size = fread(buffer + count, sizeof(char), m_flagSize - count, rfp);
			count += read_size;
		}
		
		if (checkIsEncrypt(buffer, count)){
			if (buffer)
			{
				free(buffer);
				buffer = NULL;
			}
			break;
		}
		isEncrypt = true;
		fseek(rfp, 0, SEEK_SET);
		fwrite(m_encrypt_flag.c_str(), sizeof(char), m_flagSize, wfp);
		size_t keyIndex = 0;
		while (feof(rfp)== 0)
		{
			size_t read_size = fread(buffer, sizeof(char), buffersize, rfp);
			encrypt(buffer, read_size, buffer,read_size,keyIndex);
			fwrite(buffer, sizeof(char), read_size, wfp);
		}
		if (buffer)
		{
			free(buffer);
			buffer = NULL;
		}
		
	} while (0);

	
	if (rfp){
		fclose(rfp);
		rfp = NULL;
	}
	if (wfp)
	{
		fclose(wfp);
		wfp = NULL;
	}

	if (isEncrypt){
		remove(filepath);
		rename(tempFilePath.c_str(), filepath);
	}
	else{
		remove(tempFilePath.c_str());
	}
}

void ResourceEncrypt::decryptFile(const char* filepath)
{

	if (this->m_encrypt_key.empty() || this->m_encrypt_flag.empty())
	{
		return;
	}

	std::string tempFilePath = filepath;
	tempFilePath += ".temp";


	FILE* rfp = fopen(filepath, "rb");
	FILE* wfp = fopen(tempFilePath.c_str(), "wb");
	bool isEncrypt = true;
	do{
		if (!rfp || !wfp){
			break;
		}

		size_t  buffersize = m_flagSize > READ_SIZE ? m_flagSize : READ_SIZE;
		char*  buffer = (char*)malloc(buffersize* sizeof(char));

		size_t  count = 0;
		while (feof(rfp) == 0)
		{
			if (count >= m_flagSize) break;
			size_t read_size = fread(buffer + count, sizeof(char), m_flagSize - count, rfp);
			count += read_size;
		}

		if (!checkIsEncrypt(buffer, count)){
			isEncrypt = false;
			fseek(rfp, 0, SEEK_SET);
		}
		
		size_t keyIndex = 0;
		while (feof(rfp) == 0)
		{
			size_t read_size = fread(buffer, sizeof(char), buffersize, rfp);
			if (isEncrypt){
				decrypt(buffer, read_size, buffer, read_size, keyIndex);
			}
			fwrite(buffer, sizeof(char), read_size, wfp);
		}
		if (buffer)
		{
			free(buffer);
			buffer = NULL;
		}

	} while (0);


	if (rfp){
		fclose(rfp);
		rfp = NULL;
	}
	if (wfp)
	{
		fclose(wfp);
		wfp = NULL;
	}

	if (isEncrypt){
		remove(filepath);
		rename(tempFilePath.c_str(), filepath);
	}
	else{
		remove(tempFilePath.c_str());
	}
}


void ResourceEncrypt::encrypt(const char* source, size_t  source_size, char* outBuff, size_t & out_size, size_t & keyIndex)
{
	if (!m_isEncrypt)
	{
		outBuff = const_cast<char*>(source);
		out_size = source_size;
		return;
	}
	if (!source || !outBuff){
		return;
	}
	size_t count = 0;
	for (size_t i = 0; i < source_size; i++){
		char s_ch = source[i];
		outBuff[count++] = s_ch ^ (m_encrypt_key[keyIndex++%m_keySize]);
	}
	out_size = count;
}

void ResourceEncrypt::decrypt(const char* source, size_t source_size, char* outBuff, size_t& out_size,size_t& keyIndex)
{
	if (!source || !outBuff){
		return ;
	}

	size_t count = 0;
	for (size_t i = 0; i < source_size; i++){
		char s_ch = source[i];
		outBuff[count++] = s_ch ^ (m_encrypt_key[keyIndex++%m_keySize]);
	}
	out_size = count;
}


void ResourceEncrypt::encryptName(std::string source, std::string& out)
{
	if (!m_isEncrypt)
	{
		out = source;
		return;
	}
	out = source;
	size_t  source_size = source.size();
	for (size_t i = 0; i <source_size; i++)
	{
		out[i] = encryptChar(source[i], source_size%m_encrypMapSize);
	}
}

void ResourceEncrypt::decryptName(std::string source, std::string& out)
{
	out = source;
	size_t  source_size = source.size();
	for (size_t i = 0; i < source_size;i++)
	{
		out[i] = decryptChar(source[i], source_size%m_encrypMapSize);
	}
}

char ResourceEncrypt::encryptChar(char source,int offset)
{
	if (source == NULL){
		return NULL;
	}

	if (!m_isEncrypt)
	{
		return source;
	}

	bool isEncrypt = true;
	char en = 0;

	if (source >= '0' && source <= '9') // 0 - 9
	{
		en = source - '0';
	}
	else if (source >= 'a' && source <= 'z') // 10 - 35
	{
		en = source - 'a' + 10;
	}
	else if (source >= 'A' && source <= 'Z') // 10 - 35
	{
		en = source - 'A' + 10;
	}
	else
	{
		isEncrypt = false;
	}

	if (isEncrypt)
	{
		en = (en + (offset%m_encrypMapSize)) % m_encrypMapSize;
		return encrypCharMap[en];
	}

	return source;
}

char ResourceEncrypt::decryptChar(char source, int offset)
{
	if (source == NULL){
		return NULL;
	}
	char index = m_decrypMap[source];
	if (index < 0) return source;

	index = (index + m_encrypMapSize - (offset%m_encrypMapSize)) % m_encrypMapSize;

	if (index >= 0 && index < 10)
	{
		return index + '0';
	}
	else if (index >= 10 && index < 36)
	{
		return index - 10 + 'a';
	}

	return source;
}

void ResourceEncrypt::checkDecrypt(char* source, size_t source_size, char* outbuffer, size_t & outsize)
{
	if (source == NULL || outbuffer == NULL){
		return;
	}

	if (checkIsEncrypt(source, source_size)){
			size_t  keyIndex = 0;
			outsize = source_size - m_flagSize;
			decrypt(source + m_flagSize, outsize, outbuffer, outsize, keyIndex);
	}
	else{
		outbuffer = source;
		outsize = source_size;
	}
}

//path,outpath格式：A/B/C(/)
void ResourceEncrypt::encryptPath(std::string path, std::string& outpath)
{
	outpath = "";
	checkPath(path);
	std::string temppath = path;
	size_t  index = 0;
	size_t  pathsize = path.length();
	for (size_t i = 0; i < pathsize;i++)
	{
		if (temppath[i]== '/')
		{
			size_t length = i - index;
			std::string name = temppath.substr(index, length);
			std::string outname;
			encryptName(name, outname);
			outpath += outname;
			outpath += "/";
			index = i + 1;
		}
	}

	if (index<pathsize)
	{
		std::string name = temppath.substr(index);
		std::string outname;
		encryptName(name, outname);
		outpath += outname;
	}
}

//path,outpath格式：A/B/C(/)
void ResourceEncrypt::decryptPath(std::string path, std::string& outpath)
{
	outpath = "";
	checkPath(path);
	std::string temppath = path;

	size_t  pathsize = path.length();
	size_t  index = 0;
	for (size_t i = 0; i < pathsize; i++)
	{
		if (temppath[i] == '/')
		{
			size_t  length = i - index;
			std::string name = temppath.substr(index, length);
			std::string outname;
			decryptName(name, outname);
			outpath += outname;
			outpath += "/";
			index = i + 1;
		}
	}
	if (index < pathsize)
	{
		std::string name = temppath.substr(index);
		std::string outname; 
		decryptName(name, outname);
		outpath += outname;
	}
}