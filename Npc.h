// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__600048EF_818F_40E9_AC07_0681F5D27D32__INCLUDED_)
#define AFX_NPC_H__600048EF_818F_40E9_AC07_0681F5D27D32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "COM.h"
#include "Map.h"

#include "Packet.h"
#include "PathFind.h"
#include "Store.h"
#include "GuildFortress.h"
#include "EventItemNew.h"

#define CHANGE_PATH_STEP	2
#define CHANGE_PATH_RANGE	2
#define RANDOM_MOVE_LENGTH	10
#define	NPC_ITEM_BAND		2000

#define NPCTYPE_MONSTER		0
#define NPCTYPE_NPC			1
#define NPCTYPE_DOOR		2
#define NPCTYPE_GUARD		3			// �ٹ����� ���
#define MPCTYPE_GUARD_MOVE	4			// ���ƴٴϴ� ���
#define NPCTYPE_GUILD_NPC	5			// �Ϲ� �ʵ忡�� ������ �ŷ��ϴ� NPC 
#define NPCTYPE_GUILD_GUARD	6			// �Ϲ� �ʵ忡�� �����ֺ��� ��ȣ�ϴ� ���
#define NPCTYPE_GUILD_MARK	7			// �� ����Ͽ콺�� �ִ� ��帶ũ NPC
#define NPCTYPE_GUILD_DOOR	8			// ���������� ���̴� ��

#define GUILD_REPAIR_NPC_BAND	20000	// �ջ�Ǿ� ������ �ʿ��� NPC
#define GUILD_FORTRESS_NPC_BAND	25000	// ���� �����ϴµ� �ʿ��� NPC

#define MAX_MAP_SIZE		10000		//@@@@@@@@@@@@@@@@@@@@@@@@@@@22
#define MAX_PATH_SIZE		100

#define NPC_ITEM_NUM		5

#define NPC_ATTACK_SHOUT	0
#define NPC_SUBTYPE_LONG_MON 1

#define NPC_TRACING_STEP	100

#define NPC_HAVE_USER_LIST	10

#define NPC_QUEST_MOP		 10000		// ����Ʈ�� ��( 1 ~ 10000 )���� ��� 
#define NPC_EVENT_MOP		 31000		// �̺�Ʈ �� ��ȣ
#define NPC_EVENT_GREATE_MOP 30000		// Ư �̺�Ʈ ���� ��ȣ
#define NPC_MAGIC_ITEM		 300	// 1~10000���� ����
#define NPC_RARE_ITEM		 500
#define NPC_EVENT_CHANCE	40			// �̺�Ʈ ���ϰ�� ����Ȯ���� ���� Ȯ���� �÷��ش�. X 40 

/*
#define NPC_EVENT_MOON		620			// ������ ���Ȯ��
#define NPC_EVENT_SONGPEON	920			// ����/���� ���Ȯ��
#define NPC_EVENT_BOX		1420		// �������� ���Ȯ��

#define EVENTITEM_SID_MOON			862	// ������	Sid
#define EVENTITEM_SID_SONGPEON_01	863	// ����		Sid
#define EVENTITEM_SID_SONGPEON_11	864	//
#define EVENTITEM_SID_SONGPEON_31	865	//
#define EVENTITEM_SID_SONGPEON_51	866	//
#define EVENTITEM_SID_SONGPEON_71	867	//
#define EVENTITEM_SID_BOX			868 // �������� Sid
*/

// �߼� �̺�Ʈ�� ũ�������� �̺�Ʈ�� �����Ͽ� �����ϱ� ���� ������ ��
#define NPC_EVENT_MOON		620			// ��Ÿ�������Ȯ�� 5%
#define NPC_EVENT_SONGPEON	1120		// ������� ���Ȯ�� 5%
#define NPC_EVENT_BOX		1420		// ũ�������� �縻 ���Ȯ�� 3%

