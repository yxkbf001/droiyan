// RoyalRumble.cpp: implementation of the CRoyalRumble class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "RoyalRumble.h"
#include "ServerDlg.h"
#include "extern.h"
#include "bufferex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CServerDlg *g_pMainDlg;
extern CString			g_strARKRRWinner;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRoyalRumble::CRoyalRumble()
{
	m_iMaxApplyNum = 200;		// �ִ� ��û ���

	m_iMinLevel = 26;			// �ּ� �ʿ� ����

	m_bRRStatus = RR_EMPTY;		// �ξ� ������ ���� ����

	m_iStartCount = 3;			// ���۽ÿ� ���ִ� ī��Ʈ
	m_iPlayMinute = 30;			// �ξ� ���� ��� �ð�(��)

	m_iZoneNum = 59;			// �ξ� ���� ���� ��

	m_iBravoSecond = 60;		// ��� �� ���� �ð�(��)

	m_bNextRRInit = FALSE;		// ���� ��⸦ �����ߴ����� ���� �÷���

	m_iItemSid = 840;			// ��ǰ���� �ִ� �������� SID
}

CRoyalRumble::~CRoyalRumble()
{
	m_bRRStatus = RR_EMPTY;

	for( int i = 0; i < m_arRRUser.GetSize(); i++ )
	{
		if( m_arRRUser[i] )
		{
			delete m_arRRUser[i];
		}
	}
	m_arRRUser.RemoveAll();
}

void CRoyalRumble::Init(int year, int month, int day)
{
	MAP* pMap = NULL;

	for( int i = 0; i < g_zone.GetSize(); i++ )
	{
		if( g_zone[i] )
		{
			if( g_zone[i]->m_Zone == m_iZoneNum )
			{
				pMap = g_zone[i];
				break;
			}
		}
	}

	if( !pMap )	// �ο����� ���� �Ȱ����� �ִ�. �ʱ�ȭ ����
	{
		return;
	}

	m_pRRMap = pMap;
	m_iCurrentStartCount = m_iStartCount;
	m_bRRStatus = RR_IDLE;
	m_iCurrentPlaySecond = 0;
	m_iCurrentBravoSecond = 0;
	m_bNextRRInit = FALSE;

	// ��û ���� �ð�
	m_timeApplyStart	= CTime( year, month, day, 19, 0, 0 );
	m_timeApplyEnd		= CTime( year, month, day, 19, 50, 0 );

	// ���� ���� �ð�
	m_timeEnterStart	= CTime( year, month, day, 19, 50, 0 );
	m_timeEnterEnd		= CTime( year, month, day, 20, 0, 0 );

	m_dwLastEnterMsg = GetTickCount();
	m_dwLastStartMsg = GetTickCount();

	m_ItemRR.sSid = m_iItemSid;

	m_ItemRR.sLevel		= g_arItemTable[m_ItemRR.sSid]->m_byRLevel;
	m_ItemRR.sCount		= 1;
	m_ItemRR.sDuration	= g_arItemTable[m_ItemRR.sSid]->m_sDuration;
	m_ItemRR.sBullNum	= g_arItemTable[m_ItemRR.sSid]->m_sBullNum;

	m_ItemRR.tMagic[0]	= (BYTE)137;
	m_ItemRR.tMagic[1]	= (BYTE)141;
	m_ItemRR.tMagic[2]	= (BYTE)128;
	m_ItemRR.tMagic[3]	= (BYTE)42;
	m_ItemRR.tMagic[4]	= (BYTE)31;
	m_ItemRR.tMagic[5]	= (BYTE)33;
	m_ItemRR.tIQ		= UNIQUE_ITEM;

	m_ItemRR.iItemSerial= 0;
	m_ItemRR.dwMoney	= 0;
}

