#ifndef __CTP_QUOTATION_H__
#define __CTP_QUOTATION_H__


#pragma warning(disable:4786)
#include <set>
#include <map>
#include <string>
#include <stdexcept>
#include "ReffReader.h"
#include "Mktdt03Reader.h"
#include "../Configuration.h"
#include "../Infrastructure/Lock.h"
#include "../Infrastructure/Thread.h"
#include "../Infrastructure/DateTime.h"
#include "../Mktdt03_SHOpt_QuoProtocal.h"


/**
 * @class			WorkStatus
 * @brief			工作状态管理
 * @author			barry
 */
class WorkStatus
{
public:
	/**
	 * @brief				应状态值映射成状态字符串
	 */
	static	std::string&	CastStatusStr( enum E_SS_Status eStatus );

public:
	/**
	 * @brief			构造
	 * @param			eMkID			市场编号
	 */
	WorkStatus();
	WorkStatus( const WorkStatus& refStatus );

	/**
	 * @brief			赋值重载
						每次值变化，将记录日志
	 */
	WorkStatus&			operator= ( enum E_SS_Status eWorkStatus );

	/**
	 * @brief			重载转换符
	 */
	operator			enum E_SS_Status();

private:
	CriticalObject		m_oLock;				///< 临界区对象
	enum E_SS_Status	m_eWorkStatus;			///< 行情工作状态
};


/**
 * @class			Quotation
 * @brief			会话管理对象
 * @detail			封装了针对商品期货期权各市场的初始化、管理控制等方面的方法
 * @author			barry
 */
class Quotation : public SimpleTask
{
public:
	Quotation();
	~Quotation();

	/**
	 * @brief			释放ctp行情接口
	 */
	int					Release();

	/**
	 * @brief			初始化ctp行情接口
	 * @return			>=0			成功
						<0			错误
	 * @note			整个对象的生命过程中，只会启动时真实的调用一次
	 */
	int					Initialize();

public:///< 公共方法函数
	/**
	 * @brief			获取会话状态信息
	 */
	WorkStatus&			GetWorkStatus();

	/**
	 * @brief			发送登录请求包
	 */
    void				SendLoginRequest();

protected:
	/**
	 * @brief			任务函数(内循环)
	 * @return			==0					成功
						!=0					失败
	 */
	virtual int			Execute();

	int					ScanFile();

private:
	CriticalObject			m_oLock;				///< 临界区对象
	WorkStatus				m_oWorkStatus;			///< 工作状态
	ReffReader				m_oReffFile;			///< 非交易文件扫描类
	CReadMktdtFile			m_oMktdt03File;			///< 行情文件扫描类
};




#endif