#define EVENTITEM_SID_MOON			873	// ��Ÿ����	Sid
#define EVENTITEM_SID_SONGPEON_01	872	// �������	Sid
#define EVENTITEM_SID_SONGPEON_11	864	//
#define EVENTITEM_SID_SONGPEON_31	865	//
#define EVENTITEM_SID_SONGPEON_51	866	//
#define EVENTITEM_SID_SONGPEON_71	867	//
#define EVENTITEM_SID_BOX			871 // ũ�������� �縻 Sid

#define NPC_GUILDHOUSE_BAND	10000		// ����Ͽ콺�� ���õ� NPC
//#define NPC_STORE_BAND			
#define FORTRESS_BAND	1000			// ����� ���ϴ� NPC

#define NPC_EVENT_B_ITEM	57			// ����ȸ����B �̺�Ʈ�� ���� �ӽ� ����
#define NPC_EVENT_ITEM		634			// �̺�Ʈ�� ���� �ӽ� ����
#define NPC_EVENT_INIT_STAT	756			// �ؾ� ������
#define NPC_EVENT_FLOWER	773			// ���� ����߸��� �ɹٱ��� ��ȯ��
#define NPC_EVENT_LOTTO		796			// ����ī�� �̺�Ʈ

// �������� ����
										// ������� ������ �ʿ伺 �� ������
#define NPC_NON_REPAIR_STATE	0		// 0 : �ְ� ����		
#define NPC_NEED_REPAIR_STATE	1		// 1 : �ջ�Ǿ� ���� �� �ʿ䰡 �ִ� ����
#define NPC_DEAD_REPAIR_STATE	2		// 2 : ������ ������ ����

#define NPC_NCIRCLE_DEF_STATE	1
#define NPC_NCIRCLE_ATT_STATE	2

struct  DynamicMapList
{
	POINT map[NPC_MAP_SIZE];
};

struct  NpcSkillList
{
	short	sSid;
	BYTE	tLevel;
	BYTE	tOnOff;
};

struct	ExpUserList
{
	TCHAR	strUserID[CHAR_NAME_LENGTH + 1];			// ���̵�(ĳ���� �̸�)
	int		iUid;		
	int		nDamage;								// Ÿ��ġ ��
	BOOL	bIs;
};

struct	TargetUser
{
	int		iUid;
	int		sHP;
	int		sMaxHP;
};

struct ItemUserRightlist
{
	short uid;
	int nDamage;
};

//typedef CTypedPtrArray <CPtrArray, ExpUserList*>		arUserList;
typedef CArray <CPoint, CPoint> RandMoveArray;		// 8���� RandomMove�ϱ� ���� Array

class CNpc  
{
public:
	CNpc();
	virtual ~CNpc();