void CRoyalRumble::CheckRRStatus()
{
	CTime curTime = CTime::GetCurrentTime();
	CString strMsg;

	switch( m_bRRStatus )
	{
/*	case	RR_IDLE:	// ���� ��Ⱑ �������� ���� �ʴ�
		if( curTime >= m_timeApplyStart && curTime < m_timeApplyEnd )	// ��� ���� �ð��̶��
		{
			m_bRRStatus = RR_APPLY;		// ���� ���¸� ��û���·� �ٲٰ�
		}
		break;
*/
	case	RR_EMPTY:	// �̼����� �ξⷳ�� ��ȸ�� ����.
		break;

	case	RR_IDLE:	// ���� ��Ⱑ �������� ���� �ʴ�
		if( curTime >= m_timeEnterStart && curTime < m_timeEnterEnd )	// ���� �ð��̶��
		{
			m_bRRStatus = RR_ENTERING;		// ���� ���¸� ������·� �ٲ۴�
			break;
		}

		if( curTime >= m_timeApplyStart && curTime < m_timeApplyEnd )
		{
			Apply();
		}
		break;

	case	RR_ENTERING:
		if( curTime >= m_timeEnterEnd )
		{
			m_bRRStatus = RR_ENTERING_END;
			strMsg = _T("�ξⷳ�� ������ ���۵˴ϴ�.");
			g_pMainDlg->AnnounceZone( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, m_iZoneNum );
			break;
		}

		if( GetTickCount() - m_dwLastEnterMsg > 2 * 60 * 1000 )
		{
			strMsg.Format( "�ο����� �����Ͻ� �е��� %d�� %02d�� �������� ��������� �������ֽñ� �ٶ��ϴ�", m_timeEnterEnd.GetHour(), m_timeEnterEnd.GetMinute() );
			g_pMainDlg->AnnounceAllServer( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE );
			m_dwLastEnterMsg = GetTickCount();
		}
		break;

	case	RR_ENTERING_END:
		CountDownStart();
		break;

	case	RR_START:
		if( GetTickCount() - m_dwLastStartMsg > 2 * 60 * 1000 )
		{
			strMsg = _T("���� NEO A.R.K �����忡�� �ο����� ������ ������ �ֽ��ϴ�");
			g_pMainDlg->AnnounceAllServer( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE );
			m_dwLastStartMsg = GetTickCount();
		}

		CheckRREnd();
		break;

	case	RR_END:
		CheckRRBravoEnd();
		break;
	}
}

void CRoyalRumble::CountDownStart()
{
	if( m_bRRStatus != RR_ENTERING_END )
	{
		return;
	}

	if( m_iCurrentStartCount <= 0 && m_bRRStatus != RR_START )	// ī��Ʈ�� �� �����µ��� ���۵��� ���� ��Ȳ
	{
		Start();
		return;
	}

	CString strMsg;

	if( m_iCurrentStartCount <= 0 )		// ī��Ʈ�� �� ������. ���� ��Ų��.
	{
		m_bRRStatus = RR_START;
		m_iCurrentStartCount = m_iStartCount;

		Start();
		return;
	}

	strMsg.Format("%d", m_iCurrentStartCount );		m_iCurrentStartCount--;
	g_pMainDlg->AnnounceZone( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, m_iZoneNum );
}

void CRoyalRumble::Start()
{
	CString strMsg;

	strMsg = _T("���� !!!");
	g_pMainDlg->AnnounceZone( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, m_iZoneNum );

	m_dwLastStartMsg = GetTickCount();

	m_bRRStatus = RR_START;
}

void CRoyalRumble::CheckRREnd()
{
	m_iCurrentPlaySecond++;

	CString strMsg;
	int iRemainSecond = (m_iPlayMinute * 60) - m_iCurrentPlaySecond;

	if( iRemainSecond == 5 * 60 )		// ������ 5����
	{
		strMsg = _T("5�� �� �ο����� ��Ⱑ ����˴ϴ�");
		g_pMainDlg->AnnounceZone( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, m_iZoneNum );
	}
	else if( iRemainSecond == 1 * 60 )
	{
		strMsg = _T("1�� �� �ο����� ��Ⱑ ����˴ϴ�");
		g_pMainDlg->AnnounceZone( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, m_iZoneNum );
	}
	else if( iRemainSecond <= 5 && iRemainSecond > 0 )
	{
		strMsg.Format( "%d", iRemainSecond );
		g_pMainDlg->AnnounceZone( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, m_iZoneNum );
	}

	if( m_iCurrentPlaySecond >= m_iPlayMinute * 60 )
	{
		End();
	}

	GetWinner();
}

