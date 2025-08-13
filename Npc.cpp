// Npc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "USER.h"
#include "Npc.h"

#include "Extern.h"
#include "MAP.h"
#include "BufferEx.h"

#include "Mcommon.h"
#include "scdefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CRITICAL_SECTION m_CS_EventItemLogFileWrite;

//////////////////////////////////////////////////////////////////////
// Fortress Paket Variable
extern CRITICAL_SECTION m_CS_FortressData;
extern CPtrList				RecvFortressData;
extern long nFortressDataCount;
extern struct drop_info g_DropItem[256][4];

//int surround_x[8] = {-2, -1, 0, 1, 2, 1, 0, -1};
//int surround_y[8] = {0, -1, -2, -1, 0, 1, 2, 1};

int surround_x[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int surround_y[8] = {0, -1, -1, -1, 0, 1, 1, 1};

int g_iMoonEvent = 1;

//#define STEP_DELAY			//440


#define UPDATE_EVENT_INVEN_TIME		12

//////////////////////////////////////////////////////////////////////
//	Inline Function
//
inline int CNpc::GetUid(int x, int y )
{
	MAP* pMap = g_zone[m_ZoneIndex];
	return pMap->m_pMap[x][y].m_lUser;
}

inline BOOL CNpc::SetUid(int x, int y, int id)
{
	MAP* pMap = g_zone[m_ZoneIndex];

	if(pMap->m_pMap[x][y].m_bMove != 0) return FALSE;
	if(pMap->m_pMap[x][y].m_lUser != 0 && pMap->m_pMap[x][y].m_lUser != id ) return FALSE;

	pMap->m_pMap[x][y].m_lUser = id;

	return TRUE;
}

BOOL CNpc::SetUidNPC(int x, int y, int id)
{
	MAP* pMap = g_zone[m_ZoneIndex];

	if(pMap->m_pMap[x][y].m_bMove != 0) return FALSE;
	if(pMap->m_pMap[x][y].m_lUser != 0 && pMap->m_pMap[x][y].m_lUser != id ) return FALSE;

	pMap->m_pMap[x][y].m_lUser = id;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpc::CNpc()
{
	m_NpcVirtualState = NPC_STANDING;
	m_NpcState = NPC_LIVE;

	InitTarget();

	m_ItemUserLevel = 0;
//	m_Delay = 0;			// ���� ���·� ���̵Ǳ� ������ �ð�( mili/sec )
//	m_dwLastThreadTime = 0;
	m_Delay = 0;
	m_dwLastThreadTime = GetTickCount();

	m_sClientSpeed = 0;		// Ŭ���̾�Ʈ �ִϸ� ���� �̵� ����
	m_dwStepDelay = 0;		// ���� ��Ŷ������ �ð� ������
	m_tNpcAttType = 0;		// ���� ����
	m_tNpcLongType = 0;		// ���Ÿ�(1), �ٰŸ�(0)
	m_tNpcGroupType = 0;	// ������ �ִ³�(1), ���ִ³�?(0)
//	m_tNpcTraceType = 0;	// ������ ���󰣴�(1), �þ߿��� �������� �׸�(0)

	m_pPath = NULL;
	m_pOrgMap = NULL;

//	m_pMap = NULL;
	m_lMapUsed = 0;			// �� �޸𸮺�ȣ

	m_bFirstLive = TRUE;

	m_tWeaponClass = BRAWL;
	m_dwDelayCriticalDamage = 0;
	m_dwLastAbnormalTime	= GetTickCount();

	::ZeroMemory(m_pMap, sizeof(m_pMap));// ������ ������ �ʱ�ȭ�Ѵ�.

	m_tAbnormalKind = 0;
	m_dwAbnormalTime = 0;

	m_presx = -1;
	m_presy = -1;

	m_lEventNpc = 0;

	m_pGuardStore = NULL;				// ����������� �ش� ������ ������ �´�.
	m_pGuardFortress = NULL;

	m_tRepairDamaged = 0;
	m_tNCircle = NPC_NCIRCLE_DEF_STATE;
	m_lFortressState = 0;
	m_lDamage = 0;

	m_bSummon = FALSE;
	m_sSummonOrgZ = m_sOrgZ;
	m_sSummonOrgX = m_sOrgX;
	m_sSummonOrgY = m_sOrgY;
	m_SummonZoneIndex = m_ZoneIndex;

	m_bSummonDead = FALSE;
	m_lNowSummoning = 0;

	m_lKillUid = -1;
	m_sQuestSay = 0;

	InitSkill();
	InitUserList();
}

CNpc::~CNpc()
{
	ClearPathFindData();

	InitUserList();
}

//////////////////////////////////////////////////////////////////////
//	NPC ��ų������ �ʱ�ȭ �Ѵ�.
//
void CNpc::InitSkill()
{
	for(int i = 0; i < SKILL_NUM; i++)
	{
		m_NpcSkill[i].sSid = 0;
		m_NpcSkill[i].tLevel = 0;
		m_NpcSkill[i].tOnOff = 0;
	}
}

///////////////////////////////////////////////////////////////////////
//	��ã�� �����͸� �����.
//
void CNpc::ClearPathFindData()
{
	::ZeroMemory(m_pMap, sizeof(m_pMap));	// ������ ���� ����
/*	int i;

	if(m_pMap)
	{
		int **tmp = m_pMap;
		
		m_pMap = NULL;

		for(i = 0; i < m_vMapSize.cx; i++)
		{
			delete[] tmp[i];
		}
		delete[] tmp;
	}
*/
}

///////////////////////////////////////////////////////////////////////
// NPC �� ó�� ����ų� �׾��ٰ� ��Ƴ� ���� ó��
//
BOOL CNpc::SetLive(COM* pCom)
{
	NpcTrace(_T("SetLive()"));

	if(m_tRepairDamaged > 0) return FALSE;		 // �������� �ջ� �޾Ҵٸ� �����ɶ����� �״�� �����Ǿ����...
	if(m_pGuardFortress && m_tGuildWar == GUILD_WARRING)
	{
		if(!m_bFirstLive) return FALSE;
	}
	else m_tGuildWar = GUILD_WAR_AFFTER;		// ��� ���� ������� ���ؼ� �ʱ�ȭ�Ѵ�...

	if(m_bSummonDead)							// ��ȯ����� ���� ���ؼ�
	{
		m_ZoneIndex = m_TableZoneIndex;
		m_sCurZ		= m_sOrgZ	= m_sTableOrgZ;
		m_sOrgX		= m_sTableOrgX;
		m_sOrgY		= m_sTableOrgY;
		
		m_pOrgMap = g_zone[m_ZoneIndex]->m_pMap;	// MapInfo ���� ����
		
		m_bSummonDead = FALSE;
	}

	// NPC�� HP, PP �ʱ�ȭ ----------------------//	
	int i = 0, j = 0;
	m_sHP = m_sMaxHP;
	m_sPP = m_sMaxPP;
	NpcDrop=4;
	int iTryLiveCount = 0;

	InitTarget();

	InitUserList();					// Ÿ�������� ����Ʈ�� �ʱ�ȭ.	

	// NPC �ʱ���ġ ���� ------------------------//
	MAP* pMap = g_zone[m_ZoneIndex];
	
	m_nInitMinX = m_sOrgX - m_sMinX;		if(m_nInitMinX < 1) m_nInitMinX = 1; 
	m_nInitMinY = m_sOrgY - m_sMinY;		if(m_nInitMinY < 1) m_nInitMinY = 1; 
	m_nInitMaxX = m_sOrgX + m_sMaxX;		if(m_nInitMaxX >= pMap->m_sizeMap.cx) m_nInitMaxX = pMap->m_sizeMap.cx - 1;
	m_nInitMaxY = m_sOrgY + m_sMaxY;		if(m_nInitMaxY >= pMap->m_sizeMap.cy) m_nInitMaxY = pMap->m_sizeMap.cy - 1;

	CPoint pt;
	CPoint ptTemp;

	int modify_index = 0;
	char modify_send[2048];	

//	if(m_lEventNpc == 0 && m_sEvent == 1000) return TRUE;//@@@@@@@@@@@@@@@@@@@@@@ Test Code(�ӽ÷� ���� ��ȯ�ϱ�����)

	if(m_lEventNpc == 1 && !m_bFirstLive) 
	{
		// �������� NPC ��������... 
		// ���� : ���� ����� ��������ϱ⶧���� ���� ������ ���ʰ� �ö� INFO_DELETE�� ������. 
		::ZeroMemory(modify_send, sizeof(modify_send));

		for(int i = 0; i < NPC_NUM; i++)
		{
			if(g_arEventNpcThread[0]->m_ThreadInfo.pNpc[i] != NULL)
			{
				if(g_arEventNpcThread[0]->m_ThreadInfo.pNpc[i]->m_sNid == m_sNid)
				{
					FillNpcInfo(modify_send, modify_index, INFO_DELETE);
					SendInsight(pCom, modify_send, modify_index);

					g_arEventNpcThread[0]->m_ThreadInfo.pNpc[i] = NULL;
					InterlockedExchange(&m_lEventNpc, (LONG)0);
					InterlockedExchange(&g_arEventNpcThread[0]->m_ThreadInfo.m_lNpcUsed[i], (LONG)0);								
					return TRUE;
				}
			}
		}
		return TRUE;
	}

	if(m_tNpcType != NPCTYPE_MONSTER && m_bFirstLive)//NPCTYPE_DOOR || m_tNpcType == NPCTYPE_GUARD)
	{
		m_nInitX = m_sCurX = m_sOrgX;
		m_nInitY = m_sCurY = m_sOrgY;

		pMap->m_pMap[m_sCurX][m_sCurY].m_lUser = m_sNid + NPC_BAND;

//		TRACE("NPC DOOR %s(nid = %d) - %d %d\n", m_strName, m_sNid, m_sCurX, m_sCurY);
		CPoint temp = ConvertToClient(m_sCurX, m_sCurY);
		TRACE("NPC DOOR %s(nid = %d) - %d %d\n", m_strName, m_sNid, temp.x, temp.y);
	}
	else
	{
		while(1)
		{
			i++;
																						
			if(m_lEventNpc == 1)			// ��ȯ���ϰ�� �ǵ����̸� ó�� ������ ��ǥ��
			{
				if(pMap->m_pMap[m_sOrgX][m_sOrgY].m_bMove == 0)	
				{ 
					pt.x = m_sOrgX; 
					pt.y = m_sOrgY; 
					
					m_nInitX = m_sCurX = pt.x;
					m_nInitY = m_sCurY = pt.y;
					
					//ptTemp = ConvertToClient(m_sCurX, m_sCurY);
					break;
				}
				else
				{
					pt = FindNearRandomPoint(m_sOrgX, m_sOrgY);
					if(pt.x <= 0 || pt.y <= 0) 
					{
						pt.x = myrand(m_nInitMinX, m_nInitMaxX);
						pt.y = myrand(m_nInitMinY, m_nInitMaxY);
					}
				}
			}
			else
			{
				pt.x = myrand(m_nInitMinX, m_nInitMaxX);
				pt.y = myrand(m_nInitMinY, m_nInitMaxY);

				// Test Code By Youn Gyu 02-08-13 (����� �ֺ� 25���θ� ����)
				if( m_sCurZ != 1 && m_sCurZ != 1005 ) // �ϴ� ������ ã�´�.
				{
					if(m_tNpcType == NPCTYPE_MONSTER)
					{
						if( !CheckUserForNpc_Live(pt.x, pt.y) )
						{
							iTryLiveCount += 1;
							if(iTryLiveCount >= 20) return FALSE;
							else continue;
						}
					}
				}
				//TRACE("MONSTER %s(nid = %d) - %d %d\n", m_strName, m_sNid, m_sCurX, m_sCurY);
			}
			
			if(pt.x < 0 || pt.x >= pMap->m_sizeMap.cx) continue;
			if(pt.y < 0 || pt.y >= pMap->m_sizeMap.cy) continue;

			if(pMap->m_pMap[pt.x][pt.y].m_bMove != 0 || pMap->m_pMap[pt.x][pt.y].m_lUser != 0)
			{
				if(i >= 100) 
				{
					m_nInitX = m_sCurX = m_sOrgX;
					m_nInitY = m_sCurY = m_sOrgY;
//					TRACE("sid = %d, loop = %d My standing point is invalid x = %d, y = %d\n", m_sSid, i, pt.x, pt.y);
					InterlockedIncrement(&g_CurrentNPCError);
					return FALSE;
//					break;
					
/*					DeleteNPC();// ������ �ƴ�...
					TRACE("sid = %d, loop = %d My standing point is invalid x = %d, y = %d\n", m_sSid, i, pt.x, pt.y);
					return FALSE;
*/					
				}
				continue;
			}

			m_nInitX = m_sCurX = pt.x;
			m_nInitY = m_sCurY = pt.y;

//			ptTemp = ConvertToClient(m_sCurX, m_sCurY);
			break;
		}
	}

	SetUid(m_sCurX, m_sCurY, m_sNid + NPC_BAND);

	if(m_sDimension > 0) SetMapTypeBeforeGuildWar(pCom);		// ���� �����Ѵ�.

	// �����̻� ���� �ʱ�ȭ
	m_dwLastAbnormalTime	= GetTickCount();
	m_tAbnormalKind = 0;
	m_dwAbnormalTime = 0;

	// ���������� NPC HP�� ������ �ȵǾ� ������
//	if(m_pGuardFortress) SetFortressState();
	
	if(m_bFirstLive)	// NPC �� ó�� ��Ƴ��� ���
	{
		NpcTypeParser();
		m_tWeaponClass = GetWeaponClass();
		m_bFirstLive = FALSE;

		InterlockedIncrement(&g_CurrentNPC);
	}

	// ���� ���������� ���� ������ uid �ʱ�ȭ
	m_lKillUid = -1;

	// Test Code
//	CString strTemp = m_strName;
//	if(strTemp == "��" || strTemp == "�׷���Ʈ��ǲ")m_sHP = 1;

	// �������� NPC ��������...
	modify_index = 0;
	::ZeroMemory(modify_send, sizeof(modify_send));
	FillNpcInfo(modify_send, modify_index, INFO_MODIFY);

	SendInsight(pCom, modify_send, modify_index);

	m_presx = -1;
	m_presy = -1;

	SightRecalc(pCom);

	return TRUE;
}


///////////////////////////////////////////////////////////////////
//	NPC �⺻���� ������ �з�, �����Ѵ�.
//
void CNpc::NpcTypeParser()
{
	MYSHORT sAI;

	BYTE upTemp = 0;			// ���� 8��Ʈ
	BYTE dwTemp = 0;			// ���� 8��Ʈ

	sAI.i = (short)m_sAI;

	upTemp = sAI.b[0];
	dwTemp = sAI.b[1];
//	temp = m_sAI;//m_byAI

	m_tNpcAttType = upTemp >> 7;

	upTemp = upTemp << 1;
	m_tNpcLongType = upTemp >> 7;

	upTemp = upTemp << 1;
	m_tNpcGroupType = upTemp >> 7;

	m_iNormalATRatio = m_byIronSkin;
	m_iSpecialATRatio = m_byReAttack;
	m_iMagicATRatio = m_bySubAttack;

	m_tSPATRange = m_byWildShot;

/*
	switch( (int)m_byVitalC )
	{
	case	0:	// �Ϲݸ�
		m_bCanNormalAT = TRUE;
		m_bCanMagicAT = FALSE;
		m_bCanSPAT = FALSE;
		break;

	case	1:	// ������
		m_bCanNormalAT = FALSE;
		m_bCanMagicAT = TRUE;
		m_bCanSPAT = FALSE;
		break;

	case	2:	// �Ϲ�, Ư��
		m_bCanNormalAT = TRUE;
		m_bCanMagicAT = FALSE;
		m_bCanSPAT = TRUE;

		m_tSPATRange = m_byWildShot;
		m_tSPATAI = m_byExcitedRate;
		break;

	case	3:	// ����, Ư��
		m_bCanNormalAT = FALSE;
		m_bCanMagicAT = TRUE;
		m_bCanSPAT = TRUE;

		m_tSPATRange = m_byWildShot;
		m_tSPATAI = m_byExcitedRate;
		break;

	case	4:	// �Ϲ�, ����
		m_bCanNormalAT = TRUE;
		m_bCanMagicAT = FALSE;
		m_bCanSPAT = TRUE;
		break;

	case	5:	// Ư����
		m_bCanNormalAT = FALSE;
		m_bCanMagicAT = FALSE;
		m_bCanSPAT = TRUE;

		m_tSPATRange = m_byWildShot;
		m_tSPATAI = m_byExcitedRate;
		break;

	case	6:	// �Ϲ�, ����, Ư��
		m_bCanNormalAT = TRUE;
		m_bCanMagicAT = TRUE;
		m_bCanSPAT = TRUE;

		m_tSPATRange = m_byWildShot;
		m_tSPATAI = m_byExcitedRate;
		break;

	default:
		m_bCanNormalAT = TRUE;
		m_bCanMagicAT = FALSE;
		m_bCanSPAT = FALSE;
		break;

	}
*/
}

///////////////////////////////////////////////////////////////////
//	NPC �ֺ��� ���� ã�´�.
//
BOOL CNpc::FindEnemy(COM *pCom)
{
	BOOL bSearch = FALSE;

	if(m_tNpcType == NPCTYPE_NPC || m_tNpcType == NPCTYPE_DOOR || m_tNpcType == NPCTYPE_GUILD_DOOR) return FALSE;
	if(m_tNpcType == NPCTYPE_GUILD_NPC || m_tNpcType == NPCTYPE_GUILD_MARK) return FALSE;

	if(m_byAX == 0 && m_byAZ == 0 ) return FALSE;		// ���� ���ݷ��� ������ �������� �ʴ´�
	if(m_bySearchRange == 0) return FALSE;
	if(m_tNpcType == NPCTYPE_GUARD) bSearch = TRUE;
	if(m_tNpcType == NPCTYPE_GUILD_GUARD)
	{
		if(m_pGuardFortress && m_tRepairDamaged == NPC_DEAD_REPAIR_STATE) return FALSE;
		// ���������� �ش� ������ HP�� 0�̸� ���� �Ҵ��� 
		 bSearch = TRUE;
	}

	if(!bSearch && !m_tNpcAttType && m_Target.id < 0 )
	{
		return FALSE;
	}
	else
	{
//		if( (GetTickCount() - m_dLastFind) < (DWORD)1000 )
		if( (GetTickCount() - m_dLastFind) < (DWORD)2000 )
		{
			return FALSE;
		}
	}

	m_dLastFind = GetTickCount();

//	if(m_Target.id != -1) return TRUE;

	int min_x, min_y, max_x, max_y;

	min_x = m_sCurX - m_bySearchRange;		if( min_x < 0 ) min_x = 0;
	min_y = m_sCurY - m_bySearchRange;		if( min_y < 0 ) min_y = 0;
	max_x = m_sCurX + m_bySearchRange;
	max_y = m_sCurY + m_bySearchRange;

	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 2;
	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 2;

	int ix, iy;
	int target_uid;
	int uid;
	int rank = 0;

	USER *pUser = NULL;
	CNpc *pNpc = NULL;

	int tempLevel = 0, oldLevel = 1000;

	for(ix = min_x; ix <= max_x; ix++)
	{
		for(iy = min_y; iy <= max_y; iy++)
		{
			target_uid = m_pOrgMap[ix][iy].m_lUser;

			if( target_uid >= USER_BAND && target_uid < NPC_BAND )
			{
				uid = target_uid - USER_BAND;

				pUser = GetUser(pCom, uid);
				if( pUser != NULL && pUser->m_bLive == USER_LIVE)
				{
					if( ix != pUser->m_curx || iy != pUser->m_cury )
					{
						continue;
					}

					if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD)
					{
						//rank = DEATH_RANK - CITY_RANK_INTERVAL;
						if(pUser->m_sKillCount > 100 || pUser->m_bPkStatus)
						{
							m_Target.id	= target_uid;
							m_Target.failCount = 0;
							m_Target.x	= ix;
							m_Target.y	= iy;							
							return TRUE;
						}
					}
																	// ������Ҷ��� �������� ������ ����
					if(m_tNpcType == NPCTYPE_GUILD_GUARD)	
					{
						if(m_tGuildWar == GUILD_WARRING)
						{
							//if(pUser->m_dwGuild == m_pGuardStore->m_iGuildSid) continue;
							if(m_pGuardStore) 
							{
								if(pUser->m_dwGuild == m_pGuardStore->m_iGuildSid) continue;
							}
							else if(m_pGuardFortress)
							{
								if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid) continue;
							}

							m_Target.id	= target_uid;
							m_Target.failCount = 0;
							m_Target.x	= ix;
							m_Target.y	= iy;
							return TRUE;
						}
					}

					if(pUser->m_tIsOP == 1) continue;				// ����̸� ����...^^
					if(pUser->m_bPShopOpen == TRUE) continue;		// User has personal shop
//					if(pUser->m_dwHideTime > 0) continue;			// ���� ���¸� ���õȴ�.
					if(pUser->m_bSessionOnline == true)	continue;
					//�İ���...
					if(!m_tNpcAttType)		// �� ������ ���� ã�´�.
					{
						if(IsDamagedUserList(pUser) || (m_tNpcGroupType && m_Target.id == target_uid))
						{
							m_Target.id	= target_uid;
							m_Target.failCount = 0;
							m_Target.x	= ix;
							m_Target.y	= iy;
							return TRUE;
						}
					}
					else	// ������...
					{						
						if(IsSurround(ix, iy) == TRUE) continue;	//�ѷ� �׿� ������ �����Ѵ�.(���Ÿ�, �ٰŸ� ����)

						USER *pTUser;

						pTUser = pCom->GetUserUid(uid);
						if ( pTUser == NULL ) continue;

						tempLevel = pTUser->m_sLevel;

						if(tempLevel <= oldLevel) 
						{
							oldLevel = tempLevel;									
							m_Target.id	= target_uid;
							m_Target.failCount = 0;
							m_Target.x	= ix;
							m_Target.y	= iy;
							return TRUE;
						}
					}
				}
			}
		}
	}

	InitUserList();		// �ƹ��� �����Ƿ� ����Ʈ�� �����ϴ� ������ �ʱ�ȭ�Ѵ�.
	InitTarget();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	�ֺ��� ���� ������ ������ �ִ��� �˾ƺ���
//
BOOL CNpc::IsDamagedUserList(USER *pUser)
{
//	int count = m_arDamagedUserList.GetSize();
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;

	int sLen = strlen(pUser->m_strUserID);

	if(sLen < 0 || sLen > CHAR_NAME_LENGTH) return FALSE;

	for(int i = 0; i < NPC_HAVE_USER_LIST; i++)
	{
		if(strcmp(m_DamagedUserList[i].strUserID, pUser->m_strUserID) == 0) return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	Target �� NPC �� Path Finding�� �����Ѵ�.
//
BOOL CNpc::GetTargetPath(COM* pCom)
{
	USER* pUser = GetUser(pCom, m_Target.id - USER_BAND);
	if(pUser == NULL)
	{
		InitTarget();
		return FALSE;
	}
	if(pUser->m_sHP <= 0 || pUser->m_state != STATE_GAMESTARTED || pUser->m_bLive == FALSE)
	{
		InitTarget();
		return FALSE;
	}
/*	if(strcmp(m_Target.szName, pUser->m_strUserID) != 0)
	{
		InitTarget();
		return FALSE;
	}
*/
	int iTempRange = m_bySearchRange;				// �Ͻ������� �����Ѵ�.
//	if(m_arDamagedUserList.GetSize()) iTempRange *= 2;	// ���ݹ��� ���¸� ã�� ���� ����.
	if(IsDamagedUserList(pUser)) iTempRange *= 2;	// ���ݹ��� ���¸� ã�� ���� ����.
	else iTempRange += 4;
	
	int min_x = m_sCurX - iTempRange;	if(min_x < 0) min_x = 0;
	int min_y = m_sCurY - iTempRange;	if(min_y < 0) min_y = 0;
	int max_x = m_sCurX + iTempRange;	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
	int max_y = m_sCurY + iTempRange;	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;

	// ��ǥ���� Search Range�� ����� �ʴ��� �˻�
	CRect r = CRect(min_x, min_y, max_x+1, max_y+1);
	if(r.PtInRect(CPoint(pUser->m_curx, pUser->m_cury)) == FALSE) return FALSE;

	// Run Path Find ---------------------------------------------//
	CPoint start, end;
	start.x = m_sCurX - min_x;
	start.y = m_sCurY - min_y;
	end.x = pUser->m_curx - min_x;
	end.y = pUser->m_cury - min_y;

	m_ptDest.x = m_Target.x;
	m_ptDest.y = m_Target.y;

	m_min_x = min_x;
	m_min_y = min_y;
	m_max_x = max_x;
	m_max_y = max_y;

	return PathFind(start, end);
}

////////////////////////////////////////////////////////////////////////////////
//	NPC �� Path Find �Ϸ��� ������ ���� ���� �˰������ �׻� �̵��Ұ� ��ǥ�̹Ƿ�
//	������ ��ǥ�� ����
//	
//	## ����(2000-12-12) �н����ε� �˰������� �����Ǿ� �� �Լ��� �ʿ���� ##
BOOL CNpc::GetLastPoint(int sx, int sy, int& ex, int& ey)
{
	int i;
	int x = 0, y = 0;
	int nx[] = {-1, 0, 1, 1, 1, 0, -1, -1};
	int ny[] = {-1, -1, -1, 0, 1, 1, 1, 0};
	
	BOOL bSearchDest = FALSE;
	MAP* pMap = g_zone[m_ZoneIndex];
	for(i = 0; i < sizeof(nx)/sizeof(nx[0]); i++)
	{
		x = ex + nx[i]; 
		if(x >= pMap->m_sizeMap.cx) x--;
		if(x < 0) x = 0;

		y = ey + ny[i];
		if(y >= pMap->m_sizeMap.cy) y--;
		if(y < 0) y = 0;
		
		if(m_pOrgMap[x][y].m_bMove == 0 && m_pOrgMap[x][y].m_lUser == 0) 
		{
			ex = x;
			ey = y;
			bSearchDest = TRUE;
			break;
		}
	}

	if (bSearchDest) return TRUE;

	int nSearchSize = max(abs(sx - ex), abs(sy - ey));
//	ASSERT(nSearchSize);
	
	for (i = nSearchSize; i > 0; i--) 
	{
		x = sx + (ex - sx) * i / nSearchSize;
		y = sy + (ey - sy) * i / nSearchSize;
		
		if ((x + y) % 2 != 0) y++;	//����

		if(m_pOrgMap[x][y].m_bMove == 0 && m_pOrgMap[x][y].m_lUser == 0) 
		{
			ex = x;
			ey = y;
			bSearchDest = TRUE;
			break;
		}
	}

	if (!bSearchDest) return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//	NPC�� Target ���� �Ÿ��� ���� �������� ������ �Ǵ�
//
BOOL CNpc::IsCloseTarget(COM* pCom, int nRange)
{
	// Ȥ�ó� ����� NPC�� �����ϰ� �Ǹ� Ÿ���� ������ �ƴ϶� NPC�� �� �� �ִ�.
	USER* pUser = GetUser(pCom, m_Target.id - USER_BAND);
	if(pUser == NULL) 
	{
		InitTarget();
		return FALSE;
	}
	if(pUser->m_sHP <= 0 || pUser->m_state != STATE_GAMESTARTED || pUser->m_bLive == FALSE)
	{
		InitTarget();
		return FALSE;
	}

	CPoint ptUser	= ConvertToClient(pUser->m_curx, pUser->m_cury);
	CPoint ptNpc	= ConvertToClient(m_sCurX, m_sCurY);

	//^^ �Ҹ������� �Ÿ��˻��� �ƴ϶� �� �˻����� �Ÿ���� �� �߸��� ���� ��ǥ�� üũ�ϰ� ����
	int dx = abs(ptUser.x - ptNpc.x);
	int dy = abs(ptUser.y - ptNpc.y);
	int max_dist = __max(dx, dy);
	
	if(max_dist > nRange * 2) return FALSE; // Ŭ����Ʈ ��ǥ�� 2���̰� �Ѽ��̹Ƿ� *2�� �Ѵ�.

	m_Target.x = pUser->m_curx;
	m_Target.y = pUser->m_cury;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//	NPC�� Target ���� �Ÿ��� ���� �������� ������ �Ǵ�
//
BOOL CNpc::IsCloseTarget(USER *pUser, int nRange)
{
	if(pUser == NULL)
	{
		return FALSE;
	}
	if(pUser->m_sHP <= 0 || pUser->m_state != STATE_GAMESTARTED || pUser->m_bLive == FALSE)
	{
		return FALSE;
	}

	CPoint ptUser	= ConvertToClient(pUser->m_curx, pUser->m_cury);
	CPoint ptNpc	= ConvertToClient(m_sCurX, m_sCurY);

	//^^ �Ҹ������� �Ÿ��˻��� �ƴ϶� �� �˻����� �Ÿ���� �� �߸��� ���� ��ǥ�� üũ�ϰ� ����
	int dx = abs(pUser->m_curx - m_sCurX);
	int dy = abs(pUser->m_cury - m_sCurY);
	int max_dist = __max(dx, dy);
	
	if(max_dist > nRange * 2) return FALSE;

	InitTarget();
	m_Target.id = pUser->m_uid + USER_BAND;
	m_Target.x = pUser->m_curx;
	m_Target.y = pUser->m_cury;

/*	if(pUser->m_strUserID != NULL)
	{
		m_Target.nLen = strlen(pUser->m_strUserID);

		if(m_Target.nLen <= CHAR_NAME_LENGTH) strncpy(m_Target.szName, pUser->m_strUserID, m_Target.nLen);
		else								  ::ZeroMemory(m_Target.szName, sizeof(m_Target.szName));
	}
*/	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	Path Find �� ã������ �� �̵� �ߴ��� �Ǵ�
//
BOOL CNpc::IsMovingEnd()
{
	if( m_bRandMove )		// 8���� ���� �������϶�
	{
		if( m_arRandMove.GetSize() ) return FALSE;

		return TRUE;
	}

	if(!m_pPath) return TRUE;

	int min_x = m_min_x;
	int min_y = m_min_y;

	if((m_sCurX - min_x) == m_vEndPoint.x && (m_sCurY - min_y) == m_vEndPoint.y) return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
//	�ش� uid�� USER* �� ��´�.
//
USER* CNpc::GetUser(COM* pCom, int uid)
{
	if(!pCom) return NULL;
	//if(uid < 0 || uid >= MAX_USER) return NULL;

	return pCom->GetUserUid(uid);
}

/////////////////////////////////////////////////////////////////////////////////
//	Target �� ��ġ�� �ٽ� ��ã�⸦ �� ������ ���ߴ��� �Ǵ�
//	ȥ������ �����̻� ���̿����̳� ��ų���� �����Ǹ� �� �Լ��� �̿��ϸ� �� ��
//
BOOL CNpc::IsChangePath(COM* pCom, int nStep)
{
	if(!m_pPath) return TRUE;

	CPoint pt;
	GetTargetPos(pCom, pt);
	NODE* pTemp = m_pPath;

	CPoint ptPath[2];
	while(1)
	{
		if(pTemp == NULL) break;

		if(pTemp->Parent) 
		{
			ptPath[0].x = m_min_x + pTemp->x;
			ptPath[0].y = m_min_y + pTemp->y;

			pTemp = pTemp->Parent;			
		}
		else
		{			
			ptPath[1].x = m_min_x + pTemp->x;
			ptPath[1].y = m_min_y + pTemp->y;

			break;
		}
	}

	for(int i = 0; i < 2; i++)
	{
		if(abs(ptPath[i].x - pt.x) <= m_byRange && abs(ptPath[i].y - pt.y) <= m_byRange) return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
//	Target �� ���� ��ġ�� ��´�.
//
BOOL CNpc::GetTargetPos(COM *pCom, CPoint &pt)
{
	USER* pUser = GetUser(pCom, m_Target.id - USER_BAND);

	if(!pUser) return FALSE;

	pt.x = pUser->m_curx;
	pt.y = pUser->m_cury;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
//	Target �� NPC ���� ��ã�⸦ �ٽ��Ѵ�.
//
BOOL CNpc::ResetPath(COM* pCom)
{
	CPoint pt;
	GetTargetPos(pCom, pt);

	m_Target.x = pt.x;
	m_Target.y = pt.y;

	return GetTargetPath(pCom);
}

/////////////////////////////////////////////////////////////////////////////////
//	Step �� ��ŭ Ÿ���� ���� �̵��Ѵ�.
//
BOOL CNpc::StepMove(COM* pCom, int nStep)
{
//	if(m_tNpcType == NPCTYPE_GUILD_DOOR)	return FALSE;	// �̵����ϰ�...

	if(!m_pPath && !m_bRandMove) return FALSE;
	if(m_NpcState != NPC_MOVING && m_NpcState != NPC_TRACING && m_NpcState != NPC_BACK) return FALSE;
	
	int min_x;
	int min_y;
	int will_x;
	int will_y;

	CPoint ptPre;

	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return FALSE;
	if( !pMap->m_pMap ) return FALSE;

	for(int i = 0; i < nStep; i++)
	{
		if( m_bRandMove )
		{
			if( !m_arRandMove.GetSize() ) return FALSE;

			min_x = m_min_x;
			min_y = m_min_y;

			will_x = min_x + m_arRandMove[0].x;
			will_y = min_y + m_arRandMove[0].y;

			m_arRandMove.RemoveAt( 0 );

			if( will_x >= pMap->m_sizeMap.cx || will_x < 0 || will_y >= pMap->m_sizeMap.cy || will_y < 0 )
			{
				m_vEndPoint.x = m_sCurX - min_x;
				m_vEndPoint.y = m_sCurY - min_y;

				return FALSE;
			}

			if( pMap->m_pMap[will_x][will_y].m_bMove != 0 || pMap->m_pMap[will_x][will_y].m_lUser != 0 )
			{
				m_vEndPoint.x = m_sCurX - min_x;
				m_vEndPoint.y = m_sCurY - min_y;

				return FALSE;
			}

			ptPre.x = m_sCurX;
			ptPre.y = m_sCurY;

			m_sCurX = will_x;
			m_sCurY = will_y;

			// �þ� ����...
			SightRecalc( pCom );

			break;
		}
		else if(m_pPath->Parent)
		{
			m_pPath = m_pPath->Parent;

			min_x = m_min_x;
			min_y = m_min_y;

			will_x = min_x + m_pPath->x;
			will_y = min_y + m_pPath->y;

			if(will_x >= pMap->m_sizeMap.cx || will_x < 0 || will_y >= pMap->m_sizeMap.cy || will_y < 0) 
			{
				m_vEndPoint.x = m_sCurX - min_x;
				m_vEndPoint.y = m_sCurY - min_y;
				return FALSE;
			}
			
			if(pMap->m_pMap[will_x][will_y].m_bMove != 0 || pMap->m_pMap[will_x][will_y].m_lUser != 0)
			{
				m_vEndPoint.x = m_sCurX - min_x;
				m_vEndPoint.y = m_sCurY - min_y;
				return FALSE;
			}

			ptPre.x = m_sCurX;
			ptPre.y = m_sCurY;

			m_sCurX = will_x;
			m_sCurY = will_y;

			//�þ� ����...
			SightRecalc(pCom);

			break;
		}
		
		return FALSE;
	}

	if(SetUid(m_sCurX, m_sCurY, m_sNid + NPC_BAND))
	{
		pMap->m_pMap[ptPre.x][ptPre.y].m_lUser = 0;
		return TRUE;
	}
	else return FALSE;

//	return SetUid(m_sCurX, m_sCurY, m_sNid + NPC_BAND);
}

//////////////////////////////////////////////////////////////////////////////
//	Target �� ���� ���� ó��
//
int CNpc::Attack(COM *pCom)
{
	if(!pCom) return 10000;

	int ret = 0;
	int nStandingTime = m_sStandTime;

	// �ѱ�迭 �϶��� Ÿ�ٰ��� �Ÿ������ �޸��ؾ� �Ѵ�.
//	if(m_tNpcType != NPCTYPE_GUARD && m_tNpcType != NPCTYPE_GUILD_GUARD)// ����� �ƴϸ� �þ� ���
//	{
	if(IsCloseTarget(pCom, m_byRange) == FALSE)// Check Code (���� ������� ���鿡�� ���� �ڵ�)
	{
		if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD)
		{
			m_NpcState = NPC_STANDING;
			return 0;
		}
		m_NpcState = NPC_TRACING;			// �����ϰ� �������� ������ ���� �������(������ ���� ������) 
		return 0;							// IsCloseTarget()�� ���� x, y���� �����ϰ� Delay = 0���� ��
	}
//	}

	short	sTempHP		= 0;
	CNpc*	pNpc		= NULL;	
	USER*	pUser		= NULL;

	CByteArray	arSkillAction1, arSkillAction2;

	int		nHit = 0;
	int		nAvoid = 0;

	BOOL	bIsHit = FALSE;
	BOOL	bIsCritical = FALSE;

	int		nDamage		= 0;
	int		nDefense	= 0;

	int		iRandom = 0;
//	int		iDefenseDex = 0;
//	double	determine	= 0;
	int		determine = 0;
	int		iDexHitRate = 0, iLevelHitRate = 0;

	int nID = m_Target.id;					// Target �� ���Ѵ�.

	// ���߿��� �Ǵ� ���� �ʱ�ȭ
	bIsHit = FALSE;	
	

	

	// ȸ�ǰ�/��������/������ ��� -----------------------------------------//
	if(nID >= USER_BAND && nID < NPC_BAND)	// Target �� User �� ���
	{
		pUser = GetUser(pCom, nID - USER_BAND);
		
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)// User �� Invalid �� ���
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}

		if(pUser->m_bLive == USER_DEAD)			// User �� �̹� �������
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}

		if(pUser->m_dwNoDamageTime != 0)		// User �� ����Ÿ�ӿ� �������
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}
		
		if(pUser->m_bPShopOpen == TRUE)			// User has personal shop 
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}
		if(pUser->m_bSessionOnline == true)			// ���߲����ֹ���
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}

/*	������ �̹� üũ ��
		if(pUser->m_state == STATE_DISCONNECTED)
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}
*/
		if(m_tNpcType == NPCTYPE_GUILD_GUARD)	
		{
			if(m_tGuildWar == GUILD_WARRING && pUser->m_dwGuild > 0)
			{
				if(m_pGuardStore) 
				{
					if(pUser->m_dwGuild == m_pGuardStore->m_iGuildSid) return nStandingTime;
				}
				else if(m_pGuardFortress)
				{
					if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid) return nStandingTime;
				}
			}
		}
												// ���� �����̸� ����� ������ ��� ���� �ش�			
//		if(m_tNpcType != NPCTYPE_GUARD && pUser->m_dwHideTime > 0)
//		{
//			InitTarget();
//			m_NpcState = NPC_MOVING;
//			return nStandingTime;
//		}

		if(pUser->m_tIsOP == 1)
		{
			InitTarget();
			m_NpcState = NPC_MOVING;
			return nStandingTime;
		}

		// ȸ�ǰ� ��� 
		nAvoid = pUser->GetAvoid(); //BOSS���Ի�
		
		// ���߿��� �Ǵ�
		iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5); 
		
		iDexHitRate = (int)( 30.0 * ( (double)m_sDEX/(m_sDEX + pUser->m_sMagicDEX) ) + 15.0 );
		iLevelHitRate = (int)( 70.0 * ( (double)m_byClassLevel/(pUser->m_sLevel + m_byClassLevel) ) + 15.0);
        
		determine = iDexHitRate + iLevelHitRate - (nAvoid+pUser->m_Avoid);
/*
		iDefenseDex = pUser->m_sMagicDEX;
		if(iDefenseDex < 0) iDefenseDex = 0; //��� �ڵ�

		determine = 200 * ((double)m_sDEX / (m_sDEX + iDefenseDex)) * ((double)m_byClassLevel / (m_byClassLevel + pUser->m_sLevel));
		determine = determine - nAvoid;
*/
		if(determine < ATTACK_MIN) determine = ATTACK_MIN;			// �ּ� 20
		else if(determine > ATTACK_MAX) determine = ATTACK_MAX;		// �ִ�

		if(iRandom < determine)	bIsHit = TRUE;		// ����
		

		// ���� �̽�
		if(bIsHit == FALSE)					
		{
			SendAttackMiss(pCom, nID);
			return m_sAttackDelay;;
		}

		// �����̸� //Damage ó�� ----------------------------------------------------------------//

		nDamage = GetFinalDamage(pUser);	// ���� �����
		// nDamage += (int)(nDamage * 0.8);//�ս���
		//�����˺������������˺�����
		//nDamage=nDamage-(pUser->m_DynamicMagicItem[5]+pUser->m_DynamicMagicItem[6]);
	//	nDamage=nDamage-(pUser->m_DynamicUserData[MAGIC_PHY_ATTACK_DOWN]+pUser->m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]);
		nDamage=nDamage-(pUser->m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+pUser->m_DynamicUserData[MAGIC_PHY_ATTACK_DOWN]);////���������˺�
		if(nDamage < 0) nDamage = 15;
		if(pUser->m_tAbnormalKind == ABNORMAL_BYTE_COLD) nDamage += 10;		// ������� �ñ� �̻��̸� ������ �߰�

		//������ڻ�����30%�ļ����ȹ�������
		if(pUser->m_tHuFaType &&pUser->m_nHuFaHP>0)
		{
			nID = nID+USER_HUFA_BAND;
			//�˺�����
			pUser->SendDamageNum(0,nID,(short)nDamage);

			SendAttackSuccess(pCom, nID, bIsCritical, pUser->m_nHuFaHP, pUser->m_nHuFaMaxHP);//yskang 0.3
			if(nDamage > 0)
				pUser->SetHuFaDamage(nDamage);
			if(pUser->m_nHuFaHP>0)
			{
				pUser->HuFaAttack(m_sNid+NPC_BAND);
			}
			return m_sAttackDelay;
		}

		if(nDamage > 0) pUser->SetDamage(nDamage);

		// ����� ������ ����
		pUser->SendDamagedItem(nDamage);
		pUser->SendDamageNum(0,pUser->m_uid+USER_BAND,nDamage);
//		if(pUser->m_bLive == USER_DEAD || pUser->m_sHP <= 0)//@@@ ���߿� ��ħ
		if(pUser->m_lDeadUsed == 1)
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			m_Delay = m_sStandTime;

			if(m_NpcVirtualState == NPC_STANDING)
			{
				if(m_sPid == 179) pUser->GetLevelDownExp(FALSE, -1, TRUE,m_strName);//���� �����ϰ�� ����ġ 1%����
				else				pUser->GetLevelDownExp(FALSE, -1, FALSE,m_strName);// ����ġ�� �׿� ��ȭ���� �ݿ��Ѵ�.
			}
			if(m_tNpcType == NPCTYPE_GUARD) pUser->SendCityRank(1);				// ��񺴿��� ������ PK ī��Ʈ 1 ����
																				// ���� ����� �븻 ���ݸ� �ϹǷ� �̰����� �߰� 
																				// Add by JJS 2002.05.24
		}

		//yskang 0.3 SendAttackSuccess(pCom, nID, arSkillAction1, arSkillAction2, pUser->m_sHP, pUser->m_sMagicMaxHP);
		SendAttackSuccess(pCom, nID, bIsCritical, pUser->m_sHP, pUser->m_sMagicMaxHP);//yskang 0.3
	}

	return m_sAttackDelay;
}