	BOOL SetLive(COM* pCom);
	BOOL FindEnemy(COM* pCom);
	BOOL GetTargetPath(COM* pCom);
	BOOL IsCloseTarget(COM* pCom, int nRange = 1);
	BOOL IsCloseTarget(USER *pUser, int nRange = 1);
	BOOL IsMovingEnd();
	BOOL IsChangePath(COM* pCom, int nStep = CHANGE_PATH_STEP);
	USER* GetUser(COM* pCom, int uid);
	BOOL ResetPath(COM* pCom);
	BOOL GetTargetPos(COM* pCom, CPoint& pt);
	BOOL StepMove(COM* pCom, int nStep);
	BOOL GetLastPoint(int sx, int sy, int& ex, int& ey);

protected:
	void ClearPathFindData(void);
	void InitSkill();
	
public:	
	void SendNpcInfoBySummon(COM* pCom);
	BOOL CheckUserForNpc_Live(int x, int y);
	DWORD GetItemThrowTime();
	void UserListSort();
	CPoint FindNearRandomPointForItem(int x, int y);
	BOOL UpdateEventItemNewRemain(CEventItemNew* pEventItem);
	void GiveItemToMap(COM* pCom, ItemList* pItem);
	void GiveEventItemNewToUser(USER* pUser);
	void SetISerialToItem(ItemList *pItem, int iEventItemSid);
	int AreaAttack(COM* pCom);
	void GetWideRangeAttack(COM* pCom, int x, int y, int damage, int except_uid);
	int PsiAttack(COM* pCom);
	void TestCode(COM *pCom, USER *pUser);
	void SendFortressInsight(COM *pCom, TCHAR *pBuf, int nLength);
	void SetMapAfterGuildWar();
	void SetMapTypeBeforeGuildWar(COM *pCom);
	void SendFortressNCircleColor(COM *pCom);
	//void SetDoorDamagedInFortressWar(int nDamage, TCHAR *id, int uuid, COM *pCom);
	void SetDoorDamagedInFortressWar(int nDamage, USER *pUser);
	//void SetDamagedInFortressWar(int nDamage, TCHAR *id, int uuid, COM *pCom);
	void SetDamagedInFortressWar(int nDamage, USER *pUser);
	void SetFortressState();
	BOOL UpdateEventItem(int sid);
	int GetEventItemNum(COM *pCom);
	void GiveEventItemToUser(USER *pUser);
	void Send(USER* pUser, TCHAR* pBuf, int nLength);
	int GetCityNumForVirtualRoom(int zone);
	//void SetDamagedInGuildWar(int nDamage, TCHAR *id, int uuid, COM *pCom);
	void SetDamagedInGuildWar(int nDamage, USER *pUser);
	void SetGuildType(COM *pCom);
	void EventNpcInit(int x, int y);
	int IsMagicItem(COM* pCom, ItemList *pItem, int iTable);
	int IsTransformedItem(int sid);
	void ToTargetMove(COM* pCom, USER* pUser);
	void NpcTrace(TCHAR* pMsg);
	CPoint ConvertToServer(int x, int y);
	void Init();
	BOOL IsStepEnd();
	BOOL PathFind(CPoint start, CPoint end);
	void InitTarget(void);
	void SendToRange(COM* pCom, char* temp_send, int index, int min_x, int min_y, int max_x, int max_y);
	CPoint ConvertToClient(int x, int y);
	void FillNpcInfo(char* temp_send, int& index, BYTE flag);
	void SendUserInfoBySightChange(int dir_x, int dir_y, int prex, int prey, COM* pCom);
	int GetFinalDamage(USER* pUser, int type = 1);
	int SetHuFaFinalDamage(USER* pUser, int nDamage);
	void DeleteNPC();
	void SetFireDamage();
	BOOL CheckClassItem(int artable, int armagic);
	void ChangeSpeed(COM *pCom, int delayTime);
	CNpc* GetNpc(int nid);
	void InitUserList();
	BOOL GetBackPoint(int &x, int &y);
	void GetBackDirection(int sx, int sy, int ex, int ey);
	void NpcBack(COM *pCom);
	BOOL IsDamagedUserList(USER *pUser);
	void FindFriend();
	void NpcStrategy(BYTE type);
	void NpcTypeParser();
	void NpcFighting(COM *pCom);
	void NpcTracing(COM *pCom);
	void NpcAttacking(COM *pCom);
	void NpcMoving(COM *pCom);
	void NpcStanding(COM *pCom);
	void NpcLive(COM *pCom);
	void ChangeTarget(USER *pUser, COM* pCom);
	BOOL IsSurround(int targetx, int targety);
	BOOL CheckNpcRegenCount();
	void IsUserInSight(COM *pCom);
	void SendExpToUserList(COM *pCom);
	BOOL SetDamage(int nDamage, int uid, COM *pCom);
	void SetColdDamage(void);
	CPoint FindNearRandomPoint(int xpos, int ypos);
	BOOL IsMovable(int x, int y);
	void GiveNpcHaveItem(COM *pCom,BOOL TreeBaoLv);
	int GetCriticalInitDamage(BOOL* bSuccessSkill);
	int GetNormalInitDamage();
	void SendAttackMiss(COM* pCom, int tuid);
	void IsSkillSuccess(BOOL *bSuccess);
	BYTE GetWeaponClass();
	//yskang 0.3 void SendAttackSuccess(COM *pCom, int tuid, CByteArray &arAction1, CByteArray &arAction2, short sHP, short sMaxHP);
	void SendAttackSuccess(COM *pCom, int tuid,BOOL bIsCritical, short sHP, short sMaxHP);//yskang 0.3
	void GiveItemToMap(COM *pCom, int iItemNum, BOOL bItem, int iEventNum = 0);
	BOOL GetDistance(int xpos, int ypos, int dist);
	void SendInsight(COM* pCom, TCHAR *pBuf, int nLength);
	void SendExactScreen(COM* pCom, TCHAR* pBuf, int nLength);

