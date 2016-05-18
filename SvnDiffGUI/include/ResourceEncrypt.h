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

	void setEncryptInfo(std::string key, std::string flag);  //���ü�����Ϣ
	void setIsEncrypt(bool isEncrypt);  //�����Ƿ����

	void encryptFile(const char*  filepath);  //����ĳ���ļ�
	void decryptFile(const char* filepath);// ����ĳ���ļ�

	void encryptName(std::string source,std::string& out);  //��������
	void decryptName(std::string source,std::string& out); // ��������
	char encryptChar(char source,int offset); //���ܵ����ַ�
	char decryptChar(char source, int offset);// ���ܵ����ַ�

	void checkDecrypt(char* source, size_t source_size, char* outbuffer, size_t & outsize);  //����
	bool checkIsEncrypt(const char* buffer, size_t size);  //����Ƿ���Ҫ����

	void encryptPath(std::string path, std::string& outpath);   //����·��
	void decryptPath(std::string path, std::string& outpath);  //����·��

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