void CRoyalRumble::End()
{
	CString strMsg;
	char strTitle[128];
	char strContent[2048];

	CTime time = CTime::GetCurrentTime();

	sprintf( strTitle, "%d�� %d�� �ο����� ��ȸ �����", time.GetMonth(), time.GetDay() );

	m_bRRStatus = RR_END;
	g_strARKRRWinner = _T("");
	m_bNextRRInit = FALSE;

	USER* pUser = GetWinner();

	if( !pUser )
	{
		strMsg = _T("�ο����� ��Ⱑ ����Ǿ����ϴ�");
		g_pMainDlg->AnnounceAllServer( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE );

		sprintf( strContent, "\r\n %d�� %d�� �ο����� ��ȸ��\r\n\r\n����ڰ� �����ϴ�.", time.GetMonth(), time.GetDay() );

		BBSWrite( strTitle, strContent );

		UpdateRoyalRumbleWinner();
		InitRRUser();

		return;
	}

	strMsg.Format( "%s���� �ο����� ����ڰ� �Ǽ̽��ϴ�. ���ϵ帳�ϴ�", pUser->m_strUserID );
	g_pMainDlg->AnnounceAllServer( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE );

	sprintf( strContent, "\r\n %d�� %d�� �ο����� ��ȸ ����ڴ�\r\n\r\n %s �� �Դϴ�\r\n\r\n *** ���ϵ帳�ϴ� ***", time.GetMonth(), time.GetDay(), pUser->m_strUserID );

	BBSWrite( strTitle, strContent );

	g_strARKRRWinner.Format( "%s", pUser->m_strUserID );

	UpdateRoyalRumbleWinner();

	GiveItemToWinner( pUser );

	InitRRUser();
}

USER* CRoyalRumble::GetWinner()
{
	int i = 0;
	int iCount = 0;

	USER* pUser = NULL;
	USER* pWinner = NULL;

	COM* pCom = g_pMainDlg->GetCOM();
	if( !pCom ) return NULL;

	if( m_bRRStatus == RR_END )	// ������ ������ ��� ó��
	{
		for( i = 0; i < m_arRRUser.GetSize(); i++ )
		{
			if( m_arRRUser[i] )
			{
				if( !m_arRRUser[i]->m_bLive ) continue;
				if( m_arRRUser[i]->m_iUID < 0 || m_arRRUser[i]->m_iUID > MAX_USER ) continue;

				pUser = pCom->GetUserUid( m_arRRUser[i]->m_iUID );

				if( !pUser ) continue;
				if( pUser->m_state != STATE_GAMESTARTED ) continue;
				if( pUser->m_curz != m_iZoneNum ) continue;
				if( strcmp( pUser->m_strUserID, m_arRRUser[i]->m_strUserID ) ) continue;
				if( pUser->m_bLive == FALSE ) continue;
				if( pUser->m_sHP <= 0 ) continue;

				iCount++;	// �� ����� ��Ƴ��� ����̴�.
				pWinner = pUser;
			}
		}

		// �� 1�� ��Ƴ��ƾ� ����ڷ� ó��
		if( iCount == 1 )
		{
			return pWinner;
		}
		else
			return NULL;
	}
	else if( m_bRRStatus == RR_START )	// ���� �� ó��
	{
		for( i = 0; i < m_arRRUser.GetSize(); i++ )
		{
			if( m_arRRUser[i] )
			{
				if( !m_arRRUser[i]->m_bLive ) continue;
				if( m_arRRUser[i]->m_iUID < 0 || m_arRRUser[i]->m_iUID > MAX_USER ) continue;

				iCount++;
			}
		}

		if( iCount <= 1 )
		{
			m_bRRStatus = RR_END;
			End();
			return NULL;
		}
	}

	return NULL;
}

BOOL CRoyalRumble::CheckEnteringByTime()
{
	if( m_bRRStatus == RR_ENTERING ) return TRUE;

	return FALSE;
}

BOOL CRoyalRumble::CheckEnteringByMaxUser()
{
	if( m_arRRUser.GetSize() < m_iMaxApplyNum ) return TRUE;

	return FALSE;
}

BOOL CRoyalRumble::Enter(USER *pUser)
{
	if( !pUser ) return FALSE;

	CRoyalRumbleUser* pNewUser = new CRoyalRumbleUser;

	pNewUser->m_iUID		= pUser->m_uid;
	strcpy( pNewUser->m_strUserID, pUser->m_strUserID );

	m_arRRUser.Add( pNewUser );

	return TRUE;
}