	void SightRecalc(COM* pCom);

	BOOL IsInRange();
	BOOL RandomMove(COM* pCom);
	int SendDead(COM* pCom, int type = 1,BOOL TreeBaoLv = FALSE);
	void Dead(void);
	BOOL SetDamage(int nDamage);
	int GetDefense();
	int GetAttack();
	int Attack(COM* pCom);

	// Inline Function
	int GetUid(int x, int y );
	BOOL SetUid(int x, int y, int id);

	BOOL SetUidNPC(int x, int y, int id);
	void shouhu_rand(	ItemList *pMapItem);

	struct Target
	{
		int	id;
		int x;
		int y;
		int failCount;
//		int nLen;
//		TCHAR szName[CHAR_NAME_LENGTH + 1];
	};
	Target	m_Target;
	int		m_ItemUserLevel;		// ������ ���� �̻� �����۸� ���������� �����ؾ��ϴ� �����Ƿ���

	int		m_TotalDamage;	// �� ������ �������
	ExpUserList m_DamagedUserList[NPC_HAVE_USER_LIST]; // ������ Ÿ��ġ�� �� ���������� ����Ʈ�� �����Ѵ�.(����ġ �й�)
//	arUserList m_arDamagedUserList;

	BOOL	m_bFirstLive;		// NPC �� ó�� �����Ǵ��� �׾��� ��Ƴ����� �Ǵ�.
	BYTE	m_NpcState;			// NPC�� ���� - ��Ҵ�, �׾���, ���ִ� ���...
	BYTE	m_NpcVirtualState;	// NPC���� -- ������ ���ö��� Ȱ��ȭ (��ȸ������ ��Ƴ�)
	int		m_ZoneIndex;		// NPC �� �����ϰ� �ִ� ���� �ε���

	short	m_sNid;				// NPC (��������)�Ϸù�ȣ

	MapInfo	**m_pOrgMap;		// ���� MapInfo �� ���� ������

	int		m_nInitX;			// ó�� ������ ��ġ X
	int		m_nInitY;			// ó�� ������ ��ġ Y

	int		m_sCurZ;			// Current Zone;
	int		m_sCurX;			// Current X Pos;
	int		m_sCurY;			// Current Y Pos;

	int		m_sOrgZ;			// ���� DB�� Zone
	int		m_sOrgX;			// ���� DB�� X Pos
	int		m_sOrgY;			// ���� DB�� Y Pos

	int		m_presx;			// ���� �þ� X
	int		m_presy;			// ���� �þ� Y

	//
	//	PathFind Info
	//
	CPoint	m_ptDest;
	int		m_min_x;
	int		m_min_y;
	int		m_max_x;
	int		m_max_y;

	long	m_lMapUsed;			// �� �޸𸮸� ��ȣ�ϱ�����(����� : 1, �ƴϸ� : 0)
//	int		**m_pMap;

	int		m_pMap[MAX_MAP_SIZE];// 2���� -> 1���� �迭�� x * sizey + y

	CSize	m_vMapSize;
	CPoint	m_vStartPoint, m_vEndPoint;

//	CPoint m_curStartPoint;
	CPathFind m_vPathFind;

	NODE	*m_pPath;

	int		m_nInitMinX;
	int		m_nInitMinY;
	int		m_nInitMaxX;
	int		m_nInitMaxY;

	CPoint		m_ptCell;					// ���� Cell ��ġ
	int			m_nCellZone;

	// NPC Class, Skill
	NpcSkillList	m_NpcSkill[SKILL_NUM];	// NPC �� ������ �ִ� ��ų
	BYTE			m_tWeaponClass;			// NPC �� ���� Ŭ���� 

