///////////////////////////////////////////////////////////////////
//	COM.cpp : implementation file for the COM class
//  Copyright (C) 2000,  Jin-Soo Jeon
//
//	If this code works, it was written by JinSoo Jeon.
//	If not, I don't know who wrote it.
//
#include "stdafx.h"
#include "Server.h"
#include "USER.h"
#include "COM.h"
#include "BufferEx.h"
#include "Extern.h"
#include <process.h>
#include "ServerDlg.h"
#include "Search.h"
#include "RoyalRumble.h"
#include "EventZone.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define GUILD_BEGIN_WAR_TIME		10

//////////////////////////////////////////////////////////////////////
//	Global Variable
// IKING 2001.1.
extern CServerDlg			*g_pMainDlg;
extern int					MAXPOSSIBLE_USER;
extern CSearch				*g_pUserList;
extern CRITICAL_SECTION		m_CS_SqlData;
extern long					nSqlDataCount;		
extern CPtrList				RecvSqlData;
extern CRoyalRumble			g_RR;
CString winName;

HANDLE				g_hIOCP				= NULL;
SOCKET				g_sdListen			= INVALID_SOCKET;
WSAEVENT			g_hListenEvent		= WSA_INVALID_EVENT;

_int64				g_OnlineEnd			= 0;
_int64				g_OnlineMinStart	= 0;
_int64				g_Online_Update_Min_ticks = 0;

long				g_GameTime			= 0;
long				g_GameDay			= 0;
long				g_GameMinute		= 0;

int					PKnum;		//PK剩余人数
DWORD				ShaRenNum;	//杀人王最大值
CString				ShaRenName;	//杀人王姓名
BOOL				ShaRenEnd;
BOOL				PKover;		//标识PK结束


volatile long g_bShutDown = 0;
int mIndex = 0;

// Quest Event Class
CEventZone	g_QuestEventZone;
///////////////////////////////////////////////////////////////////////////
// Global functions

//DWORD WINAPI SendThreadMain(LPVOID pVoid)
//unsigned __stdcall SendThreadMain( void *pVoid )
UINT SendThreadMain( void *pVoid )
{
	COM* pCom = (COM*)pVoid;

	int					nRet				= 0;
	int					iRemainCount		= 0;
	DWORD				dwBytesTransferred	= 0;
	DWORD				dwKey				= 0;
	LPOVERLAPPED		pOverlapped			= NULL;

	SEND_DATA*			pSendData			= NULL;
	SEND_DATA*			pNewData			= NULL;

	int					i					= 0;

	int					modsid				= 0;
	CRITICAL_SECTION*	pSendCS				= NULL;

	CPtrList			LocalDataList;
	int					nLocalDataCount = 0;

	while( TRUE )
	{
		nRet = ::GetQueuedCompletionStatus(pCom->m_hSendIOCP, &dwBytesTransferred, &dwKey, &pOverlapped, INFINITE);

		// assume we are always using an INFINITE timeout
		if( nRet == FALSE && pOverlapped == NULL )
		{
			TRACE("####Error In SendThreadMain()\n");
			break;
		}

		if( nRet == FALSE ) continue;


		modsid = (int)dwKey;

		if( modsid < 0 || modsid > AUTOMATA_THREAD ) continue;


		pSendCS = &(pCom->m_critSendData[modsid]);

		if( !pSendCS ) continue;


		pCom->nSendDataCount[modsid] = pCom->SendDataList[modsid].GetCount();

		if( pCom->nSendDataCount[modsid] < 1 ) continue;


		EnterCriticalSection( pSendCS );

		while( pCom->nSendDataCount[modsid] >= 1 )
		{
			pSendData = (SEND_DATA*)pCom->SendDataList[modsid].RemoveHead();
			pCom->nSendDataCount[modsid] = pCom->SendDataList[modsid].GetCount();

			if( pSendData )
			{
				LocalDataList.AddTail( pSendData );
			}
		}

		LeaveCriticalSection( pSendCS );

		nLocalDataCount = LocalDataList.GetCount();


		while( nLocalDataCount >= 1 )
		{
			pSendData = (SEND_DATA*)LocalDataList.RemoveHead();
			nLocalDataCount = LocalDataList.GetCount();

			if( !pSendData ) continue;

			pCom->Send( pSendData );

			delete pSendData;
			pSendData = NULL;
		}
	}

	return 0;
}

//DWORD WINAPI TimerThread(LPVOID lParam)
//unsigned __stdcall  TimerThread(void *lParam)
UINT TimerThread(void *lParam)
{
	COM* pCom = (COM*) lParam;

	BOOL bRain = FALSE;
	int	 i, iTime = 0;
	int  iUserCountUpdate = 0;

	int iRainTime[30] = {1, 1, 14, 11, 14, 16, 11, 13, 16, 12,
						 11, 12, 13, 11, 14, 10, 11, 13, 15, 12,
						 14, 12, 9, 11, 14, 16, 16, 13, 16, 12};

	g_GameTime = 3;		// 抛胶飘甫 困秦...


	while(TRUE)
	{
		//线程退出
		if(g_bShutDown)
			break;
		pCom->CheckCurrentUserTime();
	    pCom->CheckMessageTime();
		pCom->CheckPKTime();
		pCom->CheckPKShaRenTime();
		pCom->CheckDevilTime();
		pCom->CheckGuildWarTime();	// 辨靛傈牢瘤 眉农茄促.
		pCom->Check_DGB_Massage(); //电光板排队
		pCom->UpdateUnLineUserTime(); ////离线更新数据
        pCom->CheckFortressWarTime();
		g_RR.CheckRRStatus();

		QueryPerformanceCounter((LARGE_INTEGER*)&g_OnlineEnd);

		if((g_OnlineEnd - g_OnlineMinStart) >= g_Online_Update_Min_ticks)
		{
			g_OnlineMinStart += g_Online_Update_Min_ticks;

			InterlockedExchangeAdd(&g_GameMinute, 10);				// 公炼扒 10盒究 刘啊
																	// 60盒 捞搁 0栏肺 檬扁拳	
			if(g_GameMinute == 60)
			{
				::InterlockedExchange(&g_GameMinute, 0);

				InterlockedExchangeAdd(&g_GameTime, 1);				// 1矫埃究 刘啊茄促. 
				if(g_GameTime == 24) 
				{ 
					InterlockedExchangeAdd(&g_GameDay, 1);			// 24矫捞搁 窍风 刘啊茄促.
					::InterlockedExchange(&g_GameTime, 1);			// 矫埃篮 1矫肺 悸泼
				}
				if(g_GameDay == 31) ::InterlockedExchange(&g_GameDay, 1);// 朝楼档 窍风

				pCom->SendTimeToAll();
				
				if(iRainTime[g_GameDay] == g_GameTime)
				{
//					TRACE("厚棵矫埃牢单...Day : %d, Time : %d\n", g_GameDay, iRainTime[g_GameDay]);

					for(i = 0; i < g_WeatherZone.GetSize(); i++)
					{
	//					if(rand()%2 == 0)	//犬伏 50%
	//					{
//							TRACE("厚啊柯促...Day : %d, Time : %d\n", g_GameDay, iRainTime[g_GameDay]);
							bRain = TRUE;
							iTime = g_GameTime;
							g_WeatherZone[i]->bRaining = TRUE;
							pCom->SetWeatherZone(g_WeatherZone[i]->iZone, 2, 1);
	//					}
					}
				}

				if(bRain && (iTime+2 == g_GameTime))
				{
//					TRACE("厚~场...Day : %d, Time : %d\n", g_GameDay, iRainTime[g_GameDay]);
					for(i = 0; i < g_WeatherZone.GetSize(); i++)
					{
						if(g_WeatherZone[i]->bRaining) 
						{
							bRain = FALSE;
							g_WeatherZone[i]->bRaining = FALSE;
							pCom->SetWeatherZone(g_WeatherZone[i]->iZone, 2, 0);
						}
					}
				}
			}

//			TRACE("瘤陛 盒俊辑绰 矫埃 %d : %d盒\n", g_GameTime, g_GameMinute);
		}
		Sleep(1000);
/*
		iUserCountUpdate++;

		if( iUserCountUpdate >= 600 )
		{
			iUserCountUpdate = 0;

			pCom->UpdateUserCount();
*/
/*
			int Datalength;
			SQLDATAPACKET *pSDP;
			pSDP = new SQLDATAPACKET;
			pSDP->code = DB_USER_COUNT;
			Datalength = 0;
			pSDP->dcount = Datalength;
			pSDP->UID = 0;
			pSDP->pData = NULL;

			EnterCriticalSection( &m_CS_SqlData );
			RecvSqlData.AddTail(pSDP);
			nSqlDataCount = RecvSqlData.GetCount();
			LeaveCriticalSection( &m_CS_SqlData );
*/
//		}
	}
	return 1;
}