void CRoyalRumble::Exit(USER* pUser)
{
	if( !pUser ) return;

	int i;

	for( i = 0; i < m_arRRUser.GetSize(); i++ )
	{
		if( m_arRRUser[i] )
		{
			if( m_arRRUser[i]->m_iUID == pUser->m_uid )
			{
				if( !strcmp( m_arRRUser[i]->m_strUserID, pUser->m_strUserID ) )
				{
					m_arRRUser[i]->m_bLive = FALSE;
				}
			}
		}
	}

	GetWinner();
}

void CRoyalRumble::CheckRRBravoEnd()
{
	m_iCurrentBravoSecond++;

	if( m_iCurrentBravoSecond >= m_iBravoSecond )
	{
		m_bRRStatus = RR_IDLE;
		ForceExit();
	}
}

void CRoyalRumble::ForceExit()
{
	USER* pUser = NULL;
	COM* pCom = g_pMainDlg->GetCOM();
	if( !pCom ) return;

	for( int i = 0; i < MAX_USER; i++ )
	{
		pUser = pCom->GetUserUid( i );

		if( !pUser ) continue;

		if( pUser->m_state != STATE_GAMESTARTED ) continue;

		if( pUser->m_curz != m_iZoneNum ) continue;

		if( pUser->CheckInvalidMapType() != 12 ) continue;

		pUser->TownPotal();
	}
}

void CRoyalRumble::BBSWrite(char *strTitle, char *strContent)
{
	char strWriter[128];
	sprintf( strWriter, "���" );

	SDWORD sTitleLen	= _tcslen(strTitle);
	SDWORD sContentLen	= _tcslen(strContent);
	SDWORD sIDLen		= _tcslen(strWriter);

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	int bbsnum = 3;		// �ο������ �Խ���

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_WRITE ( %d, ?, ?, ? )}" ), bbsnum );

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) { return; }

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		return;
	}

	int i = 1;
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 20,		0, (TCHAR*)strWriter,	0, &sIDLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 50,		0, (TCHAR*)strTitle,	0, &sTitleLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 5000,	0, (TCHAR*)strContent,	0, &sContentLen );

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else if (retcode == SQL_ERROR)
	{
		DisplayErrorMsg(hstmt);
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return;
	}
	
	SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

	return;
}

void CRoyalRumble::UpdateRoyalRumbleData()
{
	if( m_bNextRRInit )		// ���� ��� ������ �����Ǿ����Ƿ� ������Ʈ ���� �ʴ´�.
	{
		return;
	}

	m_bNextRRInit = TRUE;	// ���� ��� ������ �����Ǿ���.

	CTime curtime = CTime::GetCurrentTime();
	CTimeSpan addtime( 28, 0, 0, 0 );

	curtime = curtime + addtime;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	int i = 0;

	SQLINTEGER	sInd;
	SQLSMALLINT sYear, sMonth, sDay;
	SQLCHAR		strWinner[CHAR_NAME_LENGTH+1];

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strWinner, sizeof(strWinner));

	sYear = curtime.GetYear();
	sMonth = curtime.GetMonth();
	sDay = curtime.GetDay();

	sInd = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("update royal_rumble set nYear = %d, nMonth = %d, nDay = %d, strWinner = \'%s\'"), sYear, sMonth, sDay, g_strARKRRWinner);

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Royal Rumble Data !!\n");

		return;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	BridgeServerARKWinnerChangeReq();
	return;
}

