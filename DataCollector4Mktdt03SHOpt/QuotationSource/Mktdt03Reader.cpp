#include "ReffReader.h"
#include "Mktdt03Reader.h"
#include "../DataCollector4Mktdt03SHOpt.h"


CReadMktdtFile::CReadMktdtFile()
{
}

int CReadMktdtFile::LoadMktdt()
{
	MTxtFile	mfile;
	int			nRecordNum =0;					//总记录个数
	char		pszTemp[50] = { 0 };
	char		pszDateTime[50] = { 0 };		//全市场日期和时间
	char		pszMarketStatus[9] = { 0 };		//全市场状态

	if( mfile.OpenRead( Configuration::GetConfig().GetMktdt03FilePath().c_str() ) > 0 )
	{
		//读取第一行
		mfile.ReadNextLine();
		mfile.GetToken(0, pszTemp, 50);
		if (strnicmp(pszTemp,"HEADER", 6) !=0)
		{
			mfile.Close();
			return -1;
		}
	
		nRecordNum = mfile.GetTokenInteger(3);
		mfile.GetToken(6, pszDateTime, 49);
		mfile.GetToken(8, pszMarketStatus,8);
		int nreaddate=0;
//		int nreadtime= inner_datetime_crack(pszDateTime,strlen(pszDateTime), nreaddate);

		for( int idx = 0; mfile.ReadNextLine() && idx < nRecordNum; idx++ )
		{
			tagSHOptSnapData_LF112			tagDataLF = { 0 };
			tagSHOptSnapData_HF113			tagDataHF = { 0 };
			tagSHOptSnapBuySell_HF114		tagDataBS = { 0 };

			mfile.GetToken(0, pszTemp, 49);
			if (strnicmp(pszTemp,"M0301", 5) !=0)
			{
				continue;
			}

			char szsecuritycode[10]={0};
			mfile.GetToken(1, szsecuritycode, 8);
/*			int ii = Global_DataIO.GetSerialByCode(szsecuritycode);
			if (ii <0)
			{
				continue;
			}*/
/*
			int							nRate = 1000;
			tagCcComm_ShOptNameTable	tagName;
			if( false == Global_DataIO.GetNameTable().GetOneBySerial( &tagName, ii ) )
				::printf( "xxx = code = %s \n", szsecuritycode );
			else
				nRate = Global_DataIO.GetMarket().GetTypeRate( tagName.Type, true );

			memset(&tagStock, 0, sizeof(tagShOptMem_StockData));
			memcpy(tagStock.SecurityID, szsecuritycode,8);
			tagStock.Position = mfile.GetTokenInteger64(2);
			tagStock.Volume = mfile.GetTokenInteger64(3);
			tagStock.Amount = mfile.GetTokenDouble(4)*100 + DIGIT_POS;
			tagStock.PreSettlePx = mfile.GetTokenDouble(5)*nRate + DIGIT_POS;
			tagStock.OpenPx = mfile.GetTokenDouble(6)*nRate+ DIGIT_POS;
			tagStock.AuctionPrice= mfile.GetTokenDouble(7)*nRate+ DIGIT_POS;
			tagStock.AuctionQty = mfile.GetTokenInteger64(8);
			tagStock.HighPx = mfile.GetTokenDouble(9)*nRate+ DIGIT_POS;
			tagStock.LowPx = mfile.GetTokenDouble(10)*nRate+ DIGIT_POS;
			tagStock.Now = mfile.GetTokenDouble(11)*nRate+ DIGIT_POS;

			tagStock.Buy[0].Price = mfile.GetTokenDouble(12)*nRate+ DIGIT_POS;
			tagStock.Buy[0].Volume = mfile.GetTokenInteger64(13);
			tagStock.Sell[0].Price = mfile.GetTokenDouble(14)*nRate+ DIGIT_POS;
			tagStock.Sell[0].Volume = mfile.GetTokenInteger64(15);
			
		//	DOUBLE a=0.1611*10000 +0.500001;
		//	DOUBLE a=0.1619*10000;
		//	 long b =a;
		//	tagStock.Buy[1].Price = a;
			tagStock.Buy[1].Price = mfile.GetTokenDouble(16)*nRate+ DIGIT_POS;
			tagStock.Buy[1].Volume = mfile.GetTokenInteger64(17);
			tagStock.Sell[1].Price = mfile.GetTokenDouble(18)*nRate+ DIGIT_POS;
			tagStock.Sell[1].Volume = mfile.GetTokenInteger64(19);

			tagStock.Buy[2].Price = mfile.GetTokenDouble(20)*nRate+ DIGIT_POS;
			tagStock.Buy[2].Volume = mfile.GetTokenInteger64(21);
			tagStock.Sell[2].Price = mfile.GetTokenDouble(22)*nRate+ DIGIT_POS;
			tagStock.Sell[2].Volume = mfile.GetTokenInteger64(23);

			tagStock.Buy[3].Price = mfile.GetTokenDouble(24)*nRate+ DIGIT_POS;
			tagStock.Buy[3].Volume = mfile.GetTokenInteger64(25);
			tagStock.Sell[3].Price = mfile.GetTokenDouble(26)*nRate+ DIGIT_POS;
			tagStock.Sell[3].Volume = mfile.GetTokenInteger64(27);

			tagStock.Buy[4].Price = mfile.GetTokenDouble(28)*nRate+ DIGIT_POS;
			tagStock.Buy[4].Volume = mfile.GetTokenInteger64(29);
			tagStock.Sell[4].Price = mfile.GetTokenDouble(30)*nRate+ DIGIT_POS;
			tagStock.Sell[4].Volume = mfile.GetTokenInteger64(31);

			tagStock.SettlePrice = mfile.GetTokenDouble(32)*nRate+ DIGIT_POS;  
			tagStock.SettlePrice = 0;  //改用clpr03文件中结算价
			mfile.GetToken(33, tagStock.TradingPhaseCode, 4);

			mfile.GetToken(34, sztemp, 49);
			tagStock.DataTimeStamp = inner_time_crack(sztemp, strlen(sztemp) );
*/
			QuoCollector::GetCollector()->OnData( 112, (char*)&tagDataLF, sizeof(tagDataLF), false );
			QuoCollector::GetCollector()->OnData( 113, (char*)&tagDataHF, sizeof(tagDataHF), false );
			QuoCollector::GetCollector()->OnData( 114, (char*)&tagDataBS, sizeof(tagDataBS), false );
		}
		
		mfile.Close();
	}

	return 0;
}