////////////////////////////////////////////////////////////////////////
//	Event 扁埃阑 悸泼窍扁 困茄 Thread
//
UINT EventTimerThread(void *lParam)
{
	COM* pCom = (COM*) lParam;

	while(TRUE)
	{
		/*
		SYSTEMTIME st;
		::GetLocalTime(&st);
		COleDateTime oleEventStart(2002, 12, 27, 9, 0, 0), oleEventEnd(2002, 12, 28, 9, 0, 0);
		COleDateTime oleToday;

		if(st.wYear == 2002 && st.wMonth == 12)
		{
			if(st.wDay >= 27 && st.wDay <= 28) 
			{
				oleToday.SetDateTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				if(oleToday >= oleEventStart && oleToday <= oleEventEnd) g_iMoonEvent = 1;
				else g_iMoonEvent = 1;
			}
			else 
			{
				g_iMoonEvent = 1;
			}
		}
		else 
		{
			g_iMoonEvent = 1;
		}
		*/
		//线程退出
		if(g_bShutDown)
			break;
		pCom->ThrowItemRefresh();

		Sleep(1000 * 30);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

COM::COM()
{
	m_bInit = FALSE;
	m_dwConcurrency = 1;
}

COM::~COM()
{
	if(m_bInit)
	{
		for(int i = 0; i < AUTOMATA_THREAD+1; i++) DeleteCriticalSection( &m_critSendData[i] );

		DeleteCriticalSection( &m_critGuild );
		DeleteCriticalSection( &m_critEvent );

		// IKING 2001.1.
		DeleteCriticalSection( &m_critThrowItem );
		//

		DeleteAll();
	}
}

///////////////////////////////////////////////////////////////////////
//	劝寸沁带 皋葛府甫 秦力茄促.
//
void COM::DeleteAll()
{
	DeleteThrowItemArray();
}

/////////////////////////////////////////////////////////////////////////
//	Item Array 昏力
//
void COM::DeleteThrowItemArray()
{
	int i;

	for(i = 0; i < MAX_THROW_ITEM; i++ )
	{
		if( m_ThrowItemArray[i] )
		{
			delete m_ThrowItemArray[i];
			m_ThrowItemArray[i] = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//	甘俊 阶捞绰 酒捞袍 Array 檬扁拳
//
void COM::InitThrowItemArray()
{
	int i;
	CThrowItem  *f=NULL;
	CThrowItem  *n=NULL;

	m_ThrowAddIndex = 0;

	for(i = 0; i < MAX_THROW_ITEM; i++)
	{
		
		m_ThrowItemArray[i] = new CThrowItem;
		m_ThrowItemArray[i]->ID=i;
		if(n!=NULL)
			n->next=m_ThrowItemArray[i];
		n=m_ThrowItemArray[i];
		m_ThrowItemArray[i]->front=f;
		f=m_ThrowItemArray[i];

	}
	m_ThrowItemArray[MAX_THROW_ITEM-1]->next=NULL;
	m_ThrowItemUse=NULL;//已被使用物品队列
	m_ThrowItemUnuse=m_ThrowItemArray[0];//未被使用用的物品队列
}

bool COM::ThrowItemAdd(ItemList *pItem, int x, int y, int z)
{
	CThrowItem  *ItemAdd;
	MAP* pAddMap = g_zone[z];

	EnterCriticalSection( &m_critThrowItem );
	//从空队列取一点
	ItemAdd=ItemGetID();
	if(ItemAdd==NULL){
		LeaveCriticalSection( &m_critThrowItem );
		return false;
	}

	//把新物品存到取出结构中
	ItemAdd->m_pItem = pItem;
	ItemAdd->m_z = z;			
	ItemAdd->m_x = x;
	ItemAdd->m_y = y;
	ItemAdd->dTime=GetTickCount();
	//把取出结构插到已经用的队列中
	ItemUseInset(ItemAdd);
	pAddMap->m_pMap[x][y].iIndex = ItemAdd->ID;
	LeaveCriticalSection( &m_critThrowItem );
	return true;
}
CThrowItem * COM::ItemGetID()
{
	CThrowItem *Item=NULL;
    Item=m_ThrowItemUnuse;
	if(m_ThrowItemUnuse==NULL)
		return NULL;
	m_ThrowItemUnuse=m_ThrowItemUnuse->next;
	if(m_ThrowItemUnuse!=NULL)
		m_ThrowItemUnuse->front=NULL;
	return Item;
}
void COM::ItemUseInset(CThrowItem *Add)
{
	CThrowItem *tmp;

	tmp=m_ThrowItemUse;
	m_ThrowItemUse=Add;
	Add->next=tmp;
	if(tmp!=NULL)
		tmp->front=Add;
	

}
void COM::ThrowItemDel(CThrowItem *Del)
{
	CThrowItem *tmp;
	//删除节点
	if(m_ThrowItemUse==Del){ //头节点删除
		m_ThrowItemUse=Del->next;
		if(m_ThrowItemUse!=NULL)
			m_ThrowItemUse->front=NULL;
	}else{
		if(Del->next!=NULL){ //尾节点删除
			Del->next->front=Del->front;
		}
		Del->front->next=Del->next;
	}
	//将未用的节点放入未用的队列中
	tmp=m_ThrowItemUnuse;
	m_ThrowItemUnuse=Del;
	Del->next=tmp;
	if(tmp!=NULL)
		tmp->front=Del;
}
//服务器所有物品刷新
void COM::ThrowItemRefresh()
{
	CThrowItem *p;
	DWORD TimeNow=GetTickCount();

	p=m_ThrowItemUse;
	EnterCriticalSection( &m_critThrowItem );
//	while(p){
	for(int i=0 ;i<MAX_THROW_ITEM;i++){
		p=m_ThrowItemArray[i];
		if(!p->m_pItem){
		/*	MAP* pAddMap = g_zone[p->m_z];
			int mapIndex= pAddMap->m_pMap[p->m_x][p->m_y].iIndex;
			*/
			continue;
		}
		//刷新时间到了 清掉这个物品
	//	if((TimeNow - p->m_pItem->dwTime) > (180 *`1000)){
		//TRACE( "牛牛物品刷新 %d\n", TimeNow - p->dTime);
		if((TimeNow - p->dTime) > (60 * 1000)){
		//	SendItemFieldInfo(ITEM_INFO_PICKUP, p->m_pItem, p->m_x,p->m_y,p->m_z);
			
			MAP* pAddMap = g_zone[p->m_z];
			int mapIndex= pAddMap->m_pMap[p->m_x][p->m_y].iIndex;

			if(mapIndex!=-1){
				if ( m_ThrowItemArray[mapIndex] != NULL )		//清除
				{
					if(m_ThrowItemArray[mapIndex]->m_pItem)
					{
						CPoint t = ConvertToClient( p->m_x, p->m_y, pAddMap->m_vMoveCell.m_vDim.cx, pAddMap->m_vMoveCell.m_vDim.cy );
						if( t.x == -1 || t.y == -1 ) continue;

						int index = 0;
						char temp_send[1024];

						SetByte( temp_send, FIELD_ITEM_INFO, index );
						SetShort( temp_send, 1, index );
						SetByte( temp_send, ITEM_INFO_DELETE, index );

						SetShort( temp_send, t.x, index );
						SetShort( temp_send, t.y, index );

						if(m_ThrowItemArray[mapIndex]->m_pItem->tType == TYPE_ITEM)
						{
							SetShort( temp_send, m_ThrowItemArray[mapIndex]->m_pItem->sSid, index );
							SetDWORD( temp_send, m_ThrowItemArray[mapIndex]->m_pItem->sCount, index );
							SetByte( temp_send, m_ThrowItemArray[mapIndex]->m_pItem->tIQ, index);
						}
						else
						{
							SetShort( temp_send, TYPE_MONEY_SID, index );
							SetDWORD( temp_send, m_ThrowItemArray[mapIndex]->m_pItem->dwMoney, index );
							SetByte( temp_send, 0, index);
						}

						SEND_DATA* pNewData = NULL;

						pNewData = new SEND_DATA;

						if( !pNewData ) return;

						pNewData->flag = SEND_INSIGHT;
						pNewData->len = index;

						memcpy( pNewData->pBuf, temp_send, index );


						pNewData->uid = 0;
						pNewData->x = p->m_x;
						pNewData->y = p->m_y;
						pNewData->z = pAddMap->m_Zone;
						pNewData->zone_index = m_ThrowItemArray[mapIndex]->m_z;

						Send(pNewData);
						delete pNewData;

						delete m_ThrowItemArray[mapIndex]->m_pItem;
						m_ThrowItemArray[mapIndex]->m_pItem = NULL;
						ThrowItemDel(m_ThrowItemArray[mapIndex]);
						pAddMap->m_pMap[p->m_x][p->m_y].iIndex=-1;
					}
				}
			}
		}

	}

	LeaveCriticalSection( &m_critThrowItem );
}


BOOL COM::Init(int port)
{
	GetCPUCount();

	char name[255];
//	PHOSTENT hostinfo;

	if( gethostname ( name, sizeof(name)) == 0)
	{
//		  if((hostinfo = gethostbyname(name)) != NULL)
//		  {
//				m_strIP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
//		  }
		m_strIP.Format( "%s", name );
	}

	for( int i = 0; i < AUTOMATA_THREAD+1; i++ )
		InitializeCriticalSection( &m_critSendData[i] );

	InitializeCriticalSection( &m_critGuild );
	InitializeCriticalSection( &m_critEvent );

	// IKING 2001.1.
	InitializeCriticalSection( &m_critThrowItem );
	//
	
	InitThrowItemArray();
	
	CreateTimerThread();
	CreateEventTimerThread();
	
	// New Send Thread - alisia
//	InitSendThread();
//	CreateSendThread();

	m_bInit = TRUE;
	//////////////////////////电光板排队
		InitializeCriticalSection(&m_DGB_CS); 
  ////////////////////////////////////////////////
	m_TotalUserCount = 0;

	return TRUE;
}

void COM::SendItemFieldInfo(BYTE type, ItemList *pItem, int x, int y,int z)
{
	if(!pItem) return;

	CBufferEx TempBuf;

	TempBuf.Add(FIELD_ITEM_INFO);
	TempBuf.Add((short)0x01);
	TempBuf.Add(type);
	TempBuf.Add((short)x);
	TempBuf.Add((short)y);

	if(pItem->tType == TYPE_ITEM)	
	{
		TempBuf.Add(pItem->sSid);
		TempBuf.Add((DWORD)pItem->sCount);
		TempBuf.Add((BYTE)pItem->tIQ);
	}
	else
	{
		TempBuf.Add(TYPE_MONEY_SID);
		TempBuf.Add(pItem->dwMoney);
		TempBuf.Add((BYTE)0);
	}


	SendInsight(TempBuf, TempBuf.GetLength(),x,y,z);

}
void COM::SendInsight(TCHAR *pBuf, int nLength,int x,int y,int z)
{
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL) return;

	pNewData->flag = SEND_INSIGHT;
	pNewData->len = nLength;

	::CopyMemory(pNewData->pBuf, pBuf, nLength);

	pNewData->uid = 0;
	pNewData->x = x;
	pNewData->y = y;
	pNewData->z = z;
	pNewData->zone_index = 0;

	Send(pNewData);
	if(pNewData) delete pNewData;
}
/////////////////////////////////////////////////////////////////////////////////////////
//	Send 甫 淬寸且 Thread甫 父电促. (泅犁绰 CPU 肮荐 * 2)
//
/*
void COM::CreateSendThread()
{
	// ensure we aren't wiping out a valid completion port
	ASSERT( m_hSendIOCP == INVALID_HANDLE_VALUE );

	m_hSendIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(SErr(m_hSendIOCP == NULL, _T("CreateWorkerThread"))) return;

	HANDLE			hThread = NULL;
	unsigned int	dwWorkerId;

	for(DWORD i = 0; i < AUTOMATA_THREAD * 2; i++)
	{
		//if(SErr((hThread = ::CreateThread( NULL, 0, SendThreadMain, (LPVOID)this, 0, &dwWorkerId)) == NULL, _T("CreateSendThread"))) return;
		if(SErr((hThread = (HANDLE)_beginthreadex( NULL, 0, SendThreadMain, (LPVOID)this, 0, &dwWorkerId)) == NULL, _T("CreateSendThread"))) return;
		//if(SErr((hThread = (HANDLE)_beginthreadex( NULL, 0, &SendThreadMain, (LPVOID)this, 0, &dwWorkerId)) == NULL, _T("CreateSendThread"))) return;
		
		//m_pSendThread = AfxBeginThread(SendThreadMain, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		//ASSERT_POINTER( m_pSendThread, CWinThread );
	}
}
*/

///////////////////////////////////////////////////////////////////////////////////
//	global Time甫 蜡瘤且 Timer Thread 甫 父电促.
//
BOOL COM::CreateTimerThread()
{
	//DWORD id;
	//unsigned int id;
	//DWORD dwThreadAffinityMask = 0;
	//HANDLE hTimerThread;

	_int64 frequence = 0;

	if(!QueryPerformanceFrequency((LARGE_INTEGER*)&frequence)) return FALSE;

//	g_Online_Update_ticks = (UPDATE_TICKS * frequence) / 1000;		// 500000
	g_Online_Update_Min_ticks = (UPDATE_TICKS * frequence) / 5000;	// 100000

//	QueryPerformanceCounter((LARGE_INTEGER*)&g_OnlineStart);
	QueryPerformanceCounter((LARGE_INTEGER*)&g_OnlineMinStart);

	//hTimerThread = ::CreateThread( NULL, 0, TimerThread, (LPVOID)this, 0, &id);
	//hTimerThread = (HANDLE)_beginthreadex( NULL, 0, &TimerThread, (LPVOID)this, 0, &id);
	m_hTimerThread = AfxBeginThread( TimerThread, (LPVOID)this );

	//if(SErr(NULL == hTimerThread, _T("CreateTimerThread"))) return FALSE;
	//DWORD result = SetThreadAffinityMask (hTimerThread, dwThreadAffinityMask);
	//if(result == 0) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	Create Event Timer Thread 
//
BOOL COM::CreateEventTimerThread()
{
	AfxBeginThread(EventTimerThread, (LPVOID)this );

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	秦寸 粮甫 茫酒 朝揪, 矫埃甫 舅妨霖促.
//
void COM::SetWeatherZone(int iZone, BYTE tType, BYTE tOnOff)
{
	// alisia
	BOOL bFound = FALSE;
	int iZoneIndex = 0,i;

	for( i = 0; i < g_cell.GetSize(); i++ )
	{
		if( g_cell[i] )
		{
			if( g_cell[i]->m_Zone == iZone )
			{
				bFound = TRUE;
				iZoneIndex = i;
				break;
			}
		}
	}

	if( !bFound ) return;
	// alisia

	USER *pUser1 = NULL;
	USER *pUser2 = NULL;

	for( i = 0; i < MAX_USER; i += 2 )
	{
		pUser1 = g_cell[iZoneIndex]->m_pCom->GetUserUid(i);
		pUser2 = g_cell[iZoneIndex]->m_pCom->GetUserUid(i+1);

		if(pUser1 != NULL && pUser1->m_state == STATE_GAMESTARTED && pUser1->m_curz == iZone) pUser1->SetWeather(tType, tOnOff);
		if(pUser2 != NULL && pUser2->m_state == STATE_GAMESTARTED && pUser2->m_curz == iZone) pUser2->SetWeather(tType, tOnOff);

		pUser1 = NULL;
		pUser2 = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////
//	立加茄 葛电 蜡历俊霸 矫埃阑 舅妨霖促.
//
void COM::SendTimeToAll()
{
	BYTE tType = 1;

	USER *pUser = NULL;

	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		
		pUser->SetWeather(tType, 1);
	}
}

////////////////////////////////////////////////////////////////////////
// CPU狼 肮荐甫 备茄促.
//
void COM::GetCPUCount()
{
	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	m_dwConcurrency = SystemInfo.dwNumberOfProcessors;
}

////////////////////////////////////////////////////////////////////////
//	Send Data
//
void COM::Send(SEND_DATA *pData)
{
	USER*	pUser = NULL;
	MAP*	pMap = NULL;
	int		min_x, max_x, min_y, max_y;
	int		sx, sy;
	int		i, j;
	int		temp_uid;

	if( !pData )
	{
		return;
	}

	if( !pData->len || pData->len <= 0 || pData->len >= RECV_BUF_SIZE )
	{
		TRACE("Data Length OverFlow - Send(SEND_DATA *pData)\n");
		return;
	}

	switch( pData->flag )
	{
	case	SEND_USER:
		if( pData->uid < 0 || pData->uid >= MAX_USER )
		{
			return;
		}

		pUser = GetUserUid(pData->uid);
		if ( pUser == NULL ) return;

		if( pUser->m_state == STATE_DISCONNECTED || pUser->m_state == STATE_LOGOUT )
		{
			return;
		}

		Send( pUser, pData->pBuf, pData->len );
		break;
		
	case	SEND_INSIGHT:
		sx = pData->x / SIGHT_SIZE_X;
		sy = pData->y / SIGHT_SIZE_Y;
		
		min_x = (sx-1)*SIGHT_SIZE_X; if( min_x < 0 ) min_x = 0;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y; if( min_y < 0 ) min_y = 0;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		
		pMap = g_zone[pData->zone_index];
		if( !pMap ) return;
		
		if( max_x >= pMap->m_sizeMap.cx ) max_x = pMap->m_sizeMap.cx - 1;
		if( max_y >= pMap->m_sizeMap.cy ) max_y = pMap->m_sizeMap.cy - 1;
		
		for( i = min_x; i < max_x; i++ )
		{
			for( j = min_y; j < max_y; j++ )
			{				
				temp_uid = pMap->m_pMap[i][j].m_lUser;
				if(temp_uid < USER_BAND || temp_uid >= NPC_BAND) continue;
				else temp_uid -= USER_BAND;
				
				if( temp_uid >= 0 && temp_uid < MAX_USER )
				{
					pUser = GetUserUid(temp_uid);
					if ( pUser == NULL ) continue;
					
					if( pUser->m_state == STATE_GAMESTARTED )
					{
						if( pUser->m_curx == i && pUser->m_cury == j && pUser->m_curz == pData->z )
						{

							Send( pUser, pData->pBuf, pData->len );
						}
					}
				}
			}
		}
		break;
	
	case	SEND_RANGE:
		pMap = g_zone[pData->zone_index];
		if( !pMap ) return;
		
		min_x = pData->rect.left;		if(min_x < 0 ) min_x = 0;
		max_x = pData->rect.right;		if(max_x >= pMap->m_sizeMap.cx ) max_x = pMap->m_sizeMap.cx - 1;
		min_y = pData->rect.top;		if(min_y < 0 ) min_y = 0;
		max_y = pData->rect.bottom;		if(max_y >= pMap->m_sizeMap.cy ) max_y = pMap->m_sizeMap.cy - 1;

		for( i = min_x; i < max_x; i++ )
		{
			for( j = min_y; j < max_y; j++ )
			{
				temp_uid = pMap->m_pMap[i][j].m_lUser;
				if(temp_uid < USER_BAND || temp_uid >= NPC_BAND) continue;
				else temp_uid -= USER_BAND;

				if( temp_uid >= 0 && temp_uid < MAX_USER )
				{
					pUser = GetUserUid(temp_uid);
					// IKING 2001.1.
					if ( pUser == NULL ) continue;
					//
					
					if( pUser->m_state == STATE_GAMESTARTED )
					{
						if( pUser->m_curx == i && pUser->m_cury == j && pUser->m_curz == pData->z )
						{
							Send( pUser, pData->pBuf, pData->len);
						}
					}
				}
			}
		}
		
		break;
		
	case	SEND_ZONE:
		for(i = 0; i < MAX_USER; i++)
		{
			pUser = GetUserUid(i);

			if( !pUser ) continue;
			if( pUser->m_state != STATE_GAMESTARTED )	continue;
			
			if( pData->z == pUser->m_curz )
			{
				if( pUser->m_state == STATE_GAMESTARTED )
				{
					Send( pUser, pData->pBuf, pData->len );
				}
			}
		}
		break;

	case	SEND_ALL:
		for(i = 0; i < MAX_USER; i++)
		{
			pUser = GetUserUid(i);
			
			if( !pUser ) continue;
			if( pUser->m_state != STATE_GAMESTARTED )	continue;
			
			Send( pUser, pData->pBuf, pData->len );
		}
		break;

	case	SEND_SCREEN:
		min_x = pData->x - 16; if( min_x < 0 ) min_x = 0;
		max_x = pData->x + 16;
		min_y = pData->y - 16; if( min_y < 0 ) min_y = 0;
		max_y = pData->y + 16;
		
		pMap = g_zone[pData->zone_index];
		if( !pMap ) return;
		
		if( max_x >= pMap->m_sizeMap.cx ) max_x = pMap->m_sizeMap.cx - 1;
		if( max_y >= pMap->m_sizeMap.cy ) max_y = pMap->m_sizeMap.cy - 1;
		
		for( i = min_x; i < max_x; i++ )
		{
			for( j = min_y; j < max_y; j++ )
			{
				if( ( abs( pData->x - i ) + abs( pData->y - j ) ) > 16 ) continue;

				temp_uid = pMap->m_pMap[i][j].m_lUser;

				if(temp_uid < USER_BAND || temp_uid >= NPC_BAND) continue;
				else temp_uid -= USER_BAND;
				
				if( temp_uid >= 0 && temp_uid < MAX_USER )
				{
					pUser = GetUserUid(temp_uid);
					if ( pUser == NULL ) continue;
					
					if( pUser->m_state == STATE_GAMESTARTED )
					{
						Send( pUser, pData->pBuf, pData->len );
					}
				}
			}
		}
		break;

	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////
//	Send Data
//
void COM::Send(USER *pUser, TCHAR *pBuf, int nLength)
{
	if(pUser == NULL) return;

	pUser->Send(pBuf, nLength);
}
//向地面物品数组添加一个物品
BOOL COM::SetThrowItem(ItemList *pItem, int x, int y, int z)
{
	if( !pItem )
	{
		return FALSE;
	}

	MAP* pAddMap = g_zone[z];

	if( !pAddMap ) return FALSE;
	if( pItem->tType != TYPE_MONEY && pItem->tType != TYPE_ITEM) return FALSE;

	CPoint t = ConvertToClient( x, y, pAddMap->m_vMoveCell.m_vDim.cx, pAddMap->m_vMoveCell.m_vDim.cy );
	if( t.x == -1 || t.y == -1 ) return FALSE;

	/* IKING 2001.1.
	EnterCriticalSection( &m_critThrowItem );

	m_ThrowItemArray[m_ThrowAddIndex]->m_pItem = pItem;
	m_ThrowItemArray[m_ThrowAddIndex]->m_z = z;			// 开矫 粮锅龋啊 酒聪绊 Zone Index 捞促
	m_ThrowItemArray[m_ThrowAddIndex]->m_x = x;
	m_ThrowItemArray[m_ThrowAddIndex]->m_y = y;

	pAddMap->m_pMap[x][y].iIndex = m_ThrowAddIndex;

	m_ThrowAddIndex++;
	if( m_ThrowAddIndex >= MAX_THROW_ITEM ) 
		m_ThrowAddIndex = 0;

	LeaveCriticalSection( &m_critThrowItem );
*/
	if(ThrowItemAdd(pItem, x,y,z)==false)
		return false;

	int index = 0;
	char temp_send[1024];

	SetByte( temp_send, FIELD_ITEM_INFO, index );
	SetShort( temp_send, 1, index );
	SetByte( temp_send, ITEM_INFO_MODIFY, index );

	SetShort( temp_send, t.x, index );
	SetShort( temp_send, t.y, index );

	if( pItem->tType == TYPE_MONEY )
	{
		SetShort( temp_send, TYPE_MONEY_SID, index );
		SetDWORD( temp_send, pItem->dwMoney, index );
		SetByte( temp_send, 0, index);
	}
	else
	{
		SetShort( temp_send, pItem->sSid, index );
		SetDWORD( temp_send, pItem->sCount, index );
		SetByte( temp_send, pItem->tIQ, index);
        SetByte( temp_send, pItem->tMagic[0], index);
		SetByte( temp_send, pItem->tMagic[1], index);
		SetByte( temp_send, pItem->tMagic[2], index);
		SetByte( temp_send, pItem->tMagic[3], index);
		SetByte( temp_send, pItem->tMagic[4], index);

	}

	SEND_DATA* pNewData = NULL;

	pNewData = new SEND_DATA;

	if( !pNewData ) return TRUE;

	pNewData->flag = SEND_INSIGHT;
	pNewData->len = index;

	memcpy( pNewData->pBuf, temp_send, index );

	pNewData->uid = 0;
	pNewData->x = x;
	pNewData->y = y;
	pNewData->z = pAddMap->m_Zone;
	pNewData->zone_index = z;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_critSendData) );
	//m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_critSendData) );
	//PostQueuedCompletionStatus( m_hSendIOCP, 0, 0, NULL );
	Send(pNewData);
	delete pNewData;
	//

	return TRUE;
}
//删除物品
void COM::DelThrowItem()
{
	ItemList* pThrowItem = NULL;

	int z, x, y;

	MAP* pDelMap = NULL;

	// IKING 2001.1.
	EnterCriticalSection( &m_critThrowItem );
	pThrowItem = m_ThrowItemArray[m_ThrowAddIndex]->m_pItem;
	//

	if( pThrowItem )
	{
		z = m_ThrowItemArray[m_ThrowAddIndex]->m_z;		// 粮锅龋啊 酒聪绊 Zone Index 捞促
		x = m_ThrowItemArray[m_ThrowAddIndex]->m_x;
		y = m_ThrowItemArray[m_ThrowAddIndex]->m_y;

		// IKING 
		if(z < 0 || z >= g_zone.GetSize())
		{
			LeaveCriticalSection( &m_critThrowItem );
			return;
		}
		
		pDelMap = g_zone[z];

		if( pDelMap )
		{
			if(x <= -1 || y <= -1)
			{
				LeaveCriticalSection( &m_critThrowItem );
				return;
			}

			if(x >= pDelMap->m_sizeMap.cx || y >= pDelMap->m_sizeMap.cy)
			{
				LeaveCriticalSection( &m_critThrowItem );
				return;
			}

			CPoint t = ConvertToClient( x, y, pDelMap->m_vMoveCell.m_vDim.cx, pDelMap->m_vMoveCell.m_vDim.cy );
			if( t.x == -1 || t.y == -1 )
			{
				LeaveCriticalSection( &m_critThrowItem );
				return;
			}

			pDelMap->m_pMap[x][y].iIndex = -1;
			m_ThrowItemArray[m_ThrowAddIndex]->m_pItem = NULL;

			int index = 0;
			char temp_send[1024];

			SetByte( temp_send, FIELD_ITEM_INFO, index );
			SetShort( temp_send, 1, index );
			SetByte( temp_send, ITEM_INFO_DELETE, index );

			SetShort( temp_send, t.x, index );
			SetShort( temp_send, t.y, index );

			if(pThrowItem->tType == TYPE_ITEM)
			{
				SetShort( temp_send, pThrowItem->sSid, index );
				SetDWORD( temp_send, pThrowItem->sCount, index );
				SetByte( temp_send, pThrowItem->tIQ, index);
			}
			else
			{
				SetShort( temp_send, TYPE_MONEY_SID, index );
				SetDWORD( temp_send, pThrowItem->dwMoney, index );
				SetByte( temp_send, 0, index);
			}

			delete pThrowItem;

			LeaveCriticalSection( &m_critThrowItem );

			SEND_DATA* pNewData = NULL;

			pNewData = new SEND_DATA;

			if( !pNewData ) return;

			pNewData->flag = SEND_INSIGHT;
			pNewData->len = index;

			memcpy( pNewData->pBuf, temp_send, index );

			pNewData->uid = 0;
			pNewData->x = x;
			pNewData->y = y;
			pNewData->z = pDelMap->m_Zone;
			pNewData->zone_index = z;

			//EnterCriticalSection( &(m_critSendData) );
			//m_arSendData.Add( pNewData );
			//LeaveCriticalSection( &(m_critSendData) );
			//PostQueuedCompletionStatus( m_hSendIOCP, 0, 0, NULL );
			Send(pNewData);
			delete pNewData;

		}
	}
	else
		LeaveCriticalSection( &m_critThrowItem );

	// 瘤陛 眠啊且 镑俊 酒捞袍捞 绝带啊, 酒捞袍阑 瘤快促 角菩茄 版快捞促
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	鞘靛惑痢俊 措茄 辨靛傈 矫埃阑 眉农窍绊 林牢捞 乐绰 惑痢俊 措茄 技陛阑 辨靛芒绊肺 利赋茄促.
//
void COM::CheckGuildWarTime()
{
	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);
	DWORD dwCurrTick = GetTickCount();					// 力茄 矫埃郴狼 辨靛傈阑 眉农...

	COleDateTime CurrTime = COleDateTime(guildTime);
//	SYSTEMTIME st;

	int i, j;
	int nTime = 0;
	static int nState = 0;
	CString strMsg;

	DWORD dwTick = 0, dwInter = 0;
	DWORD dwInterTime = 0;	

	for(i = 0; i < MAX_SINGLE_EVENT; i++)
	{
		if(g_QuestEventZone.m_SingleEventZone[i].m_lUsed == 1)
		{			
			dwTick = dwInter = dwInterTime = 0;
			dwTick = g_QuestEventZone.m_SingleEventZone[i].m_dwStartTime;
			dwInterTime = (DWORD)g_QuestEventZone.m_SingleEventZone[i].m_tEventTime;
			dwInter = g_QuestEventZone.m_SingleEventZone[i].m_dwInterTick;

			if(dwCurrTick - dwTick > 60000 * dwInterTime)
			{
				g_QuestEventZone.RemoveUserInEventZone(i);
				g_QuestEventZone.m_SingleEventZone[i].m_lUsed = 0;
			}
			else if(dwInter != 0 && dwCurrTick - dwInter > 10000)		// 10檬饶 磊悼栏肺 炮 抛胶飘
			{
				g_QuestEventZone.RemoveUserInEventZone(i);
				g_QuestEventZone.m_SingleEventZone[i].m_lUsed = 0;
				g_QuestEventZone.m_SingleEventZone[i].m_dwInterTick = 0;
			}
		}
	}

	for(j = 0; j < g_arGuildHouseWar.GetSize(); j++)
	{
		if(!g_arGuildHouseWar[j]) continue;

		if(g_arGuildHouseWar[j]->m_CurrentGuild.lUsed == 1)
		{
			DWORD dwTempTick = g_arGuildHouseWar[j]->m_CurrentGuild.dwTimer;
			DWORD dwInterTick = g_arGuildHouseWar[j]->m_CurrentGuild.dwIntervalTick;
			if(dwTempTick != 0 && dwCurrTick - dwTempTick > 60000 * 10)	
			{
				g_arGuildHouseWar[j]->CheckGuildHouseRank();					// Agent俊 历厘茄促.

				g_arGuildHouseWar[j]->SetNpcListToWarEnd();
				//g_arGuildHouseWar[j]->SetUserListToWarEnd(user_array);
				g_arGuildHouseWar[j]->SetUserListToWarEnd();
				
				g_arGuildHouseWar[j]->InitGuild();

				g_arGuildHouseWar[j]->m_CurrentGuild.dwIntervalTick = 0;
				InterlockedExchange(&g_arGuildHouseWar[j]->m_CurrentGuild.lUsed, 0);
			}
			else if(dwInterTick != 0 && dwCurrTick - dwInterTick > 10000)		// 10檬饶 磊悼栏肺 炮 抛胶飘
			{
				g_arGuildHouseWar[j]->m_CurrentGuild.dwIntervalTick = 0;

//				g_arGuildHouseWar[j]->CheckGuildHouseRank();					// Agent俊 历厘茄促.
																				// 荤侩吝牢 钎矫甫 钱绢霖促.			
//				g_arGuildHouseWar[j]->SetNpcListToWarEnd();
				//g_arGuildHouseWar[j]->SetUserListToWarEnd(user_array);		// 悸泼甫 钱绢霖促.
				g_arGuildHouseWar[j]->SetUserListToWarEnd();					// 悸泼甫 钱绢霖促.

				g_arGuildHouseWar[j]->InitGuild();								// 单捞磐甫 朝赴促.

				::InterlockedExchange(&g_arGuildHouseWar[j]->m_CurrentGuild.lUsed, 0); 
			}
		}

		if(CurrTime.GetDay() == GUILD_HOUSE_DAY)								// 辨靛窍快胶 盒剧 朝楼
		{
			if(CurrTime.GetHour() == 0)											// 辨靛窍快胶 盒剧 矫埃
			{
				if(CurrTime.GetMinute() == 20 && !nState)
				{
					nState = 1;

					for(i = 0; i < MAX_GUILD_HOUSE_NUM - 1; i++)				// 瘤陛 辨.窍啊 6俺挥捞扼 捞吧 DB俊 历厘窍妨搁 茄波锅俊秦具窍唱?
					{
						if(!g_arGuildHouse[i]) continue;
						if(InterlockedCompareExchange((long*)&g_arGuildHouse[i]->lUsed, (long)1, (long)0) == (long)0)
						{
							g_arGuildHouse[i]->iSid = i + 1;
							g_arGuildHouse[i]->iGuild = g_arGuildHouseWar[j]->m_TopList[i].lGuild;
							g_arGuildHouse[i]->UpdateGuildHouse();	
							
							::InterlockedExchange(&g_arGuildHouse[i]->lUsed, 0); 
						}
					}

					g_arGuildHouseWar[j]->ResetGuildHouseRank();

					RemoveUserInGuildHouse();
				}
			}
		}
	}
}

void COM::SendGuildWarBegin(CStore *pStore)
{
	if(pStore == NULL) return;
	UpdateStoreMem2DB(pStore);

	int i, j, k, type;
	USER *pUser = NULL;

	BOOL bSend = FALSE;

	CBufferEx TempBuf;
	CDWordArray arGuildList;
	arGuildList.RemoveAll();

	if(pStore->m_iGuildSid <= 0) return;				// 辨靛傈阑困秦 曼咯吝牢 傈眉 府胶飘甫 父电促.
	arGuildList.Add(pStore->m_iGuildSid);				// 郴辨靛 刚历...

	for(j =0; j < GUILD_ATTACK_MAX_NUM; j++)			// 惑措祈 辨靛...
	{
		if(pStore->m_arAttackGuild[j] > 0)
		{
			arGuildList.Add(pStore->m_arAttackGuild[j]);
		}
	}

	CString strMsg;
	CGuild *pGuild = NULL;
	for(i = 0; i < arGuildList.GetSize(); i++)			// 惑痢 傈里傈 鞘靛傈捞 乐栏搁 檬扁拳 秦霖促.
	{
		j = arGuildList[i];
		if(j <= 0 || j >= g_arGuildData.GetSize()) continue;
		
		pGuild = g_arGuildData[j];
		if(!pGuild) continue;

		pUser = GetUser(pGuild->m_strMasterName);
		if(!pUser) continue;

		if(pUser->m_tGuildWar == GUILD_WARRING && pUser->m_dwFieldWar > 0)
		{
			strMsg = _ID(IDS_USER_DRAW);
			pUser->SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// 亲汗
		}
	}

	short nCount = arGuildList.GetSize();

	if(nCount <= 1) 
	{
		strMsg = _T("");
		strMsg.Format( IDS_USER_END_NO_APPLY_GUILD, pStore->m_strGuildName);
		Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL);
		pStore->m_dwStartTick = 0;
		return;
	}

	TempBuf.Add(GUILD_WAR);
	TempBuf.Add((BYTE)0x01);							// 辨靛傈 矫累...
	TempBuf.Add((BYTE)GUILD_STORE_WARRING);					// 惑痢傈...

	TempBuf.Add((short)pStore->m_sStoreID);			// 秦寸 瘤开 牢郸胶
/*	TempBuf.Add(nCount);								// 醚 辨靛荐

	for(i = 0; i < nCount; i++)
	{
		TempBuf.Add(arGuildList[i]);
	}
*/
	for(i = 0; i < MAX_USER; i++)
	{
		type = 0;
		pUser = GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;		
		if(pUser->m_dwGuild <= 0) continue;
		
		type = ((g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_dwType & 0xFF00 ) >> 8);

//		if(g_arMapTable[type]->m_sStoreID != pStore->m_sStoreID) continue;
		if(g_arMapTable[type] == NULL) continue;
		if(type >= 0 && type < 8)
		{															 //辨靛傈捞 老绢唱绰 惑痢瘤开捞搁
			if(g_arMapTable[type]->m_sStoreID == pStore->m_sStoreID)// && g_arMapTable[type]->m_sStoreZone)		
			{																	 
				if(pStore->m_iGuildSid != pUser->m_dwGuild && pUser->m_tIsOP != 1 ) pUser->TownPotal(); // 规绢辨靛盔捞 酒聪搁 葛滴 付阑肺
			}
		}
		
		if(pStore->m_iGuildSid <= 0)	
		{  
			TempBuf.Add((BYTE)0x00);					// 老馆蜡历
			pUser->Send(TempBuf, TempBuf.GetLength());
			continue; 
		}

		if(pStore->m_iGuildSid == pUser->m_dwGuild) 
		{
			pUser->BeginGuildWar();												 // 辨靛 傈阑 矫累茄促.

			TempBuf.Add((BYTE)0x01);			// 秦寸 辨靛 蜡历
			TempBuf.Add(nCount);				// 醚 辨靛荐

			for(j = 0; j < nCount; j++)
			{
				TempBuf.Add(arGuildList[j]);
			}
			pUser->Send(TempBuf, TempBuf.GetLength());
		}
		else
		{
			for(j =0; j < GUILD_ATTACK_MAX_NUM; j++)
			{
				if(pStore->m_arAttackGuild[j] <= 0) continue;

				if(pStore->m_arAttackGuild[j] == pUser->m_dwGuild)
				{
					pUser->BeginGuildWar();			// 辨靛 傈阑 矫累茄促.

					TempBuf.Add((BYTE)0x01);		// 秦寸 辨靛 蜡历
					TempBuf.Add(nCount);			// 醚 辨靛荐
					for(k = 0; k < nCount; k++)
					{
						TempBuf.Add(arGuildList[k]);
					}
					bSend = TRUE;
					pUser->Send(TempBuf, TempBuf.GetLength());
					break;
				}
			}

			if(!bSend) 
			{
				TempBuf.Add((BYTE)0x00);					// 老馆蜡历
				pUser->Send(TempBuf, TempBuf.GetLength());
			}
		}
	}

	for(j = 0; j < pStore->m_arNpcList.GetSize(); j++) // 秦寸 NPC俊霸档 舅府绊
	{													
		type = pStore->m_arNpcList[j];

		if( type < 0 || type >= g_arNpc.GetSize() ) continue;
		g_arNpc[type]->m_tGuildWar = GUILD_WARRING;
		if(g_arNpc[type]->m_tNpcType == NPCTYPE_GUILD_GUARD) g_arNpc[type]->m_tNpcAttType = 1;
	}
}

void COM::SendGuildWarEnd(CStore *pStore)
{	
	int i, j, type;

	int index = 0;

	USER *pUser = NULL;
	if(pStore == NULL) return;

	if(pStore->UpdateGuildStore() == FALSE) return;

	pStore->InitStoreInfo(pStore->m_iGuildSid);
	pStore->SetGuildStoreTex();

	CBufferEx TempBuf;

	TempBuf.Add(GUILD_WAR);
	TempBuf.Add((BYTE)0x02);							// 辨靛傈 辆丰...
	TempBuf.Add(GUILD_STORE_WARRING);
	TempBuf.Add((short)pStore->m_sStoreID);

	for(i = 0; i < MAX_USER; i++)
	{
		pUser = GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		if(pUser->m_dwGuild <= 0) continue;

		pUser->m_tGuildWar = GUILD_WAR_AFFTER;
		pUser->m_FieldWarGMUid = 0;
		pUser->m_dwFieldWar = 0;

		pUser->Send(TempBuf, TempBuf.GetLength());

		type = pUser->CheckInvalidMapType();
		if(type >= 0 && type < 17)
		{
			if(g_arMapTable[type] == NULL) continue;
			index = g_arMapTable[type]->m_sStoreID;

			if(index < FORTRESS_BAND)
			{
				CStore *pStore = NULL;

				index = g_arMapTable[type]->m_sStoreIndex;
				pStore = pUser->GetStore(index);
				if(pStore)
				{
					pUser->SendSystemMsg( IDS_USER_END_FIELD_STORE_WAR, SYSTEM_NORMAL, TO_ME);
				}
			}
		}
	}

	for(j = 0; j < pStore->m_arNpcList.GetSize(); j++) // 秦寸 NPC俊霸档 舅府绊
	{													
		type = pStore->m_arNpcList[j];

		if( type < 0 || type >= g_arNpc.GetSize() ) continue;
		g_arNpc[type]->m_tGuildWar = GUILD_WAR_AFFTER;
		g_arNpc[type]->m_tNpcAttType = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//	辨靛傈捞 矫累登扁傈俊 辨靛芒绊狼 郴侩阑 DB肺 诀单捞飘 茄促.
//
void COM::UpdateStoreMem2DB(CStore *pStore)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];
	TCHAR			strAttackList[_GUILDLIST_DB];
	int				i;
	
	CSharedMemory* pShared = NULL;
	CMemStore* pMDStore = NULL;

	if(pStore == NULL) return;
	if(pStore->m_sStoreID >= g_arStoreSharedMemory.GetSize()) return;

	pShared = g_arStoreSharedMemory[pStore->m_sStoreID];
	
	if(pShared == NULL) return;
	if(pShared->m_hMapping == NULL) return;
	
	pMDStore = (CMemStore*) pShared->m_lpData;
	
	if(pMDStore == NULL) return;
	if(pMDStore->m_sStoreID == -1) return;
	if(pMDStore->m_iGuildSid <= 0) return;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strAttackList, sizeof(strAttackList));
	
	pStore->GuildListToStr(strAttackList);

	SDWORD sLen	= sizeof(strAttackList);

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_STORE(%d, %d, %d, %d, %d, ?)}"), 
		pMDStore->m_sStoreID, pMDStore->m_iGuildSid, pMDStore->m_sTaxRate, pMDStore->m_dwStoreDN, 
		pMDStore->m_tWarType);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strAttackList), 0, (TCHAR*)strAttackList, 0, &sLen);

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			bQuerySuccess = FALSE;
		}
	}
	else
	{
//		if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

	pMDStore->m_dwStoreDN = 0;
}