CNpc* CNpc::GetNpc(int nid)
{
	CNpc* pNpc = NULL;

	int nSize = g_arNpc.GetSize();

	if(nid < 0 || nid >= nSize) return NULL;

	for( int i = 0; i < g_arNpc.GetSize(); i++)
	{
		pNpc = g_arNpc[i];
		if( !pNpc ) continue;

		if( pNpc->m_sNid == nid )
		{
			return pNpc;
		}
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////
//	NPC �� ���ݷ��� ���´�.
//
int CNpc::GetAttack()
{
	int X = m_byAX;
	int Y = m_byAZ;

	return XdY(X, Y);
}

////////////////////////////////////////////////////////////////////////////
//	NPC �� ������ ���´�.
//
int CNpc::GetDefense()
{
	return m_iDefense;
}

/////////////////////////////////////////////////////////////////////////////
//	Damage ���, ���� m_sHP �� 0 �����̸� ���ó��
//
BOOL CNpc::SetDamage(int nDamage)
{
	if(m_NpcState == NPC_DEAD) return TRUE;
	if(m_sHP <= 0) return TRUE;
	if(nDamage <= 0) return TRUE;

	m_sHP -= nDamage;

	if( m_sHP <= 0 )
	{
		m_sHP = 0;
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//	NPC ���ó��
//
void CNpc::Dead()
{
	long lNpcUid = m_sNid + NPC_BAND;
	if(m_pOrgMap[m_sCurX][m_sCurY].m_lUser == lNpcUid)
	{
		::InterlockedExchange(&m_pOrgMap[m_sCurX][m_sCurY].m_lUser, (LONG)0);
	}
	
	m_sHP = 0;
	m_NpcState = NPC_DEAD;
	
	if(m_bSummon)
	{
		m_bSummonDead = TRUE;
		m_bSummon = FALSE;
	}
	
	if(m_NpcVirtualState == NPC_MOVING)	m_NpcVirtualState = NPC_WAIT;
	
	m_Delay = m_sRegenTime;
	m_bFirstLive = FALSE;
}

//////////////////////////////////////////////////////////////////////////////
//	NPC ���ó���� ����ġ �й踦 ����Ѵ�.(�Ϲ� ������ ���� ����ڱ���)
//
void CNpc::SendExpToUserList(COM *pCom)
{
	int i;
	int exp = 0;//, eventExp = 0;
	int totalDamage = 0;
	int firstDamage = 0;
	DWORD plusExp = 0;
	int MaxDamage=0;
	USER *KeypUser=NULL;

	if(m_NpcVirtualState == NPC_WAIT) return;		// ����������� ����ġ ����.
	if(m_tNpcType >= NPCTYPE_GUILD_NPC) return;


	if(InterlockedCompareExchange((long*)&m_lDamage, (long)1, (long)0) == (long)0){
		if(NpcDrop<=0) {
			InterlockedExchange(&m_lDamage, (LONG)0); 
			return;
		}else{
			NpcDrop=NpcDrop-1;
		}

		InterlockedExchange(&m_lDamage, (LONG)0); 
	}
//	if(m_tNpcType == NPCTYPE_GUILD_NPC || m_tNpcType == NPCTYPE_GUILD_DOOR) return;
//	if(m_tNpcType == NPCTYPE_GUILD_GUARD) return;	// ��������� ���� ����� ����ġ�� ���ش�.
//	SYSTEMTIME gTime;
//	GetLocalTime(&gTime);	

	USER *pUser = NULL;

	IsUserInSight(pCom);					// ���� �������ȿ� �ִ���?(���� �������� �� ȭ�� : �÷��� ����)

	if(m_DamagedUserList[0].iUid >= 0 && m_DamagedUserList[0].nDamage > 0)	// ù��°�� ������ �ִٸ� 2�� 
	{
		MaxDamage=firstDamage = m_DamagedUserList[0].nDamage;
		m_DamagedUserList[0].nDamage = m_DamagedUserList[0].nDamage * 2;
		
	}
														
	for(i = 0; i < NPC_HAVE_USER_LIST; i++)				// �ϴ� ����Ʈ�� �˻��Ѵ�.
	{												
		if(m_DamagedUserList[i].iUid < 0 || m_DamagedUserList[i].nDamage<= 0) continue;		// �����ڵ�
		if(m_DamagedUserList[i].bIs == TRUE) pUser = GetUser(pCom, m_DamagedUserList[i].iUid);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		KeypUser=pUser;
		if(abs(pUser->m_sLevel-m_byClassLevel) > o_yehuoini[0]->djxz)  //������ֲ��40���޷���þ������ƴ�
			 continue;

		totalDamage = m_DamagedUserList[i].nDamage;
		if(MaxDamage<totalDamage){
			MaxDamage=totalDamage;
			KeypUser=pUser;
		}

		if ( (m_sExp / 5) <= 0 ) continue;
	    if ( totalDamage == 0 ) continue;
		if(((m_TotalDamage + firstDamage) /5) <= 0) continue;
		long long t=(long long)(m_sExp / 5) * totalDamage;
		exp =(int)(t/((m_TotalDamage + firstDamage) / 5));

		pUser->m_iCityValue += m_sInclination;
		if(pUser->m_iCityValue > 2000000000) pUser->m_iCityValue = 2000000000;	// �ִ밪�� ��� ���Ƿ� ���ߴ�.
		
		if(pUser->m_iDisplayType != 5 && pUser->m_iDisplayType != 6)
			pUser->GetExpCommon((int)(exp * 1.1));
		else
			pUser->GetExpCommon((int)(exp * 0.8)); //���� ����ڴ� ����ġ�� �����.
		//---------------------------------------------------------------------------------------------
		

	}
	Dead_User_level=0;
	if(KeypUser!=NULL)
		Dead_User_level=KeypUser->m_sLevel;
	
}

//////////////////////////////////////////////////////////////////////////////
//	���� ���� �������� �� ȭ�� �����ȿ� �ִ��� �Ǵ�
//
void CNpc::IsUserInSight(COM *pCom)
{
	int j;

	USER* pUser = NULL;

	int iSearchRange = m_bySearchRange;						// �ӽ÷� ��Ҵ�.
	int min_x, min_y, max_x, max_y;

	min_x = m_sCurX - 12;		if( min_x < 0 ) min_x = 0;
	min_y = m_sCurY - 13;		if( min_y < 0 ) min_y = 0;
	max_x = m_sCurX + 12;
	max_y = m_sCurY + 13;

	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;

	int ix, iy;
	int target_uid;
	int uid;
	int iLen = 0;

	for(j = 0; j < NPC_HAVE_USER_LIST; j++)
	{
		m_DamagedUserList[j].bIs = FALSE;
	}

	for(ix = min_x; ix <= max_x; ix++)
	{
		for(iy = min_y; iy <= max_y; iy++)
		{
			target_uid = m_pOrgMap[ix][iy].m_lUser;

			if( target_uid >= USER_BAND && target_uid < NPC_BAND )
			{
				uid = target_uid - USER_BAND;
				for(j = 0; j < NPC_HAVE_USER_LIST; j++)
				{												// �����ִ� ����Ʈ���� ������ ���ٸ�		
					if(m_DamagedUserList[j].iUid == uid)		// ���� ID�� ���ؼ� �����ϸ�	
					{
						pUser = pCom->GetUserUid(uid);		
						if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED || pUser->m_curz != m_sCurZ) continue;
																
						iLen = strlen(pUser->m_strUserID);
						if(iLen <= 0 || iLen > CHAR_NAME_LENGTH) continue;

						if(strcmp(pUser->m_strUserID, m_DamagedUserList[j].strUserID) == 0) 
						{										// �̶����� �����Ѵٴ� ǥ�ø� �Ѵ�.
							m_DamagedUserList[j].bIs = TRUE;
						}
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//	NPC DEAD Packet �� �α� �������� ������.
//
int CNpc::SendDead(COM *pCom, int type,BOOL TreeBaoLv)
{
//	ASSERT(pCom);
	if(!pCom) return 0;
	if(m_NpcState != NPC_DEAD || m_sHP > 0) return 0;

	CBufferEx TempBuf;

	CPoint pt = ConvertToClient(m_sCurX, m_sCurY);

	TempBuf.Add(DEAD);
	TempBuf.Add((short)(m_sNid + NPC_BAND));
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);
		
	SendInsight(pCom, TempBuf, TempBuf.GetLength());
	if(type) GiveNpcHaveItem(pCom ,TreeBaoLv);	//������
//	if(( (type)  &&  (abs(Dead_User_level-m_byClassLevel) <=50) )|| m_sEvent!=0)
//		GiveNpcHaveItem(pCom);	// ������ ������(����̸� �ȶ���Ʈ��)
	
	return m_sRegenTime;
}

////////////////////////////////////////////////////////////////////////////////
//	�ֺ��� ���� ���ų� �������� ��� ������ ������ ��ã�⸦ �� �� �����δ�.
//
BOOL CNpc::RandomMove(COM *pCom)
{
	if(m_bySearchRange == 0) return FALSE;
	if(pCom == NULL) return FALSE;

	if(m_tNpcType == NPCTYPE_GUILD_DOOR)	return FALSE;	// �̵����ϰ�...
	if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD)	return FALSE;	// �̵����ϰ�...
	// NPC �� �ʱ� ��ġ�� ������� �Ǵ��Ѵ�.
	BOOL bIsIn = IsInRange();

	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return FALSE;
	if( !pMap->m_pMap ) return FALSE;

	CPoint pt;
	int nLoop = 0;
	int nDestX = -1, nDestY = -1;
	int min_x, min_y, max_x, max_y;
	int temp_minx = 0, temp_miny = 0, temp_maxx = 0, temp_maxy = 0;

	CRect rectIn;

	if(bIsIn)	// NPC �� �ʱ� ��ġ�� ����� �ʾ�����
	{
/*alisia
		int temp_range = m_bySearchRange / 2;

		min_x = m_sCurX - temp_range;	if(min_x < 0) min_x = 0;
		min_y = m_sCurY - temp_range;	if(min_y < 0) min_y = 0;
		max_x = m_sCurX + temp_range;	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
		max_y = m_sCurY + temp_range;	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;

		rectIn.IntersectRect(CRect(m_nInitMinX, m_nInitMinY, m_nInitMaxX, m_nInitMaxY), CRect(min_x, min_y, max_x, max_y));

		nLoop = 0;
		while(1)
		{
			nDestX = myrand(rectIn.left, rectIn.right);
			nDestY = myrand(rectIn.top, rectIn.bottom);

			if(pMap->m_pMap[nDestX][nDestY].m_bMove != 0 || pMap->m_pMap[nDestX][nDestY].m_lUser != 0)
			{
				if(nLoop++ >= 10) 
				{
					TRACE("NOT FIND~~\n");
					return FALSE;
				}
				continue;
			}
			
			break;
		}
alisia*/

		m_bRandMove = TRUE;		// ���� �������� 8���� ���� ������������ ��Ÿ���� - PathFind() �Լ� �ȿ��� �����Ѵ�

		m_arRandMove.RemoveAll();

		int axis_x[3];	axis_x[0] = -1;	axis_x[1] = 0;	axis_x[2] = 1;
		int axis_y[3];	axis_y[0] = -1;	axis_y[1] = 0;	axis_y[2] = 1;
		int rand_x, rand_y, rand_d;

		rand_x = myrand( 0, 2 );
		rand_y = myrand( 0, 2 );
		rand_d = myrand( 1, 5 );

		for( int i = 1; i <= rand_d; i++ )
		{
			m_arRandMove.Add( CPoint( axis_x[rand_x] * i, axis_y[rand_y] * i ) );
		}

		m_min_x = m_sCurX;
		m_min_y = m_sCurY;

		return TRUE;
	}
	else		// NPC �� �ʱ� ��ġ�� �������
	{
		int x = 0, y = 0;
		
		min_x = m_sCurX;
		min_y = m_sCurY;
		max_x = m_sCurX;
		max_y = m_sCurY;

		if(m_nInitMinX < m_sCurX)	{min_x -= m_bySearchRange;	x += 1;} if(min_x < 0) min_x = 0;
		if(m_nInitMinY < m_sCurY)	{min_y -= m_bySearchRange;	y += 1;} if(min_y < 0) min_y = 0;
		if(m_nInitMaxX > m_sCurX)	{max_x += m_bySearchRange;	x += 1;} if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
		if(m_nInitMaxY > m_sCurY)	{max_y += m_bySearchRange;	y += 1;} if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;
				
		nLoop = 0;
		while(1)
		{
			nDestX = min_x + (rand() % (m_bySearchRange * x + 1)); 
			if(nDestX > max_x) nDestX = max_x;

			nDestY = min_y + (rand() % (m_bySearchRange * y + 1));
			if(nDestY > max_y) nDestY = max_y;

			if(pMap->m_pMap[nDestX][nDestY].m_bMove != 0 || pMap->m_pMap[nDestX][nDestY].m_lUser != 0)
			{
				if(nLoop++ >= 10) return FALSE;
				continue;
			}
			
			break;
		}
	}

	if(nDestX < 0 || nDestY < 0)
	{
		return FALSE;
	}

	// Run Path Find ---------------------------------------------//
	CPoint start, end;
	start.x = m_sCurX - min_x;
	start.y = m_sCurY - min_y;
	end.x = nDestX - min_x;
	end.y = nDestY - min_y;

	if(start.x < 0 || start.y < 0 || end.x < 0 || end.y < 0)
	{
		return FALSE;
	}

	m_ptDest.x = nDestX;
	m_ptDest.y = nDestY;

	m_min_x = min_x;
	m_min_y = min_y;
	m_max_x = max_x;
	m_max_y = max_y;

	return PathFind(start, end);
}

/////////////////////////////////////////////////////////////////////////////////////
//	NPC �� �ʱ� ������ġ �ȿ� �ִ��� �˻�
//
BOOL CNpc::IsInRange()
{
	// NPC �� �ʱ� ��ġ�� ������� �Ǵ��Ѵ�.
//	CRect rect(m_nInitMinX, m_nInitMinY, m_nInitMaxX, m_nInitMaxY);
	
//	return rect.PtInRect(CPoint(m_sCurX, m_sCurY));

	if( m_nInitMinX > m_sCurX || m_nInitMaxX < m_sCurX ) return FALSE;
	if( m_nInitMinY > m_sCurY || m_nInitMaxY < m_sCurY ) return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
//	�þ߰� ����ƴ��� �Ǵ��ϰ� ����ƴٸ� ���泻���� Ŭ���̾�Ʈ�� �����Ѵ�.
//
void CNpc::SightRecalc(COM* pCom)
{
	int sx, sy;

	sx = m_sCurX / SIGHT_SIZE_X;
	sy = m_sCurY / SIGHT_SIZE_Y;

	int dir_x = 0;
	int dir_y = 0;

	if( sx == m_presx && sy == m_presy ) return;
	
	if( m_presx == -1 || m_presy == -1 )
	{
		dir_x = 0;
		dir_y = 0;
	}
	else
	{
		if( sx > m_presx && abs(sx-m_presx) == 1 )		dir_x = DIR_H;
		if( sx < m_presx && abs(sx-m_presx) == 1 )		dir_x = DIR_L;
		if( sy > m_presy && abs(sy-m_presy) == 1 )		dir_y = DIR_H;
		if( sy < m_presy && abs(sy-m_presy) == 1 )		dir_y = DIR_L;
		if( abs(sx-m_presx) > 1 )						dir_x = DIR_OUTSIDE;
		if( abs(sy-m_presy) > 1 )						dir_y = DIR_OUTSIDE;
	}

	int prex = m_presx;
	int prey = m_presy;
	m_presx = sx;
	m_presy = sy;

	SendUserInfoBySightChange(dir_x, dir_y, prex, prey, pCom);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�þߺ������� ���� �������� ����
//
void CNpc::SendUserInfoBySightChange(int dir_x, int dir_y, int prex, int prey, COM *pCom)
{
	int min_x = 0, min_y = 0;
	int max_x = 0, max_y = 0;

	int sx = m_presx;
	int sy = m_presy;

	int modify_index = 0;
	char modify_send[1024];		::ZeroMemory(modify_send, sizeof(modify_send));
	FillNpcInfo(modify_send, modify_index, INFO_MODIFY);

	int delete_index = 0;
	char delete_send[1024];		::ZeroMemory(delete_send, sizeof(delete_send));
	FillNpcInfo(delete_send, delete_index, INFO_DELETE);

	if( prex == -1 || prey == -1 )
	{
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
		return;
	}
	if( dir_x == DIR_OUTSIDE || dir_y == DIR_OUTSIDE )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
		return;
	}
	if( dir_x > 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx+1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
	}
	if( dir_y > 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey)*SIGHT_SIZE_Y;
		SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy+1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
	}
	if( dir_x < 0 )
	{
		min_x = (prex+1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
	}
	if( dir_y < 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey+1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
	}
}

////////////////////////////////////////////////////////////////////////////////
//	��ȭ�鳻�� �������Ը� ��������
//
void CNpc::SendExactScreen(COM* pCom, TCHAR *pBuf, int nLength)
{
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;
	
	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL) return;

	pNewData->flag = SEND_SCREEN;
	pNewData->len = nLength;

	::CopyMemory(pNewData->pBuf, pBuf, nLength);

	pNewData->uid = 0;
	pNewData->x = m_sCurX;
	pNewData->y = m_sCurY;
	pNewData->z = m_sCurZ;
	pNewData->zone_index = m_ZoneIndex;

	pCom->Send(pNewData);
	if(pNewData) delete pNewData;
}

///////////////////////////////////////////////////////////////////////////////
//	�þ߾ȿ� �ִ� �������� ������ ����
//
void CNpc::SendInsight(COM* pCom, TCHAR *pBuf, int nLength)
{
/*
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;
	
	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL) return;

	pNewData->flag = SEND_INSIGHT;
	pNewData->len = nLength;

	::CopyMemory(pNewData->pBuf, pBuf, nLength);

	pNewData->uid = 0;
	pNewData->x = m_sCurX;
	pNewData->y = m_sCurY;
	pNewData->z = m_sCurZ;
	pNewData->zone_index = m_ZoneIndex;

	EnterCriticalSection( &(pCom->m_critSendData) );
	pCom->m_arSendData.Add( pNewData );
	LeaveCriticalSection( &(pCom->m_critSendData) );

	PostQueuedCompletionStatus( pCom->m_hSendIOCP, 0, 0, NULL );
*/
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;

	int sx = m_sCurX / SIGHT_SIZE_X;
	int sy = m_sCurY / SIGHT_SIZE_Y;
	
	int min_x = (sx-1)*SIGHT_SIZE_X; if( min_x < 0 ) min_x = 0;
	int max_x = (sx+2)*SIGHT_SIZE_X;
	int min_y = (sy-1)*SIGHT_SIZE_Y; if( min_y < 0 ) min_y = 0;
	int max_y = (sy+2)*SIGHT_SIZE_Y;
	
	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return;
	
	if( max_x >= pMap->m_sizeMap.cx ) max_x = pMap->m_sizeMap.cx - 1;
	if( max_y >= pMap->m_sizeMap.cy ) max_y = pMap->m_sizeMap.cy - 1;
	
	int temp_uid;
	USER* pUser = NULL;

	for( int i = min_x; i < max_x; i++ )
	{
		for( int j = min_y; j < max_y; j++ )
		{				
			temp_uid = pMap->m_pMap[i][j].m_lUser;

			if(temp_uid < USER_BAND || temp_uid >= NPC_BAND) continue;
			else temp_uid -= USER_BAND;
			
			if( temp_uid >= 0 && temp_uid < MAX_USER )
			{
				pUser = pCom->GetUserUid(temp_uid);
				if ( pUser == NULL ) continue;
				
				if( pUser->m_state == STATE_GAMESTARTED )
				{
					if( pUser->m_curx == i && pUser->m_cury == j && pUser->m_curz == m_sCurZ )
					{
						Send( pUser, pBuf, nLength );
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
//	���� NPC �� ��ġ�� (xpos, ypos) ������ �Ÿ��� ���
//
BOOL CNpc::GetDistance(int xpos, int ypos, int dist)
{
	if(xpos >= g_zone[m_ZoneIndex]->m_sizeMap.cx || xpos < 0 || ypos >= g_zone[m_ZoneIndex]->m_sizeMap.cy || ypos < 0) return FALSE;

	int dx = abs(xpos - m_sCurX);
	int dy = abs(ypos - m_sCurY);

	if(dx + dy > dist * 2) return FALSE;
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////
//	NPC �� ���� �������� ������.
//�������������Ʒ����
void CNpc::GiveNpcHaveItem(COM *pCom,BOOL TreeBaoLv)
{
	int temp = 0;
	int iPer = 0, iVal = 0;
	int iRandom;
	int nCount = 1;
	int nDnHap = 0;
    USER *pUser = GetUser(pCom, m_DamagedUserList[0].iUid);

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)	// �߸��� �������̵��̸� ����
	{
		return;
	}
//	SYSTEMTIME gTime;
//	GetLocalTime(&gTime);	

	if(m_NpcVirtualState == NPC_WAIT) return;

/*	if(gTime.wDay > 7 && gTime.wDay < 14)					// 8�Ϻ��� 13�ϱ���	
	{
		if(m_byClassLevel <= 20)		nCount = 1;
		else if(m_byClassLevel <= 50)	nCount = 2;
		else if(m_byClassLevel <= 70)	nCount = 3;
		else							nCount = 5;
	}
	else
	{
*/
	//if (m_sEvent == 32000) //ָ�����¼�BOSS�ų�����ҩ��Ǯ �������������
 //   {
 //       nCount = 10; // ȷ������ 10 ����Ʒ
 //       for (int i = 0; i < nCount; i++)
 //       {
 //           // ֱ�ӵ�����Ʒ�����ܸ�������
 //           iRandom = myrand(1, 10000) % g_DropItem[m_sPid][m_byColor].n;
 //           temp = iVal = g_DropItem[m_sPid][m_byColor].novelity[iRandom].code1 + g_DropItem[m_sPid][m_byColor].novelity[iRandom].code2 * 256;
 //           if (iVal >= g_arItemTable.GetSize()) {
 //               continue; // ȷ�����ᳬ����Ʒ����Χ
 //           }
 //           if (g_arItemTable[temp]->m_byWear <= 5 || g_arItemTable[temp]->m_byWear == 117 || g_arItemTable[temp]->m_byWear == 20)
 //           {
 //               iVal = IsTransformedItem(g_arItemTable[temp]->m_sSid);
 //               if (iVal == -1) iVal = temp;
 //           }
 //           GiveItemToMap(pCom, iVal, TRUE);
 //       }
 //       return; // ������ 32000 �¼���ֱ�ӷ���
	//}
	if(TreeBaoLv ==TRUE || g_sanBaoLv ==TRUE || pUser->m_dwZaiXianTime != 0)		nCount = 3;		//���ݻ�Ƿ񱬶��ټ�װ��
	else							nCount = 2;

	if(m_sEvent== 32000 || m_sEvent== 30007 || m_sEvent== 30009 )                    nCount = 10;
	if( m_sEvent == NPC_EVENT_MOP )				nCount = 16;		// 31000ʱ��16����Ʒ
	if( m_sEvent == NPC_EVENT_GREATE_MOP )		nCount = 10;	// �������� �� Ư�� ������ ��� 10���� ����߸���.
//	}
/*
	for(int i = 0; i < nCount; i++)
	{
		iRandom = myrand(1, 30);

		if(iRandom < m_tItemPer)
		{

			iRandom = myrand(1, 10000);
			for(int i = 2; i < g_NpcItem.m_nField; i += 2)
			{			
//				iPer = g_NpcItem.m_ppItem[i][m_byClassLevel];
				iPer = g_NpcItem.m_ppItem[i][m_sHaveItem];
				if(iPer == 0) return;
				if(iRandom < iPer)
				{											// �켱 �⺻���̺��� �����ϱ�����	
//					temp = g_NpcItem.m_ppItem[i-1][m_byClassLevel];
					temp = g_NpcItem.m_ppItem[i-1][m_sHaveItem];
					iVal = temp;
					if(temp >= g_arItemTable.GetSize()) return;

					if(g_arItemTable[temp]->m_byWear <= 5 || g_arItemTable[temp]->m_byWear == 117 || g_arItemTable[temp]->m_byWear == 20)	// �������̺��̴�.
					{
						iVal = IsTransformedItem(g_arItemTable[temp]->m_sSid);
						if(iVal == -1) iVal = temp;//return;
					}
					if (iVal == 845 || iVal == 909 || iVal == 846 || iVal == 907 || iVal == 908) iVal = 847;
					GiveItemToMap(pCom, iVal, TRUE);
					break;
				}
			}

		}

		else if(iRandom < m_tDnPer)
		{
//			SYSTEMTIME gTime;										//@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Event Code �Ⱓ���� �Ͻ������� �÷��ش�.
//			GetLocalTime(&gTime);

			iPer = g_arDNTable[m_byClassLevel]->m_sMinDn;
			iVal = g_arDNTable[m_byClassLevel]->m_sMaxDn;
			iRandom = myrand(iPer, iVal);
																	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Event Code �Ⱓ���� �Ͻ������� �÷��ش�.	

			nDnHap += iRandom;
			//GiveItemToMap(pCom, iRandom, FALSE);
		}
	}
	if(nDnHap > 0 )
	{
      if (nDnHap == 845 || nDnHap == 909 || nDnHap == 846 || nDnHap == 907 || nDnHap == 908) nDnHap = 847;
		GiveItemToMap(pCom, nDnHap, FALSE);
	}
	*/
	int tItemHavePer;
	if(m_sPid>255)
		return;
	int tItemPer=g_DropItem[m_sPid][m_byColor].DropNovelity;
	int tLeechdomPer = g_DropItem[m_sPid][m_byColor].DropLeechdom+tItemPer;
	int tItemN=g_DropItem[m_sPid][m_byColor].n;
	int i;
	//TRACE("���� %d\n",nCount);
	for(i = 0; i < nCount; i++)
	{
		//TRACE( "i=%d\n",i);
		iRandom = myrand(1, 100);
		//TRACE("����� %d\n",iRandom);
		if(iRandom < tItemPer)
		{
			//TRACE( "��Ʒ--\n");
			iRandom = myrand(1, 10000)%tItemN;
			tItemHavePer = g_DropItem[m_sPid][m_byColor].novelity[iRandom].per;
			temp = iVal = g_DropItem[m_sPid][m_byColor].novelity[iRandom].code1+g_DropItem[m_sPid][m_byColor].novelity[iRandom].code2*256;
			if(iVal >= g_arItemTable.GetSize()){
				return;
			}
			//if(pUser->m_dwZaiXianTime != 0)//���Ƶ�����Ʒ
			//{
			//	if (iVal == 987) continue;
			//}
			iRandom = myrand(1, 1000);
			if(iRandom < tItemHavePer){	
				if(g_arItemTable[temp]->m_byWear <= 5 || g_arItemTable[temp]->m_byWear == 117 || g_arItemTable[temp]->m_byWear == 20)	// �������̺��̴�.
				{
					iVal = IsTransformedItem(g_arItemTable[temp]->m_sSid);
					if(iVal == -1) iVal = temp;
				}
				
				
			//	TRACE( "��Ʒ���� %s,%d\n",g_arItemTable[iVal]->m_strName,iVal);
				GiveItemToMap(pCom, iVal, TRUE);
				
				//break;
			}
		}
		else if(iRandom < tLeechdomPer)//����ҩ
		{
			//TRACE( "��ҩ--\n");
			if(m_byClassLevel <30)
				iVal=31;
			else if(m_byClassLevel <70)
				iVal=32;
			else if(m_byClassLevel <120)
				iVal=33;
			else
				iVal=227+256*3;
				//iVal=33;
			GiveItemToMap(pCom, iVal, TRUE);
		}else{
			
			//TRACE( "Ǯ--\n");
			int money= g_DropItem[m_sPid][m_byColor].money;
			iRandom =money+(money *myrand(0,15)%15);//15%����
			nDnHap += iRandom;
		
	  }//TRACE( "i=%d\n",i);
	}
	//TRACE( "i=%d----------------------------------------\n",i);
	if(nDnHap > 0 )
	{
		if (nDnHap == 845 || nDnHap == 909 || nDnHap == 846 || nDnHap == 907 || nDnHap == 908) nDnHap = 847;
		GiveItemToMap(pCom, nDnHap, FALSE);
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	�������� ������������ �ִ��� �Ǵ��Ѵ�.
//
int CNpc::IsTransformedItem(int sid)
{
	int iVal = 0;
	int i, j, iRandom;

	for(i = 0; i < g_ValItem.m_nRow; i++)
	{
		if(g_ValItem.m_ppItem[g_ValItem.m_nField-2][i] == sid)		// �� ������ �ʵ忡�� ������ �׸�(6�� ����)
		{
			iRandom = myrand(1, 100);
			for(j = 2; j < g_ValItem.m_nField; j+= 2)
			{
				iVal = g_ValItem.m_ppItem[j][i];
				if(iRandom < iVal)
				{
					if(sid >= g_arItemTable.GetSize()) return -1;
					else return g_ValItem.m_ppItem[j-1][i];
				}
			}
		}
	}

	return -1;
}

int CNpc::IsMagicItem(COM* pCom, ItemList *pItem, int iTable)
{
    int i = 0, j;
    int iMagicTemp = 1 , iRareTemp = 1;
    int iRandom = myrand( 1, 10000 );

    if(pItem == NULL) return 0;
    int iMagicCount = 0, iCount = 0;
    int nLoop = 0, iType = 0;
    int nEventMoon = 0;
    int nEventSongpeon = 0;
    int nEventBox = 0;
    int iMagicUp = 0;
    
    USER* pUser = NULL;

    if(m_sEvent == NPC_EVENT_MOP)
    {
        iMagicTemp = 0; 
        iRareTemp = 4; 
    }
    else if(m_sEvent == NPC_EVENT_GREATE_MOP)
    {
        iMagicTemp = 0;
        iRareTemp = 8;
    }
    else if(m_sEvent == 30009 || m_sEvent == 30007)
    {
        iMagicTemp = 0;
        iRareTemp = 10000;
    }

    if(m_lKillUid >= 0)   
    {
        pUser = GetUser(pCom, m_lKillUid);
        if(pUser != NULL && pUser->m_state == STATE_GAMESTARTED)
        {
            if(pUser->m_dwMagicFindTime != 0 || pUser->m_isDoubleBAOLV != 0 || g_sanBaoLv == TRUE || pUser->m_dwZaiXianTime != 0)
            {
                iMagicTemp = 0;
                iRareTemp = 10000;
            }
            else if(pUser->m_dwMagicFindTime == 0 || pUser->m_isDoubleBAOLV == 0 || pUser->m_dwZaiXianTime == 0)
            {
                iMagicTemp = 5000;
                iRareTemp = 5000;
            }
        }
    }

    nEventMoon = NPC_RARE_ITEM * iRareTemp + (NPC_EVENT_MOON - NPC_RARE_ITEM);
    nEventSongpeon = nEventMoon + (NPC_EVENT_SONGPEON - NPC_EVENT_MOON);
    nEventBox = nEventSongpeon + (NPC_EVENT_BOX - NPC_EVENT_SONGPEON);

    if(iRandom <= NPC_MAGIC_ITEM * iMagicTemp)
    {
        nLoop = 2;
        iType = MAGIC_ITEM;
    }
    else if((iRandom > NPC_MAGIC_ITEM * iMagicTemp) && (iRandom <= (NPC_RARE_ITEM * iRareTemp + iMagicUp)))    
    { 
        nLoop = 4;
        iType = RARE_ITEM; 
    }    
    else if(0 && iRandom > NPC_RARE_ITEM * iRareTemp && iRandom <= nEventMoon)
    {
        return EVENT_ITEM_MOON;
    }
    else if(0 && iRandom > nEventMoon && iRandom <= nEventSongpeon)
    {
        return EVENT_ITEM_SONGPEON;
    }
    else if(0 && iRandom > nEventSongpeon && iRandom <= nEventBox)
    {
        return EVENT_ITEM_BOX;
    }
    else return NORMAL_ITEM;

    int iTemp = 0;

    if(m_ItemUserLevel <= 20)       iMagicCount = 205;
    else if(m_ItemUserLevel <= 40)  iMagicCount = 205;
    else if(m_ItemUserLevel <= 60)  iMagicCount = 205;
    else                            iMagicCount = 205;

    if(iMagicCount >= g_arMagicItemTable.GetSize()) 
        iMagicCount = g_arMagicItemTable.GetSize() - 1;

    static const int allowSids[] = {
        49, 53, 56, 61, 64, 73, 74, 76, 86, 87, 88, 89, 91, 93, 94, 95, 96, 97,
        105, 106, 107, 109, 110, 113, 117, 120, 125, 128, 129, 130, 132, 135, 
        136, 137, 138, 139, 141, 145
    };
    static const int allowCount = sizeof(allowSids) / sizeof(allowSids[0]);

    while(nLoop > i)
    {
        if(m_sEvent == 32000)
        {
            // �����������Ա�������ѡ��һ��
            int randomIndex = myrand(0, allowCount - 1);
            int targetSid = allowSids[randomIndex];
            
            // ��ħ�����Ա��в��Ҷ�Ӧ������
            bool found = false;
            for(int k = 0; k < iMagicCount; k++)
            {
                if(g_arMagicItemTable[k]->m_sSid == targetSid)
                {
                    iRandom = k;
                    found = true;
                    break;
                }
            }
            
            if(!found || !g_arMagicItemTable[iRandom]->m_tUse) continue;
        }
        else
        {
            // ��32000�¼��Ĵ���
            do {
                if(pUser != NULL && pUser->m_iDisplayType != 5 && pUser->m_iDisplayType != 6)
                {
                    if(m_ItemUserLevel <= 20)
                    {
                        iRandom = myrand(0, iMagicCount);
                    }
                    else
                    {
                        iRandom = myrand(41, iMagicCount);
                    }
                }
                else
                {
                    iRandom = myrand(0, iMagicCount);
                }

                // ����Ƿ�����Ч��ħ������
                if(iRandom >= g_arMagicItemTable.GetSize()) continue;
                if(!g_arMagicItemTable[iRandom]->m_tUse) continue;

                break;
            } while(true);
        }

        if(CheckClassItem(iTable, iRandom) == FALSE) 
        {
            if(i == 0) continue;
            else if(iType == RARE_ITEM && i <= 3) continue;
            else { i++; continue; }
        }

        for(j = 0; j < 4; j++)
        {
            if (pItem->tMagic[j] < 0 || pItem->tMagic[j] >= iMagicCount) continue;
                
            if(o_yehuoini[0]->chongdie == 0)
            {
                iCount = g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType;
            }
            if(iCount != 0 && iCount == g_arMagicItemTable[iRandom]->m_sSubType)
            {
                iCount = g_arMagicItemTable[pItem->tMagic[j]]->m_sChangeValue;
                if(iCount < g_arMagicItemTable[iRandom]->m_sChangeValue)
                {
                    iTemp = g_arMagicItemTable[pItem->tMagic[j]]->m_tLevel;
                    if(pItem->sLevel - iTemp > 0) pItem->sLevel -= iTemp;
                    pItem->sLevel += g_arMagicItemTable[iRandom]->m_tLevel;
                    pItem->tMagic[j] = iRandom; 

                    if(g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType == MAGIC_DURATION_UP)
                    {
                        iTemp = g_arMagicItemTable[pItem->tMagic[j]]->m_sChangeValue;
                        if(pItem->sDuration - iTemp > 0) pItem->sDuration -= iTemp;
                        pItem->sDuration += g_arMagicItemTable[iRandom]->m_sChangeValue;
                    }
                    break;
                }
                else if(iCount == g_arMagicItemTable[iRandom]->m_sChangeValue) break;
            }

            if(pItem->tMagic[j] > 0) continue;
            else
            { 
                pItem->tMagic[j] = iRandom; i++; 
                if(g_arMagicItemTable[iRandom]->m_tLevel > 0) pItem->sLevel += g_arMagicItemTable[iRandom]->m_tLevel;
                if(g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType == MAGIC_DURATION_UP)
                {
                    pItem->sDuration += g_arMagicItemTable[iRandom]->m_sChangeValue;
                }
                break; 
            }
        }
    }

    return iType;
}
///////////////////////////////////////////////////////////////////////////////////
//	�������� ���� �������� ����帱���ִ��� �Ǵ�
//���������Ʒ������
//int CNpc::IsMagicItem(COM* pCom, ItemList *pItem, int iTable)
//{
//	int i = 0, j;
//	int iMagicTemp = 1 , iRareTemp = 1;
//	int iRandom = myrand( 1, 10000 );
//
//	if(pItem == NULL) return 0;
//	int iMagicCount = 0, iCount = 0;
//	int nLoop = 0, iType = 0;
//	int nEventMoon = 0;
//	int nEventSongpeon = 0;
//	int nEventBox = 0;
//	int iMagicUp=0;
//	
//
//    USER* pUser = NULL;
//
//	/*SYSTEMTIME gTime;										//@@@@@@@@@@@@@@@ Event Code �Ⱓ���� �Ͻ������� �÷��ش�.
//	GetLocalTime(&gTime);
//	/*if(gTime.wMonth < 7) { iMagicTemp = 10; iRareTemp = 2; }
//	else if(gTime.wMonth == 7) 
//	{ 
//		if(gTime.wDay <= 7) { iMagicTemp = 10; iRareTemp = 2; } 
//	}
//	*/
///*	if(m_sEvent== 32000){
//		iMagicTemp=0;
//		iRareTemp = 3; 
//	}else*/if(m_sEvent == NPC_EVENT_MOP )				// ���� ��ǲ,��,�����ǿ�
//	{
//		iMagicTemp = 0; 
//		iRareTemp = 4; 
//	}
//	else if(m_sEvent == NPC_EVENT_GREATE_MOP)	// ��������
//	{
//		iMagicTemp = 0;  //70
//		iRareTemp = 8;    //75
//	}
//	
//	else if(m_sEvent == 30009 || m_sEvent == 30007)	// ��������
//	{
//		iMagicTemp = 0;  //70
//		iRareTemp = 10000;    //75
//	}
//
//
//	/*if(m_lKillUid >= 0)
//	{
//		pUser = GetUser(pCom, m_lKillUid);
//		if(pUser != NULL && pUser->m_state == STATE_GAMESTARTED)
//		{
//			if(pUser->m_dwMagicFindTime != 0)	// ���� ���δ��� ���� �����̸�
//			{
//					iMagicTemp=0;
//					iMagicUp=10000;
//			}
//		}
//	}*/
//	if(m_lKillUid >= 0)   
//	{
//		pUser = GetUser(pCom, m_lKillUid);
//		if(pUser != NULL && pUser->m_state == STATE_GAMESTARTED)
//		{
//			if(pUser->m_dwMagicFindTime != 0 || pUser->m_isDoubleBAOLV != 0 || g_sanBaoLv == TRUE || pUser->m_dwZaiXianTime != 0)
//			{
//				//iMagicTemp *= o_yehuoini[0]->xyl;  //����ʱ����װ��
//				//iRareTemp *= o_yehuoini[0]->xyh;    //����ʱ����װ��
//				iMagicTemp = 0;  //����ʱ����װ��
//				iRareTemp = 10000;    //����ʱ����װ��
//			}
//			else if(pUser->m_dwMagicFindTime == 0 || pUser->m_isDoubleBAOLV == 0 || pUser->m_dwZaiXianTime == 0)
//			{
//				//iMagicTemp *= o_yehuoini[0]->lan;  //û������ʱ����װ��
//				//iRareTemp *= o_yehuoini[0]->huang;    //û������ʱ����װ��
//				iMagicTemp = 5000;  //����ʱ����װ��
//				iRareTemp = 5000;    //����ʱ����װ��
//			}
//            
//		}
//	}
//
//	nEventMoon		= NPC_RARE_ITEM * iRareTemp + (NPC_EVENT_MOON - NPC_RARE_ITEM);
//	nEventSongpeon	= nEventMoon + (NPC_EVENT_SONGPEON - NPC_EVENT_MOON);
//	nEventBox		= nEventSongpeon + (NPC_EVENT_BOX - NPC_EVENT_SONGPEON);
//
//    if(iRandom <= NPC_MAGIC_ITEM * iMagicTemp)	// ����
//	{
//		nLoop = 2;
//		iType = MAGIC_ITEM;
//	}
//	else if((iRandom > NPC_MAGIC_ITEM * iMagicTemp) && (iRandom <=( NPC_RARE_ITEM * iRareTemp+iMagicUp)))	
//	{ 
//		nLoop = 4;
//		iType = RARE_ITEM; 
//	}	
//	else if(0 && iRandom > NPC_RARE_ITEM * iRareTemp && iRandom <= nEventMoon)
//	{
//		return EVENT_ITEM_MOON;
//	}
//	else if(0 && iRandom > nEventMoon && iRandom <= nEventSongpeon)
//	{
//		return EVENT_ITEM_SONGPEON;
//	}
//	else if(0 && iRandom > nEventSongpeon && iRandom <= nEventBox)
//	{
//		return EVENT_ITEM_BOX;
//	}
//	else return NORMAL_ITEM;								// �Ϲݾ�����
//
//	int iTemp = 0;
//
//	if(m_ItemUserLevel <= 20)       iMagicCount = 205;	//42;	// ������Ʒ���Կ���    //neo�汾
//	else if(m_ItemUserLevel <= 40)  iMagicCount = 205;  //106
//	else if(m_ItemUserLevel <= 60)  iMagicCount = 205;  //143
//	else							iMagicCount = 205;    //��ֵԽ��,��������Խ��
//
//	
//
//	if(iMagicCount >= g_arMagicItemTable.GetSize()) iMagicCount = g_arMagicItemTable.GetSize() - 1;
//
//	while(nLoop > i)										// ������ 4���Ӽ����� ���Ѵ�.
//	{
//		
//		iRandom = myrand(0, iMagicCount);
//		//------------------------------------------------------------------------------------------------
//		//yskang 0.6 �������ڿ��� �����̾� ���� �ɼ��� �������� ��������.
//		if(pUser != NULL)
//		{
//			if(pUser->m_iDisplayType !=5 && pUser->m_iDisplayType !=6)//���� ������̴�.
//			{
//				if(m_ItemUserLevel <= 20)
//					iRandom = myrand(0, iMagicCount); //���� �������� ������ Ȯ���� ���δ�.
//				else
//					iRandom = myrand(41, iMagicCount); 
//			}
//		}
//		
//		if(!g_arMagicItemTable[iRandom]->m_tUse) continue;
//
//		if(CheckClassItem(iTable, iRandom) == FALSE) 
//		{
//			if(i == 0) continue;							// ������ �⺻�� 1��
////			else if(iType == RARE_ITEM && i <= 2) continue;	// ���ƻ�ɫ��Ʒ���Ա���Ϊ2������
//			else if(iType == RARE_ITEM && i <= 3) continue;	// ���ƻ�ɫ��Ʒ���Ա���Ϊ3������
//			else { i++; continue; }
//		}
//
//		for(j = 0; j < 4; j++)
//		{
//			if (pItem->tMagic[j] < 0 || pItem->tMagic[j] >= iMagicCount) continue;
//				
//			if(o_yehuoini[0]->chongdie == 0)
//			{
//				iCount = g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType;//������Ʒ�����ظ����Դ���
//		    }
//			if(iCount != 0 && iCount == g_arMagicItemTable[iRandom]->m_sSubType)	// �Ӽ��� ��ĥ�� �����Ƿ� ���� ū���� ����	
//			{
//				iCount = g_arMagicItemTable[pItem->tMagic[j]]->m_sChangeValue;
//				if(iCount < g_arMagicItemTable[iRandom]->m_sChangeValue)
//				{
//					iTemp = g_arMagicItemTable[pItem->tMagic[j]]->m_tLevel;
//					if(pItem->sLevel - iTemp > 0) pItem->sLevel -= iTemp;
//					pItem->sLevel += g_arMagicItemTable[iRandom]->m_tLevel;
//					pItem->tMagic[j] = iRandom; 
//
//					if(g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType == MAGIC_DURATION_UP)
//					{
//						iTemp = g_arMagicItemTable[pItem->tMagic[j]]->m_sChangeValue;
//						if(pItem->sDuration - iTemp > 0) pItem->sDuration -= iTemp;
//						pItem->sDuration += g_arMagicItemTable[iRandom]->m_sChangeValue; // ������ ���� �Ӽ��� ���õɶ� 
//					}
//					break;
//				}
//				else if(iCount == g_arMagicItemTable[iRandom]->m_sChangeValue) break;
//			}
//
//			if(pItem->tMagic[j] > 0) continue;	// �̹� ���Կ� ���� ������ �Ѿ
//			else
//			{ 
//				pItem->tMagic[j] = iRandom; i++; 
//				if(g_arMagicItemTable[iRandom]->m_tLevel > 0) pItem->sLevel += g_arMagicItemTable[iRandom]->m_tLevel;
//				if(g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType == MAGIC_DURATION_UP)
//				{
//					pItem->sDuration += g_arMagicItemTable[iRandom]->m_sChangeValue; // ������ ���� �Ӽ��� ���õɶ� 
//				}
//				break; 
//			}
//		}
////		i++;
//	}
//
//	return iType;
//}
//����ػ�����
void CNpc::shouhu_rand(	ItemList *pMapItem)
{
	int a;

	a=myrand(0,15)%100;
	pMapItem->tIQ=0x09;//�ػ���ɫ
	switch (a){
		case 0:pMapItem->tMagic[0]=1; return;//����2 3 5
		case 1:pMapItem->tMagic[0]=4; return;//�����ȼ�����1 2 3
		case 2:pMapItem->tMagic[0]=7; return;//���Է�����2 5 10ת�����Լ�����
		case 3:pMapItem->tMagic[0]=10; return;//����Χ���2% 5 10�˺�
		case 4:pMapItem->tMagic[0]=13; return;//13 ��������3 6 10
		case 5:pMapItem->tMagic[0]=16; return;//ħ����������3 6 10
		case 6:pMapItem->tMagic[0]=19; return;//�����˶��Լ������˺�2% 3 5 ���䵽��������
		case 7:pMapItem->tMagic[0]=22; return;// ��־���ֵ���2% 3% 5%
		case 8:pMapItem->tMagic[0]=25; return;//������10% ������20% ������50%
		case 9:pMapItem->tMagic[0]=28; return;// ���м�������1  ���м�������2  ���м�������3
		case 10:pMapItem->tMagic[0]=31; return;//������Ʒ���2% ������Ʒ���3% ������Ʒ���5%
		case 11:pMapItem->tMagic[0]=34; return;//����������10 ����������20 ����������30
		case 12:pMapItem->tMagic[0]=37; return;//��ȡ�Է�����10 ��ȡ�Է�����20 ��ȡ�Է�����30
		case 13:pMapItem->tMagic[0]=40; return;//��������10 ��������20 ��������30
		case 14:pMapItem->tMagic[0]=1; return;//����2 3 5
		case 15:pMapItem->tMagic[0]=13; return;//13 ��������3 6 10
		case 16:pMapItem->tMagic[0]=1; return;//����2 3 5
		case 17:pMapItem->tMagic[0]=34; return;//����������10 ����������20 ����������30
		case 18:pMapItem->tMagic[0]=13; return;//13 ��������3 6 10
		case 19:pMapItem->tMagic[0]=1; return;//����2 3 5
		case 20:pMapItem->tMagic[0]=40; return;//��������10 ��������20 ��������30
		case 21:pMapItem->tMagic[0]=1; return;//����2 3 5
		case 22:pMapItem->tMagic[0]=13; return;//13 ��������3 6 10
		case 23:pMapItem->tMagic[0]=40; return;//��������10 ��������20 ��������30
		case 24:pMapItem->tMagic[0]=40; return;//��������10 ��������20 ��������30
		case 25:pMapItem->tMagic[0]=1; return;//����2 3 5
		case 26:pMapItem->tMagic[0]=13; return;//13 ��������3 6 10
		case 27:pMapItem->tMagic[0]=1; return;//����2 3 5
		case 28:pMapItem->tMagic[0]=40; return;//��������10 ��������20 ��������30
		case 29:pMapItem->tMagic[0]=34; return;//����������10 ����������20 ����������30
		case 30:pMapItem->tMagic[0]=1; return;//����2 3 5
		default:
			pMapItem->tMagic[0]=0;return;
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	NPC Item�� �ʿ� ������.
//
void CNpc::GiveItemToMap(COM *pCom, int iItemNum, BOOL bItem, int iEventNum)
{
	int i, iRandom = 0;
	int iType = 0;
	BYTE tEBodySid = 0;

	CPoint pt;
	pt = FindNearRandomPointForItem(m_sCurX, m_sCurY);							// ���� �ڱ���ǥ�� ������ 24ĭ
	if(pt.x <= -1 || pt.y <= -1) return;
	if(pt.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pt.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return;

	ItemList *pMapItem = NULL;
	if(InterlockedCompareExchange((long*)&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_FieldUse, (long)1, (long)0) == (long)0)
	{
		pMapItem = new ItemList;

		if(!bItem)					// ���� ���
		{
			pMapItem->tType = TYPE_MONEY;
			pMapItem->dwMoney = iItemNum;

			pMapItem->uid[0] = m_iHaveItemUid[0].uid;
			pMapItem->uid[1] = m_iHaveItemUid[1].uid;
			pMapItem->uid[2] = m_iHaveItemUid[2].uid;

			pMapItem->SuccessRate[0] = (BYTE)m_iHaveItemUid[0].nDamage;
			pMapItem->SuccessRate[1] = (BYTE)m_iHaveItemUid[1].nDamage;
			pMapItem->SuccessRate[2] = (BYTE)m_iHaveItemUid[2].nDamage;
			pMapItem->dwTime = GetItemThrowTime();
		}
		else							// ������ ����ϰ��
		{
			if(iItemNum >= g_arItemTable.GetSize())
			{
				if(pMapItem) delete pMapItem;
				return; 
			}
			else
			{
				pMapItem->tType = TYPE_ITEM;
				pMapItem->sLevel = g_arItemTable[iItemNum]->m_byRLevel;
				pMapItem->sSid = g_arItemTable[iItemNum]->m_sSid;
				pMapItem->sDuration = g_arItemTable[iItemNum]->m_sDuration;
				pMapItem->sCount = 1;
				pMapItem->sBullNum = g_arItemTable[iItemNum]->m_sBullNum;
				for(i = 0; i < MAGIC_NUM; i++) pMapItem->tMagic[i] = 0;	// ���߿� Magic Item �߰��� ��
				pMapItem->tIQ = NORMAL_ITEM;
				pMapItem->iItemSerial = 0;

				pMapItem->uid[0] = m_iHaveItemUid[0].uid;				// �켱 ����
				pMapItem->uid[1] = m_iHaveItemUid[1].uid;
				pMapItem->uid[2] = m_iHaveItemUid[2].uid;

				pMapItem->SuccessRate[0] = (BYTE)m_iHaveItemUid[0].nDamage;	// �켱 ���� ����
				pMapItem->SuccessRate[1] = (BYTE)m_iHaveItemUid[1].nDamage;
				pMapItem->SuccessRate[2] = (BYTE)m_iHaveItemUid[2].nDamage;

				pMapItem->dwTime = GetItemThrowTime();
				//GetLocalTime(&pMapItem->ThrowTime);
			
				int iWear = g_arItemTable[iItemNum]->m_byWear;
				
				if(iWear >= 1 && iWear <= 5) 
				{
					iType = IsMagicItem(pCom, pMapItem, iItemNum);
					if(iType == MAGIC_ITEM)
					{
						pMapItem->tIQ = MAGIC_ITEM;	// ������ ó��...
					}
					else if(iType == RARE_ITEM)
					{
						pMapItem->tIQ = RARE_ITEM;	// ��� ó��...

// 						int n = pMapItem->tMagic[0] + pMapItem->tMagic[1] + pMapItem->tMagic[2] + pMapItem->tMagic[3];
// 						if(n > 500) {
// 							int iRandom = myrand(1, 100);
// 							//��Ʒ�������
// 							if(iRandom < 30){
// 								pMapItem->tMagic[0] = 0;
// 							}
// 						}
						
					}
					else if(iType == EVENT_ITEM_MOON)	// ������
					{
						iItemNum = EVENTITEM_SID_MOON;
						pMapItem->sLevel = g_arItemTable[iItemNum]->m_byRLevel;
						pMapItem->sSid = g_arItemTable[iItemNum]->m_sSid;
						pMapItem->sDuration = g_arItemTable[iItemNum]->m_sDuration;
						pMapItem->sCount = 1;
						pMapItem->sBullNum = g_arItemTable[iItemNum]->m_sBullNum;
						iWear = g_arItemTable[iItemNum]->m_byWear;
					}
					else if(iType == EVENT_ITEM_SONGPEON)	// ���� �Ǵ� ����
					{
						/*
						if(m_byClassLevel < 11) iItemNum = EVENTITEM_SID_SONGPEON_01;
						else if(m_byClassLevel >= 11 && m_byClassLevel < 31) iItemNum = EVENTITEM_SID_SONGPEON_11;
						else if(m_byClassLevel >= 31 && m_byClassLevel < 51) iItemNum = EVENTITEM_SID_SONGPEON_31;
						else if(m_byClassLevel >= 51 && m_byClassLevel < 71) iItemNum = EVENTITEM_SID_SONGPEON_51;
						else if(m_byClassLevel >= 71) iItemNum = EVENTITEM_SID_SONGPEON_71;
						*/
						iItemNum = EVENTITEM_SID_SONGPEON_01;
						
						pMapItem->sLevel = g_arItemTable[iItemNum]->m_byRLevel;
						pMapItem->sSid = g_arItemTable[iItemNum]->m_sSid;
						pMapItem->sDuration = g_arItemTable[iItemNum]->m_sDuration;
						pMapItem->sCount = 1;
						pMapItem->sBullNum = g_arItemTable[iItemNum]->m_sBullNum;
						iWear = g_arItemTable[iItemNum]->m_byWear;
					}
					else if(iType == EVENT_ITEM_BOX)	// ��������
					{
						iItemNum = EVENTITEM_SID_BOX;
						pMapItem->sLevel = g_arItemTable[iItemNum]->m_byRLevel;
						pMapItem->sSid = g_arItemTable[iItemNum]->m_sSid;
						pMapItem->sDuration = g_arItemTable[iItemNum]->m_sDuration;
						pMapItem->sCount = 1;
						pMapItem->sBullNum = g_arItemTable[iItemNum]->m_sBullNum;
						iWear = g_arItemTable[iItemNum]->m_byWear;
					}
				}
				////////////////////////////////////////////////////////////////////////������
				else if(iItemNum==818 || iItemNum==733 || iItemNum==735 ){
					byte tMagic=0;
					iRandom = myrand(1, 9);
					switch(iRandom){
						case 1: tMagic=78;break;//����5
						case 2: tMagic=107;break;//����5
						case 3: tMagic=108;break;//����5
						case 4: tMagic=109;break;//����5
						case 5: tMagic=110;break;//�ǻ�5
                        //case 6: tMagic=111;break;//����5
						case 6: tMagic=33;break;//�ر�5
						case 7: tMagic=14;break;//ħ������5
						case 8: tMagic=6;break;//����5
						case 9: tMagic=31;break;//����5
					    default: tMagic=0;break;
					}
					pMapItem->tIQ = SET_ITEM;
					pMapItem->tMagic[0] =tMagic;
				}
				 //////////////////////////////////////////////////////////////////////////////
				// �Ǽ��縮 ó��
				else if(iWear >= 6 && iWear <= 8)	// ���ε���
				{
					pMapItem->tIQ = MAGIC_ITEM;	// �Ǽ��縮�� ������ ����
					pMapItem->tMagic[0] = g_arItemTable[iItemNum]->m_bySpecial;
				}
				
				//else if(iItemNum==1043) //��������ɫΪ5
				//{
				//	pMapItem->tIQ = 5;	
				//}
				//else if(iWear == 126) //��ʯ����....���������.
				//{
				//	iRandom = myrand(1, 1000);
				//	for(i = 0; i < g_arEBodyTable.GetSize(); i++)
				//	{
				//		if(iRandom <= g_arEBodyTable[i]->m_sRandom) 
				//		{
				//			tEBodySid = g_arEBodyTable[i]->m_tSid;
				//			break;
				//		}	
				//	}
				//	pMapItem->tIQ = MAGIC_ITEM;	//
				//	pMapItem->tMagic[0] = tEBodySid;
				//}
				else if(iWear == 126) //��ʯ����....���������. BOSSֻ��������
				{
					if(  m_sEvent== 30007 ||  m_sEvent== 30009  )
					{
						byte tMagic=0;
					    iRandom = myrand(1, 7);
						switch(iRandom){
						case 1: tMagic=5;break; //5��
						case 2: tMagic=14;break;//�ͷ�2
						case 3: tMagic=16;break;//10����
						case 4: tMagic=22;break;//10����
						case 5: tMagic=23;break;//10�ر�
						case 6: tMagic=24;break;//10����
						case 7: tMagic=27;break;//10�ǻ�
						default: tMagic=0;break;}
						
						pMapItem->tIQ = MAGIC_ITEM;	//
						pMapItem->tMagic[0] = tMagic;
					}else{

						iRandom = myrand(1, 1000);
						for(i = 0; i < g_arEBodyTable.GetSize(); i++)
						{
							if(iRandom <= g_arEBodyTable[i]->m_sRandom) 
							{
								tEBodySid = g_arEBodyTable[i]->m_tSid;
								break;
							}	
						}
						pMapItem->tIQ = MAGIC_ITEM;	//
						pMapItem->tMagic[0] = tEBodySid;
					}
				}
				
				else if(iWear==130)
				{
					shouhu_rand(pMapItem);

				}else if(iWear==143){//��ʯ
					byte tMagic=0;
					iRandom = myrand(1, 6);
					switch(iRandom){
						case 1: tMagic=3;break;
						case 2: tMagic=13;break;
						case 3: tMagic=23;break;
						case 4: tMagic=43;break;
						case 5: tMagic=53;break;
						case 6: tMagic=63;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
					pMapItem->tMagic[5]= 1;

				}else if(iItemNum==1051){//����оƬ
					byte tMagic=0;
					iRandom = myrand(1, 3);
					switch(iRandom){
						case 1: tMagic=5;break;//15��
						case 2: tMagic=12;break;//15����
						case 3: tMagic=6;break;//15����
				        
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
				
				}else if(iWear==166){//���򿨵����������
					byte tMagic=0;
					iRandom = myrand(1, 44);
					switch(iRandom){
						case 1: tMagic=1;break;
						case 2: tMagic=2;break;
						case 3: tMagic=3;break;
						case 4: tMagic=4;break;
						case 5: tMagic=5;break;
						case 6: tMagic=6;break;
						case 7: tMagic=7;break;
						case 8: tMagic=8;break;
						case 9: tMagic=9;break;
						case 10: tMagic=10;break;
						case 11: tMagic=11;break;
						case 12: tMagic=12;break;
						case 13: tMagic=13;break;
						case 14: tMagic=14;break;
						case 15: tMagic=15;break;
						case 16: tMagic=16;break;
						case 17: tMagic=17;break;
						case 18: tMagic=18;break;
						case 19: tMagic=19;break;
						case 20: tMagic=20;break;
						case 21: tMagic=21;break;
						case 22: tMagic=22;break;
						case 23: tMagic=23;break;
						case 24: tMagic=24;break;
						case 25: tMagic=25;break;
						case 26: tMagic=26;break;
						case 27: tMagic=27;break;
						case 28: tMagic=38;break;
						case 29: tMagic=29;break;
						case 30: tMagic=30;break;
						case 31: tMagic=31;break;
						case 32: tMagic=32;break;
						case 33: tMagic=33;break;
						case 34: tMagic=34;break;
						case 35: tMagic=35;break;
						case 36: tMagic=36;break;
						case 37: tMagic=37;break;
						case 38: tMagic=38;break;
						case 39: tMagic=39;break;
						case 40: tMagic=40;break;
						case 41: tMagic=41;break;
						case 42: tMagic=42;break;
						case 43: tMagic=43;break;
						case 44: tMagic=44;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
				
				}else if(iWear==167){//���ͼ���ϻ����漴��������
					byte tMagic=0;
					iRandom = myrand(1, 6);
					switch(iRandom){
						case 1: tMagic=1;break;
						case 2: tMagic=2;break;
						case 3: tMagic=3;break;
						case 4: tMagic=4;break;
						case 5: tMagic=5;break;
						case 6: tMagic=6;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
				
				}else if(iWear==168){//���ͼ����оƬ�漴��������
					byte tMagic=0;
					iRandom = myrand(1, 6);
					switch(iRandom){
						case 1: tMagic=1;break;
						case 2: tMagic=2;break;
						case 3: tMagic=3;break;
						case 4: tMagic=4;break;
						case 5: tMagic=5;break;
						case 6: tMagic=6;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
				
				}else if(iWear==169){//���ͼ���ϻ�·�漴��������
					byte tMagic=0;
					iRandom = myrand(1, 6);
					switch(iRandom){
						case 1: tMagic=1;break;
						case 2: tMagic=2;break;
						case 3: tMagic=3;break;
						case 4: tMagic=4;break;
						case 5: tMagic=5;break;
						case 6: tMagic=6;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
				
				}else if(iWear==170){//���׸�����ɪ�漴��������
					byte tMagic=0;
					iRandom = myrand(1, 3);
					switch(iRandom){
						case 1: tMagic=1;break;
						case 2: tMagic=2;break;
						case 3: tMagic=3;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
				
				}else if(iWear==171){//���׸�����Ƭ�漴��������
					byte tMagic=0;
					iRandom = myrand(1, 3);
					switch(iRandom){
						case 1: tMagic=1;break;
						case 2: tMagic=2;break;
						case 3: tMagic=3;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
				
				}else if(iWear==172){//���׸����ⶩ�漴��������
					byte tMagic=0;
					iRandom = myrand(1, 3);
					switch(iRandom){
						case 1: tMagic=1;break;
						case 2: tMagic=2;break;
						case 3: tMagic=3;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
				
				}else if(iWear==173){//���׸�������̹���漴��������
					byte tMagic=0;
					iRandom = myrand(1, 3);
					switch(iRandom){
						case 1: tMagic=1;break;
						case 2: tMagic=2;break;
						case 3: tMagic=3;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
				
				}else if(iItemNum==1178){   //�����̶�����
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =4;
				
				}else if(iItemNum==1179){  //����̶�����
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =5;
				
				}else if(iItemNum==987){
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =59;

				}
			}
		}		

		// �ش� �������� �˸���.			
	//	pCom->DelThrowItem();
		pCom->SetThrowItem( pMapItem, pt.x, pt.y, m_ZoneIndex );

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_FieldUse, 0);
	}
}

//////////////////////////////////////////////////////////////////////////////////
//	���� ��ġ�� �߽����� 25 ���� �����۸� ���������ִ� ��ǥ�� �������� ����
//
CPoint CNpc::FindNearRandomPoint(int x, int y)
{
	CPoint t;
	int i;
	int iX, iY;
	int rand_x = 1, rand_y = 1;

	MAP *pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return CPoint(-1, -1);
	if( !pMap->m_pMap ) return CPoint(-1, -1);

	int dir[25][2];

	//	X					Y
	dir[0][0]  =  0;		dir[0][1] =  0;		// 
	dir[1][0]  = -1;		dir[1][1] =  0;		// 
	dir[2][0]  = -1;		dir[2][1] =  1;		// 
	dir[3][0]  =  0;		dir[3][1] =  1;		// 
	dir[4][0]  =  1;		dir[4][1] =  1;		// 

	dir[5][0]  =  1;		dir[5][1] =  0;		// 
	dir[6][0]  =  1;		dir[6][1] = -1;		// 
	dir[7][0]  =  0;		dir[7][1] = -1;		// 
	dir[8][0]  = -1;		dir[8][1] = -1;		// 
	dir[9][0]  = -2;		dir[9][1] = -1;		// 

	dir[10][0] = -2;		dir[10][1] =  0;	// 
	dir[11][0] = -2;		dir[11][1] =  1;	// 
	dir[12][0] = -2;		dir[12][1] =  2;	// 
	dir[13][0] = -1;		dir[13][1] =  2;	// 
	dir[14][0] =  0;		dir[14][1] =  2;	// 

	dir[15][0] =  1;		dir[15][1] =  2;	// 
	dir[16][0] =  2;		dir[16][1] =  2;	// 
	dir[17][0] =  2;		dir[17][1] =  1;	// 
	dir[18][0] =  2;		dir[18][1] =  0;	// 
	dir[19][0] =  2;		dir[19][1] = -1;	// 

	dir[20][0] =  2;		dir[20][1] = -2;	// 
	dir[21][0] =  1;		dir[21][1] = -2;	// 
	dir[22][0] =  0;		dir[22][1] = -2;	// 
	dir[23][0] = -1;		dir[23][1] = -2;	// 
	dir[24][0] = -2;		dir[24][1] = -2;	// 

	rand_x = myrand(1, 8, TRUE);
	rand_y = myrand(0, 1, TRUE);

	iX = dir[rand_x][rand_y] + x;
	iY = dir[rand_x][rand_y] + y;

	rand_x = iX; rand_y = iY;
		
	if(rand_x >= pMap->m_sizeMap.cx || rand_x < 0 || rand_y >= pMap->m_sizeMap.cy || rand_y < 0) return CPoint(-1, -1);

	if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].m_lUser == 0)
	{
		if( IsMovable( rand_x, rand_y ) )
		{
//			if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].iIndex == -1) return CPoint( rand_x, rand_y );
			return CPoint( rand_x, rand_y );
		}
	}

	rand_x = x, rand_y = y;

	for( i = 1; i < 25; i++)
	{
		iX = rand_x + dir[i][0];
		iY = rand_y + dir[i][1];

		if( iX >= pMap->m_sizeMap.cx || iX < 0 || iY >= pMap->m_sizeMap.cy || iY < 0) continue;

		if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].m_lUser != 0) continue;	// �� ������ Ȯ���Ѵ�.

		if( IsMovable( iX, iY ) )
		{
//			if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].iIndex == -1) return CPoint( iX, iY );
			return CPoint( iX, iY );
		}
	}

	return CPoint(-1, -1);
}

CPoint CNpc::FindNearRandomPointForItem(int x, int y)
{
	CPoint t;
	int i;
	int iX, iY;
	int rand_x = 1, rand_y = 1;

	MAP *pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return CPoint(-1, -1);
	if( !pMap->m_pMap ) return CPoint(-1, -1);

	int dir[25][2];

	//	X					Y
	dir[0][0]  =  0;		dir[0][1] =  0;		// 
	dir[1][0]  = -1;		dir[1][1] =  0;		// 
	dir[2][0]  = -1;		dir[2][1] =  1;		// 
	dir[3][0]  =  0;		dir[3][1] =  1;		// 
	dir[4][0]  =  1;		dir[4][1] =  1;		// 

	dir[5][0]  =  1;		dir[5][1] =  0;		// 
	dir[6][0]  =  1;		dir[6][1] = -1;		// 
	dir[7][0]  =  0;		dir[7][1] = -1;		// 
	dir[8][0]  = -1;		dir[8][1] = -1;		// 
	dir[9][0]  = -2;		dir[9][1] = -1;		// 

	dir[10][0] = -2;		dir[10][1] =  0;	// 
	dir[11][0] = -2;		dir[11][1] =  1;	// 
	dir[12][0] = -2;		dir[12][1] =  2;	// 
	dir[13][0] = -1;		dir[13][1] =  2;	// 
	dir[14][0] =  0;		dir[14][1] =  2;	// 

	dir[15][0] =  1;		dir[15][1] =  2;	// 
	dir[16][0] =  2;		dir[16][1] =  2;	// 
	dir[17][0] =  2;		dir[17][1] =  1;	// 
	dir[18][0] =  2;		dir[18][1] =  0;	// 
	dir[19][0] =  2;		dir[19][1] = -1;	// 

	dir[20][0] =  2;		dir[20][1] = -2;	// 
	dir[21][0] =  1;		dir[21][1] = -2;	// 
	dir[22][0] =  0;		dir[22][1] = -2;	// 
	dir[23][0] = -1;		dir[23][1] = -2;	// 
	dir[24][0] = -2;		dir[24][1] = -2;	// 

	rand_x = myrand(1, 8, TRUE);
	rand_y = myrand(0, 1, TRUE);

	iX = dir[rand_x][rand_y] + x;
	iY = dir[rand_x][rand_y] + y;

	rand_x = iX; rand_y = iY;
		
	if(rand_x >= pMap->m_sizeMap.cx || rand_x < 0 || rand_y >= pMap->m_sizeMap.cy || rand_y < 0) return CPoint(-1, -1);

//	if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].m_lUser == 0)
//	{
//		if( IsMovable( rand_x, rand_y ) )
//		{
//			if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].iIndex == -1) return CPoint( rand_x, rand_y );
//		}
//	}

	if( g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].m_FieldUse == 0 )
	{
		if( g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].iIndex == -1 ) return CPoint( rand_x, rand_y );
	}

	rand_x = x, rand_y = y;

	for( i = 1; i < 25; i++)
	{
		iX = rand_x + dir[i][0];
		iY = rand_y + dir[i][1];

		if( iX >= pMap->m_sizeMap.cx || iX < 0 || iY >= pMap->m_sizeMap.cy || iY < 0) continue;

//		if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].m_lUser != 0) continue;	// �� ������ Ȯ���Ѵ�.
		if( g_zone[m_ZoneIndex]->m_pMap[iX][iY].m_FieldUse != 0 ) continue;	// ��������� üũ.

//		if( IsMovable( iX, iY ) )
//		{
//			if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].iIndex == -1) return CPoint( iX, iY );
//		}
		if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].iIndex == -1) return CPoint( iX, iY );
	}

	return CPoint(-1, -1);
}

///////////////////////////////////////////////////////////////////////////////////
//	x, y �� ������ �� �ִ� ��ǥ���� �Ǵ�
//
BOOL CNpc::IsMovable(int x, int y)
{
	if(x < 0 || y < 0 ) return FALSE;

	if(!g_zone[m_ZoneIndex] ) return FALSE;
	if(!g_zone[m_ZoneIndex]->m_pMap) return FALSE;
	if(x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return FALSE;
	if(g_zone[m_ZoneIndex]->m_pMap[x][y].m_bMove || g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser) return FALSE;

	return TRUE;
}
			
//////////////////////////////////////////////////////////////////////////////////////////////
//	NPC ���� ����
//
void CNpc::SendAttackSuccess(COM *pCom, int tuid, BOOL bIsCritical, short sHP, short sMaxHP)
{
	if(pCom == NULL) return;

	CBufferEx TempBuf;
//	CByteArray arAction1;
//	CByteArray arAction2;
	
	TempBuf.Add(ATTACK_RESULT);
	//--------------------------------------------------
	//yskang 0.3 NPC�� ũ��Ƽ�� ������ ����.
	//--------------------------------------------------
	TempBuf.Add(ATTACK_SUCCESS);
	TempBuf.Add((int)(m_sNid + NPC_BAND));
	TempBuf.Add(tuid);
	//----------------------------------------------------

/*	BYTE tAction1 = (BYTE)arAction1.GetSize();
	BYTE tAction2 = (BYTE)arAction2.GetSize();
	int i = 0;

	TempBuf.Add(tAction1);
	if(tAction1 > 0)
	{
		for(i = 0; i < arAction1.GetSize(); i++)
		{
			TempBuf.Add(arAction1[i]);
		}
	}
	TempBuf.Add(tAction2);
	if(tAction2 > 0)
	{
		for(i = 0; i < arAction2.GetSize(); i++)
		{
			TempBuf.Add(arAction2[i]);
		}
	}
	
*/	
	TempBuf.Add((int)sHP);
	TempBuf.Add((int)sMaxHP);

//	SendInsight(pCom, TempBuf, TempBuf.GetLength());
	SendExactScreen(pCom, TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////
//	NPC ���� �̽�
//
void CNpc::SendAttackMiss(COM *pCom, int tuid)
{
	CBufferEx TempBuf;
	
	TempBuf.Add(ATTACK_RESULT);
	TempBuf.Add(ATTACK_MISS);
	TempBuf.Add((int)(m_sNid + NPC_BAND));
	TempBuf.Add(tuid);

//	SendInsight(pCom, TempBuf, TempBuf.GetLength());
	SendExactScreen(pCom, TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////
//	NPC �� ���� �迭�� ���Ѵ�.
//
BYTE CNpc::GetWeaponClass()
{
	BYTE tClass = BRAWL;

	switch (m_byClass)
	{
	case 1:
		tClass = BRAWL;
		break;
		
	case 2:
		tClass = STAFF;
		break;
		
	case 4:
		tClass = EDGED;
		break;
		
	case 8:
		tClass = FIREARMS;
		break;
	}

	return tClass;
}

/////////////////////////////////////////////////////////////////////////////////////
//	���ݽø��� ��ų���� ���θ� üũ�Ѵ�.
//
void CNpc::IsSkillSuccess(BOOL *bSuccess)
{
	int iOnCount = 0;
	int i = 0;
	
	for(i = 0; i < SKILL_NUM; i++) bSuccess[i] = FALSE;

	for(i = 0; i < SKILL_NUM; i++) 
	{
		if(m_NpcSkill[i].tOnOff == 1) iOnCount++;
	}

	int iRandom = XdY(1, 100);
	int iRate = 0;

	for(i = 0; i < SKILL_NUM; i++)
	{
		iRate = (int)((double)m_sWIS * 0.5 + m_NpcSkill[i].tLevel * 2 - iOnCount * 25 + 50 /* +Magic Bonus*/);		//!Magic

		if(iRandom <= iRate) 
		{
			bSuccess[i] = TRUE;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////
//	�ʱ� �Ϲ� �������� ��´�.
//
int CNpc::GetNormalInitDamage()
{
	int nHit = 0;
	int nDamage = 0;
	int xyz = 0;

	xyz = XdY(m_byAX, m_byAY) + m_byAZ;

//  2002-10-17 by Youn Gyu
//	if(m_byClass == FIREARMS) nHit = (int)((double)m_sDEX/3 + 10.5);
//	else					  nHit = (int)((double)m_sSTR/2 + 0.5);

    if(nHit < 0) nHit = 0;

	nDamage = nHit + xyz;
	return nDamage;
}

///////////////////////////////////////////////////////////////////////////////////////
//	�ʱ� ũ��Ƽ�� �������� ��´�.
//
int CNpc::GetCriticalInitDamage(BOOL *bSuccessSkill)
{
	int nDamage = 0;
	int xyz = 0;

	xyz = XdY(m_byAX, m_byAY) + m_byAZ;
/*
	// �������϶� ��ȹ���� �˷��ٿ���...
*/
	return nDamage;

}
///////////////////////////////////////////////////////////////////////////
//	�ñ� �������� �����Ѵ�.
//
void CNpc::SetColdDamage()
{
	if(m_tAbnormalKind != ABNORMAL_BYTE_NONE) return;		// �̹� �����̻��� �ɷ��ִ� �����̸� ����

	m_tAbnormalKind = ABNORMAL_BYTE_COLD;
	m_dwAbnormalTime = COLD_TIME;
	m_dwLastAbnormalTime = GetTickCount();
}

///////////////////////////////////////////////////////////////////////////
//	ȭ���������� �����Ѵ�.
//
void CNpc::SetFireDamage()
{
	if(m_tAbnormalKind != ABNORMAL_BYTE_NONE) return;		// �̹� �����̻��� �ɷ��ִ� �����̸� ����

	m_tAbnormalKind = ABNORMAL_BYTE_FIRE;
	m_dwAbnormalTime = FIRE_TIME;
	m_dwLastAbnormalTime = GetTickCount();
}

/////////////////////////////////////////////////////////////////////////////
//	Damage ���, ���� m_sHP �� 0 �����̸� ���ó��
//
BOOL CNpc::SetDamage(int nDamage, int uuid, COM *pCom)
{
	if(m_NpcState == NPC_DEAD) return TRUE;
	if(m_sHP <= 0) return TRUE;
	if(nDamage <= 0) return TRUE;

	if(m_tNpcType == NPCTYPE_GUARD) return TRUE;

	if(m_tGuildWar == GUILD_WAR_AFFTER)
	{
		if(m_tNpcType >= NPCTYPE_GUILD_NPC) return TRUE;
//		if(m_tNpcType == NPCTYPE_GUILD_NPC) return TRUE;
//		if(m_tNpcType == NPCTYPE_GUILD_GUARD) return TRUE;
//		if(m_tNpcType == NPCTYPE_GUILD_DOOR) return TRUE;
	}

	if(InterlockedCompareExchange((long*)&m_lDamage, (long)1, (long)0) == (long)0)
	{
		int i;
		int iLen = 0;
		int userDamage = 0;
		ExpUserList *tempUser = NULL;

		int uid = uuid - USER_BAND;

		USER* pUser = GetUser(pCom, uid);
														// �ش� ��������� ����
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) 
		{
			InterlockedExchange(&m_lDamage, (LONG)0);
			return TRUE;
		}

		iLen = strlen(pUser->m_strUserID);
		if(iLen <= 0 || iLen > CHAR_NAME_LENGTH) 
		{
			InterlockedExchange(&m_lDamage, (LONG)0);
			return TRUE;
		}

		if(m_tGuildWar == GUILD_WARRING)// ������� �������� ������ ���� ���
		{
			if(m_tNpcType == NPCTYPE_GUILD_NPC)		// ������ ���������� NPC�� ���� �ٸ� ����� ����� ����
			{
//				if(m_pGuardStore) { SetDamagedInGuildWar(nDamage, pUser); InterlockedExchange(&m_lDamage, (LONG)0); return TRUE; }
				if(m_pGuardFortress) { SetDamagedInFortressWar(nDamage, pUser); InterlockedExchange(&m_lDamage, (LONG)0); return TRUE; }
			}
			else if(m_tNpcType == NPCTYPE_GUILD_DOOR)	// �������� ���� Ư����	  
			{
				if(m_pGuardFortress) { SetDoorDamagedInFortressWar(nDamage, pUser); InterlockedExchange(&m_lDamage, (LONG)0); return TRUE; }
			}
			else if(m_tNpcType == NPCTYPE_GUILD_GUARD)	// �� ��忡 ���� ����� �ڱ�������κ��� ��ȣ(?)�ޱ�����
			{
				if(pUser->m_dwGuild > 0)
				{
/*					if(m_pGuardStore) 
					{ 
						if(m_pGuardStore->m_iGuildSid == pUser->m_dwGuild)
						{
							InterlockedExchange(&m_lDamage, (LONG)0);
							return TRUE; 
						}
*/					if(m_pGuardFortress) 
					{ 
						if(m_pGuardFortress->m_iGuildSid == pUser->m_dwGuild)
						{
							InterlockedExchange(&m_lDamage, (LONG)0); 
							return TRUE; 
						}
					}
				}
			}
		}

		//if ( pUser->m_byClass==1)
		// {

		//if( m_sEvent == NPC_EVENT_GREATE_MOP || m_sEvent== NPC_EVENT_MOP|| m_sEvent== 30009 || m_sEvent== 30007)		 //�����BOSS����ֻ��ƽʱ��50%.
		//{
		//	nDamage = (int)( (double)nDamage * 0.8 );
		//}
		//else
		//	nDamage = (int)( (double)nDamage * 0.8 );

	 //   }

		userDamage = nDamage;
																
		if( (m_sHP - nDamage) < 0 ) userDamage = m_sHP;

		for(i = 0; i < NPC_HAVE_USER_LIST; i++)
		{
			if(m_DamagedUserList[i].iUid == uid)
			{
				if(strcmp(m_DamagedUserList[i].strUserID, pUser->m_strUserID) == 0) 
				{ 
					m_DamagedUserList[i].nDamage += userDamage; 
					goto go_result;
				}
			}
		}

		for(i = 0; i < NPC_HAVE_USER_LIST; i++)				// �ο� ������ ���� ������� ������ ��ġ��?
		{
			if(m_DamagedUserList[i].iUid == -1)
			{
				if(m_DamagedUserList[i].nDamage <= 0)
				{
					strncpy(m_DamagedUserList[i].strUserID, pUser->m_strUserID, iLen);
					m_DamagedUserList[i].iUid = uid;
					m_DamagedUserList[i].nDamage = userDamage;
					m_DamagedUserList[i].bIs = FALSE;
					break;
				}
			}
		}

go_result:
		m_TotalDamage += userDamage;
		m_sHP -= nDamage;
		
		if( m_sHP <= 0 )
		{
			UserListSort();							// �������� ������ ������

			m_ItemUserLevel = pUser->m_sLevel;
			m_sHP = 0;

			InterlockedExchange(&m_lKillUid, (LONG)uid);
			if(m_sPid==190)
				m_sEvent=2;
			// ���� ���� ����Ʈ�� ���� �̺�Ʈ ���ΰ�� �ش� �̺�Ʈ�� ����
			if(m_sEvent > 0 && m_sEvent <= NPC_QUEST_MOP)	
				pUser->RunQuestEvent(this, m_sCurZ, m_sEvent);
			Dead();
			InterlockedExchange(&m_lDamage, (LONG)0);
			return FALSE;
		}

		ChangeTarget(pUser, pCom);

		InterlockedExchange(&m_lDamage, (LONG)0);
	}
	return TRUE;
}

BOOL CNpc::CheckNpcRegenCount()
{
/*	if(m_NpcState != NPC_DEAD) return FALSE;

	QueryPerformanceCounter((LARGE_INTEGER*)&m_RegenLastCount);

	if((m_RegenLastCount - m_RegenStartCount) >= g_Online_Update_Min_ticks)
	{
		m_RegenStartCount += g_Online_Update_Min_ticks;
		m_RegenCount += 10000;
	}

	if(m_RegenCount >= (DWORD)m_sRegenTime) return TRUE;
*/
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
//	Ÿ���� �ѷ� �׿� ������ ���� Ÿ���� ã�´�.
//
BOOL CNpc::IsSurround(int targetx, int targety)
{
	if(m_tNpcLongType) return FALSE;		//���Ÿ��� ���

	for(int i = 0; i < (sizeof(surround_x) / sizeof(surround_x[0])); i++)		// �ֺ� 8����
	{
		if(IsMovable(targetx + surround_x[i], targety + surround_y[i])) return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	���� ������ ������ Ÿ������ ��´�.(���� : ���� HP�� �������� ����)
//
void CNpc::ChangeTarget(USER *pUser, COM* pCom)
{
	if(pCom == NULL) return;
	int preDamage, lastDamage;
	int dist;

	if(m_byAX == 0 && m_byAZ == 0 ) return;		// ���� ���ݷ��� ������ �������� �ʴ´�
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_bLive == USER_DEAD) return;
	if(pUser->m_tIsOP == 1) return;		// ��ڴ� ����...^^
	if(pUser->m_bPShopOpen == TRUE) return;
    if(pUser->m_bSessionOnline == true)//���ﲻ�������߹һ�����
		return;
	USER *preUser = NULL;
	preUser = GetUser(pCom, m_Target.id - USER_BAND);

	if(pUser == preUser) return;

	if(preUser != NULL && preUser->m_state == STATE_GAMESTARTED)
	{
		if(strcmp(pUser->m_strUserID, preUser->m_strUserID) == 0) return;
		
		preDamage = 0; lastDamage = 0;
		preDamage = GetFinalDamage(preUser, 0);
		lastDamage = GetFinalDamage(pUser, 0); 

		dist = abs(preUser->m_curx - m_sCurX) + abs(preUser->m_cury - m_sCurY);
		if(dist == 0) return;
		preDamage = (int)((double)preDamage/dist + 0.5);
		dist = abs(pUser->m_curx - m_sCurX) + abs(pUser->m_cury - m_sCurY);
		if(dist == 0) return;
		lastDamage = (int)((double)lastDamage/dist + 0.5);

		if(preDamage > lastDamage) return;
	}
		
	m_Target.id	= pUser->m_uid + USER_BAND;
	m_Target.x	= pUser->m_curx;
	m_Target.y	= pUser->m_cury;

/*	if(pUser->m_strUserID != NULL)
	{
		m_Target.nLen = strlen(pUser->m_strUserID);

		if(m_Target.nLen <= CHAR_NAME_LENGTH) strncpy(m_Target.szName, pUser->m_strUserID, m_Target.nLen);
		else								  ::ZeroMemory(m_Target.szName, sizeof(m_Target.szName));
	}
*/										// ��� �Ÿ��µ� �����ϸ� �ٷ� �ݰ�
	if(m_NpcState == NPC_STANDING || m_NpcState == NPC_MOVING)
	{									// ������ ������ �ݰ����� �̾�����
		if(IsCloseTarget(pUser, m_byRange) == TRUE)
		{
			m_NpcState = NPC_FIGHTING;
			NpcFighting(pCom);
		}
		else							// �ٷ� �������� ��ǥ�� �����ϰ� ����	
		{
			if(GetTargetPath(pCom) == TRUE)	// �ݰ� ������ �ణ�� ������ �ð��� ����	
			{
				m_NpcState = NPC_TRACING;
				NpcTracing(pCom);
			}
			else
			{
				ToTargetMove(pCom, pUser);
			}
		}
	}
//	else m_NpcState = NPC_ATTACKING;	// ���� �����ϴµ� ���� �����ϸ� ��ǥ�� �ٲ�

	if(m_tNpcGroupType)					// ����Ÿ���̸� �þ߾ȿ� ���� Ÿ�Կ��� ��ǥ ����
	{
		m_Target.failCount = 0;
		FindFriend();
	}
}

/////////////////////////////////////////////////////////////////////////////
//	NPC ���º��� ��ȭ�Ѵ�.
//
void CNpc::NpcLive(COM *pCom)
{
	if(SetLive(pCom))
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;
        if(m_sEvent == 30009)
		{
			if(m_sCurZ == 36)
			{
				CString strMsg;
				strMsg.Format("ˢ����ʾ���� %s ���� [ �糡���� ] ˢ����!", m_strName);//������ʾ
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			
			
			if(m_sCurZ == 16)
			{
				CString strMsg;
				strMsg.Format("ˢ����ʾ���� %s ����  [ ɳ�涴Ѩ ] ˢ����!", m_strName);//������ʾ
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 19)
			{
				CString strMsg;
				strMsg.Format("ˢ����ʾ���� %s ����  [ ������Ѩ ] ˢ����!", m_strName);//������ʾ
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 400)
			{
				CString strMsg;
				strMsg.Format("ˢ����ʾ���� %s ����  [ �ϻ����� ] ˢ����!", m_strName);//������ʾ
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 49)
			{
				CString strMsg;
				strMsg.Format("ˢ����ʾ���� %s ����  [ ��ɽ�� ] ˢ����!", m_strName);//������ʾ
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 12)
			{
				CString strMsg;
				strMsg.Format("ˢ����ʾ���� %s ����  [ ��ԭ���� ] ˢ����!", m_strName);//������ʾ
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			
				
		}
	}
	else
	{
		m_NpcState = NPC_LIVE;
		m_Delay = m_sStandTime * 10;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	NPC�� ���ִ°��.
//
void CNpc::NpcStanding(COM *pCom)
{
	NpcTrace(_T("NpcStanding()"));

	if(RandomMove(pCom) == TRUE)
	{
		m_NpcState = NPC_MOVING;

		if( m_sStandTime > 2500 )
		{
			m_Delay = m_sStandTime - 2000;
		}
		else
		{
			m_Delay = m_sStandTime;
		}

//		m_Delay = m_sStandTime;
//		m_Delay = m_sSpeed;		 // 2001-09-01, jjs07 
		return;
	}

	m_NpcState = NPC_STANDING;

	m_Delay = m_sStandTime;
}

/////////////////////////////////////////////////////////////////////////////
//	NPC�� �̵��ϴ� ���.
//
void CNpc::NpcMoving(COM *pCom)
{
	NpcTrace(_T("NpcMoving()"));

	if(m_sHP <= 0) 
	{
		Dead();
		return;
	}

	if(FindEnemy(pCom) == TRUE)		// ���� ã�´�. 
	{
		if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD) 
		{ 
			m_NpcState = NPC_FIGHTING; 
			m_Delay = 0; 
		}
		else 
		{ 
			m_NpcState = NPC_ATTACKING;
			m_Delay = m_sSpeed;
		}
		return;
	}

	if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD) // �̵����ϰ�...
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;
		return;
	}

	if(IsMovingEnd())				// �̵��� ��������
	{
		m_NpcState = NPC_STANDING;

		//���� �ۿ� ������ ���ִ� �ð��� ª��...
		if(IsInRange())	m_Delay = m_sStandTime;
		else m_Delay = m_sStandTime - 1000;

		if(m_Delay < 0) m_Delay = 0;

		return;
	}

	if(StepMove(pCom, 1) == FALSE)	// ��ĭ ������(�ȴµ���, �޸����� 2ĭ)
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;
		return;
	}

	CBufferEx TempBuf;

	CPoint t = ConvertToClient( m_sCurX, m_sCurY );		// �����̷��� ������ǥ�� Ŭ���̾�Ʈ���� �������̴� ��ǥ�� ����

	if(IsStepEnd())	TempBuf.Add(MOVE_END_RESULT);
	else			TempBuf.Add(MOVE_RESULT);

	TempBuf.Add(SUCCESS);
	TempBuf.Add((int)(NPC_BAND + m_sNid));
	TempBuf.Add((short)t.x);
	TempBuf.Add((short)t.y);

	SendInsight(pCom, TempBuf, TempBuf.GetLength());

	m_Delay = m_sSpeed;
}

/////////////////////////////////////////////////////////////////////////////
//	NPC�� �����ϴ°��.
//
void CNpc::NpcAttacking(COM *pCom)
{
	NpcTrace(_T("NpcAttacking()"));
	
	int ret = 0;

	if( m_byPsi > 0 && m_byPsi < g_arMonsterPsi.GetSize() )	// ������ �ִ� ���̶��...
	{
		CMonsterPsi* pMagic = g_arMonsterPsi[(int)m_byPsi];

		if( pMagic )
		{
			if( IsCloseTarget( pCom, pMagic->m_byRange ) )
			{
				m_NpcState = NPC_FIGHTING;
				m_Delay = 0;
				return;
			}
		}
	}

	if(IsCloseTarget(pCom, m_byRange))	// ������ �� �ִ¸�ŭ ����� �Ÿ��ΰ�?
	{
		m_NpcState = NPC_FIGHTING;
		m_Delay = 0;
		return;
	}

	if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD)// ���ִ� ����϶� ������ �����ϸ� ��� ���� �������Ѵ�. 
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime/2;
		return;
	}

	if(GetTargetPath(pCom) == FALSE)
	{
		if(RandomMove(pCom) == FALSE)
		{
			m_NpcState = NPC_STANDING;
			m_Delay = m_sStandTime;
			return;
		}

		m_NpcState = NPC_MOVING;
		m_Delay = m_sSpeed;
		return;
	}

	m_NpcState = NPC_TRACING;
	m_Delay = 0;
}

/////////////////////////////////////////////////////////////////////////////
//	NPC�� ������ �����ϴ� ���.
//
void CNpc::NpcTracing(COM *pCom)
{
	NpcTrace(_T("NpcTracing()"));

	if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD) return;

	if(GetUser(pCom, (m_Target.id - USER_BAND)) == NULL)	// Target User �� �����ϴ��� �˻�
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;
		return;
	}

	if( m_byPsi > 0 && m_byPsi < g_arMonsterPsi.GetSize() )	// ������ �ִ� ���̶��...
	{
		CMonsterPsi* pMagic = g_arMonsterPsi[(int)m_byPsi];

		if( pMagic )
		{
			if( IsCloseTarget( pCom, pMagic->m_byRange ) )
			{
				m_NpcState = NPC_FIGHTING;
				m_Delay = 0;
				return;
			}
		}
	}

	if(IsCloseTarget(pCom, m_byRange))						// �������� ���ϸ�ŭ ����� �Ÿ��ΰ�?
	{
		m_NpcState = NPC_FIGHTING;
		m_Delay = 0;
		return;
	}

	if(IsSurround(m_Target.x, m_Target.y))					// ��ǥ Ÿ���� �ѷ��׿� ������ ���� Ÿ���� ã�´�.
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;
		return;
	}

	if(IsChangePath(pCom))									// ��ã�⸦ �ٽ� �Ҹ�ŭ Target �� ��ġ�� ���ߴ°�?
	{
		if(ResetPath(pCom) == FALSE)// && !m_tNpcTraceType)
		{
			m_NpcState = NPC_STANDING;
			m_Delay = m_sStandTime;
			return;
		}
	}
	
	if(StepMove(pCom, 1) == FALSE)							// ��ĭ ������(�ȴµ���, �޸����� 2ĭ)
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;	
		return;
	}

	CBufferEx TempBuf;

	CPoint t = ConvertToClient( m_sCurX, m_sCurY );		// �����̷��� ������ǥ�� Ŭ���̾�Ʈ���� �������̴� ��ǥ�� ����

	if(IsStepEnd())	TempBuf.Add(MOVE_END_RESULT);
	else			TempBuf.Add(MOVE_RESULT);

	TempBuf.Add(SUCCESS);
	TempBuf.Add((int)(NPC_BAND + m_sNid));
	TempBuf.Add((short)t.x);
	TempBuf.Add((short)t.y);

	SendInsight(pCom, TempBuf, TempBuf.GetLength());

	m_Delay = m_sSpeed;
}

/////////////////////////////////////////////////////////////////////////////
//	���� �ӵ� ��ȭ�� �˷� �ش�.
//
void CNpc::ChangeSpeed(COM *pCom, int delayTime)
{
/*	CBufferEx TempBuf;

	int tempTime = delayTime * NPC_TRACING_STEP;
	
	if(m_Delay > m_sSpeed) m_Delay = m_sSpeed;// ���ĵ� �ð��� 1000�ϰ�� 

	m_Delay = m_Delay + tempTime;			// 10, 50, 100������ ��.��
	
	if(m_Delay <= 500) m_Delay = 500;		// �ּҴ� �׻� 500��	
											// 500�� 100%�̸� 600�� 80���� ����	
	short step = 100 - (m_Delay - 500) * 10/50;

	TempBuf.Add(SET_SPEED_MONSTER);

	TempBuf.Add(m_sNid + NPC_BAND);
	TempBuf.Add(step);

		// NPC ������ �������� ��������
	CPoint ptOld;
	if(SightRecalc(ptOld))
	{
		SendRemainSight(pCom, TempBuf, TempBuf.GetLength(), ptOld);
	}
	else SendInsight(pCom, TempBuf, TempBuf.GetLength());
*/
}

/////////////////////////////////////////////////////////////////////////////
//	NPC�� �����ϴ� ���.
//  ��ħ���Ĺ���
void CNpc::NpcFighting(COM *pCom)
{
	NpcTrace(_T("NpcFighting()"));

	if(m_sHP <= 0) 
	{
		Dead();
		return;
	}

	m_dwDelayCriticalDamage = 0;

	int at_type_total3 = m_iNormalATRatio + m_iSpecialATRatio + m_iMagicATRatio;	
	int at_type[300], i;
	for( i = 0; i < m_iNormalATRatio; i++ ) at_type[i] = 1;
	int rand_index = m_iNormalATRatio;
	for( i = rand_index; i < rand_index+m_iSpecialATRatio; i++ )	at_type[i] = 2;
	rand_index += m_iSpecialATRatio;
	for( i = rand_index; i < rand_index+m_iMagicATRatio; i++ )	at_type[i] = 3;
	int at_type_rand = myrand( 0, at_type_total3);	

	if( at_type[at_type_rand] == 3 && m_byPsi > 0)
	{		
		m_Delay = PsiAttack( pCom );				
		if( m_Delay == -1 )
		{
			m_NpcState = NPC_ATTACKING;
			m_Delay = m_sSpeed;				
		}											
		return;
	}
	else if( at_type[at_type_rand] == 2 )
	{
			m_Delay = AreaAttack( pCom );		
			if( m_Delay == -1 )
			{
				m_NpcState = NPC_ATTACKING;
				m_Delay = m_sSpeed;
			}
			return;
	}else	m_Delay = Attack(pCom);		 	
}


/////////////////////////////////////////////////////////////////////////////
//	Ÿ�ٰ��� �Ÿ��� �����Ÿ� ������ �����Ѵ�.(������)
//
void CNpc::NpcBack(COM *pCom)
{
	NpcTrace(_T("NpcBack()"));

	if(GetUser(pCom, (m_Target.id - USER_BAND)) == NULL)	// Target User �� �����ϴ��� �˻�
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sSpeed;//STEP_DELAY;
		return;
	}

	if(IsMovingEnd())								// �̵��� ��������
	{
		m_Delay = m_sSpeed;
		NpcFighting(pCom);
		return;
	}
	
	if(StepMove(pCom, 1) == FALSE)					// ��ĭ ������(�ȴµ���, �޸����� 2ĭ)
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sSpeed;//STEP_DELAY;
		return;
	}

	m_Delay = m_sSpeed;//STEP_DELAY;
}

/////////////////////////////////////////////////////////////////////////////
//	�ٸ� ������ ���踦 ���ؼ�...
//
void CNpc::NpcStrategy(BYTE type)
{
	switch(type)
	{
	case NPC_ATTACK_SHOUT:
		m_NpcState = NPC_TRACING;
		m_Delay = m_sSpeed;//STEP_DELAY;
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	�þ� �������� �����Ḧ ã�´�.
//
void CNpc::FindFriend()
{
	CNpc* pNpc = NULL;

	if(m_bySearchRange == 0) return;

	int min_x, min_y, max_x, max_y;

	min_x = m_sCurX - m_bySearchRange;		if( min_x < 0 ) min_x = 0;
	min_y = m_sCurY - m_bySearchRange;		if( min_y < 0 ) min_y = 0;
	max_x = m_sCurX + m_bySearchRange;
	max_y = m_sCurY + m_bySearchRange;

	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;

	int ix, iy;
	int target_uid;
	int uid;

	int tempLevel = 0, oldLevel = 1000;

	if(m_Target.id == -1) return;

	for(ix = min_x; ix <= max_x; ix++)
	{
		for(iy = min_y; iy <= max_y; iy++)
		{
			target_uid = m_pOrgMap[ix][iy].m_lUser;

			if( target_uid >= NPC_BAND && target_uid < INVALID_BAND)
			{
				uid = target_uid - NPC_BAND;				
				pNpc = g_arNpc[uid];
				if(pNpc == NULL) continue;
									
				if(pNpc->m_tNpcGroupType && m_sNid != uid && pNpc->m_sFamilyType == m_sFamilyType)
				{
//					pNpc->m_Target.nLen = strlen(pNpc->m_Target.szName);	// �̹� ��ǥ�� �־ �̹� �����ϰ� ������...
//					if(pNpc->m_Target.nLen > 0 && pNpc->m_NpcState == NPC_FIGHTING) continue;
					if(pNpc->m_Target.id >= 0 && pNpc->m_NpcState == NPC_FIGHTING) continue;

					pNpc->m_Target.id = m_Target.id;		// ���� Ÿ���� ���ῡ�� ������ ��û�Ѵ�.
					pNpc->m_Target.x = m_Target.x;			// ���� ��ǥ�� �������ڰ�...
					pNpc->m_Target.y = m_Target.y;

/*					if(m_Target.szName != NULL)
					{
						pNpc->m_Target.nLen = strlen(m_Target.szName);

						if(pNpc->m_Target.nLen <= CHAR_NAME_LENGTH) strncpy(pNpc->m_Target.szName, m_Target.szName, pNpc->m_Target.nLen);
						else								  ::ZeroMemory(pNpc->m_Target.szName, sizeof(pNpc->m_Target.szName));
					}
*/					pNpc->m_Target.failCount = 0;
					pNpc->NpcStrategy(NPC_ATTACK_SHOUT);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	Ÿ�������κ��� �ִ��� �ָ� ������ ���� ã�´�.(���� : ���Ÿ��� �ӵ��� ����� �Ѵ�...�ȱ׷��� ��� �Ÿ��� �����ϴ� �״¼��� �ִ�.)
//
BOOL CNpc::GetBackPoint(int &x, int &y)
{
	int ex = m_sCurX;
	int ey = m_sCurY;

	int dx = m_Target.x - m_sCurX;
	int dy = m_Target.y - m_sCurY;

	int min = ( abs(dx) + abs(dy) )/2;
	int max = m_byRange - min;
	int count = myrand(min, max);

	if(count <= 0) return FALSE;							// 0�̸� ���� �����Ÿ��� ����.
	if(count >= m_byRange && count > 2) count -= 1;			// Ȥ�ó� ���� �ִ��� �����Ѵ�.

	if(dy > 0)
	{
		if(dx > 0)		{ ex -= count; ey -= count; }
		else if(dx < 0)	{ ex += count; ey -= count; }
		else			{ ey -= (count*2); }				// Ȧ��, ¦���� �����.
	}
	else if(dy < 0)
	{
		if(dx > 0)		{ ex -= count; ey += count; }
		else if(dx < 0)	{ ex += count; ey += count; }
		else			{ ey += (count*2); }
	}
	else
	{
		if(dx > 0)		{ ex -= (count*2); }
		else			{ ex += (count*2); }
	}

	if(IsMovable(ex, ey) == FALSE)							// ã�� ���� ������ ���� ���̶�� 8�������� Ž��
	{
		for(int i = 0; i < (sizeof(surround_x) / sizeof(surround_x[0])); i++)		// �ֺ� 8����
		{
			if(IsMovable(ex + surround_x[i], ey + surround_y[i])) 
			{
				x = ex; y = ey;
				return TRUE;
			}
		}
	}
	else
	{
		x = ex; y = ey;
		return TRUE;
	}

	return FALSE;									
}

/////////////////////////////////////////////////////////////////////////////
//	��������Ʈ�� �ʱ�ȭ�Ѵ�.
//
void CNpc::InitUserList()
{
	m_TotalDamage = 0;
	for(int i = 0; i < NPC_HAVE_USER_LIST; i++)
	{
		m_DamagedUserList[i].bIs = FALSE;
		m_DamagedUserList[i].iUid = -1;
		m_DamagedUserList[i].nDamage = 0;
		::ZeroMemory(m_DamagedUserList[i].strUserID, sizeof(m_DamagedUserList[i].strUserID));
	}
/*	int i;
									// ����ġ �й踦 ���� �����ϴ� ����Ʈ
	for(i = 0; i < m_arDamagedUserList.GetSize(); i++)
	{
		if(m_arDamagedUserList[i])
		{
			delete m_arDamagedUserList[i];
			m_arDamagedUserList[i] = NULL;
		}
	}
	m_arDamagedUserList.RemoveAll();
*/
}

/////////////////////////////////////////////////////////////////////////////
//	�ش� ���� �Ӽ��� ������ �迭 �� ���������� �´��� üũ�Ѵ�.
//
BOOL CNpc::CheckClassItem(int artable, int armagic)
{
	if(artable < 0 || artable >= g_arItemTable.GetSize()) return FALSE;
	if(armagic < 0 || armagic >= g_arMagicItemTable.GetSize()) return FALSE;
	if(armagic==148||armagic==149||armagic==150||armagic==151||armagic==152||armagic==153||armagic==156|| armagic==158 || armagic==162||armagic==164 || armagic==166){
		return FALSE;
	}
	int iWear;

	BYTE armWear = g_arItemTable[artable]->m_byWear;			// ������ �迭 1: ���� 2~8 : ���������
	BYTE tNeedClass = g_arItemTable[artable]->m_byClass;
	BYTE armMagic = g_arMagicItemTable[armagic]->m_tNeedClass;	// �����Ӽ� �迭

	if(armMagic != 31/*15*/)
	{
		BYTE tTemp = 1;	
		BYTE tFire = 0;
		BYTE tEdge = 0;
		BYTE tStaff = 0;
		BYTE tBrawl = 0;
		BYTE tJudge = 0;

		tFire	 = tTemp & tNeedClass; tTemp = 2; 
		tEdge	 = tTemp & tNeedClass; tTemp = 4;
		tStaff	 = tTemp & tNeedClass; tTemp = 8;
		tJudge   = tTemp & tNeedClass; tTemp = 16;
		tBrawl	 = tTemp & tNeedClass;

		tFire = tFire & armMagic;
		tEdge = tEdge & armMagic;
		tStaff = tStaff & armMagic;
		tBrawl = tBrawl & armMagic;
		tJudge = tJudge & armMagic;

		tTemp = tFire^tEdge^tStaff^tBrawl^tJudge;
		if(!tTemp) return FALSE;
//		if(tNeedClass != armMagic) return FALSE;
	}

	iWear = g_arMagicItemTable[armagic]->m_tWearInfo;		// ���� ������ �߸��� �Ӽ��� �ٴ°��� ����

	if(iWear == 0) return TRUE;
	else if(iWear == 1)											
	{														// 1���̸� ������� �ٴ´�.
		if(armWear != 1) return FALSE;
		else return TRUE;
	}
	else if(iWear == 2)										// 2���̸� ���⸦ ������ ��������ۿ� �ٴ´�.
	{
		if(armWear <= 1 || armWear >= 9) return FALSE;
		else return TRUE;
	}
	else return FALSE;
}

void CNpc::DeleteNPC()
{
	// ���� ���͸� ������ �ʴ´�. ����, ������ �󿡼� ���� ���ϵ��� ���⸸ �Ѵ�.
	m_bFirstLive = FALSE;
	m_tNpcType = 2;

	// ���߿� �������.
}

//////////////////////////////////////////////////////////////////////////////////
//	���� �������� ���Ѵ�.
//
int CNpc::GetFinalDamage(USER *pUser, int type)
{
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return 0;

	int iInitDamage = GetNormalInitDamage();
	int iFinalDamage = 0, iFinalTemp = 0;
	
//	int iBasic = (int)((double)(pUser->m_sCON + pUser->m_DynamicUserData[MAGIC_CON_UP])/3 + 0.5);					// �⺻����
	int iBasic = (int)((double)(pUser->m_sMagicCON + pUser->m_DynamicUserData[MAGIC_CON_UP] + (int)((double)pUser->m_DynamicEBodyData[EBODY_CON_TO_DEFENSE] / 100 * (double)pUser->m_sMagicCON) )/3 + 0.5);				// �⺻����
	if(iBasic < 0) iBasic = 0;

	BYTE tWeaponClass = 255;
	BOOL bCanUseSkill = pUser->IsCanUseWeaponSkill(tWeaponClass);

	int		iDefense = 1;
	double	dIron = 0;
	double	dShield = 0;
	double	dGuard = 0;
	int		iCAttack = 0;
	double	dAdamantine = 0;
	double	dDefenseUP = 0;
	double	dABDefense = 0;

	int		iIronLevel = 0;
	int		iGuardLevel = 0;
	int		iVitalLevel = 0;
	int		iCounterAttackLevel = 0;
	int		iDefenseUPLevel = 0;
	int		iABDefenseLevel = 0;

	int		iIS = 0;
	int		iCA = 0;

	int i = 0;
	int iRandom = 0;
	int iSkillSid = 0;
	int tClass = tWeaponClass * SKILL_NUM;

	iDefense = pUser->GetDefense();							// ��

	//iDefense = (int)((double)iDefense * 0.8); //����ֻ��ƽʱ��80


	if(tWeaponClass != 255)
	{
		for(i = tClass; i < tClass + SKILL_NUM; i++)	// IronSkill
		{
			iSkillSid = pUser->m_UserSkill[i].sSid;

			if(iSkillSid == SKILL_IRON)					// 1 index
			{
				iIronLevel = pUser->m_UserSkill[i].tLevel;
				if(iIronLevel < 0) iIronLevel = 0;
				
				// �����ۿ� ���� ��ų ���� ����
				if(iIronLevel >= 1) iIronLevel += pUser->m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iIronLevel >= SKILL_LEVEL) iIronLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;

				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iIronLevel)) iIS = 1;
				
				// ���̾�Ų�� ���� �⺻ ������ ����
				iBasic = (int)((double)iBasic * (1 + (double)(iIS * g_arSkillTable[iSkillSid]->m_arInc.GetAt(iIronLevel) / 100)) );
			}

			if(iSkillSid == SKILL_CRITICAL_GUARD)					// Critical Guard 11 index
			{
				iGuardLevel = pUser->m_UserSkill[i].tLevel;		
				if(iGuardLevel < 0) iGuardLevel = 0;
				
				// �����ۿ� ���� ��ų ���� ����
				if(iGuardLevel >= 1) iGuardLevel += pUser->m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iGuardLevel >= SKILL_LEVEL) iGuardLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;
				
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iGuardLevel))
				{				
					//dGuard = (double)(iInitDamage *g_arSkillTable[iSkillSid]->m_arInc.GetAt(iGuardLevel))/100.0;
				}
			}

			if(iSkillSid == SKILL_BACK_ATTACK )					// �ݰ� 2 index
			{
				iCounterAttackLevel = pUser->m_UserSkill[i].tLevel;		
				if(iCounterAttackLevel < 0) iCounterAttackLevel = 0;
				
				// �����ۿ� ���� ��ų ���� ����
				if(iCounterAttackLevel >= 1) iCounterAttackLevel += pUser->m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iCounterAttackLevel >= SKILL_LEVEL) iCounterAttackLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;

				if(pUser->GetDistance(m_sCurX, m_sCurY, 1) == FALSE && pUser->m_dwFANTAnTime == 0 ) iCA = 0;
				else
				{ 
					iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
					if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iCounterAttackLevel)) iCA = 1;
				}
				
				  if(m_sEvent < 30000 ) //boss������  �����ػ�
				  {
					  iCAttack = (int)(iInitDamage * iCA * (double)((g_arSkillTable[iSkillSid]->m_arInc.GetAt(iCounterAttackLevel)) / 100.0) ); 
				  }
			}
			
			if(iSkillSid == SKILL_ABSOLUTE_DEFENSE)					// Absolute Defense 
			{
				iABDefenseLevel = pUser->m_UserSkill[i].tLevel;		
				if(iABDefenseLevel < 0) iABDefenseLevel = 0;
				
				// �����ۿ� ���� ��ų ���� ����
				if(iABDefenseLevel >= 1) iABDefenseLevel += pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iABDefenseLevel >= SKILL_LEVEL) iABDefenseLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;
				
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iABDefenseLevel))
				{
					dABDefense = (double)(iDefense * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iABDefenseLevel ) /100.0);
				}
			}
			if(iSkillSid == SKILL_DEFENSE_UP)					// Defense up
			{
				iDefenseUPLevel = pUser->m_UserSkill[i].tLevel;		
				if(iDefenseUPLevel < 0) iDefenseUPLevel = 0;
				
				// �����ۿ� ���� ��ų ���� ����
				if(iDefenseUPLevel >= 1) iDefenseUPLevel += pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iDefenseUPLevel >= SKILL_LEVEL) iDefenseUPLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;
				
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iDefenseUPLevel))
				{
					dDefenseUP = (double)(iDefense * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iDefenseUPLevel)/100.0);
				}
			}
		}
	}

//	if(pUser->m_dwShieldTime != 0)	dShield = (double)(iInitDamage * 0.2);
	if(pUser->m_bNecklaceOfShield && pUser->m_dwShieldTime != 0)		dShield = (double)(iInitDamage * 0.3);
	else if(pUser->m_bNecklaceOfShield && pUser->m_dwBigShieldTime != 0)	dShield = (double)(iInitDamage * 0.35);
	else if(pUser->m_bNecklaceOfShield || pUser->m_dwShieldTime != 0 )	dShield = (double)(iInitDamage * 0.2);
    else if(pUser->m_dwBigShieldTime !=0 && !pUser->m_bNecklaceOfShield ) dShield = (double)(iInitDamage * 0.25);
	if(pUser->m_bNecklaceOfShield) pUser->SendAccessoriDuration(SID_NECKLACE_OF_SHIELD);
	

	/*if( pUser->m_dwAdamantineTime != 0 ) //���ڼ��� ��ջ���   ��ħ��ֱ����ʾ����
	{
		dAdamantine = (double)( (double)iDefense * 0.1 );
	}*/

	iDefense = (int)( iDefense + dABDefense + dDefenseUP + dAdamantine );

	iFinalDamage = (int)(iInitDamage - (iDefense + iBasic + dShield + dGuard)); 

	if(iFinalDamage < 0) iFinalDamage = 0;
	if(iFinalDamage <= 15)
	{
		iFinalTemp = iFinalDamage;
		iFinalDamage += (int)((double)iInitDamage * 0.2 + 1.5);	// �ּҴ������ �ִ� 15���� �Ѵ�.
		if(iFinalDamage > 15) iFinalDamage = 15;
		iFinalDamage = max(iFinalDamage, iFinalTemp);
	}
	////�����ػ�
	// CString str;
	// str.Format( "���ﷴ�������˺���%d", iFinalDamage);//�����ػ�
 //    pUser->SendEventMsg(str.GetBuffer(0));


	if(pUser->m_tAbnormalKind == ABNORMAL_BYTE_COLD) iFinalDamage += 10;


	if(iCAttack > 0 && type) 
	{
		iCA = iCAttack;		// ���� �ݰ� ������

		if(iCA > 0)			// �ݰ��� 0���� Ŭ�� ���� ����Ʈ�� ���ϰ� �й�...
		{
//			pUser->SetCounterAttack(m_sNid, iCA);
			
			// alisia
			int iDamage = iCA;

			if( pUser->GetDistance(m_sCurX, m_sCurY, 2)  || pUser->m_dwFANTAnTime != 0 ) 

			{
				if( SetDamage(iDamage, (pUser->m_uid) + USER_BAND, pUser->m_pCom) == FALSE )
				{
					SendExpToUserList( pUser->m_pCom ); // ����ġ �й�!!
					SendDead( pUser->m_pCom );

					if( m_NpcVirtualState == NPC_STANDING )
					{
						CheckMaxValue( pUser->m_dwXP, 1);	// ���� �������� 1 ����!	
						pUser->SendXP();
					}
				}
			     pUser->SendDamageNum(1,m_sNid + NPC_BAND,(short)iDamage);//ȭ����������
			     pUser->SendNpcHP(m_sNid + NPC_BAND,m_sHP);	
			}

		}
	}

	return iFinalDamage;
}
//
int CNpc::SetHuFaFinalDamage(USER *pUser, int iDamage)
{
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) 
		return 0;

	if( SetDamage(iDamage, (pUser->m_uid) + USER_BAND, pUser->m_pCom) == FALSE )
	{
		SendExpToUserList( pUser->m_pCom ); 
		SendDead( pUser->m_pCom );

		if( m_NpcVirtualState == NPC_STANDING )
		{
			CheckMaxValue( pUser->m_dwXP, 1);	// ���� �������� 1 ����!	
			pUser->SendXP();
		}
	}


	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////
//	NPC ������ ���ۿ� �����Ѵ�.
//  NPCģ��
void CNpc::FillNpcInfo(char *temp_send, int &index, BYTE flag)
{
	CPoint t;

	SetByte(temp_send, NPC_INFO, index );
	SetByte(temp_send, flag, index );
	SetShort(temp_send, m_sNid+NPC_BAND, index );

	if(flag != INFO_MODIFY)	return;

	SetShort(temp_send, m_sPid, index);
	SetVarString(temp_send, m_strName, _tcslen(m_strName), index);

	t = ConvertToClient(m_sCurX, m_sCurY);
	
	SetShort(temp_send, t.x, index);
	SetShort(temp_send, t.y, index);

	if(m_sHP <= 0) SetByte(temp_send, 0x00, index);
	else SetByte(temp_send, 0x01, index);

	SetByte(temp_send, m_tNpcType, index);
	SetInt(temp_send, m_sMaxHP, index);
	SetInt(temp_send, m_sHP, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetShort(temp_send, m_sClientSpeed, index);

	SetByte(temp_send, m_byColor, index);

	if(m_tNpcType == NPCTYPE_GUILD_DOOR) 
	{
		SetShort(temp_send, m_sDimension, index);
	}
	SetShort(temp_send, m_sQuestSay, index);
}

///////////////////////////////////////////////////////////////////////////////////////
//	������ǥ�� Ŭ���̾�Ʈ ��ǥ�� �ٲ۴�.
//
CPoint CNpc::ConvertToClient(int x, int y)
{
	if(!g_zone[m_ZoneIndex]) return CPoint(-1,-1);

	int tempx, tempy;
	int temph = g_zone[m_ZoneIndex]->m_vMoveCell.m_vDim.cy / 2 - 1;

	if( y >= g_zone[m_ZoneIndex]->m_sizeMap.cy || x >= g_zone[m_ZoneIndex]->m_sizeMap.cx ) return CPoint(-1,-1);

	tempx = x - temph + y;
	tempy = y - x + temph;

	return CPoint( tempx, tempy );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���� ������ �������� �����͸� ������.
//
void CNpc::SendToRange(COM *pCom, char *temp_send, int index, int min_x, int min_y, int max_x, int max_y)
{
/*
	if( index <= 0 || index >= SEND_BUF_SIZE ) return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;

	if( !pNewData ) return;

	pNewData->flag = SEND_RANGE;
	pNewData->len = index;

	::CopyMemory( pNewData->pBuf, temp_send, index );

	pNewData->uid = 0;
	pNewData->z = m_sCurZ;
	pNewData->rect.left		= min_x;
	pNewData->rect.right	= max_x;
	pNewData->rect.top		= min_y;
	pNewData->rect.bottom	= max_y;
	pNewData->zone_index = m_ZoneIndex;

	EnterCriticalSection( &(pCom->m_critSendData) );

	pCom->m_arSendData.Add( pNewData );

	LeaveCriticalSection( &(pCom->m_critSendData) );

	PostQueuedCompletionStatus( pCom->m_hSendIOCP, 0, 0, NULL );
*/

	if( index <= 0 || index >= SEND_BUF_SIZE ) return;

	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return;
	
	int tmin_x = min_x;		if(tmin_x < 0 ) tmin_x = 0;
	int tmax_x = max_x;		if(tmax_x >= pMap->m_sizeMap.cx ) tmax_x = pMap->m_sizeMap.cx - 1;
	int tmin_y = min_y;		if(tmin_y < 0 ) tmin_y = 0;
	int tmax_y = max_y;		if(tmax_y >= pMap->m_sizeMap.cy ) tmax_y = pMap->m_sizeMap.cy - 1;

	int temp_uid;
	USER* pUser = NULL;

	for( int i = tmin_x; i < tmax_x; i++ )
	{
		for( int j = tmin_y; j < tmax_y; j++ )
		{
			temp_uid = pMap->m_pMap[i][j].m_lUser;

			if(temp_uid < USER_BAND || temp_uid >= NPC_BAND) continue;
			else temp_uid -= USER_BAND;

			if( temp_uid >= 0 && temp_uid < MAX_USER )
			{
				pUser = pCom->GetUserUid(temp_uid);
				if ( pUser == NULL ) continue;
				
				if( pUser->m_state == STATE_GAMESTARTED )
				{
					if( pUser->m_curx == i && pUser->m_cury == j && pUser->m_curz == m_sCurZ )
					{
						Send( pUser, temp_send, index);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//	���ݴ��(Target)�� �ʱ�ȭ �Ѵ�.
//
inline void CNpc::InitTarget()
{
	m_Target.id = -1;
	m_Target.x = 0;
	m_Target.y = 0;
	m_Target.failCount = 0;
//	m_Target.nLen = 0; 
//	::ZeroMemory(m_Target.szName, sizeof(m_Target.szName));
}

/////////////////////////////////////////////////////////////////////////////////////////
//	PathFind �� �����Ѵ�.
//
BOOL CNpc::PathFind(CPoint start, CPoint end)
{
	m_bRandMove = FALSE;

	if(start.x < 0 || start.y < 0 || end.x < 0 || end.y < 0)
	{
		return FALSE;
	}

	int i, j;

	int min_x, max_x;
	int min_y, max_y;

	min_x = m_min_x;
	min_y = m_min_y;
	max_x = m_max_x;
	max_y = m_max_y;

	if(InterlockedCompareExchange((LONG*)&m_lMapUsed, (long)1, (long)0) == (long)0)
	{
		ClearPathFindData();

		m_vMapSize.cx = max_x - min_x + 1;		
		m_vMapSize.cy = max_y - min_y + 1;

	
/*		m_pMap = new int*[m_vMapSize.cx];

		for(i = 0; i < m_vMapSize.cx; i++)
		{
			m_pMap[i] = new int[m_vMapSize.cy];
		}
*/
		for(i = 0; i < m_vMapSize.cy; i++)
		{
			for(j = 0; j < m_vMapSize.cx; j++)
			{
				if( min_x+j == m_sCurX && min_y+i == m_sCurY )
				{
					m_pMap[j*m_vMapSize.cy + i] = 0;
//					m_pMap[j][i] = 0;
				}
				else
				{
					if(m_pOrgMap[min_x + j][min_y + i].m_bMove || m_pOrgMap[min_x + j][min_y + i].m_lUser != 0 )
					{
//						m_pMap[j][i] = 1;
						m_pMap[j*m_vMapSize.cy + i] = 1;
					}
					else
					{
//						m_pMap[j][i] = 0;
						m_pMap[j*m_vMapSize.cy + i] = 0;
					}
				}
			}
		}

		m_vStartPoint  = start;		m_vEndPoint = end;
		m_pPath = NULL;
		m_vPathFind.SetMap(m_vMapSize.cx, m_vMapSize.cy, m_pMap);

		m_pPath = m_vPathFind.FindPath(end.x, end.y, start.x, start.y);

		::InterlockedExchange(&m_lMapUsed, 0);

		if(m_pPath)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else return FALSE; 

}

/////////////////////////////////////////////////////////////////////////////
//	�н� ���ε忡�� ã�� ��θ� �� �̵� �ߴ��� üũ
//
BOOL CNpc::IsStepEnd()
{
	if( !m_pPath )	return FALSE;

	if( m_NpcState != NPC_MOVING && m_NpcState != NPC_TRACING ) return FALSE;

	if( !m_pPath->Parent )	return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	NPC �⺻���� �ʱ�ȭ
//
void CNpc::Init()
{		
	int i, j;
	m_dLastFind = GetTickCount();
	m_Delay = 0;
	m_dwLastThreadTime = GetTickCount();

	if((m_sOrgX + m_sOrgY) % 2 != 0) m_sOrgX++;
	CPoint pt = ConvertToServer(m_sOrgX, m_sOrgY);

	if(pt.x == -1 || pt.y == -1)
	{
		CString szTemp;
		szTemp.Format(_T("Invalid NPC AXIS : Name = %s, x = %d, y = %d"), m_strName, m_sOrgX, m_sOrgY);
		AfxMessageBox(szTemp);
		InterlockedIncrement(&g_CurrentNPCError);
	}
	else
	{
		m_sTableOrgX = m_sOrgX = pt.x;
		m_sTableOrgY = m_sOrgY = pt.y;

		m_NpcVirtualState = NPC_STANDING;
		
		if(m_sGuild >= NPC_GUILDHOUSE_BAND)			// ���ø� �������� 0�� ����, 1, 2 �̷������� ���� m_sGuild = 10000(ó��)
		{											// 0������ = �糪�� 1������ = ��Ʈ��... �̰� 10000������
			int index = 0;
			index = GetCityNumForVirtualRoom(m_sCurZ);
			
			if(index >= 0) g_arGuildHouseWar[index]->m_CurrentGuild.arNpcList.Add(m_sNid);

			m_NpcVirtualState = NPC_WAIT;
		}
		else if(m_tNpcType == NPCTYPE_MONSTER && m_sGuild >= FORTRESS_BAND && m_sGuild < NPC_GUILDHOUSE_BAND)	// �̰� 1000������
		{
			for(i = 0; i < g_arGuildFortress.GetSize(); i++)
			{
				if(!g_arGuildFortress[i]) continue;

				if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
				{
					g_arGuildFortress[i]->m_arViolenceNpcList.Add(m_sNid);

					m_NpcVirtualState = NPC_WAIT;

//					m_pGuardFortress = g_arGuildFortress[i];
					break;
				}
			}			
		}

		switch(m_tNpcType)
		{
		case NPCTYPE_GUILD_GUARD: case NPCTYPE_GUILD_NPC:
			{
				if(m_sGuild < FORTRESS_BAND)				// ������ ���� ����̸�
				{
					CStore *pStore = NULL;
					for(i = 0; i < g_arStore.GetSize(); i++)
					{
						if(g_arStore[i] == NULL) continue;

						pStore = g_arStore[i];					// ���� �����͸� ���´�.(��������� ��������)
						if(pStore->m_sStoreID == (short)m_sGuild) 
						{	
							pStore->m_arNpcList.Add(m_sNid);	// �� ���̵� ������ ����Ѵ�.
							m_pGuardStore = pStore;	
							break; 
						}
					}
				}
				else											// ����� ���� NPC�̸�...
				{
					for(i = 0; i < g_arGuildFortress.GetSize(); i++)
					{
						if(!g_arGuildFortress[i]) continue;

						if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
						{
							if(m_sEZone >= GUILD_FORTRESS_NPC_BAND)
							{
								for(j = 0; j < FORTRESS_TARGET_MAX_NUM; j++)
								{
									if(InterlockedCompareExchange((long*)&g_arGuildFortress[i]->m_arFortressTarget[j].lUsed, (long)1, (long)0) == (long)0)
									{
										g_arGuildFortress[i]->m_arFortressTarget[j].sTargertID = m_sNid;
										break;
									}
								}
							}
							else if(m_sEZone < GUILD_FORTRESS_NPC_BAND && m_sEZone >= GUILD_REPAIR_NPC_BAND)	// �����ɼ� �ִ� ��, ������ 
							{
								g_arGuildFortress[i]->m_arRepairNpcList.Add(m_sNid);
							}

							g_arGuildFortress[i]->m_arNpcList.Add(m_sNid);

							m_pGuardFortress = g_arGuildFortress[i];
							break;
						}
					}
				}
			}
			break;

		case NPCTYPE_GUILD_MARK:
			{
				if(m_sGuild >= 0 && m_sGuild < g_arGuildData.GetSize())
				{
					m_sPid = g_arGuildHouse[m_sGuild]->iGuild;		
					g_arGuildHouse[m_sGuild]->iMarkNpc = m_sNid;
					::ZeroMemory(m_strName, sizeof(m_strName));
				
					if( m_sPid >= 0 && m_sPid < g_arGuildData.GetSize())
					{										// ��忡 ����ȭ�� ���ʿ䰡 ����.
						if(g_arGuildData[m_sPid])
						{
							int nLen = 0;
							
							m_sMaxHP = g_arGuildData[m_sPid]->m_sVersion;
							nLen = strlen(g_arGuildData[m_sPid]->m_strGuildName);
							if(nLen > 0)
							{							
								strncpy(m_strName, g_arGuildData[m_sPid]->m_strGuildName, nLen);
							}
						}
					}				
				}
			}
			break;

		case NPCTYPE_GUILD_DOOR:
			{
				for(i = 0; i < g_arGuildFortress.GetSize(); i++)
				{
					if(!g_arGuildFortress[i]) continue;

					if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
					{
						if(m_sEZone < GUILD_FORTRESS_NPC_BAND && m_sEZone >= GUILD_REPAIR_NPC_BAND)	// �����ɼ� �ִ� ��, ������ 
						{
							g_arGuildFortress[i]->m_arRepairNpcList.Add(m_sNid);
						}

						g_arGuildFortress[i]->m_arNpcList.Add(m_sNid);

						m_pGuardFortress = g_arGuildFortress[i];
						break;
					}
				}
			}
/*
		case NPCTYPE_FORTRESS:
			{
				for(i = 0; i < g_arGuildFortress.GetSize(); i++)
				{
					if(!g_arGuildFortress[i]) continue;

					if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
					{
						for(j = 0; j < FORTRESS_TARGET_MAX_NUM; j++)
						{
							if(InterlockedCompareExchange(&g_arGuildFortress[i]->m_arFortressTarget[j].lUsed, (LONG)1, (LONG)0) == 0)
							{
								g_arGuildFortress[i]->m_arFortressTarget[j].bChange = FALSE;
								g_arGuildFortress[i]->m_arFortressTarget[j].sTargertID = m_sNid;
								m_pGuardFortress = g_arGuildFortress[i];
								break;
							}
						}
					}
				}
			}
			break;
/*
		case NPCTYPE_REPAIR_GUARD:
			{
				for(i = 0; i < g_arGuildFortress.GetSize(); i++)
				{
					if(!g_arGuildFortress[i]) continue;

					if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
					{
						g_arGuildFortress[i]->m_arRepairNpcList.Add(m_sNid);
						m_pGuardFortress = g_arGuildFortress[i];
						break;
					}
				}
			}
			break;
*/		}
/*
	else
	{
		m_sOrgX = pt.x;
		m_sOrgY = pt.y;

		m_NpcVirtualState = NPC_STANDING;

		if(m_sGuild >= NPC_GUILDHOUSE_BAND)			// ���ø� �������� 0�� ����, 1, 2 �̷������� ���� m_sGuild = 10000(ó��)
		{											// 0������ = �糪�� 1������ = ��Ʈ��...
			int index = 0;
			index = GetCityNumForVirtualRoom(m_sCurZ);
			
			g_arGuildHouseWar[index]->m_CurrentGuild.arNpcList.Add(m_sNid);

			m_NpcVirtualState = NPC_WAIT;
		}		
		else if(m_tNpcType == NPCTYPE_GUILD_GUARD || m_tNpcType == NPCTYPE_GUILD_NPC)
		{
			if(m_sGuild < FORTRESS_BAND)				// ������ ���� ����̸�
			{
				CStore *pStore = NULL;
				for(i = 0; i < g_arStore.GetSize(); i++)
				{
					if(g_arStore[i] == NULL) continue;

					pStore = g_arStore[i];					// ���� �����͸� ���´�.(��������� ��������)
					if(pStore->m_sStoreID == (short)m_sGuild) 
					{	
						pStore->m_arNpcList.Add(m_sNid);	// �� ���̵� ������ ����Ѵ�.
						m_pGuardStore = pStore;	
						break; 
					}
				}
			}
			else											// ����� ���� NPC�̸�...
			{
				for(i = 0; i < g_arGuildFortress.GetSize(); i++)
				{
					if(!g_arGuildFortress[i]) continue;

					if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
					{
						g_arGuildFortress[i]->m_arNpcList.Add(m_sNid);
						m_pGuardFortress = g_arGuildFortress[i];
						break;
					}
				}
			}
		}
		else if(m_tNpcType == NPCTYPE_GUILD_MARK)
		{
			if(m_sGuild >= 0 && m_sGuild < g_arGuildData.GetSize())
			{
				m_sPid = g_arGuildHouse[m_sGuild]->iGuild;		
				g_arGuildHouse[m_sGuild]->iMarkNpc = m_sNid;
				::ZeroMemory(m_strName, sizeof(m_strName));
			
				if( m_sPid >= 0 && m_sPid < g_arGuildData.GetSize())
				{										// ��忡 ����ȭ�� ���ʿ䰡 ����.
					if(g_arGuildData[m_sPid])
					{
						int nLen = 0;
						
						m_sMaxHP = g_arGuildData[m_sPid]->m_sVersion;
						nLen = strlen(g_arGuildData[m_sPid]->m_strGuildName);
						if(nLen > 0)
						{							
							strncpy(m_strName, g_arGuildData[m_sPid]->m_strGuildName, nLen);
						}
					}
				}				
			}
		}
		else if(m_tNpcType == NPCTYPE_FORTRESS)
		{
			for(i = 0; i < g_arGuildFortress.GetSize(); i++)
			{
				if(!g_arGuildFortress[i]) continue;

				if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
				{
					for(j = 0; j < FORTRESS_TARGET_MAX_NUM; j++)
					{
						if(InterlockedCompareExchange(&g_arGuildFortress[i]->m_arFortressTarget[j].lUsed, (LONG)1, (LONG)0) == 0)
						{
							g_arGuildFortress[i]->m_arFortressTarget[j].bChange = FALSE;
							g_arGuildFortress[i]->m_arFortressTarget[j].sTargertID = m_sNid;
							m_pGuardFortress = g_arGuildFortress[i];
							break;
						}
					}
				}
			}
		}
		else if(m_tNpcType == NPCTYPE_REPAIR_GUARD)
		{
			for(i = 0; i < g_arGuildFortress.GetSize(); i++)
			{
				if(!g_arGuildFortress[i]) continue;

				if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
				{
					g_arGuildFortress[i]->m_arRepairNpcList.Add(m_sNid);
					m_pGuardFortress = g_arGuildFortress[i];
					break;
				}
			}
		}
*/	}

	m_pOrgMap = g_zone[m_ZoneIndex]->m_pMap;	// MapInfo ���� ����
}

int CNpc::GetCityNumForVirtualRoom(int zone)		// ������ ���ù�ȣ���� ���߿� VirtualRoom�� ��� �߰��Ǹ�..
{													// �ٲپ�� �ȴ�. (int zone, int &curGuild)
	int nRet = -1;

	switch(zone)									// �߰��� ������...
	{
	case 1005:										// 1004�� �̸�..
		nRet = SANAD;								// ���ô� �糪��, m_CurrentGuild = 0��°
		break;

	default:
		break;
	}

	return nRet;
}

////////////////////////////////////////////////////////////////////////////////////
//	Client ��ǥ�� ������ǥ�� ��ȯ�Ѵ�
//
CPoint CNpc::ConvertToServer(int x, int y)
{
	if(!g_zone[m_ZoneIndex]) return CPoint(-1,-1); 

	int tempx, tempy;
	int temph = g_zone[m_ZoneIndex]->m_vMoveCell.m_vDim.cy / 2 - 1;

	if( y >= g_zone[m_ZoneIndex]->m_vMoveCell.m_vDim.cy || x >= g_zone[m_ZoneIndex]->m_vMoveCell.m_vDim.cx ) return CPoint(-1,-1);

	if( (x+y)%2 == 0 )
	{
		tempx = temph - ( y / 2 ) + ( x / 2 );

		if( x % 2 ) tempy = ( y / 2 ) + ( ( x / 2 ) + 1 );
		else        tempy = ( y / 2 ) + ( x / 2 );

		return CPoint( tempx, tempy );
	}
	else return CPoint(-1,-1);
}

////////////////////////////////////////////////////////////////////////////////////
//	������� ���� NPC ������ TRACE �Ѵ�.
//
void CNpc::NpcTrace(TCHAR *pMsg)
{
	if(g_bDebug == FALSE) return;

	CString szMsg = _T("");
	CPoint pt = ConvertToClient(m_sCurX, m_sCurY);
	szMsg.Format(_T("%s : uid = %d, name = %s, xpos = %d, ypos = %d\n"), pMsg, m_sNid, m_strName, pt.x, pt.y);
	TRACE(szMsg);
}

///////////////////////////////////////////////////////////////////////////////////
//	������ �������� �н����ε��� �����ϸ� �������� ����������� �����δ�.
//
void CNpc::ToTargetMove(COM *pCom, USER *pUser)
{
	if(!pCom) return;
	if(!pUser) return;
	if(!g_zone[m_ZoneIndex]) return;

	int xx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int yy[] = {0, -1, -1, -1, 0, 1, 1, 1};

	CPoint ptUser = ConvertToClient(pUser->m_curx, pUser->m_cury);

	struct _min
	{
		int x, y;
		int value;
	}min;

	int minindex;
	int i, j;

	int dx, dy;
	CPoint ptNew;
	int max_dist;
/*
	int test1[8], test2[8];

	for(i = 0; i < 8; i++)
	{
		ptNew = ConvertToClient(m_sCurX + xx[i], m_sCurY + yy[i]);
		dx = abs(ptUser.x - ptNew.x);
		dy = abs(ptUser.y - ptNew.y);
		test1[i] = dx + dy;
	}
*/	
	for(i = 0; i < sizeof(xx)/sizeof(xx[0]) - 1; i++)
	{
		minindex = i;
		
		ptNew = ConvertToClient(m_sCurX + xx[i], m_sCurY + yy[i]);

		if(ptNew.x <= -1 || ptNew.y <= -1) continue;
		if(ptNew.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptNew.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) continue;

		dx = abs(ptUser.x - ptNew.x);
		dy = abs(ptUser.y - ptNew.y);
		max_dist = dx + dy;

		min.value = max_dist;
		min.x = xx[i];
		min.y = yy[i];

		for(j = i + 1; j < sizeof(xx)/sizeof(xx[0]); j++)
		{
			ptNew = ConvertToClient(m_sCurX + xx[j], m_sCurY + yy[j]);

			if(ptNew.x <= -1 || ptNew.y <= -1) continue;
			if(ptNew.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptNew.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) continue;

			dx = abs(ptUser.x - ptNew.x);
			dy = abs(ptUser.y - ptNew.y);
			max_dist = dx + dy;

			if(min.value > max_dist)
			{
				min.value = max_dist;
				min.x = xx[j];
				min.y = yy[j];
				minindex = j;
			}
		}

		xx[minindex] = xx[i];
		yy[minindex] = yy[i];

		xx[i] = min.x;
		yy[i] = min.y;
	}
/*
	for(i = 0; i < 8; i++)
	{
		ptNew = ConvertToClient(m_sCurX + xx[i], m_sCurY + yy[i]);
		dx = abs(ptUser.x - ptNew.x);
		dy = abs(ptUser.y - ptNew.y);
		test2[i] = dx + dy;
	}
*/
	MAP* pMap = g_zone[m_ZoneIndex];
	CPoint ptPre(m_sCurX, m_sCurY);
	int will_x, will_y;
	BOOL bMove = FALSE;
	int new_dist = 0, cur_dist = 0;

	CPoint ptCurr = ConvertToClient(m_sCurX, m_sCurY);
	cur_dist = abs(ptUser.x - ptCurr.x) + abs(ptUser.y - ptCurr.y);

	for(i = 0; i < sizeof(xx)/sizeof(xx[0]); i++)
	{
		will_x = m_sCurX + xx[i];
		will_y = m_sCurY + yy[i];

		ptNew = ConvertToClient(m_sCurX + xx[i], m_sCurY + yy[i]);
		new_dist = abs(ptUser.x - ptNew.x) + abs(ptUser.y - ptNew.y);

		if(new_dist > cur_dist) continue;

		if(will_x <= -1 || will_y <= -1) continue;
		if(will_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || will_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) continue;

		if(pMap->m_pMap[will_x][will_y].m_bMove != 0 || pMap->m_pMap[will_x][will_y].m_lUser != 0)
		{
			continue;
		}
		else
		{
			if(InterlockedCompareExchange((LONG*)&m_pOrgMap[will_x][will_y].m_lUser,
				(long)m_pOrgMap[m_sCurX][m_sCurY].m_lUser, (long)0) == (long)0)
			{
				::InterlockedExchange(&m_pOrgMap[m_sCurX][m_sCurY].m_lUser, 0);
				m_sCurX = will_x;
				m_sCurY = will_y;
				SightRecalc(pCom);
				bMove = TRUE;
				break;
			}
			else continue;
		}
	}

	if(!bMove) return;

	CBufferEx TempBuf;
	CPoint t = ConvertToClient(m_sCurX, m_sCurY);		// �����̷��� ������ǥ�� Ŭ���̾�Ʈ���� �������̴� ��ǥ�� ����
	if(t.x <= -1 || t.y <= -1) return;

	TempBuf.Add(MOVE_RESULT);

	TempBuf.Add(SUCCESS);
	TempBuf.Add((int)(NPC_BAND + m_sNid));
	TempBuf.Add((short)t.x);
	TempBuf.Add((short)t.y);

	SendInsight(pCom, TempBuf, TempBuf.GetLength());

	m_Delay = m_sSpeed;
}



void CNpc::EventNpcInit(int x, int y)
{
	m_dwLastThreadTime = GetTickCount();

	m_sOrgX = x;
	m_sOrgY = y;

	m_pOrgMap = g_zone[m_ZoneIndex]->m_pMap;	// MapInfo ���� ����

	m_Delay = 0;
}

///////////////////////////////////////////////////////////////////////////////////
//	������� ���� Ÿ���� �ٲ۴�. 
//
void CNpc::SetGuildType(COM *pCom)
{
/*	int modify_index = 0;
	char modify_send[2048];	

	::ZeroMemory(modify_send, sizeof(modify_send));

	if(m_tGuildWar == GUILD_WARRING)
	{
		m_tNpcAttType = 1;
		if(m_tNpcType == NPCTYPE_GUARD) m_tNpcType = NPCTYPE_GUILD_GUARD;
		else if(m_tNpcType == NPCTYPE_NPC) m_tNpcType = NPCTYPE_GUILD_NPC;

		// �������� NPC ��������...
		FillNpcInfo(modify_send, modify_index, INFO_MODIFY);
		SendInsight(pCom, modify_send, modify_index);
	}
	else if(m_tGuildWar == GUILD_WAR_AFFTER)
	{
		m_tNpcAttType = 1;
		if(m_tNpcType == NPCTYPE_GUILD_GUARD) m_tNpcType = NPCTYPE_GUARD;
		else if(m_tNpcType == NPCTYPE_GUILD_NPC) m_tNpcType = NPCTYPE_NPC;

		FillNpcInfo(modify_send, modify_index, INFO_MODIFY);
		SendInsight(pCom, modify_send, modify_index);
	}
*/
}

void CNpc::SetDamagedInGuildWar(int nDamage, USER *pUser)// COM *pCom)
{
	int i, j;
	BOOL flag = FALSE;

	CNpc *pNpc = NULL;
//	int uid = uuid - USER_BAND;
//	USER* pUser = GetUser(pCom, uid);
														// ������� ��û�� ������� �Ǵ�.
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_dwGuild <= 0 || pUser->m_tGuildWar == GUILD_WAR_AFFTER) return;
	if(pUser->m_dwGuild == m_pGuardStore->m_iGuildSid) return;	// ������� ���Ƿ� �����ϴ°��� �����Ѵ�.

	if(m_pGuardStore->m_lUsed == 0) return;				// ������� ������.

	for(j = 0; j < GUILD_ATTACK_MAX_NUM; j++)
	{
		if(pUser->m_dwGuild != m_pGuardStore->m_arAttackGuild[j]) continue;

		m_sHP -= nDamage;
		if( m_sHP <= 0 )								// ���⿡�� ������� ������.
		{
			m_sHP = m_sMaxHP;
			if(InterlockedCompareExchange((LONG*)&m_pGuardStore->m_lUsed, (long)0, (long)1) == (long)1)
			{											// 1���� 0���� ����� �ش� ���������� �������� �˸���.
				if(pUser->StoppingTheGuildWar(m_pGuardStore))
				{										// �ش� NPC���� �˷��ش�.
					for(i =0; i < m_pGuardStore->m_arNpcList.GetSize(); i++)
					{
						pNpc = GetNpc(m_pGuardStore->m_arNpcList[i]);
						if(pNpc) 
						{
							pNpc->m_tGuildWar = GUILD_WAR_AFFTER;
							pNpc->m_tNpcAttType = 0;
						}
					}
					m_tGuildWar = GUILD_WAR_AFFTER;
					flag = TRUE;
					break;
				}
			}
		}
	}

	if(flag)
	{
		for(j = 0; j < GUILD_ATTACK_MAX_NUM; j++)
		{
			m_pGuardStore->m_arAttackGuild[j] = 0;
		}
	}

	return;
}

void CNpc::Send(USER *pUser, TCHAR *pBuf, int nLength)
{
	if ( !pUser ) return;

	pUser->Send( pBuf, nLength );
}

///////////////////////////////////////////////////////////////////////////////////
//	�ӽ� �̺�Ʈ �ڵ��� (�Ⱓ : 2001�� 12�� 29�� ~~ 2002�� 1�� 2��)
//
//@@@@@@@@@@@@@@@@@@@@@@@@
void CNpc::GiveEventItemToUser(USER *pUser)
{
	return;

	BOOL bFlag = FALSE;

	int iEventItemSid = 0;
	int iEventNum = -1;
	int iEvent = 0;
	int iSlot = 0;
	
	int j;

	SYSTEMTIME gTime;
	GetLocalTime(&gTime);
//	if(gTime.wYear == 2002 && gTime.wDay >= 2) return;

//	if(gTime.wMonth != 2) return;					// 2�� ������ �̺�Ʈ
	if(gTime.wDay < 7 || gTime.wDay > 13) return;// 8�Ϻ��� 13�ϱ���	

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	
	if(abs(m_byClassLevel - pUser->m_sLevel) > 25)
	{
		if(m_byClassLevel < pUser->m_sLevel) return;
	}

	iEventNum = GetEventItemNum(pUser->m_pCom);

	if(iEventNum < 0) return;
	
	int type = (int)g_arAddEventItemTable[iEventNum]->m_tType;
	if(type < 100 || type > 255) return;

	if(!UpdateEventItem(iEventNum)) 
	{
		g_arAddEventItemTable[iEventNum]->m_tEnd = 0;
		return;
	}

	CString strMsg = _T("");

	iSlot = pUser->GetEmptySlot(INVENTORY_SLOT);

	if(iSlot != -1)
	{
		if(NPC_EVENT_ITEM >= g_arItemTable.GetSize())
		{
			int ttt = 0;
		}
		if(pUser->m_iMaxWeight >= pUser->m_iCurWeight + g_arItemTable[NPC_EVENT_ITEM]->m_byWeight) bFlag = TRUE;
	}

	switch(type)
	{
/*	case 1:
		if(bFlag) { iEvent = EVENT_SP1_ITEM; strMsg.Format("���� %s�Բ��� ��ȭ�� ��ǰ�Ǹ� �����̽��ϴ�.", pUser->m_strUserID); }
		else      iEvent = 1001;
		break;
	case 2:
		if(bFlag) { iEvent = EVENT_SP2_ITEM; strMsg.Format("���� %s�Բ��� ��ȭ ��ǰ�Ǹ� �����̽��ϴ�.", pUser->m_strUserID); }
		else      iEvent = 1002;
		break;
	case 3:
		if(bFlag) { iEvent = EVENT_DEF_ITEM; strMsg.Format("���� %s�Բ��� �� ��ȯ�Ǹ� �����̽��ϴ�.", pUser->m_strUserID); }
		else      iEvent = 1003;
		break;
	case 4:
		if(bFlag) { iEvent = EVENT_ATT_ITEM; strMsg.Format("���� %s�Բ��� ���� ��ȯ�Ǹ� �����̽��ϴ�.", pUser->m_strUserID); }
		else      iEvent = 1004;
		break;
	case 5:
		if(bFlag) { iEvent = EVENT_POT_ITEM; strMsg.Format("���� %s�Բ��� ���� ��ȯ�Ǹ� �����̽��ϴ�.", pUser->m_strUserID); }
		else      iEvent = 1005;
		break;
*/
	case EVENT_ATT7_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_ATT7_ITEM; strMsg.Format(IDS_EVENT_ATT7_ITEM, pUser->m_strUserID); }
		else      iEvent = 1001;
		break;
	case EVENT_DEF7_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_DEF7_ITEM; strMsg.Format(IDS_EVENT_DEF7_ITEM, pUser->m_strUserID); }
		else      iEvent = 1002;
		break;
	case EVENT_ATT6_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_ATT6_ITEM; strMsg.Format(IDS_EVENT_ATT6_ITEM, pUser->m_strUserID); }
		else      iEvent = 1003;
		break;
	case EVENT_DEF6_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_DEF6_ITEM; strMsg.Format(IDS_EVENT_DEF6_ITEM, pUser->m_strUserID); }
		else      iEvent = 1004;
		break;
	case EVENT_ATT_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_ATT_ITEM; strMsg.Format(IDS_EVENT_ATT5_ITEM, pUser->m_strUserID); }
		else      iEvent = 1005;
		break;
	case EVENT_DEF_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_DEF_ITEM; strMsg.Format(IDS_EVENT_DEF5_ITEM, pUser->m_strUserID); }
		else      iEvent = 1006;
		break;
	case EVENT_ATT4_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_ATT4_ITEM; strMsg.Format(IDS_EVENT_ATT4_ITEM, pUser->m_strUserID); }
		else      iEvent = 1007;
		break;
	case EVENT_DEF4_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_DEF4_ITEM; strMsg.Format(IDS_EVENT_DEF4_ITEM, pUser->m_strUserID); }
		else      iEvent = 1008;
		break;
	case EVENT_ATT3_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_ATT3_ITEM; strMsg.Format(IDS_EVENT_ATT3_ITEM, pUser->m_strUserID); }
		else      iEvent = 1009;
		break;
	case EVENT_DEF3_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_DEF3_ITEM; strMsg.Format(IDS_EVENT_DEF3_ITEM, pUser->m_strUserID); }
		else      iEvent = 1010;
		break;

	case EVENT_INIT_STAT_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_INIT_STAT; iEvent = EVENT_INIT_STAT_ITEM; strMsg.Format(IDS_EVENT_RESET_STAT, pUser->m_strUserID); }
		else      iEvent = 1011;
		break;

	case EVENT_USER_GAME_TIME:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_USER_GAME_TIME; strMsg.Format(IDS_EVENT_PERSONAL, pUser->m_strUserID); }
		else      iEvent = 1012;
		break;
		break;
	default:
		return;
		break;
	}

	if(bFlag)								// �ڵ����� �κ��� ã�Ƽ� ����.
	{
		if(iEventItemSid == NPC_EVENT_ITEM || iEventItemSid == NPC_EVENT_INIT_STAT)
		{
			pUser->m_UserItem[iSlot].tType = TYPE_ITEM;
			pUser->m_UserItem[iSlot].sLevel = g_arItemTable[iEventItemSid]->m_byRLevel;
			pUser->m_UserItem[iSlot].sSid = g_arItemTable[iEventItemSid]->m_sSid;
			pUser->m_UserItem[iSlot].sCount = 1;
			pUser->m_UserItem[iSlot].sDuration = g_arItemTable[iEventItemSid]->m_sDuration;
			pUser->m_UserItem[iSlot].sBullNum = g_arItemTable[iEventItemSid]->m_sBullNum;
			pUser->m_UserItem[iSlot].tIQ = (BYTE)iEvent;
			pUser->m_UserItem[iSlot].iItemSerial = 0;

			SetISerialToItem(&pUser->m_UserItem[iSlot], iEventNum);
/*			for(j = 0; j < MAGIC_NUM; j++)
			{
				pUser->m_UserItem[iSlot].tMagic[j] = 0;
				pUser->m_UserItem[iSlot].tMagic[j] = tSerial[j];//g_arAddEventItemTable[iEventNum]->m_strSerialNum[j];
			}
*/
			CBufferEx TempBuf;

			TempBuf.Add(ITEM_LOAD_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add((BYTE)0x01);

			TempBuf.Add((BYTE)iSlot);
			TempBuf.Add(pUser->m_UserItem[iSlot].sLevel);
			TempBuf.Add(pUser->m_UserItem[iSlot].sSid);
			TempBuf.Add(pUser->m_UserItem[iSlot].sDuration);
			TempBuf.Add(pUser->m_UserItem[iSlot].sBullNum);
			TempBuf.Add(pUser->m_UserItem[iSlot].sCount);
			for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)pUser->m_UserItem[iSlot].tMagic[j]);

			TempBuf.Add((BYTE)pUser->m_UserItem[iSlot].tIQ);

			pUser->Send(TempBuf, TempBuf.GetLength());

			pUser->m_iCurWeight += g_arItemTable[iEventItemSid]->m_byWeight;
			pUser->GetRecoverySpeed();			// ������ ���Կ� ������ ����� ȸ���ӵ� ��ȯ

			pUser->m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL);
//			pUser->m_pCom->Announce(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL);
		}
		return;
	}
											// �κ��ֱ⿡ �����ϸ� �ʿ� ����
	GiveItemToMap(pUser->m_pCom, iEvent, TRUE, iEventNum);		// �̺�Ʈ ������
}

///////////////////////////////////////////////////////////////////////////////////
//	�̺�Ʈ �ڵ��� (�Ⱓ : 2002�� 4�� 8�� ~~ 2002�� 4�� 13��)
//
//@@@@@@@@@@@@@@@@@@@@@@@@
void CNpc::SetISerialToItem(ItemList *pItem, int iEventSid)
{
	int i, j = 0;
	TCHAR strTemp[3];

	if(!pItem) return;
	if(iEventSid < 0 || iEventSid >= g_arAddEventItemTable.GetSize()) return;

	for(i = 0; i < MAGIC_NUM; i++)
	{	
		::ZeroMemory(strTemp, sizeof(strTemp));
		strncpy(strTemp,g_arAddEventItemTable[iEventSid]->m_strSerialNum+j, 3);

		pItem->tMagic[i] = 0;
		pItem->tMagic[i] = atoi(strTemp);
		j = j + 4;
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	�̺�Ʈ �ڵ��� (�Ⱓ : 2002�� 4�� 8�� ~~ 2002�� 4�� 13��)
//
//@@@@@@@@@@@@@@@@@@@@@@@@
int CNpc::GetEventItemNum(COM *pCom)
{
	int i, iRet = -1;
	DWORD dwCurTick = 0;
	DWORD dwPreTick = 0;

	EnterCriticalSection( &(pCom->m_critEvent) );

	dwCurTick = GetTickCount();

	for(i = 0; i < g_arAddEventItemTable.GetSize(); i++)
	{
		int tt = g_arAddEventItemTable[i]->m_tEnd;
		if(!g_arAddEventItemTable[i]->m_tEnd)				// ���� �������� �ʾҴٸ�
		{
			if(i == 0) dwPreTick = 0;
			else dwPreTick = g_arAddEventItemTable[i - 1]->m_dwTick;
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22
			if(dwCurTick - dwPreTick >= 60000 * 20)		// 24���� �Ѿ��ٸ�
//			if(dwCurTick - dwPreTick >= 1000)				// 1���� �Ѿ��ٸ�	
			{
				g_arAddEventItemTable[i]->m_dwTick = dwCurTick;
				g_arAddEventItemTable[i]->m_tEnd = 1;
														// �������� ��ȣ�� �����ؼ� �ش�.	
				if(g_arAddEventItemTable[i]->m_tGiveFlag)	iRet = g_arAddEventItemTable[i]->m_sSid;
			}

			LeaveCriticalSection( &(pCom->m_critEvent) );
			return iRet;
		}
	}

	LeaveCriticalSection( &(pCom->m_critEvent) );
	return iRet;
}

///////////////////////////////////////////////////////////////////////////////////
//	�ӽ� �̺�Ʈ �ڵ��� (�Ⱓ : 2001�� 12�� 29�� ~~ 2002�� 1�� 2��)
//
//@@@@@@@@@@@@@@@@@@@@@@@@
BOOL CNpc::UpdateEventItem(int sid)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		

	SQLINTEGER		iRetInd = SQL_NTS;

	SQLSMALLINT		sRet = 0;

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_EVENT_ITEM(%d, ?)}"), sid);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		return FALSE;
	}

	retcode = SQLBindParameter( hstmt, 1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT,0,0, &sRet,0, &iRetInd);
	if( retcode != SQL_SUCCESS )
	{
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		return FALSE;
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
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;
	if(sRet = 0) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	������ �ȵǾ� ������ ���� HP�� �����Ѵ�.	
//
void CNpc::SetFortressState()
{
	for(int i = 0; i < GUILD_REPAIR_MAX_NUM; i++)
	{
		if(m_pGuardFortress->m_arRepairDBList[i].sUid == m_sEZone)
		{
			if(m_pGuardFortress->m_arRepairDBList[i].sHP < m_sMaxHP)
			{
				m_sHP = m_pGuardFortress->m_arRepairDBList[i].sHP;

				if(m_sHP == 0)  m_tRepairDamaged = NPC_DEAD_REPAIR_STATE; 
				else			m_tRepairDamaged = NPC_NEED_REPAIR_STATE;

				break;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	N_Circle�� ���� ó���� ����Ѵ�.
//
//void CNpc::SetDamagedInFortressWar(int nDamage, TCHAR *id, int uuid, COM *pCom)
void CNpc::SetDamagedInFortressWar(int nDamage, USER *pUser)
{
	int i;
	int iCount = 0;
	int index = 0;

	BOOL bSuccess = FALSE;
	
	CBufferEx TempBuf;
	CNpc *pNpc = NULL;
														// ������� ��û�� ������� �Ǵ�.
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_dwGuild <= 0 || !m_pGuardFortress) return;

	if(pUser->m_tFortressWar == GUILD_WAR_AFFTER) return;

	if(m_pGuardFortress->m_lUsed == 0) return;			// ������� ������.

	if(m_pGuardFortress->m_lChangeUsed == 1) return;

	for(i = 0; i < GUILDFORTRESS_ATTACK_MAX_NUM; i++)
	{
		if(pUser->m_dwGuild == m_pGuardFortress->m_arAttackGuild[i].lGuild)
		{
			bSuccess = TRUE;
			break;
		}
	}

	if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid) bSuccess = TRUE;
	
	if(!bSuccess) return;								// ��û�� ��尡 �ƴ�

	if(InterlockedCompareExchange((LONG*)&m_lFortressState, (long)1, (long)0) == (long)0)
	{
		if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid)	// ������� 
		{
			if(m_tNCircle != NPC_NCIRCLE_ATT_STATE)
			{
				InterlockedExchange(&m_lFortressState, (LONG)0); 
				return;
			}

			m_sHP -= nDamage;									// �������� - ��
			if(m_sHP <= 0)
			{
				m_sHP = m_sMaxHP;
				m_byColor = 0;
				m_tNCircle = NPC_NCIRCLE_DEF_STATE;
				SendFortressNCircleColor(pUser->m_pCom);
			}
		}
		else
		{
			if(m_tNCircle != NPC_NCIRCLE_DEF_STATE) 
			{
				InterlockedExchange(&m_lFortressState, (LONG)0); 
				return;
			}

			m_sHP -= nDamage;									// �������� - ��
			if(m_sHP <= 0)
			{
				m_sHP = m_sMaxHP;
				m_byColor = 1;
				m_tNCircle = NPC_NCIRCLE_ATT_STATE;
				SendFortressNCircleColor(pUser->m_pCom);
			}
		}

		iCount = 0;
		for(i = 0; i < FORTRESS_TARGET_MAX_NUM; i++)
		{
			pNpc = NULL;
			pNpc = GetNpc(m_pGuardFortress->m_arFortressTarget[i].sTargertID);	
			if(pNpc)
			{
				if(pNpc->m_tNCircle == NPC_NCIRCLE_ATT_STATE) iCount++; 
			}
		}

		if(iCount == FORTRESS_TARGET_MAX_NUM)
		{
			if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid)
			{
				InterlockedExchange(&m_lFortressState, (LONG)0); 
				return;
			}

			for(i = 0; i < g_arGuildFortress.GetSize(); i++)
			{
				if(!g_arGuildFortress[i]) continue;

				if(g_arGuildFortress[i]->m_sFortressID == m_pGuardFortress->m_sFortressID)
				{
					if(g_arGuildFortress[i]->m_lUsed == 1)
					{
						if(InterlockedCompareExchange((LONG*)&g_arGuildFortress[i]->m_lChangeUsed, (long)1, (long)0) == (long)0)
						{
							FORTRESSDATAPACKET *pFDP = NULL;
							pFDP = new FORTRESSDATAPACKET;

							pFDP->sFortressIndex = i;
							
							memset(pFDP->FORTRESS, NULL, CHAR_NAME_LENGTH+sizeof(int)+1);
							index = strlen(pUser->m_strGuildName);
							if(index > 0 && index <= CHAR_NAME_LENGTH) memcpy(pFDP->FORTRESS, pUser->m_strGuildName, index );

							EnterCriticalSection( &m_CS_FortressData );
							RecvFortressData.AddTail(pFDP);
 							nFortressDataCount = RecvFortressData.GetCount();
							LeaveCriticalSection( &m_CS_FortressData );	

							pUser->StoppingTheFortressWar(g_arGuildFortress[i]);		// ������ �ð����̹Ƿ� ������ ��ӵǾ�� �Ѵ�. 
							InterlockedExchange(&g_arGuildFortress[i]->m_lChangeUsed, (LONG)0); 
						}
					}
					break;
				}
			}
		}

		InterlockedExchange(&m_lFortressState, (LONG)0); 
	}

	return;
}

//void CNpc::SetDoorDamagedInFortressWar(int nDamage, TCHAR *id, int uuid, COM *pCom)
void CNpc::SetDoorDamagedInFortressWar(int nDamage, USER *pUser)
{													// ������ �Ⱓ�̶�� ������ �����ϵ���.. �� ������� �ȵ�
	if(!pUser || !m_pGuardFortress) return;
	if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid) return;	// ������� ���Ƿ� �����ϴ°��� �����Ѵ�.

	if(m_pGuardFortress->m_lUsed == 0) return;				// ������� ������.

	m_sHP -= nDamage;

	if( m_sHP <= 0 )
	{
		m_sHP = 0;

		MAP* pMap = g_zone[m_ZoneIndex];
		pMap->m_pMap[m_sCurX][m_sCurY].m_lUser = 0;

		m_NpcState = NPC_DEAD;

		m_Delay = m_sRegenTime;
		m_bFirstLive = FALSE;

		SetMapAfterGuildWar();

		SendDead(pUser->m_pCom);
	}
}

void CNpc::SendFortressNCircleColor(COM *pCom)
{
	int modify_index = 0;
	char modify_send[2048];	

	CBufferEx TempBuf;

	TempBuf.Add(GUILD_FORTRESS_NCIRCLE);
	TempBuf.Add((BYTE)0x00);					// �ش� N_Circle�� ���� ��ȭ
	TempBuf.Add((int)(m_sNid + NPC_BAND));
	TempBuf.Add(m_tNCircle);

	SendFortressInsight(pCom, TempBuf, TempBuf.GetLength());

	::ZeroMemory(modify_send, sizeof(modify_send));
	FillNpcInfo(modify_send, modify_index, INFO_MODIFY);
	SendFortressInsight(pCom, modify_send, modify_index);

}


void CNpc::SetMapTypeBeforeGuildWar(COM *pCom)
{
	int i;
	int uid = 0;

	USER *pUser = NULL;
	CNpc *pNpc = NULL;

	int x, y;

	long lNpcUid = 0;

//	POINT temp1Map[12] = {{-2,-2}, {-2,-1}, {-2,0}, {-1,-1}, {-1,0}, {0,-1}, {0,0},{1,-1},{1,0}, {2,-1}, {2,0}, {2,1}};
	POINT temp1Map[16] = {{-3,-2}, {-4,-1}, {-3,-1}, {-2,-1}, {-4,0}, {-3,0}, {-2,0},{-1,0},{0,0}, {1,0}, {2,0}, {1,1}, {2,1}, {2,2},{3,2},{3,3}};
	POINT temp2Map[17] = {{-2,-3}, {-1,-3}, {0,-3}, {-1,-2}, {0,-2}, {0,-1}, {0,0},{0,1},{1,1}, {0,2}, {1,2}, {2,2}, {0,3}, {1,3},{2,3},{0,4},{1,4}};
	POINT temp3Map[6] = {{-2,0},{-1,0},{0,0},{1,0},{2,0},{2,1}};
	
//	POINT temp2Map[] = {{1,-2}, {1,-1}, {1,0}, {1,1}, {1,2}, {0,-2},{0,-1},{0,0},{0,1},{0,2}};

	MAP* pMap = g_zone[m_ZoneIndex];

	switch(m_sDimension)
	{
	case 1:
		lNpcUid = m_sNid + NPC_BAND;

		for(i =0; i < sizeof(temp1Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp1Map[i].x;
			y = m_sCurY + temp1Map[i].y;
	
			uid = pMap->m_pMap[x][y].m_lUser;

			if(uid >= USER_BAND && uid < NPC_BAND)	// Target �� User �� ���
			{
				pUser = GetUser(pCom, uid - USER_BAND);
				if( pUser->m_tIsOP != 1 ) pUser->TownPotal();
			}

			::InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, lNpcUid);
		}
		break;
	case 2:
		lNpcUid = m_sNid + NPC_BAND;

		for(i =0; i < sizeof(temp2Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp2Map[i].x;
			y = m_sCurY + temp2Map[i].y;
	
			uid = pMap->m_pMap[x][y].m_lUser;

			if(uid >= USER_BAND && uid < NPC_BAND)	// Target �� User �� ���
			{
				pUser = GetUser(pCom, uid - USER_BAND);
				if( pUser->m_tIsOP != 1 ) pUser->TownPotal();
			}

			::InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, lNpcUid);
		}
		break;
	case 3:
		lNpcUid = m_sNid + NPC_BAND;

		for(i =0; i < sizeof(temp3Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp3Map[i].x;
			y = m_sCurY + temp3Map[i].y;
	
			uid = pMap->m_pMap[x][y].m_lUser;

			if(uid >= USER_BAND && uid < NPC_BAND)	// Target �� User �� ���
			{
				pUser = GetUser(pCom, uid - USER_BAND);
				if( pUser->m_tIsOP != 1 ) pUser->TownPotal();
			}

			::InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, lNpcUid);
		}
		break;

	}
}


void CNpc::SetMapAfterGuildWar()
{
	int i;
	int uid = 0;

	USER *pUser = NULL;

	int x, y;

//	POINT temp1Map[12] = {{-2,-2}, {-2,-1}, {-2,0}, {-1,-1}, {-1,0}, {0,-1}, {0,0},{1,-1},{1,0}, {2,-1}, {2,0}, {2,1}};
//	POINT temp2Map[] = {{1,-2}, {1,-1}, {1,0}, {1,1}, {1,2}, {0,-2},{0,-1},{0,0},{0,1},{0,2}};
	POINT temp1Map[16] = {{-3,-2}, {-4,-1}, {-3,-1}, {-2,-1}, {-4,0}, {-3,0}, {-2,0},{-1,0},{0,0}, {1,0}, {2,0}, {1,1}, {2,1}, {2,2},{3,2},{3,3}};
	POINT temp2Map[17] = {{-2,-3}, {-1,-3}, {0,-3}, {-1,-2}, {0,-2}, {0,-1}, {0,0},{0,1},{1,1}, {0,2}, {1,2}, {2,2}, {0,3}, {1,3},{2,3},{0,4},{1,4}};
	POINT temp3Map[6] = {{-2,0},{-1,0},{0,0},{1,0},{2,0},{2,1}};

	MAP* pMap = g_zone[m_ZoneIndex];

	switch(m_sDimension)
	{
	case 1:
		for(i =0; i < sizeof(temp1Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp1Map[i].x;
			y = m_sCurY + temp1Map[i].y;

			InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, 0);
		}
		break;
	case 2:
		for(i =0; i < sizeof(temp2Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp2Map[i].x;
			y = m_sCurY + temp2Map[i].y;

			InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, 0);
		}
		break;
	case 3:
		for(i =0; i < sizeof(temp3Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp3Map[i].x;
			y = m_sCurY + temp3Map[i].y;

			InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, 0);
		}
		break;
	}
}

void CNpc::SendFortressInsight(COM *pCom, TCHAR *pBuf, int nLength)
{
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;

	int insight_range = 10;

	int sx = m_sCurX / SIGHT_SIZE_X;
	int sy = m_sCurY / SIGHT_SIZE_Y;
	
	int min_x = (sx-8)*(SIGHT_SIZE_X); if( min_x < 0 ) min_x = 0;
	int max_x = (sx+9)*(SIGHT_SIZE_X);
	int min_y = (sy-8)*(SIGHT_SIZE_Y); if( min_y < 0 ) min_y = 0;
	int max_y = (sy+9)*(SIGHT_SIZE_Y);
	
	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return;
	
	if( max_x >= pMap->m_sizeMap.cx ) max_x = pMap->m_sizeMap.cx - 1;
	if( max_y >= pMap->m_sizeMap.cy ) max_y = pMap->m_sizeMap.cy - 1;
	
	int temp_uid;
	USER* pUser = NULL;

	for( int i = min_x; i < max_x; i++ )
	{
		for( int j = min_y; j < max_y; j++ )
		{				
			temp_uid = pMap->m_pMap[i][j].m_lUser;

			if(temp_uid < USER_BAND || temp_uid >= NPC_BAND) continue;
			else temp_uid -= USER_BAND;
			
			if( temp_uid >= 0 && temp_uid < MAX_USER )
			{
				pUser = pCom->GetUserUid(temp_uid);
				if(pUser == NULL) continue;
				
				if( pUser->m_state == STATE_GAMESTARTED )
				{
					if( pUser->m_curx == i && pUser->m_cury == j && pUser->m_curz == m_sCurZ )
					{
						Send( pUser, pBuf, nLength );
					}
				}
			}
		}
	}
}

void CNpc::TestCode(COM *pCom, USER *pUser)
{
	int i;
	int uid = 0;

	CNpc *pNpc = NULL;

	int x, y;

	long lNpcUid = 0;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

	POINT temp1Map[12] = {{-2,-2}, {-2,-1}, {-2,0}, {-1,-1}, {-1,0}, {0,-1}, {0,0},{1,-1},{1,0}, {2,-1}, {2,0}, {2,1}};// server
//	POINT temp2Map[8] = {{0,0}, {0,1}, {0,2}, {0,3}, {-1,0}, {-1,1}, {-1,2},{-1,3}};
//	POINT temp1Map[12] = {{147,1183}, {148,1184}, {149,1185}, {149,1183}, {150,1184}, {150,1182}, {151,1183},{151,1181},{152,1182}, {152,1180}, {153,1181}, {154,1182}};
	POINT temp2Map[] = {{-1,-3}, {0,-2}, {1,-1}, {2,0}, {3,1}, {-2,-2}, {-1,-1}, {0,0}, {1,1}, {2,2}};	// client

	MAP* pMap = g_zone[m_ZoneIndex];

	CPoint temp = ConvertToClient(m_sCurX, m_sCurY);
	switch(m_sDimension)
	{
	case 1:
		lNpcUid = m_sNid + NPC_BAND;

		for(i =0; i < 12; i++)
		{
			x = m_sCurX + temp1Map[i].x;
			y = m_sCurY + temp1Map[i].y;
	
			CString strMsg = _T("");
			CPoint pt = ConvertToClient(x, y);
//			CPoint pt = ConvertToServer(temp1Map[i].x, temp1Map[i].y);
			strMsg.Format("1Luinet locked door x = %d, y = %d", pt.x - temp.x, pt.y - temp.y);

//			pUser->NormalChat(strMsg.GetBuffer(strMsg.GetLength()));

//			::InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, lNpcUid);
		}
		break;
	case 2:
		lNpcUid = m_sNid + NPC_BAND;

		for(i =0; i < 10; i++)
		{
			x = temp.x + temp2Map[i].x;
			y = temp.y + temp2Map[i].y;

			CPoint pt = ConvertToServer(x, y);

			CString strMsg = _T("");			
//			CPoint pt = ConvertToServer(temp1Map[i].x, temp1Map[i].y);
			strMsg.Format("1Sanad locked door x = %d, y = %d", pt.x - m_sCurX, pt.y - m_sCurY);

//			pUser->NormalChat(strMsg.GetBuffer(strMsg.GetLength()));

		}
		break;
	}
}

int CNpc::PsiAttack(COM *pCom) //NPCħ������
{
	DWORD	dwExp = 0;
	int		nDamage = 0;
	int		nTempHP = 0;

	USER*	pUser = NULL;
	CNpc*	pNpc = NULL;

	BYTE	tWeaponClass = 0;
	BOOL	bCanUseSkill = FALSE;
	int		bSuccessSkill[SKILL_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE};

	int		nPsiRange = 0;
	int		nTPosX	= 0;
	int		nTPosY	= 0;
	int		nDist	= 100;
	short	sNeedPP	= 25000;
	BYTE	tPsiRegi = 0;
	DWORD	dwPsiCast = 0;

	BOOL	bPsiSuccess = FALSE;

	int index = 0;

	int delay = -1;
	int nTargetID	= m_Target.id;			// Target ID �� ��´�.
	BYTE byPsi		= m_byPsi;				// Psionic sid �� ��´�.

	int nPsiX = -1;							// Teleport�� ��ġ
	int nPsiY = -1;
	CPoint ptPsi(-1, -1);

	if( byPsi < 0 || byPsi >= g_arMonsterPsi.GetSize() ) return -1;

	if( nTargetID < USER_BAND || nTargetID >= INVALID_BAND ) return-1;	// �߸��� Target �̸� return

	pUser = GetUser( pCom, nTargetID - USER_BAND );
	if( !pUser ) return -1;

	CMonsterPsi* pMagic = g_arMonsterPsi[(int)byPsi];
	if( !pMagic ) return -1;
	
	// �����Ÿ� ��� ------------------------------------------------------------------------//
	if( !IsCloseTarget( pCom, (int)pMagic->m_byRange ) ) return -1;

	short damage, result;

	if(pMagic->m_sSid != 0)	
	{	
		damage = myrand( pMagic->m_sMinDmg, pMagic->m_sMaxDmg );
	
		result = damage * m_sVOL - pUser->GetUserSpellDefence();

		result = result - ( pUser->m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+ pUser->m_DynamicUserData[MAGIC_PSI_ATTACK_DOWN]); //���������˺�
		if(result < 0) result = 15;

/*		//������ڻ������򹥻�����
		if(pUser->m_tHuFaType &&pUser->m_nHuFaHP>0)
		{
			int nID = pUser->m_uid+USER_BAND+USER_HUFA_BAND;
			//�˺�����
			pUser->SendDamageNum(0,nID,(short)result);

			SendAttackSuccess(pCom, nID, 0, pUser->m_nHuFaHP, pUser->m_nHuFaMaxHP);//yskang 0.3
			if(nDamage > 0)
				pUser->SetHuFaDamage(result);
			if(pUser->m_nHuFaHP>0)
			{
				pUser->HuFaAttack(m_sNid+NPC_BAND);
			}
			CBufferEx TempBuf;
			TempBuf.Add(PSI_ATTACK_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add( (BYTE)33 );				// Psionic sid
			TempBuf.Add( m_sNid + NPC_BAND );
			TempBuf.Add( nID );
			SendExactScreen(pCom, TempBuf, TempBuf.GetLength());
			return 1200;
		}*/
		pUser->SetDamage((int)result);	
//		pUser->SetFaNu(result);
		pUser->SendDamageNum(0,pUser->m_uid+USER_BAND,result);
		if(pUser->m_lDeadUsed == 1)
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			delay = m_sStandTime;

			if(m_NpcVirtualState == NPC_STANDING) 
			{
				if(m_sPid == 179) pUser->GetLevelDownExp(FALSE, -1, TRUE,m_strName); // ħ���� �����ϰ�� ����ġ 1%����
				else			pUser->GetLevelDownExp(FALSE, -1, FALSE,m_strName);		// ����ġ�� �׿� ��ȭ���� �ݿ��Ѵ�.
			}
		}
		else
		{
			
			if(m_sPid == 212) //�ڰ����� ��ħ���й�
			{
				int iRandom = myrand(1, 10000);
				if(iRandom <= 3000)	
				{					
					CBufferEx TempBuf;
					TempBuf.Add(PSI_ATTACK_RESULT);
					TempBuf.Add(SUCCESS);
					TempBuf.Add( (BYTE)33 );				// Psionic sid
					TempBuf.Add( m_sNid + NPC_BAND );
					TempBuf.Add( nTargetID );
					SendExactScreen(pCom, TempBuf, TempBuf.GetLength());
					delay = (int)1200;					
					CPoint pt(-1, -1);
					pt = pUser->FindNearAvailablePoint_S(m_sCurX, m_sCurY);
					for(int i = 0; i< 2;i++){
						pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
				    //    pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					 //   pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					  //  pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					   // pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					    //pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					    //pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					}
					return (int)delay;
				}									
				
			}
			switch(pMagic->m_sSid)
			{
				case	8:
				case	27:
				case	31:
				case	32:
					GetWideRangeAttack( pCom, pUser->m_curx, pUser->m_cury, (int)damage, nTargetID - USER_BAND );				
					break;
				case	2:		// ��� ����
				case	5:
				case	28:
				case	37:
				case	38:
				case	40:
				case	41:
				case	42:
				case	43:
					pUser->SetFireDamage();					
					break;				
				case	4:
				case	10:
				case	21:				
					pUser->SetColdDamage();					
					break;				
				case	24:
//					pUser->SetConFusion();					
					GetWideRangeAttack( pCom, pUser->m_curx, pUser->m_cury, (int)damage, nTargetID - USER_BAND );				
					break;				
			}
			
		}

	}

	CBufferEx TempBuf;

	TempBuf.Add(PSI_ATTACK_RESULT);
	TempBuf.Add(SUCCESS);

	TempBuf.Add( (BYTE)pMagic->m_sPid );				// Psionic sid
	TempBuf.Add( m_sNid + NPC_BAND );
	TempBuf.Add( nTargetID );

//	SendInsight( pCom, TempBuf, TempBuf.GetLength());
	SendExactScreen(pCom, TempBuf, TempBuf.GetLength());

	delay = (int)pMagic->m_sCasting;

	return (int)delay;
}

void CNpc::GetWideRangeAttack(COM* pCom, int x, int y, int damage, int except_uid)	// ������ ���ݸ� ó��...
{
	int dir[9][2];
	int ix, iy;
	int nTarget = 0;
	int nDamage = 0;
	double result = 0;

	USER* pUser = NULL;
	MAP* pMap = g_zone[m_ZoneIndex];
	if(!pMap) return;

	dir[0][0]  =  0;		dir[0][1] =  0;		// 
	dir[1][0]  = -1;		dir[1][1] =  0;		// 
	dir[2][0]  = -1;		dir[2][1] =  1;		// 
	dir[3][0]  =  0;		dir[3][1] =  1;		// 
	dir[4][0]  =  1;		dir[4][1] =  1;		// 

	dir[5][0]  =  1;		dir[5][1] =  0;		// 
	dir[6][0]  =  1;		dir[6][1] = -1;		// 
	dir[7][0]  =  0;		dir[7][1] = -1;		// 
	dir[8][0]  = -1;		dir[8][1] = -1;		// 

	for(int i = 1; i < 9; i++)
	{
		ix = x + dir[i][0];
		iy = y + dir[i][1];

		if(ix < 0) ix = 0;
		if(iy < 0) iy = 0;
		if(ix >= pMap->m_sizeMap.cx) ix = pMap->m_sizeMap.cx - 1;
		if(iy >= pMap->m_sizeMap.cy) iy = pMap->m_sizeMap.cy - 1;

		nTarget = pMap->m_pMap[ix][iy].m_lUser;

		if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
		{
			pUser = GetUser( pCom, nTarget - USER_BAND);			// User Pointer �� ��´�.

			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;						// �߸��� USER �̸� ����
			if(pUser->m_bLive == USER_DEAD)	continue;		// Target User �� �̹� �׾������� ����
			if(pUser->m_uid == except_uid ) continue;	// �߽ɿ� �ִ� ������ ������� �ʴ´�

			result = (double)damage * (double)( m_sVOL * 20 ) / (double)( pUser->m_sMagicVOL * 15 + pUser->m_DynamicUserData[MAGIC_PSI_RESIST_UP] + m_sVOL * 20 );
			pUser->SetDamage((int)result);

			if(pUser->m_sHP > 0)		// ���� ��� ���ⵥ���� �߰�
			{
//				pUser->SetColdDamage();
			}
			else
			{
//				IsChangeCityRank(pUser);
				if(m_sPid == 179) pUser->GetLevelDownExp(FALSE, -1, TRUE,m_strName); // ħ���� �����ϰ�� ����ġ 1%����
				else			pUser->GetLevelDownExp(USER_PK, -1, FALSE,m_strName);			// ����ġ�� �׿� ��ȭ���� �ݿ��Ѵ�.
			}
		}
/*
		else if(nTarget >= NPC_BAND)				// NPC
		{
			pNpc = GetNpc(nTarget - NPC_BAND);				// NPC Point �� ��´�.
			if(pNpc == NULL) continue;					// �߸��� NPC �̸� ����
			if(pNpc->m_NpcState == NPC_DEAD || pNpc->m_tNpcType != NPCTYPE_MONSTER) continue;	// NPC �� �̹� �׾� ������ ����
			if(pNpc->m_sHP <= 0) continue;

			nDamage = (int)(damage *  ((double)m_sMagicVOL / (m_sMagicVOL + pNpc->m_sVOL)));
			nDamage = (int)((double)nDamage/2 + 0.5);	// �������� 50%�� ����.

			if(pNpc->SetDamage(nDamage, m_strUserID, m_uid + USER_BAND, m_pCom) == FALSE)
			{
				if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT)
				{
					CheckGuildHouseWarEnd();
				}

				pNpc->SendExpToUserList(m_pCom); // ����ġ �й�!!
				pNpc->SendDead(m_pCom);
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
				if(difflevel < 30)
				{
					CheckMaxValue(m_dwXP, 1);		// ���� �������� 1 ����!	
					SendXP();
				}
			}
			else									// ���� ��� ���ⵥ���� �߰�
			{
//				pNpc->SetColdDamage();
			}
		}
*/
	}
}

int CNpc::AreaAttack(COM *pCom)
{
	if(!pCom) return 10000;
	if(m_tNpcType == NPCTYPE_GUARD) return -1;
	if(m_tNpcType == NPCTYPE_GUILD_GUARD) return -1;

	int nStandingTime = m_sStandTime;

	// �ѱ�迭 �϶��� Ÿ�ٰ��� �Ÿ������ �޸��ؾ� �Ѵ�.
	if(IsCloseTarget(pCom, m_byRange) == FALSE)// Check Code (���� ������� ���鿡�� ���� �ڵ�)
	{
		m_NpcState = NPC_TRACING;			// �����ϰ� �������� ������ ���� �������(������ ���� ������) 
		TRACE("AreaAttack - �Ÿ� �־ ����\n");
		return -1;							// IsCloseTarget()�� ���� x, y���� �����ϰ� Delay = 0���� ��
	}

	USER* pUser = NULL;
	int nRange = 1;				// ���� ���� : 1 - ���� 8ĭ, 2 - ���� 24ĭ...
	int nTargetCount = 0;
	int target_uid = -1;

	int center_x = m_sCurX;		// ���� ������ �߽��� : �߽��� �����ϴ� NPC�� ���� �ְ� � ���� ���� ���� �ִ�.
	int center_y = m_sCurY;		// ����� �ڱ� ���� �������� ����

	switch( (int)m_tSPATRange )
	{
	case	0:
	case	1:
		nRange = 2;
		center_x = m_sCurX;
		center_y = m_sCurY;
		break;

	case	2:
		nRange = 2;
		center_x = m_sCurX;
		center_y = m_sCurY;
		break;

	case	3:
		nRange = 1;
		center_x = m_Target.x;
		center_y = m_Target.y;
		break;

	case	4:
		nRange = 2;
		center_x = m_Target.x;
		center_y = m_Target.y;
		break;

	default:
		nRange = 1;
		center_x = m_sCurX;
		center_y = m_sCurY;
		break;
	}

	MAP* pMap = g_zone[m_ZoneIndex];
	if(!pMap)
	{
		TRACE("AreaAttack - �ʾ�� ����\n");
		return -1;
	}

	int min_x = center_x - nRange;		if( min_x < 0 ) min_x = 0;
	int min_y = center_y - nRange;		if( min_y < 0 ) min_y = 0;
	int max_x = center_x + nRange;
	int max_y = center_y + nRange;

	if(max_x >= pMap->m_sizeMap.cx) max_x = pMap->m_sizeMap.cx - 1;
	if(max_y >= pMap->m_sizeMap.cy) max_y = pMap->m_sizeMap.cy - 1;

	TargetUser tuser[25];


	int		nAvoid = 0;
	int		iRandom = 0;
	int		determine = 0;
	int		iDexHitRate = 0, iLevelHitRate = 0;
	short	sTempHP		= 0;

	int		nHit = 0;

	BOOL	bIsHit = FALSE;
	BOOL	bIsCritical = FALSE;

	int		nDamage		= 0;
	int		nDefense	= 0;

	int nID = m_Target.id;					// Target �� ���Ѵ�.

	// ���߿��� �Ǵ� ���� �ʱ�ȭ
	bIsHit = FALSE;		


	for( int ix = min_x; ix <= max_x; ix++ )
	{
		for( int iy = min_y; iy <= max_y; iy++ )
		{
			target_uid = pMap->m_pMap[ix][iy].m_lUser;

			if( target_uid < USER_BAND || target_uid >= NPC_BAND )
			{
				continue;
			}

			pUser = GetUser(pCom, target_uid - USER_BAND);

			if( !pUser ) continue;
			if( pUser->m_bLive != USER_LIVE ) continue;

			if( ix != pUser->m_curx || iy != pUser->m_cury ) continue;

			//if(pUser->m_state == STATE_DISCONNECTED) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;
			if(pUser->m_tIsOP == 1 ) continue;
			if(pUser->m_bPShopOpen == TRUE) continue;
            if(pUser->m_bSessionOnline == true) continue;
			// ȸ�ǰ� ��� 
			nAvoid = pUser->GetAvoid();

			// ���߿��� �Ǵ�
			iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5); 
			
			iDexHitRate = (int)( 30.0 * ( (double)m_sDEX/(m_sDEX + pUser->m_sMagicDEX) ) + 15.0 );
			iLevelHitRate = (int)( 70.0 * ( (double)m_byClassLevel/(pUser->m_sLevel + m_byClassLevel) ) + 15.0);

			determine = iDexHitRate + iLevelHitRate - (nAvoid+pUser->m_Avoid);

			if(determine < ATTACK_MIN) determine = ATTACK_MIN;			// �ּ� 20
			else if(determine > ATTACK_MAX) determine = ATTACK_MAX;		// �ִ�

			if(iRandom < determine)	bIsHit = TRUE;		// ����

			// ���� �̽�
			if(bIsHit == FALSE)
			{
				TRACE("AreaAttack - ���� �̽�\n");
				continue;
			}

			// �����̸� //Damage ó�� ----------------------------------------------------------------//

			nDamage = GetFinalDamage(pUser);	// ���� �����

			if(nDamage > 0) 
			{
		        pUser->SetDamage(nDamage);
			    pUser->SendDamageNum(0,pUser->m_uid+USER_BAND,nDamage);
			}
			// ����� ������ ����
			pUser->SendDamagedItem(nDamage);

//			if(pUser->m_bLive == USER_DEAD || pUser->m_sHP <= 0)//@@@ ���߿� ��ħ
			if(pUser->m_lDeadUsed == 1)
			{
				if(m_NpcVirtualState == NPC_STANDING) 
				{
					if(m_sPid == 179) pUser->GetLevelDownExp(FALSE, -1, TRUE,m_strName); // ħ���� �����ϰ�� ����ġ 1%����
					else			pUser->GetLevelDownExp(FALSE, -1, FALSE,m_strName);		// ����ġ�� �׿� ��ȭ���� �ݿ��Ѵ�.
				}
			}

			tuser[nTargetCount].iUid = target_uid;
			tuser[nTargetCount].sHP = pUser->m_sHP;
			tuser[nTargetCount].sMaxHP = pUser->m_sMagicMaxHP;
			pUser->SendHP();

			nTargetCount++;

			if( nTargetCount >= 25 ) break;
		}

		if( nTargetCount >= 25 ) break;
	}

	if( !nTargetCount )
	{
		TRACE("AreaAttack - ���� �ȿ� ���� ��� ����\n");
//		return -1;
	}

	CBufferEx TempBuf;
//	2a 0 0 0 ca 1 1 9f 5b 0 0 6a 2c 0 0 0 0
	TempBuf.Add(AREA_ATTACK_RESULT);
	TempBuf.Add(ATTACK_SUCCESS);
	TempBuf.Add( (byte)1 );
	TempBuf.Add((int)(m_sNid + NPC_BAND));
	
	for(int i = 0; i < 1; i++ )
	{
		TempBuf.Add( (int)tuser[i].iUid );
		TempBuf.Add( (short)0 );
		TempBuf.Add( (short)0 );
	}

	SendInsight(pCom, TempBuf, TempBuf.GetLength());
//	SendExactScreen(pCom, TempBuf, TempBuf.GetLength());

	TRACE("AreaAttack - ����\n");
	
	return m_sAttackDelay;
}



void CNpc::GiveEventItemNewToUser(USER *pUser)
{
	if( !pUser ) return;
	if( pUser->m_state != STATE_GAMESTARTED ) return;
//	if( pUser->m_iDisplayType == 6 && pUser->m_sLevel > 25) return; //yskang 0.5
	if( pUser->m_iDisplayType == 6) return; //yskang 0.5

	int i;
	CEventItemNew* pNewItem = NULL;

	BOOL bFlag = FALSE;

	int sItemSid = -1;
	BYTE tItemQuality = 0;
	BYTE tItemWear = 0;

	int j;
	int iSlot = -1;

	SYSTEMTIME time;
	GetLocalTime( &time );
	CString strMsg = _T("");

	MYSHORT upper;	upper.i = 0;
	MYINT lower;	lower.i = 0;

	for( i = 0; i < g_arEventItemNew.GetSize(); i++ )
	{
		if( g_arEventItemNew[i] )
		{
			pNewItem = g_arEventItemNew[i];

			if( ::InterlockedCompareExchange( (long*)&(pNewItem->m_lGive), (long)0, (long)1 ) == (long)0 ) continue;

			if( pNewItem->m_sSid != NPC_EVENT_LOTTO )
			{
				// ������ �ƴ� ��� �������� ���� ���� ���̰� 25���� �ʰ��ϸ� ���� �ʴ´�.
				if(abs(m_byClassLevel - pUser->m_sLevel) > 25)
				{
					if(m_byClassLevel < pUser->m_sLevel) return;
				}
			}
			else
			{
				// ������ �������� ���� ���� ���̰� 40���� �ʰ��ϸ� ���� �ʴ´�.
				if(abs(m_byClassLevel - pUser->m_sLevel) > 40)
				{
					if(m_byClassLevel < pUser->m_sLevel) return;
				}
			}

			//////////////////////////////////////////////////////////////////////
			// �߰��Ǵ� �̺�Ʈ �������� ������ �Ʒ��� �߰��Ѵ�.
			//////////////////////////////////////////////////////////////////////

			// �ɹٱ��� �̺�Ʈ
			if( pNewItem->m_sSid == NPC_EVENT_FLOWER )
			{
				if( time.wYear == 2002 && time.wMonth == 5 && ( time.wDay >= 1 || time.wDay <= 5 ) )
				{
					sItemSid = pNewItem->m_sSid;
					tItemQuality = 0;
				}
				else
				{
					return;
				}
			}

			if( pNewItem->m_sSid == NPC_EVENT_LOTTO )
			{
				if( time.wYear == 2002 && ( ( time.wMonth == 5 && time.wDay >= 16 ) || ( time.wMonth == 6 && time.wDay <= 22 ) ) )
				{
					sItemSid = pNewItem->m_sSid;
					tItemQuality = pNewItem->m_tQuality;
				}
				else
				{
					return;
				}

				if( pUser->m_sLevel < 25 )		// �̺�Ʈ ������ 25���� �̸��� ���� �ʴ´�.
				{
					return;
				}
			}

			/////////////////////////////////////////////////////////////////////

			if( sItemSid < 0 || sItemSid >= g_arItemTable.GetSize() ) return;
			CItemTable* pItemTable = g_arItemTable[sItemSid];

			iSlot = pUser->GetEmptySlot(INVENTORY_SLOT);

			if( iSlot != -1 )
			{
				if(pUser->m_iMaxWeight >= pUser->m_iCurWeight + pItemTable->m_byWeight) bFlag = TRUE;
			}

			switch( sItemSid )
			{
				case NPC_EVENT_FLOWER:
					if(bFlag) { strMsg.Format(IDS_EVENT_FLOWER, pUser->m_strUserID); }
					break;
				case NPC_EVENT_LOTTO:
					if(bFlag) { strMsg.Format(IDS_EVENT_LOTTO); }
					break;
				default:
					return;
			}

			ItemList newItem;
			pUser->ReSetItemSlot( &newItem );

			newItem.tType = TYPE_ITEM;
			newItem.sLevel = pItemTable->m_byRLevel;
			newItem.sSid = sItemSid;
			newItem.sCount = 1;
			newItem.sDuration = pItemTable->m_sDuration;
			newItem.sBullNum = pItemTable->m_sBullNum;
			newItem.tIQ = tItemQuality;
			newItem.iItemSerial = 0;

			for( j = 0; j < MAGIC_NUM; j++ ) newItem.tMagic[j] = 0;

			// �̺�Ʈ ������ �ֱ� ���̺����� ���� ������ �Ѱ� �ٿ��ش�.
			pNewItem->m_sRemain--;

			if( pNewItem->m_sRemain < 0 )
			{
				pNewItem->m_sRemain = 0;
			}
			if( !UpdateEventItemNewRemain( pNewItem ) )
			{
				pNewItem->m_sRemain++;
				::InterlockedExchange( &(pNewItem->m_lGive), 1 );
				return;
			}

			if( pNewItem->m_tSerialExist != 255 )		// �ø��� ��ȣ�� �ο��ؾ� �ϴ� ��Ȳ�̶��
			{
				upper.i = pNewItem->m_tSerialExist;		// 10000 ���� ��ȣ
				lower.i = pNewItem->m_sRemain;

				newItem.tMagic[0] = upper.b[0];
				newItem.tMagic[1] = upper.b[1];

				newItem.tMagic[2] = lower.b[0];
				newItem.tMagic[3] = lower.b[1];
				newItem.tMagic[4] = lower.b[2];
				newItem.tMagic[5] = lower.b[3];
			}

			// bFlag - �κ��� �󽽷��� �ְ�, �������ѿ� �ɸ��� �ʾ����� TRUE�̴�.
			if(bFlag)
			{
				pUser->m_UserItem[iSlot].tType = newItem.tType;
				pUser->m_UserItem[iSlot].sLevel = newItem.sLevel;
				pUser->m_UserItem[iSlot].sSid = newItem.sSid;
				pUser->m_UserItem[iSlot].sCount = newItem.sCount;
				pUser->m_UserItem[iSlot].sDuration = newItem.sDuration;
				pUser->m_UserItem[iSlot].sBullNum = newItem.sBullNum;
				pUser->m_UserItem[iSlot].tIQ = newItem.tIQ;
				pUser->m_UserItem[iSlot].iItemSerial = newItem.iItemSerial;

				for( j = 0; j < MAGIC_NUM; j++ ) pUser->m_UserItem[iSlot].tMagic[j] = newItem.tMagic[j];

				CBufferEx TempBuf;

				TempBuf.Add(ITEM_LOAD_RESULT);
				TempBuf.Add(SUCCESS);
				TempBuf.Add((BYTE)0x01);

				TempBuf.Add((BYTE)iSlot);
				TempBuf.Add(pUser->m_UserItem[iSlot].sLevel);
				TempBuf.Add(pUser->m_UserItem[iSlot].sSid);
				TempBuf.Add(pUser->m_UserItem[iSlot].sDuration);
				TempBuf.Add(pUser->m_UserItem[iSlot].sBullNum);
				TempBuf.Add(pUser->m_UserItem[iSlot].sCount);
				for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)pUser->m_UserItem[iSlot].tMagic[j]);

				TempBuf.Add((BYTE)pUser->m_UserItem[iSlot].tIQ);

				pUser->Send(TempBuf, TempBuf.GetLength());

				pUser->m_iCurWeight += pItemTable->m_byWeight;
				pUser->GetRecoverySpeed();			// ������ ���Կ� ������ ����� ȸ���ӵ� ��ȯ

				switch( sItemSid )
				{
					case NPC_EVENT_FLOWER:
						pUser->m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL);
//						pUser->m_pCom->Announce(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL);
						break;
					case NPC_EVENT_LOTTO:
						pUser->SendSystemMsg( IDS_EVENT_LOTTO, SYSTEM_NORMAL, TO_ME);
						break;
					default:
						return;
				}

				strMsg.Format("(%04d-%02d-%02d %02d:%02d:%02d) %s - Get %d Item(%d)\r\n",
					time.wYear, 
					time.wMonth, 
					time.wDay, 
					time.wHour, 
					time.wMinute, 
					time.wSecond,	
					pUser->m_strUserID, 
					newItem.sSid,
					upper.i * 10000 + lower.i );

				EnterCriticalSection( &m_CS_EventItemLogFileWrite );
				g_fpEventItem.Write( strMsg, strMsg.GetLength() );
				LeaveCriticalSection( &m_CS_EventItemLogFileWrite );
			}
			else
			{
				// �κ��ֱ⿡ �����ϸ� �ʿ� ����
				GiveItemToMap( pUser->m_pCom, &newItem );

				strMsg.Format("(%04d-%02d-%02d %02d:%02d:%02d) %s - Map %d Item(%d)\r\n",
					time.wYear, 
					time.wMonth, 
					time.wDay, 
					time.wHour, 
					time.wMinute, 
					time.wSecond,	
					pUser->m_strUserID, 
					newItem.sSid,
					upper.i * 10000 + lower.i );

				EnterCriticalSection( &m_CS_EventItemLogFileWrite );
				g_fpEventItem.Write( strMsg, strMsg.GetLength() );
				LeaveCriticalSection( &m_CS_EventItemLogFileWrite );
			}
		}
	}
}

