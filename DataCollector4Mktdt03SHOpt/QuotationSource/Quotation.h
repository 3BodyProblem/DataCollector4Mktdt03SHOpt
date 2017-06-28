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
 * @brief			����״̬����
 * @author			barry
 */
class WorkStatus
{
public:
	/**
	 * @brief				Ӧ״ֵ̬ӳ���״̬�ַ���
	 */
	static	std::string&	CastStatusStr( enum E_SS_Status eStatus );

public:
	/**
	 * @brief			����
	 * @param			eMkID			�г����
	 */
	WorkStatus();
	WorkStatus( const WorkStatus& refStatus );

	/**
	 * @brief			��ֵ����
						ÿ��ֵ�仯������¼��־
	 */
	WorkStatus&			operator= ( enum E_SS_Status eWorkStatus );

	/**
	 * @brief			����ת����
	 */
	operator			enum E_SS_Status();

private:
	CriticalObject		m_oLock;				///< �ٽ�������
	enum E_SS_Status	m_eWorkStatus;			///< ���鹤��״̬
};


/**
 * @class			Quotation
 * @brief			�Ự�������
 * @detail			��װ�������Ʒ�ڻ���Ȩ���г��ĳ�ʼ����������Ƶȷ���ķ���
 * @author			barry
 */
class Quotation : public SimpleTask
{
public:
	Quotation();
	~Quotation();

	/**
	 * @brief			�ͷ�ctp����ӿ�
	 */
	int					Release();

	/**
	 * @brief			��ʼ��ctp����ӿ�
	 * @return			>=0			�ɹ�
						<0			����
	 * @note			������������������У�ֻ������ʱ��ʵ�ĵ���һ��
	 */
	int					Initialize();

public:///< ������������
	/**
	 * @brief			��ȡ�Ự״̬��Ϣ
	 */
	WorkStatus&			GetWorkStatus();

	/**
	 * @brief			���͵�¼�����
	 */
    void				SendLoginRequest();

protected:
	/**
	 * @brief			������(��ѭ��)
	 * @return			==0					�ɹ�
						!=0					ʧ��
	 */
	virtual int			Execute();

	int					ScanFile();

private:
	CriticalObject			m_oLock;				///< �ٽ�������
	WorkStatus				m_oWorkStatus;			///< ����״̬
	ReffReader				m_oReffFile;			///< �ǽ����ļ�ɨ����
	CReadMktdtFile			m_oMktdt03File;			///< �����ļ�ɨ����
};




#endif