void COM::CheckGuildWarSchedule(CStore *pStore)
{
	if(pStore == NULL) return;

	int nSize = g_arStoreSharedMemory.GetSize();
	CSharedMemory* pShared = NULL;
	CMemStore* pData = NULL;

	if(pStore->m_sStoreID >= nSize) return;

	pShared = g_arStoreSharedMemory[pStore->m_sStoreID];
	
	if(pShared == NULL) return;
	if(pShared->m_hMapping == NULL) return;
	
	pData = (CMemStore*) pShared->m_lpData;
	
	if(pData == NULL) return;
	
	pStore->m_tWarType = GUILD_WAR_DECISION;
	pData->m_tWarType = GUILD_WAR_DECISION;
}

void COM::Announce(TCHAR *pBuf, BYTE sysType)
{
	CBufferEx TempBuf;

	TempBuf.Add(SYSTEM_MSG);			
	TempBuf.Add(sysType);
	TempBuf.Add(pBuf, _tcslen(pBuf));

	USER* pUser = NULL;

	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
        
		pUser->Send(TempBuf, TempBuf.GetLength());
	}
}
void COM::Announce2(TCHAR *pBuf, BYTE sysType) //关闭全服评分显示
{
	CBufferEx TempBuf;

	TempBuf.Add(SYSTEM_MSG);			
	TempBuf.Add(sysType);
	TempBuf.Add(pBuf, _tcslen(pBuf));

	USER* pUser = NULL;

	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
        if(pUser->m_GB == 0) continue;
		pUser->Send(TempBuf, TempBuf.GetLength());
	}
}