	DWORD			m_dwDelayCriticalDamage;	// Delay Critical Damage
	DWORD			m_dwLastAbnormalTime;		// �����̻��� �ɸ��ð�

	BOOL			m_bRandMove;				// 8���� ���� Random Move �ΰ�?
	RandMoveArray	m_arRandMove;				// 8���� ���� Random Move���� ����ϴ� ��ǥ Array

	DWORD			m_dLastFind;				// FindEnemy �Լ��� ������ Term�� �ֱ� ���� ����ϴ� TickCount

	BOOL			m_bCanNormalAT;				// �Ϲ� ������ �ϴ°�?
	BOOL			m_bCanMagicAT;				// ���̿��� ������ �ϴ°�?
	BOOL			m_bCanSPAT;					// Ư�� ������ �ϴ°�?

	BYTE			m_tSPATRange;				// Ư�� ���� ����
	BYTE			m_tSPATAI;					// Ư�� ������ �� �� �ִ� ���϶� ��� ��쿡 Ư�� ������ �Ұ���

	int				m_iNormalATRatio;			// �Ϲ� ���� Ȯ��
	int				m_iSpecialATRatio;			// Ư�� ���� Ȯ��
	int				m_iMagicATRatio;			// ���� ���� Ȯ��


	//----------------------------------------------------------------
	//	�������� ó���ϴ� ����
	//----------------------------------------------------------------
	ItemList	m_NpcHaveItem[NPC_ITEM_NUM];	

	//----------------------------------------------------------------
	//	MONSTER DB �ʿ� �ִ� ������
	//----------------------------------------------------------------
	int		m_sSid;				// MONSTER(NPC) Serial ID
	int		m_sPid;				// MONSTER(NPC) Picture ID
	TCHAR	m_strName[20];		// MONSTER(NPC) Name

	int		m_sSTR;				// ��
	int		m_sDEX;				// ��ø
	int		m_sVOL;				// ����
	int		m_sWIS;				// ����

	int		m_sMaxHP;			// �ִ� HP
	int		m_sHP;				// ���� HP
	int		m_sMaxPP;			// �ִ� PP
	int		m_sPP;				// ���� PP

	BYTE	m_byClass;			// ����迭
	BYTE	m_byClassLevel;		// ����迭 ����
	
	DWORD	m_sExp;				// ����ġ

	int 	m_byAX;				// ���ݰ� X
	int 	m_byAY;				// ���ݰ� Y
	int	    m_byAZ;				// ���ݰ� Z

	int		m_iDefense;		// ��
	BYTE	m_byRange;			// �����Ÿ�

	int		m_sAI;				// �ΰ����� �ε���
	int		m_sAttackDelay;		// ���ݵ�����

	BYTE	m_byVitalC;			// ��ü������ ũ��Ƽ��
	BYTE	m_byWildShot;		// ���� ����
	BYTE	m_byExcitedRate;	// ��� ����
	BYTE	m_byIronSkin;
	BYTE	m_byReAttack;
	BYTE	m_bySubAttack;		// �����̻� �߻�(�ΰ�����)
	BYTE	m_byState;			// ���� (NPC) �����̻�

	BYTE	m_byPsi;			// ���̿��� ����
	BYTE	m_byPsiLevel;		// ���̿��з���

	BYTE	m_bySearchRange;	// �� Ž�� ����
	int		m_sSpeed;			// �̵��ӵ�	

	int		m_sInclination;
	BYTE	m_byColor;
	int		m_sStandTime;
	BYTE	m_tNpcType;			// NPC Type
								// 0 : Normal Monster
								// 1 : NPC
								// 2 : �� �Ա�,�ⱸ NPC
								// 3 : ���

	int		m_sFamilyType;		// �� ���鰣�� �������� ����
	BYTE	m_tItemPer;			// ������ ������ Ȯ��
	BYTE	m_tDnPer;			// �� ������ Ȯ��
	//----------------------------------------------------------------
	//	MONSTER AI�� ���õ� ������
	//----------------------------------------------------------------
	BYTE	m_tNpcLongType;		// ���� �Ÿ� : ���Ÿ�(1), �ٰŸ�(0)
	BYTE	m_tNpcAttType;		// ���� ���� : ����(1), �İ�(0)
	BYTE	m_tNpcGroupType;	// ������ �����ϳ�(1), ���ϳ�?(0)
//	BYTE	m_tNpcTraceType;	// ������ ���󰣴�(1), �þ߿��� �������� �׸�(0)

	//----------------------------------------------------------------
	//	MONSTER_POS DB �ʿ� �ִ� ������
	//----------------------------------------------------------------
	int		m_sMinX;			// NPC ������ ����
	int		m_sMinY;
	int		m_sMaxX;
	int		m_sMaxY;

	int		m_Delay;			// ���� ���·� ���̵Ǳ� ������ �ð�
	DWORD	m_dwLastThreadTime;	// NPC Thread ���� ���������� ������ �ð�

	DWORD	m_dwStepDelay;
	short	m_sClientSpeed;		// �ִ� �ӵ� ������ ����(500 : 100 = 600 : x)

	BYTE	m_byType;
	int		m_sRegenTime;		// NPC ����ð�
	int		m_sEvent;			// �̺�Ʈ ��ȣ
	int		m_sEZone;			// ���� ��ü�� ������ �̺�Ʈ��

	int		m_sGuild;			// NPCTYPE_GUILD_GUARD��� �ش� �ʵ���� �ε���
								// NPCTYPE_GUILD_NPC���...

	short	m_sDimension;		// �̺�Ʈ�� �Ǵ� �׿� �ʿ��� 2�� �̻� �ڸ��� �����ϴ� NPC�� ����

	int		m_sHaveItem;		// ���Ͱ� ���� ��� �� ������ �ε���

	long	m_lEventNpc;

	//----------------------------------------------------------------
	//	�����̻����
	//----------------------------------------------------------------
	BYTE	m_tAbnormalKind;
	DWORD	m_dwAbnormalTime;
	
	//----------------------------------------------------------------
	//	������� ����
	//----------------------------------------------------------------
	BYTE	m_tNCircle;			// 0 : ó������
								// 1 : ����� ����
								// 2 : ������ ����

	BYTE	m_tRepairDamaged;	// ������� ������ �ʿ伺 �� ������
								// 0 : �ְ� ����
								// 1 : �ջ�Ǿ� ���� �� �ʿ䰡 �ִ� ����
								// 2 : ������ ������ ����

	CGuildFortress *m_pGuardFortress;
	CStore* m_pGuardStore;		// ����̶�� �ش� ������ ������ �ִ�.
	BYTE	m_tGuildWar;		// 0 : ����� �غ�ܰ�
								// 1 : �������
								// 2 : �Ϲ� �ܰ�

	ItemUserRightlist		m_iHaveItemUid[NPC_HAVE_USER_LIST];
	long					m_lFortressState;
	long					m_lDamage;	// �� �������� �ѹ��� �ϳ��� �������� ����ȭ �Ѵ�. by Youn Gyu 02-10-08

	BOOL	m_bSummon;
	int		m_sSummonOrgZ;
	int		m_sSummonOrgX;
	int		m_sSummonOrgY;
	int		m_SummonZoneIndex;

	BOOL	m_bSummonDead;
	LONG	m_lNowSummoning;

	int		m_TableZoneIndex;
	int		m_sTableOrgZ;
	int		m_sTableOrgX;
	int		m_sTableOrgY;

	LONG	m_lKillUid;					// ���� ���������� ���� ������ uid
	short	m_sQuestSay;				// ����Ʈ�� ������ �ִ� �̺�Ʈ ���ΰ�� != 0 (1 ~~ 10000)
	int Dead_User_level;
	int NpcDrop; //���ﱬ����Ʒ����
};

#endif // !defined(AFX_NPC_H__600048EF_818F_40E9_AC07_0681F5D27D32__INCLUDED_)
