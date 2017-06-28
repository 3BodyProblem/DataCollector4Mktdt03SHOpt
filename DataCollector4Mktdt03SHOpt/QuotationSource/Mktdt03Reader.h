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
 * @brief					上海Level1的Mktdt00.txt文件读取类
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