void COM::AnnounceZone(TCHAR *pBuf, BYTE sysType, int iZone)
{
	CBufferEx TempBuf;

	TempBuf.Add(SYSTEM_MSG);			
	TempBuf.Add(sysType);
	TempBuf.Add(pBuf, _tcslen(pBuf));

	USER* pUser = NULL;

	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = GetUserUid(i);

		if( pUser == NULL || pUser->m_state != STATE_GAMESTARTED ) continue;
		if( pUser->m_curz != iZone ) continue;

		pUser->Send(TempBuf, TempBuf.GetLength());
	}
}

USER* COM::GetUser(TCHAR* id)
{
	if(!strlen(id)) return NULL;
	
	CString szSource = id;
	USER *pUser = NULL;

	for( int i = 0; i < MAX_USER; i++)
	{
		pUser = GetUserUid(i);
		if(pUser == NULL) continue;

		if( pUser->m_state == STATE_DISCONNECTED || pUser->m_state == STATE_LOGOUT ) continue;

		if(!szSource.CompareNoCase(pUser->m_strUserID ) ) return pUser;
	}

	return NULL;
}

USER* COM::GetUserUid(int uid)
{
	USER *pUser = NULL;

	if (uid >= 0 && uid < MAXPOSSIBLE_USER )
	{
		pUser = g_pUserList->GetUserUid(uid);
/*		if ( pUser )
		{
			if ( pUser->m_SockFlag != 1 )
			{
				g_pMainDlg->UserFree( uid );
				return NULL;
			}
		}
*/		return pUser;
	}

	return NULL;
}