void CNpc::GiveItemToMap(COM *pCom, ItemList *pItem)
{
	CPoint pt = FindNearRandomPointForItem(m_sCurX, m_sCurY);							// ���� �ڱ���ǥ�� ������ 24ĭ
	if(pt.x <= -1 || pt.y <= -1) return;
	if(pt.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pt.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return;

	if( InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_FieldUse, (long)1, (long)0) == (long)0 )
	{
		ItemList* pNewItem = new ItemList;

		memcpy( pNewItem, pItem, sizeof( ItemList ) );

		// �ش� �������� �˸���.			
		//pCom->DelThrowItem();
		pCom->SetThrowItem( pNewItem, pt.x, pt.y, m_ZoneIndex );

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_FieldUse, 0);
	}
}

BOOL CNpc::UpdateEventItemNewRemain(CEventItemNew *pEventItem)
{
	SQLSMALLINT	sRet = -1;
	SQLINTEGER	iRetInd = SQL_NTS;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call update_event_item_new_remain ( %d, %d, ? )}"), 
		pEventItem->m_sIndex, 
		pEventItem->m_sRemain );

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

void CNpc::UserListSort()
{
	int i, j;
	int total = 0;

	ItemUserRightlist temp;

	for(i = 0; i < NPC_HAVE_USER_LIST; i++)
	{
		m_iHaveItemUid[i].uid = -1;
		m_iHaveItemUid[i].nDamage = 0;

		if( m_DamagedUserList[i].nDamage > 0 )
		{
			m_iHaveItemUid[i].uid = m_DamagedUserList[i].iUid;
			m_iHaveItemUid[i].nDamage = m_DamagedUserList[i].nDamage;
		}
	}

	for(i = 2; i < NPC_HAVE_USER_LIST; i++)
	{
		temp.uid = m_iHaveItemUid[i].uid;
		temp.nDamage = m_iHaveItemUid[i].nDamage;
		
		j = i;

		while(m_iHaveItemUid[j-1].nDamage < temp.nDamage)
		{
			m_iHaveItemUid[j].uid = m_iHaveItemUid[j-1].uid;
			m_iHaveItemUid[j].nDamage = m_iHaveItemUid[j-1].nDamage;
			j--;

			if(j <= 0) break;
		}

		m_iHaveItemUid[j].uid = temp.uid;
		m_iHaveItemUid[j].nDamage = temp.nDamage;
	}

	for(i = 0; i < ITEM_USER_RIGHT_NUM; i++)
	{
		if(m_iHaveItemUid[i].nDamage > 0) total += m_iHaveItemUid[i].nDamage;
	}

	if(total <= 0) total = 1;

	for(i = 0; i < ITEM_USER_RIGHT_NUM; i++)
	{
		j = 0;
		j = (int)( (m_iHaveItemUid[i].nDamage * 100)/total );

		if(j > 100) j = 100;
		else if(j <= 0) j = 1;

		m_iHaveItemUid[i].nDamage = (BYTE)j;	
	}
}