void CRoyalRumble::RobItem()
{
	return;

	if( g_strARKRRWinner.GetLength() <= 0 ) return;

	USER* pUser = NULL;
	COM* pCom = g_pMainDlg->GetCOM();
	if( !pCom ) return;

	char tempUserID[CHAR_NAME_LENGTH+1];
	strcpy( tempUserID, g_strARKRRWinner );

	pUser = pCom->GetUser( tempUserID );

	if( !pUser ) return;
	if( pUser->m_state != STATE_GAMESTARTED ) return;

	CBufferEx TempBuf;

	int i, j;

	for( i = 0; i < TOTAL_ITEM_NUM; i++ )
	{
		if( pUser->m_UserItem[i].sSid != m_iItemSid ) continue;

		pUser->ReSetItemSlot( &(pUser->m_UserItem[i]) );
		pUser->CheckMagicItemMove();
		pUser->UpdateUserItemDN();

		pUser->GetRecoverySpeed();			// �ٽ� ȸ���ӵ��� ���

		TempBuf.Add(ITEM_GIVE_RESULT);
		TempBuf.Add(SUCCESS);
		TempBuf.Add((BYTE)i);
		TempBuf.Add(pUser->m_UserItem[i].sLevel);
		TempBuf.Add(pUser->m_UserItem[i].sSid);
		TempBuf.Add(pUser->m_UserItem[i].sDuration);
		TempBuf.Add(pUser->m_UserItem[i].sBullNum);
		TempBuf.Add(pUser->m_UserItem[i].sCount);
		for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add(pUser->m_UserItem[i].tMagic[j]);

		TempBuf.Add(pUser->m_UserItem[i].tIQ);

		pUser->Send(TempBuf, TempBuf.GetLength());
	}

	for( i = 0; i < TOTAL_BANK_ITEM_NUM; i++ )
	{
		if( pUser->m_UserBankItem[i].sSid != 840 ) continue;

		pUser->ReSetItemSlot( &(pUser->m_UserBankItem[i]) );
		pUser->UpdateUserBank();

		pUser->GetRecoverySpeed();			// �ٽ� ȸ���ӵ��� ���

		TempBuf.Add(ITEM_GIVE_RESULT);
		TempBuf.Add(SUCCESS);
		TempBuf.Add((BYTE)i);
		TempBuf.Add(pUser->m_UserBankItem[i].sLevel);
		TempBuf.Add(pUser->m_UserBankItem[i].sSid);
		TempBuf.Add(pUser->m_UserBankItem[i].sDuration);
		TempBuf.Add(pUser->m_UserBankItem[i].sBullNum);
		TempBuf.Add(pUser->m_UserBankItem[i].sCount);
		for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add(pUser->m_UserBankItem[i].tMagic[j]);

		TempBuf.Add(pUser->m_UserBankItem[i].tIQ);

		pUser->Send(TempBuf, TempBuf.GetLength());
	}

	pUser->SendSystemMsg( "�ο����� ��ǰ �������� ȸ�� �Ǿ����ϴ�", SYSTEM_NORMAL, TO_ME);
}

void CRoyalRumble::GiveItemToWinner(USER *pUser)
{
	return;

	int iWeight = 0;
	int iSlot;

	iWeight = g_arItemTable[m_ItemRR.sSid]->m_byWeight;

	CWordArray		arEmptySlot, arSameSlot;

	iSlot = pUser->GetEmptySlot( INVENTORY_SLOT );

	if( iSlot < 0 ) return;

	pUser->ReSetItemSlot( &(pUser->m_UserItem[iSlot]) );

	pUser->m_UserItem[iSlot].sLevel			= m_ItemRR.sLevel;
	pUser->m_UserItem[iSlot].sSid			= m_ItemRR.sSid;
	pUser->m_UserItem[iSlot].sCount			= m_ItemRR.sCount;
	pUser->m_UserItem[iSlot].sDuration		= m_ItemRR.sDuration;
	pUser->m_UserItem[iSlot].sBullNum		= m_ItemRR.sBullNum;
	pUser->m_UserItem[iSlot].tIQ			= m_ItemRR.tIQ;

	pUser->m_UserItem[iSlot].iItemSerial	= m_ItemRR.iItemSerial;
	pUser->m_UserItem[iSlot].dwMoney		= m_ItemRR.dwMoney;

	for( int i = 0; i < MAGIC_NUM; i++ ) pUser->m_UserItem[iSlot].tMagic[i] = m_ItemRR.tMagic[i];

	arEmptySlot.Add(iSlot); 

	pUser->m_iCurWeight += iWeight;
	pUser->GetRecoverySpeed();					// ������ ���Կ� ������ ����� ȸ���ӵ� ��ȯ

	pUser->UpdateInvenSlot(&arEmptySlot, &arSameSlot);

	return;
}

void CRoyalRumble::UpdateRoyalRumbleWinner()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	int i = 0;

	SQLINTEGER	sInd;
	SQLCHAR		strWinner[CHAR_NAME_LENGTH+1];

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strWinner, sizeof(strWinner));

	sInd = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("update royal_rumble set strWinner = \'%s\'"), g_strARKRRWinner);

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Royal Rumble Data !!\n");

		return;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	BridgeServerARKWinnerChangeReq();

	return;
}