void COM::UpdateUserCount()
{
	int nCount = 0;
	USER *pUser = NULL;
	nCount = 0;

	for (int i = 0; i < MAX_USER; i++ )
	{
		pUser = GetUserUid(i);
		if( pUser && pUser->m_state == STATE_GAMESTARTED ) nCount++;
	}

	if(nCount >= 500) nCount = (int)((double)nCount * 1.1 + 0.5);


	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UpdateGameUserCount ( \'%s\', %d )}"), m_strIP, nCount );
	
	hstmt = NULL;

	int db_index = -1;
	CDatabase* pDB = g_DBSession[0].GetDB( db_index );
	if( !pDB ) return;
	
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
		return;
	}

	retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
	if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
	}
	else if (retcode==SQL_ERROR)
	{
//		DisplayErrorMsg(hstmt);
	}
	else if (retcode==SQL_NO_DATA)
	{
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

	g_DBSession[0].ReleaseDB(db_index);

	return;
}
////////////////////////////离线更新数据
void COM::UpdateUnLineUserTime()									//
{
	for(int i = 0; i < MAX_USER; i++)
	{
		USER *pUser = GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		if(pUser->m_bSessionOnline)
		{
			pUser->UserTimer();
		}
	}
}

////尾巴

void COM::ShowCurrentUser2()
{
   /* int nCount = 0;
	USER *pUser = NULL;
	nCount = 0;
	for (int i = 0; i < MAX_USER; i++ )
	{
		pUser = GetUserUid(i);
		if(pUser && pUser->m_state == STATE_GAMESTARTED ) nCount++;
	}

	if(nCount >= 500) nCount = (int)((double)nCount * 1.1 + 0.5);
	
    CString strMsg;
	strMsg = _T("");
	strMsg.Format( "当前在线人数 %d"  ,nCount*3);
	Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL);*/
}
void COM::CheckCurrentUserTime() 
{
	SYSTEMTIME messageTime;
	GetLocalTime(&messageTime);
	
	DWORD dwCurrTick = GetTickCount();					

	COleDateTime CurrTime = COleDateTime(messageTime); 
	int nTime = 0;
	static int nState = 0;
	if( messageTime.wMinute%5==0 && messageTime.wSecond ==0) 
	{
		ShowCurrentUser2();
	}
}
void COM::CheckMessageTime() //公告
{
	SYSTEMTIME messageTime;
	GetLocalTime(&messageTime);
	
	DWORD dwCurrTick = GetTickCount();					// 力茄 矫埃郴狼 辨靛傈阑 眉农...

	COleDateTime CurrTime = COleDateTime(messageTime); //当时间
	
	int nTime = 0;
	static int nState = 0;
	if ( messageTime.wHour == 20)  //活动期间不提示
		return;

	
/*	if(messageTime.wSecond%5==0 )
	{
		int dgnumer = g_arDGArray.GetSize();
		if(dgnumer > 0)
		{		
			CBufferEx	TempBuf;
			TempBuf.Add((byte)0x1f);
			TempBuf.Add((byte)1);
			TempBuf.Add((byte)0x25);
			TempBuf.Add(0);					
			TempBuf.AddString(g_arDGArray[0]->m_strUserID);	
			TempBuf.AddString(g_arDGArray[0]->m_DGBAN_BODY);
			SendAll(TempBuf, TempBuf.GetLength());
			g_arDGArray.RemoveAt(0);			
		}
	}*/
/*	if( messageTime.wMinute%5==0 && messageTime.wSecond ==0) 
	{
		CString strMsg;
		//const char p[] = { 0xCE, 0xC2, 0xF3, 0xC0, 0xCC, 0xE1, 0xCA, 0xBE, 0x3A, 0x00};
		//strMsg.Format(strcat((char*)p,"%s"), g_arMessAgeArray[mIndex]->m_MESSAGE_BODY);
		strMsg.Format("%s", g_arMessAgeArray[mIndex]->m_MESSAGE_BODY);
		Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);		
		mIndex++;
		if(mIndex >= g_arMessAgeArray.GetSize() -1) mIndex = 0;	
	}
	SetHourExp(1);*/
	
	if(messageTime.wHour == 23 && messageTime.wMinute == 59 && messageTime.wSecond == 59)  // 清理
	{
		SubQLPKDASAITime();
		PersonalShopOpen1();
	}
//==============================================================================================================================
	if(messageTime.wSecond % 10 == 0)//排名
	{
		for(int i = 0; i < MAX_USER; i++)
		{
			if(i < 0) i = 0;
			USER *pUser = NULL;
			pUser = GetUserUid(i);
			if(pUser != NULL && pUser->m_state == STATE_GAMESTARTED)
			{
				if(pUser->m_dwPD > 0)
				{
					pUser->m_dwPD = 0;
					CString str;
                    str.Format("%s",pUser->m_strLoveName);
                    int pos = str.Find("]");
					if(pos > 0){ str = str.Mid(pos + 1,str.GetLength());}
					::ZeroMemory(pUser->m_strLoveName,sizeof(pUser->m_strLoveName));
                    sprintf(pUser->m_strLoveName,"@9[%d]%s",pUser->m_dwPD,str);
					//strcpy(pUser->m_strLoveName,"");
					CBufferEx TempBuf;
					TempBuf.Add(LOVE_NAME);
					TempBuf.Add(pUser->m_uid + USER_BAND);
					TempBuf.AddString(pUser->m_strLoveName);
					pUser->SendExactScreen(TempBuf, TempBuf.GetLength());
				}
			}
		}
		GetUserTopTime();
		GetUserPaiMing();
	}
//==============================================================================================================================
}

