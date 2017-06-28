#ifndef __MEngine_CReadFjyFileH__20140918
#define __MEngine_CReadFjyFileH__20140918


#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include "../Configuration.h"
#include "../Infrastructure/Lock.h"
#include "../Infrastructure/Thread.h"
#include "../Infrastructure/DateTime.h"
#include "../Mktdt03_SHOpt_QuoProtocal.h"


bool UTF8ToMB( std::vector<char>& pmb, const char* pu8, int utf8Len );


#define  MAXLINECHAR	512
class MTxtFile
{
public:
	MTxtFile();
	virtual ~MTxtFile();

public:
	int			OpenRead(const char * file);
	void		Close();

public:
	bool		ReadNextLine();
	int			GetToken(int fieldno,char * pszvalue, int nvalen);

	int			GetTokenInteger(int fieldno);
	long		GetTokenLong(int fieldno);
	__int64		GetTokenInteger64(int fieldno);
	double		GetTokenDouble(int fieldno);

protected:
	std::string		m_strFile;
	char			m_szLine[MAXLINECHAR];
	FILE *			m_pf;
	int				m_lastfieldno;	//缓存最近访问的fieldno
	char *			m_plastchar;	//缓存最近访问fieldno之后的指针位置
};


//--------------------------------------------------------------------------------------------------------------------------

#define		NAME_SAVEINDEX		"保留指数"
//#define	NAME_OPTION			"上证期权"
#define		NAME_OPTION			"个股期权"
#define		NAME_ETF			"ETF期权"

class MarketInfo
{
private:
	MarketInfo();

public:
	static MarketInfo&		GetObj();

	void					BuildImage();

	double					GetTypeRate( int Type, bool bPow = false );

protected:
	std::map<int,tagSHOptKindDetail_LF110>		m_mapMkKind;
};

#define DIGIT_POS 0.500001

class ReffReader
{
public:
	ReffReader();
	int						LoadReff( bool bUTF8 );

public:
	CriticalObject			m_oLock;				///< 临界区对象

protected:
	std::string				m_sFilePath;			///< fjy.txt路径
	std::ifstream			m_fInput;				///< 读文件对象
};


#endif