void CRoyalRumble::ForceInit()
{
	RobItem();
	g_strARKRRWinner = _T("");
	UpdateRoyalRumbleWinner();
	m_bNextRRInit = TRUE;

	InitRRUser();

	m_iCurrentStartCount = m_iStartCount;
	m_bRRStatus = RR_IDLE;
	m_iCurrentPlaySecond = 0;
	m_iCurrentBravoSecond = 0;

	CTime curtime = CTime::GetCurrentTime();
	CTimeSpan addtime( 0, 0, 10, 0 );

	m_timeApplyStart	= curtime;
	m_timeApplyEnd		= curtime;

	// ���� ���� �ð�
	m_timeEnterStart	= curtime;
	m_timeEnterEnd		= curtime + addtime;

	m_dwLastEnterMsg = 0;
	m_dwLastStartMsg = 0;

	MAP* pMap = NULL;

	for( int i = 0; i < g_zone.GetSize(); i++ )
	{
		if( g_zone[i] )
		{
			if( g_zone[i]->m_Zone == m_iZoneNum )
			{
				pMap = g_zone[i];
				break;
			}
		}
	}

	if( !pMap )	// �̻���� �߻�
	{
		AfxMessageBox( "Can't find RR Map" );
		return;
	}

	m_pRRMap = pMap;

	m_ItemRR.sSid = m_iItemSid;

	m_ItemRR.sLevel		= g_arItemTable[m_ItemRR.sSid]->m_byRLevel;
	m_ItemRR.sCount		= 1;
	m_ItemRR.sDuration	= g_arItemTable[m_ItemRR.sSid]->m_sDuration;
	m_ItemRR.sBullNum	= g_arItemTable[m_ItemRR.sSid]->m_sBullNum;

	m_ItemRR.tMagic[0]	= (BYTE)137;
	m_ItemRR.tMagic[1]	= (BYTE)141;
	m_ItemRR.tMagic[2]	= (BYTE)128;
	m_ItemRR.tMagic[3]	= (BYTE)42;
	m_ItemRR.tMagic[4]	= (BYTE)31;
	m_ItemRR.tMagic[5]	= (BYTE)33;
	m_ItemRR.tIQ		= UNIQUE_ITEM;

	m_ItemRR.iItemSerial= 0;
	m_ItemRR.dwMoney	= 0;
}

void CRoyalRumble::InitRRUser()
{
	int i;

	for( i = 0; i < m_arRRUser.GetSize(); i++ )
	{
		if( m_arRRUser[i] )
		{
			delete m_arRRUser[i];
		}
	}
	m_arRRUser.RemoveAll();
}

void CRoyalRumble::Apply()	// ���� ��û�� �޴� �Լ���... ���� ������ ���� �ʱ�ȭ�� ���̴� ������ �뵵 ����
{
	RobItem();
	g_strARKRRWinner = _T("");

	UpdateRoyalRumbleData();

	CString strMsg;
	CTime time = CTime::GetCurrentTime();

	if( ( time.GetMinute() % 10 ) == 5 ||  ( time.GetMinute() % 10 ) == 0 )		// 5�� ���� �ѹ���
	{
		strMsg.Format( "���� 8�� NEO A.R.K �����忡�� �ο������Ⱑ �����ϴ�. ���� ���� ��Ź�帳�ϴ�" );
		g_pMainDlg->AnnounceAllServer( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE );
	}
}

void CRoyalRumble::BridgeServerARKWinnerChangeReq()
{
	int index = 0;
	TCHAR TempBuf[SEND_BUF_SIZE];

	char strWinner[CHAR_NAME_LENGTH+1];		memset( strWinner, NULL, CHAR_NAME_LENGTH+1 );
	strcpy( strWinner, g_strARKRRWinner );

	SetByte( TempBuf, SERVER_ARK_WINNER, index );
	SetByte( TempBuf, strlen( strWinner ), index );
	SetString( TempBuf, strWinner, strlen( strWinner ), index );

	g_pMainDlg->m_pBridgeSocket->Send( TempBuf, index );
}