void COM::PersonalShopOpen1()
{
	USER *pUser = NULL;
	int i;
	
	for(i = 0; i < MAX_USER; i++)
	{
		pUser = g_pUserList->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		if( pUser->m_dwZaiXianTime > 1)
		{
			pUser->m_dwZaiXianTime -= 1;
		}else if(pUser->m_dwZaiXianTime == 1)
		{
			pUser->m_dwZaiXianTime =0;
			pUser->SendEventMsg("您的会员时间已到期");
		}
		pUser->m_dwLingQu = 1;
		pUser->m_dwShaGuai = 0;
		pUser->SetUserToMagicUser();
		pUser->CheckMagicItemMove();
		pUser->UpdateUserData();
		CString strMsg;
		strMsg = _T("");
		strMsg.Format( "友情提示：现在已是凌晨0点,请注意休息!");
		Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
	}
}
//////////////////////////////////////
void COM::SubQLPKDASAITime()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	TCHAR			szSQL[1024];		

	::ZeroMemory(szSQL, sizeof(szSQL));
	int index = 0;
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_PK_QL}")); 

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return;
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return;
	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
		}
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
}
void COM::CheckPKTime()		//PK竞赛
{
	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);
	DWORD dwCurrTick = GetTickCount();					// 力茄 矫埃郴狼 辨靛傈阑 眉农...

	COleDateTime CurrTime = COleDateTime(guildTime);

	int nTime = 0;
	CString strMsg;


	PKnum = 0;//开始前把人数初始化
	PKover = FALSE;//设置PK没结束
     
	if(o_yehuoini[0]->PKDSKG == 1)
	{
     
	if(guildTime.wHour == 19 && guildTime.wMinute>= 55)	// 19点45进行公告!
	{
		nTime = 60 - guildTime.wMinute;
		if(guildTime.wSecond == 0)				//一分钟公告一次
		{
			strMsg.Format( "PK王大赛还有 %d 分钟将会开放进入,20:05时将会关闭,请大家抓紧进入!", nTime);//"恶魔广场还有 %d 分钟将会开放,请大家准备就绪!"
			Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
		}
	}
	if(guildTime.wHour == 20 && guildTime.wMinute>= 0 && guildTime.wMinute <= 4)	// PK开启前准备
	{
		winName ="";
		if(guildTime.wSecond == 0)
		{
			strMsg.Format( "各位勇士紧急做好PK准备.20:05分比赛正式开启,当前时间是:%d:0%d", guildTime.wHour,guildTime.wMinute);
			Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
		}
	}
	if(guildTime.wHour == 20 && guildTime.wMinute == 5)	// 厮杀开始
	{
		if(guildTime.wSecond  >=0 &&  guildTime.wSecond <=10)
		{
			strMsg.Format("PK大赛正式开始,请勇者们开始厮杀吧!!");
			Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
		}
	}
	if(guildTime.wHour >= 20 && guildTime.wMinute > 5 && PKover == FALSE)
	{
		if(guildTime.wSecond % 20 == 0)		//10秒一次更新人数
		{ 
			USER* pUser = NULL;
			for(int i = 0; i < MAX_USER; i++)
			{
				pUser = GetUserUid(i);
				if(pUser && pUser->m_state == STATE_GAMESTARTED && pUser->m_curz == 67 && pUser->m_bLive == USER_LIVE)
					PKnum ++;
			}
			if( PKnum > 1 && guildTime.wMinute < 15)
			{
				strMsg.Format("现在进行激烈的战斗...PK赛场剩余勇士:[ %d ]个.",PKnum);
				Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(PKnum == 1)
			{
				
				for(int i = 0; i < MAX_USER; i++)
				{
					pUser = GetUserUid(i);
					if(pUser && pUser->m_state == STATE_GAMESTARTED && pUser->m_curz == 67 && pUser->m_bLive == USER_LIVE)
					{
						winName = pUser->m_strUserID;
						pUser->m_PKWin = TRUE;
					}
				}
				PKover = TRUE;
			}
		}
	}
	if( PKover == TRUE)
	{
		if (winName.GetLength() > 0)
		{
				strMsg.Format("玩家[ %s ]获得了最终胜利.成为本届PK王并获赠价值100元标志!!!",winName);
				Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			PKover = FALSE;
			o_yehuoini[0]->PKDSKG = 0;


		}
	   }
	}
}
////////////////////////////////////////////////////////////////////
//PK杀人狂
//
void COM::CheckPKShaRenTime()		//PK竞赛
{
	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);
	DWORD dwCurrTick = GetTickCount();					// 力茄 矫埃郴狼 辨靛傈阑 眉农...

	COleDateTime CurrTime = COleDateTime(guildTime);

	int nTime = 0;
	CString strMsg;

	
	

	if(guildTime.wHour == 20 && guildTime.wMinute>= 15 && guildTime.wMinute< 20)	// 20点15分开始公告
	{
		ShaRenEnd = FALSE;
		ShaRenNum = 0; //初始化最大值
		ShaRenName=""; //初始化杀人王名字

		nTime = 20 - guildTime.wMinute;
		if(guildTime.wSecond == 0)				
		{
		//	strMsg.Format( "PK杀人狂大赛还有 %d 分钟将会开放进入,杀人最多就是胜利!", nTime);
		//	Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
		}
		else if(guildTime.wSecond == 30)			
		{
		//	strMsg.Format( "为了大家踊跃参加大赛,每杀一个玩家奖励20W游戏币,封顶值为400W,也就是杀20个人以下都奖励.");
		//	Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
		}
		else if(guildTime.wSecond == 50)			
		{
		//	strMsg.Format( "杀人狂大赛的进入地点是菲利普的银行门口.综合服务NPC雷米处选择进入");
		//	Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
		}
	}
	
	
	if(guildTime.wHour == 20 && guildTime.wMinute >= 20 && guildTime.wMinute < 50)		//活动统计中.. 8:20至 8:50分进行大赛
	{
		nTime = 50 - guildTime.wMinute;

		if(guildTime.wSecond % 30==0)		//20秒一次更新人数
		{
			USER* pUser = NULL;
			for(int i = 0; i < MAX_USER; i++)
			{
				pUser = GetUserUid(i);
				if( pUser && pUser->m_state == STATE_GAMESTARTED)
				{
					if(pUser->m_dwAutoMoney > ShaRenNum)  
					{
						ShaRenNum = pUser->m_dwAutoMoney;
						ShaRenName = pUser->m_strUserID;
						
					}
				}
			}
			if( ShaRenName.GetLength() > 0)
			{
			//	strMsg.Format("杀人狂大赛消息:现在杀人数最高的玩家是[ %s ],一共杀了[ %d ]名玩家.离活动结束还有[%d]分钟!",ShaRenName,ShaRenNum,nTime);
			//	Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);

			}
		}
	}
	if(guildTime.wHour == 20 && guildTime.wMinute == 50 && guildTime.wSecond == 5)	
	{
		
		if( ShaRenName.GetLength() > 0 )
		{
		//	strMsg.Format("本届杀人狂第一名玩家是[ %s ],一共杀了[ %d ]名玩家！",ShaRenName,ShaRenNum);
		//	Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			ShaRenEnd = TRUE;
		}
		else
		{
		//	strMsg.Format("本届杀人狂大赛结束!谢谢广大玩家对本游戏的支持!",ShaRenName,ShaRenNum);
		//	Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
		}
	}

}
void COM::CheckFortressWarTime()
{
	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);
	DWORD dwCurrTick = GetTickCount();					// 力茄 矫埃郴狼 辨靛傈阑 眉农...

	COleDateTime CurrTime = COleDateTime(guildTime);
	SYSTEMTIME st;

	int i, j;
	int nTime = 0;
	static int nState = 0;
	CString strMsg;

	CheckViolenceInFortress();

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i]) continue;

		int time = 0;
		CTimeSpan DiffTime;

		if(g_arGuildFortress[i]->m_lUsed == 0) 
		{
			st = g_arGuildFortress[i]->m_wLastWarTime;
			COleDateTime LastTime(st.wYear, st.wMonth, st.wDay, st.wHour, 0, 0);

			if(CurrTime.m_status == COleDateTime::valid && LastTime.m_status == COleDateTime::valid)
			{
				CTime curr(CurrTime.GetYear(), CurrTime.GetMonth(), CurrTime.GetDay(), CurrTime.GetHour(), 0, 0);
				CTime last(LastTime.GetYear(), LastTime.GetMonth(), LastTime.GetDay(), LastTime.GetHour(), 0, 0);
				DiffTime = curr - last;			
			}

			if(g_arGuildFortress[i]->m_iGuildSid < SYSTEM_GUILD_BAND && g_arGuildFortress[i]->m_iGuildSid > 0)		// 惑痢俊 林牢捞 乐绊
			{
				time = (int)DiffTime.GetTotalHours();
				if(time >= UPDATE_GUILD_INVEN_TIME)			//12盒付促
//				time = DiffTime.GetTotalMinutes();
//				if(time >= 5)			//12盒付促
				{
					SetFortressWarTime(g_arGuildFortress[i]);					

					UpdateFortress2DB(g_arGuildFortress[i]);

					g_arGuildFortress[i]->m_iGuildDN = 0;
					GetLocalTime(&g_arGuildFortress[i]->m_wLastWarTime);
				}
			}
		}

		if(g_arGuildFortress[i]->m_lUsed == 0 && g_arGuildFortress[i]->m_tWarType == GUILD_WAR_DECISION)
		{
			if(guildTime.wYear == g_arGuildFortress[i]->m_wPlanWarTime.wYear)
			{
				if(guildTime.wMonth == g_arGuildFortress[i]->m_wPlanWarTime.wMonth)
				{
					if(guildTime.wDay == g_arGuildFortress[i]->m_wPlanWarTime.wDay)
					{
						if(guildTime.wHour + 1 == g_arGuildFortress[i]->m_wPlanWarTime.wHour)
						{
							int tt = g_arGuildFortress[i]->m_iStandingTime;
							if(guildTime.wMinute == g_arGuildFortress[i]->m_iStandingTime)
							{
								nTime = 60 - guildTime.wMinute;
								if(nTime > 0)
								{
									if(g_arGuildFortress[i]->m_sFortressID == 1000)
									{
										strMsg.Format( IDS_USER_START_LUINET_SOON, nTime);
									}
									else if(g_arGuildFortress[i]->m_sFortressID == 1001)
									{
										strMsg.Format( IDS_USER_START_SANAD_SOON, nTime);
									}
									else if(g_arGuildFortress[i]->m_sFortressID == 1002)
									{

										strMsg.Format( IDS_USER_START_ZAMELLYA_SOON, nTime);
									}
									Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
								}
								g_arGuildFortress[i]->m_iStandingTime += 3;
							}
						}

						if(guildTime.wHour == g_arGuildFortress[i]->m_wPlanWarTime.wHour)
						{
							if(guildTime.wMinute == 0 && g_arGuildFortress[i]->m_lUsed == 0)// 辨靛傈 矫累...
							{	
								if(dwCurrTick - g_arGuildFortress[i]->m_dwStartTick < 70000) continue;

								g_arGuildFortress[i]->m_iStandingTime = 50;
								InterlockedExchange(&g_arGuildFortress[i]->m_lUsed, 1);

								g_arGuildFortress[i]->m_dwStartTick = dwCurrTick;
								GetLocalTime(&g_arGuildFortress[i]->m_wLastWarTime);
								SendFortressWarBegin(g_arGuildFortress[i]);//比赛开始
							}
						}
					}
				}
			}
		}
		else if(g_arGuildFortress[i]->m_lUsed == 1) 
		{
			SYSTEMTIME ut;
			ut = g_arGuildFortress[i]->m_wLastWarTime;
			COleDateTime LastTime(ut.wYear, ut.wMonth, ut.wDay, ut.wHour, 0, 0);

			if(dwCurrTick - g_arGuildFortress[i]->m_dwStartTick > 60000 * g_dwFortressTime )	// 60盒 抛胶飘 * 2矫埃
			{				// 辨靛傈捞 场车促.
				InterlockedExchange(&g_arGuildFortress[i]->m_lUsed, 0);
				g_arGuildFortress[i]->m_dwStartTick = dwCurrTick;
				SendFortressWarEnd(g_arGuildFortress[i]);//比赛结束
				GetLocalTime(&g_arGuildFortress[i]->m_wLastWarTime);
			}
			else if(CurrTime.m_status == COleDateTime::valid && LastTime.m_status == COleDateTime::valid)
			{
				CTime curr(CurrTime.GetYear(), CurrTime.GetMonth(), CurrTime.GetDay(), CurrTime.GetHour(), CurrTime.GetMinute(), CurrTime.GetSecond());
				CTime last(LastTime.GetYear(), LastTime.GetMonth(), LastTime.GetDay(), LastTime.GetHour(), LastTime.GetMinute(), LastTime.GetSecond());
				DiffTime = curr - last;	
				
				nTime =(int)( g_dwFortressTime - DiffTime.GetTotalMinutes());

				for(j = 0; j < GUILD_WAR_MAX_MSG_TIMER; j++)
				{
					if( g_arGuildFortress[i]->m_MsgTimer[j].iMsgTime == nTime )
					{
						if(g_arGuildFortress[i]->m_MsgTimer[j].iUsed == 1) break;

						if(g_arGuildFortress[i]->m_sFortressID == 1000)
						{
							strMsg.Format( IDS_USER_END_LUINET_SOON, g_arGuildFortress[i]->m_MsgTimer[j].iMsgTime);
						}
						else if(g_arGuildFortress[i]->m_sFortressID == 1001)
						{
							strMsg.Format( IDS_USER_END_SANAD_SOON, g_arGuildFortress[i]->m_MsgTimer[j].iMsgTime);
						}
						else if(g_arGuildFortress[i]->m_sFortressID == 1002)
						{
							strMsg.Format( IDS_USER_END_ZAMELLYA_SOON, g_arGuildFortress[i]->m_MsgTimer[j].iMsgTime);
						}
						Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
						g_arGuildFortress[i]->m_MsgTimer[j].iUsed = 1;
						break;
					}
				}
			}
		}
	}
}

void COM::CheckDevilTime()		//恶魔广场公告
{
	SYSTEMTIME messageTime;
	GetLocalTime(&messageTime);

	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);
	DWORD dwCurrTick = GetTickCount();					// 力茄 矫埃郴狼 辨靛傈阑 眉农...

	COleDateTime CurrTime = COleDateTime(guildTime);

	int nTime = 0;
	CString strMsg;
	                if(o_yehuoini[0]->EMGCKG == 1)
	                 {


						if(guildTime.wHour == 19 && guildTime.wMinute>= 45)	// 18点45进行公告!
						{
								nTime = 60 - guildTime.wMinute;         //计算15分钟后开放
								if(guildTime.wSecond == 0)				//一分钟公告一次
								{
									strMsg.Format( "恶魔广场还有 %d 分钟开放,请大家做好备战准备!", nTime);//"恶魔广场还有 %d 分钟将会开放,请大家准备就绪!"
									Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
								}
						}
						
						if(guildTime.wHour  == 20 && guildTime.wMinute <= 30)
						{
								nTime = 30 - guildTime.wMinute;
								if(nTime <=5 && guildTime.wSecond == 0)
								{
									strMsg.Format( "恶魔广场在 %d 分钟后结束! ", nTime);//"恶魔广场还有 %d 分钟将会关闭!"
									Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
								}
						  }
					}
}

void COM::ReUpdateFortress2DB(CGuildFortress *pFort)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	TCHAR			szSQL[8000];
	TCHAR			strWarTime[30];

	CString strTime = _T("");

	if(pFort == NULL) return;

	::ZeroMemory(szSQL, sizeof(szSQL));

	::ZeroMemory(strWarTime, sizeof(strWarTime));

	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);
	CTime curr(guildTime);

	curr += CTimeSpan( o_yehuoini[0]->ysjg, 0, 0, 0 ); //本次要塞与下次要塞间隔时间(目前是7天一次)

	strTime.Format("%d-%d-%d 21:00:00", curr.GetYear(), curr.GetMonth(), curr.GetDay());
	::CopyMemory(strWarTime, strTime.GetBuffer(strTime.GetLength()), strTime.GetLength());
	strTime.ReleaseBuffer();
		
	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call REUPDATE_GUILD_FORTRESS(%d,%d,\'%s\',\'%s\')}"), 
	pFort->m_sFortressID, pFort->m_iGuildSid, pFort->m_strGuildName, strWarTime);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return ;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild_Store Data Only!!\n");

		//g_DBNew[m_iModSid].ReleaseDB(db_index);
		return ;
	}

	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

			g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
			return ;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return ;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
}
void COM::SetHourExp(int type)
{
	/*SYSTEMTIME Time;
	GetLocalTime(&Time);
	//晚上18.59分自动开3倍
	if(Time.wHour == 18 && Time.wMinute == 59 && Time.wSecond == 30 )
	{
		g_sanJingYan = TRUE;
		g_sanBaoLv = TRUE;
		USER *pUser = NULL;
		for (int i = 0; i < MAX_USER; i++ )
		{
			pUser = GetUserUid(i);
			if(pUser && pUser->m_state == STATE_GAMESTARTED )
			{
				pUser->SetXingfen();
				pUser->SetXingYun();
			}
		}
		Announce("系统:启动全服3倍兴奋和3倍幸运黄金时间,祝大家游戏愉快!", SYSTEM_ANNOUNCE);
	}
	//22点59分30秒自动关3倍
	if(Time.wHour == 22 && Time.wMinute == 59 && Time.wSecond == 30)
	{
		/*g_sanJingYan = FALSE;
		g_sanBaoLv = FALSE;
		USER *pUser = NULL;
		for (int i = 0; i < MAX_USER; i++ )
		{
			pUser = GetUserUid(i);
			if(pUser && pUser->m_state == STATE_GAMESTARTED )
			{
				pUser->DelXingfen();
				pUser->DelXingYun();
			}
		}
		Announce("系统:全服3倍兴奋和3倍幸运时间结束,明天晚上19~23点继续开放!", SYSTEM_ANNOUNCE);	
    }*/
}
void COM::SendFortressWarBegin(CGuildFortress *pFort)
{
	if(pFort == NULL) return;
	UpdateFortress2DB(pFort);

	int mapindex = -1;
	BOOL bSend = FALSE;
	int i, j, k, type;
	int nLen = 0;
	USER *pUser = NULL;

	CBufferEx TempBuf;
	CDWordArray arGuildList;
	arGuildList.RemoveAll();

	if(pFort->m_iGuildSid <= 0) return;				// 辨靛傈阑困秦 曼咯吝牢 傈眉 府胶飘甫 父电促.
	arGuildList.Add(pFort->m_iGuildSid);				// 郴辨靛 刚历...

	for(j =0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)			// 惑措祈 辨靛...
	{
		if(pFort->m_arAttackGuild[j].lUsed == 1)
		{
			arGuildList.Add(pFort->m_arAttackGuild[j].lGuild);
		}
	}

	CString strMsg;

	short nCount = arGuildList.GetSize();

	if(nCount <= 1) 
	{
		strMsg = _T("");
		if(pFort->m_sFortressID == 1000) strMsg.Format( IDS_USER_END_LUINET_NO_APPLY );
		else if(pFort->m_sFortressID == 1001) strMsg.Format( IDS_USER_END_SANAD_NO_APPLY );
		else if(pFort->m_sFortressID == 1002) strMsg.Format( IDS_USER_END_ZAMELLYA_NO_APPLY );
		Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL);
		pFort->m_dwStartTick = 0;
		return;
	}

	TempBuf.Add(GUILD_WAR);
	TempBuf.Add((BYTE)0x01);							// 辨靛傈 矫累...
	TempBuf.Add((BYTE)GUILD_FOTRESS_WARRING);			// 惑痢傈...

	TempBuf.Add((short)pFort->m_sFortressID);			// 秦寸 瘤开 牢郸胶

	strMsg = _T("");
	if(pFort->m_sFortressID == 1000) strMsg.Format( IDS_USER_START_LUINET );
	else if(pFort->m_sFortressID == 1001) strMsg.Format( IDS_USER_START_SANAD );
	else if(pFort->m_sFortressID == 1002) strMsg.Format( IDS_USER_START_ZAMELLYA );

	for(i = 0; i < MAX_USER; i++)
	{
		type = 0;
		nLen = 0;
		mapindex = -1;

		pUser = GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;		

		pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);

		type = ((g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_dwType & 0xFF00 ) >> 8);

		//if(type >= 0 && type < 17)
		mapindex = pUser->GetGuildMapIndex(type);
		if(mapindex >= 0) 
		{
			if(g_arMapTable[mapindex] == NULL) continue;
			if(g_arMapTable[mapindex]->m_sStoreID == pFort->m_sFortressID)// && g_arMapTable[type]->m_sStoreZone)		
			{																		 // 家蜡 辨靛啊 绝栏搁 葛滴 付阑肺 焊辰促.				
				if(pFort->m_iGuildSid != pUser->m_dwGuild && pUser->m_tIsOP != 1 ) pUser->TownPotal();
			}
		}
		
		CBufferEx DataBuf;
		DataBuf.AddData(TempBuf, TempBuf.GetLength());

		nLen = strlen(pUser->m_strGuildName);
		if(pUser->m_dwGuild <= 0 || nLen <= 0) 
		{			
			DataBuf.Add((BYTE)0x00);					// 老馆蜡历			
			pUser->Send(DataBuf, DataBuf.GetLength());
			continue;
		}

		if(pFort->m_iGuildSid == pUser->m_dwGuild) 
		{
			pUser->BeginFortressWar();					// 辨靛 傈阑 矫累茄促.
			
			DataBuf.Add((BYTE)0x01);					// 秦寸 辨靛 蜡历
			DataBuf.Add(nCount);						// 醚 辨靛荐

			for(j = 0; j < nCount; j++)
			{
				DataBuf.Add(arGuildList[j]);
			}

			pUser->Send(DataBuf, DataBuf.GetLength());
		}
		else
		{
			for(j =0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)
			{
				if(pFort->m_arAttackGuild[j].lUsed == 0) continue;

//				if(strcmp(pFort->m_arAttackGuild[j].strGuildName, pUser->m_strGuildName) == 0)
				if(pFort->m_arAttackGuild[j].lGuild == pUser->m_dwGuild)
				{
					pUser->BeginFortressWar();			// 辨靛 傈阑 矫累茄促.
					
					DataBuf.Add((BYTE)0x01);			// 秦寸 辨靛 蜡历
					DataBuf.Add(nCount);				// 醚 辨靛荐
					for(k = 0; k < nCount; k++)
					{
						DataBuf.Add(arGuildList[k]);
					}
					bSend = TRUE;
					pUser->Send(DataBuf, DataBuf.GetLength());
					break;
				}
			}

			if(!bSend)
			{				
				DataBuf.Add((BYTE)0x00);					// 老馆蜡历
				pUser->Send(DataBuf, DataBuf.GetLength());
			}
		}
	}

	pFort->SetNpcToFortressWarBegin();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
