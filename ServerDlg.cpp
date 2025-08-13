/////////////////////////////////////////////////////////////////////
//	ServerDlg.cpp : implementation file of the the CServerDlg class
//  Copyright (C) 2000,  Jin-Soo Jeon
//
//	If this code works, it was written by Jin-Soo Jeon.
//	If not, I don't know who wrote it.
//

#include "stdafx.h"
#include "Server.h"
#include "USER.h"
#include "ServerDlg.h"

#include "ZoneInfoSet.h"

#include "YhuoIni.h"
#include "NpcTableSet.h"
#include "NpcTable.h"
#include "NpcPosSet.h"
#include "NpcItemSet.h"
#include "NpcChatSet.h"
#include "CityNpcTableSet.h"

#include "DBItemInfo.h"

#include "ItemTableSet.h"
#include "SkillTableSet.h"
#include "PsiTableSet.h"

#include "LevelUpTableSet.h"
#include "PAMAExpSet.h"

#include "PsiStoreSet.h"
#include "EBodyStoreSet.h"
#include "StoreSet.h"
#include "StoreSellSet.h"
#include "StoreRepairSet.h"

#include "BufferEx.h"

#include "InitItemTableSet.h"

#include "MagicItemTableSet.h"

#include "RecoverSet.h"
#include "RecoverRateSet.h"
#include "Recover.h"
#include "RecoverRate.h"

#include "DNTableSet.h"
#include "DNTable.h"

#include "ValItemTableSet.h"
#include "ValItemTable.h"

#include "Guild.h"

#include "EBodyTable.h"
#include "EBodyTableSet.h"

#include "EBodyUpgradeTableSet.h"
#include "EBodyIdentifyTableSet.h"
#include "HuanshiTableSet.h"

#include "MemUser.h"

#include "GuildStoreSet.h"
#include "GuildStoreTable.h"
#include "GuildMapInfoSet.h"
#include "GuildMapInfoTable.h"

#include "GuildHouseRankSet.h"

#include "VirtualRoom.h"
#include "GuildHouseSet.h"
#include "UserLevelUpSet.h"
#include "UserLevelUpTable.h"

#include "Extern.h"

#include "Search.h"
#include "SocketManager.h"
#include "UserManager.h"

#include "EventItemTable.h"
#include "EventItemTableSet.h"
#include "AddEventItemTableSet.h"

#include "NpcSayTable.h"
#include "NpcSayEventSet.h"

//#include "GuildFortress.h"
#include "GuildFortressSet.h"
#include "GuildFortressTaxSet.h"

#include "TownPotalTableSet.h"

#include "AccessoriUpTableSet.h"

#include "RoyalRumble.h"
#include "RoyalRumbleUser.h"

#include "EventBlockingTable.h"

#include "RemodelingTableSet.h"
#include "Remodeling02TableSet.h"
#include "Remodeling03TableSet.h"

#include "BoxEventTableSet.h"

#include "SharedQueue.h"   // zinee 02/11/26  for Mail Agent
#include "ShopSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Global Variable
#define UM_START_SERVER		10000	
#define ALIVE_FILL_TIMER	5000

CServerDlg			*g_pMainDlg = NULL;

CMyDB				g_DB[AUTOMATA_THREAD+1];
CMyDB				g_DBSession[AUTOMATA_THREAD+1];
CMyDB				g_DBNew[AUTOMATA_THREAD+1];

SharedMemoryArray	g_arSharedMemory;
SharedMemoryArray	g_arStoreSharedMemory;
SharedMemoryArray	g_arFortressSharedMemory;
SharedMemoryArray	g_arGuildHouseSharedMemory;

CSharedQueue		g_MailAgentQueue;

TableItemArray		g_DBItemArray;	//  2001.1.17

cyehuoini			o_yehuoini;

ServerArray			g_server;

ZoneArray			g_zone;
// alisia
ZoneArray			g_zonesize;
//

CellArray			g_cell;

EventArray			g_event;
NpcTableArray		g_arNpcTable;
NpcTableArray		g_arCityNpcTable;
NpcThreadArray		g_arNpcThread;
CNpcItem			g_NpcItem;
struct drop_info    g_DropItem[256][4];
struct convert_table convert[80];
BOOL				g_bNpcExit		= FALSE;

ItemTableArray		g_arItemTable;
SkillTableArray		g_arSkillTable;
PsiTableArray		g_arPsiTable;

LevelUpTableArray	g_arLevelUpTable;
PAMAExpArray		g_arPAMAExp;

StoreArray			g_arStore;
StoreArray			g_arPsiStore;
StoreArray			g_arEBodyStore;

NpcChatArray		g_arNpcChat;

CErrorLog			g_ErrorLog;

WeatherArray		g_WeatherZone;	
TownPotalArray		g_TownPotal;
RandomTownPotalArray	g_RandomTownPotal;

InitItemTableArray	g_arInitItem;

MagicItemTableArray	g_arMagicItemTable;

AccessoriUpTableArray	g_arAccessoriUpTable;		// Accessori Upgrade Table
CRipel					g_Ripel;					// Ripel's crest (Accessori °ü·Ã)

RemodelingTableArray	g_arRemodelingTable1;		// 1 ´Ü °³Á¶ Table
RemodelingTableArray	g_arRemodelingTable2;		// 2 ´Ü °³Á¶ Table
RemodelingTableArray	g_arRemodelingTable3;		// 3 ´Ü °³Á¶ Table

CWordArray				g_arRemodelingRandomIndex1;	// 1 ´Ü °³Á¶ È®À² ÀÎµ¦½º
int						g_iRemodelingArmorIndex1;	// 1 ´Ü °³Á¶ ¹æ¾î±¸ ½ÃÀÛ ÀÎµ¦½º
CWordArray				g_arRemodelingRandomIndex2;	// 2 ´Ü °³Á¶ È®À² ÀÎµ¦½º
int						g_iRemodelingArmorIndex2;	// 2 ´Ü °³Á¶ ¹æ¾î±¸ ½ÃÀÛ ÀÎµ¦½º
CWordArray				g_arRemodelingRandomIndex3;	// 3 ´Ü °³Á¶ È®À² ÀÎµ¦½º
int						g_iRemodelingArmorIndex3;	// 3 ´Ü °³Á¶ ¹æ¾î±¸ ½ÃÀÛ ÀÎµ¦½º

BoxEventTableArray		g_arBoxEventTable;			// Box Event Table Array
SummonTableArray		g_arSummonTable;			// Summon Monster Table

#ifdef KOR_VER
short					g_sSummonException[] = {103, 104, 105, 106, 107, 119, 128, 129, 130, 140};
#endif

#ifdef CHN_VER
short					g_sSummonException[] = {103, 104, 105, 106, 107, 111, 113, 115, 117,
												118, 119, 122, 123, 124, 125, 126, 127, 
												128, 129, 130, 140
												};

#endif

#ifdef ENG_VER
short					g_sSummonException[] = {103, 104, 105, 106, 107, 111, 113, 115, 117,
												118, 119, 122, 123, 124, 125, 126, 127, 
												128, 129, 130, 140
												};

#endif

NpcArray			g_arNpc;

RecoverArray		g_arRecoverTable;
RecoverRateArray	g_arRecoverRateTable;

DNArray				g_arDNTable;
CNpcItem			g_ValItem;

NpcThreadArray		g_arEventNpcThread;

GuildArray			g_arGuildData;
GuildUserArray		g_arGuildUser;
//GuildStoreArray		g_arGuildStore;

EBodyArray					g_arEBodyTable;
EBodyUpgradeTableArray		g_arEBodyUpgradeTable;
EBodyIdentifyTableArray		g_arEBodyIdentifyTable;
HuanshiArray			g_arHuanshiTable;
MessAgeArray			g_arMessAgeArray;
OnlineShopArray			    g_arOnlineShopTable;
SxOnlineShopArray			g_arSxOnlineShopTable;
KaixiangziArray			g_arKaixiangziTable;
OnlineRMBShopArray		g_arOnlineRMBShopTable;			//ÔÚÏßÉÌµê

GuildMapArray		g_arMapTable;

GuildHouseWarArray	g_arGuildHouseWar;

VirtualRoomArray	g_arVirtualRoom;
GuildHouseArray		g_arGuildHouse;

UserLevelArray		g_arUserLevel;

EventItemArray		g_arEventItemTable;
EventItemArray		g_arAddEventItemTable;
EventItemNewArray	g_arEventItemNew;

NpcSayArray			g_arNpcSayEventTable;

GuildFortressArray	g_arGuildFortress;

MonsterPsiArray		g_arMonsterPsi;
DressingDataArray	g_arDressingData;

MyServerArray		g_arMyServer;

CRoyalRumble		g_RR;
CString				g_strARKRRWinner;

CEventBlockingTable	*g_pEventBlock;

long				g_TotalNPC;
long				g_CurrentNPC;
long				g_CurrentNPCError;
long				g_CurrentGuildCount;

int					g_ItemSerialIndex;

struct	_StoreDB
{
	short	StoreID;
	short	ItemID;
};

typedef CTypedPtrArray <CPtrArray, _StoreDB*>	_StoreDBArray;

CFile g_fpSpeedHack;
CFile g_fpSpeedHack0;
CFile g_fpSpeedHack1;//±êÖ¾¼ÇÂ¼
CFile g_fpBackServer;
CFile g_fpGuildHouseRank;
CFile g_fpEventItem;
CFile g_fpGM;

CRITICAL_SECTION m_CS_EventItemLogFileWrite;

volatile long g_dwItemSerial;		// Item Logging
DWORD g_dwFortressTime;				// Æ÷Æ®¸®½ºÀü À¯Áö ½Ã°£
BYTE g_byMyServerIndex;

// IKING 2001.1.
extern CSocketManager	*g_pSQM;
extern CUserManager		*g_pUQM;
extern CSearch			*g_pUserList;
//extern CRITICAL_SECTION m_CS_ReceiveData[];
extern CRITICAL_SECTION m_CS_FileWrite;
// IKING 2002.1.
extern CRITICAL_SECTION m_CS_LoginData;
extern CRITICAL_SECTION m_CS_LogoutData;
extern CRITICAL_SECTION m_CS_SqlDataProcess;
extern CRITICAL_SECTION m_CS_SqlData;
extern CRITICAL_SECTION m_CS_EventItemLogFileWrite;

//////////////////////////////////////////////////////////////////////
// Fortress Paket Variable
extern CRITICAL_SECTION m_CS_FortressData;

extern DWORD g_ThreadAliveTime[];
extern CRoyalRumble		g_RR;
extern CString			g_strARKRRWinner;

ShopSystem g_Shop;
//

TCHAR g_arServerIPAddr[16];
extern DWORD g_ThreadErr[];

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent), m_InstanceChecker(_T("droiyan Server Single Instance"))
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_editCtrl = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nTimerID = 0;
	m_nEventItemTimerID = 0;
	m_nAliveTimerID = 0;
	m_nBridgeTimerID = 0;

	// IKING 2001.1.
	m_nThreadCount = 0;
	m_bLogOutExit = FALSE;
	m_CurUserCount = 0;

//	for ( int i = 0; i < AUTOMATA_THREAD+1; i++ )
//	{
//		InitializeCriticalSection(&m_CS_ReceiveData[i]);
//		m_ThreadNo[i] = i;
//	}

	InitializeCriticalSection(&m_CS_FileWrite);
	InitializeCriticalSection(&m_CS_LoginData);
	InitializeCriticalSection(&m_CS_LogoutData);
	InitializeCriticalSection(&m_CS_SqlDataProcess);
	InitializeCriticalSection(&m_CS_SqlData);
	InitializeCriticalSection(&m_CS_EventItemLogFileWrite);

	// Fortress Packet CRT
	InitializeCriticalSection(&m_CS_FortressData);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_ZONE, m_staticZone);
	DDX_Control(pDX, IDC_PORT, m_staticPort);
	DDX_Control(pDX, IDC_INDEX, m_staticIndex);
	DDX_Control(pDX, IDC_CLIENT_VER, m_staticClientVer);
	DDX_Text(pDX, IDC_EDIT, m_editCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)     
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE( UM_START_SERVER, OnStartAutomata )
	ON_MESSAGE(WM_CLOSE_PROCESS, OnCloseProcess)
	ON_BN_CLICKED(IDQUANOUT, &CServerDlg::OnBnClickedQuanout)//È«ÏÂÏß°´Å¦
	ON_BN_CLICKED(IDC_NEW_ANNOUNCE, &CServerDlg::OnBnClickedNewAnnounce)//ÖØ¶Á±¬ÂÊ
	ON_BN_CLICKED(IDC_NEW_ANNOUNCE2, &CServerDlg::OnBnClickedNewAnnounce2)//ÖØÔØini
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////////////////////////////////
//	MFC °¡ ¸¸µç ÃÊ±âÈ­
//
void CServerDlg::DefaultInit()
{
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
}

///////////////////////////////////////////////////////////////////////////////
//	°¢Á¾ ÃÊ±âÈ­
//
BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Default Init ...
	DefaultInit();
	if (g_Shop.ShopLoadConfig() == FALSE)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	int i, j;
	g_pMainDlg = this;

	// Åë½Å È¯°æ ÃÊ±âÈ­...
	if(!InitEnv())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	// RollBack Memory Open
	if(!OpenRollBack())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Memory DB Open
	if(!OpenMemoryDB())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!OpenMailQueue() )
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	TCHAR strConnect[256];

	wsprintf(strConnect, _T("ODBC;DSN=%s;UID=%s;PWD=%s"), "drgame", "drgame", "drgame");
//	wsprintf(strConnect, _T("ODBC;DSN=%s;UID=%s;PWD=%s"), "drgame", "drgame", "drgame");
	for ( i = 0; i < AUTOMATA_THREAD+1;i++)
	{
//		g_DB[i].Init(5);	
		g_DB[i].Init(10);	
		g_DB[i].DBConnect(strConnect);
	}
	
	wsprintf(strConnect, _T("ODBC;DSN=%s;UID=%s;PWD=%s"), "drgamenew", "drgame", "drgame");
//	wsprintf(strConnect, _T("ODBC;DSN=%s;UID=%s;PWD=%s"), "drgamenew", "drgame", "drgame");
	for ( i = 0; i < AUTOMATA_THREAD+1;i++)
	{
//		g_DBNew[i].Init(2);
		g_DBNew[i].Init(10);
		g_DBNew[i].DBConnect(strConnect);
	}

	TCHAR strConnect2[256];
	wsprintf(strConnect2, _T("ODBC;DSN=%s;UID=%s;PWD=%s"), "droiyan", "droiyan", "droiyan");
