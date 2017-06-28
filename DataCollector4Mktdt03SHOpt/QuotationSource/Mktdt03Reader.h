#ifndef __CTP_BASIC_CACHE_H__
#define __CTP_BASIC_CACHE_H__


#pragma warning(disable:4786)
#include <map>
#include <string>
#include <fstream>
#include <stdexcept>
#include "../Configuration.h"
#include "../Infrastructure/Lock.h"
#include "../Infrastructure/Thread.h"
#include "../Infrastructure/DateTime.h"


/**
 * @class					CReadMktdtFile
 * @brief					�Ϻ�Level1��Mktdt00.txt�ļ���ȡ��
 * @author					barry
 */
class CReadMktdtFile
{
public:
	CReadMktdtFile();

public:
	int				LoadMktdt();
};




#endif