void COM::SendFortressWarEnd(CGuildFortress *pFort) //要塞结束了
{
	int mapindex;
	int i, type;
	USER *pUser = NULL;
	if(pFort == NULL) return;

	pFort->SetNpcToFortressWarEnd(this);

	CString strMsg = _T("");
	strMsg.Format( IDS_USER_END_FORTRESS, pFort->m_strGuildName);


	CBufferEx TempBuf;

	TempBuf.Add(GUILD_WAR);
	TempBuf.Add((BYTE)0x02);							// 辨靛傈 辆丰...
	TempBuf.Add((BYTE)GUILD_FOTRESS_WARRING);
	TempBuf.Add((short)pFort->m_sFortressID);

//	Send(TempBuf, TempBuf.GetLength());

	for(i = 0; i < MAX_USER; i++)
	{
		mapindex = -1;

		pUser = GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		type = pUser->CheckInvalidMapType();
		mapindex = pUser->GetGuildMapIndex(type);
		if(mapindex >= 0)
		{		
			if(g_arMapTable[mapindex] == NULL) continue;
			if(g_arMapTable[mapindex]->m_sStoreID == pFort->m_sFortressID) //&& g_arMapTable[type]->m_sStoreZone)		
			{											// 规绢辨靛盔捞 酒聪搁 葛滴 付阑肺																				 
				if(pFort->m_iGuildSid != pUser->m_dwGuild && pUser->m_tIsOP != 1) pUser->TownPotal();
			}
		}

		pUser->m_tFortressWar = GUILD_WAR_AFFTER;

		pUser->Send(TempBuf, TempBuf.GetLength());

		if(pFort->m_iZone == pUser->m_curz)
		{
			if(pFort->m_iGuildSid != pUser->m_dwGuild && pUser->m_tIsOP != 1 ) pUser->TownPotal();
		}

		pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);					
	}

	pFort->UpdateInitFortress();

	ReUpdateFortress2DB(pFort);	
}

void COM::RemoveUserInGuildHouse()
{
	int i, j;
	USER *pUser = NULL;

	for(i = 0; i < MAX_USER; i++)
	{
		pUser = GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		if(pUser->m_dwGuild <= 0) continue;
	
		for(j = 0; j < g_arGuildHouse.GetSize(); j++)
		{
			if(g_arGuildHouse[j]->iZone == pUser->m_curz)
			{
				if(g_arGuildHouse[j]->iGuild != pUser->m_dwGuild && pUser->m_tIsOP != 1 ) 
				{
					pUser->TownPotal();
					break;
				}
			}
		}
	}
}

void COM::UpdateFortress2DB(CGuildFortress *pFort)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];
	int				i;	

	if(pFort == NULL) return;

	TCHAR			strAttackList[_GUILDLIST_DB], strRepairList[_GUILDREPAIRLIST_DB];

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strRepairList, sizeof(strRepairList));
	::ZeroMemory(strAttackList, sizeof(strAttackList));

	pFort->GuildAttListToStr(strAttackList);
	pFort->FortressRepairListToStr(strRepairList);

	SDWORD sAttLen		= sizeof(strAttackList);
	SDWORD sRepairLen	= sizeof(strRepairList);

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_FORTRESS(%d,%d,\'%s\',%d,%d,%d,?,?)}"), 
	pFort->m_sFortressID, pFort->m_iGuildSid, pFort->m_strGuildName, pFort->m_tTaxRate, pFort->m_iGuildDN, pFort->m_tWarType);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return ;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild_Store Data Only!!\n");

		//g_DBNew[m_iModSid].ReleaseDB(db_index);
		return ;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strAttackList),0, (TCHAR*)strAttackList,	0, &sAttLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strRepairList),0, (TCHAR*)strRepairList,	0, &sRepairLen );

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

			g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
			return ;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return ;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return ;

}

void COM::UpdateFortressMem2DB(CGuildFortress *pFort)
{
/*	int				i;
	TCHAR			strAttackList[_GUILDLIST_DB], strRepairList[_GUILDREPAIRLIST_DB];

	if(pFort == NULL) return;

	::ZeroMemory(strRepairList, sizeof(strRepairList));
	::ZeroMemory(strAttackList, sizeof(strAttackList));

	pFort->GuildAttListToStr(strAttackList);
	pFort->FortressRepairListToStr(strRepairList);

	SDWORD sAttLen		= sizeof(strAttackList);
	SDWORD sRepairLen	= sizeof(strRepairList);
	
	CSharedMemory* pShared = NULL;
	CMemFortress* pMDFort = NULL;

	for(i = 0; i < g_arFortressSharedMemory.GetSize(); i++)
	{
		pShared = g_arFortressSharedMemory[i];
	
		if(pShared == NULL) continue;
		if(pShared->m_hMapping == NULL) continue;
		
		pMDFort = (CMemFortress*) pShared->m_lpData;
		
		if(pMDFort == NULL) continue;
		if(pMDFort->m_sFortressID <= 0) continue;

		if(pMDFort->m_sFortressID == pFort->m_sFortressID)
		{
			pMDFort->m_iGuildSid = pFort->m_iGuildSid;
			pMDFort->m_sTaxRate = 0;
			pMDFort->m_dwStoreDN = 0;
			pMDFort->m_tWarType = GUILD_WAR_PREPARE;
			pFort->FortressRepairListToStr(pMDFort->m_RepairList);
			pFort->GuildAttListToStr(pMDFort->m_AttackList);
			return;
		}
	}
*/
}

void COM::LoadFortressCityDegree(CGuildFortress *pFort)
{
/*	if(!pFort) return;
	if(!pFort->m_bHaveGuild) return;
	if(pFort->m_iGuildSid <= 0) return;

	int				i;
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	SQLINTEGER	iRet = -1;
	SQLINTEGER	iRetInd = SQL_NTS;
	
	SQLINTEGER iCityDegree;

	::ZeroMemory(szSQL, sizeof(szSQL));
	
	iCityDegree = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_GUILD_FORTRESS_CITYDEGREE(%d, ?)}"), pFort->m_iGuildSid);

//	SQLINTEGER	sInd;

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild_Store Attack List Data !!\n");

//		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return ;
	}

	i = 1;
	SQLBindParameter( hstmt, i++ ,SQL_PARAM_OUTPUT,SQL_C_SLONG, SQL_INTEGER,0,0, &iRet,0, &iRetInd);
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );
		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
//			SQLGetData( hstmt, i++, SQL_C_SLONG,  &iCityDegree,sizeof(iCityDegree),&sInd );
//			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sCityDegree,	 sizeof(sCityDegree),	&sInd );
//			SQLGetData( hstmt, i++, SQL_C_ULONG, &sCityDegree,	 sizeof(sCityDegree),	&sInd );
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return ;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

//	pFort->m_iCityDegree = iRet;
*/
}


void COM::SetFortressWarTime(CGuildFortress *pFort)
{
	CString			strTime;
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strWarTime[30];

	SYSTEMTIME guildTime;
	CTimeSpan DiffTime;
	if(pFort == NULL) return;

	if(pFort->m_iGuildSid <= 0) return;

	int iYear = 0;
	int iMon = 0;
	int iDay = 0;
	long temp = -1;

	strTime = _T("");
	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strWarTime, sizeof(strWarTime));

	GetLocalTime(&guildTime);

	CTime curr(guildTime);
	CTime last(pFort->m_wPlanWarTime);
	DiffTime = last - curr;	
	
	temp = (int)DiffTime.GetDays();
	
	if(temp < 0)
	{
		curr += CTimeSpan( o_yehuoini[0]->ysjg, 0, 0, 0 ); //本次要塞与下次要塞间隔时间(目前是7天一次)

		iYear = curr.GetYear();
		iMon = curr.GetMonth();
		iDay = curr.GetDay();
	}
	else 
	{
		if(pFort->m_tWarType == GUILD_WAR_DECISION) return;

		iYear = last.GetYear();
		iMon = last.GetMonth();
		iDay = last.GetDay();	
	}
	
	pFort->m_wPlanWarTime.wYear = iYear;
	pFort->m_wPlanWarTime.wMonth = iMon;
	pFort->m_wPlanWarTime.wDay = iDay;

	strTime.Format("%d-%d-%d 21:00:00", iYear, iMon, iDay);
	::CopyMemory(strWarTime, strTime.GetBuffer(strTime.GetLength()), strTime.GetLength());
	strTime.ReleaseBuffer();
	
	SDWORD sLen	= sizeof(strWarTime);

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_FORTRESS_WARTIME(%d,%d,\'%s\')}"), pFort->m_sFortressID, pFort->m_iGuildSid, strWarTime);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return ;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Fortress War Data Only!!\n");
		return ;
	}

	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

			g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
			return ;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return ;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
	
	pFort->m_wPlanWarTime.wYear = iYear;
	pFort->m_wPlanWarTime.wMonth = iMon;
	pFort->m_wPlanWarTime.wDay = iDay;
	pFort->m_wPlanWarTime.wHour = 21;

	pFort->m_tWarType = GUILD_WAR_DECISION;// 12矫埃 傈俊 搬沥捞 救登绢 乐促搁 官肺 被洒扁 累诀俊 甸绢埃促.

	UpdateMemFortressWarType(pFort);
}