DWORD CNpc::GetItemThrowTime()
{
	DWORD dwCurTime = 0;

	SYSTEMTIME SaveTime;
	GetLocalTime(&SaveTime);
	
	WORD wTemp = 0;
	DWORD dwYear = 0;
	DWORD dwMon = 0;
	DWORD dwDay = 0;
	DWORD dwHour = 0;
	DWORD dwMin = 0;
	DWORD dwSecond = 0;
										// 2 Byte ������
	wTemp = SaveTime.wYear << 12;		// ���� 4 Byte
	wTemp = wTemp >> 12;
	dwYear = (DWORD)wTemp; 
	dwYear = dwYear << 26;

	wTemp = SaveTime.wMonth << 12;		// 4 Byte
	wTemp = wTemp >> 12;
	dwMon = (DWORD)wTemp; 
	dwMon = dwMon << 22;

	wTemp = SaveTime.wDay << 11;		// 5 Byte
	wTemp = wTemp >> 11;
	dwDay = (DWORD)wTemp;
	dwDay = dwDay << 17;

	wTemp = SaveTime.wHour << 11;		// 5 Byte
	wTemp = wTemp >> 11;
	dwHour = (DWORD)wTemp;
	dwHour = dwHour << 12;

	wTemp = SaveTime.wMinute << 10;		// 6 Byte
	wTemp = wTemp >> 10;
	dwMin = (DWORD)wTemp;
	dwMin = dwMin << 6;

	wTemp = SaveTime.wSecond << 10;		// 6 Byte
	wTemp = wTemp >> 10;
	dwSecond = (DWORD)wTemp;

	dwCurTime = dwYear^dwMon^dwDay^dwHour^dwMin^dwSecond;

	return dwCurTime;
}

