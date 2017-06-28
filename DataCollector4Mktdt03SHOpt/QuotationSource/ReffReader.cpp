#include <math.h>
#include "ReffReader.h"
#include "../DataCollector4Mktdt03SHOpt.h"
using namespace std;


// UTF8编码转为多字节编码  
bool UTF8ToMB(vector<char>& pmb, const char* pu8, int utf8Len)  
{  
    // convert an UTF8 string to widechar   
    int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);  
	
    WCHAR* lpszW = NULL;  
    try  
    {  
        lpszW = new WCHAR[nLen];  
    }  
    catch(bad_alloc &)  
    {  
        return false;  
    }  
	
    int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, lpszW, nLen);  
	
    if(nRtn != nLen)  
    {  
        delete[] lpszW;  
        return false;  
    }  
	
    // convert an widechar string to Multibyte   
    int MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
    if (MBLen <=0)  
    {  
        return false;  
    }  
    pmb.resize(MBLen);  
    nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, &*pmb.begin(), MBLen, NULL, NULL);  
    delete[] lpszW;  
	
    if(nRtn != MBLen)  
    {  
        pmb.clear();  
        return false;  
    }  
    return true;  
}

void inner_trimright(char * buf, int nbuflen)
{
	int i=0;
	for (i=nbuflen-1; i>0; --i)
	{
		if (buf[i] == '\0')
			continue;
		
		if (buf[i] == ' ')
			buf[i] = '\0';
		else
			break;
	}
	
}


MTxtFile::MTxtFile()
{
	m_pf =0;
	m_lastfieldno =-1;
	m_plastchar =0;
	memset(m_szLine, 0, 512);
}

MTxtFile::~MTxtFile()
{
}

int	MTxtFile::OpenRead(const char * file)
{
	if (m_pf)
	{
		fclose(m_pf);
		m_pf =0;
	}
	m_lastfieldno =-1;
	m_plastchar =0;
	memset(m_szLine, 0, 512);
	m_strFile = file;
	m_pf = fopen(file,"rb");
	return m_pf>0;
}

void	MTxtFile::Close()
{
	if (m_pf)
	{
		fclose(m_pf);
		m_pf =0;
	}
	m_lastfieldno = -1;
	m_plastchar =0;
	memset(m_szLine, 0, 512);
}

bool		MTxtFile::ReadNextLine()
{
	if (m_pf)
	{
		if (fgets(m_szLine, 512, m_pf) != NULL)
		{
			m_plastchar = m_szLine;
			m_lastfieldno = -1;
			return true;
		}
	}
	return false;
}

int			MTxtFile::GetToken(int fieldno,char * pszvalue, int nvalen)
{
	if (0 == m_plastchar)
	{
		return (-1);
	}

	if (nvalen>0)
		memset(pszvalue, 0, nvalen);

	if (m_lastfieldno>= fieldno)
	{
		m_lastfieldno = -1;
		m_plastchar = m_szLine;
	}
	
	int idx =m_lastfieldno+1;
	bool bfind =false;
	char * pprechar = m_plastchar;
	char * ppostchar =m_plastchar;
	do 
	{
		bfind =false;

		while (*ppostchar && ppostchar <m_szLine + MAXLINECHAR -1)
		{
			if (*ppostchar == '|' || *ppostchar == '\n')
			{
				ppostchar++;
				bfind=true;
				break;
			}
			ppostchar++;
		}
		
		if (!bfind)
		{
			return 0;
		}
		if (fieldno == idx)
		{
			int nlen = ppostchar-pprechar-1;
			if (nlen > nvalen)
				nlen = nvalen;
			
			if (nlen >0)
			{
				strncpy(pszvalue, pprechar, nlen);
			}
		
			m_plastchar = ppostchar;
			m_lastfieldno = fieldno;
			return nlen;
		}

		idx++;
		pprechar = ppostchar;
	} while (1);

	return 0;
}

int			MTxtFile::GetTokenInteger(int fieldno)
{
	char sztoken[50]={0};
	int iret = GetToken(fieldno, sztoken, 50);
	if (iret >0)
	{
		return atoi(sztoken);
	}
	return 0;
}