//	wsprintf(strConnect, _T("ODBC;DSN=%s;UID=%s;PWD=%s"), "Droiyan", "droiyan", "droiyan");
	for ( i = 0; i < AUTOMATA_THREAD+1;i++)
	{
//		g_DBSession[i].Init(2);
		g_DBSession[i].Init(5);
		g_DBSession[i].DBConnect(strConnect2);
	}

	CheckRollBack();

	// Error Log Init
	g_ErrorLog.OpenFile();

	// Speed Hack File
	g_fpSpeedHack.Open( "./QQ165678867/SpeedHackList.txt", CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone );
	g_fpSpeedHack.SeekToEnd();
		// Speed Hack File
	g_fpSpeedHack0.Open( "·Ç·¨¼ì²â.txt", CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone );
	g_fpSpeedHack0.SeekToEnd();
	g_fpSpeedHack1.Open( "±êÖ¾¼ÇÂ¼.txt", CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone );
	g_fpSpeedHack1.SeekToEnd();

	g_fpGM.Open( "GM²Ù×÷¼ÇÂ¼.txt", CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone );
	g_fpGM.SeekToEnd();


	g_fpBackServer.Open( "./QQ165678867/BackServerLog.txt", CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone );
	g_fpBackServer.SeekToEnd();

	g_fpGuildHouseRank.Open( "./QQ165678867/GuildHouseRankLog.txt", CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone );
	g_fpGuildHouseRank.SeekToEnd();

	g_fpEventItem.Open( "./QQ165678867/EventItemLog.txt", CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone );
	g_fpEventItem.SeekToEnd();

	// Read Ini File ...
	TCHAR strPath[_MAX_PATH + 1];
	::GetCurrentDirectory(_MAX_PATH, strPath);
	CString szIni = strPath;
	szIni += _T("\\DrOnline.ini");
	int nSID = GetPrivateProfileInt(_T("INDEX"), _T("ME"), 0, (LPCTSTR)szIni);
	g_ItemSerialIndex = GetPrivateProfileInt( _T("INDEX"), _T("SERIAL"), 0, (LPCTSTR)szIni );

	int nCount = GetPrivateProfileInt(_T("SERVERS"), _T("COUNT"), 0, (LPCTSTR)szIni);

	GetPrivateProfileString( _T("BRIDGE"), _T("IP"), "192.168.2.100", m_strBridgeIP, (DWORD)sizeof(m_strBridgeIP), (LPCTSTR)szIni );
	m_nBridgePort = GetPrivateProfileInt( _T("BRIDGE"), _T("PORT"), 11000, (LPCTSTR)szIni );

	TCHAR strSID[10];
	int iServerIndex = -1;
	int* newServer = NULL;

	for( i = 1; i <= nCount; i++ )
	{
		sprintf( strSID, "%02d", i );

		iServerIndex = GetPrivateProfileInt( _T("SERVERS"), strSID, 0, (LPCTSTR)szIni );

		if( iServerIndex >= 0 && iServerIndex <= 12 )
		{
			newServer = new int;
			memcpy( newServer, &iServerIndex, sizeof(int) );

			g_arMyServer.Add( newServer );
		}
	}

	g_byMyServerIndex = nSID;

	//----------------------------------------------------------------------
	//	Sets a random number starting point.
	//----------------------------------------------------------------------
	srand((int)time(NULL) );
	for(i = 0; i < 10; i++) myrand(1, 10000);	// don't delete
    
	//----------------------------------------------------------------------
	//	yehuo.ini Open & Load ...
	//----------------------------------------------------------------------
	if(!GetLoadYeHuoINI())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	//----------------------------------------------------------------------
	//	Server Information DB Open & Load ...
	//----------------------------------------------------------------------
	if(!GetServerInfoData())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Zone(TownPotal) Information DB Open & Load ...
	//----------------------------------------------------------------------
	if(!GetZoneInfoData())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetTownPotalTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	//----------------------------------------------------------------------
	//	Find Server Index in Server Information
	//----------------------------------------------------------------------
	int nServerIndex = -1;
	for( i = 0; i < g_server.GetSize(); i++)
	{
		if(g_server[i]->m_sid == nSID)
		{
			nServerIndex = i;
			break;
		}
	}
	if(nServerIndex == -1) 
	{
		AfxMessageBox( IDS_SERVERDLG_SERVERINFOERROR );
		EndDialog(IDCANCEL);
	}

	//----------------------------------------------------------------------
	//	Load Zone Size
	//----------------------------------------------------------------------
	MAP*	pNewMapSize		= NULL;
	int		nZoneNumberSize;
	BOOL	bFoundDBIndex = FALSE;

	for( i = 0; i < g_server.GetSize(); i++ )
	{
		bFoundDBIndex = FALSE;

		if( g_server[i] )
		{
			if( g_server[i]->m_sDBIndex != -1 )		// -1ÀÌ¸é ¸ðµç À¯Àú¸¦ ¹ÞÀ» ¼ö ÀÖ´Â ¼­¹öÀÌ¹Ç·Î Á¤º¸ ÀÐ¾îµÐ´Ù, ¾Æ´Ï¶ó¸é...
			{
				for( j = 0; j < g_arMyServer.GetSize(); j++ )
				{
					if( *(g_arMyServer[j]) == g_server[i]->m_sDBIndex )	// ³»°¡ ÀÐ¾î¾ß ÇÏ´Â Á¤º¸ÀÏ °æ¿ì ÀÐ°Ô ÇÑ´Ù.
					{
						bFoundDBIndex = TRUE;
					}
				}

				if( !bFoundDBIndex ) continue;
			}
		}

		for( j = 0; j < g_server[i]->m_zone.GetSize(); j++ )
		{
			pNewMapSize = new MAP;
			nZoneNumberSize = *(g_server[i]->m_zone[j]);

			if( !pNewMapSize->LoadMapSizeByNum( nZoneNumberSize ) )
			{
				delete pNewMapSize;
				AfxMessageBox( "Error in load zone size" );
				return FALSE;
			}

			g_zonesize.Add( pNewMapSize );
		}
	}

	//----------------------------------------------------------------------
	//	Load Zone & Event...
	//----------------------------------------------------------------------
	MAP*	pNewMap		= NULL;
	CELL*	pNewCell	= NULL;
	EVENT*	pNewEvent	= NULL;
	
	CString szZone = _T(""), szTemp = _T("");
	int nZoneSize = g_server[nServerIndex]->m_zone.GetSize();
	int nZoneNumber;
	BOOL bRet = TRUE;

	for( i = 0; i < nZoneSize; i++)
	{
		// Map
		pNewMap = new MAP;
		nZoneNumber = *(g_server[nServerIndex]->m_zone[i]);
		TRACE("%d  ",nZoneNumber);
		if(!pNewMap->LoadMapByNum(nZoneNumber))
		{
			bRet = FALSE;
			delete pNewMap;
			continue;
		}
		g_zone.Add( pNewMap );

		// Cell
		pNewCell = new CELL(pNewMap->m_sizeMap, pNewMap->m_Zone, &m_Com);
		g_cell.Add(pNewCell);

		// Event
		pNewEvent = new EVENT;
		if(!pNewEvent->LoadEvent(nZoneNumber))
		{
			bRet = FALSE;
			delete pNewEvent;
			continue;
		}
		g_event.Add( pNewEvent );

		szTemp.Format( "%d ", nZoneNumber );
		szZone += szTemp;
	}

	if(bRet == FALSE) 
	{
		szTemp.Format("[ %d ] Map, Event Information Loding Fail!!!!", nZoneNumber);
		AfxMessageBox(szTemp);
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Communication Part Initialize ...
	//----------------------------------------------------------------------
	m_Com.Init(g_server[nServerIndex]->m_nPort);

	//----------------------------------------------------------------------
	//	Load Item & Skill & PSI
	//----------------------------------------------------------------------
	if( !GetItemTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

//	AfxMessageBox("8");

	if( !GetMagicItemTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if( !GetEBodyTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	if( !GetEBodyUpgradeTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	if( !GetEBodyIdentifyTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	if( !GetHuanshiTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	if(!GetMESSAGETable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
    if( !GetOnlineShopTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if( !GetKaixiangziTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if( !GetSxOnlineShopTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	if( !GetOnlineRMBShopTable())//ÔÚÏßÏûºÄÆ·ÉÌµê
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	if( !GetSkillTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if( !GetPsiTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

//	AfxMessageBox("9");

	//----------------------------------------------------------------------
	//	Load »ç¿ëÀÚ¿¡°Ô ÁÙ ÃÊ±â ¾ÆÀÌÅÛ Table
	//----------------------------------------------------------------------
	if( !GetIntiItemTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load Remodeling Table
	//----------------------------------------------------------------------
	if(!GetRemodelingTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load BoxEvent Table
	//----------------------------------------------------------------------
	if(!GetBoxEventTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Guild Data¸¦ ÀÐ¾îµéÀÎ´Ù.
	//----------------------------------------------------------------------
	if(!GetGuildTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetGuildUserData())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load Level Up Table
	//----------------------------------------------------------------------
	if(!GetLevelUpTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetPAMAExpTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load HP,SP,PP Recover Table
	//----------------------------------------------------------------------
	if(!GetUserRecoverTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetUserRecoverRateTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load Guild House Rank Table...
	//----------------------------------------------------------------------
	if(!GetGuildHouseRankTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetVirtualRoomTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetGuildHouseTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load Store Table
	//----------------------------------------------------------------------
	if(!GetStoreTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetPsiStoreTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetGuildStoreTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetGuildMapInfoTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetGuildFortressTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetGuildFortressTaxTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load NPC Item Table
	//----------------------------------------------------------------------
	if(!GetNpcItemTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	if(!GetNpcItemNewTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	if(!get_convert_table())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	//----------------------------------------------------------------------
	//	Load NPC Chat Table
	//----------------------------------------------------------------------
	if(!GetNpcChatTable())
	{
		AfxMessageBox("### Fail To Read NPC_CHAT!! ###");
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load NPC_SAY_EVENT Table
	//----------------------------------------------------------------------
	if(!GetNpcSayEventTable())
	{
		AfxMessageBox("### Fail To Read NPC_SAY_EVENT!! ###");
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load NPC Data & Activate NPC
	//----------------------------------------------------------------------
	if(!GetNpcTableData())		// NPC Æ¯¼ºÄ¡ Å×ÀÌºí Load
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	//AfxMessageBox("1");
	if(!GetCityNpcTableData())		// NPC Æ¯¼ºÄ¡ Å×ÀÌºí Load
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	//AfxMessageBox("2");
	if(!CreateNpcThread())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
//	AfxMessageBox("3");
	//----------------------------------------------------------------------
	//	Load NPC DN Table
	//----------------------------------------------------------------------
	if(!GetMonHaveDNTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load NPC VAL_ITEM Table
	//----------------------------------------------------------------------
	if(!GetValItemTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load Monster Psi Table
	//----------------------------------------------------------------------
	if(!GetMonsterPsiTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Load Dressing Table
	//----------------------------------------------------------------------
	if(!GetDressingTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	Add Point User_Level Table
	//----------------------------------------------------------------------
	if(!GetUserLevelUpTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	ÀÌº¥Æ® Å×ÀÌºí
	//----------------------------------------------------------------------
	if(!GetEventItemTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetAddEventItemTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetEventItemNewTable())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!GetEventBlockData())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
/*
	//----------------------------------------------------------------------
	//	Royal Rumble Initialize
	//----------------------------------------------------------------------
	if( !GetRoyalRumbleData() )
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
*/
	//----------------------------------------------------------------------
	//	Catch Server Local IP Address...(218.50.5. bandwith)
	//----------------------------------------------------------------------
	if(!GetLocalIPAddress())
	{
		AfxMessageBox("Error GetLocalIPAddress()");
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//----------------------------------------------------------------------
	//	To Remove Users In CURRENTUSER Table for revival Server
	//----------------------------------------------------------------------
	if(!RemoveSessionCurrentUser())
	{
		AfxMessageBox("Error 2");
		EndDialog(IDCANCEL);
		return FALSE;
	}

	g_dwItemSerial = 0;		// Item Logging

	g_dwFortressTime = 30;//ÒªÈûÊ±¼ä

	//----------------------------------------------------------------------
	//	Show current Server Setting ...
	//----------------------------------------------------------------------
	SetIndexText(nServerIndex);
	SetPortText(g_server[nServerIndex]->m_nPort);
	//SetPortText(8900);
	SetZoneText(szZone);

	//----------------------------------------------------------------------
	//	Start NPC THREAD
	//----------------------------------------------------------------------
	ResumeAI();

	//----------------------------------------------------------------------
	//	Start Accepting...
	//----------------------------------------------------------------------
//	do
//	{
//		if(g_TotalNPC == (g_CurrentNPC + g_CurrentNPCError)) break;
//
//	} while(TRUE);
//
//	TRACE("ÃÑ %d NPC Áß ÇöÀç %d NPC ÃÊ±âÈ­°¡ ³¡³µ½À´Ï´Ù.\n", g_TotalNPC, g_CurrentNPC);
	
	//m_Com.StartAccepting();

	if( !InitBridgeServerConnection() )
	{
		AfxMessageBox( "Can't open to bridge server" );
		return FALSE;
	}

	m_nTimerID = SetTimer(10000, 1000*60, NULL);

	m_nEventItemTimerID = SetTimer( 10001, 60 * 1000, NULL );				// ÀÌº¥Æ® ¾ÆÀÌÅÛÀ» »Ñ¸®±â À§ÇÑ Å¸ÀÌ¸Ó
	
	m_nBridgeTimerID = SetTimer( 10002, 1000, NULL );

	//track this instance so any other instances can find us.
	m_InstanceChecker.TrackFirstInstanceRunning();

	// Åë½Å ½º·¹µå ÃÊ±âÈ­...
	InitThreads();

	// »ç¿ëÀÚ Á¢¼Ó ÃÊ±âÈ­...
	if ( !m_UserBase.Listen( g_server[nServerIndex]->m_nPort ) )  //PORT_ADD_ZERO
		return FALSE;
	//
	//if ( !m_UserBase.Listen( 8900 ) )  //PORT_ADD_ZERO
	//	return FALSE;
	//m_UserBase.m_bAcceptEnableFlag = TRUE;

	// Å¸ÀÌ¸Ó ¼¼ÆÃ...
	m_nAliveTimerID = SetTimer( ALIVE_FILL_TIMER, 30000, NULL );

	// À¯Àú¼ö Ä«¿îÆÃ
	m_iUserCountCount = 0;

//	PostMessage(UM_START_SERVER,0,0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CServerDlg::GetLoadYeHuoINI()
{
	TCHAR strPath[_MAX_PATH + 1];
	::GetCurrentDirectory(_MAX_PATH, strPath);
	CString szfilename = strPath;
	szfilename += _T("\\QQ165678867.ini");	//yehuo.ini
	CString tttmp;
		
		YhuoIni *yhini = new YhuoIni;
		GetPrivateProfileString("WEBURL", "WEB_URL",  "", tttmp.GetBuffer(MAX_PATH),MAX_PATH, szfilename);	strcpy_s(yhini->WEB_URL,tttmp);
		GetPrivateProfileString("EXP", "50¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_50  = atoi(tttmp);
		GetPrivateProfileString("EXP", "60¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_60  = atoi(tttmp);
		GetPrivateProfileString("EXP", "70¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_70  = atoi(tttmp);
		GetPrivateProfileString("EXP", "80¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_80  = atoi(tttmp);
		GetPrivateProfileString("EXP", "90¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_90  = atoi(tttmp);
		GetPrivateProfileString("EXP", "100¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_100  = atoi(tttmp);
		GetPrivateProfileString("EXP", "110¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_110  = atoi(tttmp);
		GetPrivateProfileString("EXP", "120¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_120  = atoi(tttmp);
		GetPrivateProfileString("EXP", "130¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_130  = atoi(tttmp);
		GetPrivateProfileString("EXP", "140¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_140  = atoi(tttmp);
		GetPrivateProfileString("EXP", "150¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_150  = atoi(tttmp);
		GetPrivateProfileString("EXP", "155¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_155  = atoi(tttmp);
		GetPrivateProfileString("EXP", "160¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_160  = atoi(tttmp);
		GetPrivateProfileString("EXP", "165¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_165  = atoi(tttmp);
		GetPrivateProfileString("EXP", "170¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_170  = atoi(tttmp);
		GetPrivateProfileString("EXP", "175¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_175  = atoi(tttmp);
		GetPrivateProfileString("EXP", "180¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_180  = atoi(tttmp);
		GetPrivateProfileString("EXP", "185¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_185  = atoi(tttmp);
		GetPrivateProfileString("EXP", "190¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_190  = atoi(tttmp);
		GetPrivateProfileString("EXP", "195¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_195  = atoi(tttmp);
		GetPrivateProfileString("EXP", "200¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->EXP_200  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "°æ±¾×ª»» ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->bbzh  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "×î¸ßµÈ¼¶ ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->zgdj  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÃÜÂë±£»¤¿ª¹Ø ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->mimabaohu  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÈËÎïºÍ¹ÖÎïÏà²îµÈ¼¶ÏÞÖÆ ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->djxz  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÒªÈû¼ä¸ôÌìÊý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->ysjg  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÅÝÐË·ÜÐÒÔËÐÒ¸£ÒªÇóµÈ¼¶ ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->pjy  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "×°±¸ÎäÆ÷×î¸ß¸ÄÊý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->gaishu  = atoi(tttmp);
        GetPrivateProfileString("ÉèÖÃ", "×°±¸ÊôÐÔÊÇ·ñÖØµþ ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->chongdie  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÐÒÔËµôÀ¶×°±¸±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->xyl  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÐÒÔËµô»Æ×°±¸±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->xyh  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÆÕÍ¨µôÀ¶×°±¸±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->lan  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÆÕÍ¨µô»Æ×°±¸±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->huang  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "³¬ÄÜ¿ª¹Ø ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->chaoneng  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "·Ç·¨¼ì²â¿ª¹Ø ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->jiance  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÄÚ²â¿ª¹Ø ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->neice  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "¸±ÊÖÎäÆ÷¼ÆËã ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		yhini->FSWEEP  = atoi(tttmp);
		GetPrivateProfileString("ÃüÁî·û", "Ë¢ÎïÆ·",  "", tttmp.GetBuffer(MAX_PATH),MAX_PATH, szfilename);     strcpy(yhini->MAKE,tttmp);//Ë¢×°±¸
		GetPrivateProfileString("ÃüÁî·û", "Ë¢½ðÇ®",  "", tttmp.GetBuffer(MAX_PATH),MAX_PATH, szfilename);     strcpy(yhini->MONEY,tttmp);//Ë¢Ç®
		
		 o_yehuoini.Add(yhini);
	return TRUE;
}
BOOL CServerDlg::LoadYeHuoINI() //ÖØÔØini
{
	TCHAR strPath[_MAX_PATH + 1];
	::GetCurrentDirectory(_MAX_PATH, strPath);
	CString szfilename = strPath;
	szfilename += _T("\\QQ165678867.ini");	//yehuo.ini  
	CString tttmp;
		
		
		GetPrivateProfileString("EXP", "50¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_50  = atoi(tttmp);
		GetPrivateProfileString("EXP", "60¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_60  = atoi(tttmp);
		GetPrivateProfileString("EXP", "70¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_70  = atoi(tttmp);
		GetPrivateProfileString("EXP", "80¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_80  = atoi(tttmp);
		GetPrivateProfileString("EXP", "90¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_90  = atoi(tttmp);
		GetPrivateProfileString("EXP", "100¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_100  = atoi(tttmp);
		GetPrivateProfileString("EXP", "110¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_110  = atoi(tttmp);
		GetPrivateProfileString("EXP", "120¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_120  = atoi(tttmp);
		GetPrivateProfileString("EXP", "130¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_130  = atoi(tttmp);
		GetPrivateProfileString("EXP", "140¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_140  = atoi(tttmp);
		GetPrivateProfileString("EXP", "150¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_150  = atoi(tttmp);
		GetPrivateProfileString("EXP", "155¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_155  = atoi(tttmp);
		GetPrivateProfileString("EXP", "160¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_160  = atoi(tttmp);
		GetPrivateProfileString("EXP", "165¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_165  = atoi(tttmp);
		GetPrivateProfileString("EXP", "170¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_170  = atoi(tttmp);
		GetPrivateProfileString("EXP", "175¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_175  = atoi(tttmp);
		GetPrivateProfileString("EXP", "180¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_180  = atoi(tttmp);
		GetPrivateProfileString("EXP", "185¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_185  = atoi(tttmp);
		GetPrivateProfileString("EXP", "190¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_190  = atoi(tttmp);
		GetPrivateProfileString("EXP", "195¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_195  = atoi(tttmp);
		GetPrivateProfileString("EXP", "200¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->EXP_200  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "°æ±¾×ª»» ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->bbzh  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "×î¸ßµÈ¼¶ ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->zgdj  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÃÜÂë±£»¤¿ª¹Ø ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->mimabaohu  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÈËÎïºÍ¹ÖÎïÏà²îµÈ¼¶ÏÞÖÆ ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->djxz  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÒªÈû¼ä¸ôÌìÊý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->ysjg  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÅÝÐË·ÜÐÒÔËÐÒ¸£ÒªÇóµÈ¼¶ ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->pjy  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "×°±¸ÎäÆ÷×î¸ß¸ÄÊý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->gaishu  = atoi(tttmp);
        GetPrivateProfileString("ÉèÖÃ", "×°±¸ÊôÐÔÊÇ·ñÖØµþ ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->chongdie  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÐÒÔËµôÀ¶×°±¸±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->xyl  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÐÒÔËµô»Æ×°±¸±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->xyh  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÆÕÍ¨µôÀ¶×°±¸±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->lan  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÆÕÍ¨µô»Æ×°±¸±¶Êý ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->huang  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "³¬ÄÜ¿ª¹Ø ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->chaoneng  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "·Ç·¨¼ì²â¿ª¹Ø ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->jiance  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "ÄÚ²â¿ª¹Ø ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->neice  = atoi(tttmp);
		GetPrivateProfileString("ÉèÖÃ", "¸±ÊÖÎäÆ÷¼ÆËã ",  "", tttmp.GetBuffer(MAX_PATH),24, szfilename);		o_yehuoini[0]->FSWEEP  = atoi(tttmp);
		
		// o_yehuoini.Add(yhini);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////
//	Server Information DB Open & Load ...
//
BOOL CServerDlg::GetServerInfoData()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	int i, iNum = 0;
	int* pZone = NULL;

	SQLINTEGER	sInd;
	SQLSMALLINT sSid, sPort, sZone, sDBIndex;
	SQLCHAR		strAddr[16];

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strAddr, sizeof(strAddr));

	sSid =sPort = sZone = 0;
	sDBIndex = -1;
	sInd = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_SERVERINFO}"));

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild_Store Attack List Data !!\n");

//		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );
			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				i = 1;

				SERVERINFO* pNewData = new SERVERINFO;

				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sSid, sizeof(SQLSMALLINT), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sDBIndex, sizeof(SQLSMALLINT), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  strAddr,sizeof(strAddr),	  &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sPort, sizeof(SQLSMALLINT),&sInd );

				while(TRUE)
				{
					retcode = SQLGetData( hstmt,i++,SQL_C_SSHORT,&sZone,sizeof(SQLSMALLINT),&sInd );

					if( retcode != SQL_SUCCESS ) break;

					iNum = pNewData->m_zone.GetSize();
					if(iNum >= 1)
					{						
						if( sZone == *(pNewData->m_zone[iNum - 1]) ) break;
						if( sZone == -1 ) continue;

						if(i > 1000) return FALSE;
					}
					
					pZone = new int;	
					*pZone = (int)sZone;
					pNewData->m_zone.Add(pZone);
					TRACE("%d ",sZone);
					sZone = -1;
				};

				pNewData->m_sid		= sSid;			// Field 0
				pNewData->m_sDBIndex= sDBIndex;		// Field 1
				pNewData->m_szAddr	= strAddr;		// Field 2
				pNewData->m_nPort	= sPort;		// Field 3

				g_server.Add(pNewData);
			}
			else break;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	Zone Town Potal Information DB Open & Load ...
//
BOOL CServerDlg::GetZoneInfoData()
{
	CZoneInfoSet ZoneInfoSet;

	try 
	{
		if(ZoneInfoSet.IsOpen()) ZoneInfoSet.Close();
		if(!ZoneInfoSet.Open()) 
		{
			AfxMessageBox(_T("ZoneInfo DB Open Fail"));
			return FALSE;
		}
		if(ZoneInfoSet.IsBOF()) 
		{
			AfxMessageBox(_T("ZoneInfo DB Empty"));
			return FALSE;
		}
		
		g_WeatherZone.RemoveAll();
		
		while(!ZoneInfoSet.IsEOF())
		{
			ZONEINFO* pNewData = new ZONEINFO;
			
			pNewData->iSid = ZoneInfoSet.m_sSid;
			pNewData->iZone = ZoneInfoSet.m_sZone;
			pNewData->iPkMode = (int)ZoneInfoSet.m_tPkMode;
			pNewData->iPotalX = ZoneInfoSet.m_sPotalX;
			pNewData->iPotalY = ZoneInfoSet.m_sPotalY;
			pNewData->iPotalX1 = ZoneInfoSet.m_sPotalX1;
			pNewData->iPotalY1 = ZoneInfoSet.m_sPotalY1;
			pNewData->iPotalZone = ZoneInfoSet.m_sPotalZone;

			if(ZoneInfoSet.m_tWeather == 1) SetWeatherZone(ZoneInfoSet.m_sZone);

			g_TownPotal.Add(pNewData);
			ZoneInfoSet.MoveNext();
		}

		ZoneInfoSet.Close();
		return TRUE;
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
//	¸Þ¸ð¸® Á¤¸®
//
BOOL CServerDlg::DestroyWindow() 
{

//	USER *pUser = NULL;
	int i, j, x, y;


	if(g_fpSpeedHack.m_hFile != CFile::hFileNull) g_fpSpeedHack.Close();
	if(g_fpBackServer.m_hFile != CFile::hFileNull) g_fpBackServer.Close();
	if(g_fpGuildHouseRank.m_hFile != CFile::hFileNull) g_fpGuildHouseRank.Close();
	if(g_fpEventItem.m_hFile != CFile::hFileNull) g_fpEventItem.Close();

	if(m_nTimerID) KillTimer(10000);
	if(m_nEventItemTimerID) KillTimer(10001);

	// Alive Å¸ÀÌ¸Ó Áö¿ì±â...
	if(m_nAliveTimerID) KillTimer(ALIVE_FILL_TIMER);

	if( m_nBridgeTimerID ) KillTimer( 10002 );

	g_bNpcExit = TRUE;
	// rollback yehuoini
	for(i = 0; i < o_yehuoini.GetSize();i++)
	{
		if(o_yehuoini[i]) 
		{
			delete o_yehuoini[i];
		}
	}
	o_yehuoini.RemoveAll();
	

    // RollBack Shared Memory ¿¡ Á¤»ó Á¾·áÀÓÀ» ±â·Ï
	if(m_MemRollBack.m_hMapping != NULL)
	{
		TCHAR* pData = (TCHAR*)m_MemRollBack.m_lpData;
		*pData = 0;
	}

	// User DB Shared Memory Á¦°Å
	for(i = 0; i < g_arSharedMemory.GetSize(); i++)
	{
		if( g_arSharedMemory[i] )
		{
			delete g_arSharedMemory[i];
		}
	}
	g_arSharedMemory.RemoveAll();

	for(i = 0; i < g_arStoreSharedMemory.GetSize(); i++)
	{
		if( g_arStoreSharedMemory[i] )
		{
			delete g_arStoreSharedMemory[i];
		}
	}
	g_arStoreSharedMemory.RemoveAll();

	for(i = 0; i < g_arFortressSharedMemory.GetSize(); i++)
	{
		if( g_arFortressSharedMemory[i] )
		{
			delete g_arFortressSharedMemory[i];
		}
	}
	g_arFortressSharedMemory.RemoveAll();

	for(i = 0; i < g_arGuildHouseSharedMemory.GetSize(); i++)
	{
		if( g_arGuildHouseSharedMemory[i] )
		{
			delete g_arGuildHouseSharedMemory[i];
		}
	}
	g_arGuildHouseSharedMemory.RemoveAll();

	for(i = 0; i < g_arNpcThread.GetSize(); i++)
	{
		WaitForSingleObject(g_arNpcThread.GetAt(i)->m_pThread->m_hThread, INFINITE);
	}

	for(i = 0; i < g_arEventNpcThread.GetSize(); i++)
	{
		WaitForSingleObject(g_arEventNpcThread.GetAt(i)->m_pThread->m_hThread, INFINITE);
	}

	// ¸ÞÀÏ ¿¡ÀÌÀüÆ® °øÀ¯¸Þ¸ð¸® ÇØÁ¦
	CloseMailQueue();

	// ¾ÆÀÌÅÛ DBÅ×ÀÌºí »èÁ¦ 2001.1.17
	for( i = 0; i < g_DBItemArray.GetSize(); i++)
	{
		if( g_DBItemArray[i] )
		{
			delete g_DBItemArray[i];
		}
	}
	g_DBItemArray.RemoveAll();

	// Item Table Array Delete ...
	for(i = 0; i < g_arItemTable.GetSize(); i++)
	{
		if(g_arItemTable[i]) 
		{
			delete g_arItemTable[i];
		}
	}
	g_arItemTable.RemoveAll();

	// Magic Item Table Array Delete ...
	for(i = 0; i < g_arMagicItemTable.GetSize(); i++)
	{
		if(g_arMagicItemTable[i]) 
		{
			delete g_arMagicItemTable[i];
		}
	}
	g_arMagicItemTable.RemoveAll();

	// EBody Table Array Delete ...
	for(i = 0; i < g_arEBodyTable.GetSize(); i++)
	{
		if(g_arEBodyTable[i]) 
		{
			delete g_arEBodyTable[i];
		}
	}
	g_arEBodyTable.RemoveAll();

	// EBody Upgrade Table Array Delete ...
	for(i = 0; i < g_arEBodyUpgradeTable.GetSize(); i++)
	{
		if(g_arEBodyUpgradeTable[i]) 
		{
			delete g_arEBodyUpgradeTable[i];
		}
	}
	g_arEBodyUpgradeTable.RemoveAll();

	// EBody Identify Table Array Delete ...
	for(i = 0; i < g_arEBodyIdentifyTable.GetSize(); i++)
	{
		if(g_arEBodyIdentifyTable[i]) 
		{
			delete g_arEBodyIdentifyTable[i];
		}
	}
	g_arEBodyIdentifyTable.RemoveAll();
    //»ÃÊ¯
	for(i = 0; i < g_arHuanshiTable.GetSize(); i++)
	{
		if(g_arHuanshiTable[i]) 
		{
			delete g_arHuanshiTable[i];
		}
	}
	g_arHuanshiTable.RemoveAll();

	for(i = 0; i < g_arMessAgeArray.GetSize(); i++)
	{
		if(g_arMessAgeArray[i]) 
		{
			delete g_arMessAgeArray[i];
		}
	}
	g_arMessAgeArray.RemoveAll();

	//onlineshop array
	for(i = 0; i < g_arOnlineShopTable.GetSize(); i++)
	{
		if( g_arOnlineShopTable[i] )
		{
			delete g_arOnlineShopTable[i];
		}
	}
	g_arOnlineShopTable.RemoveAll();
	for(i = 0; i < g_arOnlineRMBShopTable.GetSize(); i++)
	{
		if(g_arOnlineRMBShopTable[i]) 
		{
			//delete g_arOnlineRMBShopTable[i];
		}
	}
	g_arOnlineRMBShopTable.RemoveAll();
	//¿ªÏä×Ó
	for(i = 0; i < g_arKaixiangziTable.GetSize(); i++)
	{
		if( g_arKaixiangziTable[i] )
		{
			delete g_arKaixiangziTable[i];
		}
	}
	g_arKaixiangziTable.RemoveAll();
	// Accessori Upgrade Table Array Delete ...
	for(i = 0; i < g_arAccessoriUpTable.GetSize(); i++)
	{
		if(g_arAccessoriUpTable[i]) 
		{
			delete g_arAccessoriUpTable[i];
		}
	}
	g_arAccessoriUpTable.RemoveAll();

	//
	// Remodeling Table Array Delete -----------------------------------------
	for(i = 0; i < g_arRemodelingTable1.GetSize(); i++)
	{
		if(g_arRemodelingTable1[i]) 
		{
			delete g_arRemodelingTable1[i];
		}
	}
	g_arRemodelingTable1.RemoveAll();

	for(i = 0; i < g_arRemodelingTable2.GetSize(); i++)
	{
		if(g_arRemodelingTable2[i]) 
		{
			delete g_arRemodelingTable2[i];
		}
	}
	g_arRemodelingTable2.RemoveAll();

	for(i = 0; i < g_arRemodelingTable3.GetSize(); i++)
	{
		if(g_arRemodelingTable3[i]) 
		{
			delete g_arRemodelingTable3[i];
		}
	}
	g_arRemodelingTable3.RemoveAll();

	g_arRemodelingRandomIndex1.RemoveAll();
	//-------------------------------------------------------------------------

	// Box Event Table Array Delete
	for(i = 0; i < g_arBoxEventTable.GetSize(); i++)
	{
		if(g_arBoxEventTable[i]) 
		{
			delete g_arBoxEventTable[i];
		}
	}
	g_arBoxEventTable.RemoveAll();

	// Summon Table Array Delete
	for(i = 0; i < g_arSummonTable.GetSize(); i++)
	{
		if(g_arSummonTable[i]) 
		{
			delete g_arSummonTable[i];
		}
	}
	g_arSummonTable.RemoveAll();
	
	// »ç¿ëÀÚ¿¡°Ô Ã³À½ ¾ÆÀÌÅÛÀ» ÁÜ
	for(i = 0; i < g_arInitItem.GetSize(); i++)
	{
		if(g_arInitItem[i])
		{
			delete g_arInitItem[i];
		}
	}	
	g_arInitItem.RemoveAll();

	// Npc Chat Array Delete ...
	for( i = 0; i < g_arNpcChat.GetSize(); i++)
	{
		if( g_arNpcChat[i] )
		{
			if(g_arNpcChat[i]->m_strTalk != NULL)
			{
				delete [] g_arNpcChat[i]->m_strTalk;
				g_arNpcChat[i]->m_strTalk = NULL;
			}
			delete g_arNpcChat[i];
		}
	}
	g_arNpcChat.RemoveAll();

	// Skill Table Array Delete ...
	for(i = 0; i < g_arSkillTable.GetSize(); i++)
	{
		if(g_arSkillTable[i]) 
		{
			delete g_arSkillTable[i];
		}
	}
	g_arSkillTable.RemoveAll();

	// Psionic Table Array Delete ...
	for(i = 0; i < g_arPsiTable.GetSize(); i++)
	{
		if(g_arPsiTable[i]) 
		{
			delete g_arPsiTable[i];
		}
	}
	g_arPsiTable.RemoveAll();

	// Level Up Table Array Delete ...
	for(i = 0; i < g_arLevelUpTable.GetSize(); i++)
	{
		if(g_arLevelUpTable[i]) 
		{
			delete g_arLevelUpTable[i];
		}
	}
	g_arLevelUpTable.RemoveAll();

	// Store Table Array Delete ...
	for(i = 0; i < g_arStore.GetSize(); i++)
	{
		if(g_arStore[i]) 
		{
			delete g_arStore[i];
		}
	}
	g_arStore.RemoveAll();

	// Psi Store Table Array Delete ...
	for(i = 0; i < g_arPsiStore.GetSize(); i++)
	{
		if(g_arPsiStore[i]) 
		{
			delete g_arPsiStore[i];
		}
	}
	g_arPsiStore.RemoveAll();

	for(i = 0; i < g_arEBodyStore.GetSize(); i++)
	{
		if(g_arEBodyStore[i]) 
		{
			delete g_arEBodyStore[i];
		}
	}
	g_arEBodyStore.RemoveAll();
	
	for(i = 0; i < g_arMapTable.GetSize(); i++)
	{
		if(g_arMapTable[i]) 
		{
			delete g_arMapTable[i];
		}
	}
	g_arMapTable.RemoveAll();
	
	// PA, MA Level Up Table Array Delete ...
	for(i = 0; i < g_arPAMAExp.GetSize(); i++)
	{
		if(g_arPAMAExp[i]) 
		{
			delete g_arPAMAExp[i];
		}
	}
	g_arPAMAExp.RemoveAll();

	// ServerInfo array Delete ...
	for(i = 0; i < g_server.GetSize(); i++)
	{
		if(g_server[i])
		{
			delete g_server[i];
		}
	}
	g_server.RemoveAll();

	// ZoneInfo (TownPotal) Zone array Delete...
	for(i = 0; i < g_TownPotal.GetSize(); i++)
	{
		if(g_TownPotal[i])
		{
			delete g_TownPotal[i];
		}
	}
	g_TownPotal.RemoveAll();

	for(i = 0; i < g_RandomTownPotal.GetSize(); i++)
	{
		if(g_RandomTownPotal[i])
		{
			delete g_RandomTownPotal[i];
		}
	}
	g_RandomTownPotal.RemoveAll();
	
	// Wheather Zone array Delete...
	for(i = 0; i < g_WeatherZone.GetSize(); i++)
	{
		if(g_WeatherZone[i])
		{
			delete g_WeatherZone[i];
		}
	}
	g_WeatherZone.RemoveAll();

	// Map(Zone) Array Delete...
	for(i = 0; i < g_zone.GetSize(); i++)
	{
		if(g_zone[i])
		{			
			for(x = 0; x < g_zone[i]->m_sizeMap.cx; x++)
			{
				for(y = 0; y < g_zone[i]->m_sizeMap.cy; y++)
				{
					if(g_zone[i]->m_pMap[x][y].m_Item.GetSize() > 0)
					{
						for(j = 0; j < g_zone[i]->m_pMap[x][y].m_Item.GetSize(); j++)
						{
							if(g_zone[i]->m_pMap[x][y].m_Item[j]) delete g_zone[i]->m_pMap[x][y].m_Item[j];
						}
					}
				}
			}
			delete g_zone[i];
		}
	}
	g_zone.RemoveAll();

	// Cell Array Delete...
	for(i = 0; i < g_cell.GetSize(); i++)
	{
		if(g_cell[i])
		{
			delete g_cell[i];
		}
	}
	g_cell.RemoveAll();

	// Event Array Delete
	for(i = 0; i < g_event.GetSize(); i++)
	{
		if(g_event[i])
		{
			delete g_event[i];
		}
	}
	g_event.RemoveAll();

	// NpcTable Array Delete
	for(i = 0; i < g_arNpcTable.GetSize(); i++)
	{
		if(g_arNpcTable[i])
		{
			delete g_arNpcTable[i];
		}
	}
	g_arNpcTable.RemoveAll();

	// CityNpcTable Array Delete
	for(i = 0; i < g_arCityNpcTable.GetSize(); i++)
	{
		if(g_arCityNpcTable[i])
		{
			delete g_arCityNpcTable[i];
		}
	}
	g_arCityNpcTable.RemoveAll();
	
	// NpcThread Array Delete
	for(i = 0; i < g_arNpcThread.GetSize(); i++)
	{
		if(g_arNpcThread[i]) 
		{
			delete g_arNpcThread[i];
			g_arNpcThread[i] = NULL;
		}
	}
	g_arNpcThread.RemoveAll();

	// DNTable Array Delete
	for(i = 0; i < g_arDNTable.GetSize(); i++)
	{
		if(g_arDNTable[i])
		{
			delete g_arDNTable[i];
		}
	}
	g_arDNTable.RemoveAll();
	
	// User Status Ablility Array Delete
	for(i = 0; i < g_arRecoverTable.GetSize(); i++)
	{
		if(g_arRecoverTable[i])
		{
			delete g_arRecoverTable[i];
		}
	}
	g_arRecoverTable.RemoveAll();

	// User Status Ablility Rate Array Delete
	for(i = 0; i < g_arRecoverRateTable.GetSize(); i++)
	{
		if(g_arRecoverRateTable[i])
		{
			delete g_arRecoverRateTable[i];
		}
	}
	g_arRecoverRateTable.RemoveAll();

	// NpcThread Array Delete
	for(i = 0; i < g_arEventNpcThread.GetSize(); i++)
	{
		if(g_arEventNpcThread[i]) 
		{
			delete g_arEventNpcThread[i];
			g_arEventNpcThread[i] = NULL;
		}
	}
	g_arEventNpcThread.RemoveAll();

	for( i = 0; i < g_arGuildData.GetSize(); i++)
	{
		if( g_arGuildData[i] )
		{
/*			for(j = 0; j < g_arGuildData[i]->m_arUser.GetSize(); j++)
			{
				if(g_arGuildData[i]->m_arUser[j]) delete g_arGuildData[i]->m_arUser[j];
			}

			g_arGuildData[i]->m_arUser.RemoveAll();
*/
			delete g_arGuildData[i];
		}
	}

	g_arGuildData.RemoveAll();

	for(i = 0; i < g_arGuildHouseWar.GetSize(); i++)
	{
		if(g_arGuildHouseWar[i]) 
		{
			delete g_arGuildHouseWar[i];
			g_arGuildHouseWar[i] = NULL;
		}
	}

	g_arGuildHouseWar.RemoveAll();

	for(i = 0; i < g_arVirtualRoom.GetSize(); i++)
	{
		if(g_arVirtualRoom[i]) 
		{
			delete g_arVirtualRoom[i];
			g_arVirtualRoom[i] = NULL;
		}
	}

	g_arVirtualRoom.RemoveAll();

	for(i = 0; i < g_arGuildHouse.GetSize(); i++)
	{
		if(g_arGuildHouse[i]) 
		{
			delete g_arGuildHouse[i];
			g_arGuildHouse[i] = NULL;
		}
	}

	g_arGuildHouse.RemoveAll();

	for(i = 0; i < g_arUserLevel.GetSize(); i++)
	{
		if(g_arUserLevel[i]) 
		{
			delete g_arUserLevel[i];
			g_arUserLevel[i] = NULL;
		}
	}

	g_arUserLevel.RemoveAll();

	for(i = 0; i < g_arNpcSayEventTable.GetSize(); i++)
	{
		if(g_arNpcSayEventTable[i]) 
		{
			delete g_arNpcSayEventTable[i];
			g_arNpcSayEventTable[i] = NULL;
		}
	}

	g_arNpcSayEventTable.RemoveAll();

	for(i = 0; i < g_arEventItemTable.GetSize(); i++)
	{
		if(g_arEventItemTable[i]) 
		{
			delete g_arEventItemTable[i];
			g_arEventItemTable[i] = NULL;
		}
	}
	g_arEventItemTable.RemoveAll();

	for(i = 0; i < g_arAddEventItemTable.GetSize(); i++)
	{
		if(g_arAddEventItemTable[i]) 
		{
			delete g_arAddEventItemTable[i];
			g_arAddEventItemTable[i] = NULL;
		}
	}
	g_arAddEventItemTable.RemoveAll();

	for(i = 0; i < g_arEventItemNew.GetSize(); i++)
	{
		if(g_arEventItemNew[i]) 
		{
			delete g_arEventItemNew[i];
			g_arEventItemNew[i] = NULL;
		}
	}
	g_arEventItemNew.RemoveAll();

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(g_arGuildFortress[i]) 
		{
			delete g_arGuildFortress[i];
			g_arGuildFortress[i] = NULL;
		}
	}
	g_arGuildFortress.RemoveAll();

	// MonsterPsi Array
	for(i = 0; i < g_arMonsterPsi.GetSize(); i++)
	{
		if(g_arMonsterPsi[i])
		{
			delete g_arMonsterPsi[i];
		}
	}
	g_arMonsterPsi.RemoveAll();

	// Dressing Array
	for(i = 0; i < g_arDressingData.GetSize(); i++)
	{
		if(g_arDressingData[i])
		{
			delete g_arDressingData[i];
		}
	}
	g_arDressingData.RemoveAll();

	for( i = 0; i < g_arMyServer.GetSize(); i++ )
	{
		if( g_arMyServer[i] )
		{
			delete g_arMyServer[i];
		}
	}
	g_arMyServer.RemoveAll();

	if( g_pEventBlock )
	{
		for( i = 0; i < g_pEventBlock->m_arBuyBlock.GetSize(); i++ )
		{
			if(g_pEventBlock->m_arBuyBlock[i]) delete g_pEventBlock->m_arBuyBlock[i];
		}

		for( i = 0; i < g_pEventBlock->m_arCureBlock.GetSize(); i++ )
		{
			if(g_pEventBlock->m_arCureBlock[i]) delete g_pEventBlock->m_arCureBlock[i];
		}

		for( i = 0; i < g_pEventBlock->m_arRepaireBlock.GetSize(); i++ )
		{
			if(g_pEventBlock->m_arRepaireBlock[i]) delete g_pEventBlock->m_arRepaireBlock[i];
		}

		for( i = 0; i < g_pEventBlock->m_arSellBlock.GetSize(); i++ )
		{
			if(g_pEventBlock->m_arSellBlock[i]) delete g_pEventBlock->m_arSellBlock[i];
		}

		for( i = 0; i < g_pEventBlock->m_arTelBlock.GetSize(); i++ )
		{
			if(g_pEventBlock->m_arTelBlock[i]) delete g_pEventBlock->m_arTelBlock[i];
		}

		delete g_pEventBlock;

		g_pEventBlock = NULL;
	}

	// ¼­¹ö Á¢¼Ó ¸®¼Ò½º Áö¿ì±â...
	if ( g_pSQM )
		delete g_pSQM;
	g_pSQM = NULL;

	// »ç¿ëÀÚ Á¢¼Ó ¸®¼Ò½º Áö¿ì±â...
	if ( g_pUQM )
		delete g_pUQM;
	g_pUQM = NULL;

	// ¼­Ä¡ ±¸Á¶ Áö¿ì±â...
	if ( g_pUserList )
		delete g_pUserList;
	g_pUserList = NULL;

	// Å©¸®Æ¼ÄÃ ¼¼¼Ç Áö¿ì±â...
//	for ( i = 0; i < AUTOMATA_THREAD+1; i++ )
//	{
//		DeleteCriticalSection(&m_CS_ReceiveData[i]);
//	}

	DeleteCriticalSection(&m_CS_FileWrite);
	DeleteCriticalSection(&m_CS_LoginData);
	DeleteCriticalSection(&m_CS_LogoutData);
	DeleteCriticalSection(&m_CS_SqlDataProcess);
	DeleteCriticalSection(&m_CS_SqlData);
	DeleteCriticalSection(&m_CS_EventItemLogFileWrite);

	// Fortress Packet CRT
	DeleteCriticalSection(&m_CS_FortressData);
	return CDialog::DestroyWindow();
}

////////////////////////////////////////////////////////////////////////
//	ÀÎµ¦½º Á¤º¸¸¦ º¸ÀÎ´Ù.
//
void CServerDlg::SetIndexText(int nIndex)
{
	CString szTemp = _T("");
	szTemp.Format(_T("Index: %d"), nIndex);
	m_staticIndex.SetWindowText(szTemp);
}

/////////////////////////////////////////////////////////////////////////
//	Listen Port Á¤º¸¸¦ º¸ÀÎ´Ù.
//
void CServerDlg::SetPortText(int nPort)
{
	CString szTemp = _T("");
	szTemp.Format(_T("Port: %d"), nPort);
	m_staticPort.SetWindowText(szTemp);
}

/////////////////////////////////////////////////////////////////////////
//	Zone Á¤º¸¸¦ º¸ÀÎ´Ù.
//
void CServerDlg::SetZoneText(CString szList)
{
	CString szTemp = _T("Zone: ");
	szTemp += szList;
	m_staticZone.SetWindowText(szTemp);
}

/////////////////////////////////////////////////////////////////////////
//	NPC Item Table À» ÀÐ´Â´Ù.
//
BOOL CServerDlg::GetNpcItemTable()
{
	CNpcItemSet NpcItemSet;
	CDBVariant varValue;
	int nRowCount = 0;
	int i;
	int nItem = 0;

	try
	{
		if(NpcItemSet.IsOpen()) NpcItemSet.Close();

		NpcItemSet.m_strSort = _T("sIndex");

		if(!NpcItemSet.Open())
		{
			AfxMessageBox(_T("MONSTER ITEM DB Open Fail!"));
			return FALSE;
		}

		if(NpcItemSet.IsBOF()) 
		{
			AfxMessageBox(_T("MONSTER ITEM DB Empty!"));
			return FALSE;
		}

		while(!NpcItemSet.IsEOF())
		{
			nRowCount++;
			NpcItemSet.MoveNext();
		}

		g_NpcItem.m_nField = NpcItemSet.m_nFields;
		g_NpcItem.m_nRow = nRowCount;

		if(nRowCount == 0) return FALSE;

		g_NpcItem.m_ppItem = new int* [g_NpcItem.m_nField];
		for(i = 0; i < g_NpcItem.m_nField; i++)
		{
			g_NpcItem.m_ppItem[i] = new int[g_NpcItem.m_nRow];
		}

		NpcItemSet.MoveFirst();

		nRowCount = 0;
		while(!NpcItemSet.IsEOF())
		{
			for(i = 0; i < g_NpcItem.m_nField; i++)
			{
				NpcItemSet.GetFieldValue(i, varValue, SQL_C_SSHORT);
				nItem = varValue.m_iVal;
				g_NpcItem.m_ppItem[i][nRowCount] = varValue.m_iVal;
			}
			nRowCount++;
			NpcItemSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();

		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();

		return FALSE;
	}

	return TRUE;
}
BOOL CServerDlg::GetNpcItemNewTable()
{
	int number, i;
	char szfilename[256]="drop.ini";
	char tmp[512];
	byte code1,code2;
	FILE *fp;
	char n1[20], n2[20], n3[20], n4[20], n5[20], n6[20];

	fp = fopen(szfilename, "r");
	if (fp==NULL){
		return FALSE;
	}

	fscanf(fp, "ÊýÁ¿ = %s\n", tmp);
	number = atoi(tmp);
	fscanf(fp, "##%s\n", tmp);
	for(i=0;i<number;i++){
		//¿¨ÌØ - 109 - 0 - 60 - 15 - 100 - 18
		//          1    2   3    4    5     6   7     8
		fscanf(fp,"%s - %s - %s - %s - %s - %s - %s\n",tmp,n1,n2,n3,n4,n5,n6);
		TRACE( "Å£Å£µô±¦ %s\n",tmp);
		code1=atoi(n1);
		code2=atoi(n2);
		strcpy(g_DropItem[code1][code2].name,tmp);
		g_DropItem[code1][code2].code1=code1;
		g_DropItem[code1][code2].code2=code2;
		g_DropItem[code1][code2].DropNovelity=atoi(n3);
		g_DropItem[code1][code2].DropLeechdom=atoi(n4);
		g_DropItem[code1][code2].money=atoi(n5);
		g_DropItem[code1][code2].n=atoi(n6);
		if(!GetOneNpcItemNewTable(&g_DropItem[code1][code2])){
			CString szTemp;
			szTemp.Format("%sÀà ¹ÖÎïµô±¦¶ÁÈ¡Ê§°Ü",g_DropItem[code1][code2].name);
			AfxMessageBox(szTemp);
			return FALSE;
		};
	}
	fclose(fp);
	return TRUE;
}

BOOL CServerDlg::get_convert_table()
{
	int number, i;
	char szfilename[256]="./convert.inf";
	char tmp[512];
	FILE *fp;
	int code1,code2;
	char n1[20], n2[20], n3[20], n4[20];


	fp = fopen(szfilename, "r");
	if (fp==NULL){
		return FALSE;
	}
	fscanf(fp, "ÊýÁ¿ = %s\n", tmp);
	number = atoi(tmp);
	for(i=0;i<number;i++){
		//Õ½Éñ¼× - 249 - 2 - ÁÒÑæÕ½¼× - 33 - 4 
		fscanf(fp,"%s - %s - %s - %s - %s - %s\n",convert[i].src_name,n1,n2,convert[i].dst_name,n3,n4);
		code1=atoi(n1);
		code2=atoi(n2);
		convert[i].Ssid=code1+code2*256;
		code1=atoi(n3);
		code2=atoi(n4);
		convert[i].Dsid=code1+code2*256;
		convert[i].have=TRUE;
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////
//¶ÁÈ¡Ò»¸ö¹ÖÎïµô±¦
BOOL CServerDlg::GetOneNpcItemNewTable(struct drop_info *dropItem)
{
	char szfilename[100];
	FILE *fp;
	int i;
	char tmp[111], n1[20], n2[20], n3[20], n4[20], n5[20], n6[20];

	sprintf(szfilename,"./drop/%s.ini",dropItem->name);
	fp = fopen(szfilename, "r");
	if (fp==NULL){
		return FALSE;
	}
	fscanf(fp,"%s - %s - %s - %s - %s - %s - %s\n",tmp,n1,n2,n3,n4,n5,n6);
	for(i=0;i<dropItem->n;i++){
		//ÔÂÖ®ïí - 44 - 0 - 500 - 300
		fscanf(fp,"%s - %s - %s - %s\n",tmp,n1,n2,n3);
		//TRACE( "Å£Å£ÎïÆ· %s %s %s\n",tmp,n1,n2);
		strcpy(dropItem->novelity[i].name,tmp);
		dropItem->novelity[i].code1=atoi(n1);
		dropItem->novelity[i].code2=atoi(n2);
		dropItem->novelity[i].per = atoi(n3);
		if(dropItem->novelity[i].code1>256 ||dropItem->novelity[i].code2>7){
			TRACE("´íÎóÎïÆ·´úÂë %s ,%s\n",dropItem->name,dropItem->novelity[i].name);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//	NPC Chat Table Data ¸¦ ÀÐ´Â´Ù.
//
BOOL CServerDlg::GetNpcChatTable()
{
	int i = 1;
	BOOL bQuerySuccess = FALSE;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[1024];		memset(szSQL, 0x00, sizeof(szSQL));
	SQLCHAR		strContent[5001];	memset( strContent, NULL, sizeof(strContent));
	
	SQLSMALLINT	sIndex, sSize;
	SQLINTEGER	sInd;

	int db_index = -1;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	sIndex = sSize = 0;
	sInd = 0;

	CNpcChat* pNewChat = NULL;

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call EVENT_CHAT_LOAD }" ));

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS ) 
	{ 
		TRACE("Fail To Read NPC_CHAT!!\n"); 

		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_USHORT,	&sIndex,	sizeof(SQLSMALLINT), &sInd );
				SQLGetData( hstmt, i++, SQL_C_USHORT,	&sSize,		sizeof(SQLSMALLINT), &sInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strContent,	sizeof(strContent),	&sInd );

				ASSERT(sSize>0);
				pNewChat  = new CNpcChat;
				pNewChat->m_sCid	 = sIndex;
				pNewChat->m_sSize = sSize;
				pNewChat->m_strTalk = new TCHAR[sSize+1];
				memcpy(pNewChat->m_strTalk, strContent, sSize);
				pNewChat->m_strTalk[sSize] = NULL;

				g_arNpcChat.Add(pNewChat);

				memset( strContent, NULL, sizeof(strContent));
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else break;
		}
	}
	
	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

	if( !bQuerySuccess )
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//	Monster Table Data ¸¦ ÀÐ´Â´Ù.
//
BOOL CServerDlg::GetNpcTableData()
{
	CNpcTableSet NpcTableSet;

	g_arSummonTable.RemoveAll();

	try 
	{
		if(g_arNpcTable.GetSize()) return FALSE;

		if(NpcTableSet.IsOpen()) NpcTableSet.Close();
		
		if(!NpcTableSet.Open())
		{
			AfxMessageBox(_T("MONSTER DB Open Fail!"));
			return FALSE;
		}
		if(NpcTableSet.IsBOF()) 
		{
			AfxMessageBox(_T("MONSTER DB Empty!"));
			return FALSE;
		}

		while(!NpcTableSet.IsEOF())
		{
			CNpcTable* Npc = new CNpcTable;
			
			Npc->m_sSid			= NpcTableSet.m_sSid;		// MONSTER(NPC) Serial ID
			Npc->m_sPid			= NpcTableSet.m_sPid;		// MONSTER(NPC) Picture ID
			_tcscpy(Npc->m_strName, NpcTableSet.m_strName);	// MONSTER(NPC) Name
			
			Npc->m_sSTR			= NpcTableSet.m_sSTR;		// Èû
			Npc->m_sDEX			= NpcTableSet.m_sDEX;		// ¹ÎÃ¸
			Npc->m_sVOL			= NpcTableSet.m_sVOL;		// ÀÇÁö
			Npc->m_sWIS			= NpcTableSet.m_sWIS;		// ÁöÇý
			
			Npc->m_sMaxHP		= NpcTableSet.m_sMaxHP;		// ÃÖ´ë HP
			Npc->m_sMaxPP		= NpcTableSet.m_sMaxPP;		// ÃÖ´ë PP
			
			Npc->m_byClass		= NpcTableSet.m_byClass;	// ¹«±â°è¿­
			Npc->m_byClassLevel	= NpcTableSet.m_byClassLevel;			// ¹«±â°è¿­ ·¹º§

			Npc->m_sExp			= NpcTableSet.m_sExp;		// °æÇèÄ¡
			
			Npc->m_byAX			= NpcTableSet.m_byAX;		// °ø°Ý°ª X
			Npc->m_byAY			= NpcTableSet.m_byAY;		// °ø°Ý°ª Y
			Npc->m_byAZ			= NpcTableSet.m_byAZ;		// °ø°Ý°ª Z

			Npc->m_iDefense		= NpcTableSet.m_byDefense;	// ¹æ¾î°ª
			Npc->m_byRange		= NpcTableSet.m_byRange;	// »çÁ¤°Å¸®

			Npc->m_sAI			= NpcTableSet.m_sAI;		// ÀÎ°øÁö´É ÀÎµ¦½º
			Npc->m_sAttackDelay	= NpcTableSet.m_sAttackDelay;			// °ø°Ýµô·¹ÀÌ
			Npc->m_byVitalC		= NpcTableSet.m_byVitalC;	// ½ÅÃ¼µ¥¹ÌÁö Å©¸®Æ¼ÄÃ
			Npc->m_byWildShot	= NpcTableSet.m_byWildShot;	// ³­»ç ·¹º§
			Npc->m_byExcitedRate= NpcTableSet.m_byExcitedRate;			// ÈïºÐ ·¹º§
			Npc->m_byIronSkin	= NpcTableSet.m_byIronSkin;
			Npc->m_byReAttack	= NpcTableSet.m_byReAttack;
			Npc->m_bySubAttack	= NpcTableSet.m_bySubAttack;// »óÅÂÀÌ»ó ¹ß»ý(ºÎ°¡°ø°Ý)
			Npc->m_byState		= NpcTableSet.m_byState;	// ¸ó½ºÅÍ (NPC) »óÅÂÀÌ»ó
			Npc->m_byPsi		= NpcTableSet.m_byPsi;		// »çÀÌ¿À´Ð Àû¿ë
			Npc->m_byPsiLevel	= NpcTableSet.m_byPsiLevel;	// »çÀÌ¿À´Ð·¹º§

			Npc->m_bySearchRange= NpcTableSet.m_bySearchRange;			// Àû Å½Áö ¹üÀ§
			Npc->m_sSpeed		= NpcTableSet.m_sSpeed;		// ÀÌµ¿¼Óµµ	
			
			Npc->m_sInclination = NpcTableSet.m_sInclination;
			Npc->m_byColor		= NpcTableSet.m_byColor;
			Npc->m_sStandTime	= NpcTableSet.m_sStandTime;
			Npc->m_tNpcType		= NpcTableSet.m_tNpcType;	// NPC Type

			Npc->m_sFamilyType	= NpcTableSet.m_sFamilyType;// ¸÷µé°£ÀÇ °¡Á· ±ÔÁ¤
			Npc->m_tItemPer		= NpcTableSet.m_tItemPer;	// ¾ÆÀÌÅÛÀÌ ¶³¾îÁúÈ®·ü
			Npc->m_tDnPer		= NpcTableSet.m_tDnPer;		// µ·ÀÌ ¶³¾îÁúÈ®·ü
			
			g_arNpcTable.Add(Npc);

			if(!CheckSummonException(Npc->m_sSid))
			{
				CSummonTable* pSummon = new CSummonTable;

				pSummon->m_sSid		= Npc->m_sSid;
				pSummon->m_strName	= Npc->m_strName;

				g_arSummonTable.Add(pSummon);
			}

			NpcTableSet.MoveNext();
		}

		NpcTableSet.Close();
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();

		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();

		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//	NPC Table Data ¸¦ ÀÐ´Â´Ù.
//
BOOL CServerDlg::GetCityNpcTableData()
{
	CCityNpcTableSet NpcTableSet;

	try 
	{
		if(g_arCityNpcTable.GetSize()) return FALSE;

		if(NpcTableSet.IsOpen()) NpcTableSet.Close();
		
		if(!NpcTableSet.Open())
		{
			AfxMessageBox(_T("City NPC DB Open Fail!"));
			return FALSE;
		}
		if(NpcTableSet.IsBOF()) 
		{
			AfxMessageBox(_T("City NPC DB Empty!"));
			return FALSE;
		}

		while(!NpcTableSet.IsEOF())
		{
			CNpcTable* Npc = new CNpcTable;
			
			Npc->m_sSid			= NpcTableSet.m_sSid;		// (NPC) Serial ID
			Npc->m_sPid			= NpcTableSet.m_sPid;		// (NPC) Picture ID
			_tcscpy(Npc->m_strName, NpcTableSet.m_strName);	// (NPC) Name
			
			Npc->m_sSTR			= NpcTableSet.m_sSTR;		// Èû
			Npc->m_sDEX			= NpcTableSet.m_sDEX;		// ¹ÎÃ¸
			Npc->m_sVOL			= NpcTableSet.m_sVOL;		// ÀÇÁö
			Npc->m_sWIS			= NpcTableSet.m_sWIS;		// ÁöÇý
			
			Npc->m_sMaxHP		= NpcTableSet.m_sMaxHP;		// ÃÖ´ë HP
			Npc->m_sMaxPP		= NpcTableSet.m_sMaxPP;		// ÃÖ´ë PP
			
			Npc->m_byClass		= NpcTableSet.m_byClass;	// ¹«±â°è¿­
			Npc->m_byClassLevel	= NpcTableSet.m_byClassLevel;			// ¹«±â°è¿­ ·¹º§

			Npc->m_sExp			= NpcTableSet.m_sExp;		// °æÇèÄ¡
			
			Npc->m_byAX			= NpcTableSet.m_byAX;		// °ø°Ý°ª X
			Npc->m_byAY			= NpcTableSet.m_byAY;		// °ø°Ý°ª Y
			Npc->m_byAZ			= NpcTableSet.m_byAZ;		// °ø°Ý°ª Z

			Npc->m_iDefense		= NpcTableSet.m_byDefense;	// ¹æ¾î°ª
			Npc->m_byRange		= NpcTableSet.m_byRange;	// »çÁ¤°Å¸®

			Npc->m_sAI			= NpcTableSet.m_sAI;		// ÀÎ°øÁö´É ÀÎµ¦½º
			Npc->m_sAttackDelay	= NpcTableSet.m_sAttackDelay;			// °ø°Ýµô·¹ÀÌ
			Npc->m_byVitalC		= NpcTableSet.m_byVitalC;	// ½ÅÃ¼µ¥¹ÌÁö Å©¸®Æ¼ÄÃ
			Npc->m_byWildShot	= NpcTableSet.m_byWildShot;	// ³­»ç ·¹º§
			Npc->m_byExcitedRate= NpcTableSet.m_byExciteRate;			// ÈïºÐ ·¹º§
			Npc->m_byIronSkin	= NpcTableSet.m_byIronSkin;
			Npc->m_byReAttack	= NpcTableSet.m_byReAttack;
			Npc->m_bySubAttack	= NpcTableSet.m_bySubAttack;// »óÅÂÀÌ»ó ¹ß»ý(ºÎ°¡°ø°Ý)
			Npc->m_byState		= NpcTableSet.m_byState;	// ¸ó½ºÅÍ (NPC) »óÅÂÀÌ»ó
			Npc->m_byPsi		= NpcTableSet.m_byPsi;		// »çÀÌ¿À´Ð Àû¿ë
			Npc->m_byPsiLevel	= NpcTableSet.m_byPsiLevel;	// »çÀÌ¿À´Ð·¹º§
			
			Npc->m_bySearchRange= NpcTableSet.m_bySearchRange;			// Àû Å½Áö ¹üÀ§
			Npc->m_sSpeed		= NpcTableSet.m_sSpeed;		// ÀÌµ¿¼Óµµ	
			
			Npc->m_sInclination = NpcTableSet.m_sInclination;
			Npc->m_byColor		= NpcTableSet.m_byColor;
			Npc->m_sStandTime	= NpcTableSet.m_sStandTime;
			Npc->m_tNpcType		= NpcTableSet.m_tNpcType;	// NPC Type

			Npc->m_sFamilyType	= NpcTableSet.m_sFamilyType;// ¸÷µé°£ÀÇ °¡Á· ±ÔÁ¤
			Npc->m_tItemPer		= NpcTableSet.m_tItemPer;	// ¾ÆÀÌÅÛÀÌ ¶³¾îÁúÈ®·ü
			Npc->m_tDnPer		= NpcTableSet.m_tDnPer;		// µ·ÀÌ ¶³¾îÁúÈ®·ü
			
			g_arCityNpcTable.Add(Npc);

			NpcTableSet.MoveNext();
		}

		NpcTableSet.Close();
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();

		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();

		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	Npc Thread ¸¦ ¸¸µç´Ù.
//
BOOL CServerDlg::CreateNpcThread()
{
	BOOL	bMoveNext	= TRUE;
	int		nSerial		= 0;
	int		nNpcCount	= 0;
	int		i			= 0;

	g_TotalNPC = 0;			// DB¿¡ ÀÖ´Â ¼ö
	g_CurrentNPC = 0;
	g_CurrentNPCError = 0;

	CNpcPosSet	NpcPosSet;
	CNpcTable*	pNpcTable = NULL;

	g_arNpc.RemoveAll();

	try 
	{
		if(NpcPosSet.IsOpen()) NpcPosSet.Close();
		if(!NpcPosSet.Open())
		{
			AfxMessageBox(_T("MONSTER_POS DB Open Fail!"));
			return FALSE;
		}
		if(NpcPosSet.IsBOF()) 
		{
			AfxMessageBox(_T("MONSTER_POS DB Empty!"));
			return FALSE;
		}

		while(!NpcPosSet.IsEOF())
		{
//			CNpcThread* pNpcThread	= new CNpcThread;
			if(NpcPosSet.m_byType == 1)
			{
				CNpc*		pNpc		= new CNpc;

				pNpc->m_sNid	= nSerial++;			// ¼­¹ö ³»¿¡¼­ÀÇ °íÀ¯ ¹øÈ£
				pNpc->m_sSid	= NpcPosSet.m_sSid;		// MONSTER(NPC) Serial ID

				pNpcTable = NULL;
				for( i = 0; i < g_arCityNpcTable.GetSize(); i++)
				{
					if(pNpc->m_sSid == g_arCityNpcTable[i]->m_sSid ) 
					{
						pNpcTable = g_arCityNpcTable[i];
						break;
					}
				}

				if(!pNpcTable)
				{
					if(pNpc) delete pNpc;
					AfxMessageBox("Not NPC Data!!");
					return FALSE;
				}

				if( bMoveNext )
				{
					bMoveNext = FALSE;
					nNpcCount = NpcPosSet.m_sCount;
				}

				pNpc->m_sPid			= pNpcTable->m_sPid;		// MONSTER(NPC) Picture ID
				_tcscpy(pNpc->m_strName, pNpcTable->m_strName);		// MONSTER(NPC) Name
				
				pNpc->m_sSTR			= pNpcTable->m_sSTR;		// Èû
				pNpc->m_sDEX			= pNpcTable->m_sDEX;		// ¹ÎÃ¸
				pNpc->m_sVOL			= pNpcTable->m_sVOL;		// ÀÇÁö
				pNpc->m_sWIS			= pNpcTable->m_sWIS;		// ÁöÇý
				
				pNpc->m_sHP				= pNpcTable->m_sMaxHP;		// ÃÖ´ë HP
				pNpc->m_sMaxHP			= pNpcTable->m_sMaxHP;		// ÇöÀç HP
				pNpc->m_sPP				= pNpcTable->m_sMaxPP;		// ÃÖ´ë PP
				pNpc->m_sMaxPP			= pNpcTable->m_sMaxPP;		// ÇöÀç PP
				
				pNpc->m_byClass			= pNpcTable->m_byClass;		// ¹«±â°è¿­
				pNpc->m_byClassLevel	= pNpcTable->m_byClassLevel;// ¹«±â°è¿­ ·¹º§
				pNpc->m_sExp			= pNpcTable->m_sExp;		// °æÇèÄ¡
				
				pNpc->m_byAX			= pNpcTable->m_byAX;		// °ø°Ý°ª X
				pNpc->m_byAY			= pNpcTable->m_byAY;		// °ø°Ý°ª Y
				pNpc->m_byAZ			= pNpcTable->m_byAZ;		// °ø°Ý°ª Z

				pNpc->m_iDefense		= pNpcTable->m_iDefense;	// ¹æ¾î°ª
				pNpc->m_byRange			= pNpcTable->m_byRange;		// »çÁ¤°Å¸®
				pNpc->m_sAI				= pNpcTable->m_sAI;		// ÀÎ°øÁö´É ÀÎµ¦½º
				pNpc->m_sAttackDelay	= pNpcTable->m_sAttackDelay;// °ø°Ýµô·¹ÀÌ
				pNpc->m_byVitalC		= pNpcTable->m_byVitalC;	// ½ÅÃ¼µ¥¹ÌÁö Å©¸®Æ¼ÄÃ
				pNpc->m_byWildShot		= pNpcTable->m_byWildShot;	// ³­»ç ·¹º§
				pNpc->m_byExcitedRate	= pNpcTable->m_byExcitedRate;			// ÈïºÐ ·¹º§
				pNpc->m_byIronSkin		= pNpcTable->m_byIronSkin;
				pNpc->m_byReAttack		= pNpcTable->m_byReAttack;
				pNpc->m_bySubAttack		= pNpcTable->m_bySubAttack;	// »óÅÂÀÌ»ó ¹ß»ý(ºÎ°¡°ø°Ý)
				pNpc->m_byState			= pNpcTable->m_byState;		// ¸ó½ºÅÍ (NPC) »óÅÂÀÌ»ó
				pNpc->m_byPsi			= pNpcTable->m_byPsi;		// »çÀÌ¿À´Ð Àû¿ë
				pNpc->m_byPsiLevel		= pNpcTable->m_byPsiLevel;	// »çÀÌ¿À´Ð·¹º§

				pNpc->m_bySearchRange	= pNpcTable->m_bySearchRange;			// Àû Å½Áö ¹üÀ§
				pNpc->m_sSpeed			= pNpcTable->m_sSpeed;		// ÀÌµ¿¼Óµµ	
				
				pNpc->m_sInclination	= pNpcTable->m_sInclination;
				pNpc->m_byColor			= pNpcTable->m_byColor;
				pNpc->m_sStandTime		= pNpcTable->m_sStandTime;

				//////// MONSTER POS ////////////////////////////////////////
				pNpc->m_sCurZ			= pNpc->m_sOrgZ = NpcPosSet.m_sZone;
				pNpc->m_sCurX			= pNpc->m_sOrgX	= NpcPosSet.m_sX;
				pNpc->m_sCurY			= pNpc->m_sOrgY	= NpcPosSet.m_sY;
				
				pNpc->m_sMinX			= NpcPosSet.m_sMinX;
				pNpc->m_sMinY			= NpcPosSet.m_sMinY;
				pNpc->m_sMaxX			= NpcPosSet.m_sMaxX;
				pNpc->m_sMaxY			= NpcPosSet.m_sMaxY;
				
				pNpc->m_sRegenTime		= NpcPosSet.m_sRegenTime * 1000;	// ÃÊ(DB)´ÜÀ§-> ¹Ð¸®¼¼ÄÁµå·Î
	//			pNpc->m_sRegenTime		= NpcPosSet.m_sRegenTime * 2000;	// ÃÊ(DB)´ÜÀ§-> ¹Ð¸®¼¼ÄÁµå·Î

				pNpc->m_sEvent			= NpcPosSet.m_sEvent;		// ÀÌº¥Æ® ¹øÈ£

				pNpc->m_sEZone			= NpcPosSet.m_sEZone;
				pNpc->m_sGuild			= NpcPosSet.m_sGuild;		// ±æµå¿¡ ¼ÓÇÑ »óÁ¡ÀÇ NPCÀÓ

				pNpc->m_sHaveItem		= NpcPosSet.m_sHaveItem;
				pNpc->m_sQuestSay		= NpcPosSet.m_sSay;
				pNpc->m_byType			= NpcPosSet.m_byType;
				pNpc->m_sDimension		= NpcPosSet.m_sDimension;	// ÇöÀç ¸î¼¿¸¦ Â÷ÁöÇÏ´ÂÁö ÆÇ´Ü

				pNpc->m_tNpcType		= pNpcTable->m_tNpcType;	// NPC Type

				pNpc->m_sFamilyType		= pNpcTable->m_sFamilyType;	// NPC Type
				pNpc->m_tItemPer		= pNpcTable->m_tItemPer;	// NPC Type
				pNpc->m_tDnPer			= pNpcTable->m_tDnPer;	// NPC Type

				pNpc->m_ZoneIndex		= -1;

				// ±âÁØ ¼Óµµ : 500¸¦ 100 %·Î ÇÏ¸é 50Àº Áõ°¡À² 10 % °¡ µÈ´Ù.
				// ½ÄÀº (pNpc->m_sSpeed - 500) * 10/50 ÀÓ --> 50:10 = () : x
				pNpc->m_sClientSpeed = 100 - (pNpc->m_sSpeed - 500) * 10/50;
				pNpc->m_dwStepDelay = GetTickCount();

				if(pNpc->m_sClientSpeed <= 20) pNpc->m_sClientSpeed = 20;	// ¹æ¾î ÄÚµå;

				for(i = 0; i < g_zone.GetSize(); i++)
				{
					MAP* pAddMap = g_zone[i];
					if(g_zone[i]->m_Zone == pNpc->m_sCurZ) 
					{
						pNpc->m_ZoneIndex = i;
						break;
					}
				}
				if(pNpc->m_ZoneIndex == -1) 
				{
					if(pNpc) delete pNpc;
					AfxMessageBox("Invalid zone Index!!");
					return FALSE;
				}

				pNpc->Init();		
/*
			CString msg;
			msg = "ÀÚÀÌ¾ðÆ® ºòÇ²";
			if(msg.Compare(pNpc->m_strName) == 0 && pNpc->m_sCurX == 160 && pNpc->m_sCurY == 480)
			{
				pNpc->m_sNid	= nSerial++;
				g_arNpc.Add(pNpc);
				g_TotalNPC = nSerial;
			}
			else 
			{
				if(pNpc) delete pNpc;
			}
*/
				g_arNpc.Add(pNpc);
				g_TotalNPC = nSerial;
				if(--nNpcCount > 0) continue;
			}
			//if(g_arNpc.GetSize() >= 100) break;	//yskang Test Code
			
			bMoveNext = TRUE;
			nNpcCount = 0;

			NpcPosSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	try 
	{
		if(NpcPosSet.IsOpen()) NpcPosSet.Close();
		if(!NpcPosSet.Open())
		{
			AfxMessageBox(_T("MONSTER_POS DB Open Fail!"));
			return FALSE;
		}
		if(NpcPosSet.IsBOF()) 
		{
//			NpcPosSet.MoveFirst();
			AfxMessageBox(_T("MONSTER_POS DB Empty!"));
			return FALSE;
		}

		while(!NpcPosSet.IsEOF())
		{
//			CNpcThread* pNpcThread	= new CNpcThread;
			if(NpcPosSet.m_byType != 1)
			{
				CNpc*		pNpc		= new CNpc;

				pNpc->m_sNid	= nSerial++;			// ¼­¹ö ³»¿¡¼­ÀÇ °íÀ¯ ¹øÈ£
				pNpc->m_sSid	= NpcPosSet.m_sSid;		// MONSTER(NPC) Serial ID

				pNpcTable = NULL;
				for( i = 0; i < g_arNpcTable.GetSize(); i++)
				{
					if(pNpc->m_sSid == g_arNpcTable[i]->m_sSid ) 
					{
						pNpcTable = g_arNpcTable[i];
						break;
					}
				}

				if(!pNpcTable)
				{
					if(pNpc) delete pNpc;

					AfxMessageBox("Not Monster Data!!");
					return FALSE;
				}

				if( bMoveNext )
				{
					bMoveNext = FALSE;
					nNpcCount = NpcPosSet.m_sCount;
				}

				pNpc->m_sPid			= pNpcTable->m_sPid;		// MONSTER(NPC) Picture ID
				_tcscpy(pNpc->m_strName, pNpcTable->m_strName);		// MONSTER(NPC) Name
				
				pNpc->m_sSTR			= pNpcTable->m_sSTR;		// Èû
				pNpc->m_sDEX			= pNpcTable->m_sDEX;		// ¹ÎÃ¸
				pNpc->m_sVOL			= pNpcTable->m_sVOL;		// ÀÇÁö
				pNpc->m_sWIS			= pNpcTable->m_sWIS;		// ÁöÇý
				
				pNpc->m_sHP				= pNpcTable->m_sMaxHP;		// ÃÖ´ë HP
				pNpc->m_sMaxHP			= pNpcTable->m_sMaxHP;		// ÇöÀç HP
				pNpc->m_sPP				= pNpcTable->m_sMaxPP;		// ÃÖ´ë PP
				pNpc->m_sMaxPP			= pNpcTable->m_sMaxPP;		// ÇöÀç PP
				
				pNpc->m_byClass			= pNpcTable->m_byClass;		// ¹«±â°è¿­
				pNpc->m_byClassLevel	= pNpcTable->m_byClassLevel;// ¹«±â°è¿­ ·¹º§
				pNpc->m_sExp			= pNpcTable->m_sExp;		// °æÇèÄ¡
				
				pNpc->m_byAX			= pNpcTable->m_byAX;		// °ø°Ý°ª X
				pNpc->m_byAY			= pNpcTable->m_byAY;		// °ø°Ý°ª Y
				pNpc->m_byAZ			= pNpcTable->m_byAZ;		// °ø°Ý°ª Z

				pNpc->m_iDefense		= pNpcTable->m_iDefense;	// ¹æ¾î°ª
				pNpc->m_byRange			= pNpcTable->m_byRange;		// »çÁ¤°Å¸®
				pNpc->m_sAI				= pNpcTable->m_sAI;		// ÀÎ°øÁö´É ÀÎµ¦½º
				pNpc->m_sAttackDelay	= pNpcTable->m_sAttackDelay;// °ø°Ýµô·¹ÀÌ
				pNpc->m_byVitalC		= pNpcTable->m_byVitalC;	// ½ÅÃ¼µ¥¹ÌÁö Å©¸®Æ¼ÄÃ
				pNpc->m_byWildShot		= pNpcTable->m_byWildShot;	// ³­»ç ·¹º§
				pNpc->m_byExcitedRate	= pNpcTable->m_byExcitedRate;			// ÈïºÐ ·¹º§
				pNpc->m_byIronSkin		= pNpcTable->m_byIronSkin;
				pNpc->m_byReAttack		= pNpcTable->m_byReAttack;
				pNpc->m_bySubAttack		= pNpcTable->m_bySubAttack;	// »óÅÂÀÌ»ó ¹ß»ý(ºÎ°¡°ø°Ý)
				pNpc->m_byState			= pNpcTable->m_byState;		// ¸ó½ºÅÍ (NPC) »óÅÂÀÌ»ó
				pNpc->m_byPsi			= pNpcTable->m_byPsi;		// »çÀÌ¿À´Ð Àû¿ë
				pNpc->m_byPsiLevel		= pNpcTable->m_byPsiLevel;	// »çÀÌ¿À´Ð·¹º§

				pNpc->m_bySearchRange	= pNpcTable->m_bySearchRange;			// Àû Å½Áö ¹üÀ§
				pNpc->m_sSpeed			= pNpcTable->m_sSpeed;		// ÀÌµ¿¼Óµµ	
				
				pNpc->m_sInclination	= pNpcTable->m_sInclination;
				pNpc->m_byColor			= pNpcTable->m_byColor;
				pNpc->m_sStandTime		= pNpcTable->m_sStandTime;
				
				//////// MONSTER POS ////////////////////////////////////////
				pNpc->m_sCurZ			= pNpc->m_sOrgZ = pNpc->m_sTableOrgZ = NpcPosSet.m_sZone;
				pNpc->m_sCurX			= pNpc->m_sOrgX	= pNpc->m_sTableOrgX = NpcPosSet.m_sX;
				pNpc->m_sCurY			= pNpc->m_sOrgY	= pNpc->m_sTableOrgY = NpcPosSet.m_sY;
				
				pNpc->m_sMinX			= NpcPosSet.m_sMinX;
				pNpc->m_sMinY			= NpcPosSet.m_sMinY;
				pNpc->m_sMaxX			= NpcPosSet.m_sMaxX;
				pNpc->m_sMaxY			= NpcPosSet.m_sMaxY;
				
				pNpc->m_sRegenTime		= NpcPosSet.m_sRegenTime * 1000;	// ÃÊ(DB)´ÜÀ§-> ¹Ð¸®¼¼ÄÁµå·Î
		//		pNpc->m_sRegenTime		= NpcPosSet.m_sRegenTime * 2000;	// ÃÊ(DB)´ÜÀ§-> ¹Ð¸®¼¼ÄÁµå·Î

				pNpc->m_sEvent			= NpcPosSet.m_sEvent;		// ÀÌº¥Æ® ¹øÈ£

				pNpc->m_sEZone			= NpcPosSet.m_sEZone;
				pNpc->m_sGuild			= NpcPosSet.m_sGuild;		// ±æµå¿¡ ¼ÓÇÑ »óÁ¡ÀÇ NPCÀÓ

				pNpc->m_sHaveItem		= NpcPosSet.m_sHaveItem;
				pNpc->m_sQuestSay		= NpcPosSet.m_sSay;
				pNpc->m_byType			= NpcPosSet.m_byType;
				pNpc->m_sDimension		= NpcPosSet.m_sDimension;	// ÇöÀç ¸î¼¿¸¦ Â÷ÁöÇÏ´ÂÁö ÆÇ´Ü

				pNpc->m_tNpcType		= pNpcTable->m_tNpcType;	// NPC Type

				pNpc->m_sFamilyType		= pNpcTable->m_sFamilyType;	// NPC Type
				pNpc->m_tItemPer		= pNpcTable->m_tItemPer;	// NPC Type
				pNpc->m_tDnPer			= pNpcTable->m_tDnPer;	// NPC Type

				pNpc->m_ZoneIndex		= -1;

				// ±âÁØ ¼Óµµ : 500¸¦ 100 %·Î ÇÏ¸é 50Àº Áõ°¡À² 10 % °¡ µÈ´Ù.
				// ½ÄÀº (pNpc->m_sSpeed - 500) * 10/50 ÀÓ --> 50:10 = () : x
				pNpc->m_sClientSpeed = 100 - (pNpc->m_sSpeed - 500) * 10/50;
				pNpc->m_dwStepDelay = GetTickCount();

				if(pNpc->m_sClientSpeed <= 20) pNpc->m_sClientSpeed = 20;	// ¹æ¾î ÄÚµå;

				for(i = 0; i < g_zone.GetSize(); i++)
				{
					MAP* pAddMap = g_zone[i];
					if(g_zone[i]->m_Zone == pNpc->m_sCurZ) 
					{
						pNpc->m_ZoneIndex = pNpc->m_TableZoneIndex = i;
						break;
					}
				}
				if(pNpc->m_ZoneIndex == -1) 
				{
					if(pNpc) delete pNpc;
					AfxMessageBox("Invalid zone Index!!");
					return FALSE;
				}

				pNpc->Init();
	/*
				CString msg;
				msg = "ÀÚÀÌ¾ðÆ® ºòÇ²";
				if(msg.Compare(pNpc->m_strName) == 0 && pNpc->m_sCurX == 160 && pNpc->m_sCurY == 480)
				{
					pNpc->m_sNid	= nSerial++;
					g_arNpc.Add(pNpc);
					g_TotalNPC = nSerial;
				}
				else 
				{
					if(pNpc) delete pNpc;
				}
	*/
				g_arNpc.Add(pNpc);

				g_TotalNPC = nSerial;
	//			if(g_arNpc.GetSize() >= 100) break;	//@ Test Code
				if(--nNpcCount > 0) continue;
			}

			bMoveNext = TRUE;
			nNpcCount = 0;

			NpcPosSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	int step = 0;
	CNpcThread* pNpcThread = NULL;
	for(i = 0; i < g_arNpc.GetSize(); i++)
	{
		if( step == 0 )
		{
			pNpcThread = NULL;
			pNpcThread = new CNpcThread;
		}
		pNpcThread->m_pNpc[step] = g_arNpc[i];
		
		++step;
		
		if( step == NPC_NUM )
		{
			pNpcThread->m_pCom = &m_Com;				
			pNpcThread->m_pThread = AfxBeginThread(NpcThreadProc, &(pNpcThread->m_ThreadInfo), THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
			g_arNpcThread.Add( pNpcThread );
			step = 0;
		}
	}

	if( step != 0 )
	{
		pNpcThread->m_pCom = &m_Com;
		pNpcThread->m_pThread = AfxBeginThread(NpcThreadProc, &(pNpcThread->m_ThreadInfo), THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		g_arNpcThread.Add( pNpcThread );
	}

	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& Test Code
	pNpcThread = NULL;
	pNpcThread = new CNpcThread;	
	pNpcThread->m_pCom = &m_Com;

	for(i = 0; i < NPC_NUM; i++) // ¹Ì¸® ÃÖ´ë ¼ÒÈ¯ NPC 10¸¶¸® ¸Þ¸ð¸® ÇÒ´ç
	{
		CNpc*	pNpc = new CNpc;
		pNpc->m_sNid = nSerial++;
		pNpc->m_NpcState = NPC_DEAD;
		pNpc->m_lEventNpc = 0;
		pNpcThread->m_pNpc[i] = pNpc;
		g_arNpc.Add(pNpc);
	}
	;
	pNpcThread->m_pThread = AfxBeginThread(NpcThreadProc, &(pNpcThread->m_ThreadInfo), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	g_arEventNpcThread.Add( pNpcThread );

	return TRUE;
}

BOOL CServerDlg::GetDBItemData()
{
	CDBItemInfo		DBItemInfo;		// DBItem 2001.1.17
	
	try
	{
		if(DBItemInfo.IsOpen()) DBItemInfo.Close();
		
		if( !DBItemInfo.Open() )
		{
			AfxMessageBox(_T("Basic Item DB Not Connectting"));
			return FALSE;
		}
		if(DBItemInfo.IsBOF()) 
		{
			AfxMessageBox(_T("Basic Item DB Empty!"));
			return FALSE;
		}
		
		while( !DBItemInfo.IsEOF() )
		{
			TableItem* newItem = new TableItem;
			
			newItem->m_Num			= DBItemInfo.m_sNum;
			newItem->m_Cost			= (DWORD)DBItemInfo.m_iCost;
			newItem->m_Arm			= DBItemInfo.m_tArm;
			newItem->m_UseLevel		= DBItemInfo.m_tUseLevel;
			newItem->m_UseType		= DBItemInfo.m_tUseType;
			newItem->m_PicNum		= DBItemInfo.m_sPicNum;
			newItem->m_Wg			= DBItemInfo.m_sWg;
			newItem->m_Dur			= DBItemInfo.m_sDur;
			newItem->m_MaxAt		= DBItemInfo.m_sMaxAt;
			newItem->m_AtDelay		= DBItemInfo.m_sAtDelay;
			newItem->m_DmgX			= DBItemInfo.m_tDmgX;
			newItem->m_DmgY			= DBItemInfo.m_tDmgY;
			newItem->m_DmgZ			= DBItemInfo.m_tDmgZ;
			newItem->m_Df			= DBItemInfo.m_tDf;
			newItem->m_At			= DBItemInfo.m_tAt;
			newItem->m_Crit			= DBItemInfo.m_tCrit;
			newItem->m_Range		= DBItemInfo.m_tRange;
			newItem->m_Out			= DBItemInfo.m_tOut;
			newItem->m_BullNum		= DBItemInfo.m_tBullNum;
			newItem->m_BullType		= DBItemInfo.m_tBullType;
			newItem->m_StErr		= DBItemInfo.m_tStErr;
			newItem->m_StDf			= DBItemInfo.m_tStDf;
			newItem->m_StCure		= DBItemInfo.m_tStCure;
			newItem->m_HP			= DBItemInfo.m_tHP;
			newItem->m_PP			= DBItemInfo.m_tPP;
			newItem->m_SP			= DBItemInfo.m_sSP;
			
			strcpy( newItem->m_Name, DBItemInfo.m_strName );
			
			g_DBItemArray.Add(newItem);
			
			DBItemInfo.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//	Item Table À» ÀÐ´Â´Ù.
//
BOOL CServerDlg::GetItemTable()
{
	CItemTableSet	ItemTableSet;
	
	try
	{
		if(ItemTableSet.IsOpen()) ItemTableSet.Close();

		ItemTableSet.m_strSort = "sSid ASC";

		if( !ItemTableSet.Open() )
		{
			AfxMessageBox(_T("Item DB Open Fail!"));
			return FALSE;
		}
		if(ItemTableSet.IsBOF()) 
		{
			AfxMessageBox(_T("Item DB Empty!"));
			return FALSE;
		}
		
		while( !ItemTableSet.IsEOF() )
		{
			CItemTable* pNewItem = new CItemTable;
					
			pNewItem->m_sSid			= ItemTableSet.m_sSid;
			pNewItem->m_sPid			= ItemTableSet.m_sPid;
			pNewItem->m_strName			= ItemTableSet.m_strName;
			pNewItem->m_strText			= ItemTableSet.m_strText;
			pNewItem->m_byWeight		= ItemTableSet.m_byWeight;
			pNewItem->m_iDN				= ItemTableSet.m_iDN;
			pNewItem->m_byRLevel		= ItemTableSet.m_byRLevel;
			pNewItem->m_byClass			= ItemTableSet.m_byClass;
			pNewItem->m_byWear			= ItemTableSet.m_byWear;
			pNewItem->m_sDuration		= ItemTableSet.m_sDuration;
			pNewItem->m_sDefense		= ItemTableSet.m_sDefense;
			pNewItem->m_byAX			= ItemTableSet.m_byAX;
			pNewItem->m_byAY			= ItemTableSet.m_byAY;
			pNewItem->m_byAZ			= ItemTableSet.m_byAZ;
			pNewItem->m_byRstr			= ItemTableSet.m_byRstr;
			pNewItem->m_byRdex			= ItemTableSet.m_byRdex;
			pNewItem->m_byRvol			= ItemTableSet.m_byRvol;
			pNewItem->m_sAttackDelay	= ItemTableSet.m_sAttackDelay;
			pNewItem->m_byRange			= ItemTableSet.m_byRange;
			pNewItem->m_byErrorRate		= ItemTableSet.m_byErrorRate;
			pNewItem->m_sBullNum		= ItemTableSet.m_sBullNum;
			pNewItem->m_byBullType		= ItemTableSet.m_byBullType;
			pNewItem->m_bySubDefense	= ItemTableSet.m_bySubDefense;
			pNewItem->m_bySubCure		= ItemTableSet.m_bySubCure;
			pNewItem->m_sRepairHP		= ItemTableSet.m_sRepairHP;
			pNewItem->m_sRepairPP		= ItemTableSet.m_sRepairPP;
			pNewItem->m_sRepairSP		= ItemTableSet.m_sRepairSP;
			pNewItem->m_sEvent			= ItemTableSet.m_sEvent;
			pNewItem->m_sZone			= ItemTableSet.m_sZone;
			pNewItem->m_byMPP			= ItemTableSet.m_byMPP;
			pNewItem->m_sCTime			= ItemTableSet.m_sCTime;
			pNewItem->m_bySpecial		= ItemTableSet.m_bySpecial;

			g_arItemTable.Add(pNewItem);
			
			ItemTableSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//	Skill Table À» ÀÐ´Â´Ù.
//
BOOL CServerDlg::GetSkillTable()
{
/*
	int iLimitDamage = 0;
	CSkillTableSet	SkillTableSet;
	short			sSameSkill = -1;
	CSkillTable*	pNewSkill = NULL;

	try
	{
		if(SkillTableSet.IsOpen()) SkillTableSet.Close();
		
		if( !SkillTableSet.Open() )
		{
			AfxMessageBox(_T("Skill DB Open Fail!"));
			return FALSE;
		}
		if(SkillTableSet.IsBOF()) 
		{
			AfxMessageBox(_T("Sill DB Empty!"));
			return FALSE;
		}
		
		while( !SkillTableSet.IsEOF() )
		{
			if(sSameSkill != SkillTableSet.m_sSid)
			{
				if(sSameSkill != -1) 
				{
					g_arSkillTable.Add(pNewSkill);
					AddSkillTable();			// ? ½ºÅ³¸¦ ¼­¹ö¿¡¼­ À¯ÁöÇÏ±âÀ§ÇØ ºó °÷À» Ã¤¿î´Ù.
				}

				pNewSkill = new CSkillTable;
				sSameSkill = SkillTableSet.m_sSid;

				pNewSkill->m_sSid		= SkillTableSet.m_sSid;
				pNewSkill->m_sPid		= SkillTableSet.m_sPid;
				pNewSkill->m_tClass		= SkillTableSet.m_tClass;

				pNewSkill->m_arInc.Add(0);			// ½ºÅ³·¹º§À» 1~20 À¸·Î ±×³É ¾²±âÀ§ÇØ 
				pNewSkill->m_arRepair.Add(0);
				pNewSkill->m_arSuccess.Add(0);
			}
			else
			{
//				if(pNewSkill->m_sSid == 12)			// ÃÖ¼Ò ´ë¹ÌÁö sid = 12; ¼Ò¼öÁ¡ÀÌ±â¶§¹®¿¡ Ã³À½¿¡ º¯È¯ÇÑ´Ù.
//				{
//					iLimitDamage = (int)((double)SkillTableSet.m_tInc/5 + 0.5);
//					pNewSkill->m_arInc.Add(iLimitDamage);
//				}
//				else 
				pNewSkill->m_arInc.Add(SkillTableSet.m_tInc);

				pNewSkill->m_arRepair.Add(SkillTableSet.m_tRepair);
				pNewSkill->m_arSuccess.Add(SkillTableSet.m_tSuccess);
			}
			
			SkillTableSet.MoveNext();
		}

		g_arSkillTable.Add(pNewSkill);
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
*/

	int iLimitDamage = 0;
	CSkillTableSet	SkillTableSet;
	short			sSameSkill = -1;
	CSkillTable*	pNewSkill = NULL;
	CSkillTable*	pCurSkill = NULL;

	try
	{
		if(SkillTableSet.IsOpen()) SkillTableSet.Close();
		
		if( !SkillTableSet.Open() )
		{
			AfxMessageBox(_T("Skill DB Open Fail!"));
			return FALSE;
		}
		if(SkillTableSet.IsBOF()) 
		{
			AfxMessageBox(_T("Sill DB Empty!"));
			return FALSE;
		}

		for( int i = 0; i < SKILL_COUNT; i++ )
		{
			pNewSkill = new CSkillTable;

			pNewSkill->m_sSid		= -1;
			pNewSkill->m_sPid		= -1;
			pNewSkill->m_tClass		= 0;
/*
			pNewSkill->m_arInc.Add(0);			// ½ºÅ³·¹º§À» 1~20 À¸·Î ±×³É ¾²±âÀ§ÇØ 
			pNewSkill->m_arRepair.Add(0);
			pNewSkill->m_arSuccess.Add(0);
*/
			g_arSkillTable.Add( pNewSkill );
		}

		while( !SkillTableSet.IsEOF() )
		{
			pCurSkill = g_arSkillTable[SkillTableSet.m_sSid];

			if( pCurSkill )
			{
				pCurSkill->m_sSid		= SkillTableSet.m_sSid;
				pCurSkill->m_sPid		= SkillTableSet.m_sPid;
				pCurSkill->m_tClass		= SkillTableSet.m_tClass;

				pCurSkill->m_arInc.Add( SkillTableSet.m_tInc );
				pCurSkill->m_arRepair.Add( SkillTableSet.m_tRepair );
				pCurSkill->m_arSuccess.Add( SkillTableSet.m_tSuccess );
			}
			
			SkillTableSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////
//	½ºÅ³Å×ÀÌºíÀº 0~15±îÁö ÀÌ°í ¼­¹ö¿¡¼­ °ü¸®ÇÏ´Â °ÍÀº 0~19±îÁöÀÌ¹Ç·Î ³ª¸ÓÁö 4°³¸¦ Ãß°¡
//
void CServerDlg::AddSkillTable()
{
	CSkillTable*	pNewSkill = NULL;

	int addNum = g_arSkillTable.GetSize();

	switch(addNum)
	{
	case 4:	case 9: case 14: case 19:

		pNewSkill = new CSkillTable;

		pNewSkill->m_sSid		= -1;
		pNewSkill->m_sPid		= -1;
		pNewSkill->m_tClass		= 0;

		pNewSkill->m_arInc.Add(0);			// ½ºÅ³·¹º§À» 1~20 À¸·Î ±×³É ¾²±âÀ§ÇØ 
		pNewSkill->m_arRepair.Add(0);
		pNewSkill->m_arSuccess.Add(0);

		g_arSkillTable.Add(pNewSkill);
		break;

	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////
//	Psionic Table À» ÀÐ´Â´Ù.
//
BOOL CServerDlg::GetPsiTable()
{
	CPsiTableSet	PsiTableSet;
	
	try
	{
		if(PsiTableSet.IsOpen()) PsiTableSet.Close();
		
		if( !PsiTableSet.Open() )
		{
			AfxMessageBox(_T("Psionic DB Open Fail!"));
			return FALSE;
		}
		if(PsiTableSet.IsBOF()) 
		{
			AfxMessageBox(_T("Psionic DB Empty!"));
			return FALSE;
		}
		
		while( !PsiTableSet.IsEOF() )
		{
			CPsiTable* pNewPsionic = new CPsiTable;
			
			pNewPsionic->m_sSid			= PsiTableSet.m_sSid;			
			pNewPsionic->m_sPid			= PsiTableSet.m_sPid;			
			pNewPsionic->m_strName		= PsiTableSet.m_strName;		
			pNewPsionic->m_iNeedDN		= PsiTableSet.m_iNeedDN;		
			pNewPsionic->m_iNeedXP		= PsiTableSet.m_iNeedXP;		
			pNewPsionic->m_sDelayTime	= PsiTableSet.m_sDelayTime;	
			pNewPsionic->m_sHoldTime	= PsiTableSet.m_sHoldTime;	
			pNewPsionic->m_tClass		= PsiTableSet.m_tClass;		
			pNewPsionic->m_tNeedLevel	= PsiTableSet.m_tNeedLevel;	
			pNewPsionic->m_tNeedPP		= PsiTableSet.m_tNeedPP;		
			pNewPsionic->m_tRange		= PsiTableSet.m_tRange;		
			pNewPsionic->m_tRegi		= PsiTableSet.m_tRegi;		
			pNewPsionic->m_tTarget		= PsiTableSet.m_tTarget;		
			pNewPsionic->m_strText		= PsiTableSet.m_strText;		
			pNewPsionic->m_sBasic		= PsiTableSet.m_sBasic;
			pNewPsionic->m_sLevelUp		= PsiTableSet.m_sLevelUp;

			g_arPsiTable.Add(pNewPsionic);
			
			PsiTableSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//	·¹º§¾÷ Å×ÀÌºíÀ» ÀÐ´Â´Ù.
//
BOOL CServerDlg::GetLevelUpTable()
{
	CLevelUpTableSet	LevelUpTableSet;
	
	try
	{
		if(LevelUpTableSet.IsOpen()) LevelUpTableSet.Close();
		
		if( !LevelUpTableSet.Open() )
		{
			AfxMessageBox(_T("LEVEL_UP DB Open Fail!"));
			return FALSE;
		}
		if(LevelUpTableSet.IsBOF()) 
		{
			AfxMessageBox(_T("LEVEL_UP DB Empty!"));
			return FALSE;
		}
		
		while( !LevelUpTableSet.IsEOF() )
		{
			CLevelUpTable* pNewLevel = new CLevelUpTable;
			
			pNewLevel->m_sLevel		= LevelUpTableSet.m_sLevel;
			pNewLevel->m_dwExp		= LevelUpTableSet.m_iExp;
			pNewLevel->m_tBasicUp	= LevelUpTableSet.m_tBasicUp;

			g_arLevelUpTable.Add(pNewLevel);
			
			LevelUpTableSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//	PA, MA Exp ·¹º§¾÷ Å×ÀÌºíÀ» ÀÐ´Â´Ù.
//
BOOL CServerDlg::GetPAMAExpTable()
{
	CPAMAExpSet	PAMAExpSet;
	
	try
	{
		if(PAMAExpSet.IsOpen()) PAMAExpSet.Close();
		
		if( !PAMAExpSet.Open() )
		{
			AfxMessageBox(_T("PAMAExp DB Open Fail!"));
			return FALSE;
		}
		if(PAMAExpSet.IsBOF()) 
		{
			AfxMessageBox(_T("PAMAExp DB Empty!"));
			return FALSE;
		}
		
		while( !PAMAExpSet.IsEOF() )
		{
			CPAMAExp* pNewPAMA = new CPAMAExp;
			
			pNewPAMA->m_dwPAMAExp	= PAMAExpSet.m_iPAMAExp;

			g_arPAMAExp.Add(pNewPAMA);
			
			PAMAExpSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	¾ÆÀÌÅÛ, »çÀÌ¿À´Ð, ¸ÞÄ«´Ð »óÁ¡ Å×ÀÌºíÀ» ·ÎµåÇÑ´Ù.
//
BOOL CServerDlg::GetStoreTable()
{
	CStoreSet		StoreSet;
	CStoreSellSet	StoreSellSet;
	CStoreRepairSet	StoreRepairSet;

	_StoreDBArray	arTemp;
	_StoreDBArray	arSellTemp;
	_StoreDBArray	arRepairTemp;

	_StoreDB*		pTempDB = NULL;
	
	try
	{
		StoreSet.m_strSort = _T("sStoreID");
		StoreSellSet.m_strSort = _T("sStoreID");
		StoreRepairSet.m_strSort = _T("sStoreID");

		if(StoreSet.IsOpen()) StoreSet.Close();
		if(StoreSellSet.IsOpen()) StoreSellSet.Close();
		if(StoreRepairSet.IsOpen()) StoreRepairSet.Close();
		
		if(!StoreSet.Open())
		{
			AfxMessageBox(_T("Store DB Open Fail!"));
			return FALSE;
		}
		if(StoreSet.IsBOF()) 
		{
			AfxMessageBox(_T("Store DB Empty!"));
			return FALSE;
		}
		if(!StoreSellSet.Open())
		{
			AfxMessageBox(_T("Store Sell DB Open Fail!"));
			return FALSE;
		}
		if(StoreSellSet.IsBOF()) 
		{
			AfxMessageBox(_T("Store Sell DB Empty!"));
			return FALSE;
		}
		
		if(!StoreRepairSet.Open())
		{
			AfxMessageBox(_T("Store Repair DB Open Fail!"));
			return FALSE;
		}
		if(StoreRepairSet.IsBOF()) 
		{
			AfxMessageBox(_T("Store Repair DB Empty!"));
			return FALSE;
		}

		while(!StoreSet.IsEOF())
		{
			pTempDB = new _StoreDB;

			pTempDB->StoreID	= StoreSet.m_sStoreID;
			pTempDB->ItemID		= StoreSet.m_sItemID;

			arTemp.Add(pTempDB);
			
			StoreSet.MoveNext();
		}

		while(!StoreSellSet.IsEOF())
		{
			pTempDB = new _StoreDB;

			pTempDB->StoreID	= StoreSellSet.m_sStoreID;
			pTempDB->ItemID		= StoreSellSet.m_sItemID;

			arSellTemp.Add(pTempDB);
			
			StoreSellSet.MoveNext();
		}

		while(!StoreRepairSet.IsEOF())
		{
			pTempDB = new _StoreDB;

			pTempDB->StoreID	= StoreRepairSet.m_sStoreID;
			pTempDB->ItemID		= StoreRepairSet.m_sItemID;

			arRepairTemp.Add(pTempDB);
			
			StoreRepairSet.MoveNext();
		}

		CStore*	pNewStore = NULL;
		int nTempID = -1;
		int i;
		BOOL bAdd = FALSE;

		int nStoreSize = arTemp.GetSize();
		int nStoreSellSize = arSellTemp.GetSize();
		int nStoreRepairSize = arRepairTemp.GetSize();
/*		if(nStoreSize != nStoreSellSize) 
		{
			AfxMessageBox( IDS_SERVERDLG_STOREERROR );
			return FALSE;
		}
*/		
		for(i = 0; i < nStoreSize; i++)
		{
			if(nTempID != arTemp[i]->StoreID)
			{
				if(nTempID != -1) 
				{
					g_arStore.Add(pNewStore);
				}

				pNewStore = new CStore;
				nTempID = pNewStore->m_sStoreID = arTemp[i]->StoreID;
				pNewStore->m_arItems.Add(arTemp[i]->ItemID);
			}
			else
			{
				pNewStore->m_arItems.Add(arTemp[i]->ItemID);
			}
		}
		g_arStore.Add(pNewStore);

		for(i = 0; i < nStoreSellSize; i++)
		{
			g_arStore[arSellTemp[i]->StoreID]->m_arSellList.Add(arSellTemp[i]->ItemID);
		}

		for(i = 0; i < nStoreRepairSize; i++)
		{
//			int id = arRepairTemp[i]->StoreID;
//			int item = arRepairTemp[i]->ItemID;
			g_arStore[arRepairTemp[i]->StoreID]->m_arRepairList.Add(arRepairTemp[i]->ItemID);
		}

/*
		for(i = 0; i < nStoreSellSize; i++)
		{
			g_arStore[arSellTemp[i]->StoreID]->m_arSellList.Add(arSellTemp[i]->ItemID);
		}

		for(i = 0; i < nStoreRepairSize; i++)
		{
//			int id = arRepairTemp[i]->StoreID;
//			int item = arRepairTemp[i]->ItemID;
			g_arStore[arRepairTemp[i]->StoreID]->m_arRepairList.Add(arRepairTemp[i]->ItemID);
		}
*/
		for(i = 0; i < nStoreSize; i++)
		{
			if(arTemp[i])	delete arTemp[i];
		}
		arTemp.RemoveAll();

		for(i = 0; i < nStoreSellSize; i++)
		{
			if(arSellTemp[i])	delete arSellTemp[i];
		}
		arSellTemp.RemoveAll();

		for(i = 0; i < nStoreRepairSize; i++)
		{
			if(arRepairTemp[i])	delete arRepairTemp[i];
		}
		arRepairTemp.RemoveAll();

	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	NPC Thread µéÀ» ÀÛµ¿½ÃÅ²´Ù.
//
void CServerDlg::ResumeAI()
{
	int i, j;

	for(i = 0; i < g_arNpcThread.GetSize(); i++)
	{
		g_arNpcThread[i]->m_ThreadInfo.hWndMsg = this->GetSafeHwnd();
		for(j = 0; j < NPC_NUM; j++)
		{
			g_arNpcThread[i]->m_ThreadInfo.pNpc[j] = g_arNpcThread[i]->m_pNpc[j];
		}
//		g_arNpcThread[i]->m_ThreadInfo.m_pUser = g_arNpcThread[i]->m_pUser;
		g_arNpcThread[i]->m_ThreadInfo.pCom = &m_Com;

		::ResumeThread(g_arNpcThread[i]->m_pThread->m_hThread);
	}

	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& Test Code
	g_arEventNpcThread[0]->m_ThreadInfo.hWndMsg = this->GetSafeHwnd();
	for(j = 0; j < NPC_NUM; j++)
	{
		g_arEventNpcThread[0]->m_ThreadInfo.pNpc[j] = NULL;	// ÃÊ±â ¼ÒÈ¯ ¸÷ÀÌ ´ç¿¬È÷ ¾øÀ¸¹Ç·Î NULL·Î ÀÛµ¿À» ¾È½ÃÅ´
		g_arEventNpcThread[0]->m_ThreadInfo.m_lNpcUsed[i] = 0;
	}
	g_arEventNpcThread[0]->m_ThreadInfo.pCom = &m_Com;

	::ResumeThread(g_arEventNpcThread[0]->m_pThread->m_hThread);

}

///////////////////////////////////////////////////////////////////////////////////
//	Edit Control ¿¡¼­ ¿£ÅÍÅ°¸¦ Ã³¸®ÇÏ±â À§ÇØ¼­
//
BOOL CServerDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN) 
		{
			CWnd* pWnd = FromHandle(pMsg->hwnd);
			CWnd* pInput = GetDlgItem(IDC_EDIT_ANNOUNCE);
			
			if(pWnd == pInput) 
			{
				OnAnnounce();
				return TRUE;
			}
		}
		if(pMsg->wParam == VK_ESCAPE) return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////////////////
//	ÀüÃ¼ °ÔÀÓ »ç¿ëÀÚ¿¡°Ô °øÁö¸¦ ÇÑ´Ù.
//
void CServerDlg::OnAnnounce()
{
	int i = 0;
	if(m_Com.m_bInit == FALSE) return;

	CString strMsg = _T("");
//	CString strTemp = _T("");

	GetDlgItemText(IDC_EDIT_ANNOUNCE, strMsg);

	USER* pUser = NULL;
/*
	if(strMsg.GetAt(0) == '/')
	{
		strTemp = strMsg;
		strTemp.TrimLeft('/');
		strTemp.MakeUpper();

		if(strTemp.CompareNoCase("LOGOUT") == 0)
		{
			CloseGuildWhareHouse();
		}

		SetDlgItemText(IDC_EDIT_ANNOUNCE, _T(""));
		return;
	}
*/
	CBufferEx TempBuf;

	TempBuf.Add(SYSTEM_MSG);
	TempBuf.Add(SYSTEM_NORMAL);
	TempBuf.Add((LPTSTR)(LPCTSTR)strMsg, strMsg.GetLength());

	for(i = 0; i < MAX_USER; i++)
	{
		pUser = m_Com.GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		pUser->Send(TempBuf, TempBuf.GetLength());
	}

	SetDlgItemText(IDC_EDIT_ANNOUNCE, _T(""));
}

////////////////////////////////////////////////////////////////////////////////////
//	DB¿¡¼­ Á¸¹øÈ£¸¦ ÀúÀåÇÑ´Ù.
//
void CServerDlg::SetWeatherZone(int iZone)
{
	WEATHER_INFO *pWeather = NULL;

	pWeather = new WEATHER_INFO;

	pWeather->iZone = iZone;
	pWeather->bRaining = FALSE;
	g_WeatherZone.Add(pWeather);
}

/*
////////////////////////////////////////////////////////////////////////////////////
//	°ÔÀÓ»óÀÇ ½Ã°£À» ÀúÀåÇÑ´Ù.
//
void CServerDlg::SetClientWorldTime(CString strTime)
{
	CString strTemp = _T("");
	CString strTrim = _T("");
	CString strDate[4]; 
	int i, j;
	int iLength;

	if(strTime.GetLength() > 13) return;

	strTemp = strTime;

	for(i = 0; i < 4; i++)
	{
		strDate[i] = _T("");

		iLength = strTemp.GetLength();
		for(j = 0; j < iLength; j++)
		{
			if(strTemp.GetAt(j) == '.')
			{
				strTrim = strTemp.Right(iLength - j - 1);
				strDate[i] = strTemp.Left(j);	
				strTemp = strTrim;
				strTrim = _T("");
				break;
			}
		}

		if(i == 3) strDate[i] = strTemp;
	}

	g_GameYear	= atoi(strDate[0]);
	g_GameMon	= atoi(strDate[1]);
	g_GameDay	= atoi(strDate[2]);
	g_GameTime	= atoi(strDate[3]);
/*
	Temp = strTime.Left(4);
	
	g_GameYear = atoi(Temp);

	Temp = strTime.Left(7);
	g_GameMon = atoi(Temp.Right(2));

	Temp = strTime.Right(5);
	g_GameDay = atoi(Temp.Left(2));

	Temp = strTime.Right(2);
	g_GameTime = atoi(Temp);
	
}

*/


BOOL CServerDlg::GetPsiStoreTable()
{
	CPsiStoreSet	PsiStoreSet;

	_StoreDBArray	arTemp;

	_StoreDB*		pTempDB = NULL;
	
	try
	{
		PsiStoreSet.m_strSort = _T("sStoreID");

		if(PsiStoreSet.IsOpen()) PsiStoreSet.Close();
		
		if(!PsiStoreSet.Open())
		{
			AfxMessageBox(_T("Psi Store DB Open Fail!"));
			return FALSE;
		}
		if(PsiStoreSet.IsBOF()) 
		{
			AfxMessageBox(_T("Psi Store DB Empty!"));
			return FALSE;
		}

		while(!PsiStoreSet.IsEOF())
		{
			pTempDB = new _StoreDB;

			pTempDB->StoreID	= PsiStoreSet.m_sStoreID;
			pTempDB->ItemID		= PsiStoreSet.m_sPsiID;

			arTemp.Add(pTempDB);
			
			PsiStoreSet.MoveNext();
		}


		CStore*	pNewStore = NULL;
		int nTempID = -1;
		int i;
		BOOL bAdd = FALSE;

		int nStoreSize = arTemp.GetSize();

		for(i = 0; i < nStoreSize; i++)
		{
			if(nTempID != arTemp[i]->StoreID)
			{
				if(nTempID != -1) 
				{
					g_arPsiStore.Add(pNewStore);
				}

				pNewStore = new CStore;
				nTempID = pNewStore->m_sStoreID = arTemp[i]->StoreID;
				pNewStore->m_arItems.Add(arTemp[i]->ItemID);
			}
			else
			{
				pNewStore->m_arItems.Add(arTemp[i]->ItemID);
			}
		}
		g_arPsiStore.Add(pNewStore);

		for(i = 0; i < nStoreSize; i++)
		{
			if(arTemp[i])	delete arTemp[i];
		}
		arTemp.RemoveAll();
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////
//	°¢ °è¿­ÀÇ »ç¿ëÀÚ°¡ Ã³À½À¸·Î ¼ÒÁöÇÒ¼öÀÖ´Â ¾ÆÀÌÅÛÀ» ÀÐ¾îµéÀÎ´Ù.
//
BOOL CServerDlg::GetIntiItemTable()
{
	CInitItemTableSet	InitItemSet;

	try
	{
		if(InitItemSet.IsOpen()) InitItemSet.Close();
		
		if( !InitItemSet.Open() )
		{
			AfxMessageBox(_T("Item DB Open Fail!"));
			return FALSE;
		}
		if(InitItemSet.IsBOF()) 
		{
			AfxMessageBox(_T("Item DB Empty!"));
			return FALSE;
		}
		
		while( !InitItemSet.IsEOF() )
		{
			InitItemTable *item = new InitItemTable;
			item->tClass = InitItemSet.m_tTypeClass;		// °è¿­
			item->iSid = InitItemSet.m_sSid;				// ÁÖ ¹«±â 
			item->iSubSid = InitItemSet.m_sSubSid;			// º¸Á¶ ¹«±â (¿¹)ÅºÃ¢...

			g_arInitItem.Add(item);
			
			InitItemSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	¸ÅÁ÷ ¼Ó¼º¸¦ Ç¥½ÃÇÏ´Â ÂüÁ¶Ç¥¸¦ ÀÐ¾îµéÀÎ´Ù.
//
BOOL CServerDlg::GetMagicItemTable()
{
	CMagicItemTableSet	MagicItemSet;

	try
	{
		if(MagicItemSet.IsOpen()) MagicItemSet.Close();
		
		MagicItemSet.m_strSort = "sSid ASC";

		if( !MagicItemSet.Open() )
		{
			AfxMessageBox(_T("Item DB Open Fail!"));
			return FALSE;
		}
		if(MagicItemSet.IsBOF()) 
		{
			AfxMessageBox(_T("Item DB Empty!"));
			return FALSE;
		}
		
		while( !MagicItemSet.IsEOF() )
		{
			CMagicItemTable *magic_item = new CMagicItemTable;

			magic_item->m_sSid			= MagicItemSet.m_sSid;
			magic_item->m_sSubType		= MagicItemSet.m_sSubType;
			magic_item->m_sChangeValue	= MagicItemSet.m_sChangeValue;
			magic_item->m_tUpgrade		= MagicItemSet.m_tUpgrade;
			magic_item->m_tNeedClass	= MagicItemSet.m_tNeedClass;
			magic_item->m_tWearInfo		= MagicItemSet.m_tWearInfo;
			magic_item->m_tLevel		= MagicItemSet.m_tLevel;
			magic_item->m_strText       = MagicItemSet.m_strText;//ÆÀ·ÖÏÔÊ¾Õß
			magic_item->m_tUse			= MagicItemSet.m_tUse;
			magic_item->m_sAid			= 0;

			g_arMagicItemTable.Add(magic_item);
			
			MagicItemSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	CAccessoriUpTableSet	AccessoriUpSet;

	try
	{
		if(AccessoriUpSet.IsOpen()) AccessoriUpSet.Close();
		
		if( !AccessoriUpSet.Open() )
		{
			AfxMessageBox(_T("Accessori Upgrade DB Open Fail!"));
			return FALSE;
		}
		if(AccessoriUpSet.IsBOF()) 
		{
			AfxMessageBox(_T("Accessori Upgrade DB Empty!"));
			return FALSE;
		}
		
		while( !AccessoriUpSet.IsEOF() )
		{
			CAccessoriUpTable *AccUp = new CAccessoriUpTable;

			AccUp->m_sAid	= AccessoriUpSet.m_sAid;
			AccUp->m_sSid	= AccessoriUpSet.m_sSid;
			AccUp->m_tGroup	= AccessoriUpSet.m_tGroup;
			AccUp->m_tType	= AccessoriUpSet.m_tType;

			g_arAccessoriUpTable.Add(AccUp);
			
			if(AccUp->m_tType == 0 && AccUp->m_tGroup == 1) g_Ripel.m_arRipelTop.Add(AccUp->m_sSid);
			if(AccUp->m_tType == 1 && AccUp->m_tGroup == 1) g_Ripel.m_arRipelBottom.Add(AccUp->m_sSid);
			if(AccUp->m_tType == 2 && AccUp->m_tGroup == 1) g_Ripel.m_arRipelLeft.Add(AccUp->m_sSid);
			if(AccUp->m_tType == 3 && AccUp->m_tGroup == 1) g_Ripel.m_arRipelRight.Add(AccUp->m_sSid);
			//if(AccUp->m_tGroup == 1) g_Ripel.m_arRipelCrest.Add(AccUp->m_sSid);
			if(AccUp->m_tType == 4 && AccUp->m_tGroup == 1) g_Ripel.m_arRipelCrest.Add(AccUp->m_sSid);//Ê×ÊÎÊôÐÔ±í

			AccessoriUpSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	short sSid;
	for(int i = 0; i < g_arAccessoriUpTable.GetSize(); i++)
	{
		sSid = g_arAccessoriUpTable[i]->m_sSid;
		for(int j = 0; j < g_arMagicItemTable.GetSize(); j++)
		{
			if(sSid == g_arMagicItemTable[j]->m_sSid) 
			{
				g_arMagicItemTable[j]->m_sAid = g_arAccessoriUpTable[i]->m_sAid;
				break;
			}
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	°¢ °è¿­ÀÇ È¸º¹ ·®À» °áÁ¤
//
BOOL CServerDlg::GetUserRecoverTable()
{
	CRecoverSet	RecoverSet;

	try
	{
		if(RecoverSet.IsOpen()) RecoverSet.Close();
		
		if( !RecoverSet.Open() )
		{
			AfxMessageBox(_T("HP,PP,SP Recover DB Open Fail!"));
			return FALSE;
		}
		if(RecoverSet.IsBOF()) 
		{
			AfxMessageBox(_T("HP,PP,SP Recover DB Empty!"));
			return FALSE;
		}
		
		while( !RecoverSet.IsEOF() )
		{
			CRecover *recover = new CRecover;

			recover->m_sSid = RecoverSet.m_sSid;
			recover->m_byHPDivide = RecoverSet.m_byHPDivide;
			recover->m_byPPDivide = RecoverSet.m_byPPDivide;
			recover->m_bySPDivide = RecoverSet.m_bySPDivide;
			recover->m_byTown = RecoverSet.m_byTown;

			g_arRecoverTable.Add(recover);
			
			RecoverSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;

}

////////////////////////////////////////////////////////////////////////////////////
//	°¢ °è¿­ÀÇ È¸º¹µÇ´Â ½Ã°£°áÁ¤
//
BOOL CServerDlg::GetUserRecoverRateTable()
{
	CRecoverRateSet	RecoverRateSet;

	try
	{
		if(RecoverRateSet.IsOpen()) RecoverRateSet.Close();
		
		if( !RecoverRateSet.Open() )
		{
			AfxMessageBox(_T("HP,PP,SP RecoverRate DB Open Fail!"));
			return FALSE;
		}
		if(RecoverRateSet.IsBOF()) 
		{
			AfxMessageBox(_T("HP,PP,SP RecoverRate DB Empty!"));
			return FALSE;
		}
		
		while( !RecoverRateSet.IsEOF() )
		{
			CRecoverRate *recover_rate = new CRecoverRate;

			recover_rate->m_sSid = RecoverRateSet.m_sSid;
			recover_rate->m_byHpSpeedupLevel = RecoverRateSet.m_byHpSpeedupLevel;
			recover_rate->m_sHpSpeedupTime = RecoverRateSet.m_sHpSpeedupTime;

			recover_rate->m_byPpSpeedupLevel = RecoverRateSet.m_byPpSpeedupLevel;
			recover_rate->m_sPpSpeedupTime = RecoverRateSet.m_sPpSpeedupTime;

			recover_rate->m_bySpSpeedupLevel = RecoverRateSet.m_bySpSpeedupLevel;
			recover_rate->m_sSpSpeedupTime = RecoverRateSet.m_sSpSpeedupTime;

			g_arRecoverRateTable.Add(recover_rate);
			
			RecoverRateSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////
//	¸÷ÀÌ ¶³±¸´Â DNÀÇ ¹üÀ§
//
BOOL CServerDlg::GetMonHaveDNTable()
{
	CDNTableSet		DnSet;

	try
	{
		if(DnSet.IsOpen()) DnSet.Close();
		
		if( !DnSet.Open() )
		{
			AfxMessageBox(_T("DN DB Open Fail!"));
			return FALSE;
		}
		if(DnSet.IsBOF()) 
		{
			AfxMessageBox(_T("DN DB Empty!"));
			return FALSE;
		}
		
		while( !DnSet.IsEOF() )
		{
			CDNTable *Dn = new CDNTable;

			Dn->m_sIndex = DnSet.m_sIndex;
			Dn->m_sMinDn = DnSet.m_sMinDn;
			Dn->m_sMaxDn = DnSet.m_sMaxDn;

			g_arDNTable.Add(Dn);
			
			DnSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	º¯Çü¾ÆÀÌÅÛ ÂüÁ¶
//
BOOL CServerDlg::GetValItemTable()
{
	CValItemTableSet		ValItemSet;
	CDBVariant varValue;
	int nRowCount = 0;
	int i;
	int nItem = 0;

	try
	{
		if(ValItemSet.IsOpen()) ValItemSet.Close();

		ValItemSet.m_strSort = _T("sSid");

		if(!ValItemSet.Open())
		{
			AfxMessageBox(_T("VAL_ITEM DB Open Fail!"));
			return FALSE;
		}

		if(ValItemSet.IsBOF()) 
		{
			AfxMessageBox(_T("VAL_ITEM DB Empty!"));
			return FALSE;
		}

		while(!ValItemSet.IsEOF())
		{
			nRowCount++;
			ValItemSet.MoveNext();
		}

		g_ValItem.m_nField = ValItemSet.m_nFields;
		g_ValItem.m_nRow = nRowCount;

		if(nRowCount == 0) return FALSE;

		g_ValItem.m_ppItem = new int* [g_ValItem.m_nField];
		for(i = 0; i < g_ValItem.m_nField; i++)
		{
			g_ValItem.m_ppItem[i] = new int[g_ValItem.m_nRow];
		}

		ValItemSet.MoveFirst();

		nRowCount = 0;
		while(!ValItemSet.IsEOF())
		{
			for(i = 0; i < g_ValItem.m_nField; i++)
			{
				ValItemSet.GetFieldValue(i, varValue, SQL_C_SSHORT);
				nItem = varValue.m_iVal;
				g_ValItem.m_ppItem[i][nRowCount] = varValue.m_iVal;
			}
			nRowCount++;
			if(nRowCount >= g_ValItem.m_nRow) break;

			ValItemSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();

		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();

		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	°¢ ±æµå µ¥ÀÌÅÍ¸¦ Load
//
BOOL CServerDlg::GetGuildTable()
{
	int i, nLen = 0;

	CGuild* pNewGuild = NULL;

	for(i = 0; i < g_arGuildData.GetSize(); i++)
	{
		if( g_arGuildData[i] )
		{
			delete g_arGuildData[i];
		}
	}
	g_arGuildData.RemoveAll();
	g_arGuildData.SetSize(MAX_GUILD);

	for(i = 0; i < g_arGuildData.GetSize(); i++)
	{
		pNewGuild = new CGuild;

		g_arGuildData[i] = pNewGuild;
	}

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT * FROM GUILD"));
	
	SQLUINTEGER		iDN;//, iEXP;
	SQLINTEGER		iSID;
	SQLSMALLINT		sVERSION;
	SQLCHAR			strGuildName[CHAR_NAME_LENGTH + 1], strMasterName[CHAR_NAME_LENGTH + 1], strSubMasterName[CHAR_NAME_LENGTH + 1];
	SQLCHAR			strItem[_BANK_DB], strMark[GUILD_MARK_SIZE], strHaveMap[GUILD_MAP_COUNT];

	SQLINTEGER		sInd;

	iSID = 0;
	iDN = 0;//iEXP = 0;
	sVERSION = 0;

	g_CurrentGuildCount = 0;			// ÇöÀç µî·ÏÇÑ ±æµåÀÇ ¸¶Áö¸· ¹øÈ£ ¼ÂÆÃ

	::ZeroMemory(strGuildName, sizeof(strGuildName));
	::ZeroMemory(strMasterName, sizeof(strMasterName));
	::ZeroMemory(strSubMasterName, sizeof(strSubMasterName));

	::ZeroMemory(strMark, sizeof(strMark));
	::ZeroMemory(strHaveMap, sizeof(strHaveMap));
	::ZeroMemory(strItem, sizeof(strItem));

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild Data !!\n");

		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SLONG, &iSID, sizeof(iSID), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR, &strGuildName, sizeof(strGuildName), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR, &strMasterName, sizeof(strMasterName), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR, &strSubMasterName, sizeof(strSubMasterName), &sInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY, &strMark, sizeof(strMark), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sVERSION, sizeof(sVERSION), &sInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG, &iDN, sizeof(iDN), &sInd );

				SQLGetData( hstmt, i++, SQL_C_BINARY, &strItem,	sizeof(strItem), &sInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY, &strHaveMap, sizeof(strHaveMap), &sInd );
	//			SQLGetData( hstmt, i++, SQL_C_ULONG, &iEXP, sizeof(iEXP), &sInd );

				if(iSID <= 0 || iSID >= MAX_GUILD ) 
				{
					TRACE(_T("±æµå ÀüÃ¼ Å©±â°¡ MAX_GUILD¸¦ ³Ñ¾ú´Ù."));
					continue;
				}

				pNewGuild = NULL;
				pNewGuild = g_arGuildData[iSID];
//				CGuild* pNewGuild = new CGuild;

				pNewGuild->m_lSid = iSID;
				strcpy(pNewGuild->m_strGuildName, (CHAR*)strGuildName);
				strcpy(pNewGuild->m_strMasterName, (CHAR*)strMasterName);

				nLen = strlen((CHAR*)strSubMasterName);
				if(nLen > 0 && nLen <= CHAR_NAME_LENGTH) 
				{
					pNewGuild->m_lSubMaster = 1;
					strcpy(pNewGuild->m_strSubMasterName, (CHAR*)strSubMasterName);
				}
				else 
				{
					pNewGuild->m_lSubMaster = 0;
					::ZeroMemory(pNewGuild->m_strSubMasterName, sizeof(pNewGuild->m_strSubMasterName));
				}

				pNewGuild->m_sVersion = sVERSION;
				pNewGuild->m_dwGuildDN = iDN;
//				pNewGuild->m_dwExp = iEXP;

				pNewGuild->StrToGuildItem((LPTSTR)strItem);
				pNewGuild->strToGuildMap((LPTSTR)strHaveMap);
				::CopyMemory(pNewGuild->m_strMark, strMark, sizeof(pNewGuild->m_strMark));

//				g_arGuildData[iSID] = pNewGuild;

				g_CurrentGuildCount = iSID;

				::ZeroMemory(strGuildName, sizeof(strGuildName));
				::ZeroMemory(strMasterName, sizeof(strMasterName));
				::ZeroMemory(strSubMasterName, sizeof(strSubMasterName));
				::ZeroMemory(strMark, sizeof(strMark));
				::ZeroMemory(strHaveMap, sizeof(strHaveMap));
				::ZeroMemory(strItem, sizeof(strItem));
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;	
}

////////////////////////////////////////////////////////////////////////////////////
//	°¢ ±æµå À¯Àú µ¥ÀÌÅÍ¸¦ Load
//
BOOL CServerDlg::GetGuildUserData()
{
	int i;
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT * FROM GUILD_USER"));

	SQLINTEGER		iSID;
	SQLCHAR			strUserId[CHAR_NAME_LENGTH + 1];

	SQLINTEGER		sInd;

	iSID = 0;
	::ZeroMemory(strUserId, sizeof(strUserId));

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild User Data !!\n");

		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SLONG, &iSID, sizeof(iSID), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR, &strUserId, sizeof(strUserId), &sInd );

				if(iSID < 0 || iSID >= MAX_GUILD ) continue;

				if(!g_arGuildData[iSID]) continue;

//				CGuildUser* pNewGuildUser = new CGuildUser;

//				pNewGuildUser->m_lSid = iSID;
//				strcpy(pNewGuildUser->m_strUserId, (CHAR*)strUserId);

				g_arGuildData[iSID]->AddUser((TCHAR *)strUserId, iSID);
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;	
}

////////////////////////////////////////////////////////////////////////////////////
//	E_Body Identify Table À» Load ÇÑ´Ù.
//
BOOL CServerDlg::GetEBodyIdentifyTable()
{
	CEBodyIdentifyTableSet	EBodySet;

	try
	{
		if(EBodySet.IsOpen()) EBodySet.Close();
		
		if( !EBodySet.Open() )
		{
			AfxMessageBox(_T("EBody Identify Table Open Fail!"));
			return FALSE;
		}
		if(EBodySet.IsBOF()) 
		{
			AfxMessageBox(_T("EBODY_UPGRADE DB Empty!"));
			return FALSE;
		}
		
		while( !EBodySet.IsEOF() )
		{
			CEBodyIdentifyTable *eBody = new CEBodyIdentifyTable;
			
			eBody->m_sSid	= EBodySet.m_sSid;

			g_arEBodyIdentifyTable.Add(eBody);
			
			EBodySet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	E_Body Upgrade Table À» Load ÇÑ´Ù.
//
BOOL CServerDlg::GetEBodyUpgradeTable()
{
	CEBodyUpgradeTableSet	EBodySet;

	try
	{
		if(EBodySet.IsOpen()) EBodySet.Close();
		
		if( !EBodySet.Open() )
		{
			AfxMessageBox(_T("E_Body Upgrade Table Open Fail!"));
			return FALSE;
		}
		if(EBodySet.IsBOF()) 
		{
			AfxMessageBox(_T("EBODY_UPGRADE DB Empty!"));
			return FALSE;
		}
		
		while( !EBodySet.IsEOF() )
		{
			CEBodyUpgradeTable *eBody = new CEBodyUpgradeTable;
			
			eBody->m_tRandom1		= EBodySet.m_tRandom1;
			eBody->m_tRandom2		= EBodySet.m_tRandom2;
			eBody->m_tRandom3		= EBodySet.m_tRandom3;

			g_arEBodyUpgradeTable.Add(eBody);
			
			EBodySet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	E_Body Table À» Load
//
BOOL CServerDlg::GetEBodyTable()
{
	CEBodyTableSet		EBodySet;

	try
	{
		EBodySet.m_strSort = _T("tSid");
		if(EBodySet.IsOpen()) EBodySet.Close();
		
		if( !EBodySet.Open() )
		{
			AfxMessageBox(_T("E_Body Table Open Fail!"));
			return FALSE;
		}
		if(EBodySet.IsBOF()) 
		{
			AfxMessageBox(_T("E_BODY DB Empty!"));
			return FALSE;
		}
		
		while( !EBodySet.IsEOF() )
		{
			CEBodyTable *eBody = new CEBodyTable;
			
			eBody->m_tSid			= EBodySet.m_tSid;
			eBody->m_sSubType		= EBodySet.m_sSubType;
			eBody->m_sChangeValue	= EBodySet.m_sChangeValue;
			eBody->m_sRandom		= EBodySet.m_sRandom;
			eBody->m_tLevel			= EBodySet.m_tLevel;
			eBody->m_tNeedClass		= EBodySet.m_tNeedClass;
			eBody->m_tUpgrade		= EBodySet.m_tUpgrade;
			eBody->m_tWearInfo		= EBodySet.m_tWearInfo;

			g_arEBodyTable.Add(eBody);
			
			EBodySet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	E-Body »óÁ¡À» ¿¬´Ù.
//
BOOL CServerDlg::GetEBodyStoreTable()
{
	CEBodyStoreSet	EbodyStoreSet;

	_StoreDBArray	arTemp;

	_StoreDB*		pTempDB = NULL;
	
	try
	{
		EbodyStoreSet.m_strSort = _T("sStoreID");

		if(EbodyStoreSet.IsOpen()) EbodyStoreSet.Close();
		
		if(!EbodyStoreSet.Open())
		{
			AfxMessageBox(_T("E_Body Store DB Open Fail!"));
			return FALSE;
		}
		if(EbodyStoreSet.IsBOF()) 
		{
			AfxMessageBox(_T("E_Body Store DB Empty!"));
			return FALSE;
		}

		while(!EbodyStoreSet.IsEOF())
		{
			pTempDB = new _StoreDB;

			pTempDB->StoreID	= EbodyStoreSet.m_sStoreID;
			pTempDB->ItemID		= EbodyStoreSet.m_sEbodyID;

			arTemp.Add(pTempDB);
			
			EbodyStoreSet.MoveNext();
		}


		CStore*	pNewStore = NULL;
		int nTempID = -1;
		int i;
		BOOL bAdd = FALSE;

		int nStoreSize = arTemp.GetSize();

		for(i = 0; i < nStoreSize; i++)
		{
			if(nTempID != arTemp[i]->StoreID)
			{
				if(nTempID != -1) 
				{
					g_arStore.Add(pNewStore);
				}

				pNewStore = new CStore;
				nTempID = pNewStore->m_sStoreID = arTemp[i]->StoreID;
				pNewStore->m_arItems.Add(arTemp[i]->ItemID);
			}
			else
			{
				pNewStore->m_arItems.Add(arTemp[i]->ItemID);
			}
		}
		g_arEBodyStore.Add(pNewStore);

		for(i = 0; i < nStoreSize; i++)
		{
			if(arTemp[i])	delete arTemp[i];
		}
		arTemp.RemoveAll();
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////
//»ÃÊ¯
BOOL CServerDlg::GetHuanshiTable()
{
	CHuanshiTableSet		HuanshiSet;

	try
	{
		HuanshiSet.m_strSort = _T("tSid");
		if(HuanshiSet.IsOpen()) HuanshiSet.Close();
		
		if( !HuanshiSet.Open() )
		{
			AfxMessageBox(_T("Huan_stno Table Open Fail!"));
			return FALSE;
		}
		if(HuanshiSet.IsBOF()) 
		{
			AfxMessageBox(_T("Huan_Stno DB Empty!"));
			return FALSE;
		}
		
		while( !HuanshiSet.IsEOF() )
		{
			CHuanshiTable *HUAN_STNO = new CHuanshiTable;
			
			HUAN_STNO->m_tSid			= HuanshiSet.m_tSid;
			HUAN_STNO->m_sSubType		= HuanshiSet.m_sSubType;
			HUAN_STNO->m_sChangeValue	= HuanshiSet.m_sChangeValue;
			HUAN_STNO->m_sRandom		= HuanshiSet.m_sRandom;
			HUAN_STNO->m_tLevel		= HuanshiSet.m_tLevel;
			HUAN_STNO->m_tNeedClass	= HuanshiSet.m_tNeedClass;
			HUAN_STNO->m_tUpgrade		= HuanshiSet.m_tUpgrade;
			HUAN_STNO->m_tWearInfo	= HuanshiSet.m_tWearInfo;

			g_arHuanshiTable.Add(HUAN_STNO);
			
			HuanshiSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}
BOOL CServerDlg::GetMESSAGETable()
{
	int i;
	int MAX_MESSAGE = 100;
	if(g_arMessAgeArray.GetSize() > 0)		g_arMessAgeArray.RemoveAll();	
	
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT TOP 100 * FROM MESSAGE"));

	SQLSMALLINT		MID;
	SQLCHAR			MESSAGEBODY[100];
	SQLINTEGER		sInd;

	MID = 0;
	::ZeroMemory(MESSAGEBODY, sizeof(MESSAGEBODY));

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load MESSAGE Data !!\n");
		AfxMessageBox(_T("MESSAGE Table Open Fail!"));
		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &MID, sizeof(MID), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  &MESSAGEBODY, sizeof(MESSAGEBODY), &sInd );			
				
				if(MID > MAX_MESSAGE) continue;
				MESSAGE	*MSG = new MESSAGE;
				MSG->m_ID = MID;			
				strcpy_s(MSG->m_MESSAGE_BODY, (CHAR*)MESSAGEBODY);				
				g_arMessAgeArray.Add(MSG);			
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;
}
BOOL CServerDlg::GetOnlineShopTable()
{
	int i;

	for(i = 0; i < g_arOnlineShopTable.GetSize(); i++) g_arOnlineShopTable.RemoveAll();

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT * FROM OnlineShop"));

	SQLSMALLINT		oSid;
	SQLSMALLINT		iSid;
	SQLCHAR			iSname[20];
	SQLSMALLINT		price;
	SQLSMALLINT		iNum;
	SQLSMALLINT		sx1;
	SQLSMALLINT		sx2;
	SQLSMALLINT		sx3;
	SQLSMALLINT		sx4;
	SQLSMALLINT		sx5;
	SQLSMALLINT		upgrade;
	SQLSMALLINT		sx6;
	SQLSMALLINT		sx7;
	SQLSMALLINT		sx8;
	SQLSMALLINT		sx9;
	SQLSMALLINT		sx10;
	SQLCHAR			iText[50];

	SQLINTEGER		sInd;

	::ZeroMemory(iSname, sizeof(iSname));
	::ZeroMemory(iText, sizeof(iText));
	oSid = 0;	iSid = 0;	price = 0;	iNum = 0;
	sx1 = 0;	sx2 = 0;	sx3 = 0;	sx4	= 0;	sx5 = 0;
	upgrade = 0;
	sx6 = 0;	sx7 = 0;	sx8 = 0;	sx9 = 0;	sx10 = 0;

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load UserSort Data !!\n");
		AfxMessageBox(_T("OnlineShop Table Open Fail!"));
		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SSHORT, &oSid, sizeof(oSid), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &iSid, sizeof(iSid), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  &iSname, sizeof(iSname), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &price, sizeof(price), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &iNum, sizeof(iNum), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx1, sizeof(sx1), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx2, sizeof(sx2), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx3, sizeof(sx3), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx4, sizeof(sx4), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx5, sizeof(sx5), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &upgrade, sizeof(upgrade), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx6, sizeof(sx6), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx7, sizeof(sx7), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx8, sizeof(sx8), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx9, sizeof(sx9), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx10, sizeof(sx10), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  &iText, sizeof(iText), &sInd );
				
				OnlineShop *OSP = new OnlineShop;
				
				OSP->m_oSid = oSid;
				OSP->m_iSid = iSid;
				strcpy_s(OSP->m_iSname, (CHAR*)iSname);
				OSP->m_price = price;
				OSP->m_iNum	 = iNum;
				OSP->m_sx1	 = sx1;
				OSP->m_sx2	 = sx2;
				OSP->m_sx3	 = sx3;
				OSP->m_sx4	 = sx4;
				OSP->m_sx5	 = sx5;
				OSP->m_upgrade = upgrade;
				OSP->m_sx6	 = sx6;
				OSP->m_sx7	 = sx7;
				OSP->m_sx8	 = sx8;
				OSP->m_sx9	 = sx9;
				OSP->m_sx10	 = sx10;
				strcpy_s(OSP->m_iText, (CHAR*)iText);

				g_arOnlineShopTable.Add(OSP);
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;

}

BOOL CServerDlg::GetKaixiangziTable()
{
	int i;

	for(i = 0; i < g_arKaixiangziTable.GetSize(); i++) g_arKaixiangziTable.RemoveAll();

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT * FROM Kaixiangzi"));

	SQLSMALLINT		oSid;
	SQLSMALLINT		iSid;
	SQLCHAR			iSname[20];
	SQLSMALLINT		price;
	SQLSMALLINT		iNum;
	SQLSMALLINT		sx1;
	SQLSMALLINT		sx2;
	SQLSMALLINT		sx3;
	SQLSMALLINT		sx4;
	SQLSMALLINT		sx5;
	SQLSMALLINT		upgrade;
	SQLSMALLINT		sx6;
	SQLSMALLINT		sx7;
	SQLSMALLINT		sx8;
	SQLSMALLINT		sx9;
	SQLSMALLINT		sx10;
	SQLCHAR			iText[50];

	SQLINTEGER		sInd;

	::ZeroMemory(iSname, sizeof(iSname));
	::ZeroMemory(iText, sizeof(iText));
	oSid = 0;	iSid = 0;	price = 0;	iNum = 0;
	sx1 = 0;	sx2 = 0;	sx3 = 0;	sx4	= 0;	sx5 = 0;
	upgrade = 0;
	sx6 = 0;	sx7 = 0;	sx8 = 0;	sx9 = 0;	sx10 = 0;

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load UserSort Data !!\n");
		AfxMessageBox(_T("Kaixiangzi Table Open Fail!"));
		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SSHORT, &oSid, sizeof(oSid), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &iSid, sizeof(iSid), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  &iSname, sizeof(iSname), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &price, sizeof(price), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &iNum, sizeof(iNum), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx1, sizeof(sx1), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx2, sizeof(sx2), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx3, sizeof(sx3), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx4, sizeof(sx4), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx5, sizeof(sx5), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &upgrade, sizeof(upgrade), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx6, sizeof(sx6), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx7, sizeof(sx7), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx8, sizeof(sx8), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx9, sizeof(sx9), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx10, sizeof(sx10), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  &iText, sizeof(iText), &sInd );
				
				Kaixiangzi *OSP = new Kaixiangzi;
				
				OSP->m_oSid = oSid;
				OSP->m_iSid = iSid;
				strcpy_s(OSP->m_iSname, (CHAR*)iSname);
				OSP->m_price = price;
				OSP->m_iNum	 = iNum;
				OSP->m_sx1	 = sx1;
				OSP->m_sx2	 = sx2;
				OSP->m_sx3	 = sx3;
				OSP->m_sx4	 = sx4;
				OSP->m_sx5	 = sx5;
				OSP->m_upgrade = upgrade;
				OSP->m_sx6	 = sx6;
				OSP->m_sx7	 = sx7;
				OSP->m_sx8	 = sx8;
				OSP->m_sx9	 = sx9;
				OSP->m_sx10	 = sx10;
				strcpy_s(OSP->m_iText, (CHAR*)iText);

				g_arKaixiangziTable.Add(OSP);
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;

}

BOOL CServerDlg::GetSxOnlineShopTable()
{
	int i;

	for(i = 0; i < g_arSxOnlineShopTable.GetSize(); i++) g_arSxOnlineShopTable.RemoveAll();

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT * FROM SxOnlineShop"));

	SQLSMALLINT		oSid;
	SQLSMALLINT		iSid;
	SQLCHAR			iSname[20];
	SQLSMALLINT		sLevel;		
	SQLSMALLINT		sDuration;
	SQLSMALLINT		sBullNum;
	SQLSMALLINT		tIQ;
	SQLSMALLINT		prices;
	SQLSMALLINT		iNum;
	SQLSMALLINT		sx1;
	SQLSMALLINT		sx2;
	SQLSMALLINT		sx3;
	SQLSMALLINT		sx4;
	SQLSMALLINT		sx5;
	SQLSMALLINT		upgrade;
	SQLSMALLINT		sx6;
	SQLSMALLINT		sx7;
	SQLSMALLINT		sx8;
	SQLSMALLINT		sx9;
	SQLSMALLINT		sx10;
	SQLCHAR			iText[50];

	SQLINTEGER		sInd;

	::ZeroMemory(iSname, sizeof(iSname));
	::ZeroMemory(iText, sizeof(iText));
	oSid = 0;	iSid = 0;	prices = 0;	iNum = 0;
	sLevel = 0;	sDuration = 0; sBullNum = 0;  tIQ = 0;
	sx1 = 0;	sx2 = 0;	sx3 = 0;	sx4	= 0;	sx5 = 0;
	upgrade = 0;
	sx6 = 0;	sx7 = 0;	sx8 = 0;	sx9 = 0;	sx10 = 0;

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load UserSort Data !!\n");
		AfxMessageBox(_T("SxOnlineShop Table Open Fail!"));
		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SSHORT, &oSid, sizeof(oSid), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &iSid, sizeof(iSid), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  &iSname, sizeof(iSname), &sInd );
				
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sLevel, sizeof(sLevel), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sDuration, sizeof(sDuration), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBullNum, sizeof(sBullNum), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &tIQ, sizeof(tIQ), &sInd );

				SQLGetData( hstmt, i++, SQL_C_SSHORT, &prices, sizeof(prices), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &iNum, sizeof(iNum), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx1, sizeof(sx1), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx2, sizeof(sx2), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx3, sizeof(sx3), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx4, sizeof(sx4), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx5, sizeof(sx5), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &upgrade, sizeof(upgrade), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx6, sizeof(sx6), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx7, sizeof(sx7), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx8, sizeof(sx8), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx9, sizeof(sx9), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx10, sizeof(sx10), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  &iText, sizeof(iText), &sInd );
				
				SxOnlineShop *OSP = new SxOnlineShop;

				OSP->m_oSid = oSid;
				OSP->m_iSid = iSid;
				strcpy(OSP->m_iSname, (CHAR*)iSname);

				OSP->m_sLevel = sLevel;		
				OSP->m_sDuration = sDuration;
				OSP->m_sBullNum = sBullNum;
				OSP->m_tIQ = tIQ;
			
				OSP->m_prices = prices;

				OSP->m_iNum	 = iNum;

				OSP->m_sx1	 = sx1;

				OSP->m_sx2	 = sx2;

				OSP->m_sx3	 = sx3;

				OSP->m_sx4	 = sx4;

				OSP->m_sx5	 = sx5;

				OSP->m_upgrade = upgrade;

				OSP->m_sx6	 = sx6;

				OSP->m_sx7	 = sx7;

				OSP->m_sx8	 = sx8;

				OSP->m_sx9	 = sx9;

				OSP->m_sx10	 = sx10;


				strcpy(OSP->m_iText, (CHAR*)iText);

				g_arSxOnlineShopTable.Add(OSP);
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;

}
BOOL CServerDlg::GetOnlineRMBShopTable()
{
	int i;

	//for(i = 0; i < g_arOnlineRMBShopTable.GetSize(); i++) 
	g_arOnlineRMBShopTable.RemoveAll();

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT rmbpoint,iSid,iSname,tIQ,iNum,sx1,sx2,sx3,sx4,sx5,upgrade,sx6,sx7,sx8,sx9,sx10,iText FROM rmbshopitem Order by number"));

	SQLINTEGER		rmbpoint;
	SQLSMALLINT		iSid;
	SQLCHAR			iSname[20];
	SQLINTEGER		iNum;
	SQLSMALLINT		tIQ;
	SQLSMALLINT		sx1;
	SQLSMALLINT		sx2;
	SQLSMALLINT		sx3;
	SQLSMALLINT		sx4;
	SQLSMALLINT		sx5;
	SQLSMALLINT		upgrade;
	SQLSMALLINT		sx6;
	SQLSMALLINT		sx7;
	SQLSMALLINT		sx8;
	SQLSMALLINT		sx9;
	SQLSMALLINT		sx10;
	SQLCHAR			iText[200];

	SQLINTEGER		sInd;

	::ZeroMemory(iSname, sizeof(iSname));
	::ZeroMemory(iText, sizeof(iText));
	rmbpoint = 0;	iSid = 0;	iNum = 0;
	sx1 = 0;	sx2 = 0;	sx3 = 0;	sx4	= 0;	sx5 = 0;
	upgrade = 0;
	sx6 = 0;	sx7 = 0;	sx8 = 0;	sx9 = 0;	sx10 = 0;

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load UserSort Data !!\n");
		AfxMessageBox(_T("OnlineRMBShop Table Open Fail!"));
		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		CRMBExchangeShop *OSP = NULL;
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SSHORT, &rmbpoint, sizeof(rmbpoint), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &iSid, sizeof(iSid), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  &iSname, sizeof(iSname), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &tIQ, sizeof(tIQ), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SLONG, &iNum, sizeof(iNum), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx1, sizeof(sx1), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx2, sizeof(sx2), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx3, sizeof(sx3), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx4, sizeof(sx4), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx5, sizeof(sx5), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &upgrade, sizeof(upgrade), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx6, sizeof(sx6), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx7, sizeof(sx7), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx8, sizeof(sx8), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx9, sizeof(sx9), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sx10, sizeof(sx10), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  &iText, sizeof(iText), &sInd );

				OSP = new CRMBExchangeShop;
				if(OSP == NULL)
					continue;
				ZeroMemory(OSP,sizeof(CRMBExchangeShop));
				OSP->m_rmbCode = rmbpoint;
				OSP->m_iSid = iSid;
				strcpy(OSP->m_iSname, (CHAR*)iSname);
				OSP->m_iSname[sizeof(OSP->m_iSname)-1] = 0;
				OSP->m_tIQ = (BYTE)tIQ;

				OSP->m_iNum	 = iNum;
				OSP->m_sx1	 = sx1;
				OSP->m_sx2	 = sx2;
				OSP->m_sx3	 = sx3;
				OSP->m_sx4	 = sx4;
				OSP->m_sx5	 = sx5;
				OSP->m_upgrade = upgrade;
				OSP->m_sx6	 = sx6;
				OSP->m_sx7	 = sx7;
				OSP->m_sx8	 = sx8;
				OSP->m_sx9	 = sx9;
				OSP->m_sx10	 = sx10;
				if(strlen((CHAR*)iText) > 0)
					strcpy(OSP->m_iText, (CHAR*)iText);
				OSP->m_iSname[sizeof(OSP->m_iText)-1] = 0;
				g_arOnlineRMBShopTable.Add(OSP);
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////
//	DB Agent ¿¡¼­ ¸¸µç Shared Memory ¸¦ ¿¬´Ù.
//
BOOL CServerDlg::OpenMemoryDB()
{
	CString szName = _T("");
	UINT nSize = sizeof(CMemUser);

	UINT nStoreSize = sizeof(CMemStore);
	UINT nFortressSize = sizeof(CMemFortress);
	UINT nGuildHouseSize = sizeof(CMemGuildHouseRank);


	int i = 0;
	for(i = 0; i < MAX_USER; i++)
	{
		szName.Format("droiyan-Online-MemoryDB-%04d", i);
		CSharedMemory* pNew = NULL;

		try
		{
			pNew = new CSharedMemory;

			if(!pNew->Open(szName, nSize))
			{
				delete pNew;
				szName.Format("Open [ %d ] Shared Memory Fail!!, GetLastError = %d", i+1, GetLastError()); 
				AfxMessageBox(szName);
				return FALSE;
			}

			g_arSharedMemory.Add(pNew);
		}
		catch (CMemoryException* e)
		{
			szName.Format("Unable to allocate [ %d ] Shared Memory!!, GetLastError = %d", i+1, GetLastError()); 
			AfxMessageBox(szName);
			e->Delete ();
			return FALSE;
		}
	}

	for(i = 0; i < MAX_STORE; i++)
	{
		szName.Format("droiyan-Online-Store-MemoryDB-%04d", i);
		CSharedMemory* pNew2 = NULL;

		try
		{
			pNew2 = new CSharedMemory;

			if(!pNew2->Open(szName, nStoreSize))
			{
				delete pNew2;
				szName.Format("Open [ %d ] Store Shared Memory Fail!!, GetLastError = %d", i+1, GetLastError()); 
				AfxMessageBox(szName);
				return FALSE;
			}

			g_arStoreSharedMemory.Add(pNew2);
		}
		catch (CMemoryException* e)
		{
			szName.Format("Unable to allocate [ %d ] Store Shared Memory!!, GetLastError = %d", i+1, GetLastError()); 
			AfxMessageBox(szName);
			e->Delete ();
			return FALSE;
		}
	}

	for(i = 0; i < MAX_FORTRESS; i++)
	{
		szName.Format("droiyan-Online-Fortress-MemoryDB-%04d", i);
		CSharedMemory* pNew4 = NULL;

		try
		{
			pNew4 = new CSharedMemory;

			if(!pNew4->Open(szName, nFortressSize))
			{
				delete pNew4;
				szName.Format("Open [ %d ] Fortress Shared Memory Fail!!, GetLastError = %d", i+1, GetLastError()); 
				AfxMessageBox(szName);
				return FALSE;
			}

			g_arFortressSharedMemory.Add(pNew4);
		}
		catch (CMemoryException* e)
		{
			szName.Format("Unable to allocate [ %d ] Fortress Shared Memory!!, GetLastError = %d", i+1, GetLastError()); 
			AfxMessageBox(szName);
			e->Delete ();
			return FALSE;
		}
	}

	for(i = 0; i < MAX_GUILD_HOUSE_NUM - 1; i++)
	{
		szName.Format("droiyan-Online-GuildHouse-MemoryDB-%04d", i);
		CSharedMemory* pNew3 = NULL;

		try
		{
			pNew3 = new CSharedMemory;

			if(!pNew3->Open(szName, nGuildHouseSize))
			{
				delete pNew3;
				szName.Format("Open [ %d ] GuildHouse Shared Memory Fail!!, GetLastError = %d", i+1, GetLastError()); 
				AfxMessageBox(szName);
				return FALSE;
			}

			g_arGuildHouseSharedMemory.Add(pNew3);			
		}
		catch (CMemoryException* e)
		{
			szName.Format("Unable to allocate [ %d ] GuildHouse Shared Memory!!, GetLastError = %d", i+1, GetLastError()); 
			AfxMessageBox(szName);
			e->Delete ();
			return FALSE;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//	DB Agent ¿¡¼­ ¸¸µç RollBack¿ë Shared Memory ¸¦ ¿¬´Ù.
//
BOOL CServerDlg::OpenRollBack()
{
	CString szName = _T("droiyan-Online-RollBack-Memory");

	try
	{
		if(!m_MemRollBack.Open(szName, 10))
		{
			szName.Format("Open RollBack Shared Memory Fail!!, GetLastError = %d", GetLastError()); 
			AfxMessageBox(szName);
			return FALSE;
		}
	}
	catch (CMemoryException* e)
	{
		szName.Format("Unable to allocate RollBack Shared Memory!!, GetLastError = %d", GetLastError()); 
		AfxMessageBox(szName);
		e->Delete ();
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//	¼­¹ö°¡ ºñÁ¤»ó Á¾·á ‰ç¾ú´ÂÁö »ìÆìº¸°í ºñÁ¤»ó Á¾·á ‰ç¾úÀ¸¸é ¸Þ¸ð¸® DBÀÇ ³»¿ëÀ» DB·Î ¿Å±ä´Ù.
//
void CServerDlg::CheckRollBack()
{
	TCHAR* pData = (TCHAR*)m_MemRollBack.m_lpData;

	if(*pData == 0) 
	{
		*pData = 1;
		return;
	}

	int i;
	CMemUser*		pMD = NULL;
	int				nSize = g_arSharedMemory.GetSize();
	
	for(i = 0; i < nSize; i++)
	{
		pMD = (CMemUser*) g_arSharedMemory[i]->m_lpData;
		if(pMD->m_uid == -1) continue;
		if(pMD->m_sSTR == 0) continue;

		UpdateMem2DB(pMD);
		UpdateBankMem2DB(pMD);
		pMD->m_uid = -1;
		::ZeroMemory(pMD->m_strUserID, sizeof(pMD->m_strUserID));
	}

	CMemStore*		pMDStore = NULL;
	int				nStoreSize = g_arStoreSharedMemory.GetSize();
	
	for(i = 0; i < nStoreSize; i++)
	{
		pMDStore = (CMemStore*) g_arStoreSharedMemory[i]->m_lpData;
		if(pMDStore->m_sStoreID == -1) continue;
		if(pMDStore->m_iGuildSid <= 0) continue;

		UpdateStoreMem2DB(pMDStore);
		pMDStore->m_sStoreID = -1;
		pMDStore->m_iGuildSid = 0;
	}

	CMemFortress*	pMDFortress = NULL;
	int				nFortressSize = g_arFortressSharedMemory.GetSize();
	
	for(i = 0; i < nFortressSize; i++)
	{
		pMDFortress = (CMemFortress*) g_arFortressSharedMemory[i]->m_lpData;
		if(pMDFortress->m_sFortressID < 1000) continue;
		if(pMDFortress->m_iGuildSid <= 0) continue;

		UpdateFortressMem2DB(pMDFortress);
		pMDFortress->m_iGuildSid = 0;
	}

	CMemGuildHouseRank*	pMDGuildHouse = NULL;
	int				nGuildHouseSize = g_arGuildHouseSharedMemory.GetSize();
	
	for(i = 0; i < nGuildHouseSize; i++)
	{
		pMDGuildHouse = (CMemGuildHouseRank*) g_arGuildHouseSharedMemory[i]->m_lpData;
		if(pMDGuildHouse->lGuild <= 0) continue;

		UpdateGuildHouseMem2DB(pMDGuildHouse);
		pMDGuildHouse->tSid = i + 1;
		pMDGuildHouse->lGuild = 0;
	}
}

/////////////////////////////////////////////
// ¸ÞÀÏ ¿¡ÀÌÀüÆ®ÀÇ °øÀ¯¸Þ¸ð¸® ¿­±â

BOOL CServerDlg::OpenMailQueue(void)
{
	if( g_MailAgentQueue.Open( "DRSQLAgent_SharedMemory" ) == FALSE )
	{
		AfxMessageBox( "Mail Agent : Shared memory create fail!" );
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////
// ¸ÞÀÏ ¿¡ÀÌÀüÆ®ÀÇ °øÀ¯¸Þ¸ð¸® ´Ý±â

BOOL CServerDlg::CloseMailQueue(void)
{
	g_MailAgentQueue.Release();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	¸Þ¸ð¸® DBÀÇ ³»¿ëÀ» DB·Î ¿Å±ä´Ù.
//
void CServerDlg::UpdateMem2DB(CMemUser *pMD)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strFace[10], strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB], strTel[_TEL_DB];
	TCHAR			strQuickItem[_QUICKITEM_DB];
	TCHAR			strHaveEvent[_EVENT_DB];
	int				i;

//	BYTE			tPsiOneKind = 0, tPsiTwoKind = 0, tPsiThreeKind = 0;
//	DWORD			dwPsiOneTime = 0, dwPsiTwoTime = 0, dwPsiThreeTime = 0;

	::ZeroMemory(szSQL, sizeof(szSQL));

	::ZeroMemory(strFace, sizeof(strFace));
	::ZeroMemory(strSkill, sizeof(strSkill));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strPsi, sizeof(strPsi));
	::ZeroMemory(strTel, sizeof(strTel));
	::ZeroMemory(strHaveEvent, sizeof(strHaveEvent));

	::ZeroMemory(strQuickItem, sizeof(strQuickItem));
	
    ::CopyMemory(strFace, pMD->m_strFace, sizeof(strFace));
	::CopyMemory(strSkill, pMD->m_strSkill, sizeof(strSkill));
	::CopyMemory(strItem, pMD->m_strItem, sizeof(strItem));
	::CopyMemory(strPsi, pMD->m_strPsi, sizeof(strPsi));
	::CopyMemory(strTel, pMD->m_strTel, sizeof(strTel));
	::CopyMemory(strHaveEvent, pMD->m_strHaveEvent, sizeof(strHaveEvent));

	::CopyMemory(strQuickItem, pMD->m_strQuickItem, sizeof(strQuickItem));
	
    SDWORD sFaceLen		= sizeof(strFace);
	SDWORD sSkillLen	= sizeof(strSkill);
	SDWORD sItemLen		= sizeof(strItem);
	SDWORD sPsiLen		= sizeof(strPsi);
	SDWORD sQuickLen	= sizeof(strQuickItem);
	SDWORD sEventLen	= sizeof(strHaveEvent);
	SDWORD sTelLen		= sizeof(strTel);
	 ////////////////////////¼Ó×Ö¶Î

   _sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_DATA (\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,\
		?, %d,%d,%d, %d, %d,%d,  %d,%d,  %d, \
		%d,%d,%d,%d,%d,%d, %d,%d,%d,%d, \
		?,?,?,?, %d,%d,\
		%d, %d, ?, %d, %d,\
		?, %d,\
		%d, %d, %d, %d, %d, %d, %d, %d, %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
		\'%s\', \
		%d,%d,		%d,%d,		%d,%d)}"), 
		pMD->m_strUserID, pMD->m_sSTR, pMD->m_sCON,	pMD->m_sDEX, pMD->m_sVOL, pMD->m_sWIS, pMD->m_iSkin, pMD->m_iHair, pMD->m_sGender,	
		pMD->m_curz, pMD->m_curx, pMD->m_cury, pMD->m_dwBuddy, pMD->m_dwGuild, pMD->m_dwExp, pMD->m_sPA, pMD->m_sSkillPoint, pMD->m_dwXP,
		pMD->m_sMaxHP, pMD->m_sHP, pMD->m_sMaxPP, pMD->m_sPP, pMD->m_sMaxSP, pMD->m_sSP, pMD->m_dwDN, pMD->m_sCityRank, pMD->m_sLevel, pMD->m_byClass,

		pMD->m_tAbnormalKind, pMD->m_dwAbnormalTime,

		pMD->m_bLive, pMD->m_iCityValue, pMD->m_sKillCount, pMD->m_dwSaveTime, //ÐÂ¼Ó×Ö¶Î¿ØÖÆÑªÁ¿ÏÔÊ¾
		pMD->m_dwSaintTime, 
		pMD->m_dwHiExpTime, pMD->m_dwHtExpTime, pMD->m_dwMagicFindTime, pMD->m_dwMagicFtTime, pMD->m_dwNoChatTime,pMD->m_dwZF, pMD->m_dwXL,pMD->m_dwCloseTime, pMD->m_dwAutoMoney,pMD->m_dwPD,/*Ð¡¿É*/pMD->m_dwLingQu,pMD->m_dwShaGuai, pMD->m_dwGuarDianTianShi,pMD->m_dwShopPingDN, pMD->m_dwVIPTime,pMD->m_dwZaiXianTime,pMD->m_dwBFindTime,pMD->m_dwHXTime,pMD->m_dwSGTime,pMD->m_dwXYTime,pMD->m_dwZFTime,
		pMD->m_strLoveName, //yskang 0.1
		pMD->m_tPsiOneKind, pMD->m_dwPsiOneTime,		pMD->m_tPsiTwoKind, pMD->m_dwPsiTwoTime,		pMD->m_tPsiThreeKind, pMD->m_dwPsiThreeTime); 

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strFace),	0, (TCHAR*)strFace,		0, &sFaceLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strSkill),	0, (TCHAR*)strSkill,	0, &sSkillLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strPsi),		0, (TCHAR*)strPsi,		0, &sPsiLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strQuickItem),	0, (TCHAR*)strQuickItem,	0, &sQuickLen );
//		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strAbnormal), 0, (TCHAR*)strAbnormal, 0, &sAbnormalLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strHaveEvent),	0, (TCHAR*)strHaveEvent,	0, &sEventLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strTel),			0, (TCHAR*)strTel,			0, &sTelLen );
        
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
		//if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();
//		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
}

void CServerDlg::OnRefresh() //ÓÃ»§Í³¼Æ
{
	//int nCount = MAX_USER - m_Com.uid_array.GetSize();

	// IKING 2001.1.
	// Á»´õ Á¤±³ÇÏ°Ô °íÃÄ¾ßÇÔ...
	int nCount,i;
	USER *pUser = NULL;
	nCount = 0;
	for ( i = 0; i < MAX_USER; i++ )  //Í³¼ÆÈËÊý
	{
		pUser = m_Com.GetUserUid(i);
		if ( pUser && pUser->m_state == STATE_GAMESTARTED )
			nCount++;
	}
	if(nCount >= 500) nCount = (int)((double)nCount * 1.1 + 0.5);
	
	TCHAR strTitle[256];
	::ZeroMemory(strTitle, sizeof(strTitle));

	int af[AUTOMATA_THREAD+1];

	DWORD curtickcount = GetTickCount();
	for ( i = 0; i < AUTOMATA_THREAD; i++ )
	{
		if ( curtickcount - g_ThreadAliveTime[i] > 30000 )
			af[i] = 0;
		else
			af[i] = 1;
	}
	char tsbuf[100];
/*	if ( AUTOMATA_THREAD == 4 )
		wsprintf(tsbuf," [%d %d %d %d]",af[0],af[1],af[2],af[3]);
	else
		wsprintf(tsbuf," [%d %d %d %d %d %d %d %d]",af[0],af[1],af[2],af[3],af[4],af[5],af[6],af[7]);
*/
	if ( AUTOMATA_THREAD == 4 )
		wsprintf(tsbuf," [%d %d %d %d E %d %d %d %d]",af[0],af[1],af[2],af[3], g_ThreadErr[0], g_ThreadErr[1], g_ThreadErr[2], g_ThreadErr[3]);
	else
		wsprintf(tsbuf," [%d %d %d %d %d %d %d %d E %d %d %d %d %d %d %d %d]",af[0],af[1],af[2],af[3],af[4],af[5],af[6],af[7], g_ThreadErr[0], g_ThreadErr[1], g_ThreadErr[2], g_ThreadErr[3], g_ThreadErr[4], g_ThreadErr[5], g_ThreadErr[6], g_ThreadErr[7]);

	wsprintf(strTitle, _ID( IDS_SERVERDLG_CURRENTUSER ), nCount);

//	wsprintf(strTitle, _T("Total = %d, Error = %d"), g_TotalNPC, g_CurrentNPCError);
	//strcat(strTitle, tsbuf );

	SetWindowText(strTitle);
}

void CServerDlg::OnTimer(UINT nIDEvent) 
{
//	OnRefresh();

	switch( nIDEvent )
	{
	case	10000:
		m_iUserCountCount++;

		if( m_iUserCountCount >= 10 )
		{
			m_iUserCountCount = 0;

			m_Com.UpdateUserCount();
		}
		break;

	case	ALIVE_FILL_TIMER:
		SetAliveSignalFill();
		break;

	case	10001:		// ÀÌº¥Æ® ¾ÆÀÌÅÛ »Ñ¸®±â Ã³¸®
		SetEventItemNew();
		break;

	case	10002:
		if( !m_pBridgeSocket ) { InitBridgeServerConnection(); break; }
		if( m_pBridgeSocket->m_SockFlag != 1 ) { InitBridgeServerConnection(); break; }

		if( (GetTickCount()-m_dwBSAlive) >= 10000 )
		{
			m_editCtrl = "Fail to connect BridgeServer...(TIMER)\r\n";	UpdateData( FALSE );
			m_pBridgeSocket->SocketDisConnect();
			break;
		}
		else
		{
			m_editCtrl.Format( "Connected to BridgeServer...(TIME:%d)\r\n", GetTickCount()-m_dwBSAlive );//	UpdateData( FALSE );
		}
		break;
	default:
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	°¢ ±æµå¿¡¼­ ¼ÒÀ¯ÇÑ »óÁ¡¸¦ ÀÐ¾î¿Â´Ù.
//
BOOL CServerDlg::GetGuildStoreTable()
{
	int iSid = 0;
	CGuildStoreSet	GuildStoreSet;

//	g_arGuildStore.RemoveAll();

	int nSize = g_arStoreSharedMemory.GetSize();
	CSharedMemory* pShared = NULL;
	CMemStore* pData = NULL;

	try
	{
		if(GuildStoreSet.IsOpen()) GuildStoreSet.Close();
		
		if( !GuildStoreSet.Open() )
		{
			AfxMessageBox(_T("GuildStore DB Open Fail!"));
			return FALSE;
		}
/*		if(GuildStoreSet.IsBOF()) 
		{
			AfxMessageBox(_T("Item DB Empty!"));
			return FALSE;
		}
*/		
		while( !GuildStoreSet.IsEOF() )
		{
			iSid = -1;
			iSid = GuildStoreSet.m_sStoreID;

			for(int i =0; i < g_arStore.GetSize(); i++)
			{
				if(iSid == g_arStore[i]->m_sStoreID)
				{
					g_arStore[i]->m_bHaveGuild = TRUE;
					g_arStore[i]->m_sRate = GuildStoreSet.m_sTaxRate;

					g_arStore[i]->m_iGuildSid = GuildStoreSet.m_iGuildSid;
					strcpy(g_arStore[i]->m_strGuildName, GuildStoreSet.m_strGuildName);
					strcpy(g_arStore[i]->m_strMasterName, GuildStoreSet.m_strMasterName);
					g_arStore[i]->m_iGuildDN = GuildStoreSet.m_iGuildDN;
					g_arStore[i]->m_tWarType = GuildStoreSet.m_tWarType;

					g_arStore[i]->m_wLastWarTime.wYear = GuildStoreSet.m_WarTime.GetYear();
					g_arStore[i]->m_wLastWarTime.wMonth = GuildStoreSet.m_WarTime.GetMonth();
					g_arStore[i]->m_wLastWarTime.wDay = GuildStoreSet.m_WarTime.GetDay();
					g_arStore[i]->m_wLastWarTime.wHour = GuildStoreSet.m_WarTime.GetHour();
					g_arStore[i]->m_wLastWarTime.wMinute = 0;
					g_arStore[i]->m_wLastWarTime.wSecond = 0;

					g_arStore[i]->m_wPlanWarTime.wYear = GuildStoreSet.m_WarPlan.GetYear();
					g_arStore[i]->m_wPlanWarTime.wMonth = GuildStoreSet.m_WarPlan.GetMonth();
					g_arStore[i]->m_wPlanWarTime.wDay = GuildStoreSet.m_WarPlan.GetDay();
					g_arStore[i]->m_wPlanWarTime.wHour = GuildStoreSet.m_WarPlan.GetHour();
					g_arStore[i]->m_wPlanWarTime.wMinute = 0;
					g_arStore[i]->m_wPlanWarTime.wSecond = 0;
					pShared = NULL;
					pData = NULL;
					
					if(iSid >= nSize) continue;
					
					pShared = g_arStoreSharedMemory[iSid];
					
					if(pShared == NULL) continue;
					if(pShared->m_hMapping == NULL) continue;
					
					pData = (CMemStore*) pShared->m_lpData;
					
					if(pData == NULL) continue;

					pData->m_sStoreID = iSid;
					pData->m_iGuildSid = GuildStoreSet.m_iGuildSid;
					pData->m_sTaxRate = GuildStoreSet.m_sTaxRate;
					pData->m_dwStoreDN = GuildStoreSet.m_iGuildDN;
					pData->m_tWarType = GuildStoreSet.m_tWarType;

					LoadGuildStoreAttackList(g_arStore[i], pData);
				}
			}

			GuildStoreSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	°¢ ±æµå »óÁ¡ÁÖº¯ÀÇ ¼Ó¼ºÀ» ¾Ë±âÀ§ÇØ 
//
BOOL CServerDlg::GetGuildMapInfoTable()
{
	int iSid = 0;
	CGuildMapInfoSet	GuildMapSet;

	g_arMapTable.RemoveAll();

//	g_arMapTable.SetSize(500);		// ÃÖ´ë 500°³ ¼Ó¼ºÀ» ¹Ì¸® Àâ´Â´Ù.

	try
	{
		if(GuildMapSet.IsOpen()) GuildMapSet.Close();
		
		if( !GuildMapSet.Open() )
		{
			AfxMessageBox(_T("GuildMapInfo DB Open Fail!"));
			return FALSE;
		}
		if(GuildMapSet.IsBOF()) 
		{
			AfxMessageBox(_T("Item DB Empty!"));
			return FALSE;
		}
		
		while( !GuildMapSet.IsEOF() )
		{
			CGuildMapInfoTable *guildMap = new CGuildMapInfoTable;

			iSid = GuildMapSet.m_sMapIndex;
			guildMap->m_sMapIndex = GuildMapSet.m_sMapIndex;
			guildMap->m_tPkMode = GuildMapSet.m_tPkMode;
			guildMap->m_sStoreID = GuildMapSet.m_sReleatedStore;
			guildMap->m_sStoreZone = GuildMapSet.m_sGuildZone;

			guildMap->m_sStoreIndex = -1;

/*			for(i = 0; i < g_arStore.GetSize(); i++)
			{
				if(g_arStore[i]->m_sStoreID == guildMap->m_sStoreID)
				{
					guildMap->m_sStoreIndex = i;
					break;
				}				
			}
*/
//			g_arMapTable[iSid] = guildMap;
			g_arMapTable.Add(guildMap);

			GuildMapSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

void CServerDlg::UpdateStoreMem2DB(CMemStore *pMDStore)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];
	TCHAR			strAttackList[_GUILDLIST_DB];
	int				i;

	if(pMDStore->m_iGuildSid <= 0) return;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strAttackList, sizeof(strAttackList));
	
	::CopyMemory(strAttackList, pMDStore->m_AttackList, sizeof(strAttackList));
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
		//if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

//		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

	pMDStore->m_dwStoreDN = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//	ÇÊµå »óÁ¡ ±æµåÀü¿¡ ½ÅÃ»ÇÑ ±æµå ¸®½ºÆ®
//
BOOL CServerDlg::LoadGuildStoreAttackList(CStore *pStore, CMemStore* pData)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];
	TCHAR			strAttackList[_GUILDLIST_DB];

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strAttackList, sizeof(strAttackList));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_GUILD_STORE_ATTACKLIST(%d)}"), pStore->m_sStoreID);

	SQLINTEGER	sInd;

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild_Store Attack List Data !!\n");

//		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			int i = 1;
			SQLGetData( hstmt, i++, SQL_C_BINARY, strAttackList, sizeof(strAttackList),	&sInd );
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

	pStore->StrToGuildList((LPTSTR)strAttackList);
	strncpy(pData->m_AttackList, strAttackList, _GUILDLIST_DB);

	return TRUE;
}

BOOL CServerDlg::UpdateBankMem2DB(CMemUser *pMD)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strBankItem[_BANK_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strBankItem, sizeof(strBankItem));
	
	::CopyMemory(strBankItem, pMD->m_UB.m_UserBankItem, sizeof(strBankItem));
	
	SDWORD sBankItemLen	= sizeof(strBankItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_BANK_DATA_ONLY (\'%s\',%d, ?)}"), pMD->m_UB.m_strUserID, pMD->m_UB.m_dwBankDN);

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To Update User Bank Data Only!!\n");

//		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strBankItem), 0, (TCHAR*)strBankItem, 0, &sBankItemLen);

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//			BREAKPOINT();

			g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
			return FALSE;
		}
	}	

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

BOOL CServerDlg::UpdateAccountBankMem2DB(CMemUser *pMD)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strBankItem[_ACCOUNT_BANK_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strBankItem, sizeof(strBankItem));
	
	::CopyMemory(strBankItem, pMD->m_AB.m_AccountBankItem, sizeof(strBankItem));
	
	SDWORD sBankItemLen	= sizeof(strBankItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_ACCOUNT_BANK_DATA_ONLY (\'%s\',%d, ?)}")/*, pMD->m_iMyServer*/, pMD->m_AB.m_strAccount, pMD->m_AB.m_dwBankDN);

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		return FALSE;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strBankItem), 0, (TCHAR*)strBankItem, 0, &sBankItemLen);

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

			g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
			return FALSE;
		}
	}	

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	±æµåÇÏ¿ì½º ÀïÅ»¸¦À§ÇØ ´©ÀûÁ¡¼ö¸¦ ±âÁØÀ¸·Î ¼øÀ§¸¦ ¾ò´Â´Ù.
//
BOOL CServerDlg::GetGuildHouseRankTable()
{
	int iSid=  0;
	int nLen = 0;
	int gIndex = 0;
	CGuildHouseRankSet	guildRankSet;

	g_arGuildHouseWar.RemoveAll();

	int nSize = g_arGuildHouseSharedMemory.GetSize();

	CSharedMemory* pShared = NULL;
	CMemGuildHouseRank* pData = NULL;


	try
	{
		if(guildRankSet.IsOpen()) guildRankSet.Close();
		
		if( !guildRankSet.Open() )
		{
			AfxMessageBox(_T("Guild_Rank DB Open Fail!"));
			return FALSE;
		}
		if(guildRankSet.IsBOF()) 
		{
			AfxMessageBox(_T("Guild_Rank DB Empty!"));
			return FALSE;
		}
		
		CGuildHouseWar *guild = new CGuildHouseWar;

		while( !guildRankSet.IsEOF() )
		{			
			iSid = guildRankSet.m_tSid - 1;
			
			guild->m_TopList[iSid].iGuildNum = guildRankSet.m_tNum;
			guild->m_TopList[iSid].iGuildLevel = guildRankSet.m_sGuildLevel;
			guild->m_TopList[iSid].iLastValue = guildRankSet.m_sValue;
			guild->m_TopList[iSid].dwLastTime = guildRankSet.m_iLastTime;
			guild->m_TopList[iSid].lGuild = guildRankSet.m_iGuild;
			 
			::ZeroMemory(guild->m_TopList[iSid].strGuildName, sizeof(guild->m_TopList[iSid].strGuildName));
			
			guild->m_CurrentGuild.iWarZone = guildRankSet.m_sZone;

			gIndex = guildRankSet.m_iGuild;
			if(gIndex > 0 && gIndex < g_arGuildData.GetSize())
			{
				if(g_arGuildData[gIndex])
				{
					nLen = strlen(g_arGuildData[gIndex]->m_strGuildName);
					strncpy(guild->m_TopList[iSid].strGuildName, g_arGuildData[gIndex]->m_strGuildName, nLen);
				}
			}


			pShared = NULL;
			pData = NULL;
			
			iSid = iSid;
			pShared = g_arGuildHouseSharedMemory[iSid];
			
			if(pShared == NULL) continue;
			if(pShared->m_hMapping == NULL) continue;
			
			pData = (CMemGuildHouseRank *) pShared->m_lpData;
			
			if(pData == NULL) continue;

			pData->tSid = (BYTE)iSid + 1;
			pData->lGuild = guildRankSet.m_iGuild;
			pData->iLastValue = guildRankSet.m_sValue;
			pData->iGuildNum = guildRankSet.m_tNum;
			pData->iGuildLevel = guildRankSet.m_sGuildLevel;
			pData->dwLastTime = guildRankSet.m_iLastTime;
			
			guildRankSet.MoveNext();
		}

		guild->InitGuild();											// ±æµå°ü·Ã ¼öÄ¡¸¦ ÃÊ±âÈ­
		g_arGuildHouseWar.Add(guild);
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	°¡»ó °ø°£¿¡¼­ À§Ä¡ÇÒ °¢ ÁÂÇ¥¸¦ ¼ÂÆÃ	
//
BOOL CServerDlg::GetVirtualRoomTable()
{
	CVirtualRoom *VR = NULL;
	VR = new CVirtualRoom;
	
	VR->m_iCity = SANAD;
	VR->m_sZone = 1005;

	VR->m_Move[0].x = 37; VR->m_Move[0].y = 73;
	VR->m_Move[1].x = 39; VR->m_Move[1].y = 71;
	VR->m_Move[2].x = 41; VR->m_Move[2].y = 69;
	VR->m_Move[3].x = 44; VR->m_Move[3].y = 66;
	VR->m_Move[4].x = 45; VR->m_Move[4].y = 63; 
	VR->m_Move[5].x = 47; VR->m_Move[5].y = 61;
	VR->m_Move[6].x = 50; VR->m_Move[6].y = 60;
	VR->m_Move[7].x = 51; VR->m_Move[7].y = 57;
	VR->m_Move[8].x = 51; VR->m_Move[8].y = 55;
	VR->m_Move[9].x = 49; VR->m_Move[9].y = 57;
	VR->m_Move[10].x = 46;VR->m_Move[10].y = 58;
	VR->m_Move[11].x = 44;VR->m_Move[11].y = 66;
	VR->m_Move[12].x = 43;VR->m_Move[12].y = 61;
	VR->m_Move[13].x = 40;VR->m_Move[13].y = 64;
	VR->m_Move[14].x = 39;VR->m_Move[14].y = 67;
	VR->m_Move[15].x = 37;VR->m_Move[15].y = 67;
	VR->m_Move[16].x = 33;VR->m_Move[16].y = 75;
	VR->m_Move[17].x = 32;VR->m_Move[17].y = 74;
	VR->m_Move[18].x = 33;VR->m_Move[18].y = 71;
	VR->m_Move[19].x = 34;VR->m_Move[19].y = 70;

	g_arVirtualRoom.Add(VR);

	return TRUE;
}

void CServerDlg::UpdateGuildHouseMem2DB(CMemGuildHouseRank *pMD)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_HOUSE_RANK(%d, %d, %d, %d, %d, %d)}"), 
//		pMD->tSid + 1, pMD->lGuild, pMD->iGuildNum, pMD->iLastValue, pMD->iGuildLevel, pMD->dwLastTime);
		pMD->tSid, pMD->lGuild, pMD->iGuildNum, pMD->iLastValue, pMD->iGuildLevel, pMD->dwLastTime);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode == SQL_SUCCESS)
	{
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
		//if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

//		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

	pMD->lGuild = 0;
}

BOOL CServerDlg::GetGuildHouseTable()
{
	CGuildHouseSet	guildSet;

	g_arGuildHouse.RemoveAll();							
//	g_arGuildHouse.SetSize(GUILD_HOUSE_MAX_NUM);		

	try
	{
		if(guildSet.IsOpen()) guildSet.Close();
		
		if( !guildSet.Open() )
		{
			AfxMessageBox(_T("Guild_House DB Open Fail!"));
			return FALSE;
		}
		if(guildSet.IsBOF()) 
		{
			AfxMessageBox(_T("Guild_House DB Empty!"));
			return FALSE;
		}
		
		while( !guildSet.IsEOF() )
		{	
			CGuildHouse *pGuild = new CGuildHouse;

			pGuild->iSid = guildSet.m_tSid;
			pGuild->iGuild = guildSet.m_iGuild;
			pGuild->iZone = guildSet.m_sZone;
			pGuild->iPotalX = guildSet.m_sPotalX;
			pGuild->iPotalY = guildSet.m_sPotalY;

			pGuild->iMarkNpc = -1;

			g_arGuildHouse.Add(pGuild);

			guildSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

BOOL CServerDlg::CheckGuildHouseData()
{
/*	int i, j;

	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);

	COleDateTime CurrTime = COleDateTime(guildTime);

	if(CurrTime.GetDay() == GUILD_HOUSE_DAY && CurrTime.GetHour() == 0)
	{
		for(i = 0; i < g_arGuildHouseWar.GetSize(); i++)
		{
			for(j = 0; i < MAX_GUILD_HOUSE_NUM - 1; j++)
			{
				if(g_arGuildHouseWar[i]->m_TopList[j].lGuild != g_arGuildHouse[j]->iGuild)
				{
					g_arGuildHouse[j]->UpdateGuildHouse();
				}
			}
		}
	}
*/
	return TRUE;
}

BOOL CServerDlg::GetUserLevelUpTable()
{
	CUserLevelUpSet	UserLevel;

	g_arUserLevel.RemoveAll();							

	try
	{
		if(UserLevel.IsOpen()) UserLevel.Close();
		
		if( !UserLevel.Open() )
		{
			AfxMessageBox(_T("UserLevelUp DB Open Fail!"));
			return FALSE;
		}
		if(UserLevel.IsBOF()) 
		{
			AfxMessageBox(_T("UserLevleUp DB Empty!"));
			return FALSE;
		}
		
		while( !UserLevel.IsEOF() )
		{	
			CUserLevelUpTable *pLevel = new CUserLevelUpTable;

			pLevel->m_sLevel = UserLevel.m_sLevel;
			pLevel->m_sHP = UserLevel.m_sHP;
			pLevel->m_sPP = UserLevel.m_sPP;
			pLevel->m_sDamage = UserLevel.m_sDamage;
			pLevel->m_sDefense = UserLevel.m_sDefense;
			pLevel->m_sWeight = UserLevel.m_sWeight;

			g_arUserLevel.Add(pLevel);

			UserLevel.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
//
//	¿¬¸» ¿¬½Ã¿ë ÀÌº¥Æ®
//
BOOL CServerDlg::GetEventItemTable()
{
	int nLen = 0;
	CEventItemTableSet	EventItemSet;

	g_arEventItemTable.RemoveAll();							

	try
	{
		if(EventItemSet.IsOpen()) EventItemSet.Close();
		
		if( !EventItemSet.Open() )
		{
			AfxMessageBox(_T("EventItem DB Open Fail!"));
			return FALSE;
		}
		if(EventItemSet.IsBOF()) 
		{
			AfxMessageBox(_T("EventItem DB Empty!"));
			return FALSE;
		}
		
		while( !EventItemSet.IsEOF() )
		{
			nLen = 0;
			nLen = EventItemSet.m_strSerialNum.GetLength();							// ÀÌº¥Æ® »óÇ°±Ç È¸¼ö
			if(nLen <= 0 || EventItemSet.m_tUsed == 1) EventItemSet.MoveNext();
			
			CEventItemTable *pEvent = new CEventItemTable;

			pEvent->m_sSid = EventItemSet.m_sSid;
			pEvent->m_tType = EventItemSet.m_tType;
			pEvent->m_tGiveFlag = EventItemSet.m_tGiveFlag;

			::ZeroMemory(pEvent->m_strSerialNum, sizeof(pEvent->m_strSerialNum)); 
			strncpy(pEvent->m_strSerialNum, EventItemSet.m_strSerialNum.GetBuffer(nLen), nLen);

			pEvent->m_tEnd = EventItemSet.m_tEnd;
			pEvent->m_tGiveItem = 0;
			pEvent->m_lUsed = 0;

			g_arEventItemTable.Add(pEvent);

			EventItemSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
//
//	¼³³¯¿ë ÀÌº¥Æ®
//
BOOL CServerDlg::GetAddEventItemTable()
{
	int nLen = 0;
	CAddEventItemTableSet	EventItemSet;

	g_arAddEventItemTable.RemoveAll();							

	try
	{
		if(EventItemSet.IsOpen()) EventItemSet.Close();
		
		if( !EventItemSet.Open() )
		{
			AfxMessageBox(_T("EventItem DB Open Fail!"));
			return FALSE;
		}
		if(EventItemSet.IsBOF()) 
		{
			AfxMessageBox(_T("EventItem DB Empty!"));
			return FALSE;
		}
		
		while( !EventItemSet.IsEOF() )
		{
			nLen = 0;
			nLen = EventItemSet.m_strSerialNum.GetLength();							// ÀÌº¥Æ® »óÇ°±Ç È¸¼ö
			if(nLen <= 0)
			{
				EventItemSet.MoveNext();
				continue;
			}
			
			CEventItemTable *pEvent = new CEventItemTable;

			pEvent->m_sSid = EventItemSet.m_sSid;
			pEvent->m_tType = EventItemSet.m_tType;
			pEvent->m_tGiveFlag = EventItemSet.m_tGiveFlag;

			::ZeroMemory(pEvent->m_strSerialNum, sizeof(pEvent->m_strSerialNum)); 
			strncpy(pEvent->m_strSerialNum, EventItemSet.m_strSerialNum.GetBuffer(nLen), nLen);

			pEvent->m_tEnd = EventItemSet.m_tEnd;
			pEvent->m_tGiveItem = EventItemSet.m_tUsed;

			if(EventItemSet.m_tUsed == 1) pEvent->m_lUsed = 1;
			else						  pEvent->m_lUsed = 0;

			g_arAddEventItemTable.Add(pEvent);

			EventItemSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

BOOL CServerDlg::GetNpcSayEventTable()
{
	CNpcSayEventSet	SayNpcSet;

	g_arNpcSayEventTable.RemoveAll();							

	try
	{
		if(SayNpcSet.IsOpen()) SayNpcSet.Close();
		
		if( !SayNpcSet.Open() )
		{
			AfxMessageBox(_T("NpcSayEvent DB Open Fail!"));
			return FALSE;
		}
		if(SayNpcSet.IsBOF()) 
		{
			AfxMessageBox(_T("NpcSayEvent DB Empty!"));
			return FALSE;
		}
		
		while( !SayNpcSet.IsEOF() )
		{
			CNpcSayTable *pSay = new CNpcSayTable;

			pSay->iSid = SayNpcSet.m_sSid;
			pSay->Say = SayNpcSet.m_strNpcSay;

			g_arNpcSayEventTable.Add(pSay);

			SayNpcSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

BOOL CServerDlg::GetGuildFortressTable()
{
	int iSid = 0;
	CGuildFortressSet	GuildFortSet;

	int nSize = g_arFortressSharedMemory.GetSize();
	CSharedMemory* pShared = NULL;
	CMemFortress* pData = NULL;

	CGuild *pGuild = NULL;

	try
	{
		if(GuildFortSet.IsOpen()) GuildFortSet.Close();
		
		if( !GuildFortSet.Open() )
		{
			AfxMessageBox(_T("GuildFortress DB Open Fail!"));
			return FALSE;
		}
/*		if(GuildFortSet.IsBOF()) 
		{
			AfxMessageBox(_T("Item DB Empty!"));
			return FALSE;
		}
*/		
		while( !GuildFortSet.IsEOF() )
		{
			CGuildFortress	*pFortress = new CGuildFortress;

			pFortress->m_bHaveGuild = TRUE;
			pFortress->m_sFortressID = GuildFortSet.m_sFortressID;
			pFortress->m_tTaxRate = GuildFortSet.m_sTaxRate;
			pFortress->m_iGuildSid = GuildFortSet.m_iGuildSid;

			strcpy(pFortress->m_strGuildName, GuildFortSet.m_strGuildName);

			pFortress->m_iGuildDN = 0;
			pFortress->m_tWarType = GuildFortSet.m_tWarType;

			pFortress->m_wLastWarTime.wYear = GuildFortSet.m_WarTime.GetYear();
			pFortress->m_wLastWarTime.wMonth = GuildFortSet.m_WarTime.GetMonth();
			pFortress->m_wLastWarTime.wDay = GuildFortSet.m_WarTime.GetDay();
			pFortress->m_wLastWarTime.wHour = GuildFortSet.m_WarTime.GetHour();
			pFortress->m_wLastWarTime.wMinute = 0;
			pFortress->m_wLastWarTime.wSecond =0;

			pFortress->m_wPlanWarTime.wYear = GuildFortSet.m_WarPlan.GetYear();
			pFortress->m_wPlanWarTime.wMonth = GuildFortSet.m_WarPlan.GetMonth();
			pFortress->m_wPlanWarTime.wDay = GuildFortSet.m_WarPlan.GetDay();
			pFortress->m_wPlanWarTime.wHour = GuildFortSet.m_WarPlan.GetHour();
			pFortress->m_wPlanWarTime.wMinute = 0;
			pFortress->m_wPlanWarTime.wSecond =0;

			pFortress->m_wMopPartyTime.wYear = GuildFortSet.m_MopPartyTime.GetYear();
			pFortress->m_wMopPartyTime.wMonth = GuildFortSet.m_MopPartyTime.GetMonth();
			pFortress->m_wMopPartyTime.wDay = GuildFortSet.m_MopPartyTime.GetDay();
			pFortress->m_wMopPartyTime.wHour = GuildFortSet.m_MopPartyTime.GetHour();
			pFortress->m_wMopPartyTime.wMinute = 0;
			pFortress->m_wMopPartyTime.wSecond =0;

			pFortress->m_iZone = GuildFortSet.m_sZone;
			pFortress->m_iPotalX = GuildFortSet.m_sPotalX;
			pFortress->m_iPotalY = GuildFortSet.m_sPotalY;

			if(pFortress->m_iGuildSid > 0 && pFortress->m_iGuildSid != 900000)
			{
				pGuild = NULL;
				pGuild = g_arGuildData[pFortress->m_iGuildSid];
				if(pGuild)
				{
					if(strcmp(g_arGuildData[pFortress->m_iGuildSid]->m_strGuildName, pFortress->m_strGuildName) == 0)
					{
						strcpy(pFortress->m_strMasterName, g_arGuildData[pFortress->m_iGuildSid]->m_strMasterName);

						LoadGuildFortressUserData(pFortress); 
					}
				}
			}

			pFortress->StrFortressRepairList(GuildFortSet.m_strRepairList);

			pShared = NULL;
			pData = NULL;
			
			pShared = g_arFortressSharedMemory[iSid];
			
			if(pShared == NULL) continue;
			if(pShared->m_hMapping == NULL) continue;
			
			pData = (CMemFortress*) pShared->m_lpData;
			
			if(pData == NULL) continue;

			pData->m_sFortressID = GuildFortSet.m_sFortressID;
			pData->m_iGuildSid = GuildFortSet.m_iGuildSid;
			pData->m_sTaxRate = GuildFortSet.m_sTaxRate;
			pData->m_dwStoreDN = 0;
			pData->m_tWarType = GuildFortSet.m_tWarType;

			LoadGuildFortressList(pFortress);
			g_arGuildFortress.Add(pFortress);

			iSid++;
			GuildFortSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

BOOL CServerDlg::LoadGuildFortressList(CGuildFortress *pFort)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	::ZeroMemory(szSQL, sizeof(szSQL));

	SQLCHAR		strGuildName[CHAR_NAME_LENGTH + 1];	memset( strGuildName, NULL, sizeof(strGuildName));
	
	SQLSMALLINT	sSid;
	SQLINTEGER	sGuildID;
	SQLINTEGER	sInd;

	int i;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_FORTRESS_WARLIST(%d)}"), pFort->m_sFortressID);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild_Store Attack List Data !!\n");

//		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				i = 1;
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sSid,		sizeof(SQLSMALLINT),	&sInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG,  &sGuildID,	sizeof(SQLINTEGER),		&sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  strGuildName,	sizeof(strGuildName),	&sInd );

				pFort->StrToGuildAttList((LPTSTR)strGuildName, sGuildID);

				sGuildID = 0;
				memset( strGuildName, NULL, sizeof(strGuildName));
			}
			else break;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;
}


void CServerDlg::UpdateFortressMem2DB(CMemFortress *pMDFortress)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];
	TCHAR			strAttackList[_GUILDLIST_DB], strRepairList[_GUILDREPAIRLIST_DB];
	int				i;

	if(pMDFortress->m_iGuildSid <= 0) return;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strAttackList, sizeof(strAttackList));
	::ZeroMemory(strRepairList, sizeof(strRepairList));
	
	::CopyMemory(strAttackList, pMDFortress->m_AttackList, sizeof(strAttackList));
	::CopyMemory(strRepairList, pMDFortress->m_RepairList, sizeof(strRepairList));

	SDWORD sLen	= sizeof(strAttackList);
	SDWORD sRepairLen = sizeof(strRepairList);

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_MEM_FORTRESS(%d,%d,%d,%d,%d,?,?)}"), 
		pMDFortress->m_sFortressID, pMDFortress->m_iGuildSid, pMDFortress->m_sTaxRate, 
		pMDFortress->m_dwStoreDN, pMDFortress->m_tWarType);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strAttackList), 0, (TCHAR*)strAttackList, 0, &sLen);
		SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strRepairList),0, (TCHAR*)strRepairList,	0, &sRepairLen );

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			bQuerySuccess = FALSE;
			if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		}
	}
	else
	{
		//if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

//		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

	pMDFortress->m_dwStoreDN = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	Æ÷Æ®¸®½º ÀÌ±ÇÀ» ÁÖ±âÀ§ÇØ »óÁ¡ ¸®½ºÆ®¸¦ °®´Â´Ù.
//
BOOL CServerDlg::GetGuildFortressTaxTable()
{
	int i, iSid = 0;

	CGuildFortressTaxSet	FortTaxSet;

	try
	{
		if(FortTaxSet.IsOpen()) FortTaxSet.Close();
		
		if( !FortTaxSet.Open() )
		{
			AfxMessageBox(_T("GuildFortressTax DB Open Fail!"));
			return FALSE;
		}
		if(FortTaxSet.IsBOF()) 
		{
			AfxMessageBox(_T("Item DB Empty!"));
			return FALSE;
		}
		
		while( !FortTaxSet.IsEOF() )
		{
			for(i = 0; i < g_arGuildFortress.GetSize(); i++)
			{
				if(g_arGuildFortress[i]->m_sFortressID == FortTaxSet.m_sFortressID)
				{
					g_arGuildFortress[i]->arStoreTax.Add(FortTaxSet.m_sStoreID01);
					g_arGuildFortress[i]->arStoreTax.Add(FortTaxSet.m_sStoreID02);
					g_arGuildFortress[i]->arStoreTax.Add(FortTaxSet.m_sStoreID03);
					g_arGuildFortress[i]->arStoreTax.Add(FortTaxSet.m_sStoreID04);
					g_arGuildFortress[i]->arStoreTax.Add(FortTaxSet.m_sStoreID05);
					g_arGuildFortress[i]->arStoreTax.Add(FortTaxSet.m_sStoreID06);
					g_arGuildFortress[i]->arStoreTax.Add(FortTaxSet.m_sStoreID07);
					g_arGuildFortress[i]->arStoreTax.Add(FortTaxSet.m_sStoreID08);
					g_arGuildFortress[i]->arStoreTax.Add(FortTaxSet.m_sStoreID09);
					g_arGuildFortress[i]->arStoreTax.Add(FortTaxSet.m_sStoreID10);
				
					break;
				}
			}

			FortTaxSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	SetStoreTaxForGuildFortress();

	return TRUE;	
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	Æ÷Æ®¸®½º ¼¼±ÝÀ» »óÁ¡¿¡ ¼ÂÆÃÇÑ´Ù. 
//
void CServerDlg::SetStoreTaxForGuildFortress()
{
	for(int i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		g_arGuildFortress[i]->ApplyTaxToStore();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	ÀÌº¥Æ® ¸÷ÀÌ³ª °Å´ë ¸÷À»À§ÇØ ¸Ê¿¡ ¼ÂÆÃÇÒ ÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
//
void CServerDlg::LoadDynamicMap()
{
	
}

BOOL CServerDlg::GetTownPotalTable()
{
	CTownPotalTableSet	TownPotalSet;

	try
	{
		if(TownPotalSet.IsOpen()) TownPotalSet.Close();
		
		if( !TownPotalSet.Open() )
		{
			AfxMessageBox(_T("Town Potal DB Open Fail!"));
			return FALSE;
		}
		if(TownPotalSet.IsBOF()) 
		{
			AfxMessageBox(_T("Town Potal DB Empty!"));
			return FALSE;
		}
		
		while( !TownPotalSet.IsEOF() )
		{
			TOWNPOTAL *pPotal = new TOWNPOTAL;

			pPotal->iPotalZone = TownPotalSet.m_sPotalZone;
			pPotal->iPotal[0].x = TownPotalSet.m_sPotalX1;
			pPotal->iPotal[0].y = TownPotalSet.m_sPotalY1;
			pPotal->iPotal[1].x = TownPotalSet.m_sPotalX2;
			pPotal->iPotal[1].y = TownPotalSet.m_sPotalY2;
			pPotal->iPotal[2].x = TownPotalSet.m_sPotalX3;
			pPotal->iPotal[2].y = TownPotalSet.m_sPotalY3;

			g_RandomTownPotal.Add(pPotal);

			TownPotalSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}


BOOL CServerDlg::GetMonsterPsiTable()
{
	CMonsterPsiSet	MonsterPsiSet;

	try
	{
		if(MonsterPsiSet.IsOpen()) MonsterPsiSet.Close();
		
		if( !MonsterPsiSet.Open() )
		{
			AfxMessageBox(_T("Monster Psionic DB Open Fail!"));
			return FALSE;
		}
		if(MonsterPsiSet.IsBOF()) 
		{
			AfxMessageBox(_T("Monster Psionic DB Empty!"));
			return FALSE;
		}
		
		while( !MonsterPsiSet.IsEOF() )
		{
			CMonsterPsi* pNewPsionic = new CMonsterPsi;
			
			pNewPsionic->m_sSid			= MonsterPsiSet.m_sSid;
			pNewPsionic->m_sPid			= MonsterPsiSet.m_sPid;
			pNewPsionic->m_sCasting		= MonsterPsiSet.m_sCasting;
			pNewPsionic->m_sMinDmg		= MonsterPsiSet.m_sMinDmg;
			pNewPsionic->m_sMaxDmg		= MonsterPsiSet.m_sMaxDmg;
			pNewPsionic->m_byRange		= MonsterPsiSet.m_byRange;

			g_arMonsterPsi.Add( pNewPsionic );
			
			MonsterPsiSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

BOOL CServerDlg::LoadGuildFortressUserData(CGuildFortress *pFort)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	::ZeroMemory(szSQL, sizeof(szSQL));

	SQLCHAR		strUserName[CHAR_NAME_LENGTH + 1];	memset(strUserName, NULL, sizeof(strUserName));
	
	SQLSMALLINT	sCityRank;
	SQLINTEGER	sInd;

	int i;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_FORTRESS_USER_DATA(%d)}"), pFort->m_iGuildSid);

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild_Store Attack List Data !!\n");

//		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				i = 1;
				SQLGetData( hstmt, i++, SQL_C_CHAR,	  strUserName,	sizeof(strUserName),	&sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT,  &sCityRank,	sizeof(SQLSMALLINT),	&sInd );

				pFort->AddGuildUserForFortressCityRank((LPTSTR)strUserName, sCityRank);

				sCityRank = 0;
				memset(strUserName, NULL, sizeof(strUserName));
			}
			else break;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;
}

void CServerDlg::CloseGuildWhareHouse()
{
	int iLen = 0;
	int iGuildID = 0;
	USER* pUser = NULL;
	CGuild *pGuild = NULL;

	for(int i = 0; i < MAX_USER; i++)
	{
		iGuildID = 0;
		pUser = m_Com.GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		pUser->UpdateUserData();

		iGuildID = pUser->m_dwGuild;
		if(iGuildID > 0 && iGuildID < g_arGuildData.GetSize())
		{
			iLen = strlen(g_arGuildData[iGuildID]->m_strUsedUser);

			if(iLen > 0 && iLen < CHAR_NAME_LENGTH)
			{
				if(strcmp(pUser->m_strUserID, g_arGuildData[iGuildID]->m_strUsedUser) == 0)
				{
					pUser->GuildWharehouseCloseReq();	// ±æµå Ã¢°í ±ÇÇÑÀ» ÃÊ±âÈ­ÇÑ´Ù.
					InterlockedExchange(&g_arGuildData[iGuildID]->m_lUsed, (LONG)1);
					strcpy(g_arGuildData[iGuildID]->m_strUsedUser, _ID(IDS_SERVERDLG_OPERATOR ) );
					pUser->UpdateUserItemDN();
//					pUser->UpdateGuildWarehouse();					
				}
			}
		}

//		pUser->SockCloseProcess();
	}
	//´¦ÀíËùÓÐÏß³ÌÍË³ö
	Sleep(2000);
}

void CServerDlg::OnCancel()
{
	USER* pUser = NULL; //·þÎñÆ÷¹Ø±ÕÊ±TËùÓÐÈË
	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = m_Com.GetUserUid(i);		
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		pUser->UpdateUserData();
		pUser->m_bSessionOnline = false;
		pUser->SoftClose();
			
	}

	g_bShutDown = 1;
    _exit(0);//ÐÂÔö¼Ó´¦ÀíTÈË
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	PostMessage(WM_CLOSE_PROCESS);
}

BOOL CServerDlg::GetDressingTable()
{
	CDressingSet	DSSet;

	try
	{
		if(DSSet.IsOpen()) DSSet.Close();
		
		if( !DSSet.Open() )
		{
			AfxMessageBox(_T("Dressing DB Open Fail!"));
			return FALSE;
		}
		if(DSSet.IsBOF()) 
		{
			AfxMessageBox(_T("Dressing DB Empty!"));
			return FALSE;
		}
		
		while( !DSSet.IsEOF() )
		{
			DRESSING_DATA* pNewData = new DRESSING_DATA;

			pNewData->sSid				= DSSet.m_sSid;
			pNewData->sItemSid			= DSSet.m_sItemNum;
			pNewData->sCountMin			= DSSet.m_sCountMin;
			pNewData->sCountMax			= DSSet.m_sCountMax;
			pNewData->sItem[0]			= DSSet.m_sItem01;
			pNewData->sItemRatio[0]		= DSSet.m_sItemRatio01;
			pNewData->sItem[1]			= DSSet.m_sItem02;
			pNewData->sItemRatio[1]		= DSSet.m_sItemRatio02;
			pNewData->sItem[2]			= DSSet.m_sItem03;
			pNewData->sItemRatio[2]		= DSSet.m_sItemRatio03;
			pNewData->sItem[3]			= DSSet.m_sItem04;
			pNewData->sItemRatio[3]		= DSSet.m_sItemRatio04;
			pNewData->sItem[4]			= DSSet.m_sItem05;
			pNewData->sItemRatio[4]		= DSSet.m_sItemRatio05;
			pNewData->sItem[5]			= DSSet.m_sItem06;
			pNewData->sItemRatio[5]		= DSSet.m_sItemRatio06;
			pNewData->sItem[6]			= DSSet.m_sItem07;
			pNewData->sItemRatio[6]		= DSSet.m_sItemRatio07;
			pNewData->sItem[7]			= DSSet.m_sItem08;
			pNewData->sItemRatio[7]		= DSSet.m_sItemRatio08;
			pNewData->sItem[8]			= DSSet.m_sItem09;
			pNewData->sItemRatio[8]		= DSSet.m_sItemRatio09;
			pNewData->sItem[9]			= DSSet.m_sItem10;
			pNewData->sItemRatio[9]		= DSSet.m_sItemRatio10;

			g_arDressingData.Add( pNewData );
			
			DSSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

BOOL CServerDlg::GetLocalIPAddress()
{
	BOOL bFind = FALSE;
	CString strLocalIP;

	char szHostName[128];

	::ZeroMemory(g_arServerIPAddr, sizeof(g_arServerIPAddr));

	if( gethostname(szHostName, 128) == 0 )
	{
		// Get host adresses
		struct hostent * pHost;
		int i;
 
		pHost = gethostbyname(szHostName);
 
		for( i = 0; pHost!= NULL && pHost->h_addr_list[i]!= NULL; i++ )
 		{
 			int j;
 
			strLocalIP = _T("");

 			for( j = 0; j < pHost->h_length; j++ )
 			{
				 CString addr;
 
 				if( j > 0 )
					strLocalIP += ".";
 
 				addr.Format("%u", (unsigned int)((unsigned
		 		char*)pHost->h_addr_list[i])[j]);
				strLocalIP += addr;
 			}  

			/*if(strLocalIP.Left(9).CompareNoCase(_T("218.50.5.")) == 0 || strLocalIP.Left(12).CompareNoCase(_T("192.203.141.")) == 0 ||
			   strLocalIP.Left(11).CompareNoCase(_T("210.92.127.")) == 0 || strLocalIP.Left(9).CompareNoCase(_T("172.20.0."))==0)
			{*/
				strcpy(g_arServerIPAddr, strLocalIP);
				bFind = TRUE;
				break;
			//}
		}
	}

	return bFind;
}

BOOL CServerDlg::RemoveSessionCurrentUser()
{
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	SQLINTEGER		iRetInd = SQL_NTS;

	SQLSMALLINT		sRet = 0;

	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call DEL_CURRENTUSER (\'%s\',?)}"), g_arServerIPAddr);
	
	hstmt = NULL;

	int db_index = -1;
	CDatabase* pDB = g_DBSession[1].GetDB( db_index );
	if( !pDB ) return FALSE;
	
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
		return FALSE;
	}
	
	retcode = SQLBindParameter( hstmt, 1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT,0,0, &sRet,0, &iRetInd);
	if( retcode != SQL_SUCCESS )
	{
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		return FALSE;
	}

	retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
	if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		retcode = SQLFetch( hstmt );
		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{	
		}
	}
	else if (retcode==SQL_ERROR)
	{
//		DisplayErrorMsg(hstmt);
	}
	else if (retcode==SQL_NO_DATA)
	{
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

	if(sRet = 0) return FALSE;

	return TRUE;
}

BOOL CServerDlg::GetEventItemNewTable()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	int i = 0;

	SQLINTEGER	sInd;
	SQLSMALLINT sIndex, sSid, sRemain, sNextYear, sNextMonth, sNextDay, sNextHour, sNextMin;
	SQLCHAR		tQuality, tSerialExist;
	SQLINTEGER	iThrowTerm, iThrowRandom;

	::ZeroMemory(szSQL, sizeof(szSQL));

	sIndex = sSid = -1;
	sRemain = sNextYear = sNextMonth = sNextDay = sNextHour = sNextMin = 0;
	tQuality = tSerialExist = 0;
	iThrowTerm = iThrowRandom = 0;
	sInd = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("select * from event_item_new"));

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild_Store Attack List Data !!\n");

		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );
			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				i = 1;

				CEventItemNew* pNewData = new CEventItemNew;

				SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sIndex,		sizeof(SQLSMALLINT),	&sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sSid,			sizeof(SQLSMALLINT),	&sInd );
				SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tQuality,		sizeof(tQuality),		&sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sRemain,		sizeof(SQLSMALLINT),	&sInd );

				SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tSerialExist,	sizeof(tSerialExist),	&sInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iThrowTerm,	sizeof(iThrowTerm),		&sInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iThrowRandom,	sizeof(iThrowRandom),	&sInd );

				SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sNextYear,		sizeof(SQLSMALLINT),	&sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sNextMonth,	sizeof(SQLSMALLINT),	&sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sNextDay,		sizeof(SQLSMALLINT),	&sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sNextHour,		sizeof(SQLSMALLINT),	&sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sNextMin,		sizeof(SQLSMALLINT),	&sInd );

				pNewData->m_sIndex			= sIndex;
				pNewData->m_sSid			= sSid;
				pNewData->m_sRemain			= sRemain;
				pNewData->m_sNextYear		= sNextYear;
				pNewData->m_sNextMonth		= sNextMonth;
				pNewData->m_sNextDay		= sNextDay;
				pNewData->m_sNextHour		= sNextHour;
				pNewData->m_sNextMin		= sNextMin;

				pNewData->m_tQuality		= tQuality;
				pNewData->m_tSerialExist	= tSerialExist;

				pNewData->m_iThrowTerm		= iThrowTerm;
				pNewData->m_iThrowRandom	= iThrowRandom;

				g_arEventItemNew.Add( pNewData );
			}
			else break;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;
}

void CServerDlg::SetEventItemNew()
{
	// ÀÌ ÇÔ¼ö´Â 1ºÐ¸¶´Ù ºÒ¸®´Â ÇÔ¼öÀÌ´Ù

	int i;
	int iRandomPlusMinus = 0;
	int iResultRandom = 0;
	CEventItemNew* pNewItem = NULL;
	CTime time;
	time = CTime::GetCurrentTime();

	for( i = 0; i < g_arEventItemNew.GetSize(); i++ )
	{
		if( g_arEventItemNew[i] )
		{
			pNewItem = g_arEventItemNew[i];

			if( !pNewItem ) continue;

			// ³²Àº ¼ö·®ÀÌ ¾ø´Ù¸é ´Ù »Ñ¸°°ÍÀÌ¹Ç·Î ±×³É Áö³ª°£´Ù.
			if( !pNewItem->m_sRemain ) continue;

			// »Ñ¸®·Á°í ÇÃ·¡±×°¡ ¼ÂÆÃµÇ¾î ÀÖÀ¸¸é ±×³É Áö³ª°£´Ù. »Ñ·ÁÁú¶§±îÁö ±â´Ù¸°´Ù.
			if( pNewItem->m_lGive ) continue;

			// Sid°¡ 0ÀÌÇÏ°Å³ª ¾ÆÀÌÅÛ Å×ÀÌºíÀÇ SID ¹üÀ§¸¦ ³Ñ¾î°£´Ù¸é ±×³É Áö³ª°£´Ù.
			if( pNewItem->m_sSid < 0 || pNewItem->m_sSid >= g_arItemTable.GetSize() ) continue;

			// Ã³À½ »Ñ¸®´Â °Å¶ó¸é
			if( pNewItem->m_sNextYear == 0 )
			{
				iRandomPlusMinus = myrand( 1, 100 );

				if( iRandomPlusMinus <= 50 )	// »©´Â°Å´Ù.
				{
					iResultRandom = pNewItem->m_iThrowTerm + ( -1 * myrand( 1, pNewItem->m_iThrowRandom ) );
				}
				else
				{
					iResultRandom = pNewItem->m_iThrowTerm + myrand( 1, pNewItem->m_iThrowRandom );
				}

				CTimeSpan timeadd( iResultRandom );
				CTime resulttime = time + timeadd;

				pNewItem->m_sNextYear = time.GetYear();
				pNewItem->m_sNextMonth = resulttime.GetMonth();
				pNewItem->m_sNextDay = resulttime.GetDay();
				pNewItem->m_sNextHour = resulttime.GetHour();
				pNewItem->m_sNextMin = resulttime.GetMinute();

				// DB ¾÷µ¥ÀÌÆ® ÇØÁà¾ß ÇÑ´Ù.
				if( !UpdateEventItemNew( pNewItem ) )
				{
					pNewItem->m_sNextYear = 0;
					pNewItem->m_sNextMonth = 0;
					pNewItem->m_sNextDay = 0;
					pNewItem->m_sNextHour = 0;
					pNewItem->m_sNextMin = 0;
				}

				continue;
			}

			// ÀúÀåµÈ ½Ã°£Àº CTimeÀ¸·Î ±¸¼ºÇÑ´Ù
			CTime savedtime( 
				pNewItem->m_sNextYear, 
				pNewItem->m_sNextMonth, 
				pNewItem->m_sNextDay, 
				pNewItem->m_sNextHour,
				pNewItem->m_sNextMin,
				0 );

			// ÀúÀåµÈ ½Ã°£ÀÌ ÇöÀç ½Ã°£º¸´Ù ÀÌÀüÀÌ¶ó¸é
			if( time >= savedtime )
			{
				// ´ÙÀ½ »Ñ¸± ½Ã°£À» ¼ÂÆÃÇÑ´Ù
				iRandomPlusMinus = myrand( 1, 100 );

				if( iRandomPlusMinus <= 50 )	// »©´Â°Å´Ù.
				{
					iResultRandom = pNewItem->m_iThrowTerm + ( -1 * myrand( 1, pNewItem->m_iThrowRandom ) );
				}
				else
				{
					iResultRandom = pNewItem->m_iThrowTerm + myrand( 1, pNewItem->m_iThrowRandom );
				}

				CTimeSpan timeadd( iResultRandom );
				CTime resulttime = time + timeadd;

				pNewItem->m_sNextYear = time.GetYear();
				pNewItem->m_sNextMonth = resulttime.GetMonth();
				pNewItem->m_sNextDay = resulttime.GetDay();
				pNewItem->m_sNextHour = resulttime.GetHour();
				pNewItem->m_sNextMin = resulttime.GetMinute();

				// DB ¾÷µ¥ÀÌÆ® ÇØÁà¾ß ÇÑ´Ù.
				if( !UpdateEventItemNew( pNewItem ) )
				{
					pNewItem->m_sNextYear = 0;
					pNewItem->m_sNextMonth = 0;
					pNewItem->m_sNextDay = 0;
					pNewItem->m_sNextHour = 0;
					pNewItem->m_sNextMin = 0;

					continue;
				}

				// »Ñ¸°´Ù
				::InterlockedExchange( &(pNewItem->m_lGive), 1 );
				// RemainÀº ¾ÆÀÌÅÛÀ» »Ñ¸®´Â NPCÂÊ¿¡¼­ °¨¼Ò½ÃÅ²´Ù.
				// °¨¼Ò ½ÃÅ²ÈÄ m_lGive ÇÃ·¡±×¸¦ ¸®¼ÂÇÑ´Ù.

				continue;
			}
		}
	}
}

BOOL CServerDlg::UpdateEventItemNew(CEventItemNew *pEventItem)
{
	SQLSMALLINT	sRet = -1;
	SQLINTEGER	iRetInd = SQL_NTS;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call update_event_item_new ( %d, %d, %d, %d, %d, %d, %d, ? )}"), 
		pEventItem->m_sIndex, 
		pEventItem->m_sRemain, 
		pEventItem->m_sNextYear, 
		pEventItem->m_sNextMonth, 
		pEventItem->m_sNextDay, 
		pEventItem->m_sNextHour,
		pEventItem->m_sNextMin );

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	int i = 1;
	SQLBindParameter( hstmt, i++ ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &iRetInd);

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

	if( sRet == -1 ) return FALSE;

	return TRUE;
}

BOOL CServerDlg::InitBridgeServerConnection()
{
//	m_pBridgeSocket = NULL;

	CSSocket* pSocket = NULL;

	pSocket = GetServerSocket( &m_ServerBase );

	if( !pSocket )
	{
		return FALSE;
	}

	pSocket->m_ReConnectFlag = TRUE;

	if ( !pSocket->Create() )
	{
		pSocket->SoftClose();
		m_editCtrl = "Fail to connect BridgeServer...\r\n";	UpdateData( FALSE );
		return FALSE; 
	}

	if ( !pSocket->Connect( m_strBridgeIP, m_nBridgePort ) )
	{
		pSocket->SoftClose();
		m_editCtrl = "Fail to connect BridgeServer...\r\n";	UpdateData( FALSE );
		return FALSE;
	}

	pSocket->Init();
	pSocket->m_ReConnectFlag = FALSE;
	pSocket->m_Type = SOCKET_FOR_SERVER;

	TCHAR TempBuf[SEND_BUF_SIZE];
	int index = 0;
	SetByte(TempBuf, SERVER_LOGIN_REQ, index );
	SetByte(TempBuf, g_byMyServerIndex, index );
	pSocket->Send(TempBuf, index);

	m_dwBSAlive = GetTickCount();

	m_editCtrl = "Connected to BridgeServer\r\n";	UpdateData( FALSE );

	m_pBridgeSocket = pSocket;

	return TRUE;
}

void CServerDlg::BridgeServerPhasor(CSSocket *pServerSock, char *pTBuf, int length)
{
	char pBuf[RECEIVE_BUF_SIZE+1];

	if( length > sizeof(pBuf) ) return;

	memcpy( pBuf, pTBuf, length );

	int index = 0;
	BYTE byte = pBuf[index++];

	switch ( byte )
	{
	case SERVER_LOGIN_RESULT:
		BridgeServerLoginResult( pBuf+index );
		break;

	case SERVER_USER_LOGIN_RESULT:
		BridgeServerUserLoginResult( pBuf+index );
		break;

	case SERVER_USER_RESTART:
		BridgeServerUserRestartResult( pBuf+index );
		break;

	case SERVER_USER_ZONELOGIN_RESULT:
		BridgeServerUserZoneLoginResult( pBuf+index );
		break;

	case SERVER_USER_ZONE_LOGOUT:
		BridgeServerUserZoneLogOutResult( pBuf+index );
		break;

	case SERVER_DOUBLE_ACCOUNT_CLOSE:
		BridgeServerDoubleAccountClose( pBuf+index );
		break;

	case SERVER_DOUBLE_ID_CLOSE:
		BridgeServerDoubleIDClose( pBuf+index );
		break;

	case SERVER_USER_WHISPER_REQ:					// ´Ù¸¥ ¼­¹ö·Î ºÎÅÍ Àü´ÞµÈ ±Ó¼Ó¸»À» Ã³¸®ÇÑ´Ù.
		BridgeServerUserWhisperReq( pBuf+index );
		break;

	case SERVER_USER_WHISPER_RESULT:
		BridgeServerUserWhisperResult( pBuf+index );
		break;

	case SERVER_USER_GUILD_CHAT:
		BridgeServerUserGuildChatResult( pBuf+index );
		break;

	case SERVER_GUILD_NEW:
		BridgeServerGuildNewResult( pBuf+index );
		break;

	case SERVER_GUILD_DISPERSE:
		BridgeServerGuildDisperseResult( pBuf+index );
		break;

	case SERVER_GUILD_INVITE:
		BridgeServerGuildInviteResult( pBuf+index );
		break;

	case SERVER_GUILD_OFF:
		BridgeServerGuildOffResult( pBuf+index );
		break;

	case SERVER_GUILD_MARK:
		BridgeServerGuildMarkResult( pBuf+index );
		break;

	case SERVER_ALIVE:
		m_dwBSAlive = GetTickCount();
		break;

	case SERVER_ANNOUNCE:
		BridgeServerAnnounce( pBuf+index );
		break;

	case SERVER_ARK_WINNER:
		BridgeServerArkWinnerChangeResult( pBuf+index );
		break;

	case SERVER_GUILD_SUBMASTER_ON:
		BridgeDeputeGuildMasterOfPowerResult( pBuf+index );
		break;

	case SERVER_GUILD_SUBMASTER_OFF:
		BridgeRemoveGuildMasterOfPowerResult( pBuf+index );
		break;
	};	
}

void CServerDlg::BridgeServerLoginResult(TCHAR *pBuf)
{
	int index = 0;

	BYTE result = GetByte( pBuf, index );

	if( result == FAIL )
	{
		AfxMessageBox( "Can't connect bridgeserver" );
		return;
	}

	int sid = GetShort( pBuf, index );

//	m_pBridgeSocket->m_Sid = sid;

	PostMessage(UM_START_SERVER,0,0);
}

void CServerDlg::BridgeServerWhisperChatReq(int uid, char *pSendID, char *pRecvID, char *strMsg)
{
	int index = 0;
	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_USER_WHISPER_REQ, index );
	SetInt( TempBuf, uid, index );
	SetByte( TempBuf, strlen( pSendID ), index );
	SetString( TempBuf, pSendID, strlen( pSendID ), index );
	SetByte( TempBuf, strlen( pRecvID ), index );
	SetString( TempBuf, pRecvID, strlen( pRecvID ), index );
	SetByte( TempBuf, strlen( strMsg ), index );
	SetString( TempBuf, strMsg, strlen( strMsg ), index );

	m_pBridgeSocket->Send( TempBuf, index );
}

void CServerDlg::BridgeServerUserLogIn(int uid, char *strAccount, char *strUserID)
{
	int index = 0;
	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_USER_LOGIN_REQ, index );
	SetInt( TempBuf, uid, index );
	SetByte( TempBuf, g_byMyServerIndex, index );
	SetByte( TempBuf, (BYTE)strlen( strAccount ), index );
	SetString( TempBuf, strAccount, strlen( strAccount ), index );
	SetByte( TempBuf, (BYTE)strlen( strUserID ), index );
	SetString( TempBuf, strUserID, strlen( strUserID ), index );

	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerUserLoginResult(TCHAR *pBuf)
{
	int index = 0;
	CBufferEx	TempBuf;
	char strAccount[ACCOUNT_LENGTH+1];
	char strUserID[CHAR_NAME_LENGTH+1];
	int nAccount, nUserID;

	int uid = GetInt( pBuf, index );

	if( uid < 0 || uid > MAX_USER ) return;

	USER* pUser = m_Com.GetUserUid( uid );

	if( !pUser ) return;

	BYTE result = GetByte( pBuf, index );

	if( result == FAIL )
	{
		BYTE fail_reason = GetByte( pBuf, index );

		TempBuf.Add(GAME_START_RESULT);
		TempBuf.Add(FAIL);

		if( fail_reason == 2 )	// double id
		{
			TempBuf.Add( (BYTE)1 );
		}
		else
		{
			TempBuf.Add( (BYTE)255 );
		}

		pUser->Send(TempBuf, TempBuf.GetLength());

		return;
	}

	nAccount = GetVarString( sizeof( strAccount), strAccount, pBuf, index );

	if( nAccount <= 0 || nAccount > ACCOUNT_LENGTH )
	{
		TempBuf.Add(GAME_START_RESULT);
		TempBuf.Add(FAIL);
		TempBuf.Add( (BYTE)255 );
		pUser->Send(TempBuf, TempBuf.GetLength());
		return;
	}

	nUserID = GetVarString( sizeof( strUserID ), strUserID, pBuf, index );

	if( nUserID <= 0 || nUserID > CHAR_NAME_LENGTH )
	{
		TempBuf.Add(GAME_START_RESULT);
		TempBuf.Add(FAIL);
		TempBuf.Add( (BYTE)255 );
		pUser->Send(TempBuf, TempBuf.GetLength());
		return;
	}

	pUser->m_iTimeInterval = GetInt( pBuf, index );
	pUser->m_iDisplayType = GetInt( pBuf, index );
	pUser->m_iValidTime = GetInt( pBuf, index );
	
	//----------------------------------------------------------------------------------------------
	//yskang 0.5 ¿¥°ÔÀÓ »ç¿ëÀÚ´Â 1±º 2±º¿¡¼­ ¸ðµÎ ¹«·á·Î »ç¿ëÇÒ ¼ö ÀÖ´Ù.
	//----------------------------------------------------------------------------------------------
	pUser->m_bMGame = FALSE;//¿¥°ÔÀÓ »ç¿ëÀÚ°¡ ¾Æ´Ï´Ù·Î ÃÊ±âÈ­
	if(pUser->m_iDisplayType > 200)
	{
		pUser->m_iDisplayType -= 200;//¿¥°ÔÀÓ »ç¿ëÀÚ´Â Å¸ÀÔ¿¡ 200ÀÌ ´õÇØÁ® ÀÖ´Ù.
		pUser->m_bMGame = TRUE;//¿¥°ÔÀÓ »ç¿ëÀÚÀÌ°í 1±º 2±º¼­¹ö ÀÌ´Ù.
	}
	
	if(pUser->m_iDisplayType == 1 || pUser->m_iDisplayType == 3 || pUser->m_iDisplayType == 5) // ±â°£Á¦¶ó¸é 
	{
		pUser->m_validDate.wYear = GetShort( pBuf, index );
		pUser->m_validDate.wMonth = GetShort( pBuf, index );
		pUser->m_validDate.wDay = GetShort( pBuf, index );
		pUser->m_validDate.wHour = GetShort( pBuf, index );
		pUser->m_validDate.wMinute = GetShort( pBuf, index );
	}
	//GetCheckValidTime();			// ½Ã°£À» Ç¥½ÃÇÑ´Ù.
	
	//-------------------------------------------------------------------------------------------------

	pUser->BridgeServerUserLogin( strAccount, strUserID );
}

void CServerDlg::BridgeServerUserLogOut(int uid, char *strUserID)
{
	int index = 0;
	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_USER_LOGOUT_REQ, index );
	SetInt( TempBuf, uid, index );
	SetByte( TempBuf, g_byMyServerIndex, index );
	SetByte( TempBuf, (BYTE)strlen( strUserID ), index );
	SetString( TempBuf, strUserID, strlen( strUserID ), index );

	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerUserRestart(int uid, char *strUserID)
{
	int index = 0;
	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_USER_RESTART, index );
	SetInt( TempBuf, uid, index );
	SetByte( TempBuf, g_byMyServerIndex, index );
	SetByte( TempBuf, (BYTE)strlen( strUserID ), index );
	SetString( TempBuf, strUserID, strlen( strUserID ), index );

	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerUserZoneLogIn(int uid, char *strAccount, char *strUserID)
{
	int index = 0;
	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_USER_ZONELOGIN_REQ, index );
	SetInt( TempBuf, uid, index );
	SetByte( TempBuf, g_byMyServerIndex, index );
	SetByte( TempBuf, (BYTE)strlen( strAccount ), index );
	SetString( TempBuf, strAccount, strlen( strAccount ), index );
	SetByte( TempBuf, (BYTE)strlen( strUserID ), index );
	SetString( TempBuf, strUserID, strlen( strUserID ), index );

	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerUserZoneLoginResult(TCHAR *pBuf)
{
	int index = 0;
	CBufferEx	TempBuf;
	char strAccount[ACCOUNT_LENGTH+1];
	char strUserID[CHAR_NAME_LENGTH+1];
	int nAccount, nUserID;

	int uid = GetInt( pBuf, index );

	if( uid < 0 || uid > MAX_USER ) return;

	USER* pUser = m_Com.GetUserUid( uid );

	if( !pUser ) return;

	BYTE result = GetByte( pBuf, index );

	if( result == FAIL )
	{
		BYTE fail_reason = GetByte( pBuf, index );

		TempBuf.Add(GAME_START_RESULT);
		TempBuf.Add(FAIL);

		if( fail_reason == 2 )	// double id
		{
			TempBuf.Add( (BYTE)5 );
		}
		else
		{
			TempBuf.Add( (BYTE)255 );
		}

		pUser->Send(TempBuf, TempBuf.GetLength());

		return;
	}

	nAccount = GetVarString( sizeof( strAccount ), strAccount, pBuf, index );

	if( nAccount <= 0 || nAccount > ACCOUNT_LENGTH )
	{
	}

	nUserID = GetVarString( sizeof( strUserID ), strUserID, pBuf, index );

	if( nUserID <= 0 || nUserID > CHAR_NAME_LENGTH )
	{
	}

	pUser->m_iTimeInterval = GetInt( pBuf, index );
	pUser->m_iDisplayType = GetInt( pBuf, index );
	pUser->m_iValidTime = GetInt( pBuf, index );
	//----------------------------------------------------------------------------------------------
	//yskang 0.5 ¿¥°ÔÀÓ »ç¿ëÀÚ´Â 1±º 2±º¿¡¼­ ¹«·á·Î »ç¿ëÇÒ ¼ö ÀÖ´Ù.
	//----------------------------------------------------------------------------------------------

	pUser->m_bMGame = FALSE;//¿¥°ÔÀÓ »ç¿ëÀÚ°¡ ¾Æ´Ï´Ù·Î ÃÊ±âÈ­
	if(pUser->m_iDisplayType > 200)
	{
		pUser->m_iDisplayType -= 200;//¿¥°ÔÀÓ »ç¿ëÀÚ´Â Å¸ÀÔ¿¡ 200ÀÌ ´õÇØÁ® ÀÖ´Ù.
		pUser->m_bMGame = TRUE;//¿¥°ÔÀÓ »ç¿ëÀÚÀÌ°í 1±º 2±º¼­¹ö ÀÌ´Ù.
	}
	
	if(pUser->m_iDisplayType == 1 || pUser->m_iDisplayType == 3 || pUser->m_iDisplayType == 5) // ±â°£Á¦¶ó¸é 
	{
		pUser->m_validDate.wYear = GetShort( pBuf, index );
		pUser->m_validDate.wMonth = GetShort( pBuf, index );
		pUser->m_validDate.wDay = GetShort( pBuf, index );
		pUser->m_validDate.wHour = GetShort( pBuf, index );
		pUser->m_validDate.wMinute = GetShort( pBuf, index );
	}
	//GetCheckValidTime();			// ½Ã°£À» Ç¥½ÃÇÑ´Ù.
	
	//-------------------------------------------------------------------------------------------------

/*	if(pUser->m_iDisplayType == 1 || pUser->m_iDisplayType == 3 || pUser->m_iDisplayType == 5) // ±â°£Á¦¶ó¸é
	{
		pUser->m_validDate.wYear = GetShort( pBuf, index );
		pUser->m_validDate.wMonth = GetShort( pBuf, index );
		pUser->m_validDate.wDay = GetShort( pBuf, index );
		pUser->m_validDate.wHour = GetShort( pBuf, index );
		pUser->m_validDate.wMinute = GetShort( pBuf, index );
	}
*/
	pUser->BridgeServerUserZoneLogin( strAccount, strUserID );
}

void CServerDlg::BridgeServerDoubleAccountClose(TCHAR *pBuf)
{
	int index = 0;

	USER* pUser = NULL;
	char strAccount[ACCOUNT_LENGTH+1];
	int nAccountID;

	int uid = GetInt( pBuf, index );

	if( uid == -1 )
	{
		nAccountID = GetVarString( sizeof( strAccount ), strAccount, pBuf, index );

		if( nAccountID <= 0 || nAccountID > ACCOUNT_LENGTH ) return;

		CString strSource = strAccount;

		for( int i = 0; i < MAX_USER; i++)
		{
			pUser = m_Com.GetUserUid(i);
			if(pUser == NULL ) continue;

			if( !strSource.CompareNoCase(pUser->m_strAccount))
			{
				if( pUser->m_state != STATE_DISCONNECTED && pUser->m_state != STATE_LOGOUT)
				{
					pUser->SendSystemMsg( IDS_USER_DOUBLE_ACCOUNT, SYSTEM_SPECIAL, TO_ME);
					pUser->SoftClose();
					break;
				}
			}
		}
		return;
	}

	if( uid < 0 || uid >= MAX_USER ) return;

	pUser = m_Com.GetUserUid( uid );

	if( !pUser ) return;

	nAccountID = GetVarString( sizeof( strAccount ), strAccount, pBuf, index );

	if( nAccountID <= 0 || nAccountID > ACCOUNT_LENGTH ) return;

	CString tempAccount;	tempAccount.Format( "%s", strAccount );

	if( tempAccount.CompareNoCase( pUser->m_strAccount ) ) return;

	pUser->SendSystemMsg( IDS_USER_DOUBLE_CHAR, SYSTEM_SPECIAL, TO_ME);
	pUser->SoftClose();
}

void CServerDlg::BridgeServerDoubleIDClose(TCHAR *pBuf)
{
	int index = 0;

	int uid = GetInt( pBuf, index );

	if( uid < 0 || uid >= MAX_USER ) return;

	USER* pUser = m_Com.GetUserUid( uid );

	if( !pUser ) return;

	char strUserID[CHAR_NAME_LENGTH+1];

	int nUserID = GetVarString( sizeof( strUserID ), strUserID, pBuf, index );

	if( nUserID <= 0 || nUserID > ACCOUNT_LENGTH ) return;

	CString tempUserID;	tempUserID.Format( "%s", strUserID );

	if( tempUserID.CompareNoCase( pUser->m_strUserID ) ) return;

	pUser->SendSystemMsg( IDS_USER_DOUBLE_CHAR, SYSTEM_SPECIAL, TO_ME);
	pUser->SoftClose();
}

void CServerDlg::BridgeServerUserWhisperOnOff(int uid, BYTE onoff)
{
	int index = 0;
	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_USER_WHISPER_ONOFF, index );
	SetInt( TempBuf, uid, index );
	SetByte( TempBuf, onoff, index );

	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerUserWhisperReq(TCHAR *pBuf)
{
	int index = 0;
	CBufferEx TempBuf;
	CString tempUserID;

	int uid = GetInt( pBuf, index );

	if( uid < 0 || uid >= MAX_USER ) return;

	USER* pUser = m_Com.GetUserUid( uid );

	if( !pUser ) return;

	char strSendID[CHAR_NAME_LENGTH+1];
	char strRecvID[CHAR_NAME_LENGTH+1];
	char strMsg[512];

	int nSendID = GetVarString( sizeof( strSendID ), strSendID, pBuf, index );
	if( nSendID <= 0 || nSendID > CHAR_NAME_LENGTH ) return;

	int nRecvID = GetVarString( sizeof( strRecvID ), strRecvID, pBuf, index );
	if( nRecvID <= 0 || nRecvID > CHAR_NAME_LENGTH ) return;

	int nMsg = GetVarString( sizeof( strMsg ), strMsg, pBuf, index );
	if( nMsg <= 0 || nMsg >= 512 ) return;

	tempUserID.Format( "%s", strRecvID );

	if( tempUserID.CompareNoCase( pUser->m_strUserID ) ) return;

	pUser->BridgeServerUserWhisper( strSendID, strRecvID, strMsg );
}

void CServerDlg::BridgeServerUserWhisperResult(TCHAR *pBuf)
{
	int index = 0;

	BYTE type = GetByte( pBuf, index );
	int uid = GetInt( pBuf, index );

	if( uid < 0 || uid >= MAX_USER ) return;

	USER* pUser = m_Com.GetUserUid( uid );

	if( !pUser ) return;

	char strSendID[CHAR_NAME_LENGTH+1];
	char strRecvID[CHAR_NAME_LENGTH+1];
	char strMsg[512];

	int nSendID;
	int nRecvID;
	int nMsg;

	// type - 0 : ¹ÞÀ» »ç¶÷ ¾øÀ½, type - 1 : °ÅÀý ¸Þ¼¼Áö(º¸³½»ç¶÷ÇÑÅ× µÇµ¹¾Æ°¨), type - 2 : ¹Þ´Â »ç¶÷ÇÑÅ× °¨.

	CBufferEx TempBuf;
	BYTE result = SUCCESS;
	BYTE error_code;

	if( type == 0 ) // ±Ó¸» »ó´ë ¾øÀ½
	{
		error_code = ERR_3;
		result = FAIL;
		goto result_send;
	}

	if( type == 1 ) // »ó´ëÆíÀÌ ±Ó¸» °ÅºÎÁß
	{
		error_code = ERR_2;
		result = FAIL;
		goto result_send;
	}

	nSendID = GetVarString( sizeof( strSendID ), strSendID, pBuf, index );
	if( nSendID <= 0 || nSendID > CHAR_NAME_LENGTH ) return;

	nRecvID = GetVarString( sizeof( strRecvID ), strRecvID, pBuf, index );
	if( nRecvID <= 0 || nRecvID > CHAR_NAME_LENGTH ) return;

	nMsg = GetVarString( sizeof( strMsg ), strMsg, pBuf, index );
	if( nMsg <= 0 || nMsg >= 512 ) return;

result_send:
	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(result);

	if(result != SUCCESS)
	{
		TempBuf.Add(error_code);
		pUser->Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.Add(WHISPER_CHAT);
	TempBuf.Add((int)0x00);
	TempBuf.AddString( strRecvID );
	TempBuf.AddString( strMsg );
	pUser->Send(TempBuf, TempBuf.GetLength());
}

void CServerDlg::BridgeServerUserGuildChatReq(int uid, int guildnum, char *strUserID, char *strMsg)
{
	int index = 0;
	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_USER_GUILD_CHAT, index );
	SetInt( TempBuf, uid, index );
	SetInt( TempBuf, guildnum, index );
	SetByte( TempBuf, (BYTE)strlen( strUserID ), index );
	SetString( TempBuf, strUserID, strlen( strUserID ), index );
	SetByte( TempBuf, (BYTE)strlen( strMsg ), index );
	SetString( TempBuf, strMsg, strlen( strMsg ), index );

	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerUserGuildChatResult(TCHAR *pBuf)
{
	int index = 0;
	int i;

	int uid = GetInt( pBuf, index );

	if( uid < 0 || uid >= MAX_USER ) return;

	int guildnum = GetInt( pBuf, index );

	char strUserID[CHAR_NAME_LENGTH+1];
	char strMsg[512];

	int nUserID = GetVarString( sizeof( strUserID ), strUserID, pBuf, index );

	if( nUserID <= 0 || nUserID > CHAR_NAME_LENGTH ) return;

	int nMsg = GetVarString( sizeof( strMsg ), strMsg, pBuf, index );

	if( nMsg <= 0 || nMsg >= 512 ) return;

	USER *pUser = NULL;

	CBufferEx TempBuf;

	TempBuf.Add( CHAT_RESULT );
	TempBuf.Add( SUCCESS );
	TempBuf.Add( GUILD_CHAT );
	TempBuf.Add( uid + USER_BAND );
	TempBuf.AddString( strUserID );
	TempBuf.AddString( strMsg );

	for( i = 0; i < MAX_USER; i++ )
	{
		pUser = m_Com.GetUserUid( i );

		if( pUser == NULL || pUser->m_state != STATE_GAMESTARTED ) continue;

		if( guildnum == (int)pUser->m_dwGuild ) pUser->Send( TempBuf, TempBuf.GetLength() );
	}
}

void CServerDlg::BridgeServerGuildNewReq(int uid, char *strUserID, int guildnum, char *strGuildName)
{
	int index = 0;

	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_GUILD_NEW, index );

	SetInt( TempBuf, uid, index );

	SetByte( TempBuf, (BYTE)strlen( strUserID ), index );
	SetString( TempBuf, strUserID, strlen( strUserID ), index );

	SetInt( TempBuf, guildnum, index );

	SetByte( TempBuf, (BYTE)strlen( strGuildName ), index );
	SetString( TempBuf, strGuildName, strlen( strGuildName ), index );

	// º¸³¾¶§ ½ÇÆÐÇÏ¸é ½ÇÆÐÃ³¸® ÇØÁà¾ß ÇÑ´Ù. - ¿Ö? ±æµå°¡ ¾È¸¸µé¾î Áö´Ï±î.
	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerGuildNewResult(TCHAR *pBuf)
{
	int index = 0;

	int uid, i;
	USER* pUser = NULL;

	BYTE result = GetByte( pBuf, index );

	if( result != SUCCESS )
	{
		uid = GetInt( pBuf, index );

		if( uid < 0 || uid >= MAX_USER ) return;

		pUser = m_Com.GetUserUid( uid );

		if( !pUser ) return;

		pUser->SendSystemMsg( _T("Áö±ÝÀº ±æµå¸¦ ¸¸µé ¼ö ¾ø½À´Ï´Ù. Àá½Ã ÈÄ ´Ù½Ã ½ÃµµÇØÁÖ½Ê½Ã¿À"), SYSTEM_NORMAL, TO_ME );
		return;
	}

	BYTE serverindex = GetByte( pBuf, index );

	uid = GetInt( pBuf, index );

	if( uid < 0 || uid >= MAX_USER ) return;

	char strUserID[CHAR_NAME_LENGTH+1];
	char strGuildName[255];

	int nUserID = GetVarString( sizeof( strUserID ), strUserID, pBuf, index );
	if( nUserID <= 0 || nUserID > CHAR_NAME_LENGTH ) return;

	int guildnum = GetInt( pBuf, index );

	if( guildnum <= 0 || guildnum >= MAX_GUILD ) return;

	int nGuildName = GetVarString( sizeof( strGuildName ), strGuildName, pBuf, index );
	if( nGuildName <= 0 || nGuildName >= 255 ) return;

	if( serverindex == g_byMyServerIndex )				// ¹ÞÀº ¼­¹ö ÀÎµ¦½º¿Í ³» ÀÎµ¦½º°¡ °°´Ù. ³»°¡ ±æµå¸¦ DB¿¡ ¾´´Ù.
	{
		pUser = m_Com.GetUserUid( uid );

		if( !pUser ) return;
		if( pUser->m_state != STATE_GAMESTARTED ) return;
		if( strcmp( pUser->m_strUserID, strUserID ) ) return;

		pUser->BridgeServerGuildNewResult( guildnum, strGuildName );
	}
	else												// ´Ù¸£´Ù. ´Ù¸¥ ¼­¹ö°¡ ¿äÃ»ÇÑ °á°ú¸¦ ¹Þ¾Æ ¸Þ¸ð¸®¸¸ ¼ÂÆÃÇÑ´Ù.
	{
		EnterCriticalSection( &(m_Com.m_critGuild) );

		CGuild* pGuild = g_arGuildData[guildnum];

		if( !pGuild )
		{
			LeaveCriticalSection( &(m_Com.m_critGuild) );
			return;
		}

		if( pGuild->m_lSid > 0 )
		{
			LeaveCriticalSection( &(m_Com.m_critGuild) );
			return;
		}

		pGuild->m_lSid = guildnum;

		strcpy( pGuild->m_strGuildName, strGuildName );
		strcpy( pGuild->m_strMasterName, strUserID );

		pGuild->m_dwGuildDN = 0;
		pGuild->m_sVersion = -1;

		WORD *pwMark = (WORD*)(&pGuild->m_strMark[0]);
		for (i=0; i<GUILD_MARK_SIZE/sizeof(WORD); i++) {
			pwMark[i] = (0x1f<<11 | 0x1f);	// Client¿¡¼­ »ç¿ëÇÏ´Â Åõ¸í»öÀ¸·Î ÃÊ±âÈ­ÇÑ´Ù.
		}

		pGuild->AddUser( strUserID, guildnum );

		LeaveCriticalSection( &(m_Com.m_critGuild) );
	}
}

void CServerDlg::BridgeServerGuildDisperseReq(int uid, char *strUserID, int guildnum)
{
	int index = 0;

	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_GUILD_DISPERSE, index );

	SetInt( TempBuf, uid, index );

	SetByte( TempBuf, (BYTE)strlen( strUserID ), index );
	SetString( TempBuf, strUserID, strlen( strUserID ), index );

	SetInt( TempBuf, guildnum, index );

	// º¸³¾¶§ ½ÇÆÐÇÏ¸é ½ÇÆÐÃ³¸® ÇØÁà¾ß ÇÑ´Ù. - ¿Ö? ±æµå°¡ ¾È¸¸µé¾î Áö´Ï±î.
	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerGuildDisperseResult(TCHAR *pBuf)
{
	int index = 0;

	int uid;
	USER* pUser = NULL;

	BYTE result = GetByte( pBuf, index );

	if( result != SUCCESS )
	{
		uid = GetInt( pBuf, index );

		if( uid < 0 || uid >= MAX_USER ) return;

		pUser = m_Com.GetUserUid( uid );

		if( !pUser ) return;

		pUser->SendSystemMsg( _T("Áö±ÝÀº ±æµå¸¦ ÇØ»êÇÒ ¼ö ¾ø½À´Ï´Ù. Àá½Ã ÈÄ ´Ù½Ã ½ÃµµÇØÁÖ½Ê½Ã¿À"), SYSTEM_NORMAL, TO_ME );
		return;
	}

	BYTE serverindex = GetByte( pBuf, index );

	uid = GetInt( pBuf, index );

	if( uid < 0 || uid >= MAX_USER ) return;

	char strUserID[CHAR_NAME_LENGTH+1];

	int nUserID = GetVarString( sizeof( strUserID ), strUserID, pBuf, index );
	if( nUserID <= 0 || nUserID > CHAR_NAME_LENGTH ) return;

	int guildnum = GetInt( pBuf, index );

	if( guildnum <= 0 || guildnum >= MAX_GUILD ) return;

	if( serverindex == g_byMyServerIndex )				// ¹ÞÀº ¼­¹ö ÀÎµ¦½º¿Í ³» ÀÎµ¦½º°¡ °°´Ù. ³»°¡ ±æµå¸¦ DB¿¡ ¾´´Ù.
	{
		pUser = m_Com.GetUserUid( uid );

		if( !pUser ) return;
		if( pUser->m_state != STATE_GAMESTARTED ) return;
		if( strcmp( pUser->m_strUserID, strUserID ) ) return;

		pUser->BridgeServerGuildDisperseResult( guildnum );
	}
	else												// ´Ù¸£´Ù. ´Ù¸¥ ¼­¹ö°¡ ¿äÃ»ÇÑ °á°ú¸¦ ¹Þ¾Æ ¸Þ¸ð¸®¸¸ ¼ÂÆÃÇÑ´Ù.
	{
		int i;
		int nLen = 0;
		BYTE error_code = 0;

		CGuild *pGuild = NULL;
		CGuildUser *pGuildUser = NULL;

		CStore *pStore = NULL;
		CGuildFortress *pFort = NULL;

		if( guildnum <= 0 || guildnum >= g_arGuildData.GetSize() ) return;

		EnterCriticalSection( &(m_Com.m_critGuild) );

		pGuild = g_arGuildData[guildnum];

		if( !pGuild )
		{
			LeaveCriticalSection( &(m_Com.m_critGuild) );
			return;
		}

/*		for(i = 0; i < g_arStore.GetSize(); i++)
		{
			pStore = g_arStore[i];
			if(!pStore) continue;

			if(pStore->m_iGuildSid == guildnum)
			{
				::ZeroMemory(pStore->m_strGuildName, CHAR_NAME_LENGTH + 1);
				::ZeroMemory(pStore->m_strMasterName, CHAR_NAME_LENGTH + 1);

				pStore->InitStoreInfo(-1);

				int nSize = g_arStoreSharedMemory.GetSize();
				CSharedMemory* pShared = NULL;
				CMemStore* pData = NULL;

				if(pStore->m_sStoreID >= nSize) continue;

				pShared = g_arStoreSharedMemory[pStore->m_sStoreID];

				if(pShared == NULL) continue;
				if(pShared->m_hMapping == NULL) continue;

				pData = (CMemStore*) pShared->m_lpData;

				if(pData == NULL) continue;

				pData->m_sStoreID = pStore->m_sStoreID;
				pData->m_iGuildSid = pStore->m_iGuildSid;
				pData->m_sTaxRate = 0;
				pData->m_dwStoreDN = 0;
				pData->m_tWarType = GUILD_WAR_PREPARE;

				pStore->GuildListToStr(pData->m_AttackList);

				break;
			}
		}
*/
		// ±æµåÇÏ¿ì½º°¡ ÀÖ´Ù¸é ÀÌ¿ëºÒ°¡·Î ¸¸µé¾î ÁØ´Ù.
		for( i = 0; i < g_arGuildHouse.GetSize(); i++ )
		{
			if( guildnum == g_arGuildHouse[i]->iGuild )
			{
				CNpc *pNpc = NULL;				// ¹®ÆÐµµ...

				if( g_arGuildHouse[i]->iMarkNpc < 0 || g_arGuildHouse[i]->iMarkNpc >= g_arNpc.GetSize() ) continue;

				pNpc = g_arNpc[g_arGuildHouse[i]->iMarkNpc];
				if(pNpc)
				{
					pNpc->m_sPid = 0;
					pNpc->m_sMaxHP = 1;
					::ZeroMemory(pNpc->m_strName, sizeof(pNpc->m_strName));
				}

				g_arGuildHouse[i]->iGuild = 0;
				break;
			}
		}

		// ¿ä»õ°¡ ÀÖ´Ù¸é ÀÌ¿ëºÒ°¡·Î ¸¸µé¾î ÁØ´Ù.
		for( i = 0; i < g_arGuildFortress.GetSize(); i++ )
		{
			if(!g_arGuildFortress[i]) continue;

			pFort = g_arGuildFortress[i];

			if(pFort->m_iGuildSid == guildnum)
			{
				if(pFort->m_lViolenceUsed == 1)
				{
					pFort->m_wMopPartyTime.wYear = 2030;		// ±æµå°¡ ÇØ»êµÇ¸é¼­ ³ª¸ÓÁö´Â ¸ðµÎ ÃÊ±âÈ­¸¦
					pFort->SetNpcToFortressViolenceEnd(&m_Com);
					pFort->GetOutof(&m_Com);
				}

				pFort->SetInitFortress();
				pFort->InitMemFortress(GUILD_WAR_DECISION);
				break;
			}
		}

		g_arGuildData[guildnum]->InitGuild();

		LeaveCriticalSection( &(m_Com.m_critGuild) );

		for(i = 0; i < MAX_USER; i++)		// ±æµå¿øµé ¼ÂÆÃ
		{
			pUser = m_Com.GetUserUid(i);

			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
			if(pUser->m_dwGuild <= 0) continue;

			if(pUser->m_dwGuild == guildnum)
			{
				pUser->SendSystemMsg( IDS_USER_DISMISS_COMPLETED, SYSTEM_NORMAL, TO_ME);

				pUser->m_dwGuild = -1;							// ÃÊ±âÈ­ÇÑ´Ù.
				pUser->m_sGuildVersion = -1;					// ±æµå ¹®¾ç ¹öÁ¯
				::ZeroMemory(pUser->m_strGuildName, sizeof(pUser->m_strGuildName));
				pUser->m_bGuildMaster = FALSE;					// ±æµå ¸¶½ºÅÍ
				pUser->m_bRepresentationGuild = FALSE;			// ±ÇÇÑ ´ëÇà ¿©ºÎ

				pUser->m_bFieldWarApply = FALSE;
				pUser->m_tGuildWar = GUILD_WAR_AFFTER;
				pUser->m_tGuildHouseWar = GUILD_WAR_AFFTER;
				pUser->m_tFortressWar = GUILD_WAR_AFFTER;

				pUser->SendMyGuildInfo();
			}
		}
	}
}

void CServerDlg::BridgeDeputeGuildMasterOfPowerReq(int muid, int guid, char *strMasterID, char *strSubMasterID, int guildnum)
{
	int index = 0;

	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_GUILD_SUBMASTER_ON, index );

	SetInt( TempBuf, muid, index );
	SetInt( TempBuf, guid, index );

	SetByte( TempBuf, (BYTE)strlen( strMasterID ), index );
	SetString( TempBuf, strMasterID, strlen( strMasterID ), index );

	SetByte( TempBuf, (BYTE)strlen( strSubMasterID ), index );
	SetString( TempBuf, strSubMasterID, strlen( strSubMasterID ), index );

	SetInt( TempBuf, guildnum, index );

	// º¸³¾¶§ ½ÇÆÐÇÏ¸é ½ÇÆÐÃ³¸® ÇØÁà¾ß ÇÑ´Ù. - ¿Ö? ±æµå°¡ ¾È¸¸µé¾î Áö´Ï±î.
	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeDeputeGuildMasterOfPowerResult(TCHAR *pBuf)
{
	int index = 0;

	int muid, guid;
	USER* pUser = NULL;

	BYTE result = GetByte( pBuf, index );

	if( result != SUCCESS )
	{
		muid = GetInt( pBuf, index );

		if( muid < 0 || muid >= MAX_USER ) return;

		pUser = m_Com.GetUserUid( muid );

		if( !pUser ) return;

		pUser->SendSystemMsg( _T("Áö±ÝÀº ±æµå ´ëÇàÀÚºÐÀ» ÀÓ¸íÇÏ½Ç¼ö ¾ø½À´Ï´Ù. Àá½Ã ÈÄ ´Ù½Ã ½ÃµµÇØÁÖ½Ê½Ã¿À"), SYSTEM_NORMAL, TO_ME );
		return;
	}

	BYTE serverindex = GetByte( pBuf, index );

	muid = GetInt( pBuf, index );
	guid = GetInt( pBuf, index );

	if( muid < 0 || muid >= MAX_USER ) return;
	if( guid < 0 || guid >= MAX_USER ) return;

	char strMasterID[CHAR_NAME_LENGTH+1];
	char strSubMasterID[CHAR_NAME_LENGTH+1];

	int nMasterID = GetVarString( sizeof( strMasterID ), strMasterID, pBuf, index );
	if( nMasterID <= 0 || nMasterID > CHAR_NAME_LENGTH ) return;

	int nGuestID = GetVarString( sizeof( strSubMasterID ), strSubMasterID, pBuf, index );
	if( nGuestID <= 0 || nGuestID > CHAR_NAME_LENGTH ) return;

	int guildnum = GetInt( pBuf, index );

	if( guildnum <= 0 || guildnum >= MAX_GUILD ) return;

	if( serverindex == g_byMyServerIndex )				// ¹ÞÀº ¼­¹ö ÀÎµ¦½º¿Í ³» ÀÎµ¦½º°¡ °°´Ù. ³»°¡ ±æµå¸¦ DB¿¡ ¾´´Ù.
	{
		pUser = m_Com.GetUserUid( muid );

		if( !pUser ) return;
		if( pUser->m_state != STATE_GAMESTARTED ) return;
		if( strcmp( pUser->m_strUserID, strMasterID ) ) return;

		pUser->BridgeDeputeGuildMasterOfPowerResult( guid );
		return;
	}
	else												// ´Ù¸£´Ù. ´Ù¸¥ ¼­¹ö°¡ ¿äÃ»ÇÑ °á°ú¸¦ ¹Þ¾Æ ¸Þ¸ð¸®¸¸ ¼ÂÆÃÇÑ´Ù.
	{
		CGuild* pGuild = NULL;

		EnterCriticalSection( &(m_Com.m_critGuild) );

		pGuild = g_arGuildData[guildnum];

		if( !pGuild )
		{
			LeaveCriticalSection( &(m_Com.m_critGuild) );
			return;
		}

		pGuild->CheckGuildSubMaster();
		pGuild->SetSubGuildMaster( strSubMasterID );

		LeaveCriticalSection( &(m_Com.m_critGuild) );

		// ´Ù¸¥ ±æµå¿ø¿¡°Ô ´ëÇàÀÚ°¡ ÀÓ¸íµÇ¾ú´Ù´Â °Å ¾Ë¸®±â
		int i;
		USER *pGUser = NULL;
		CBufferEx TempBuf;

		CString strMsg = _T("");
		strMsg.Format( IDS_USER_GUILD_SUBMASTER_ON, strSubMasterID);

		for(i = 0; i< MAX_USER; i++)
		{
			pGUser = m_Com.GetUserUid(i);

			if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;

			if(guildnum == pGUser->m_dwGuild) pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}

	}
}

void CServerDlg::BridgeRemoveGuildMasterOfPowerReq(int muid, char *strMasterID, int guildnum)
{
	int index = 0;

	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_GUILD_SUBMASTER_OFF, index );

	SetInt( TempBuf, muid, index );

	SetByte( TempBuf, (BYTE)strlen( strMasterID ), index );
	SetString( TempBuf, strMasterID, strlen( strMasterID ), index );

	SetInt( TempBuf, guildnum, index );

	// º¸³¾¶§ ½ÇÆÐÇÏ¸é ½ÇÆÐÃ³¸® ÇØÁà¾ß ÇÑ´Ù. - ¿Ö? ±æµå°¡ ¾È¸¸µé¾î Áö´Ï±î.
	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeRemoveGuildMasterOfPowerResult(TCHAR *pBuf)
{
	int index = 0;

	int muid;
	USER* pUser = NULL;

	BYTE result = GetByte( pBuf, index );

	if( result != SUCCESS )
	{
		muid = GetInt( pBuf, index );

		if( muid < 0 || muid >= MAX_USER ) return;

		pUser = m_Com.GetUserUid( muid );

		if( !pUser ) return;

		pUser->SendSystemMsg( _T("Áö±ÝÀº ´ëÇàÀÚºÐÀÇ ±ÇÇÑ¸¦ ÁßÁö ÇÏ½Ç¼ö ¾ø½À´Ï´Ù. Àá½Ã ÈÄ ´Ù½Ã ½ÃµµÇØÁÖ½Ê½Ã¿À"), SYSTEM_NORMAL, TO_ME );
		return;
	}

	BYTE serverindex = GetByte( pBuf, index );

	muid = GetInt( pBuf, index );
	if( muid < 0 || muid >= MAX_USER ) return;

	char strMasterID[CHAR_NAME_LENGTH+1];

	int nMasterID = GetVarString( sizeof( strMasterID ), strMasterID, pBuf, index );
	if( nMasterID <= 0 || nMasterID > CHAR_NAME_LENGTH ) return;

	int guildnum = GetInt( pBuf, index );

	if( guildnum <= 0 || guildnum >= MAX_GUILD ) return;

	if( serverindex == g_byMyServerIndex )				// ¹ÞÀº ¼­¹ö ÀÎµ¦½º¿Í ³» ÀÎµ¦½º°¡ °°´Ù. ³»°¡ ±æµå¸¦ DB¿¡ ¾´´Ù.
	{
		pUser = m_Com.GetUserUid( muid );

		if( !pUser ) return;
		if( pUser->m_state != STATE_GAMESTARTED ) return;
		if( strcmp( pUser->m_strUserID, strMasterID ) ) return;

		pUser->BridgeRemoveGuildMasterOfPowerResult();
		return;
	}
	else												// ´Ù¸£´Ù. ´Ù¸¥ ¼­¹ö°¡ ¿äÃ»ÇÑ °á°ú¸¦ ¹Þ¾Æ ¸Þ¸ð¸®¸¸ ¼ÂÆÃÇÑ´Ù.
	{
		TCHAR strSubMasterID[CHAR_NAME_LENGTH + 1];
		::ZeroMemory(strSubMasterID, sizeof(strSubMasterID));

		CGuild* pGuild = NULL;

		EnterCriticalSection( &(m_Com.m_critGuild) );

		pGuild = g_arGuildData[guildnum];

		if( !pGuild )
		{
			LeaveCriticalSection( &(m_Com.m_critGuild) );
			return;
		}

		::CopyMemory(strSubMasterID, pGuild->m_strSubMasterName, sizeof(strSubMasterID));
		pGuild->RemoveSubGuildMaster();

		LeaveCriticalSection( &(m_Com.m_critGuild) );

		// ´Ù¸¥ ±æµå¿ø¿¡°Ô ´ëÇàÀÚÀÚ¸®°¡ ºñ¾ú´Ù´Â °Å ¾Ë¸®±â
		int i;
		USER *pGUser = NULL;
		CBufferEx TempBuf;

		CString strMsg = _T("");
		strMsg.Format( IDS_USER_GUILD_SUBMASTER_OFF, strSubMasterID);

		for(i = 0; i< MAX_USER; i++)
		{
			pGUser = m_Com.GetUserUid(i);

			if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;

			if(guildnum == pGUser->m_dwGuild) pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}
}

void CServerDlg::BridgeServerGuildInviteReq(int muid, int guid, char *strMasterID, char *strGuestID, int guildnum)
{
	int index = 0;

	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_GUILD_INVITE, index );

	SetInt( TempBuf, muid, index );
	SetInt( TempBuf, guid, index );

	SetByte( TempBuf, (BYTE)strlen( strMasterID ), index );
	SetString( TempBuf, strMasterID, strlen( strMasterID ), index );

	SetByte( TempBuf, (BYTE)strlen( strGuestID ), index );
	SetString( TempBuf, strGuestID, strlen( strGuestID ), index );

	SetInt( TempBuf, guildnum, index );

	// º¸³¾¶§ ½ÇÆÐÇÏ¸é ½ÇÆÐÃ³¸® ÇØÁà¾ß ÇÑ´Ù. - ¿Ö? ±æµå°¡ ¾È¸¸µé¾î Áö´Ï±î.
	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerGuildInviteResult(TCHAR *pBuf)
{
	int index = 0;

	int muid, guid;
	USER* pUser = NULL;

	BYTE result = GetByte( pBuf, index );

	if( result != SUCCESS )
	{
		muid = GetInt( pBuf, index );

		if( muid < 0 || muid >= MAX_USER ) return;

		pUser = m_Com.GetUserUid( muid );

		if( !pUser ) return;

		pUser->SendSystemMsg( _T("Áö±ÝÀº ±æµå °¡ÀÔ Ã³¸®¸¦ ÇÒ ¼ö ¾ø½À´Ï´Ù. Àá½Ã ÈÄ ´Ù½Ã ½ÃµµÇØÁÖ½Ê½Ã¿À"), SYSTEM_NORMAL, TO_ME );
		return;
	}

	BYTE serverindex = GetByte( pBuf, index );

	muid = GetInt( pBuf, index );
	guid = GetInt( pBuf, index );

	if( muid < 0 || muid >= MAX_USER ) return;
	if( guid < 0 || guid >= MAX_USER ) return;

	char strMasterID[CHAR_NAME_LENGTH+1];
	char strGuestID[CHAR_NAME_LENGTH+1];

	int nMasterID = GetVarString( sizeof( strMasterID ), strMasterID, pBuf, index );
	if( nMasterID <= 0 || nMasterID > CHAR_NAME_LENGTH ) return;

	int nGuestID = GetVarString( sizeof( strGuestID ), strGuestID, pBuf, index );
	if( nGuestID <= 0 || nGuestID > CHAR_NAME_LENGTH ) return;

	int guildnum = GetInt( pBuf, index );

	if( guildnum <= 0 || guildnum >= MAX_GUILD ) return;

	if( serverindex == g_byMyServerIndex )				// ¹ÞÀº ¼­¹ö ÀÎµ¦½º¿Í ³» ÀÎµ¦½º°¡ °°´Ù. ³»°¡ ±æµå¸¦ DB¿¡ ¾´´Ù.
	{
		pUser = m_Com.GetUserUid( muid );

		if( !pUser ) return;
		if( pUser->m_state != STATE_GAMESTARTED ) return;
		if( strcmp( pUser->m_strUserID, strMasterID ) ) return;

		pUser->BridgeServerGuildInviteResult( guid, strGuestID );
	}
	else												// ´Ù¸£´Ù. ´Ù¸¥ ¼­¹ö°¡ ¿äÃ»ÇÑ °á°ú¸¦ ¹Þ¾Æ ¸Þ¸ð¸®¸¸ ¼ÂÆÃÇÑ´Ù.
	{
		CGuild* pGuild = NULL;

		EnterCriticalSection( &(m_Com.m_critGuild) );

		pGuild = g_arGuildData[guildnum];

		if( !pGuild )
		{
			LeaveCriticalSection( &(m_Com.m_critGuild) );
			return;
		}

		if( !pGuild->AddUser( strGuestID, pGuild->m_lSid ) )
		{
			LeaveCriticalSection( &(m_Com.m_critGuild) );
			return;
		}

		LeaveCriticalSection( &(m_Com.m_critGuild) );
	}
}

void CServerDlg::BridgeServerGuildOffReq(int uid, char *strUserID, char *strOffID, int guildnum)
{
	int index = 0;

	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_GUILD_OFF, index );

	SetInt( TempBuf, uid, index );

	SetByte( TempBuf, (BYTE)strlen( strUserID ), index );
	SetString( TempBuf, strUserID, strlen( strUserID ), index );

	SetByte( TempBuf, (BYTE)strlen( strOffID ), index );
	SetString( TempBuf, strOffID, strlen( strOffID ), index );

	SetInt( TempBuf, guildnum, index );

	// º¸³¾¶§ ½ÇÆÐÇÏ¸é ½ÇÆÐÃ³¸® ÇØÁà¾ß ÇÑ´Ù. - ¿Ö? ±æµå°¡ ¾È¸¸µé¾î Áö´Ï±î.
	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerGuildOffResult(TCHAR *pBuf)
{
	int index = 0;

	int uid;
	USER* pUser = NULL;

	BYTE result = GetByte( pBuf, index );

	if( result != SUCCESS )
	{
		uid = GetInt( pBuf, index );

		if( uid < 0 || uid >= MAX_USER ) return;

		pUser = m_Com.GetUserUid( uid );

		if( !pUser ) return;

		pUser->SendSystemMsg( _T("Áö±ÝÀº ±æµå Å»Åð Ã³¸®¸¦ ÇÒ ¼ö ¾ø½À´Ï´Ù. Àá½Ã ÈÄ ´Ù½Ã ½ÃµµÇØÁÖ½Ê½Ã¿À"), SYSTEM_NORMAL, TO_ME );
		return;
	}

	BYTE serverindex = GetByte( pBuf, index );

	uid = GetInt( pBuf, index );

	if( uid < 0 || uid >= MAX_USER ) return;

	char strReqID[CHAR_NAME_LENGTH+1];
	char strOffID[CHAR_NAME_LENGTH+1];

	int nReqID = GetVarString( sizeof( strReqID ), strReqID, pBuf, index );
	if( nReqID <= 0 || nReqID > CHAR_NAME_LENGTH ) return;

	int nOffID = GetVarString( sizeof( strOffID ), strOffID, pBuf, index );
	if( nOffID <= 0 || nOffID > CHAR_NAME_LENGTH ) return;

	int guildnum = GetInt( pBuf, index );

	if( guildnum <= 0 || guildnum >= MAX_GUILD ) return;

	if( serverindex == g_byMyServerIndex )				// ¹ÞÀº ¼­¹ö ÀÎµ¦½º¿Í ³» ÀÎµ¦½º°¡ °°´Ù. ³»°¡ ±æµå¸¦ DB¿¡ ¾´´Ù.
	{
		pUser = m_Com.GetUserUid( uid );

		if( !pUser ) return;
		if( pUser->m_state != STATE_GAMESTARTED ) return;
		if( strcmp( pUser->m_strUserID, strReqID ) ) return;

		pUser->BridgeServerGuildOffResult( strReqID, strOffID );
	}
	else												// ´Ù¸£´Ù. ´Ù¸¥ ¼­¹ö°¡ ¿äÃ»ÇÑ °á°ú¸¦ ¹Þ¾Æ ¸Þ¸ð¸®¸¸ ¼ÂÆÃÇÑ´Ù.
	{
		CGuild* pGuild = NULL;

		EnterCriticalSection( &(m_Com.m_critGuild) );

		pGuild = g_arGuildData[guildnum];

		if( !pGuild )
		{
			LeaveCriticalSection( &(m_Com.m_critGuild) );
			return;
		}

		if(!pGuild->RemoveUser( strOffID ))
		{
			LeaveCriticalSection( &(m_Com.m_critGuild) );
			return;
		}

		LeaveCriticalSection( &(m_Com.m_critGuild) );

		// ´Ù¸¥ ±æµå¿ø¿¡°Ô ´©°¡ ±æµå¿¡¼­ Å»ÅðµÇ¾ú´Ù´Â °Å ¾Ë¸®±â
		int i, j;
		USER *pGUser = NULL;
		CBufferEx TempBuf;

		CString strMsg = _T("");
		strMsg.Format( IDS_USER_OUT_MEMBER, strOffID);

		for(i = 0; i< MAX_USER; i++)
		{
			pGUser = m_Com.GetUserUid(i);

			if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;

			if(guildnum == pGUser->m_dwGuild) pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}

		// Æ÷Æ®¸®½º Á¤º¸¿¡¼­ Å»ÅðµÈ»ç¶÷ Á¤º¸ »©±â
		int ilen = 0;

		for(i = 0; i < g_arGuildFortress.GetSize(); i++)
		{
			if(!g_arGuildFortress[i] || !g_arGuildFortress[i]->m_bHaveGuild) continue;

			if(g_arGuildFortress[i]->m_iGuildSid == guildnum)
			{
				for(j = 0; j < MAX_GUILD_USER; j++)
				{
					ilen = strlen(g_arGuildFortress[i]->m_arCityRankList[j].strUserID);
					if(ilen <= 0 || ilen > CHAR_NAME_LENGTH) continue;

					if(strcmp(strOffID, g_arGuildFortress[i]->m_arCityRankList[j].strUserID) == 0)
					{
						::ZeroMemory(g_arGuildFortress[i]->m_arCityRankList[j].strUserID, sizeof(g_arGuildFortress[i]->m_arCityRankList[j].strUserID));
						g_arGuildFortress[i]->m_arCityRankList[j].lCityRank = 0;
						InterlockedExchange(&g_arGuildFortress[i]->m_arCityRankList[j].lUsed, (long)0);
						break;
					}
				}
			}
		}


		USER* pUser = m_Com.GetUser( strOffID );

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

		pUser->m_dwGuild = -1;
		pUser->m_sGuildVersion = -1;		// ±æµå ¹®¾ç ¹öÁ¯
		::ZeroMemory(pUser->m_strGuildName, sizeof(pUser->m_strGuildName));
		pUser->m_bGuildMaster = FALSE;			// ±æµå ¸¶½ºÅÍ
		pUser->m_bRepresentationGuild = FALSE;	// ±ÇÇÑ ´ëÇà ¿©ºÎ
		pUser->SendMyGuildInfo();
		pUser->SendSystemMsg( IDS_USER_KICKOUT_GUILD, SYSTEM_NORMAL, TO_ME);
	}
}

void CServerDlg::BridgeServerGuildMarkReq(int uid, char *strUserID, int guildnum, char *pGuildMark)
{
	int index = 0;

	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_GUILD_MARK, index );

	SetInt( TempBuf, uid, index );

	SetByte( TempBuf, (BYTE)strlen( strUserID ), index );
	SetString( TempBuf, strUserID, strlen( strUserID ), index );

	SetInt( TempBuf, guildnum, index );

	::CopyMemory( TempBuf+index, pGuildMark, GUILD_MARK_SIZE);

	index += GUILD_MARK_SIZE;

	// º¸³¾¶§ ½ÇÆÐÇÏ¸é ½ÇÆÐÃ³¸® ÇØÁà¾ß ÇÑ´Ù. - ¿Ö? ±æµå°¡ ¾È¸¸µé¾î Áö´Ï±î.
	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerGuildMarkResult(TCHAR *pBuf)
{
	int index = 0;

	int uid;
	USER* pUser = NULL;
	TCHAR guildmark[GUILD_MARK_SIZE+1];

	BYTE result = GetByte( pBuf, index );

	if( result != SUCCESS )
	{
		uid = GetInt( pBuf, index );

		if( uid < 0 || uid >= MAX_USER ) return;

		pUser = m_Com.GetUserUid( uid );

		if( !pUser ) return;

		pUser->SendSystemMsg( _T("Áö±ÝÀº ±æµå ¸¶Å©¸¦ µî·ÏÇÒ ¼ö ¾ø½À´Ï´Ù. Àá½Ã ÈÄ ´Ù½Ã ½ÃµµÇØÁÖ½Ê½Ã¿À"), SYSTEM_NORMAL, TO_ME );
		return;
	}

	BYTE serverindex = GetByte( pBuf, index );

	uid = GetInt( pBuf, index );

	if( uid < 0 || uid >= MAX_USER ) return;

	char strUserID[CHAR_NAME_LENGTH+1];

	int nUserID = GetVarString( sizeof( strUserID ), strUserID, pBuf, index );
	if( nUserID <= 0 || nUserID > CHAR_NAME_LENGTH ) return;

	int guildnum = GetInt( pBuf, index );

	if( guildnum <= 0 || guildnum >= MAX_GUILD ) return;

	::CopyMemory( guildmark, pBuf+index, GUILD_MARK_SIZE );

	if( serverindex == g_byMyServerIndex )				// ¹ÞÀº ¼­¹ö ÀÎµ¦½º¿Í ³» ÀÎµ¦½º°¡ °°´Ù. ³»°¡ ±æµå¸¦ DB¿¡ ¾´´Ù.
	{
		pUser = m_Com.GetUserUid( uid );

		if( !pUser ) return;
		if( pUser->m_state != STATE_GAMESTARTED ) return;
		if( strcmp( pUser->m_strUserID, strUserID ) ) return;

		pUser->BridgeServerGuildMarkResult( guildmark );
	}
	else												// ´Ù¸£´Ù. ´Ù¸¥ ¼­¹ö°¡ ¿äÃ»ÇÑ °á°ú¸¦ ¹Þ¾Æ ¸Þ¸ð¸®¸¸ ¼ÂÆÃÇÑ´Ù.
	{
		CGuild* pGuild = NULL;
		CBufferEx TempBuf;

		EnterCriticalSection( &(m_Com.m_critGuild) );

		pGuild = g_arGuildData[guildnum];

		if(!pGuild)
		{
			LeaveCriticalSection( &(m_Com.m_critGuild) );
			return;
		}

		if(pGuild->m_sVersion >= 0) CheckMaxValue((short &)pGuild->m_sVersion, (short)1);
		else pGuild->m_sVersion += 1;

		::CopyMemory(pGuild->m_strMark, guildmark, GUILD_MARK_SIZE);

		LeaveCriticalSection( &(m_Com.m_critGuild) );

												// ÇØ´ç ±æµåÇÏ¿ì½º ¹®¾çÀ» º¯°æÇÑ´Ù.
		for(int i = 0; i < g_arGuildHouse.GetSize(); i++)
		{
			if(pGuild->m_lSid == g_arGuildHouse[i]->iGuild)
			{
				int modify_index = 0;
				char modify_send[2048];

				CNpc *pNpc = NULL;
				
				if( g_arGuildHouse[i]->iMarkNpc < 0 || g_arGuildHouse[i]->iMarkNpc >= g_arNpc.GetSize() ) return;
				pNpc = g_arNpc[g_arGuildHouse[i]->iMarkNpc];
				if(!pNpc) return;

				modify_index = 0;
				pNpc->m_sMaxHP = pGuild->m_sVersion;
				::ZeroMemory(modify_send, sizeof(modify_send));
				pNpc->FillNpcInfo(modify_send, modify_index, INFO_MODIFY);
				pNpc->SendInsight(&m_Com, modify_send, modify_index);

				break;
			}
		}
	}
}

void CServerDlg::BridgeServerUserRestartResult(TCHAR *pBuf)
{
	int index = 0;

	int uid = GetInt( pBuf, index );

	if( uid < 0 || uid > MAX_USER ) return;

	char strUserID[CHAR_NAME_LENGTH+1];
	int nUserID = GetVarString( sizeof( strUserID ), strUserID, pBuf, index );

	if( nUserID <= 0 || nUserID > CHAR_NAME_LENGTH ) return;

	USER* pUser = m_Com.GetUserUid( uid );

	if( pUser )
	{
		if( !strcmp( strUserID, pUser->m_strUserID ) )
			pUser->BridgeServerUserRestartResult();
	}
}

void CServerDlg::BridgeServerUserZoneLogOut(int uid, char *strUserID, int z, int x, int y)
{
	int index = 0;
	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_USER_ZONE_LOGOUT, index );
	SetInt( TempBuf, uid, index );
	SetByte( TempBuf, g_byMyServerIndex, index );
	SetByte( TempBuf, (BYTE)strlen( strUserID ), index );
	SetString( TempBuf, strUserID, strlen( strUserID ), index );
	SetShort( TempBuf, z, index );
	SetShort( TempBuf, x, index );
	SetShort( TempBuf, y, index );

	m_pBridgeSocket->Send(TempBuf, index);
}

void CServerDlg::BridgeServerUserZoneLogOutResult(TCHAR *pBuf)
{
	int index = 0;

	int uid = GetInt( pBuf, index );

	if( uid < 0 || uid > MAX_USER ) return;

	char strUserID[CHAR_NAME_LENGTH+1];
	int nUserID = GetVarString( sizeof( strUserID ), strUserID, pBuf, index );

	if( nUserID <= 0 || nUserID > CHAR_NAME_LENGTH ) return;

	USER* pUser = m_Com.GetUserUid( uid );

	int z = GetShort( pBuf, index );
	int x = GetShort( pBuf, index );
	int y = GetShort( pBuf, index );

	if( pUser )
	{
		if( !strcmp( strUserID, pUser->m_strUserID ) )
			pUser->BridgeServerUserZoneLogOutResult( z, x, y);
	}
}

void CServerDlg::Announce(TCHAR *pBuf, BYTE sysType)
{
	m_Com.Announce( pBuf, sysType );
}

void CServerDlg::AnnounceAllServer(TCHAR *pBuf, BYTE sysType)
{
	int index = 0;
	TCHAR TempBuf[SEND_BUF_SIZE];

	SetByte( TempBuf, SERVER_ANNOUNCE, index );
	SetByte( TempBuf, sysType, index );
	SetByte( TempBuf, strlen( pBuf ), index );
	SetString( TempBuf, pBuf, strlen( pBuf ), index );

	m_pBridgeSocket->Send( TempBuf, index );
}

void CServerDlg::AnnounceZone(TCHAR *pBuf, BYTE sysType, int iZone)
{
	m_Com.AnnounceZone( pBuf, sysType, iZone );
}

BOOL CServerDlg::GetRoyalRumbleData()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	int i, iNum = 0;
	int* pZone = NULL;

	SQLINTEGER	sInd;
	SQLSMALLINT sYear, sMonth, sDay;
	SQLCHAR		strWinner[CHAR_NAME_LENGTH+1];

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strWinner, sizeof(strWinner));

	sYear = sMonth = sDay = 0;
	sInd = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("select * from royal_rumble"));

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Royal Rumble Data !!\n");

		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			i = 1;

			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sYear, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sMonth, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sDay, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_CHAR,	  strWinner,sizeof(strWinner),	  &sInd );
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	if( sYear == 0 || sMonth == 0 || sDay == 0 ) return TRUE;

	g_RR.Init( (int)sYear, (int)sMonth, (int)sDay );

	g_strARKRRWinner.Format( "%s", strWinner );

	return TRUE;
}

void CServerDlg::BridgeServerAnnounce(TCHAR *pBuf)
{
	int index = 0;

	BYTE sysType = GetByte( pBuf, index );

	char strMsg[1024];

	int nMsg = GetVarString( sizeof(strMsg), strMsg, pBuf, index );

	if( nMsg <= 0 || nMsg > 1024 ) return;

	m_Com.Announce( strMsg, sysType );
}

void CServerDlg::BridgeServerArkWinnerChangeResult(TCHAR *pBuf)
{
	int index = 0;

	char strWinner[CHAR_NAME_LENGTH+1];		memset( strWinner, NULL, CHAR_NAME_LENGTH+1 );

	int nID = GetVarString( sizeof(strWinner), strWinner, pBuf, index );

	if( nID < 0 || nID > CHAR_NAME_LENGTH ) return;
	if( nID == 0 ) memset( strWinner, NULL, CHAR_NAME_LENGTH+1 );

	if( g_strARKRRWinner.CompareNoCase( strWinner ) )
	{
		char tempID[CHAR_NAME_LENGTH+1];
		strcpy( tempID, g_strARKRRWinner );

		g_RR.RobItem();
	}

	g_strARKRRWinner.Format( strWinner );
}

BOOL CServerDlg::GetEventBlockData()
{
	int i;
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	SQLSMALLINT		sBlockType, sZone, sX, sY;
	SQLINTEGER		sInd;

	sBlockType = 0, sZone = 0, sX = 0, sY = 0;

	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT * FROM EVENT_BLOCKING"));

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load EVENT_BLOCKING Data !!\n");

		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	g_pEventBlock = new CEventBlockingTable;

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				Event	*pEvent = new Event;

				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBlockType, sizeof(sBlockType), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sZone, sizeof(sZone), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sX, sizeof(sX), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sY, sizeof(sY), &sInd );

				pEvent->sBlockType = sBlockType;
				pEvent->sZone = sZone;
				pEvent->sX = sX;
				pEvent->sY = sY;

				if(sBlockType == SELL_BLOCK) g_pEventBlock->m_arSellBlock.Add(pEvent);
				else if(sBlockType == BUY_BLOCK) g_pEventBlock->m_arBuyBlock.Add(pEvent);
				else if(sBlockType == REPAIR_BLOCK) g_pEventBlock->m_arRepaireBlock.Add(pEvent);
				else if(sBlockType == CURE_BLOCK) g_pEventBlock->m_arCureBlock.Add(pEvent);
				else if(sBlockType == TEL_BLOCK) g_pEventBlock->m_arTelBlock.Add(pEvent);
				else 
				{
					if(pEvent) delete pEvent;
				}

				sBlockType = 0, sZone = 0, sX = 0, sY = 0;
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);

	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//	°³Á¶ Å×ÀÌºíÀ» °¡Á®¿Â´Ù.
//
BOOL CServerDlg::GetRemodelingTable()
{
	CRemodelingTable *pRemodel = NULL;

	int i = 0, j = 0;
	int iFirst = 0;

	CRemodelingTableSet	RemodelingSet;

	try
	{
		if(RemodelingSet.IsOpen()) RemodelingSet.Close();

		if( !RemodelingSet.Open() )
		{
			AfxMessageBox(_T("Remodeling DB Open Fail!"));
			return FALSE;
		}
		if(RemodelingSet.IsBOF()) 
		{
			AfxMessageBox(_T("Remodeling DB Empty!"));
			return FALSE;
		}
		
		i = 0;
		while( !RemodelingSet.IsEOF() )
		{
			pRemodel = new CRemodelingTable;

			pRemodel->m_sRid			= RemodelingSet.m_sRid;
			pRemodel->m_sSid			= RemodelingSet.m_sSid;
			pRemodel->m_tWearInfo		= RemodelingSet.m_tWearInfo;
			pRemodel->m_tNeedClass		= RemodelingSet.m_tNeedClass;
			pRemodel->m_sPlan			= RemodelingSet.m_sPlan;
			pRemodel->m_tRandom1		= RemodelingSet.m_tRandom1;
			pRemodel->m_tRandom2		= RemodelingSet.m_tRandom2;
			pRemodel->m_tReplace		= RemodelingSet.m_tReplace;

			if(pRemodel->m_tRandom1 != 0) 
			{
				g_arRemodelingRandomIndex1.Add(i);
				j++;
			}
			if(pRemodel->m_tWearInfo != 1 && i != 0 && iFirst == 0) 
			{
				g_iRemodelingArmorIndex1 = j - 2;
				iFirst = 1;
			}

			g_arRemodelingTable1.Add(pRemodel);
			
			RemodelingSet.MoveNext();
			i++;
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	i = j = iFirst = 0;
	CRemodeling02TableSet	RemodelingSet2;

	try
	{
		if(RemodelingSet2.IsOpen()) RemodelingSet2.Close();

		if( !RemodelingSet2.Open() )
		{
			AfxMessageBox(_T("Remodeling 02 DB Open Fail!"));
			return FALSE;
		}
		if(RemodelingSet2.IsBOF()) 
		{
			AfxMessageBox(_T("Remodeling 02 DB Empty!"));
			return FALSE;
		}
		
		i = 0;
		while( !RemodelingSet2.IsEOF() )
		{
			pRemodel = new CRemodelingTable;

			pRemodel->m_sRid			= RemodelingSet2.m_sRid;
			pRemodel->m_sSid			= RemodelingSet2.m_sSid;
			pRemodel->m_tWearInfo		= RemodelingSet2.m_tWearInfo;
			pRemodel->m_tNeedClass		= RemodelingSet2.m_tNeedClass;
			pRemodel->m_sPlan			= RemodelingSet2.m_sPlan;
			pRemodel->m_tRandom1		= RemodelingSet2.m_tRandom1;
			pRemodel->m_tRandom2		= RemodelingSet2.m_tRandom2;
			pRemodel->m_tReplace		= RemodelingSet2.m_tReplace;

			if(pRemodel->m_tRandom1 != 0) 
			{
				g_arRemodelingRandomIndex2.Add(i);
				j++;
			}
			if(pRemodel->m_tWearInfo != 1 && i != 0 && iFirst == 0) 
			{
				g_iRemodelingArmorIndex2 = j - 2;
				iFirst = 1;
			}

			g_arRemodelingTable2.Add(pRemodel);
			
			RemodelingSet2.MoveNext();
			i++;
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	i = j = iFirst = 0;
	CRemodeling03TableSet	RemodelingSet3;

	try
	{
		if(RemodelingSet3.IsOpen()) RemodelingSet3.Close();

		if( !RemodelingSet3.Open() )
		{
			AfxMessageBox(_T("Remodeling 03 DB Open Fail!"));
			return FALSE;
		}
		if(RemodelingSet3.IsBOF()) 
		{
			AfxMessageBox(_T("Remodeling 03 DB Empty!"));
			return FALSE;
		}
		
		i = 0;
		while( !RemodelingSet3.IsEOF() )
		{
			pRemodel = new CRemodelingTable;

			pRemodel->m_sRid			= RemodelingSet3.m_sRid;
			pRemodel->m_sSid			= RemodelingSet3.m_sSid;
			pRemodel->m_tWearInfo		= RemodelingSet3.m_tWearInfo;
			pRemodel->m_tNeedClass		= RemodelingSet3.m_tNeedClass;
			pRemodel->m_sPlan			= RemodelingSet3.m_sPlan;
			pRemodel->m_tRandom1		= RemodelingSet3.m_tRandom1;
			pRemodel->m_tRandom2		= RemodelingSet3.m_tRandom2;
			pRemodel->m_tReplace		= RemodelingSet3.m_tReplace;

			if(pRemodel->m_tRandom1 != 0) 
			{
				g_arRemodelingRandomIndex3.Add(i);
				j++;
			}
			if(pRemodel->m_tWearInfo != 1 && i != 0 && iFirst == 0) 
			{
				g_iRemodelingArmorIndex3 = j - 2;
				iFirst = 1;
			}

			g_arRemodelingTable3.Add(pRemodel);
			
			RemodelingSet3.MoveNext();
			i++;
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//	Load Box Event Table
//
BOOL CServerDlg::GetBoxEventTable()
{
	CBoxEventTableSet	BoxEventTableSet;
	
	try
	{
		if(BoxEventTableSet.IsOpen()) BoxEventTableSet.Close();

		BoxEventTableSet.m_strSort = "sSid ASC";

		if( !BoxEventTableSet.Open() )
		{
			AfxMessageBox(_T("Box Event Table Open Fail!"));
			return FALSE;
		}
		if(BoxEventTableSet.IsBOF()) 
		{
			AfxMessageBox(_T("Box Event Table DB Empty!"));
			return FALSE;
		}
		
		while( !BoxEventTableSet.IsEOF() )
		{
			CBoxEventTable* pNewItem = new CBoxEventTable;
					
			pNewItem->m_sSid			= BoxEventTableSet.m_sSid;
			pNewItem->m_tType			= BoxEventTableSet.m_tType;
			pNewItem->m_sIid			= BoxEventTableSet.m_sIid;
			pNewItem->m_sCount			= BoxEventTableSet.m_sCount;
			pNewItem->m_strText			= BoxEventTableSet.m_strText;
			
			g_arBoxEventTable.Add(pNewItem);
			
			BoxEventTableSet.MoveNext();
		}
	}
	catch(CMemoryException * e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	Check Summon Exception
//
BOOL CServerDlg::CheckSummonException(short sSid)
{
	int i;
	int nCount = sizeof(g_sSummonException) / sizeof(g_sSummonException[0]);

	for(i = 0; i < nCount; i++)
	{
		if(sSid == g_sSummonException[i]) return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	¼­¹ö Á¾·á½Ã À¯Àú Á¤¸®
//
LONG CServerDlg::OnCloseProcess(UINT wParam, LONG lParam)
{
	CloseGuildWhareHouse();
	
	EndDialog(IDOK);
	return 0L;
}


void CServerDlg::OnBnClickedNewAnnounce()//ÖØ¶Á±¬ÂÊ
{
	int number, i;
	char szfilename[256]="drop.ini";
	char tmp[512];
	byte code1,code2;
	FILE *fp;
	char n1[20], n2[20], n3[20], n4[20], n5[20], n6[20];

	fp = fopen(szfilename, "r");
	if (fp==NULL)
	{
		CString szTemp;
		szTemp.Format("Î´ÕÒµ½¹ÖÎïÅäÖÃÎÄ¼þ");
		AfxMessageBox(szTemp);
	}

	fscanf(fp, "ÊýÁ¿ = %s\n", tmp);
	number = atoi(tmp);
	fscanf(fp, "##%s\n", tmp);
	for(i=0;i<number;i++)
	{
		//¿¨ÌØ - 109 - 0 - 60 - 15 - 100 - 18
		//          1    2   3    4    5     6   7     8
		fscanf(fp,"%s - %s - %s - %s - %s - %s - %s\n",tmp,n1,n2,n3,n4,n5,n6);

		code1=atoi(n1);
		code2=atoi(n2);
		strcpy_s(g_DropItem[code1][code2].name,tmp); //¹ÖÎïÃû×Ö
		g_DropItem[code1][code2].code1=code1;   //¹ÖÎï´úÂë	
		g_DropItem[code1][code2].code2=code2;   //¹ÖÎïÑÕÉ«
		g_DropItem[code1][code2].DropNovelity=atoi(n3);//
		g_DropItem[code1][code2].DropLeechdom=atoi(n4);//µôÀ¶
		g_DropItem[code1][code2].money=atoi(n5); //µô¶àÉÙÇ®
		g_DropItem[code1][code2].n=atoi(n6);//¹ÖÎïµÄÎïÆ·ÖÖÀàÊýÁ¿
		
		if(!GetOneNpcItemNewTable(&g_DropItem[code1][code2]))//ÖØ¶Á±©ÂÊ
		{
			CString szTemp;
			szTemp.Format("%sÀà ¹ÖÎïµô±¦¶ÁÈ¡Ê§°Ü",g_DropItem[code1][code2].name);
		//	AfxMessageBox(szTemp);
		}	
	}
	CString szTemp;
    szTemp.Format("±©ÂÊÖØ¶Á³É¹¦");
	AfxMessageBox(szTemp);
	fclose(fp);
}
void CServerDlg::OnBnClickedNewAnnounce2()
{
	
		LoadYeHuoINI();//ÖØÔØini
}