BOOL CNpc::CheckUserForNpc_Live(int x, int y)
{
	return TRUE;

	int min_x, min_y, max_x, max_y;

	min_x = m_sCurX - m_bySearchRange;		if( min_x < 0 ) min_x = 0;
	min_y = m_sCurY - m_bySearchRange;		if( min_y < 0 ) min_y = 0;
	max_x = m_sCurX + m_bySearchRange;
	max_y = m_sCurY + m_bySearchRange;

	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 2;
	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 2;

	int ix, iy;
	int target_uid;

	int tempLevel = 0, oldLevel = 1000;

	for(ix = min_x; ix <= max_x; ix++)
	{
		for(iy = min_y; iy <= max_y; iy++)
		{
			target_uid = m_pOrgMap[ix][iy].m_lUser;

			if( target_uid >= USER_BAND && target_uid < NPC_BAND ) return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	Summon ���� ���� ����� ��ġ������ �ֺ��� ������.
//
void CNpc::SendNpcInfoBySummon(COM *pCom)
{
	int min_x = 0, min_y = 0;
	int max_x = 0, max_y = 0;

	int sx = m_sCurX / SIGHT_SIZE_X;
	int sy = m_sCurY / SIGHT_SIZE_Y;

	int delete_index = 0;
	char delete_send[1024];		::ZeroMemory(delete_send, sizeof(delete_send));
	FillNpcInfo(delete_send, delete_index, INFO_DELETE);

	min_x = (sx-1)*SIGHT_SIZE_X;
	max_x = (sx+2)*SIGHT_SIZE_X;
	min_y = (sy-1)*SIGHT_SIZE_Y;
	max_y = (sy+2)*SIGHT_SIZE_Y;

	SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
}