long		MTxtFile::GetTokenLong(int fieldno)
{
	char sztoken[50]={0};
	int iret = GetToken(fieldno, sztoken, 50);
	if (iret >0)
	{
		return atol(sztoken);
	}
	return 0;
}

__int64		MTxtFile::GetTokenInteger64(int fieldno)
{
	char sztoken[50]={0};
	int iret = GetToken(fieldno, sztoken, 50);
	if (iret >0)
	{
		return _atoi64(sztoken);
	}
	return 0;
}

double		MTxtFile::GetTokenDouble(int fieldno)
{
	char sztoken[50]={0};
	int iret = GetToken(fieldno, sztoken, 50);
	if (iret >0)
	{
		return atof(sztoken);
	}
	return 0;
}


//--------------------------------------------------------------------------------------------------------------------------


MarketInfo::MarketInfo()
{
}

MarketInfo& MarketInfo::GetObj()
{
	static	MarketInfo	obj;

	return obj;
}

double MarketInfo::GetTypeRate( int Type, bool bPow )
{
	tagSHOptKindDetail_LF110	tagKind = m_mapMkKind[Type];

	if( false == bPow )
		return tagKind.PriceRate;
	else
		return pow( 10, (double)tagKind.PriceRate );

	return 1;
}

void MarketInfo::BuildImage()
{
	tagSHOptMarketInfo_LF109	tagMkHead = { 0 };
	tagSHOptKindDetail_LF110	tagKindInfo = { 0 };

	memset( &tagKindInfo, 0, sizeof( tagKindInfo ) );
	strcpy( tagKindInfo.KindName, NAME_SAVEINDEX );
	tagKindInfo.PriceRate = 3;
	tagKindInfo.LotSize = 1;
	m_mapMkKind[m_mapMkKind.size()] = tagKindInfo;
	QuoCollector::GetCollector()->OnImage( 110, (char*)&tagKindInfo, sizeof(tagKindInfo), false );

	memset( &tagKindInfo, 0, sizeof( tagKindInfo ) );
	strcpy( tagKindInfo.KindName, NAME_OPTION );
	tagKindInfo.PriceRate = 3;
	tagKindInfo.LotSize = 1000;
	m_mapMkKind[m_mapMkKind.size()] = tagKindInfo;
	QuoCollector::GetCollector()->OnImage( 110, (char*)&tagKindInfo, sizeof(tagKindInfo), false );

	memset( &tagKindInfo, 0, sizeof( tagKindInfo ) );
	strcpy( tagKindInfo.KindName, NAME_ETF );
	tagKindInfo.PriceRate = 4;
	tagKindInfo.LotSize = 1000;
	m_mapMkKind[m_mapMkKind.size()] = tagKindInfo;
	QuoCollector::GetCollector()->OnImage( 110, (char*)&tagKindInfo, sizeof(tagKindInfo), false );

	tagMkHead.KindCount = m_mapMkKind.size();
	tagMkHead.PeriodsCount = 2;
	tagMkHead.MarketPeriods[0][0] = 9*60+30;
	tagMkHead.MarketPeriods[0][1] = 11*60+30;
	tagMkHead.MarketPeriods[1][0] = 13*60;
	tagMkHead.MarketPeriods[1][1] = 15*60;
}


ReffReader::ReffReader()
{
}