void COM::UpdateMemFortressWarType(CGuildFortress *pFort)
{
	if(pFort == NULL) return;

	int nSize = g_arFortressSharedMemory.GetSize();
	CSharedMemory* pShared = NULL;
	CMemFortress* pData = NULL;

	for(int i = 0; i < nSize; i++)
	{
		pShared = g_arFortressSharedMemory[i];
		
		if(pShared == NULL) return;
		if(pShared->m_hMapping == NULL) return;
		
		pData = (CMemFortress*) pShared->m_lpData;
		
		if(pData == NULL) return;
		
		if(pData->m_iGuildSid == pFort->m_iGuildSid)
		{
			pData->m_tWarType = pFort->m_tWarType;
			break;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//	辨靛啊 家蜡窍绊 乐栏搁 刮缴荐摹甫 犬牢茄促.
//
void COM::CheckViolenceInFortress()
{
	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);
	DWORD dwCurrTick = GetTickCount();					// 力茄 矫埃郴狼 辨靛傈阑 眉农...

	COleDateTime CurrTime = COleDateTime(guildTime);
	SYSTEMTIME st;

	int i;
	int nTime = 0;
	static int nState = 0;
	CString strMsg;

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		int time = 0;
		CTimeSpan DiffTime;

		if(!g_arGuildFortress[i]) continue;

		if(g_arGuildFortress[i]->m_lViolenceUsed == 0) 
		{
			st = g_arGuildFortress[i]->m_wMopPartyTime;
			COleDateTime MopTime(st.wYear, st.wMonth, st.wDay, st.wHour, 0, 0);

			if(MopTime.GetYear() >= 2030) 
			{
				if(g_arGuildFortress[i]->GetTotalCityValue() > 60)	// 弥厩狼 刮缴俊 酒流 DB俊 馆康捞 救登搁 静绊 墨款飘 促款 矫累...
				{
					GetLocalTime(&g_arGuildFortress[i]->m_wMopPartyTime);
					SetFortressViolenceTime(g_arGuildFortress[i], g_arGuildFortress[i]->m_iGuildSid, FORTRESS_VIOLENCE_WARRING);
				}
				continue;
			}

			if(CurrTime.m_status == COleDateTime::valid && MopTime.m_status == COleDateTime::valid)
			{
				CTime curr(CurrTime.GetYear(), CurrTime.GetMonth(), CurrTime.GetDay(), CurrTime.GetHour(), 0, 0);
				CTime last(MopTime.GetYear(), MopTime.GetMonth(), MopTime.GetDay(), MopTime.GetHour(), 0, 0);
				DiffTime = curr - last;			
			}

			if(g_arGuildFortress[i]->m_iGuildSid < SYSTEM_GUILD_BAND && g_arGuildFortress[i]->m_iGuildSid > 0)		// 惑痢俊 林牢捞 乐绊
			{
				time = (int)DiffTime.GetTotalHours();
				if(time >= UPDATE_GUILD_INVEN_TIME)			//12矫埃付促
				{
					g_arGuildFortress[i]->m_wMopPartyTime.wYear = 2030;
					SetFortressViolenceTime(g_arGuildFortress[i], g_arGuildFortress[i]->m_iGuildSid, FORTRESS_VIOLENCE_AFFTER);

					if(g_arGuildFortress[i]->m_sFortressID == 1000)
					{
						strMsg.Format( IDS_USER_RIOT_LUINET );
					}
					else if(g_arGuildFortress[i]->m_sFortressID == 1001)
					{
						strMsg.Format( IDS_USER_RIOT_SANAD );
					}
					else if(g_arGuildFortress[i]->m_sFortressID == 1002)
					{
						strMsg.Format( IDS_USER_RIOT_ZAMELLYA );
					}
					Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);

					g_arGuildFortress[i]->m_dwViolenceTick = dwCurrTick;
					g_arGuildFortress[i]->SetNpcToFortressViolenceBegin();
					InterlockedExchange(&g_arGuildFortress[i]->m_lViolenceUsed, 1);
				}
			}
		}
		else if(g_arGuildFortress[i]->m_lViolenceUsed == 1) 
		{
			if(dwCurrTick - g_arGuildFortress[i]->m_dwViolenceTick > 60000 * 10)	// 10盒 抛胶飘
			{				// 场车促.
				InterlockedExchange(&g_arGuildFortress[i]->m_lViolenceUsed, 0);
				g_arGuildFortress[i]->m_dwStartTick = 0;				
				SetNpcToViolenceFortressWarEnd(g_arGuildFortress[i]);
//				SendFortressWarEnd(g_arGuildFortress[i]);
			}
		}
	}
}

void COM::SetFortressViolenceTime(CGuildFortress *pFort, int iGuild, int iType)
{
	CString			strTime;
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		

	CTimeSpan DiffTime;
	if(pFort == NULL) return;

	if(pFort->m_iGuildSid <= 0) return;

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_FORTRESS_VIOLENCETIME(%d,%d,%d)}"), pFort->m_sFortressID, iGuild, iType);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return ;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Fortress War Data Only!!\n");
		return ;
	}

	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

			g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
			return ;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return ;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
}

void COM::SetNpcToViolenceFortressWarEnd(CGuildFortress *pFort)
{	
	BOOL bChange = FALSE;
	CString strMsg = _T("");
    if(pFort == NULL) return;
	bChange = pFort->SetNpcToFortressViolenceEnd(this);

	if(bChange)
	{
		SetFortressViolenceTime(pFort, 900000, FORTRESS_VIOLENCE_AFFTER);

		pFort->SetInitFortressViolence();
		pFort->InitMemFortress(GUILD_WAR_DECISION);

		if(pFort->m_sFortressID == 1000)
		{
			strMsg.Format( IDS_USER_RIOT_LUINET_RETURN );
		}
		else if(pFort->m_sFortressID == 1001)
		{
			strMsg.Format( IDS_USER_RIOT_SANAD_RETURN );
		}
		else if(pFort->m_sFortressID == 1002)
		{
			strMsg.Format( IDS_USER_RIOT_ZAMELLYA_RETURN );
		}
		Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);

		pFort->GetOutof(this);
	}
}

////////////////////////////////////////////////////////////////////////电光板排队
int COM::Add_DGB_Massage( TCHAR *pBuf, int iLen )
{
	CBufferEx *pNewBuf = new CBufferEx();
	pNewBuf->AddData(pBuf,iLen);

	EnterCriticalSection(&m_DGB_CS);
	m_DGB_MessageQueue.push_back(pNewBuf);
	LeaveCriticalSection(&m_DGB_CS);
	return m_DGB_MessageQueue.size();
}

void COM::Check_DGB_Massage()
{
	//niuniu add 456
	static DWORD s_dwLaseDianGuangBanTime = GetTickCount();			//niuniu add 456
	if(GetTickCount() - s_dwLaseDianGuangBanTime < 10000 )
	{
		return;
	}

	EnterCriticalSection(&m_DGB_CS);
	if (m_DGB_MessageQueue.size())
	{
		s_dwLaseDianGuangBanTime = GetTickCount();
		CBufferEx *pBuf = (*m_DGB_MessageQueue.begin());
		m_DGB_MessageQueue.pop_front();
		for(int i = 0; i < MAX_USER; i++)
		{
			USER *pUser = GetUserUid(i);

			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

			pUser->Send(*pBuf, pBuf->GetLength());
		}
		delete pBuf;
	}

	LeaveCriticalSection(&m_DGB_CS);
}

void COM::GetUserTopTime()//称号排名
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	TCHAR			szSQL[1024];		

	::ZeroMemory(szSQL, sizeof(szSQL));
	int index = 0;
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USERTOP}")); 

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return;
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if(retcode != SQL_SUCCESS) return;
	if(retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
		}
	}
	if (hstmt != NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
}

void COM::SetUserTop(char* UserName,int type)//称号排名
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	TCHAR			szSQL[1024];
	::ZeroMemory(szSQL, sizeof(szSQL));
	int index = 0;
	_sntprintf_s(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_TOP (\'%s\',%d)}"),UserName,type); 

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return;
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return;
	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
		}
	}	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
}
BOOL COM::GetUserPaiMing() // 获取玩家排名 
{
    int i;    
    SQLHSTMT        hstmt;
    SQLRETURN       retcode;
    TCHAR           szSQL[2048];
    CString sql = _T("");
    
    // 修改SQL查询，添加排除条件和获取iExp字段
    sql.Format("SELECT TOP 10 strUserID, sLevel, iExp FROM gameuser WHERE tIsOP = 0 AND sLevel >= 70 ORDER BY sLevel DESC, iExp DESC");
    ::ZeroMemory(szSQL, sizeof(szSQL));
    wsprintf(szSQL, TEXT(sql));    
    
    SQLCHAR         strUserSortName[CHAR_NAME_LENGTH + 1];
    SQLSMALLINT     uLevel;
    SQLINTEGER      uExp; // 用于存储iExp的变量
    SQLINTEGER      sInd;

    ::ZeroMemory(strUserSortName, sizeof(strUserSortName));
    uLevel = 0;    
    uExp = 0; // 初始化iExp

    int db_index = 0;
    CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB(db_index);
    if (!pDB) return FALSE;

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt);
    if (retcode != SQL_SUCCESS)
    {
        TRACE("Fail To Load UserSort Data !!\n");
        AfxMessageBox(_T("SELECT GAMEUSER Table Open Fail!"));
        return FALSE;
    }
    
    retcode = SQLExecDirect(hstmt, (unsigned char*)szSQL, SQL_NTS);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
    {
        int cixu = 0;
        while (TRUE)
        {
            retcode = SQLFetch(hstmt);

            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
            {
                cixu++;
                i = 1;
                SQLGetData(hstmt, i++, SQL_C_CHAR, &strUserSortName, sizeof(strUserSortName), &sInd);
                SQLGetData(hstmt, i++, SQL_C_SSHORT, &uLevel, sizeof(uLevel), &sInd);
                SQLGetData(hstmt, i++, SQL_C_SLONG, &uExp, sizeof(uExp), &sInd); // 获取iExp字段

                USER *pUser = GetUser((CHAR *)strUserSortName);
                if (cixu <= 0 || cixu > 10) continue; // 排行名次
                if (pUser != NULL && pUser->m_state == STATE_GAMESTARTED)
                {
                    pUser->m_dwPD = cixu;
                    CString str = _T("");
                    str.Format("%s", pUser->m_strLoveName);
                    int pos = str.Find("]");
                    if (pos > 0)
                    {
                        str = str.Mid(pos + 1, str.GetLength());
                    }
                    ::ZeroMemory(pUser->m_strLoveName, sizeof(pUser->m_strLoveName));
                    sprintf(pUser->m_strLoveName, "@9[%d]%s", pUser->m_dwPD, str);
                    CBufferEx TempBuf;
                    TempBuf.Add(LOVE_NAME);
                    TempBuf.Add(pUser->m_uid + USER_BAND);
                    TempBuf.AddString(pUser->m_strLoveName);
                    pUser->SendExactScreen(TempBuf, TempBuf.GetLength());
                }
                SetUserTop((CHAR *)strUserSortName, cixu);
            }
            else break;
        }       
    }
    else if (retcode == SQL_NO_DATA)
    {
        g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
        return FALSE;
    }
    else
    {
        retcode = SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
        g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
        return FALSE;
    }
    
    retcode = SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

    return TRUE;
}

//BOOL COM::GetUserPaiMing()//获取玩家排名 
//{
//	int i;	
//	SQLHSTMT		hstmt;
//	SQLRETURN		retcode;
//	TCHAR			szSQL[2048];
//	CString sql = _T("");
//	sql.Format("SELECT TOP 10 strUserID, sLevel FROM gameuser WHERE tIsOP = 0 ORDER BY sLevel DESC");//排行名次
//	::ZeroMemory(szSQL, sizeof(szSQL));
//	wsprintf(szSQL,TEXT(sql));	
//	
//	SQLCHAR			strUserSortName[CHAR_NAME_LENGTH + 1];
//	SQLSMALLINT		uLevel;
//	SQLINTEGER		sInd;
//
//	::ZeroMemory(strUserSortName, sizeof(strUserSortName));
//	uLevel = 0;	
//
//	int db_index = 0;
//	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
//	if( !pDB ) return FALSE;
//
//	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
//	if( retcode != SQL_SUCCESS )
//	{
//		TRACE("Fail To Load UserSort Data !!\n");
//		AfxMessageBox(_T("SELECT GAMEUSER Table Open Fail!"));
//		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
//		return FALSE;
//	}
//	
//	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
//	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
//	{
//		int cixu = 0;
//		while(TRUE)
//		{
//			retcode = SQLFetch(hstmt);
//
//			if(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
//			{
//				cixu++;
//				i = 1;
//				SQLGetData( hstmt, i++, SQL_C_CHAR,	  &strUserSortName, sizeof(strUserSortName), &sInd );
//				SQLGetData( hstmt, i++, SQL_C_SSHORT, &uLevel, sizeof(uLevel), &sInd );
//				USER *pUser = GetUser((CHAR*)strUserSortName);
//				if(cixu <= 0 || cixu > 10) continue;//排行名次
//				if(pUser != NULL && pUser->m_state == STATE_GAMESTARTED)
//				{
//                    pUser->m_dwPD = cixu;
//					CString str = _T("");
//					str.Format("%s",pUser->m_strLoveName);
//					int pos = str.Find("]");
//					if(pos > 0)
//					{
//						str = str.Mid(pos + 1,str.GetLength());
//					}
//					::ZeroMemory(pUser->m_strLoveName,sizeof(pUser->m_strLoveName));
//					sprintf(pUser->m_strLoveName,"@9[%d]%s",pUser->m_dwPD,str);
//					CBufferEx TempBuf;
//					TempBuf.Add(LOVE_NAME);
//					TempBuf.Add(pUser->m_uid + USER_BAND);
//					TempBuf.AddString(pUser->m_strLoveName);
//					pUser->SendExactScreen(TempBuf, TempBuf.GetLength());
//				 
//				}
//				SetUserTop((CHAR*)strUserSortName,cixu);
//			}
//			else break;
//		}		
//	}
//	else if (retcode == SQL_NO_DATA)
//	{
//		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
//		return FALSE;
//	}
//	else
//	{
//		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
//		return FALSE;
//	}
//	
//	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
//
//	return TRUE;
//}
