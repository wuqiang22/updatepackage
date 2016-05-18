#ifndef  _ENCRYPT_H_
#define  _ENCRYPT_H_

#include "wx/string.h"

class Encrypt{
public:
	static Encrypt* getInstance();
	bool encryptDir(wxString path, bool isRoot);
	bool decryptDir(wxString path, bool isRoot);
};
#endif 