int ReffReader::LoadReff( bool bUTF8 )
{
	MTxtFile			mfile;
	int					idxetf = 0;
	int					idxstock = 0;
	char				pszTemp[50] = { 0 };
	char				pszFilePath[128] = { 0 };
	unsigned int		nDate = DateTime::Now().DateToLong();
	const std::string&	sReffPath = Configuration::GetConfig().GetReffFilePath();
	CriticalLock		section( m_oLock );

	::sprintf( pszFilePath, "%s%04d.txt", sReffPath.c_str(), nDate%10000 );
	QuoCollector::GetCollector()->OnLog( TLV_INFO, "ReffReader::LoadReff() : loading reff03.txt, path = %s", pszFilePath );
	if( mfile.OpenRead(pszFilePath) <= 0 )
	{
		QuoCollector::GetCollector()->OnLog( TLV_INFO, "ReffReader::LoadReff() : failed 2 load reff03.txt, path = %s", pszFilePath );
		return -1;
	}

	MarketInfo::GetObj().BuildImage();

	while( mfile.ReadNextLine() )
	{
		tagSHOptReferenceData_LF108		tagNameTable = { 0 };
		tagSHOptSnapData_LF112			tagDataLF = { 0 };
		tagSHOptSnapData_HF113			tagDataHF = { 0 };
		tagSHOptSnapBuySell_HF114		tagDataBS = { 0 };

		mfile.GetToken( 0, pszTemp, 50 );
		if( strnicmp( pszTemp, "R0301", 5 ) != 0 )
		{
			break;
		}

		tagNameTable.Kind =1;
		mfile.GetToken( 1, tagNameTable.Code, 8 );
		mfile.GetToken( 2, tagNameTable.ContractID, 19 );
		if( true == bUTF8 )
		{
			char pszUTF8[50] = { 0 };
			mfile.GetToken( 3, pszUTF8, 50 );
			vector<char> aa;
			UTF8ToMB(aa, pszUTF8, strlen(pszUTF8));
			for (int i=0; i<aa.size() && i<20 ; i++)
			{
				tagNameTable.Name[i] = aa[i];
			}
		}
		else
		{
			mfile.GetToken( 3, tagNameTable.Name, 20 );
		}
		inner_trimright(tagNameTable.ContractID, 19);		//裁剪掉右边的空格
		inner_trimright(tagNameTable.Name, 20);				//裁剪掉右边的空格

		char		UnderlyingType[3];	//标的证券类型(EBS -ETF, ASH -A股)
		mfile.GetToken( 4, UnderlyingType, 6 );
		if ( memcmp( UnderlyingType, "EBS", 3) ==0 )
		{
			tagNameTable.Kind = 2;
			idxetf++;
		}
		else
		{
			idxstock++;
		}

		int	nRate = MarketInfo::GetObj().GetTypeRate( tagNameTable.Kind, true );

		char		CallOrPut;			//认购认沽(C-   P-)
		mfile.GetToken( 8, &CallOrPut, 1 );
		if( 'C' == CallOrPut )
		{
			tagNameTable.DerivativeType = 2;
		}
		else if( 'P' == CallOrPut )
		{
			tagNameTable.DerivativeType = 3;
		}

		tagNameTable.ContractUnit = mfile.GetTokenInteger(9);
		tagNameTable.XqPrice = mfile.GetTokenDouble(10)*nRate +DIGIT_POS;
		tagNameTable.StartDate = mfile.GetTokenInteger(11);
		tagNameTable.EndDate = mfile.GetTokenInteger(12);
		tagNameTable.XqDate = mfile.GetTokenInteger(13);
		tagNameTable.DeliveryDate = mfile.GetTokenInteger(14);
		tagNameTable.ExpireDate = mfile.GetTokenInteger(15);
		mfile.GetToken(16,&tagNameTable.UpdateVersion,1);

		::memcpy( tagDataLF.Code, tagNameTable.Code, sizeof(tagDataLF.Code) );
		tagDataLF.PreOpenInterest = mfile.GetTokenInteger(17);
		tagDataLF.PreClose = mfile.GetTokenDouble(18)*nRate +DIGIT_POS;
		tagDataLF.PreSettlePrice = mfile.GetTokenDouble(19)*nRate+DIGIT_POS;
		tagDataLF.UpperPrice = mfile.GetTokenDouble(22)*nRate+DIGIT_POS;
		tagDataLF.LowerPrice = mfile.GetTokenDouble(23)*nRate+DIGIT_POS;
		tagNameTable.LotSize = mfile.GetTokenInteger(27);
		mfile.GetToken(33, tagNameTable.StatusFlag, 8);

		QuoCollector::GetCollector()->OnImage( 108, (char*)&tagNameTable, sizeof(tagNameTable), false );
	}

	mfile.Close();

//	Global_Process.WriteInfo(0,"MDBFIO","构建快照结构完毕,装载cff03文件完毕");

	return 0;
}

