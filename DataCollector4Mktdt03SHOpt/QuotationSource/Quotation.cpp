#include <math.h>
#include <algorithm>
#include "Quotation.h"
#include "../Infrastructure/IniFile.h"
#include "../DataCollector4Mktdt03SHOpt.h"


WorkStatus::WorkStatus()
: m_eWorkStatus( ET_SS_UNACTIVE )
{
}

WorkStatus::WorkStatus( const WorkStatus& refStatus )
{
	CriticalLock	section( m_oLock );

	m_eWorkStatus = refStatus.m_eWorkStatus;
}

WorkStatus::operator enum E_SS_Status()
{
	CriticalLock	section( m_oLock );

	return m_eWorkStatus;
}

std::string& WorkStatus::CastStatusStr( enum E_SS_Status eStatus )
{
	static std::string	sUnactive = "未激活";
	static std::string	sDisconnected = "断开状态";
	static std::string	sConnected = "连通状态";
	static std::string	sLogin = "登录成功";
	static std::string	sInitialized = "初始化中";
	static std::string	sWorking = "推送行情中";
	static std::string	sUnknow = "不可识别状态";

	switch( eStatus )
	{
	case ET_SS_UNACTIVE:
		return sUnactive;
	case ET_SS_DISCONNECTED:
		return sDisconnected;
	case ET_SS_CONNECTED:
		return sConnected;
	case ET_SS_LOGIN:
		return sLogin;
	case ET_SS_INITIALIZING:
		return sInitialized;
	case ET_SS_WORKING:
		return sWorking;
	default:
		return sUnknow;
	}
}

WorkStatus&	WorkStatus::operator= ( enum E_SS_Status eWorkStatus )
{
	CriticalLock	section( m_oLock );

	if( m_eWorkStatus != eWorkStatus )
	{
		QuoCollector::GetCollector()->OnLog( TLV_INFO, "WorkStatus::operator=() : Exchange Session Status [%s]->[%s]"
											, CastStatusStr(m_eWorkStatus).c_str(), CastStatusStr(eWorkStatus).c_str() );
				
		m_eWorkStatus = eWorkStatus;
	}

	return *this;
}


///< ----------------------------------------------------------------


Quotation::Quotation()
: SimpleTask( "Thread::Scanner" )
{
}

Quotation::~Quotation()
{
	Release();
}

WorkStatus& Quotation::GetWorkStatus()
{
	return m_oWorkStatus;
}

int Quotation::Initialize()
{
	if( GetWorkStatus() == ET_SS_UNACTIVE )
	{
		int					nErrCode = -1;

		QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::Initialize() : ............ Session Activating............" );

		Release();

		m_oReffFile.LoadReff( false );

		if( ( nErrCode = SimpleTask::Activate() ) < 0 )
		{
			QuoCollector::GetCollector()->OnLog( TLV_WARN, "Quotation::Initialize() : failed 2 activate quotation file scanner, errorcode=%d", nErrCode );
			return nErrCode;
		}

		m_oWorkStatus = ET_SS_DISCONNECTED;				///< 更新Quotation会话的状态
		QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::Initialize() : ............ Quotation Activated!.............." );
	}

	return 0;
}

int Quotation::Release()
{
	QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::Release() : ............ Destroying .............." );

	m_oWorkStatus = ET_SS_UNACTIVE;	///< 更新Quotation会话的状态

	QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::Release() : ............ Destroyed! .............." );

	return 0;
}

int Quotation::ScanFile()
{
	m_oMktdt03File.LoadMktdt();

	return 0;
}

int Quotation::Execute()
{
	QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::Execute() : ............ Enter .............." );

	while( true == SimpleTask::IsAlive() )
	{
		SimpleTask::Sleep( 1000*1 );

		ScanFile();
	}

	QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::Execute() : ............ Leave .............." );

	return 0;
}

void Quotation::SendLoginRequest()
{
	QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::SendLoginRequest() : sending hq login message" );
/*
	CThostFtdcReqUserLoginField	reqUserLogin = { 0 };

	strcpy( reqUserLogin.UserProductInfo,"上海乾隆高科技有限公司" );
	strcpy( reqUserLogin.TradingDay, m_pCTPApi->GetTradingDay() );
	memcpy( reqUserLogin.Password, Configuration::GetConfig().GetHQConfList().m_sPswd.c_str(), Configuration::GetConfig().GetHQConfList().m_sPswd.length() );
	memcpy( reqUserLogin.UserID, Configuration::GetConfig().GetHQConfList().m_sUID.c_str(), Configuration::GetConfig().GetHQConfList().m_sUID.length() );
	memcpy( reqUserLogin.BrokerID, Configuration::GetConfig().GetHQConfList().m_sParticipant.c_str(), Configuration::GetConfig().GetHQConfList().m_sParticipant.length() );

	if( 0 == m_pCTPApi->ReqUserLogin( &reqUserLogin, 0 ) )
	{
		QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::SendLoginRequest() : login message sended!" );
	}
	else
	{
		QuoCollector::GetCollector()->OnLog( TLV_ERROR, "Quotation::SendLoginRequest() : failed 2 send login message" );
	}
*/
}






