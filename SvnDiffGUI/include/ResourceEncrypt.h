#ifndef  _RESOURCE_ENCRYPT_H
#define _RESOURCE_ENCRYPT_H

#include <stdlib.h>
#include <string>
#include <utility>

#define  READ_SIZE  512

class ResourceEncrypt{
public:

	ResourceEncrypt();
	static ResourceEncrypt* getInstance();

	static std::string DEFAULT_ENCRYPT_KEY;
	static std::string DEFAULT_ENCRYPT_FLAG;
	static std::string DEFAULT_NAME_KEY;

	void setEncryptInfo(std::string key, std::string flag);  //设置加密信息
	void setIsEncrypt(bool isEncrypt);  //设置是否加密

	void encryptFile(const char*  filepath);  //加密某个文件
	void decryptFile(const char* filepath);// 解密某个文件

	void encryptName(std::string source,std::string& out);  //加密名字
	void decryptName(std::string source,std::string& out); // 解密名字
	char encryptChar(char source,int offset); //加密单个字符
	char decryptChar(char source, int offset);// 解密单个字符

	void checkDecrypt(char* source, size_t source_size, char* outbuffer, size_t & outsize);  //解密
	bool checkIsEncrypt(const char* buffer, size_t size);  //检测是否需要解密

	void encryptPath(std::string path, std::string& outpath);   //加密路径
	void decryptPath(std::string path, std::string& outpath);  //解密路径

	~ResourceEncrypt();
public:
	std::string  m_encrypt_key;
	std::string  m_encrypt_flag;
	size_t   m_keySize;
	size_t   m_flagSize;

	char		m_decrypMap[READ_SIZE];
	size_t  m_encrypMapSize;

	void encrypt( const char* source, size_t  source_size, char* outBuff,size_t & out_size,size_t&  keyIndex);
	void decrypt(const char* source, size_t source_size, char* outBuff,size_t& out_size,size_t & keyIndex);

	bool  m_isEncrypt;
};
#endif