#ifndef _PACKET_H_
#define _PACKET_H_

#define ACCOUNT_LENGTH		12
#define PASSWORD_LENGTH		12
#define CHAR_NAME_LENGTH	30
#define	CHAR_NAME_LENGTH_CLIENT 16
#define GUILD_MARK_SIZE		384
#define GUILD_MAP_COUNT		40 //  40°³
#define TEL_NAME_LENGTH		20
#define SHOP_NAME_LENGTH	18

#define CLASS_NUM			4
#define NEW_CLS_NUM			5				//ÐÂÔö¼ÓÖÙ²ÃÊ¦

#define SKILL_NUM			5
#define SKILL_LEVEL			41

//#define SKILL_NAME_LENGTH	14

#define EQUIP_ITEM_NUM		10
#define	INVENTORY_NUM		24
//#define QUICK_ITEM		5
#define EBODY_NUM			4
#define GUARDIAN_NUM		1
#define TOTAL_SKILL_NUM			(CLASS_NUM * SKILL_NUM)
//#define TOTAL_CLASS_SKILL_NUM	(SKILL_NUM * SKILL_LEVEL)

#define TOTAL_PSI_NUM		84

#define MAX_ITEM_NUM		255			// ÇÏ³ªÀÇ ½½·Ô´ç ¼ÒÀ¯ÇÒ¼öÀÖ´Â ÃÖ´ë°ª
#define BANK_MAX_ITEM		1000		// ÇÑ½½·Ô´ç ¼ÒÀ¯ÇÒ¼ö ÀÖ´Â ¼ö
#define ACCOUNT_BANK_MAX_ITEM	32767	// °èÁ¤ ÀºÇà¿¡ ÇÑ½½·Ô´ç ¼ÒÀ¯ÇÒ¼ö ÀÖ´Â ¼ö

#define TOTAL_BANK_ITEM_NUM 48
#define TOTAL_ACCOUNT_BANK_ITEM_NUM 150

#define TOTAL_PERSONALSHOP_ITEM_NUM 7

#define TOTAL_INVEN_MAX		((EQUIP_ITEM_NUM + INVENTORY_NUM))
#define TOTAL_ITEM_NUM		((EQUIP_ITEM_NUM + INVENTORY_NUM + EBODY_NUM + 1 + 1))
//#define TOTAL_ITEM_SLOT_NUM ((EQUIP_ITEM_NUM + INVENTORY_NUM + QUICK_ITEM))
//#define TOTAL_USER_ITEM_NUM	((TOTAL_ITEM_NUM + EBODY_ITEM))

#define _SKILL_SIZE			((sizeof(short) + sizeof(BYTE)))
#define _ITEM_SIZE			((sizeof(short) * 5) + (sizeof(BYTE) * 11) + (sizeof(BYTE)))
#define _PSI_SIZE			((sizeof(short) + sizeof(BYTE)))
#define _SKILLINFO_SIZE		((sizeof(BYTE) + sizeof(DWORD)))

#define USER_SKILL_LEN		(_SKILL_SIZE * TOTAL_SKILL_NUM)
#define USER_ITEM_LEN		(_ITEM_SIZE * TOTAL_ITEM_NUM)

#define USER_SKILLINFO_LEN	(_SKILLINFO_SIZE * TOTAL_SKILL_NUM)

#define EQUIP_SLOT			0
#define INVENTORY_SLOT		1
//#define QUICK_SLOT			2
#define TRADE_SLOT			3
#define BANK_SLOT			4
#define GUILD_SLOT			5
#define ACCOUNT_BANK_SLOT	6

#define BRAWL				0		// Character Making ½Ã »ç¿ëµÇ´Â Å¬·¡½º ¼ýÀÚ
#define STAFF				1
#define	EDGED				2
#define FIREARMS			3
#define JUDGE				4		//ÖÙ²Ã
#define NOCLASS				5
#define ALL_JOBS			31		//ËùÓÐÖ°Òµ´úÂë

#define JUDGE_ITEM			16		// ÖÙÔØ
#define BRAWL_ITEM			8		// Item Table ¿¡ Á¤ÀÇµÈ Å¬·¡½º ¼ýÀÚ
#define STAFF_ITEM			4
#define	EDGED_ITEM			2
#define FIREARMS_ITEM		1

#define MAX_MOVE_TIME		100

#define ATTACK_UPGRADE_BAND		16	
#define ATTACK_UPGRADE_PSI_BAND	4	
#define DEFENSE_UPGRADE_BAND	4	

const BYTE SUCCESS				=	1;
const BYTE FAIL					=	0;
const BYTE SUCCESS_ZONE_CHANGE	=	2;

const BYTE ERR_1				=	1;
const BYTE ERR_2				=	2;
const BYTE ERR_3				=	3;
const BYTE ERR_4				=	4;
const BYTE ERR_5				=	5;
const BYTE ERR_6				=	6;
const BYTE ERR_7				=	7;
const BYTE ERR_8				=	8;
const BYTE ERR_9				=	9;
const BYTE ERR_10				=	10;
const BYTE ERR_11				=	11;
const BYTE ERR_12				=	12;
const BYTE UNKNOWN_ERR			=	255;

const BYTE ERR_SHORT_ATTACK_DELAY	= 0;
const BYTE ERR_TOO_LONG_DISTANCE	= 1;
const BYTE ERR_BROKEN				= 2;
const BYTE ERR_EMPTY_BULLET			= 3;
const BYTE ERR_EMPTY_ENERGE			= 4;
const BYTE ERR_EMPTY_SP				= 5;

const BYTE INFO_MODIFY			=	1;
const BYTE INFO_DELETE			= 	2;
const BYTE INFO_TOWNPOTAL		=   3;
const BYTE INFO_TOWNPOTAL_DELETE=	4;

const BYTE ITEM_INFO_MODIFY		=	1;
const BYTE ITEM_INFO_DELETE		=	2;
const BYTE ITEM_INFO_PICKUP		=	3;

const BYTE NORMAL_CHAT			=	1;
const BYTE WHISPER_CHAT			=	2;
const BYTE SHOUT_CHAT			=	3;
const BYTE GUILD_CHAT			=	4;
const BYTE BUDDY_CHAT			=	5;
const BYTE BUDDY_INVITE			=	6;
const BYTE BUDDY_OFF			=	7;
const BYTE MOP_CHAT				=	10;
const BYTE WHISPER_OK			=	11;
const BYTE WHISPER_DENY			=	12;

const BYTE CALL_CHAT			=	15;		// Æ¯Á¤À¯Àú ¼ÒÈ¯
const BYTE REMOTE_CHAT			=	16;		// Æ¯Á¤À¯Àú Æ¯Á¤ÁÂÇ¥·Î °­Á¦ ÀÌµ¿
const BYTE MOP_CALL_CHAT		=	17;		// Æ¯Á¤¸÷ Æ¯Á¤ÁÂÇ¥·Î °­Á¦ ÀÌµ¿

const BYTE GUILD_INVITE			=	18;		// ±æµå °¡ÀÔ
const BYTE GUILD_DISPERSE		=	19;		// ±æµå ÇØ»ê
const BYTE GUILD_OFF			=	20;		// ±æµå Å»Åð
const BYTE GUILD_KICK_OUT		=	21;		// ±æ¸¶°¡ ±æµå¿ø Ãß¹æ

const BYTE ITEM_MODE_CHAT		=	22;		// ¾ÆÀÌÅÛ ¹Þ±â ½Â³«, °ÅÀý¸¦ Ç¥½Ã

const BYTE OP_CLOAK_ON			=	23;		// ¿î¿µÀÚ Åõ¸í¸ðµå·Î
const BYTE OP_CLOAK_OFF			=	24;		// ¿î¿µÀÚ Åõ¸í¸ðµå ÇØÁ¦·Î...
const BYTE ALL_CHAT				=	25;		// ÀüÃ¼ Á¢¼ÓÀÚ¿¡°Ô ¸Þ¼¼Áö¸¦ º¸³½´Ù.
const BYTE GUILD_FIELD_WAR		=   26;		// »ó´ë ±æµå¸¦ »ó´ë·Î ÇÊµåÀüÀ» ½ÅÃ»ÇÑ´Ù.
const BYTE GUILD_SURRENDER		=   27;		// »ó´ë ±æµå¸¦ »ó´ë·Î Ç×º¹ÇÑ´Ù.
const BYTE PK_COUNT_CHAT		=	28;
const BYTE COPY_TELEPORT		=	29;		// ÁöÁ¤ÇÑ »ó´ë¹æ°ú °°ÀÌ ÀÌµ¿ÇÑ´Ù.
const BYTE GUILD_USER_CHECK		=	30;		// ÀüÃ¼ ±æµå¿ø ¸ñ·Ï¸¦ º¸¿©ÁØ´Ù.
const BYTE GUILD_WAR_APPLY_LIST =	31;		// ±æÀüÀ» ½ÅÃ»ÇÑ ÀüÃ¼ ±æµå ¸ñ·ÏÀ» º¸¿©ÁØ´Ù.
const BYTE GUILD_FORTRESS_DOOR	=	32;		// Æ÷Æ®¸®½º ¼º¹®À» ¿­°í ´Ý´Â´Ù.
const BYTE GUILD_DEPUTE_POWER	=	33;		// ±æ¸¶ ±ÇÇÑÀ» ºÎ±æ¸¶¿¡°Ô À§ÀÓ
const BYTE GUILD_STOP_POWER		=	34;		// ºÎ±æ¸¶ ±ÇÇÑÀ» Á¤Áö
const BYTE GUILD_USER_INFO_CALL =	35;		//yskang 0.2
const BYTE GUILD_CALL_CHAT		=	36;		//yskang 0.2 Æ÷Æ®¸®½º¸¦ °¡Áö°í ÀÖ´Â ±æµåÂ¯ÀÌ Æ¯Á¤ À¯Àú¸¦ ¼ÒÈ¯ÇÑ´Ù.

const BYTE SYSTEM_SPECIAL		=	0;		// Æ¯º° Á¤º¸
const BYTE SYSTEM_NORMAL		=	1;		// ÀÏ¹Ý Á¤º¸
const BYTE SYSTEM_ERROR			=	2;		// ¿¡·¯Á¤º¸
const BYTE SYSTEM_NPC			=	3;		// NPC ´ëÈ­
const BYTE SYSTEM_ANNOUNCE		=	4;		// ¿î¿µÀÚ ¸Þ¼¼Áö Á¤º¸
const BYTE SYSTEM_DISCONNECT	=	5;		// ½Ã°£ »ç¿ëÀÚ Á¾·áÁ¤º¸	
const BYTE SYSTEM_CESHIA        =	6;

// Hit Mark List (Skill List)
const BYTE HIT_NONE				=	0;
const BYTE HIT_DAMAGE_C			=	1;
const BYTE HIT_VITAL_C			=	2;
const BYTE HIT_DELAY_C			=	3;
const BYTE HIT_CONCENTRATE		=	4;
const BYTE HIT_PSY_ACCLIMATE	=	5;
const BYTE HIT_FAST_CAST		=	6;
const BYTE HIT_WEAPON_ACCLIMATE	=	7;
const BYTE HIT_FAST_ATTACK		=	8;
const BYTE HIT_OUT_RANGE		=	9;
const BYTE HIT_STRAFE			=	10;
const BYTE HIT_BROKEN			=	11;
const BYTE HIT_FIXED			=	12;
const BYTE HIT_OVERHIT			=	13;
const BYTE HIT_FIRE				=	14;
const BYTE HIT_ICE				=	15;
const BYTE HIT_LIGHT			=	16;
const BYTE HIT_NUMB				=	17;
const BYTE HIT_POISON			=	18;
const BYTE HIT_C_GUARD			=	19;

const BYTE PSI_TELEPORT			=	7;

const BYTE BBS_NOT_OWNER		=	2;
const BYTE BBS_CONTENTS_DELETE	=	3;

// »óÅÂÀÌ»ó Á¤º¸
//#define _ABNORMAL_DB			5

const BYTE ABNORMAL_TIME			=	0;
const BYTE PSI_ONE_TIME				=	1;
const BYTE PSI_TWO_TIME				=	2;
const BYTE PSI_THREE_TIME			=	3;
const BYTE TRANS					=   4;

const BYTE ABNORMAL_BYTE_NONE		=	0;
const BYTE ABNORMAL_BYTE_POISON		=	1;
const BYTE ABNORMAL_BYTE_CONFUSION	=	2;
const BYTE ABNORMAL_BYTE_PARALYSIS	=	3;
const BYTE ABNORMAL_BYTE_BLIND		=	4;
const BYTE ABNORMAL_BYTE_LIGHT		=	5;
const BYTE ABNORMAL_BYTE_FIRE		=	6;
const BYTE ABNORMAL_BYTE_COLD		=	7;

const DWORD ABNORMAL_NONE			=	0;
const DWORD ABNORMAL_POISON			=	1;
const DWORD ABNORMAL_CONFUSION		=	1 << 2;
const DWORD ABNORMAL_PARALYSIS		=	1 << 3;
const DWORD ABNORMAL_BLIND			=	1 << 4;
const DWORD ABNORMAL_LIGHT			=	1 << 5;
const DWORD ABNORMAL_FIRE			=	1 << 6;
const DWORD ABNORMAL_COLD			=	1 << 7;
const DWORD ABNORMAL_HASTE			=	1 << 8;
const DWORD ABNORMAL_SHIELD			=	1 << 9;
const DWORD ABNORMAL_HIDE			=	1 << 10;
const DWORD ABNORMAL_DEX_UP			=	1 << 11;
const DWORD ABNORMAL_MAX_HP_UP		=	1 << 12;
const DWORD ABNORMAL_ADAMANTINE		=	1 << 13;
const DWORD ABNORMAL_MIGHTYWEAPON	=	1 << 14;
const DWORD ABNORMAL_BERSERKER		=	1 << 15;
const DWORD ABNORMAL_HIEXP			=	1 << 16;
const DWORD ABNORMAL_MAGICFIND		=	1 << 17;
const DWORD ABNORMAL_NOCHAT			=	1 << 18;

const DWORD ABNORMAL_FASTRUN		=	1 << 19;
const DWORD ABNORMAL_MINDSHOCK		=	1 << 20;
const DWORD ABNORMAL_MINDGUARD		=	1 << 21;
const DWORD ABNORMAL_PSISHIELD		=	1 << 22;
const DWORD ABNORMAL_PIERCING_SHIELD=	1 << 23;
const DWORD ABNORMAL_PAOBUTING		=	1 << 27;
const DWORD ABNORMAL_FUDAI			=	1 << 26; // ¸£´üÍ¼±ê
const DWORD ABNORMAL_JIANYU			=	1 << 28;//×øÀÎ
const DWORD ABNORMAL_BIANSHEN		=	1 << 29;//»ÃÏë¾§Ê¯
const DWORD ABNORMAL_BIGSHIELD			=	1 << 30;
const DWORD ABNORMAL_VIP	        =	2; //VIP
const DWORD OPERATION_MODE			=	1 << 31;	// ¿î¿µÀÚ Åõ¸í ¿É¼Ç


const DWORD STATE_1 = 1 << 0; //¿ÉÒÔÊ¹ÓÃ»úÐµ×´Ì¬
const DWORD STATE_0_1 = 1 << 1;
const DWORD STATE_2 = 1 << 2; 
const DWORD STATE_3 = 1 << 3; 
const DWORD STATE_4 = 1 << 4; // hp up
const DWORD STATE_5 = 1 << 5; //ÎäÆ÷¼Ó1
const DWORD STATE_6 = 1 << 6; //ÂÌ³á°ò
const DWORD STATE_7 = 1 << 7; //ÌìÊ¹
const DWORD STATE_8 = 1 << 8; 
const DWORD STATE_9 = 1 << 9; // hp down
const DWORD STATE_10 = 1 << 10; //
const DWORD STATE_11 = 1 << 11; //Í¼Æ¬sp
const DWORD STATE_12 = 1 << 12; //pp
const DWORD STATE_13 = 1 << 13; //Í¶Éí
const DWORD STATE_14 = 1 << 14; //¸ú»ÃÏë¾§ºÜÏó
const DWORD STATE_15 = 1 << 15; //°×¼¶Íâ±í
const DWORD STATE_16 = 1 << 16; //ÎÞ
const DWORD STATE_17 = 1 << 17; //ÎÞ
const DWORD STATE_18 = 1 << 18; //»ú¼×¿ñ±©
const DWORD STATE_19 = 1 << 19; //ÎÞ
const DWORD STATE_20 = 1 << 20; //ÎÞ
const DWORD STATE_21 = 1 << 21; //exp 2
const DWORD STATE_22 = 1 << 22; //magic up
const DWORD STATE_23 = 1 << 23; //Í¼Æ¬xp 3
const DWORD STATE_24 = 1 << 24;//Àñ·þ
const DWORD STATE_25 = 1 << 25; //Í¼Æ¬ÑÛ¾¦
const DWORD STATE_26 = 1 << 26; //ÎäÆ÷+3
const DWORD STATE_27 = 1 << 27; //Í¼Æ¬Ò©Æ¿MAX
const DWORD STATE_28 = 1 << 28;//·À¾Ö+1
const DWORD STATE_29 = 1 << 29;//ÎäÆ÷+2
const DWORD STATE_30 = 1 << 30;//Í¼Æ¬ÆÆËéµÄ¶ÜÅÆ
const DWORD STATE_31 = 1 << 31;//ÉÁ¹â

const DWORD ABNORMAL_CLEAR_STATUS	= (0xFFFFFFFF ^ (ABNORMAL_POISON | ABNORMAL_CONFUSION | ABNORMAL_PARALYSIS | ABNORMAL_BLIND | ABNORMAL_LIGHT | ABNORMAL_FIRE | ABNORMAL_COLD));
const DWORD ABNORMAL_CLEAR_STATUS1	= (0xFFFFFFFF ^ (STATE_1 | STATE_5 | STATE_6 | STATE_7 | STATE_13 | STATE_18 | STATE_21 | STATE_22 | STATE_23 | STATE_24 | STATE_28 | STATE_29 | STATE_30 | STATE_31));
const DWORD	ABNORMAL_CLEAR_PSI_ONE	= (0xFFFFFFFF ^ (ABNORMAL_HASTE | ABNORMAL_SHIELD | ABNORMAL_HIDE | ABNORMAL_DEX_UP | ABNORMAL_MAX_HP_UP | ABNORMAL_FASTRUN | ABNORMAL_PSISHIELD | ABNORMAL_PIERCING_SHIELD | ABNORMAL_MINDSHOCK));
const DWORD ABNORMAL_CLEAR_PSI_TWO	= (0xFFFFFFFF ^ (ABNORMAL_ADAMANTINE | ABNORMAL_MIGHTYWEAPON | ABNORMAL_BERSERKER));
const DWORD ABNORMAL_CLEAR_PSI_THREE= (0xFFFFFFFF ^ (ABNORMAL_MINDGUARD));

const short TYPE_MONEY_SID		=	10000;	// ¾ÆÀÌÅÛ °ú µ·À» ±¸ºÐÇÏ±âÀ§ÇØ sid¸¦ Å©°Ô Àâ¾Ò´Ù.

//ÐÂÔöÖ°ÒµÄ§·¨×´Ì¬
const DWORD JUDGE_PSI_NONE			=   0;

//0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,
const DWORD JUDGE_ABNORMAL_18 = 1<<18;					//¹¥»÷Á¦ÌáÉýÖÇÁ¦
//0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00
const DWORD JUDGE_ABNORMAL_20 = 1<<20;					//ÎþÉü»¤·¨

const DWORD New_Status_1	= 1<<0;//»ÆÉ«µÄÈ­¼ÓËÙ
const DWORD New_Status_2	= 1<<1;//ÀàËÆÒÆ¶¯ËÙ¶È¼Ó¿éÍ¼Æ¬
const DWORD New_Status_3	= 1<<2;//°×Ê®×ÖÍ¼Æ¬ÀàËÆÒ½Ôº±êÖ¾
const DWORD New_Status_4	= 1<<3;//Ê®×ÖÐÍ·½¿é
const DWORD New_Status_5	= 1<<4;//PCEXP20%ÉÏÉý
const DWORD New_Status_6	= 1<<5;//PCEXP30%ÉÏÉý
const DWORD New_Status_7	= 1<<6;//PCEXP50%ÉÏÉý
const DWORD New_Status_8	= 1<<7;	//°×ÈËÍ·
const DWORD New_Status_9	= 1<<8;	//°×ÐÜÍ·
const DWORD New_Status_10	= 1<<9;	//°×µ¶
const DWORD New_Status_11	= 1<<10;//°××Óµ¯
const DWORD New_Status_12	= 1<<11;//»Æ¸ñ¶·135
const DWORD New_Status_13	= 1<<12;//»Æ·¨Ê¦135
const DWORD New_Status_14	= 1<<13;//»Æ½£Ê¦135
const DWORD New_Status_15	= 1<<14;//»ÆÇ¹ÊÖ135
const DWORD New_Status_16	= 1<<15;//ÎÞ
const DWORD New_Status_17	= 1<<16;//Ðý·çÍ¼Æ¬
const DWORD New_Status_21	= 1<<17;//Ãô½ÝÌáÉý
const DWORD New_Status_19	= 1<<19;//Áé»êËøÁ´
const DWORD New_Status_22	= 1<<21;//Íæ¼ÒÃû×ÖÉÏ´øNEW Ö»ÓÐ×Ô¼ºÄÜ¿´µ½
const DWORD New_Status_23	= 1<<22;//Ãû×ÖÇ°Ãæ´øC.B.
const DWORD New_Status_24	= 1<<23;//µç»°SD
const DWORD New_Status_25	= 1<<24;//µç»°A
const DWORD New_Status_26	= 1<<25;//µç»°D
const DWORD New_Status_27	= 1<<26;//µç»°SD
const DWORD New_Status_28	= 1<<27;//ÎÞ
const DWORD New_Status_29	= 1<<28;//ÎÞ
const DWORD New_Status_30	= 1<<29;//ÎÞ
const DWORD New_Status_31	= 1<<30;//ÎÞ
const DWORD New_Status_32	= 1<<31;//ÎÞ

/*
BYTE	NORMAL_ITEM		= 0;
BYTE	REMODEL_ITEM	= 1;
BYTE	MAGIC_ITEM		= 2;
BYTE	RARE_ITEM		= 3;
BYTE	UNIQUE_ITEM		= 4;
BYTE	SET_ITEM		= 5;
BYTE	UPGRADE_ITEM	= 6;
*/

const BYTE HP_POTION			= 1;
const BYTE PP_POTION			= 2;
const BYTE SP_POTION			= 3;
const BYTE ALL_POTION			= 4;
const BYTE SALUTE				= 5;
const BYTE EXP_POTION			= 6;
const BYTE MAGIC_POTION			= 7;
const BYTE KILLREMOVE_POTION	= 8;
const BYTE DEAD_POTION			= 9;
const BYTE XMAS_SALUTE			= 10;
const BYTE YEAR_SALUTE			= 11;
const BYTE YEAR_SALUTE1			= 12;
const BYTE YEAR_SALUTE2			= 13;//×Ô¶¯ÊÕÇ®°ü 
const BYTE YEAR_SALUTE3			= 14;//¸£´ü
const BYTE BIAN_SHEN			= 15;//ÎÒ¿¿
const BYTE TEST_21				= 21;//ÎÞ

//#################################################################
//	Zone Server
//#################################################################
const BYTE	SERVER_LOGIN_REQ			= 1;

const BYTE	SERVER_LOGIN_RESULT			= 2;

const BYTE	SERVER_USER_LOGIN_REQ		= 3;

const BYTE	SERVER_USER_LOGIN_RESULT	= 4;

const BYTE	SERVER_DOUBLE_ACCOUNT_CLOSE = 5;

const BYTE	SERVER_DOUBLE_ID_CLOSE		= 6;

const BYTE	SERVER_USER_LOGOUT_REQ		= 7;

const BYTE	SERVER_USER_ZONELOGIN_REQ	= 8;

const BYTE	SERVER_USER_ZONELOGIN_RESULT= 9;

const BYTE	SERVER_USER_WHISPER_ONOFF	= 10;

const BYTE	SERVER_USER_WHISPER_REQ		= 11;

const BYTE	SERVER_USER_WHISPER_RESULT	= 12;

const BYTE	SERVER_USER_GUILD_CHAT		= 13;//×Ô¶¯ÊÕÇ®°ü 

const BYTE	SERVER_GUILD_NEW			= 14;//¸£´ü

const BYTE	SERVER_GUILD_DISPERSE		= 15;//±äÉíÐ§¹û

const BYTE	SERVER_GUILD_INVITE			= 16;//±äÉíÐ§¹û

const BYTE	SERVER_GUILD_OFF			= 17;//ºìÉ«¶¾ÆøÐ§¹û

const BYTE	SERVER_GUILD_MARK			= 18;//ÂÌÉ«ÉÁ¹âÐ§¹û

const BYTE	SERVER_USER_RESTART			= 19;//¿ñ±©Ð§¹û

const BYTE	SERVER_USER_ZONE_LOGOUT		= 20;//µ­À¶É«³ÔÒ©Ð§¹û

const BYTE	SERVER_ALIVE				= 21;//ÎÞ

const BYTE	SERVER_ANNOUNCE				= 22;//ÂÌÉ«³ÔÒ©Ð§¹û

const BYTE	SERVER_ARK_WINNER			= 23;//Ê¹ÓÃÊØ»¤ÌìÊ¹Ð§¹û

const BYTE  SERVER_GUILD_SUBMASTER_ON	= 24;//speed¼ÓËÙÐ§¹û

const BYTE  SERVER_GUILD_SUBMASTER_OFF	= 25;//attack¹¥»÷Ð§¹û

const BYTE	ZONE_LOGIN_REQ				= 233;
/*
	(C -> S)	// ´Ù¸¥ ¼­¹ö¿¡¼­ ´ã´çÇÏ´Â Á¸À¸·Î Á¸Ã¼ÀÎÁö¸¦ ÇÒ¶§  (¾ÏÈ£È­ ¾ÈÇÔ)

	VarChar		id[ACCOUNT_LENGTH];				// Account ID
	VarChar		pass[PASSWORD_LENGTH];			// Account Password
	VarChar		szCharName[CHAR_NAME_LENGTH];	// CharName

	short		myserver;
*/

const BYTE	ZONE_LOGIN_RESULT			= 234;
/*
	(C <- S)	// Game Login Result  (¾ÏÈ£È­ ¾ÈÇÔ)

	BYTE		result;			// 01	: Login Success
								// else	: Login Fail

	// ½ÇÆÐÀÎ °æ¿ì ------		// 01	: Invaild ID
								// 02	: DB¿¡ ¾ø´Â ID
								// 03	: Invalid Zone Number
								// 04	: Invalid Position
								// 05	: ÇöÀç ID »ç¿ëÁß
								// ...
								// 255	: ¾Ë ¼ö ¾ø´Â ¿¡·¯

	short		CurrZone;		// ÇöÀç Zone Number
	int			uid;			// User Index
	short		PosX;			// Map ÀÇ X ÁÂÇ¥
	short		PosY;			// Map ÀÇ Y ÁÂÇ¥

	BYTE		tDir;			// ÇöÀç º¸°íÀÖ´Â ¹æÇâ

	BYTE		tTime;			// ½Ã°£(0~24)
	BYTE		tMinute;		// ºÐ  (0~60)


    Varchar		public_key;		// int64
	DWORD		dwSerial;		// ½Ã¸®¾ó ½ÃÀÛ ¹øÈ£
*/


// CHAR INFO SERVER
const BYTE CIS_DISCONNECT				= 235;
/*
*/

//#################################################################
//	¼­¹ö Å×½ºÆ® ÄÚµåÀÓ (2001-11-15)
//#################################################################
const BYTE CHECK_ALIVE_REQ		= 255;
/*
	(C -> S)
*/
const BYTE CHECK_ALIVE_RESULT	= 255;
/*
	(C <- S)
*/

const BYTE CLIENT_TICK			= 254;
/*
	(C -> S) // Speed Hack À» ¸·±âÀ§ÇØ 5ÃÊ¸¶´Ù Å¬¶óÀÌ¾ðÆ®¿¡¼­ ¼­¹ö·Î º¸³»´Â ÆÐÅ¶
*/

//const BYTE SERVER_TICK_RESULT	= 6;
/*
	(C <- S)
	DWORD	dwTick;				// ServerÀÇ ÇöÀç Tick.
*/

const BYTE DB_USER_COUNT			= 253;
/*
	(S <- S)						//
*/

const BYTE DB_GUILD_WHEREHOUSE_EVENT_REQ = 252;
/*
	(S <- S)						//
*/

const BYTE DB_GUILD_ITEM_MOVE_REQ	= 251;
/*
	(S <- S)						//
*/

const BYTE ANNOUNCE_NOTICE			= 250;
/*
	(C -> S)						// ÀüÃ¼¼­¹ö¿¡ ÀÏ°ýÀûÀÎ °øÁö¸¦À§ÇØ
	String		strMsg;
*/

const BYTE ENCRYPTION_PKT			= 249;
/*									// ¾ÏÈ£È­¸¦ ÀÇ¹Ì
*/

const BYTE DB_USER_DEAD				= 248;
/*									// À¯Àú°¡ Á×¾úÀ»¶§ ·Î±×¸¦ ³²±è
*/

//#################################################################
//	Information		(1 ~ 9)
//#################################################################

const BYTE USER_INFO			=	1;
/*
	(C <- S)		// Send User Info

	BYTE		Mode;			// 01(INFO_MODIFY)	: À¯ÀúÁ¤º¸ º¯°æ
								// 02(INFO_DELETE)	: À¯ÀúÁ¤º¸ »èÁ¦
								// 03(INFO_TOWNPOTAL)		 : À¯Àú°¡ Å¸¿îÆ÷Å» »ç¿ë -> ¸¶À» ÁøÀÔ
								// 04(INFO_TOWNPOTAL_DELETE) : À¯Àú°¡ Å¸¿îÆ÷Å»·Î ÀÌµ¿, À¯Àú Á¤º¸ »èÁ¦

	int			uid;			// User index

	// Mode °¡ INFO_MODIFY ÀÎ °æ¿ì¸¸ ¾Æ·¡ÀÇ µ¥ÀÌÅÍ Àü¼Û
	VarChar		szName;			// User Name
	short		nPosX;			// X Position
	short		nPosY;			// Y Position

	DWORD		dwSkin;			// ÇÇºÎ»ö(COLORREF)
	DWORD		dwHair;			// ¸Ó¸®»ö(COLORREF)
	BYTE		byGender;		// ³², ¿©

	char		szFace[10];		// ¾ó±¼¸ð¾ç
								// 1 : ¸Ó¸®
								// 2 : ¾ó±¼
								// 3 : ´«
								// 4 : ÄÚ
								// 5 : ÀÔ
								// 6 : ¾Ç¼¼»ç¸® 1
								// 7 : ¾Ç¼¼»ç¸® 2
								// 8~10 : ¿¹ºñ

	short		sEquipItem[10];	// 0 = Çï¸ä
								// 1 = °©¿Ê
								// 2 = ¸Ó¸® ¾×¼¼¼­¸®
								// 3 = ¸ö ¾×¼¼¼­¸®
								// 4 = ¿À¸¥ÂÊ ¼Õ ¹«±â
								// 5 = ¿ÞÂÊ   ¼Õ ¹«±â
								// 6 = ¿À¸¥ÂÊ ¾Ç¼¼»ç¸®
								// 7 = ¿ÞÂÊ   ¾Ç¼¼»ç¸®
								// 8 = ¹ÙÁö
								// 9 = ½Å¹ß
								// ¾ÆÀÌÅÛÀ» ÀåÂøÇÏÁö ¾ÊÀº ½½·ÔÀº id == -1

	short		sHP;
	short		sMaxHP;

	BYTE		tDir;			// º¸°íÀÖ´Â ¹æÇâ

	DWORD		dwAbnormal		// »óÅÂÀÌ»óÁ¤º¸(Damaged)

								// const DWORD ABNORMAL_NONE			=	0;
								// const DWORD ABNORMAL_POISON			=	1;
								// const DWORD ABNORMAL_CONFUSION		=	1 << 2;
								// const DWORD ABNORMAL_PARALYSIS		=	1 << 3;
								// const DWORD ABNORMAL_BLIND			=	1 << 4;
								// const DWORD ABNORMAL_LIGHT			=	1 << 5;
								// const DWORD ABNORMAL_FIRE			=	1 << 6;
								// const DWORD ABNORMAL_COLD			=	1 << 7;
								// const DWORD ABNORMAL_HASTE			=	1 << 8;
								// const DWORD ABNORMAL_SHIELD			=	1 << 9;
								// const DWORD ABNORMAL_HIDE			=	1 << 10;
								// const DWORD ABNORMAL_DEX_UP			=	1 << 11;
								// const DWORD ABNORMAL_MAX_HP_UP		=	1 << 12;
								// const DWORD ABNORMAL_ADAMANTINE		=	1 << 13;
								// const DWORD ABNORMAL_MIGHTYWEAPON	=	1 << 14;
								// const DWORD ABNORMAL_BERSERKER		=	1 << 15;
								// const DWORD ABNORMAL_HIEXP			=	1 << 16;
								// const DWORD ABNORMAL_MAGICFIND		=	1 << 17;
								// const DWORD ABNORMAL_NOCHAT			=	1 << 18;

								// const DWORD ABNORMAL_FASTRUN			=	1 << 19;
								// const DWORD ABNORMAL_MINDSHOCK		=	1 << 20;
								// const DWORD ABNORMAL_MINDGUARD		=	1 << 21;
								// const DWORD ABNORMAL_PSISHIELD		=	1 << 22;
								// const DWORD ABNORMAL_PIERCING_SHIELD	=	1 << 23;

								// const DWORD OPERATION_MODE			=	1 << 31;	// ¿î¿µÀÚ Åõ¸í ¿É¼Ç

	short		sCityRank;		// ½Ã¹Îµî±Þ

	int			iGuildIndex;	// ±æµå ÀÎµ¦½º
	VarChar		szGuildName;	// ±æµå ÀÌ¸§
	short		sVersion;		// ±æµå ¹®¾ç ¹öÁ¯
	BYTE		tClass;			// Å¬·¡½º Á¤º¸
	BYTE		tKillState;		// º¸¶óµ¹ÀÌÀÎÁö ÆÇ´Ü
								// 0 : Á¤»óÀûÀÎ À¯Àú
								// 1 : º¸¶óµ¹ÀÌ À¯Àú
	
	VarChar		strLoveName		//È£Äª[¾ÖÄª] //-- yskang 0.1

	short		sEBodyItem[4];	// 0 = ¸Ó¸®
								// 1 = ¸ö
								// 2 = ÆÈ
								// 3 = ´Ù¸®
								// ¾ÆÀÌÅÛÀ» ÀåÂøÇÏÁö ¾ÊÀº ½½·ÔÀº id == -1
*/

const BYTE CHAR_DATA			=	2;
/*
(C <- S)		// Send Character Data

	VarChar		szName;			// User Name

	short		sSTR;			// Èû
	short		sCON;			// °Ç°­
	short		sDEX;			// ¹ÎÃ¸
	short		sVOL;			// ÀÇÁö
	short		sWIS;			// ÁöÇý
	short		sAbilitySum;	// ±âº» ´É·ÂÄ¡ÀÇ ÇÕ

	DWORD		dwSkin;			// ÇÇºÎ»ö
	DWORD		dwHair;			// ¸Ó¸®»ö
	BYTE		Gender;			// ¼ºº°
	TCHAR		strFace[10];	// ¾ó±¼¸ð¾ç

	DWORD		dwExp;			// °æÇèÄ¡
	DWORD		dwXP;			// ½ÇÆÐ°æÇèÄ¡

	short		sSkillPoint;	// ½ºÅ³ Æ÷ÀÎÆ®
	short		sPA;			// ¹°¸® Æ÷ÀÎÆ®

	short		sMaxHP;			// ÃÖ´ë HP
	short		sHP;			// ÇöÀç HP
	short		sMaxPP;			// ÃÖ´ë PP
	short		sPP;			// ÇöÀç PP
	short		sMaxSP;			// ÃÖ´ë SP
	short		sSP;			// ÇöÀç SP

	DWORD		dwDN;			// ¼ÒÁö±Ý
	short		sCityRank;		// ½Ã¹Îµî±Þ

	short		sLevel;			// ·¹º§
	BYTE		byClass;		// Å¬·¡½º (°ÝÅõ = 0, ÁöÆÎÀÌ = 1, µµ°Ë = 2, ÃÑ±â = 3)

	typedef struct _tagSkill	// Skill
	{
		short	sSid;
		BYTE	tLevel;
	}UserSkill[TOTAL_SKILL_NUM];

	BYTE		tHavePsi;		// »çÀÌ¿À´Ð º¸À¯¼ö·®
	typedef	struct _tagPsionic	// »çÀÌ¿À´Ð º¸À¯¼ö·®ÀÌ 1ÀÌ»óÀÎ °æ¿ì Àü¼Û
	{
		short	sSid;
		BYTE	tOnOff;
	} UserPsi[tHavePsi];

	typedef struct _tagItem		// Item;
	{
		short	sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§@@@@@@@
		short	sSid;
		short	sDuration;
		short	sBullNum;
		short	sCount;
		BYTE	tMagic[6];		// ¸ÅÁ÷¼Ó¼º
		BYTE	tIQ;			// Item Quality
	}UserItem[TOTAL_ITEM_NUM];

	short		sEbody[4];		// ÀåÂøÇÑ E_Body sSid, ¾øÀ¸¸é -1

	DWORD		dwExpNext;		// ´ÙÀ½ ·¹º§ÀÌ µÇ±âÀ§ÇÑ ÇÊ¿ä °æÇèÄ¡

	DWORD		dwAbnormal		// »óÅÂÀÌ»óÁ¤º¸(Damaged)

								// const DWORD ABNORMAL_NONE			=	0;
								// const DWORD ABNORMAL_POISON			=	1;
								// const DWORD ABNORMAL_CONFUSION		=	1 << 2;
								// const DWORD ABNORMAL_PARALYSIS		=	1 << 3;
								// const DWORD ABNORMAL_BLIND			=	1 << 4;
								// const DWORD ABNORMAL_LIGHT			=	1 << 5;
								// const DWORD ABNORMAL_FIRE			=	1 << 6;
								// const DWORD ABNORMAL_COLD			=	1 << 7;
								// const DWORD ABNORMAL_HASTE			=	1 << 8;
								// const DWORD ABNORMAL_SHIELD			=	1 << 9;
								// const DWORD ABNORMAL_HIDE			=	1 << 10;
								// const DWORD ABNORMAL_DEX_UP			=	1 << 11;
								// const DWORD ABNORMAL_MAX_HP_UP		=	1 << 12;
								// const DWORD ABNORMAL_ADAMANTINE		=	1 << 13;
								// const DWORD ABNORMAL_MIGHTYWEAPON	=	1 << 14;
								// const DWORD ABNORMAL_BERSERKER		=	1 << 15;
								// const DWORD ABNORMAL_HIEXP			=	1 << 16;
								// const DWORD ABNORMAL_MAGICFIND		=	1 << 17;
								// const DWORD ABNORMAL_NOCHAT			=	1 << 18;
								
								// const DWORD ABNORMAL_FASTRUN			=	1 << 19;
								// const DWORD ABNORMAL_MINDSHOCK		=	1 << 20;
								// const DWORD ABNORMAL_MINDGUARD		=	1 << 21;
								// const DWORD ABNORMAL_PSISHIELD		=	1 << 22;
								// const DWORD ABNORMAL_PIERCING_SHIELD	=	1 << 23;

								// const DWORD OPERATION_MODE			=	1 << 31;	// ¿î¿µÀÚ Åõ¸í ¿É¼Ç

	short		sChangeClass[CLASS_NUM];	// ÀüÁ÷Á¤º¸

  	int			iGuildIndex;	// ±æµå ÀÎµ¦½º
	VarChar		szGuildName;	// ±æµå ÀÌ¸§
	short		sVersion;		// ±æµå ¹®¾ç ¹öÁ¯

	short		sMaxWeight;
	short		sCurWeight;
*/

const BYTE NPC_INFO				= 3;
/*
(C <- S)		// Send NPC Info

	BYTE		Mode;			// 01(INFO_MODIFY)	: NPC Á¤º¸ º¯°æ
								// 02(INFO_DELETE)	: NPC Á¤º¸ »èÁ¦

	short		nid;			// NPC index
	short		pid;			// NPC Picture Index
	VarChar		szName;			// NPC Name
	short		nPosX;			// X Position
	short		nPosY;			// Y Position
	BYTE		tState;			// NPC »óÅÂ
								// 00	: NPC Dead
								// 01	: NPC Live

	BYTE		tNpcKind;		// 00	: Monster
								// 01	: NPC

  	short		sMaxHP;			// ÃÖ´ë HP
	short		sHP;			// ÇöÀç HP

	short		sStepDelay;		// °È´Â ¼Óµµ º¸Á¤

	BYTE		tColor;			// ¸÷ÀÇ »öÀ» ±¸ºÐ

	short		sDimension;		// ÇöÀç ¸Ê¿¡ ¼ÂÆÃÇÒ¼ö ÀÖ´Â Â÷¿ø ¹øÈ£
*/

const BYTE	ITEM_INFO			= 4;
/*
	short		sTotalIndex;	// ÃÑ ¾ÆÀÌÅÛ °¡Áö Á¾·ù ¼ö

	short		sSid;
	short		sPid;
	Varchar		strName;
	Varchar		strText;
	BYTE		byNeedSP;
	DWORD		iDN;
	BYTE		byRLevel;
	BYTE		byClass;
	BYTE		byWear;
	short		sDuration;
	short		sDefense;
	BYTE		byAX;
	BYTE		byAY;
	BYTE		byAZ;
	BYTE		byHitRate;
	BYTE		byCritical;
	short		sAttackDelay;
	BYTE		byRange;
	BYTE		byErrorRate;
	short		sBullNum;
	BYTE		byBullType;
	BYTE		bySubAttack;
	BYTE		bySubDefense;
	BYTE		bySubCure;
	short		sRepairHP;
	short		sRepairPP;
	short		sRepairSP;
	short		sEvent;

	...							// ÇØ´ç ¼ö¸¸Å­ ¹Ýº¹ÇÑ´Ù.
*/

const BYTE	SYSTEM_MSG			= 5;
/*
	(C <- S)		// ¼­¹ö¿¡¼­ Å¬¶óÀÌ¾ðÆ®·Î ¸Þ¼¼Áö¸¦ º¸³½´Ù.

	BYTE		type;			// 00 : Æ¯º°ÇÑ ¸Þ¼¼Áö, ¹Ýµå½Ã º¸¿©Á®¾ß ÇÏ´Â°Í, ¸Þ¼¼Áö ¹Ú½º·Î
								//		Å¬¶óÀÌ¾ðÆ®´Â °­Á¦Á¾·áÇÑ´Ù (ERROR MESSAGE).

								// 01 : Normal Message (Infomation Message)
								// 02 : Error Message (À¯Àú¿¡°Ô ¿¡·¯Á¤º¸¸¦ º¸³¾ ¶§)
								// 03 : NPC Message (NPC°¡ ÇÏ´Â ¸»)
								// 04 : ¿î¿µÀÚ ÀüÃ¼ ½ºÅ©·Ñ ¸Þ¼¼Áö
								// 05 : Dissconnect Message (»ç¿ë±â°£(½Ã°£)ÀÌ Áö³­ À¯Àú¿¡°Ô º¸³¾¶§)
	Varchar		Msg;
*/

const BYTE CLIENT_TIMER			= 6;
/*
	(C -> S)		// Å¬¶óÀÌ¾ðÆ®¿¡¼­ 1ÃÊÀÌ»ó ÆÐÅ¶Àü¼ÛÀÌ ¾øÀ¸¸é º¸³½´Ù.
	// Àü´Þ°ª ¾øÀ½
*/

const BYTE SET_CLIENT_WORLD		= 7;
/*
	(C <- S)		// ³¯¾¾¸¦ ¼ÂÆÃÇÑ´Ù

	BYTE		type;			// 01 : °ÔÀÓ»ó ½Ã°£
								// 02 : ºñ
								// ...

	// 01 : °ÔÀÓ»ó ½Ã°£
		BYTE		tTime;		// ½Ã°£(0~24)

	// 02 : ºñ
		BYTE		OnOff;			// 00 : ¿ÀÇÁ
									// 01 : ¿Â

*/

const BYTE CHANGE_ITEM_INFO		= 8;
/*
	(C <- S)		// ÀåÂø ¹«±â º¯µ¿ÀÏ¶§ º¸³½´Ù.

	int			uid;			// User index

	short		sEquipItem[10];	// 0 = Çï¸ä
								// 1 = °©¿Ê
								// 2 = ¸Ó¸® ¾×¼¼¼­¸®
								// 3 = ¸ö ¾×¼¼¼­¸®
								// 4 = ¿À¸¥ÂÊ ¼Õ ¹«±â
								// 5 = ¿ÞÂÊ   ¼Õ ¹«±â
								// 6 = ¿À¸¥ÂÊ ¾Ç¼¼»ç¸®
								// 7 = ¿ÞÂÊ   ¾Ç¼¼»ç¸®
								// 8 = ¹ÙÁö
								// 9 = ½Å¹ß
								// ¾ÆÀÌÅÛÀ» ÀåÂøÇÏÁö ¾ÊÀº ½½·ÔÀº id == -1
*/

const BYTE USERINFO_REQ			= 9;
/*
	(C -> S)		// Æ¯Á¤ USER, NPCÀÇ Á¤º¸¸¦ ¿äÃ»ÇÑ´Ù.

	int			uid;			// User or NPC ID
*/

//#################################################################
//	Login UI	(10 ~ 29)
//#################################################################

const BYTE SESSION_LOGIN_REQ	=	10;
/*
	(C -> S)		// Session Login Request

	VarChar		id[ACCOUNT_LENGTH];
	VarChar		password[PASSWORD_LENGTH];
*/

const BYTE SESSION_LOGIN_RESULT	=	11;
/*
	(C <- S)		// Session Login Result

	BYTE		result;			// 01 	: Login Success
								// else	: Login Fail
	// ¼º°øÀÎ °æ¿ì
	// º¸³»´Â °ª ¾øÀ½

	// ½ÇÆÐÀÎ °æ¿ì
	BYTE		error_code;		// 01	: Á¸ÀçÇÏÁö ¾Ê´Â ID
								// 02	: Password ºÒÀÏÄ¡
								// 03	: ÇöÀç Account »ç¿ëÁß
								// ...
								// 255	: ¾Ë ¼ö ¾ø´Â ¿¡·¯
*/

const BYTE ACCOUNT_LOGIN_REQ =		12;
/*
	(C -> S)		// Account Login (¾ÏÈ£È­ ¾ÈÇÔ)

	VarChar		id[ACCOUNT_LENGTH];
	short		myserver;		//	1 - ÀÌÄ«·ç½º
									2 - ¿¤ÆÄµµ¸£
									3 - °¡³×Áö¾Æ
									4 - ÇÊ¸³Æ÷
									5 - ¾Æ½ºÆ®
									6 - ±×¶ûµð¿¡
									7 - Æä¼¼¿ì½º
									8 - ¹Ð¶óµð
									9 - ·çÀÌ³×Æ®
									10- Å¸¸£»þ	
*/

const BYTE ACCOUNT_LOGIN_RESULT =	13;
/*
	(C <- S)	// Account Login Result (¾ÏÈ£È­ ¾ÈÇÔ)

	BYTE		result;			// 01	: Account Login Success
								// else : Account Login Fail
	// ½ÇÆÐÀÎ °æ¿ì -------
	BYTE		nErrorCode;		// 01	: Invaild Account
								// 02	: °°Àº °èÁ¤À¸·Î ÀÌ¹Ì ´©±º°¡°¡ Á¢¼ÓÇÏ¿© ÀÖ½À´Ï´Ù.
								// ...
								// 255	: ¾Ë¼ö ¾ø´Â ¿¡·¯

	// ¼º°øÀÎ °æ¿ì -------
	BYTE		nChar;			// Exist Character Number

	// nChar >= 1 ÀÎ°æ¿ì nCharÀÇ ¼ö¸¸Å­ ¹Ýº¹
	{
	BYTE		nIndex;						// Character Index
	VarChar		szName[CHAR_NAME_LENGTH];	// 01 ¹øÂ° Character
	short		STR;
	short		CON;
	short		DEX;
	short		VOL;
	short		WIS;
	DWORD		Skin;			// ÇÇºÎ»ö
	DWORD		Hair;			// ¸Ó¸®»ö
	BYTE		bGender;		// ³², ¿©
	Varchar		szFace[10];		// ¾ó±¼¸ð¾ç

	BYTE		Class;			// °ÝÅõ = 0, ÁöÆÎÀÌ = 1, µµ°Ë = 2, ÃÑ±â = 3
	short		Level;
	short		HP;
	short		PP;
	short		SP;

  	typedef struct _tagSkill	// Skill
	{
		short	sSid;
		BYTE	tLevel;
	}UserSkill[TOTAL_SKILL_NUM];

	char		strWearItem[20];// ÀåÂøÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛ ¸®½ºÆ®
								// ±×¸²¹øÈ£[2] * EQUIP_ITEM_NUM
								// ±×¸²¹øÈ£ == -1 ÀÌ¸é ÀåÂøÇÏÁö ¾ÊÀ½

	short		sEBodyItem[4];	// 0 = ¸Ó¸®
								// 1 = ¸ö
								// 2 = ÆÈ
								// 3 = ´Ù¸®
								// ¾ÆÀÌÅÛÀ» ÀåÂøÇÏÁö ¾ÊÀº ½½·ÔÀº id == -1

  	varchar		strIPAddr[20];	// ÀÌ Ä³¸¯ÅÍ°¡ ºÙ¾î¾ßÇÒ ¼­¹ö
	int			nPort;			// ÀÌ Ä³¸¯ÅÍ°¡ ºÙ¾î¾ßÇÒ ¼­¹öÀÇ ListenÆ÷Æ®
	}

	short	myserver;
	varchar	strAccount[ACCOUNT_LENGTH];
*/

const BYTE GAME_START_REQ =				14;
/*
	(C -> S)	// Game Login  (¾ÏÈ£È­ ¾ÈÇÔ)

	VarChar		szCharName[CHAR_NAME_LENGTH];

	short		myserver;		// CharInfoServer¿¡¼­ ¹ÞÀº °ª	

	varchar		szAccountID[ACCOUNT_LENGTH+1];

*/

const BYTE GAME_START_RESULT =			15;
/*
	(C <- S)	// Game Login Result  (¾ÏÈ£È­ ¾ÈÇÔ)

	BYTE		result;			// 01	: Login Success
								// else	: Login Fail

	// ½ÇÆÐÀÎ °æ¿ì ------		// 01	: Invaild ID
								// 02	: DB¿¡ ¾ø´Â ID
								// 03	: Invalid Zone Number
								// 04	: Invalid Position
								// 05	: ÇöÀç ID »ç¿ëÁß
								// ...
								// 255	: ¾Ë ¼ö ¾ø´Â ¿¡·¯

	short		CurrZone;		// ÇöÀç Zone Number
	int			uid;			// User Index
	short		PosX;			// Map ÀÇ X ÁÂÇ¥
	short		PosY;			// Map ÀÇ Y ÁÂÇ¥

	BYTE		tDir;			// ÇöÀç º¸°íÀÖ´Â ¹æÇâ

	BYTE		tTime;			// ½Ã°£(0~24)
	BYTE		tMinute;		// ºÐ  (0~60)

    Varchar		public_key;		// int64
	DWORD		dwSerial;		// ½Ã¸®¾ó ½ÃÀÛ ¹øÈ£
*/

const BYTE NEW_ACCOUNT_REQ =		16;
/*
	(C -> S)	// New Session Account Req

	VarChar	szUserID[ACCOUNT_LENGTH];
	VarChar	szPassword[PASSWORD_LENGTH];
	VarChar	szRealName[40];
	VarChar	szSocNo[15];
	VarChar	szAddress[80];
	VarChar	szTel[20];
	VarChar	szEmail[30];
	VarChar	szQuiz01[50];
	VarChar	szAnswer01[50];
*/

const BYTE NEW_ACCOUNT_RESULT =		17;
/*
	(C <- S)	// New Session Account Result
	BYTE	result;				// 01 	: New Account Create Success
								// else	: New Account Create Fail
*/

const BYTE	NEW_CHAR_REQ		= 18;
/*
	(C -> S)	// Make New Character (¾ÏÈ£È­ ¾ÈÇÔ)
	VarChar		szAccount[ACCOUNT_LENGTH];			// Account
	BYTE		nIndex;								// Char Index (¸î¹øÂ° Ä³¸¯ÅÍ?)
	VarChar		szCharName[CHAR_NAME_LENGTH];		// Char Name

	DWORD		Skin;				// ÇÇºÎ»ö
	DWORD		Hair;				// ¸Ó¸®»ö
	BYTE		bGender;			// ³², ¿©
	char		szFace[10];			// ¾ó±¼¸ð¾ç
									// 1 : ¸Ó¸®
									// 2 : ¾ó±¼
									// 3 : ´«
									// 4 : ÄÚ
									// 5 : ÀÔ
									// 6 : ¾Ç¼¼»ç¸® 1
									// 7 : ¾Ç¼¼»ç¸® 2
									// 8~10 : ¿¹ºñ

	BYTE		Class;				// °ÝÅõ = 0, ÁöÆÎÀÌ = 1, µµ°Ë = 2, ÃÑ±â = 3

*/

const BYTE	NEW_CHAR_RESULT		= 19;


const BYTE	DELETE_CHAR_REQ		= 20;
/*
	(C -> S)	// Delete Character (¾ÏÈ£È­ ¾ÈÇÔ)

	VarChar		szAccount[ACCOUNT_LENGTH];		// Account
	VarChar		szCharName[CHAR_NAME_LENGTH];	// Char Name
	BYTE		nIndex;							// Char Index (¸î¹øÂ° Ä³¸¯ÅÍ?)
	Varchar		szJumin[13];					// ÁÖ¹Îµî·Ï¹øÈ£
	Varchar		szIP[15];						// »èÁ¦ÇÑ ÄÄÇ»ÅÍÀÇ IP
*/

const BYTE	DELETE_CHAR_RESULT	= 21;
/*
	(C <- S)	// Delete Character Result (¾ÏÈ£È­ ¾ÈÇÔ)
	BYTE		result;				// 01	: Delete Character Success
									// else : Delete Character Fail

	// ¼º°øÀÎ°æ¿ì
	BYTE		nIndex;				// Character Index

	// ½ÇÆÐÀÎ °æ¿ì -------
	BYTE		nErrorCode;			// 01	: Invaild Account
									// 02	: Invaild Char Name
									// 03	: Invaild Char Index
									// 04	: Database Error
									// 05	: Invalid Jumin Bunho
									// 06	: ÁÖ¹Î¹øÈ£°¡ ºz¸²
									// 07	: ±æµå ¸¶½ºÅÍÀÏ¶§´Â ±æµå¸¦ ¸ÕÀú ÇØ»êÇÏ°í »èÁ¦..
									// ...
									// 255	: ¾Ë¼ö ¾ø´Â ¿¡·¯
*/

const BYTE	LOGOUT_REQ			= 22;
/*
	(C -> S)	// Log Out Req
	// Àü´Þ°ª ¾øÀ½
*/

const BYTE	REROLL_REQ			= 23;


const BYTE	REROLL_RESULT		= 24;


const BYTE	RESTART_REQ			= 25;
/*
	(C -> S)	// Restart¹®ÀÇ
*/

const BYTE	RESTART_RESULT		= 26;
/*
	(C <- S)	// RESTART_REQ	Result

	BYTE		result;			// 01	: Restart Success
								// else : Restart Fail
*/

const BYTE	CHECK_ID_REQ		= 27;
/*
	(C -> S)	// Check Character ID Request
	
	VarChar		szCharID[CHAR_NAME_LENGTH];	// Ä³¸¯ÅÍÀÌ¸§
*/

const BYTE	CHECK_ID_RESULT		= 27;		// Packet ÀçÈ°¿ë
/*
	(C <- S)	// Check Character ID Result
	
	BYTE		tResult;		// 00 : °°Àº ÀÌ¸§ÀÇ Ä³¸¯ÅÍ°¡ ÀÌ¹Ì Á¸ÀçÇÕ´Ï´Ù.
								// 01 : »ç¿ëÇÏ½Ç ¼ö ÀÖ´Â ¾ÆÀÌµð ÀÔ´Ï´Ù.
*/

//#################################################################
//	CHAT, BBS		(28 ~ 43)
//#################################################################

const BYTE	MOVE_CHAT_REQ		= 28;
/*
	(C -> S)	// Chat Move Req
	short		will_x;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ÁÂÇ¥ÀÇ X °ª
	short		will_y;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ÁÂÇ¥ÀÇ Y °ª
*/

const BYTE	MOVE_CHAT_RESULT	= 29;
/*
	(C <- S)	// Chat Move Result

	BYTE		result;				// 01	: Move Success
									// else	: Move Fail

	// ¼º°ø½Ã
	int			uid;				// User Index
	short		will_x;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ÁÂÇ¥ÀÇ X °ª
	short		will_y;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ÁÂÇ¥ÀÇ Y °ª
*/

const BYTE	CHAT_REQ			= 30;
/*
	(C -> S)	// Chat Req

	BYTE		Type;				// 01 : Normal Chat
									// 02 : ±Ó¼Ó¸»
									// 03 : 8 È­¸é ¿ÜÄ§
									// 04 : Bunch Chat
									// 05 : Buddy Chat
									// 06 : Buddy ÃÊÃ»
									// 07 : Buddy Å»Åð
									// 10 : ¸ó½ºÅÍ Àâ±â - ¼º°ø½Ã °á°ú´Â MOVE_CHAT_RESULT ·Î º¸³¿
									// 11 : ±Ó¸»Çã¿ë
									// 12 : ±Ó¸»°ÅºÎ
									// 13 : ÀÎ»ç
									// 14 : µµ¹ß
									// 15 : À¯Àú ¼ÛÈ¯		--¼º°ø½Ã ¾çÂÊ¿¡ °á°ú´Â MOVE_CHAT_RESULT ·Î º¸³¿
									// 16 : À¯Àú °­Á¦ ÀÌµ¿  -- ¼º°ø½Ã ¾çÂÊ¿¡ °á°ú´Â MOVE_CHAT_RESULT ·Î º¸³¿
									// 17 : ¸÷À» À¯Àú±ÙÃ³·Î °­Á¦ ¼ÒÈ¯
									// 18 : ±æµå °¡ÀÔ
									// 19 : ±æµå ÇØ»ê
									// 20 : ±æµå Å»Åð
									// 21 : ±æµå Ãß¹æ
									// 22 : ¾ÆÀÌÅÛ ¹Þ±â, °ÅºÎ
									// 23 : ¿î¿µÀÚ Åõ¸í
									// 24 : ¿î¿µÀÚ Åõ¸í ÇØÁ¦
									// 25 : ÀüÃ¼ Ã¤ÆÃ	-- ÇöÀç ¿î¿µÀÚ¸¸ ÇÒ¼öÀÖÀ½...
									// 26 : ÇÊµå ±æµåÀü
									// 27 : ÇÊµåÀü Ç×º¹
									// 28 : PK´©Àû°ª º¸±â
									// 29 : 1ÀÎ µ¿¹Ý ¿öÇÁ
									// 30 : ±æµå¿ø ¸ñ·Ï º¸±â
									// 31 : °ø¼ºÀü ½ÅÃ» ±æµå¸¦ º¸¿©ÁØ´Ù.
									// 32 : Æ÷Æ®¸®½º ¼º¹®À» ¿­°í ´Ý´Â´Ù.
									// 33 : ±æµå ±ÇÇÑ ÀÌ¾ç
									// 34 : ±æµå ±ÇÇÑ ÁßÁö
									// 35 : ±æµå ¿ø¼ÒÈ¯¿ë À¯Àú¸®½ºÆ®
									// 36 : ±æµå¿ø ¼ÒÈ¯

	VarString	UserID(CHAR_NAME_LENGTH)	// 02ÀÇ °æ¿ì ID º¸³¿

	VarString	Msg;						// ¸Þ¼¼Áö(6, 11, 12, 13, 14 ÀÇ °æ¿ì ¸Þ¼¼Áö ¾øÀ½)

	// Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÆÄ½ÌÇØ¼­ typeÀ¸·Î conversion
	|--------------------------------------------------------------------------------
	|	Ã¤ÆÃ ¸í·É¾î : '/' ·Î ½ÃÀÛµÈ´Ù.
	|--------------------------------------------------------------------------------
	|	±â´É		| ¿µ¹® ¸í·É¾î	| ÇÑ±Û ¸í·É¾î	|	¼³¸í
	|---------------|---------------|------------------------------------------------
	|*	¼³Á¤/ÇØÁ¦	| /TO ID		| /±Ó¸» ID 		|	ÇØ´ç ID¿¡ ´ëÇÑ ±Ó¼Ó¸» ¼³Á¤, ÇØÁ¦
	|	(client)	| /GUILD		| /±æµå 		|	ÀÚ½ÅÀÇ ±æµå¿ø ±Ó¼Ó¸» ¼³Á¤/ÇØÁ¦(C)
	|				| /BUDDY		| /¹öµð			|	ÀÚ½ÅÀÇ BUDDY ¿Í ±Ó¼Ó¸» ¼³Á¤/ÇØÁ¦(C)
	|				| /SHOUT		| /¿ÜÄ§			|	¿ÜÄ¡±â ¸ðµå ¼³Á¤/ÇØÁ¦(N)
	|---------------|---------------|-------------------------------------------------
	|	±Ó¼Ó¸»		| /TO ID Msg	| /±Ó¸» ID Msg	|	ÇØ´ç ID ¿¡°Ô ±Ó¼Ó¸»
	|				| /GUILD Msg	| /±æµå	Msg		|	ÀÚ½ÅÀÇ ±æµå¿ø°ú ÀüÃ¼´ëÈ­
	|				| /BUDDY Msg	| /¹öµð	Msg		|	ÀÚ½ÅÀÇ BUDDY ¿Í ±Ó¼Ó¸»(C)
	|				| /SHOUT Msg	| /¿ÜÄ§	Msg		|	»ç¿ëÀÚ¸¦ Áß½ÉÀ¸·Î 8 È­¸é ¿ÜÄ§, ½ºÅ×¹Ì³ª ¼Òºñ(C)
	|---------------|---------------|------------------------------------------------
	|	»ç¶÷Ã£±â	| /FI ID		| /Ã£±â ID		|	ÁöÁ¤ÇÑ ID °¡ °ÔÀÓÁß ÀÎÁö Ã¼Å©
	|				|				| 				|	º°µµÀÇ ÆÐÅ¶À¸·Î Ã³¸®ÇÑ´Ù
	|---------------|---------------|------------------------------------------------
	|*	¼ö½Å°ÅºÎ	| /EX ID		| /Â÷´Ü ID		|	¼ö½Å°ÅºÎ ¼³Á¤/ÇØÁ¦ (ÇØ´ç IDÀÇ ¸ðµç ¸Þ¼¼Áö °ÅºÎ)
	|---------------|---------------|------------------------------------------------
	|	±Ó¸»Çã¿ë	| /TOY			| /±Ó¸»ÄÔ		|
	|---------------|---------------|------------------------------------------------
	|	±Ó¸»°ÅºÎ	| /TON			| /±Ó¸»²û		|
	|---------------|---------------|------------------------------------------------
	|*¼ö½Å°ÅºÎ¸®½ºÆ®| /EXLIST		| /Â÷´Ü¸ñ·Ï		| ÇöÀç ¼ö½Å°ÅºÎÁßÀÎ IDµéÀ» º¸¿©ÁØ´Ù.
	|---------------|---------------|------------------------------------------------
	|*À½¾Ç/¼Ò¸® Á¶Àý| /MUSIC		| /À½¾Ç			| À½¾Ç ON/OFF(N)
	| (client)		| /SOUND		| /¼Ò¸®			| È¿°úÀ½ ON/OFF(N)
	|---------------|---------------|------------------------------------------------
	|	°Å·¡		| /TRADE ID		| /°Å·¡ ID		| ÇØ´ç ID¿¡ ´ëÇØ °Å·¡Ã¢À» ¿¬´Ù.(N)
	|---------------|---------------|------------------------------------------------
	|	À§Ä¡		| /MOVE X Y		| /ÀÌµ¿ X Y		| ÇØ´çÀ§Ä¡·Î ÀÌµ¿(N)
	|---------------|---------------|------------------------------------------------
	| ¸ó½ºÅÍÀâ±â    | /MOP ÀÌ¸§     | /¸÷ ÀÌ¸§      | ÇØ´ç ÀÌ¸§ÀÇ ¸ó½ºÅÍ ±ÙÃ³·Î ÀÌµ¿
	|---------------|---------------|------------------------------------------------
	| ¸ó½ºÅÍ¼ÒÈ¯	| /MOPCALL ÀÌ¸§	| /¸÷¼ÒÈ¯ ÀÌ¸§  | ÇØ´ç ÀÌ¸§ÀÇ ¸ó½ºÅÍ¸¦ ¿î¿µÀÚ±ÙÃ³·Î ¼ÒÈ¯
	|---------------|---------------|------------------------------------------------
	| À¯Àú ¼ÒÈ¯     | /CALL ÀÌ¸§    | /¼ÒÈ¯ ÀÌ¸§    | ÇØ´ç ÀÌ¸§ÀÇ À¯Àú¸¦ ¿î¿µÀÚ ±ÙÃ³·Î ÀÌµ¿
	|---------------|---------------|------------------------------------------------
	| À¯Àú °­Á¦ÀÌµ¿ | /REMOTE ÀÌ¸§  | /¿ø°Ý ÀÌ¸§  X Y | ÇØ´ç ÀÌ¸§ÀÇ À¯Àú¸¦ Æ¯Á¤À§Ä¡·Î ÀÌµ¿
	|---------------|---------------|------------------------------------------------
	| ¿î¿µÀÚ Åõ¸í	| /CLOAKY		| /Åõ¸í			| ¿î¿µÀÚ¸¦ Åõ¸í»óÅÂ·Î ¸¸µç´Ù.
	|---------------|---------------|------------------------------------------------
	| ¿µÀÚ Åõ¸í ÇØÁ¦| /CLOAKN		| /ÇØÁ¦			| ¿î¿µÀÚ Åõ¸í»óÅÂ¿¡¼­ ÀÏ¹Ý »óÅÂ·Î ¸¸µç´Ù.
	|---------------|---------------|------------------------------------------------
	| ÀüÃ¼ Ã¤ÆÃ		| /ANNOUNCE		| /ÀüÃ¤			| Á¢¼ÓÇÑ ¸ðµç À¯Àú¿¡°Ô ¸Þ¼¼Áö¸¦ ³¯¸°´Ù.
	|---------------|---------------|------------------------------------------------
	| ¹öµð ÃÊ´ë		| /INVITE ÀÌ¸§  | /ÃÊ´ë ÀÌ¸§	 | ÇØ´ç ÀÌ¸§ÀÇ À¯Àú¸¦ ¹öµð¿¡ °¡ÀÔ½ÃÅ²´Ù.
	|---------------|---------------|------------------------------------------------
	| ¹öµð Å»Åð		| /LEAVE		| /Å»Åð			 | ÀÚ½ÅÀÌ ¹öµð¿¡¼­ Å»ÅðÇÑ´Ù.
	|---------------|---------------|------------------------------------------------
	| ¹öµð Å»Åð		| /LEAVE  ÀÌ¸§	| /Å»Åð  ÀÌ¸§	 | ¹öµðÀåÀÌ Æ¯Á¤ À¯Àú¸¦ ¹öµð¿¡¼­ Å»Åð½ÃÅ²´Ù.
	|---------------|---------------|------------------------------------------------
	| ±æµå ¹®¾çµî·Ï	| /MARK  ÀÌ¸§	| /¹®Àå  BMP	 | (ClientÀÚÃ¼)±æµå ¹®¾çÀ» µî·ÏÇÑ´Ù.
	| ±æµå °¡ÀÔ		| /				| /±æµå°¡ÀÔ		 | ÇØ´ç ±æµå¿¡ °¡ÀÔÇÑ´Ù.
	| ±æµå ÇØ»ê		| /				| /±æµåÇØ»ê		 | ÇØ´ç ±æµå¸¦ ÇØ»êÇÑ´Ù.
	| ±æµå Å»Åð		| /				| /±æµåÅ»Åð		 | ÇØ´ç ±æµå¿¡¼­ Å»ÅðÇÑ´Ù.
	| ±æµå Ãß¹æ		| /				| /±æµåÃß¹æ	ÀÌ¸§ | ÇØ´ç ±æµå¿¡¼­ ±æ¸¶°¡ Æ¯Á¤ À¯Àú¸¦ Å»Åð½ÃÅ²´Ù.
	|---------------|---------------|------------------------------------------------
	| ¾ÆÀÌÅÛ ¹Þ±â	| /				| /¾ÆÀÌÅÛ¹Þ±â	 | ( ITEM_MODE_CHAT OFF)¾ÆÀÌÅÛ ¹Þ±â¿É¼ÇÀ» È°¼ºÈ­ÇÑ´Ù.
	| ¾ÆÀÌÅÛ °ÅºÎ	| /				| /¾ÆÀÌÅÛ°ÅºÎ	 | ( ITEM_MODE_CHAT ON )¾ÆÀÌÅÛ °ÅºÎ¿É¼ÇÀ» È°¼ºÈ­ÇÑ´Ù.
	|---------------|---------------|------------------------------------------------
	| ±æµå ÇÊµåÀü	| /				| /ÀüÀï ±æµåÀÌ¸§ | ÇØ´ç ±æµå¸¦ »ó´ë·Î ÇÊµåÀüÀ» ½ÅÃ»(/ÀüÀï Guild_ID ±æµåÀÌ¸§)
	|---------------|---------------|------------------------------------------------
	| ÇÊµåÀü Ç×º¹	| /				| /Ç×º¹			 | ÇØ´ç ±æµå¸¦ »ó´ë·Î Ç×º¹À» ¼±¾ðÇÑ´Ù.
	|---------------|---------------|------------------------------------------------
	| PK´©Àû°ª	º¸±â| /PK			| /ÇÇÄÉÀÌ		 | ÇöÀç ´©ÀûµÈ PKÈ½¼ö¸¦ º»´Ù.
	|---------------|---------------|------------------------------------------------
	| 1ÀÎ µ¿¹ÝÀ§ÇÁ	| /COPY			| /Ä«ÇÇ	ID		 | À¯Àú¸¦ ÁöÁ¤ÇÏ¿© ÀÓÀÇÀÇ Àå¼Ò·Î ÀÌµ¿ÇÑ´Ù.
	|---------------|---------------|-----------------------------------------------
	| ±æµå¿ø ÀüÃ¼º¸±â|/GUILD		| /±æµå¿ø		 | ±æµå¿ø ÀüÃ¼ ¸ñ·ÏÀ» º¸¿©ÁØ´Ù.
	|---------------|---------------|------------------------------------------------
	| ½ÅÃ»±æµå º¸±â | /				| /½ÅÃ»±æµå		 | ±æµåÀüÀ» ½ÅÃ»ÇÑ ÀüÃ¼ ±æµå ¸ñ·ÏÀ» º¸¿©ÁØ´Ù.
	|---------------|---------------|------------------------------------------------
	| ¼º¹®¿­°í ´Ý±â | /				| /ÃâÀÔ			 | Æ÷Æ®¸®½º ¼º¹®À» ¿­°í ´Ý´Â´Ù.
	|---------------|---------------|------------------------------------------------
	| ±æµå±ÇÇÑ ºÎ¿©	| /endow		| /ºÎ¿©			 | ±æ¸¶°¡ ºÎ±æ¸¶¸¦ ÁöÁ¤ÇÑ´Ù.
	|---------------|---------------|------------------------------------------------
	| ±æµå±ÇÇÑ ÁßÁö | /stop			| /±ÇÇÑÁßÁö		 | ºÎ±æ¸¶ÀÇ ±Ç¸®¸¦ ¹Ý³³ÇÑ´Ù.

  // * °¡ ÀÖ´Â ¸í·É¾î´Â Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÀÚÃ¼Ã³¸®
*/

const BYTE	CHAT_RESULT			= 31;
/*
	(C <- S)	// Chat Result

	BYTE		result;				// 01	: ¼º°ø
									// else	: ½ÇÆÐ

	// ½ÇÆÐÀÏ °æ¿ì
	BYTE		error_code;			// 01	: ½ºÅ×¹Ì³Ê ºÎÁ·(¿ÜÄ¡±âÀÇ °æ¿ì)
									// 02	: ±Ó¸» °ÅºÎÁß
									// 03	: »ó´ë ¾øÀ½ (±Ó¸», »ç¶÷Ã£±âÀÇ °æ¿ì)
									// 04	: Á¢¼ÓÇÑ ±æµå°¡ ÇÏ³ªµµ ¾øÀ½(±æµå ±Ó¸»)
									// 05	: Á¢¼ÓÇÑ Buddy°¡ ÇÏ³ªµµ ¾øÀ½(Buddy ±Ó¸»)
									// 06	: Àß¸øµÈ À¯Àú¾ÆÀÌµð(±Ó¸», »ç¶÷Ã£±âÀÇ °æ¿ì)
									// 07	: ÇØ´ç ±æµå°¡ Á¸ÀçÇÏÁö ¾Ê´Â´Ù.
									// 08	: ÇØ´ç ±æµå¿¡ °¡ÀÔÇÑ À¯Àú°¡ ¾Æ´Ï´Ù.
									// 09	: ÀÌ¹Ì ´Ù¸¥ ±æµå¿¡ °¡ÀÔÇÑ À¯Àú´Ù.
									// 10	: ±æµå Â¯	±ÇÇÑÀÌ ¾ø´Ù.
									// 11	: ÃÖ´ë ÀÎ¿ø¼ö¸¦ ³Ñ¾ú´Ù.
									// 12	: »ó´ë±æµå°¡ ÀüÀïÁßÀÌ´Ù.
									// ...
									// 255	: ¾Ë¼ö ¾ø´Â ¿¡·¯

	// ¼º°øÀÏ °æ¿ì
	BYTE		Type;				// 01 : Normal Chat
									// 02 : ±Ó¼Ó¸»
									// 03 : 8 È­¸é ¿ÜÄ§
									// 04 : Guild Chat
									// 05 : Buddy Chat
									// 10 : »ç¶÷Ã£±â
									// 11 : ±Ó¸»Çã¿ë
									// 12 : ±Ó¸»°ÅºÎ

	int			uid;				// User Index		(TypeÀÌ 01~ 10 »çÀÌ¸¸ Àü¼Û)
	VarString	Name;				// Character Name	(TypeÀÌ 01~ 10 »çÀÌ¸¸ Àü¼Û)
	VarString	Msg;				// Message			(TypeÀÌ 01~ 09 »çÀÌ¸¸ Àü¼Û)
*/

const BYTE	FIND_USER_REQ		= 32;
/*
	(C -> S)	// User °¡ Á¢¼ÓÁßÀÎÁö È®ÀÎ

	VarString	UserName[CHAR_NAME_LENGTH]	// User Name
*/

const BYTE	FIND_USER_RESULT	= 33;
/*
	(C <- S)	// Find User Result

	BYTE		result;				// 00	: ¼º°ø
									// 01	: Àß¸øµÈ À¯Àú¾ÆÀÌµð
									// 02	: »ó´ë ¾øÀ½
									// ...
									// 255	: ¾Ë¼ö ¾ø´Â ¿¡·¯

	VarString	UserName;			// Character Name
*/

const BYTE	BBS_OPEN			= 34;
/*
	(C <- S)	// BBS¸¦ Open ÇÑ´Ù.

	short	BBS_NUM;				// BBS Number
	short	nCount;					// ÁÙ¼ö
	varchar nData;					// °Ô½Ã¹° µ¥ÀÌÅÍ(¹øÈ£, ÀÛ¼ºÀÚ ID, ÀÛ¼ºÀÏ, Á¶È¸¼ö, Á¦¸ñ)
									// int		iIndex,
									// Varchar	strWriter,
									// Varchar	strTime,
									// int		iView,
									// Varchar	strTitle

*/

const BYTE	BBS_NEXT			= 35;
/*
	(C -> S) // BBSÀÇ °Ô½Ã¹° Á¤º¸¸¦ ÀÐ´Â´Ù.

	short	BBS_NUM;				// BBS Number
	int		nIndex;					// ÀÐ°íÀÚ ÇÏ´Â °Ô½Ã¹° Á¤º¸ÀÇ ½ÃÀÛ¹øÈ£
*/

const BYTE	BBS_NEXT_RESULT		= 36;
/*
	(C <- S)	// ´ÙÀ½ °Ô½Ã¹° Á¤º¸¸¦ º¸³½´Ù.

	short	BBS_NUM;				// BBS Number
	short	nCount;					// ÁÙ¼ö
	varchar nData;					// °Ô½Ã¹° µ¥ÀÌÅÍ(¹øÈ£, ÀÛ¼ºÀÚ ID, ÀÛ¼ºÀÏ, Á¶È¸¼ö, Á¦¸ñ)
									// int		iIndex,
									// Varchar	strWriter,
									// Varchar	strTime,
									// int		iView,
									// Varchar	strTitle
*/

const BYTE	BBS_READ			= 37;
/*
(C -> S) // °Ô½Ã¹°ÀÇ ³»¿ëÀ» ÀÐ´Â´Ù.

	short	BBS_NUM;				// BBS Number
	int		nIndex;					// ÀÐ°íÀÚ ÇÏ´Â °Ô½Ã¹°ÀÇ ¹øÈ£
*/

const BYTE	BBS_READ_RESULT		= 38;
/*
(C <- S) // °Ô½Ã¹°ÀÇ ³»¿ëÀ» º¸³½´Ù.

	short	BBS_NUM;				// BBS Number
	int		nIndex;					// ÀÐ°íÀÚ ÇÏ´Â °Ô½Ã¹°ÀÇ ¹øÈ£
	Varchar Writer;					// ÀÛ¼ºÀÚ
	Varchar Title;					// Á¦¸ñ
	Varchar Time;					// ÀÛ¼ºÀÏÀÚ
	VarData Contents;				// ³»¿ë
*/

const BYTE	BBS_WRITE			= 39;
/*
	(C -> S) // °Ô½Ã¹°À» ÀÛ¼ºÇÑ´Ù.

	short	BBS_NUM;				// BBS Number
	VarChar	Title;					// °Ô½Ã¹° Á¦¸ñ
	VarData	Contents;				// °Ô½Ã¹° ³»¿ë
*/

const BYTE	BBS_EDIT			= 40;
/*
	(C -> S) // °Ô½Ã¹°À» ¼öÁ¤ÇÑ´Ù.

	short	BBS_NUM;				// BBS Number
	int		nIndex;					// °Ô½Ã¹° ¹øÈ£
	VarChar	Title;					// °Ô½Ã¹° Á¦¸ñ
	VarData	Contents;				// °Ô½Ã¹° ³»¿ë
*/

const BYTE	BBS_ERROR			= 41;
/*
	(C <- S)	// °Ô½ÃÆÇ °ü·ÃµÈ ¿¡·¯ ¸®ÅÏ

	BYTE		result;				// 01	: ¼º°ø
									// 02	: ±ÇÇÑ ¾øÀ½(ÀÚ½ÅÀÇ °Ô½Ã¹°ÀÌ ¾Æ´Ï°Å³ª ¿î¿µÀÚ°¡ ¾Æ´Ô)
									// 03	: °Ô½Ã¹°ÀÌ »èÁ¦µÊ
									// ...
									// 255	: ¾Ë¼ö ¾ø´Â ¿¡·¯
*/

const BYTE	BBS_DELETE			= 42;
/*
	(C -> S) 	// °Ô½Ã¹°À» »èÁ¦ÇÑ´Ù.

	short		BBS_NUM;			// BBS Number
	int			nIndex;				// °Ô½Ã¹° ¹øÈ£
*/

//#################################################################
//	Item		(43 ~ 63)
//#################################################################
const BYTE	ITEM_BULLNUM		= 43;
/*
	(C <- S)	// ¾ÆÀÌÅÛÀÇ ÀÜÅº¼ö(·¹Æ¼·ýÆÑ·®)°¡ º¯°æµÈ°ÍÀ» ¾Ë·ÁÁØ´Ù.

	BYTE	tSlot;					// ¾ÆÀÌÅÛ ½½·Ô¹øÈ£
	short	sBullBum;				// º¯°æµÈ ÀÜÅº¼ö(·¹Æ¼·ýÆÑ·®)
*/

const BYTE	ITEM_DURATION		= 44;
/*
	(C <- S)	// ¾ÆÀÌÅÛÀÇ ³»±¸µµ°¡ º¯°æµÈ°ÍÀ» ¾Ë·ÁÁØ´Ù.

	BYTE	tSlot;					// ¾ÆÀÌÅÛ ½½·Ô¹øÈ£
	short	sDuration;				// º¯°æµÈ ³»±¸µµ
*/

const BYTE	ITEM_LOAD_REQ		= 45;
/*
	(C -> S) 	// ¸Ê ¶Ç´Â ¸÷¿¡°Ô¼­ ¾ÆÀÌÅÛÀ» È¹µæ

	BYTE	byReqType;				// 1 : ¸÷
									// 2 : ¸Ê

	// 1¹ø ¸÷ÀÏ °æ¿ì
	short	sSid;					// ¸ó½ºÅÍ ÀÎµ¦½º

	// 2¹ø ¸ÊÀÏ°æ¿ì
	short	sX;						// ¸Ê¿¡¼­
	short	sY;
*/

const BYTE	ITEM_LOAD_RESULT	= 46;
/*
	(S -> C) // ÀÎÁõÀ» ÇÏ°í ÇØ´ç ¸®½ºÆ®¸¦ º¸³½´Ù.
	BYTE	result;					// 00	: ¾ÆÀÌÅÛ ¾øÀ½(½ÇÆÐ)
									// 01	: ¼º°ø½Ã

		// 01 ¾ÆÀÌÅÛ ¼º°ø½Ã
		BYTE	ChangedSlotNum;		// º¯È­µÈ ½½·ÔÀÇ °¹¼ö

		typedef struct _tagMoveItem
		{
			BYTE	tSlot;
			short	sLevel;				//@@@@@@@@
			short	sSid;
			short	sDuration;
			short	sBullNum;
			short	sCount;
			BYTE	tMagic[6];
			BYTE	tIQ;
		} MoveItemInfo[ChangedSlotNum];

	// µ· ¼º°ø½Ã
	// (C <- S) MONEY_CHANGED·Î Àü¼Û

	// 00 : ½ÇÆÐ½Ã
	error_01 : Item Load ¹üÀ§¸¦ ¹þ¾î³µ´Ù
	error_02 : Àß¸øµÈ ÁÂÇ¥
	error_03 : ¿ì¼±±ÇÀÌ ¾ø´Ù
	error_04 : ¹«°Ô¸¦ ÃÊ°úÇÏ¿´´Ù
	error_05 : ºñ¾îÀÖ´Â ÀÎº¥ÀÌ ¾ø´Ù 
	error_08 : ÁÝ´Â È®·ü ½ÇÆÐ 

	error_06 : À¯·É¾ÆÀÌÅÛÀÌ´Ù.
	error_07 : ½Ã½ºÅÛ ¾÷µ« ½ÇÆÐ
	error_255 : ¾Ë¼ö ¾ø´Â¿¡·¯
*/

const BYTE	ITEM_GIVE_REQ		= 47;
/*
	( C -> S ) // »ó´ë¹æ¿¡°Ô ¾ÆÀÌÅÛ, µ·À» ÁÙ¶§

	BYTE	tType;					// 00	: ¸Ê»ó¿¡ ¹ö¸²
									// 01	: »ó´ë¹æ¿¡°Ô ÁØ´Ù

	BYTE	tObject					// 00	: µ·
									// 01	: ¾ÆÀÌÅÛ
	//  00	: ¸Ê»ó¿¡ ¹ö¸²

		short		will_x;			// ¸Ê»ó¿¡¼­ ÁÂÇ¥
		short		will_y;			//

  		//	00	: µ·ÀÏ°æ¿ì
			DWORD	dwDN;				// µ· ¾×¼ö

		//	01	: ¾ÆÀÌÅÛÀÏ°æ¿ì
			BYTE	bySlotId;			// ÀÎº¥Åä¸®Ã¢ÀÇ ÀÎµ¦½º
			short	sCount				// ¾ó¸¸Å­ °¹¼ö


	//  01	: »ó´ë¹æ¿¡°Ô ÁØ´Ù(?)

		//	00	: µ·ÀÏ°æ¿ì
			int		uid;				// ¹ÞÀ» »ó´ë¹æ
			DWORD	dwDN;				// µ· ¾×¼ö

		//	01	: ¾ÆÀÌÅÛÀÏ°æ¿ì
			int		uid;				// ¹ÞÀ» »ó´ë¹æ
			BYTE	bySlotId;			// ÀÎº¥Åä¸®Ã¢ÀÇ ÀÎµ¦½º
			short	sCount				// ¾ó¸¸Å­ °¹¼ö


 // µ· ¼º°ø½Ã
// (C <- S) MONEY_CHANGED·Î Àü¼Û
*/

const BYTE	ITEM_GIVE_RESULT	= 48;
/*
	( C <- S ) // »ó´ë¹æ¿¡°Ô ÁÖ°í ¶Ç´Â ¸Ê¿¡ ¹ö¸®°í ³» ÀÎº¥¿¡¼­ Áö¿î´Ù.

	BYTE	result;					// 00	: ½ÇÆÐ
									// 01	: ¼º°ø

	// 00 ½ÇÆÐ¸é
	BYTE	error;					// 01	: ÁÙ¼öÀÖ´Â »ó´ë°¡ ¾ø´Ù.
									// 02	: »ó´ë ÀÎº¥¿¡ ¿©À¯°ø°£ÀÌ ºÎÁ·ÇÏ´Ù.
									// 03	: »ó´ë¹æÀÌ °ÅºÎ
									// 04	: ¸Ê»ó¿¡ ¹ö¸±¸¸ÇÑ °ø°£ÀÌ ¾ø´Ù(?).
									// 05	: ³» ÀÎº¥¿¡ ¾ÆÀÌÅÛÀÌ ¾ø´Ù.
									// 06	: »ó´ë¿¡°Ô µ·À» ´õÁÙ¼ö°¡ ¾ø´Ù.
									// ...
									// 255	: UnKnown Error

	// 01 ¼º°øÀÌ¸é
		typedef struct _tagItem
		{
			byte	bySlot;
			short	sLevel;
			short	sSid;
			short	sDuration;
			short	sBullNum;
			short	sCount;
			BYTE	tMagic[6];
			BYTE	tIQ;
		} ItemInfo;

*/


const BYTE	ITEM_TRADE_REQ		= 49;
/*
	( C -> S -> Other C ) // ¾ÆÀÌÅÛ..±³È¯À» »ó´ë¹æ¿¡°Ô ½ÅÃ»ÇÑ´Ù

	int			uid;				// »ó´ë¹æ uid
	Varchar		UserName;			// »ó´ë¹æ ÀÌ¸§
	BYTE		Mode;				// 00 : ¿äÃ»Ãë¼Ò
									// 01 : ¿äÃ»
									// 02 : ½Â³«
									// 03 : °ÅÀý
									//...
									// 255: ¾Ë ¼ö ¾ø´Â ¿¡·¯

*/

const BYTE	ITEM_TRADE_RESULT	= 50;
/*
	( Other C -> S -> C ) // ¾ÆÀÌÅÛ..±³È¯À» »ó´ë¹æ¿¡°Ô ÀÀ´äÇÑ´Ù

	int			uid;				// »ó´ë¹æ uid
	Varchar		UserName;			// »ó´ë¹æ ÀÌ¸§
	BYTE		result;				// 01	: ¼º°ø(½Â³«)
									// else	: ½ÇÆÐ(°ÅÀýµî...)

	// ½ÇÆÐÀÏ °æ¿ì
	BYTE		error_code;			// 01 : »ó´ëÆí °ÅÀý(Ãë¼Ò)
									// 02 : ÁØºñ¾ÈµÊ
									// 03 : ´Ù¸¥»ç¶÷°ú °Å·¡Áß
									// 04 : »ó´ëÆíÀ» Ã£À» ¼ö ¾øÀ½
									// 05 : °Å¸®°¡ ³Ê¹« ¶³¾îÁü
									// 06 : ÀÚ½ÅÀÌ °ÅÀý
									// ...
									// 255: ¾Ë ¼ö ¾ø´Â ¿¡·¯
*/

const BYTE	EXCHANGE_REQ		= 51;
/*
	( C -> S -> C(¸ðµÎ´Ù))			// ½ÇÁ¦·Î ¾ÆÀÌÅÛ ±³È¯À» ÇÑ´Ù.

	int			uid;				// »ó´ëÆí uid

	BYTE		Type;				// 0 = Ãë¼Ò
									// 1 = ÀÚ½ÅÀÌ ±³È¯ ½Â³«
									// 2 = »ó´ëÆíÀÌ ±³È¯ ½Â³«
									// 3 = ÀÚ½ÅÀÌ ¾ÆÀÌÅÛ ¿Ã¸²
									// 4 = »ó´ëÆíÀÌ ¾ÆÀÌÅÛ ¿Ã¸²
									// 5 = ÀÚ½ÅÀÌ µ·À» ¿Ã¸²
									// 6 = »ó´ëÆíÀÌ µ·À» ¿Ã¸²


	// Type ÀÌ 3 ÀÏ °æ¿ì
	(C -> S)
	BYTE		tSlotId;			// ÀÎº¥Åä¸®Ã¢ÀÇ ÀÎµ¦½º		(¿Ã¸±¶§ Ç×¸ñ) ±×¿Ü´Â ¹ÞÀ»¶§ Ç×¸ñ
	short		dwCount;			// ¾ó¸¸Å­ °¹¼ö				(¿Ã¸±¶§ Ç×¸ñ)

			(S -> C)
				// Æ®·¹ÀÌµå¿¡ ¿Ã¶ó°¥ ¾ÆÀÌÅÛ
			short		sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§		///@@@@@@@@
			short		sItemID;			// ¾ÆÀÌÅÛ ÀÎµ¦½º
			short		wDur;				// ³»±¸µµ
			short		byBullNum;			// ÀåÅº¼ö
			short		dwCount;			// ¾ó¸¸Å­ °¹¼ö
			BYTE	    tMagic[6];			// ¸ÅÁ÷¼Ó¼º
			BYTE		tIQ;				// Item Quality

				// ÀÎº¥Åä¸®¿¡¼­ º¯µ¿µÈ ¾ÆÀÌÅÛ
			BYTE		tSlotId;			// ÀÎº¥Åä¸®Ã¢ÀÇ ÀÎµ¦½º
			short		sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§
			short		sItemID;			// ¾ÆÀÌÅÛ ÀÎµ¦½º
			short		wDur;				// ³»±¸µµ
			short		byBullNum;			// ÀåÅº¼ö
			short		dwCount;			// ¾ó¸¸Å­ °¹¼ö
			BYTE	    tMagic[6];			// ¸ÅÁ÷¼Ó¼º
			BYTE		tIQ;				// Item Quality

	// Type ÀÌ 4ÀÏ °æ¿ì
	short		sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§
	short		sItemID;			// ¾ÆÀÌÅÛ ÀÎµ¦½º
	short		wDur;				// ³»±¸µµ
	short		byBullNum;			// ÀåÅº¼ö
	short		dwCount;			// ¾ó¸¸Å­ °¹¼ö
	BYTE	    tMagic[6];			// ¸ÅÁ÷¼Ó¼º
	BYTE		tIQ;				// Item Quality

	// Type ÀÌ 5 ÀÏ °æ¿ì
	DWORD		dwCount;			// °Å·¡¿¡ ¿Ã¸° ÃÑ ÇÕÀÇ µ·
	DWORD		dwMyMoney;			// ÀÚ±â¿¡°Ô ³²Àºµ·

	// Type ÀÌ 6 ÀÏ °æ¿ì
	DWORD		dwCount;			// °Å·¡¿¡ ¿Ã¸° ÃÑ ÇÕÀÇ µ·
*/

const BYTE	EXCHANGE_RESULT		= 52;
/*
	( C <- S )						// °á°ú°ª

	BYTE		result;				// 0 = °ÅÀý
									// 1 = ±³È¯ µ¿ÀÇ
									// 2 = ¼­¹ö °ÅÀý
	// °ÅÀý, ¼­¹ö°ÅÀýÀÇ °æ¿ì
	BYTE		error_code;			// 1	: Àß¸øµÈ À¯Àú ¾ÆÀÌµð
									// 2	: ±³È¯½ÅÃ»À» ÇÑ »ç¿ëÀÚ°¡ ¾Æ´Ô
									// 3	: Àß¸øµÈ ¾ÆÀÌÅÛ Á¤º¸
									// 4 	: ÀÚ½ÅÀÌ ±³È¯Ãë¼Ò
									// 5	: Å¸ÀÎÀÌ ±³È¯Ãë¼Ò
									// 6	: ÀÚ½ÅÀÇ ÀÎº¥Åä¸® Ç®
									// 7	: Å¸ÀÎÀÇ ÀÎº¥Åä¸® Ç®
									// ...
									// 255 : UNKNOWN ERROR

	// ±³È¯ µ¿ÀÇÀÏ °æ¿ì

	// º¯°æ¾ÆÀÌÅÛ ¸ñ·Ï
	DWORD		dwDN				// ¼ÒÁö±Ý º¯µ¿

	BYTE		byItemNumber;		// ÃÑ ±³È¯ÇÑ ¾ÆÀÌÅÛ ¼ö ¶Ç´Â µ·
		BYTE		bySlotId;			// ÀÎº¥Åä¸®Ã¢ÀÇ ÀÎµ¦½º
		short		sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§				//@@@@@
		short		sItemID;			// ¾ÆÀÌÅÛ ÀÎµ¦½º
		short		sDuration;			// ³»±¸µµ
		short		byBullNum;			// ÀåÅº¼ö
		short		sCount				// ¾ó¸¸Å­ °¹¼ö (µ·  ¶Ç´Â ¾ÆÀÌÅÛ)
		BYTE	    tMagic[6];			// ¸ÅÁ÷¼Ó¼º
		BYTE		tIQ;				// Item Quality

		...								// °¹¼ö¸¸Å­ ¹Ýº¹ÇÑ´Ù.
*/

const BYTE	ITEM_MOVE_REQ		 = 53;
/*
	( C -> S )						// ¾ÆÀÌÅÛ ÀÌµ¿( ¿¹)ÀåºñÀåÂø, Äü¾ÆÀÌÅÛ, ÀÎº¥Åä¸®³»¿¡¼­ ÀÌµ¿µî.. )
	BYTE			tType;			// 0 : ÀÏ¹Ý ¾ÆÀÌÅÛ(ÀÎº¥Åä¸®³»)
									// 1 : Quick Item µî·Ï
									// 2 : Quick Item ÀÌµ¿, µî·Ï Ãë¼Ò

	// 0 : ÀÏ¹Ý ¾ÆÀÌÅÛ°æ¿ì
	BYTE			bySourceSlot;	// ¿ø·¡ ¾ÆÀÌÅÛÀÌ ÀÖ´ø ½½·Ô À§Ä¡
	BYTE			byDestSlot;		// ¿Å±â·Á´Â ½½·Ô À§Ä¡

	// 1, 2 ¹ø Å¸ÀÔÀº Å¬¶óÀÌ¾ðÆ® ÀÚÃ¼ Ã³¸®ÇÑ´Ù.
	>>-----------------------------[ ¾ø¾îÁø ÄÚµå : jjs07 ] ----------------------------->>
	// 1 : Quick Item µî·Ï
	BYTE			bySourceSlot;	// ÀÎº¥Åä¸® ½½·Ô À§Ä¡
	BYTE			byDestSlot;		// QuickSlotNum ½½·Ô À§Ä¡ (0,1,2,3,4)

	// 2 : Quick Item ÀÌµ¿, µî·Ï Ãë¼Ò
	BYTE			bySourceSlot;	// ÇØ´ç QuickSlotNum ½½·Ô À§Ä¡
	BYTE			byDestSlot;		// ¿Å±â·Á´Â À§Ä¡
									// ¿¹)  µî·ÏÃë¼Ò = 255
									//		±×¿Ü 0~4±îÁö ÀÌµ¿ ¹øÈ£
	<<---------------------------------------------------------------------------------<<
*/

const BYTE	ITEM_MOVE_RESULT	 = 54;
/*
	( C <- S )

	BYTE			Result;			// 01 : Success
									// else	: Fail

	// ¾ÆÀÌÅÛ ¼º°ø½Ã ³»·Á°¨
	BYTE			tType;			// 0 : ÀÏ¹Ý ¾ÆÀÌÅÛ
									// 1 : Quick Item µî·Ï				// 1, 2 ¹øÀº ÀÌÁ¦ »ç¶óÁø ÄÚµå : jjs07
									// 2 : Quick Item ÀÌµ¿, µî·Ï Ãë¼Ò
									// 3 : ±¸¸ÅÇÑ Item ¸ñ·Ï
									// 4 : ÆÇ¸ÅÇÑ Item ¸ñ·Ï
									// 5 : ¼ö¸®ÇÑ Item ¸ñ·Ï

			// 0 : ÀÏ¹Ý ¾ÆÀÌÅÛ ¼º°ø½Ã
			BYTE			ChangedSlotNum;	// º¯È­µÈ ½½·ÔÀÇ °¹¼ö

			typedef struct _tagMoveItem
			{
				byte	bySlot;
				short	sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§//@@@@@@@@@@@
				short	sSid;
				short	sDuration;
				short	sBullNum;
				short	sCount;
				BYTE	tMagic[6];
				BYTE	tIQ;
			} MoveItemInfo[ChangedSlotNum];

			// 1 : Quick Item µî·Ï½Ã
			BYTE			tQuickSlotNum;	// µî·ÏÇÑ ½½·Ô ¹øÈ£
			short			sSid;

			// 2 : Quick Item ÀÌµ¿, µî·Ï Ãë¼Ò
			BYTE			tChangedSlotNum;// º¯È­µÈ ½½·ÔÀÇ °¹¼ö

			typedef struct _tagQuickItem
			{
				byte	tSlot;			    // ÇØ´ç Quick Slot ¹øÈ£
				short	sSid;
			} QuickItemInfo[tChangedSlotNum];

		  // 3, 4, 5¸ñ·Ï Ã³¸®
			BYTE			ChangedSlotNum;	// º¯È­µÈ ½½·ÔÀÇ °¹¼ö

			typedef struct _tagMoveItem
			{
				byte	bySlot;
				short	sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§//@@@@@@@@@
				short	sSid;
				short	sDuration;
				short	sBullNum;
				short	sCount;
				BYTE	tMagic[6];
				BYTE	tIQ;
			} MoveItemInfo[ChangedSlotNum];
*/

const BYTE  ITEM_USE_REQ		= 55;
/*
	(C -> S)	// Äü¾ÆÀÌÅÛ »ç¿ë ¿äÃ»
	BYTE		tType;			// 01 : ÀÏÈ¸¼º ¼Ò¸ð ¾ÆÀÌÅÛ
								// 02 : º¹¼ö »ç¿ë ¾ÆÀÌÅÛ

	// 01 : ÀÏÈ¸¼º ¼Ò¸ð ¾ÆÀÌÅÛ
	BYTE		tSlot			// »ç¿ëÇÏ°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛ

	// 02 : º¹¼ö »ç¿ë ¾ÆÀÌÅÛ
	BYTE		tSlot;			// »ç¿ëÇÏ°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛ
	BYTE		tSid;			// ¿©·¯°³ÀÇ ¹øÈ£Áß ¾î¶² ¹øÈ£¸¦ »ç¿ëÇÒ°Å³Ä?

*/
const BYTE  ITEM_USE_RESULT		= 56;
/*
	(C <- S)	// Äü¾ÆÀÌÅÛ »ç¿ë Åëº¸

	BYTE		tType;			// 00 : ½ÇÆÐ
								// 01 : ¼º°ø

	// ¼º°øÀÏ°æ¿ì
	BYTE		tMode;			// 01  : SP, PP, HP ÇÑ¹ø¿¡ ÀÏÁ¤·®À» Ã¤¿öÁØ´Ù.
								// 08  : È¥¶õÄ¡·á
								// 16  : ¸¶ºñÄ¡·á
								// 32  : Áßµ¶Ä¡·á
								// 64  : ½Ã·ÂÄ¡·á
								// 127 : ¸ðµç »óÅÂÀÌ»óÄ¡·á
								// 100 : °Ë, ÃÑ¿ë ¸®ÇÊ
								// 101 : Æ÷Å»»ç¿ë
								// 102 : º¹±Ç ¾ÆÀÌÅÛ
								// 103 : Ä«ÇÇÄ¨ ¾ÆÀÌÅÛ
	// 01 ÀÏ¶§
	short		sSP_Refill;		// È¸º¹µÈ ¼öÄ¡
	short		sHP_Refill;
	short		sPP_Refill;

		typedef struct _tagItem	// »ç¿ëÇÑ ¾ÆÀÌÅÛ Á¤º¸(¼­¹ö¿¡¼­ °è»ê, ³²¾ÆÀÖ´Â ÃÑ¼ö)
		{
			byte	bySlot;
			short	sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§//@@@@@@@@
			short	sSid;
			short	sDuration;
			short	sBullNum;
			short	sCount;
			BYTE	tMagic[6];
			BYTE	tIQ;
		} ItemInfo;

	// 08 16 32 64 127 100 101 103: ÀÏ¶§
	typedef struct _tagItem
	{
		byte	bySlot;
		short	sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§//@@@@@@@@
		short	sSid;
		short	sDuration;
		short	sBullNum;
		short	sCount;
		BYTE	tMagic[6];
		BYTE	tIQ;
	} ItemInfo;

	// 102 : º¹±Ç¾ÆÀÌÅÛÀÏ¶§
	byte		bySlot;			// ÀÎº¥ ½½·Ô
	DWORD		dwDN;			// ´çÃ·µÈ±Ý¾×

*/

const BYTE	MONEY_CHANGED		= 57;
/*
	(C <- S)	// ¼ÒÁö±Ý º¯°æ Åëº¸

	BYTE		tType;			// 0 : °Å·¡ ÁßÀÏ¶§
								// 1 : ±×¿Ü ¼ÒÁö±Ý º¯µ¿ÀÏ¶§
	// 0 : °Å·¡ ÁßÀÏ¶§
	DWORD		dwTradeDN;		// °Å·¡ Ã¢¿¡¼­ ±Ý¾×
	DWORD		dwDN;			// ¼ÒÁö±Ý

	// 1 : ±×¿Ü ¼ÒÁö±Ý º¯µ¿ÀÏ¶§
	DWORD	dwDN;				// ¼ÒÁö±Ý
*/

const BYTE	FIELD_ITEM_INFO		= 58;

/*
	(C <- S)					// ¸Ê»ó¿¡¼­ ½×¿©ÀÖ´Â ¾ÆÀÌÅÛ Á¤º¸¸¦ º¸³½´Ù.

	short		sLoopCount;		// ÀüÃ¼ ¾ÆÀÌÅÛ ¼ö

		BYTE		Mode;			// 01(ITEM_INFO_MODIFY)	: FIELD ITEM Á¤º¸
									// 02(ITEM_INFO_DELETE)	: FIELD ITEM DELµÈ Á¤º¸¸¦ º¸³½´Ù
									// 03(ITEM_INFO_PICKUP)	: FIELD ITEM ¿¡ ÀÖ´Â ¾ÆÀÌÅÛ¸¦ ÁÝ´Â´Ù.

		short		nPosX;			// X Position
		short		nPosY;			// Y Position

		short		sSid;			//(-1)ÀÏ°æ¿ì ItemÀº ¾ø´Ù.
		DWORD		dwCount;
		BYTE		tIQ;			// ¸ÅÁ÷ ºÎÅÍ °ªÀÌ 1ÀÌ»ó ±×¿Ü´Â 0ÀÓ

		// Mode == 03(ITEM_INFO_PICKUP) ÀÏ °æ¿ì Ãß°¡.
		int			uid;			// ÁÝ´Â À¯Àú

*/

/*
const BYTE	ITEM_WEIGHT_CHANGE	= 59;
/*
	(C <- S)					// ¾ÆÀÌÅÛ ¹«°Ôº¯µ¿¸¦ ¾Ë¸°´Ù.

	short		sMaxWeight;		// ÇöÀç µé¼öÀÖ´Â ÃÖ´ë¹«°Ô
	short		sCurWeight;		// ÇöÀç ¹«°Ô
*/

const BYTE	SERVER_TIME_REQ		= 59;
/*
	(C -> S)	// ¼­¹öÀÇ °ÔÀÓ½Ã°£, ·ÎÄÃ½Ã°£À» ¿äÃ»ÇÑ´Ù.

	// Àü´ÞÀÎÀÚ ¾øÀ½
*/

const BYTE	SERVER_TIME_RESULT	= 59;
/*
	(C <- S)	// ¼­¹öÀÇ °ÔÀÓ½Ã°£, ·ÎÄÃ½Ã°£À» º¸³»ÁØ´Ù.

	short		sYear;
	short		sMonth;
	short		sDay;
	short		sHour;
	short		sMinute;
	short		sSecond;
*/

//#################################################################
//	Game PK Button	(62)
//#################################################################

const BYTE PK_BUTTON_REQ			= 62;

//#################################################################
//	Game Main UI	(63 ~ 88)
//#################################################################
const BYTE	QUESTWINDOW_OPEN_REQ	= 63;
/*
	(C -> S)	// Quest Window Open Request
*/

const BYTE	QUESTWINDOW_OPEN_RESULT	= 63;	// Packet ÀçÈ°¿ë
/*
	(C <- S)	// Quest Window Open Result

	short		sCount;				// º¸À¯ÇÑ Äù½ºÆ®ÀÇ °¹¼ö
	
	// sCount > 0 ÀÎ °æ¿ì
	short		sQuestNum[sCount];	// Äù½ºÆ® ¹øÈ£ (sCount ¸¸Å­ ¹Ýº¹ÇØ¼­ º¸³½´Ù)
*/

const BYTE	TRADE_MODE_REQ		= 64;
/*
	(C -> S)	// Trade Mode On/Off Req
	BYTE		Mode;			// 00	: Off
								// 01	: On
*/

const BYTE	TRADE_MODE_RESULT	= 65;
/*
	(C <- S)	// Trade Mode On/Off Result
	BYTE		Mode;			// 00	: Off
								// 01	: On
*/

const BYTE DH_DAMAGE_NUM = 66;
/*
 (S -> c)
 byte 1
 short tarID
 byte 0   monster
      1   user
 short num

*/
const BYTE M_SET_HP = 67;
/*
(S- C)
short nid
dword nid
dword hp
*/

const BYTE	OPEN_EQUIP_REQ		= 68;
/*
	(C -> S)	// Equip/Skill Window Open
	// Àü´Þ°ª ¾øÀ½
*/

const BYTE	OPEN_EQUIP_RESULT	= 69;
/*
	(C <- S)	// Equip/Skill Window Open Result

	short		WeaponInfo[8];
	BYTE		EvadeInfo[4];

	BYTE		Rate[5];			// ½ºÅ³ Àû¿ë ¼º°øÀ²
*/

const BYTE	CHANGE_WEAPON_REQ	= 70;
/*
	(C -> S)	// ¹«±â º¯°æ (¿À¸¥¼Õ , ¿Þ¼Õ ¹«±â ÀüÈ¯)
*/

const BYTE	CHANGE_WEAPON_RESULT= 71;
/*
	(C <- S)	// ¹«±â º¯°æ °á°ú

	BYTE		result;			// 00 : ¹«±â º¯°æ ºÒ°¡´É
								// 01 : ¼º°ø ( ¼º°ø½Ã¿¡´Â UserInfo ¹ß»ý)

	// result °¡ ¼º°øÀÏ¶§
	BYTE		tEnable;		// ÇØ´ç ¹«±â ½ºÅ³À» ¾µ¼ö ÀÖ´ÂÁö ÆÇ´Ü
								// 00 : ¾µ ¼ö ¾øÀ½
								// 01 : ¾µ ¼ö ÀÖÀ½

	BYTE		tSkillClass		// ÇØ´ç ½ºÅ³ Å¬·¡½ºÇ¥½Ã
	typedef struct _tagSkill	// Skill
	{
		short	sSid;		// Skill id
		BYTE	tLevel;		// Level
	}UserSkill[SKILL_NUM];	// SKILL_NUM = 5
*/

const BYTE	SKILL_ONOFF_REQ		= 72;
/*
	(C -> S)	// Æ¯Á¤ ½ºÅ³À» ON/OFF ÇÑ´Ù.

	short		sSid;			// Skill ID
*/

const BYTE	SKILL_ONOFF_RESULT	= 73;
/*
	(C <- S)	// ½ºÅ³ ON/OFF RESULT

	BYTE		result;			// 00 : Fail
								// 01 : Success
	// ¼º°øÀÏ °æ¿ì
	short	sSid;		// Skill id
	BYTE	tLevel;		// Level
	BYTE	tOnOff;		// On/Off Á¤º¸
*/

const BYTE	PSI_CHANGE_REQ		= 74;
/*
	(C -> S)	// »çÀÌ¿À´Ð ÀüÈ¯ ¿ä±¸

	short		sSid;			// Psionic ID
*/

const BYTE	PSI_CHANGE_RESULT	= 75;


const BYTE	LEVEL_UP			= 76;//@@@@@@@@@@@@@@@¹Ù²Þ

const BYTE	LEVEL_DOWN			= 77;
/*
	(C <- S)					// level down
								// °á°ú°ªÀ¸·Î USER_STATUS_SKILL_UP_RESULT + LEVEL_UP »ç¿ë
*/								// ³ªÁß¿¡ Áßº¹µÇ´Â ³»¿ëÀ» Á¤¸®

const BYTE	USER_STATUS_SKILL_UP_REQ= 78;//@@@@@@@@@@@@@@@¹Ù²Þ
/*
	(C -> S)					// PA, Skill¸¦ ¿Ã¸°´Ù.

	BYTE	tType;				// 01 : PA
								// 02 : Skill


	// 01 : PA ´É·ÂÄ¡ Áõ°¡
		BYTE	tPaMaNum;	// 0~4±îÁöÁß +¹øÈ£

	// 02 : Skill Level Áõ°¡
		BYTE	tSkillNum;

*/

const BYTE	USER_STATUS_SKILL_UP_RESULT= 79;//@@@@@@@@@@@@@@@¹Ù²Þ


const BYTE	BUDDY_MODE_REQ			= 80;
/*
	(C -> S)	// BUDDY Mode On/Off Req
	BYTE		Mode;			// 00	: Off
								// 01	: On
*/

const BYTE	BUDDY_MODE_RESULT		= 81;
/*
	(C <- S)	// BUDDY Mode On/Off Result
	BYTE		Mode;			// 00	: Off
								// 01	: On
*/

const BYTE	BUDDY_USER_CHANGE		= 82;



//#################################################################
//	Move			(88 ~ 106)
//#################################################################
const BYTE	CHANGE_DIR_REQ		= 88;
/*
	(C -> S)	// ¹æÇâÀüÈ¯ ¿ä±¸

	BYTE		tDir;			//	3 4 5
								//	2 8 6
								//	1 0 7
*/

const BYTE CHANGE_DIR_RESULT	= 89;
/*
	(S -> C)	// ¹æÇâÀüÈ¯ °á°ú

	int			uid;			// User id
	BYTE		tDir;			// ¹æÇâ
*/

const BYTE	MOVE_FIRST_REQ		= 90;
/*
	(C -> S)	// Move First Request

	short		will_x;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â Ã¹ ÁÂÇ¥ÀÇ X °ª
	short		will_y;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â Ã¹ ÁÂÇ¥ÀÇ Y °ª

	short		end_x;				// ¿òÁ÷ÀÌ·Á´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ X °ª
	short		end_y;				// ¿òÁ÷ÀÌ·Á´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ Y °ª
*/

const BYTE	MOVE_FIRST_RESULT	= 91;
/*
	(C <- S)	// Move First Result

	BYTE		result;				// 01	: Move First Success
									// else	: Move First Fail

	// ½ÇÆÐ½Ã
	int			uid;				// User Index
	short		cur_x;				// ¼­¹öÀÇ ÇöÀç x ÁÂÇ¥
	short		cur_y;				// ¼­¹öÀÇ ÇöÀç y ÁÂÇ¥

	// ¼º°ø½Ã
	int			uid;				// User Index
	short		will_x;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â Ã¹ ÁÂÇ¥ÀÇ X °ª
	short		will_y;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â Ã¹ ÁÂÇ¥ÀÇ Y °ª

	short		end_x;				// ¿òÁ÷ÀÌ·Á´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ X °ª
	short		end_y;				// ¿òÁ÷ÀÌ·Á´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ Y °ª
*/

const BYTE	MOVE_REQ			= 92;
/*
	(C -> S)	// Move Req
	short		will_x;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ÁÂÇ¥ÀÇ X °ª
	short		will_y;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ÁÂÇ¥ÀÇ Y °ª
*/

const BYTE	MOVE_RESULT			= 93;
/*
	(C <- S)	// Move Result

	BYTE		result;				// 01	: Move Success
									// else	: Move Fail

	// ½ÇÆÐ½Ã
	int			uid;				// User Index
	short		cur_x;				// ¼­¹öÀÇ ÇöÀç x ÁÂÇ¥
	short		cur_y;				// ¼­¹öÀÇ ÇöÀç y ÁÂÇ¥

	// ¼º°ø½Ã
	int			uid;				// User Index
	short		will_x;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ÁÂÇ¥ÀÇ X °ª
	short		will_y;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ÁÂÇ¥ÀÇ Y °ª
*/


const BYTE	MOVE_END_REQ		= 94;
/*
	(C -> S)	// Move Req
	short		end_x;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ X °ª
	short		end_y;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ Y °ª
*/

const BYTE	MOVE_END_RESULT		= 95;
/*
	(C <- S)	// Move Result

	BYTE		result;				// 01	: Move Success
									// else	: Move Fail
	// ½ÇÆÐ½Ã
	int			uid;				// User Index
	short		cur_x;				// ¼­¹öÀÇ ÇöÀç x ÁÂÇ¥
	short		cur_y;				// ¼­¹öÀÇ ÇöÀç y ÁÂÇ¥

	// ¼º°ø½Ã
	int			uid;				// User Index
	short		end_x;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ X °ª
	short		end_y;				// ¿òÁ÷ÀÌ·Á°í ÇÏ´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ Y °ª
*/

const BYTE	RUN_FIRST_REQ		= 96;
/*
	(C -> S)	// Run First Request

	short		w_x1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ X °ª
	short		w_y1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ Y °ª
	short		w_x2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª
	short		w_y2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª

	short		end_x;				// ¶Ù·Á´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ X °ª
	short		end_y;				// ¶Ù·Á´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ Y °ª
*/

const BYTE	RUN_FIRST_RESULT	= 97;
/*
	(C <- S)	// Run First Result

	BYTE		result;				// 01	: Run First Success
									// else	: Run First Fail
	// ½ÇÆÐ½Ã
	int			uid;				// User Index
	short		cur_x;				// ¼­¹öÀÇ ÇöÀç x ÁÂÇ¥
	short		cur_y;				// ¼­¹öÀÇ ÇöÀç y ÁÂÇ¥

	// ¼º°ø½Ã
	// w_x2, w_y2 °¡ -1 ÀÌ¸é µÎ¹øÂ° ÁÂÇ¥·Î´Â ÀÌµ¿ ºÒ°¡.
	int			uid;				// User Index
	short		w_x1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ X °ª
	short		w_y1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ Y °ª
	short		w_x2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª
	short		w_y2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª

	short		end_x;				// ¶Ù·Á´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ X °ª
	short		end_y;				// ¶Ù·Á´Â ¸¶Áö¸· ÁÂÇ¥ÀÇ Y °ª
*/

const BYTE	RUN_REQ				= 98;
/*
	(C -> S)	// Run Request

	short		w_x1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ X °ª
	short		w_y1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ Y °ª
	short		w_x2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª
	short		w_y2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª

*/

const BYTE	RUN_RESULT			= 99;
/*
	(C <- S)	// Run Result

	BYTE		result;				// 01	: Run Success
									// else	: Run Fail
	// ½ÇÆÐ½Ã
	int			uid;				// User Index
	short		cur_x;				// ¼­¹öÀÇ ÇöÀç x ÁÂÇ¥
	short		cur_y;				// ¼­¹öÀÇ ÇöÀç y ÁÂÇ¥

	// ¼º°ø½Ã
	// w_x2, w_y2 °¡ -1 ÀÌ¸é µÎ¹øÂ° ÁÂÇ¥·Î´Â ÀÌµ¿ ºÒ°¡.
	int			uid;				// User Index
	short		w_x1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ X °ª
	short		w_y1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ Y °ª
	short		w_x2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª
	short		w_y2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª
*/

const BYTE	RUN_END_REQ			= 100;
/*
	(C -> S)	// Run End Request

	short		w_x1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ X °ª
	short		w_y1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ Y °ª
	short		w_x2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª
	short		w_y2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª

*/

const BYTE	RUN_END_RESULT		= 101;
/*
	(C <- S)	// Run End Result

	BYTE		result;				// 01	: Run End Success
									// else	: Run End Fail

	// ½ÇÆÐ½Ã
	int			uid;				// User Index
	short		cur_x;				// ¼­¹öÀÇ ÇöÀç x ÁÂÇ¥
	short		cur_y;				// ¼­¹öÀÇ ÇöÀç y ÁÂÇ¥

	// ¼º°ø½Ã
	// w_x2, w_y2 °¡ -1 ÀÌ¸é µÎ¹øÂ° ÁÂÇ¥·Î´Â ÀÌµ¿ ºÒ°¡.
	int			uid;				// User Index
	short		w_x1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ X °ª
	short		w_y1;				// ¶Ù·Á´Â Ã¹ ÁÂÇ¥ÀÇ Y °ª
	short		w_x2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª
	short		w_y2;				// ¶Ù·Á´Â 2¹øÂ° ÁÂÇ¥ÀÇ X °ª
*/

const BYTE	SET_STAEMINA		= 102;
/*
	(C <- S)	// ½ºÅ×¹Ì³Ê Á¤º¸¸¦ ¼ÂÆÃÇÑ´Ù.

	short		sSP;				// ½ºÅ×¹Ì³Ê °ª
*/


//#################################################################
//	Battle		(106 ~ 130)
//#################################################################
const BYTE SET_XP					= 106;
/*
	(C <- S)	// XP Á¤º¸¸¦ ¼ÂÆÃÇÑ´Ù.

	DWORD		dwXP;				// XP °ª
*/

const BYTE SET_HP					= 107;
/*
	(C <- S)	// HP Á¤º¸¸¦ ¼ÂÆÃÇÑ´Ù.

	short		sHP;				// HP °ª
*/

const BYTE SET_PP					= 108;
/*
	(C <- S)	// PP Á¤º¸¸¦ ¼ÂÆÃÇÑ´Ù.

	short		sPP;				// PP °ª
*/

const BYTE PSI_ATTACK_REQ			= 109;
/*
	(C -> S)	// Psionic Attack Req

	int			TargetID;			// °ø°Ý´ë»ó ID
	BYTE		PsionicID;			// »çÀÌ¿À´Ð sid

	// »çÀÌ¿À´ÐÀÌ ÅÚ·¹Æ÷Æ®ÀÏ °æ¿ì Àü¼Û (ÅÚ·¹Æ÷Æ® ¾ÆÀÌµð´Â PSI_TELEPORT ·Î Á¤ÀÇµÇ¾î ÀÖÀ½)
	int			PosX;				// ÅÚ·¹Æ÷Æ®ÇÒ À§Ä¡ÁÂÇ¥ X
	int			PosY;				// ÅÚ·¹Æ÷Æ®ÇÒ À§Ä¡ÁÂÇ¥ Y
*/

const BYTE PSI_ATTACK_RESULT		= 110;
/*
	(C <- S)	// Psionic Attack Result

	BYTE		bSuccess;			// 00 : Attack Fail
									// 01 : Attack Success

	// Attack Miss, Attack Success ÀÇ °æ¿ì Àü¼Û, Fail ÀÏ °æ¿ì bSuccess Á¤º¸¸¸ Àü¼ÛÇÑ´Ù.
	BYTE		PsionicID;			// »çÀÌ¿À´Ð sid

	int			SourceID;			// °ø°ÝÀÚ ID
	int			DestID;				// °ø°Ý ¹ÞÀºÀÚ ID
*/

const BYTE ATTACK_REQ				= 111;
/*
	(C -> S)	// Attack Request

	short		TargetID;			// Target ID
*/

const BYTE ATTACK_RESULT			= 112;
/*
	(C <- S)	// Attack Result

	BYTE		bSuccess;			// 00	: Attack Fail
									// 01	: Attack Success
									// 02	: Attack Miss

	// Attack Fail ÀÏ °æ¿ì ¾Æ·¡ÀÇ ¿¡·¯ÄÚµå¸¸ º¸³½´Ù.
	BYTE		tError;				// 00	: Too Short Attack Delay
									// 01	: Too Long Distance
									// 02	: ÃÑ±â°íÀå
									// 03	: ÀÜÅººÎÁ·
									// 04	: ·¹Æ¼´½ÆÑ Empty
									// 05	: ½ºÅ×¹Ì³ª ºÎÁ·


	// Attack Success, Attack Miss ÀÇ °æ¿ì¿¡¸¸ ¾Æ·¡ÆÐÅ¶ Àü¼Û
	int			SourceID;			// °ø°ÝÀÚ ID
	int			DestID;				// °ø°Ý ¹ÞÀºÀÚ ID

	// Attack Success ÀÇ °æ¿ì¿¡¸¸ Àü¼Û (MISS ÀÎ °æ¿ì´Â ¾Æ·¡³»¿ë Àü¼Û¾ÈÇÔ)

	// °ø°ÝÃø¿¡ ¶ß´Â ½ºÅ³ ¾×¼Ç
	BYTE		tActionNum1;		// ½ºÅ³¾×¼Ç 1 ¼ýÀÚ
	BYTE		tSkillAction1[tActionNum1];		// Skill Action (ÇØ´ç ½ºÅ³ ¾×¼ÇÇ¥ ÂüÁ¶, DEFINE µÇ¾îÀÖÀ½)
												// 0 : None
												// 1 : Damage Critical
												// 2 : Vital Critical
												// 3 : Delay Critical
												// 4 : Concentrate (PSI)
												// 5 : PSY Acclimate
												// 6 : Fast Cast
												// 7 : Weapon Acclimate
												// 8 : Fast Attack
												// 9 : Out Range
												// 10: Strafe
												// 11: Broken
												// 12: Fixed
												// 13: Overheat
												// 14: È­¿°µ¥¹ÌÁö
												// 15: ³Ã±âµ¥¹ÌÁö
												// 16: Àü±â´ë¹ÌÁö
												// 17: ¸¶ºñ
												// 18: Áßµ¶
												// 19: Guard

	// ¹æ¾îÃø¿¡ ¶ß´Â ½ºÅ³ ¾×¼Ç
	BYTE		tActionNum2;		// ½ºÅ³¾×¼Ç 2 ¼ýÀÚ
	BYTE		tSkillAction2[tActionNum2];		// Skill Action (ÇØ´ç ½ºÅ³ ¾×¼ÇÇ¥ ÂüÁ¶, DEFINE µÇ¾îÀÖÀ½)

	short		HP;					// °ø°Ý¹ÞÀºÀÚÀÇ HP
	short		MaxHP;				// °ø°Ý¹ÞÀºÀÚÀÇ ÃÖ´ë HP
*/
//yskang 0.3 2002-11-6 º¯°æµÇ´Â ÆÐÅ¶...
/*
	(C <- S)	// Attack Result

	BYTE		bSuccess;			// 00	: Attack Fail
							// 01	: º¸Åë °ø°Ý ¼º°ø
							// 02	: º¸Åë °ø°Ý ½ÇÆÐ(»ó´ë¹æ ÇÇÇÑ°Å)
							// 03 	: Å©¸®Æ¼ÄÃ °ø°Ý ¼º°ø
							// 04   : Å©¸®Æ¼ÄÃ °ø°Ý ½ÇÆÐ(»ó´ë¹æ ÇÇÇÑ°Å)



	// Attack Fail ÀÏ °æ¿ì ¾Æ·¡ÀÇ ¿¡·¯ÄÚµå¸¸ º¸³½´Ù.
	BYTE		tError;				// 00	: Too Short Attack Delay
									// 01	: Too Long Distance
									// 02	: ÃÑ±â°íÀå
									// 03	: ÀÜÅººÎÁ·
									// 04	: ·¹Æ¼´½ÆÑ Empty
									// 05	: ½ºÅ×¹Ì³ª ºÎÁ·


	// Attack Success, Attack Miss ÀÇ °æ¿ì¿¡¸¸ ¾Æ·¡ÆÐÅ¶ Àü¼Û
	int			SourceID;			// °ø°ÝÀÚ ID
	int			DestID;				// °ø°Ý ¹ÞÀºÀÚ ID

	short		HP;					// °ø°Ý¹ÞÀºÀÚÀÇ HP
	short		MaxHP;				// °ø°Ý¹ÞÀºÀÚÀÇ ÃÖ´ë HP
*/


const BYTE	DEAD					= 113;
/*
	(C <-S)		// you are dead!!!

	short		uid;				// USER, NPC ID
	short		posx;				// X Position
	short		posy;				// Y Position
*/

const BYTE	LIVE_REQ				= 114;
/*
	(C -> S)	// Revival Request

	(C <- S)	// °á°ú°ªÀ¸·Î USER_INFO·Î ¾Ë·ÁÁØ´Ù.
*/


const BYTE	PSI_TOWN_POTAL			= 115;
/*
	(C <- S)	// PSI_ATTACK_REQ¿¡´ëÇÑ °á°ú°ª ¹ÝÈ¯(ÃÖÀûÈ­ ÇÊ¿ä)

	BYTE		tType;				// 00 : Fail
									// 01 : Success

	// ¼º°øÀÏ¶§¸¸...
	BYTE	tSuccess				// 00 : °°Àº Á¸ÀÌ´Ù
									// 01 : ´Ù¸¥ Á¸ÀÌ´Ù.

	int		uid;					// »ç¿ëÀÚ uid

	short	nPosX;					// X Position
	short	nPosY;					// Y Position
	short	sZone;					// Load Zone Number
*/

const BYTE	SET_SPEED_MONSTER		= 116;
/*
	(C <- S)	// ÀÌµ¿ ¼Óµµ º¯È­¸¦ ¾Ë·Á ÁØ´Ù.

	int		uid;					// ¸÷ uid
	short	sDelay;					// º¯È­µÈ ¼Óµµ¸¦ Ç¥½Ã
*/

const BYTE	SET_USER_STATE			= 117;
/*
	(C <- S)	// À¯Àú »óÅÂ º¯È­Á¤º¸¸¦ º¸³½´Ù.

	int		uid;			// uid
	DWORD	dwAbnormal		// »óÅÂÀÌ»óÁ¤º¸(Damaged)

							// const DWORD ABNORMAL_NONE			=	0;
							// const DWORD ABNORMAL_POISON			=	1;
							// const DWORD ABNORMAL_CONFUSION		=	1 << 2;
							// const DWORD ABNORMAL_PARALYSIS		=	1 << 3;
							// const DWORD ABNORMAL_BLIND			=	1 << 4;
							// const DWORD ABNORMAL_LIGHT			=	1 << 5;
							// const DWORD ABNORMAL_FIRE			=	1 << 6;
							// const DWORD ABNORMAL_COLD			=	1 << 7;
							// const DWORD ABNORMAL_HASTE			=	1 << 8;
							// const DWORD ABNORMAL_SHIELD			=	1 << 9;
							// const DWORD ABNORMAL_HIDE			=	1 << 10;
							// const DWORD ABNORMAL_DEX_UP			=	1 << 11;
							// const DWORD ABNORMAL_MAX_HP_UP		=	1 << 12;
							// const DWORD ABNORMAL_ADAMANTINE		=	1 << 13;
							// const DWORD ABNORMAL_MIGHTYWEAPON	=	1 << 14;
							// const DWORD ABNORMAL_BERSERKER		=	1 << 15;
							// const DWORD ABNORMAL_HIEXP			=	1 << 16;
							// const DWORD ABNORMAL_MAGICFIND		=	1 << 17;
							// const DWORD ABNORMAL_NOCHAT			=	1 << 18;
							
							// const DWORD ABNORMAL_FASTRUN			=	1 << 19;
							// const DWORD ABNORMAL_MINDSHOCK		=	1 << 20;
							// const DWORD ABNORMAL_MINDGUARD		=	1 << 21;
							// const DWORD ABNORMAL_PSISHIELD		=	1 << 22;
							// const DWORD ABNORMAL_PIERCING_SHIELD	=	1 << 23;
							  
							// const DWORD OPERATION_MODE			=	1 << 31;	// ¿î¿µÀÚ Åõ¸í ¿É¼Ç
*/

const BYTE	SET_USER_PK_STATE			= 118;
/*
	(C <- S)	// ÇöÀç ½Ã¹Îµî±Þ ¶Ç´Â PK»óÅÂ¸¦ º¸³½´Ù.

	int		uid;					// uid

	BYTE	tType;					// 01 : À¯Àú ½Ã¹Îµî±Þ
									// 02 : À¯Àú°¡ ÀÏ½ÃÀûÀÎ Ä«¿À»óÅÂ¿¡ ÀÖ´Ù.

	// 01 : À¯Àú ½Ã¹Îµî±Þ
	short	sCityRank;				// -5 ~ 5±îÁö ½Ã¹Îµî±ÞÀ» º¸³¿

	// 02 : À¯Àú°¡ ÀÏ½ÃÀûÀÎ Ä«¿À»óÅÂ¿¡ ÀÖ´Ù.
	BYTE	tPKStatus;				// ÇöÀç ÀÏ½ÃÀûÀÎ Ä«¿À»óÅÂ : 1
									// ÇöÀç Ä«¿À»óÅÂ¿¡¼­ ¹þ¾î³² : 0
*/

const BYTE	USE_POTION					= 119;
/*
	(S -> C)	// ¹°¾à »ç¿ëÁ¤º¸¸¦ º¸³½´Ù.

	int		uid;
	BYTE	tType;					// 01 : Ã¼·ÂÈ¸º¹Á¦
									// 02 : Á¤½Å¾ÈÁ¤Á¦
									// 03 : ½ºÅ×¹Ì³ÊÈ¸º¹Á¦
									// 04 : ¹ü¿ëÈ¸º¹Á¦
									// 05 : ÆøÁ×
									// 06 : °æÇèÄ¡ 2¹è
									// 07 : ¸ÅÁ÷Âù½º 5¹è
									// 08 : PK ¼öÄ¡ - 1
									// 09 : ±Ø¾à

									const BYTE HP_POTION			= 1;
									const BYTE PP_POTION			= 2;
									const BYTE SP_POTION			= 3;
									const BYTE ALL_POTION			= 4;
									const BYTE SALUTE				= 5;
									const BYTE EXP_POTION			= 6;
									const BYTE MAGIC_POTION			= 7;
									const BYTE KILLREMOVE_POTION	= 8;
									const BYTE DEAD_POTION			= 9;

*/

//#################################################################
//	Event		(131 ~ 170)
//#################################################################
const BYTE MAX_HP_EXIT				= 120;
/*
	short		sHP;
	short		sMaxHP;
*/
const BYTE	CLIENT_EVENT			= 131;
/*
	(C -> S)	// ¼­¹ö¿¡ Event ¼öÇàÀ» ¿äÃ»ÇÑ´Ù.

	BYTE		tType;				// 01 : USER, NPC ID
									// 02 : HyperText

	// 01 : USER, NPC ID
		int			uid;			// USER, NPC ID

	// 02 : HyperText ÇØ´ç Á¸¿¡¼­ ¹ß»ýÇÏ´Â ÀÌº¥Æ® ¹øÈ£¸¦ º¸³½´Ù.

		short		sZone;			// ÇØ´ç Á¸ ¹øÈ£
		short		sEventNum;		// ÀÌº¥Æ® ¹øÈ£
*/

const BYTE	STORE_OPEN				= 132;
/*
	(C <- S)	// Æ¯Á¤ »óÁ¡À» ¿¬´Ù. (BUY ÀÎÅÍÆäÀÌ½º)

	short		sStoreID;			// »óÁ¡¾ÆÀÌµð
	short		sRate;				// ½Ã¼¼(³ªÁß¿¡ Áö¿ª¿¡ µû¶ó ¼¼±ÝÀÌ Æ²·ÁÁú ¶§µî...)
	short		sCount;				// ÆÄ´Â ¾ÆÀÌÅÛÀÇ ÃÑ °¹¼ö

	typedef struct _tagSellItem		// ÆÄ´Â ¾ÆÀÌÅÛÀÇ Á¤º¸
	{
		short	sSid;	// Item serial id
		DWORD	dwCost;	// Item cost
	}SellItem[sCount];
*/

const BYTE	BUY_ITEM				= 133;
/*
	(C -> S)	// ¾ÆÀÌÅÛÀ» ±¸ÀÔÇÑ´Ù.

	short		sStoreID;			// »óÁ¡¾ÆÀÌµð
	short		sCount;				// ±¸¸ÅÇÏ´Â ¾ÆÀÌÅÛÀÇ ÃÑ°¹¼ö

	typedef struct _tagBuyItem		// ±¸¸Å ¾ÆÀÌÅÛÀÇ Á¤º¸
	{
		short	sSid;	// Item serial id
		short	sNum;	// Item count
	}BuyItem[sCount];
*/
/*
	(S -> C)	// °á°ú°ªÀ¸·Î ITEM_MOVE_RESULTÀ» ÀÌ¿ëÇÑ´Ù. tType = 3;(ÀÏ¹Ý¾ÆÀÌÅÛ) +
*/

const BYTE	CLIENT_EVENT_SAY		= 134;
/*
	(C <- S)	// HyperTextÀÇ NPC_SAY ½ÇÇàÅëº¸.

	BYTE		result;				// 01	: ¼º°ø
									// else	: ½ÇÆÐ

	short		iSayNum;			// Npc Say Number

	BYTE		tCount;				// "%s"°¡ µé¾î°£ È½¼ö

	typedef struct _tagSay			//
	{
		Varchar	strMsg;				// Message
	}strMsg[tCount];

	// ¼º°øÀÏ °æ¿ì

*/

/*
const BYTE	SELL_ITEM_OPEN_REQ		= 134;

	(C -> S)	// ¾ÆÀÌÅÛÀ» »óÁ¡¿¡ ÆÈ±âÀ§ÇØ ÆÈ¼öÀÖ´Â ¸ñ·Ï°ú °¡°ÝÀ» ¿äÃ»ÇÑ´Ù.

	short		sStoreID;
*/

const BYTE	SELL_ITEM_OPEN_RESULT	= 135;
/*
	(C <- S)	// ÇØ´ç »óÁ¡¿¡¼­ »ç´Â ¸ñ·Ï°ú °¡°ÝÀ» ³»·ÁÁØ´Ù.

	short		sStoreID;			// »óÁ¡ ¾ÆÀÌµð
	short		sCount;				// ÆÈ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÇ ÃÑ°¹¼ö

	// sCount > 0 ÀÎ °æ¿ì¿¡¸¸ Àü¼Û
	typedef struct _tagSellInfo
	{
		short	sSlot;		// ÇØ´ç »óÁ¡¿¡ ÆÈ¼ö ÀÖ´Â À¯ÀúÀÎº¥Åä¸®ÀÇ ½½·Ô ¹øÈ£
		short	sDuration; // ÇöÀç ³»±¸µµ
		DWORD	dwDN;		// °¡°Ý
	}SellInfo[sCount];
*/

const BYTE	SELL_ITEM				= 136;
/*
	(C -> S)	// ¾ÆÀÌÅÛÀ» »óÁ¡¿¡ ÆÇ´Ù.

	short		sStoreID;			// »óÁ¡¾ÆÀÌµð
	short		sCount;				// ÆÈ·Á´Â ¾ÆÀÌÅÛÀÇ ÃÑ°¹¼ö

	typedef struct _tagSellItem		// ±¸¸Å ¾ÆÀÌÅÛÀÇ Á¤º¸
	{
		short	sSlot;	// ÇØ´ç »óÁ¡¿¡ ÆÈ¼ö ÀÖ´Â À¯ÀúÀÎº¥Åä¸®ÀÇ ½½·Ô ¹øÈ£
		short	sNum;	// Item count
	}SellItem[sCount];
*/
/*
	(S -> C)	// °á°ú°ªÀ¸·Î ITEM_MOVE_RESULTÀ» ÀÌ¿ëÇÑ´Ù. tType = 4;(ÀÏ¹Ý¾ÆÀÌÅÛ) +
*/


//const BYTE	EBODY_IDENTIFY_OPEN	= 137;
/*
	(S -> C)	// È®ÀÎÇÒ ¼ö ÀÖ´Â EBody ¸ñ·ÏÀº º¸³½´Ù.

	short		sStoreID;
	short		sCount;				// È®ÀÎÇÒ ¼ö ÀÖ´Â EBodyÀÇ ÃÑ°¹¼ö

	BYTE		tSlot[sCount];		// È®ÀÎÇÒ ½½·Ô Á¤º¸

	// EBody È®ÀÎ °á°ú´Â ITEM_MOVE_RESULT ·Î º¸³½´Ù.
*/

const BYTE	REPAIR_ITEM_OPEN_RESULT	= 138;
/*
	(C <- S)	// ÇØ´ç »óÁ¡¿¡¼­ »ç´Â ¸ñ·Ï°ú °¡°ÝÀ» ³»·ÁÁØ´Ù.

	short		sStoreID;			// »óÁ¡ ¾ÆÀÌµð
	short		tCount;				// ¼ö¸® ÇÒ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÇ ÃÑ°¹¼ö

	// tCount > 0 ÀÎ °æ¿ì¿¡¸¸ Àü¼Û
	typedef struct _tagRepairInfo
	{
		short	sSlot;				// ÇØ´ç »óÁ¡¿¡¼­ ¼ö¸®ÇÒ¼ö ÀÖ´Â À¯Àú ÀÎº¥Åä¸®ÀÇ ½½·Ô ¹øÈ£
		short	sDuration;			// ÇöÀç ³»±¸µµ
		DWORD	dwDN;				// °¡°Ý
	}RepairInfo[tCount];
*/

const BYTE	REPAIR_ITEM				= 139;		
/*
	(C -> S)	// ¼ö¸® ¾ÆÀÌÅÛ¸ñ·ÏÀ» º¸³½´Ù.

	short		sStoreID;			// »óÁ¡¾ÆÀÌµð

	short		tCount;

	typedef struct _tagRepair
	{
		short	sSlot;				// ÇØ´ç »óÁ¡¿¡¼­ ¼ö¸®ÇÒ¼ö ÀÖ´Â À¯Àú ÀÎº¥Åä¸®ÀÇ ½½·Ô ¹øÈ£
	}Repair[tCount];

*/
/*
	(S -> C)	// °á°ú°ªÀ¸·Î ITEM_MOVE_RESULTÀ» ÀÌ¿ëÇÑ´Ù. tType = 5;(ÀÏ¹Ý¾ÆÀÌÅÛ)
*/
const BYTE	BUY_PSY_OPEN_RESULT		= 140;
/*
	(C <- S)	// ÇØ´ç »óÁ¡¿¡¼­ ½ÀµæÇÒ¼öÀÖ´Â ¸ñ·ÏÀ» ³»·ÁÁØ´Ù.

	short		sStoreID;			// »óÁ¡ ¾ÆÀÌµð
	short		sPsiCount;			// ÀÍÈú¼öÀÖ´Â ÃÑ Psi ¼ö

	// sPsiCount > 0 ÀÎ °æ¿ì¿¡¸¸ Àü¼Û
	typedef struct _tagPsiInfo
	{
		short	sSid;				// Psi serial id
	}PsiInfo[sPsiCount];

*/

const BYTE	BUY_PSY_REQ				= 141;
/*
	(C -> S)	// »çÀÌ¿À´Ð ±¸ÀÔÀ» º¸³½´Ù

	short		sStoreID;			// »óÁ¡ ¾ÆÀÌµð
	short		sPsiCount;			// ÃÑ Psi ¼ö

	// sPsiCount > 0 ÀÎ °æ¿ì¿¡¸¸ Àü¼Û
	typedef struct _tagPsiInfo
	{
		short	sSid;				// Psi serial id
	}PsiInfo[sPsiCount];
*/

const BYTE	BUY_PSY_RESULT			= 142;
/*
	(C <- S)	// °á°ú¸¦ ÀÎÁõÇÑ´Ù.

	BYTE		tType;				// 01 : ¼º°ø
									// else : ½ÇÆÐ


	// 00 : ½ÇÆÐÀÌ¸é
									//	error 01 : »óÁ¡ »çÀÌ¿À´ÐÀÌ ÀÚ±â Å¬·¡½º¿Í ¸Â´Â°Ô ¾ø´Ù.
									//	error 02 : ±¸ÀÔ¿¡ ÇÊ¿äÇÑ ·¹º§ÀÌ ³ô´Ù.
									//	error 03 : XP°¡ ¸ðÀÚ¶õ´Ù.
									//	error 04 : DNÀÌ ¸ðÀÚ¶õ´Ù.
									//	error 05 : ÀÍÈú¼ö ÀÖ´Â ÇÑµµ¸¦ ³Ñ¾ú´Ù.
									//	error 06 : ÀÌ¹Ì ½ÀµæÇÑ »çÀÌ¿À´Ð¸¦ ¼±ÅÃÇß´Ù.

	// 01 : ¼º°øÀÌ¸é
		short		sPsiCount;			// ÃÑ Psi ¼ö

		// sPsiCount > 0 ÀÎ °æ¿ì¿¡¸¸ Àü¼Û
		typedef struct _tagPsiInfo
		{
			short	sSid;				// Psi serial id
		}PsiInfo[sPsiCount];
*/

const BYTE	BANK_OPEN				= 143;
/*
	(C <- S)							// ÀºÇà¿¡ º¸°üµÈ ¾ÆÀÌÅÛ°ú µ·¸¦ º¸³½´Ù.

		DWORD		BankDN;				// ÀºÇà¿¡ º¸°üµÈ µ·

		BYTE		TotalSlotNum;		// ÀºÇà¿¡ º¸°üµÈ ¾ÆÀÌÅÛ ½½·ÔÀÇ °¹¼ö

		typedef struct _tagItem
		{
			byte	bySlot;
			short	sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§@@@
			short	sSid;
			short	sDuration;
			short	sBullNum;
			short	sCount;
			BYTE	tMagic[6];
			BYTE	tIQ;
		} ItemInfo[TotalSlotNum];
*/

const BYTE	BANK_ITEM_MOVE_REQ		= 144;
/*
	( C -> S )						// ¾ÆÀÌÅÛ ÀÌµ¿
	BYTE			tType;			// 1 : ÀºÇà¿¡ ¾ÆÀÌÅÛ º¸°ü
									// 2 : ÀºÇà¿¡¼­ ¾ÆÀÌÅÛ Ã£±â
									// 3 : DN ÀÔ±Ý
									// 4 : DN Ãâ±Ý

	// 1 : ÀºÇà¿¡ ¾ÆÀÌÅÛ º¸°ü
		BYTE		tSourceSlot;	// »ç¿ëÀÚ ¾ÆÀÌÅÛÀÌ ÀÖ´ø ½½·Ô À§Ä¡
		short		sCount;

	// 2 : ÀºÇà¿¡¼­ ¾ÆÀÌÅÛ Ã£±â
		BYTE		tSourceSlot;	// ÀºÇà¿¡ ¾ÆÀÌÅÛÀÌ ÀÖ´ø ½½·Ô À§Ä¡
		short		sCount;

	// 3 : DN ÀÔ±Ý
	 	DWORD		dwDN;			// ¼ÒÁö±Ý
	// 4 : DN Ãâ±Ý
		DWORD		dwDN;			// ¼ÒÁö±Ý
*/

const BYTE	BANK_ITEM_MOVE_RESULT	= 145;
/*
	( C <- S )						// ¾ÆÀÌÅÛ ÀÌµ¿
	BYTE			tType;			// 0 : ½ÇÆÐ
									// 1 : ÀºÇà¿¡ ¾ÆÀÌÅÛ º¸°ü
									// 2 : ÀºÇà¿¡¼­ ¾ÆÀÌÅÛ Ã£±â
									// 3 : DN ÀÔ±Ý
									// 4 : DN Ãâ±Ý

	// 1 : ÀºÇà¿¡ ¾ÆÀÌÅÛ º¸°ü
	// 2 : ÀºÇà¿¡¼­ ¾ÆÀÌÅÛ Ã£±â
		typedef struct _tagItem
		{
			byte	bySlot;			// ÀºÇà ÀÎº¥ ½½·Ô ¹øÈ£
			short	sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§@@@@@@@@@@@
			short	sSid;
			short	sDuration;
			short	sBullNum;
			short	sCount;
			BYTE	tMagic[6];
			BYTE	tIQ;
		};

		typedef struct _tagItem
		{
			byte	bySlot;			// »ç¿ëÀÚ ÀÎº¥ ½½·Ô ¹øÈ£
			short	sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§
			short	sSid;
			short	sDuration;
			short	sBullNum;
			short	sCount;
			BYTE	tMagic[6];
			BYTE	tIQ;
		};

	 // 3 : DN ÀÔ±Ý
	 // 4 : DN Ãâ±Ý
		DWORD		dwDN;			// ÀºÇà³» ÀÔ±ÝÇÑ ÃÑ±Ý¾×
		DWORD		dwDN;			// ¼ÒÁöÇÑ ÃÑ±Ý¾×
*/

const BYTE	CURE_RESULT				= 146;
/*
	(C <- S)						// º´¿ø¿¡¼­ È¸º¹°ú »óÅÂÀÌ»ó Ä¡·á °á°ú¸¦ º¸³½´Ù.

	BYTE	tType					// 00 : ½ÇÆÐ
									// 01 : HP, PP, SPÈ¸º¹
									// 02 : »óÅÂÀÌ»ó Ä¡·á
									...
	// 01 : HP, PP, SPÈ¸º¹
	short		sHP;				// HP °ª
	short		sPP;				// PP °ª
	short		sSP;				// SP °ª

	// 02 : »óÅÂÀÌ»ó Ä¡·á
	// ¿¹Á¤...
*/

const BYTE	HELP_REQ				= 147;
/*
	(C -> S)						// ¼­¹ö·Î µµ¿ò¸» ÀÌº¥Æ®¸¦ ¿äÃ»ÇÑ´Ù.
*/

const BYTE	ZONE_CHANGE_RESULT		= 148;
/*
	(C <- S)						// Á¸ Ã¼ÀÎÁö °á°ú°ªÀ» º¸³½´Ù.

	BYTE	tType;					// 00 : ½ÇÆÐ
									// 01 : ¼º°ø
									// 02 : ¼­¹ö¸¦ ¹Ù²ã¼­ Á¸Ã¼ÀÎÁö¸¦ ÇØ¾ßÇÑ´Ù.

	// tType = 00 ÀÏ¶§ ----> Ãß°¡·Î º¸³»´Â ÆÐÅ¶¾øÀ½

	// tType = 01 ÀÏ¶§
	int		uid;					// »ç¿ëÀÚ uid

	short	nPosX;					// X Position
	short	nPosY;					// Y Position
	short	sZone;					// Load Zone Number

	// tType = 02 ÀÏ¶§  (¼­¹ö¸¦ ¹Ù²ã¾ß ÇÏ´Â °æ¿ì)
	Varchar	strAddress;				// Á¢¼ÓÇÒ ¼­¹öÀÇ IP Address
	int		iPort;					// Á¢¼ÓÇÒ ¼­¹öÀÇ Port

*/

const BYTE	EBODY_IDENTIFY_OPEN	= 149;
/*
	(S -> C)	// È®ÀÎÇÒ ¼ö ÀÖ´Â EBody ¸ñ·ÏÀº º¸³½´Ù.

	short		sStoreID;
	short		sCount;				// È®ÀÎÇÒ ¼ö ÀÖ´Â EBodyÀÇ ÃÑ°¹¼ö

	BYTE		tSlot[sCount];		// È®ÀÎÇÒ ½½·Ô Á¤º¸

	// EBody È®ÀÎ °á°ú´Â ITEM_MOVE_RESULT ·Î º¸³½´Ù.
*/

const BYTE EBODY_IDENTIFY_REQ		= 150;
/*
	(C -> S)	// È®ÀÎÇÒ ¾ÆÀÌÅÛ¸ñ·ÏÀ» º¸³½´Ù.

	short		sStoreID;			// »óÁ¡¾ÆÀÌµð
	short		tCount;				// ÃÑ°¹¼ö

	BYTE		tSlot[sCount];		// ÇØ´ç »óÁ¡¿¡¼­ È®ÀÎÇÒ¼ö ÀÖ´Â À¯Àú ÀÎº¥Åä¸®ÀÇ ½½·Ô ¹øÈ£µé
*/

const BYTE UPGRADE_ITEM_DIALOG		= 152;
/*
	(C <- S)						// ¾ÆÀÌÅÛ ¾÷±×·¹ÀÌµå¸¦ ¿øÇÒ¶§¸¸ º¸³½´Ù.

	BYTE	tType;					// ¾ÆÀÌÅÛ ¾÷±×·¹ÀÌµå Å¸ÀÔ
									// 0 : ¹«±â, ¹æ¾î±¸
									// 1 : ¾×¼¼¼­¸®
									// 2 : Ãà ¿þÇÇ¸®½º, Ãà ¾Æ¸Ó¸®½º
									// 3 : ÀÏ¹Ý ¿þÇÇ¸®½º, ÀÏ¹Ý ¾Æ¸Ó¸®½º
									// 4 : ¾ÆÀÌÅÛ °³Á¶
									// 5 : Remagic Request with Event
									// 6 : Remagic Request with Event
									// 7 : EBody Upgrade
*/

const BYTE UPGRADE_ITEM_REQ			= 153;
/*
	(C -> S)						// ¾ÆÀÌÅÛÀ» ¼±ÅÃÇÏ°í µ¿ÀÇ ÇÒ¶§...

	BYTE	tType;					// ¾ÆÀÌÅÛ ¾÷±×·¹ÀÌµå Å¸ÀÔ
									// 0 : ¹«±â, ¹æ¾î±¸
									// 1 : ¾×¼¼¼­¸®
									// 2 : Ãà ¿þÇÇ¸®½º, Ãà ¾Æ¸Ó¸®½º
									// 3 : ÀÏ¹Ý ¿þÇÇ¸®½º, ÀÏ¹Ý ¾Æ¸Ó¸®½º
									// 4 : ¾ÆÀÌÅÛ °³Á¶
									// 5 : Remagic Request with Event
									// 6 : Remagic Request with Event
									// 7 : EBody Upgrade

	short	sSourceSlot;			// ¼±ÅÃÇÑ ¾ÆÀÌÅÛ ½½·Ô¹øÈ£ ( ¾øÀ¸¸é -1 ) 

	short	sItemSlot;				// ¿þÇÇ, ¾Æ¸Ó, ¸·´ë»çÅÁ µî Æ¯¼ö ¾ÆÀÌÅÛÀ¸·Î ¾÷±×·¹ÀÌµå, ¸®¸ÅÁ÷À» ÇÒ¶§

	short   sMaterialSlot1;			// ¸ÅÁ÷¼Ó¼º 1°³ ºÙÀº Àç·á ( ¾øÀ¸¸é -1 )
	short   sMaterialSlot2;			// ¸ÅÁ÷¼Ó¼º 1°³ ºÙÀº Àç·á ( ¾øÀ¸¸é -1 )
	short   sMaterialSlot3;			// ¸ÅÁ÷¼Ó¼º 1°³ ºÙÀº Àç·á ( ¾øÀ¸¸é -1 )
	short   sMaterialSlot4;			// ¸ÅÁ÷¼Ó¼º 1°³ ºÙÀº Àç·á ( ¾øÀ¸¸é -1 )
*/

const BYTE UPGRADE_ITEM_RESULT		= 154;
/*
	(C <- S)						// ¾ÆÀÌÅÛ ¾÷±×·¹ÀÌµå °á°ú¸¦ ¾Ë¸°´Ù.

	BYTE	tType;					// 00 : ½ÇÆÐ
									// 01 : ¼º°ø
									// 02 : °³Á¶¼º°ø
									// 03 : ¸®¸ÅÁ÷ ¼º°ø

	BYTE	tChangedSlotNum;		// º¯È­µÈ ½½·ÔÀÇ °¹¼ö

	typedef struct _tagItem
	{
		byte	bySlot;			// »ç¿ëÀÚ ÀÎº¥ ½½·Ô ¹øÈ£
		short	sLevel;			// ¾ÆÀÌÅÛ ÀåÂø ·¹º§
		short	sSid;
		short	sDuration;
		short	sBullNum;
		short	sCount;
		BYTE	tMagic[6];
		BYTE	tIQ;
	}ChangeItems[tChangedSlotNum];
*/

const BYTE TELEPORT_REQ				= 155;
/*
	(C -> S)						// ÇöÀç À¯Àú°¡ º¸À¯ÇÏ°íÀÖ´Â ±â¾ïÁÂÇ¥°ªÀ» ¿ä±¸
*/

const BYTE TELEPORT_RESULT			= 156;
/*
	(C <- S)

	BYTE	tCount;

		typedef struct _tagTel
		{
			BYTE	tIndex;			// ÀúÀåµÈ ¼ø¼­ 
			Varchar	TelName;		// ±â¾ïÀå¼Ò ÀÌ¸§
			int		nZone;			// Á¸¹øÈ£ //yskang 0.9 handy ¼öÁ¤
		};
*/

const BYTE TELEPORT_EDIT_REQ		= 157;
/*
	(C -> S)						// ÇöÀç À¯Àú°¡ º¸À¯ÇÏ°íÀÖ´Â ±â¾ïÁÂÇ¥Áß ÀÏºÎ°ªÀ» »èÁ¦ ¿ä±¸

	BYTE	tType;					// 1 : ±â¾ïÀå¼Ò Ãß°¡
									// 2 : ±â¾ïÀå¼Ò »èÁ¦

	// 1 : ±â¾ïÀå¼Ò Ãß°¡
	Varchar	TelName;				// ±â¾ïÀå¼Ò ÀÌ¸§

	// 2 : ±â¾ïÀå¼Ò »èÁ¦
  	BYTE	tIndex;					// ÀúÀåµÈ ¼ø¼­
	Varchar	TelName;				// ±â¾ïÀå¼Ò ÀÌ¸§
*/

const BYTE TELEPORT_EDIT_RESULT		= 158;
/*
	(C <- S)

	BYTE	tSuccess;				// 1 : ¼º°ø
									// else ½ÇÆÐ

	// 1 : ¼º°ø
	BYTE	tType;					// 1 : ±â¾ïÀå¼Ò Ãß°¡
									// 2 : ±â¾ïÀå¼Ò »èÁ¦

  	BYTE	tIndex;					// ÀúÀåµÈ ¼ø¼­ 
	Varchar	TelName;				// ±â¾ïÀå¼Ò ÀÌ¸§
	int		nZone;					// Á¸ ¹øÈ£ //yskang 0.9 handy ¼öÁ¤
*/

const BYTE TELEPORT_COPY_REQ		= 159;
/*
	(C -> S)						// ÇØ´ç À¯Àú¿¡°Ô ÀÌµ¿ ¼ö¶ô¿©ºÎ¸¦ ¿ä±¸

	int		uid;					// User Index
	VarChar	strUserName;			// ÇÔ²² ÀÌµ¿À»  ½ÅÃ»ÇÑ À¯ÀúÀÌ¸§

	BYTE	tSlot;					// »ç¿ëÇÏ°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛ
	BYTE	tIndex;					// ÀúÀåµÈ ¼ø¼­
	Varchar	TelName;				// ±â¾ïÀå¼Ò ÀÌ¸§
*/

const BYTE TELEPORT_COPY_DIALOG_REQ	= 160;
/*
	(C <- S)						// ÇØ´ç À¯Àú¿¡°Ô ÀÌµ¿ ¼ö¶ô¿©ºÎ¸¦ ¿ä±¸

	BYTE	tType;					// 1 : ½ÅÃ»ÀÎ
									// else

	int		uid;					// User Index
	VarChar	strUserName;			// ÇÔ²² ÀÌµ¿À»  ½ÅÃ»ÇÑ À¯ÀúÀÌ¸§
*/

const BYTE TELEPORT_COPY_RESULT		= 161;
/*
	(C -> S -> C)					// ÀÌµ¿ ¼ö¶ô ¿©ºÎ¸¦ °áÁ¤

	BYTE	tType;					// ½Â³« : 1
									// °ÅÀý : 2
									// Ãë¼Ò : 3
									// else error

	int		uid;					// User Index
	VarChar	strUserName;			// ÇÔ²² ÀÌµ¿À»  ½ÅÃ»ÇÑ À¯ÀúÀÌ¸§
*/

const BYTE GUARD_BOW				= 162;
/*
	(C <- S)						// °æºñº´ ÀÎ»ç¸¦ ¾Ë¸°´Ù.

	int		uid;					// °æºñº´ uid
	BYTE	tDir;					// º¸°íÀÖ´Â ¹æÇâ
*/

const BYTE EVENT_UPGRADE_ITEM_REQ   = 163;
/*
	(C -> S)						// »óÇ°±Ç ÀÌº¥Æ®¸¦ È¸¼öÇÑ´Ù.

	BYTE	tEventSlot;				// ÀÌº¥Æ® »óÇ°±Ç ½½·Ô
	BYTE	tItemSlot;				// ¾÷±ÛÇÏ±âÀ§ÇÑ ÇØ´ç ½½·Ô
*/

//#################################################################
//	Guild		(170 ~ 200)
//#################################################################
const BYTE  GUILD_DIALOG			= 170;
/*
	(C <- S)						// ±æµå Ã¢¸³ Á¶°Ç¿¡ ¸ÂÀ»¶§¸¸ º¸³½´Ù.
*/

const BYTE GUILD_OPEN_REQ			= 171;
/*
	(C -> S)						// ±æµå Ã¢¼³ ¹®ÀÇ

	Varchar		strGuildName;		// ±æµå ÀÌ¸§
*/

const BYTE GUILD_OPEN_RESULT		= 172;
/*
	(C <- S)						// ±æµå Ã¢¼³ °á°ú°ªÀ» º¸³½´Ù.

	BYTE		tTYpe;				// 00 : ½ÇÆÐ
									// 01 : µî·Ï ¼º°ø

	// 00 : ½ÇÆÐ
	BYTE		err					//	01 : Á¦ÇÑ ±æÀÌ¸¦ ³Ñ´Â ±æµå¸íÄª
									//	02 : ±æµå Ã¢¼³ ·¹º§ÀÌ ¸ðÀÚ¶÷
									//	03 : ±æµå Ã¢¼³ ÀÚ±ÝÀÌ ¸ðÀÚ¶÷
									//	04 : ÀÌ¹Ì µî·ÏÇÑ ±æµå ¸íÄªÀÓ
									//	05 : ´Ù¸¥ ±æµåÀÇ ±æµå¿øÀÌ´Ù.
									//	255 : UnKnown Error
	// 01 : µî·Ï ¼º°ø
	int			iSid;				// ±æµå ÀÎµ¦½º
	VarChar		szGuildName;		// ±æµå ÀÌ¸§
*/

const BYTE GUILD_ADD_REQ			= 173;
/*
	(C -> S)						// »õ·Î¿î ±æµå°¡ Ãß°¡µÇ¾î Å¬¶óÀÌ¾ðÆ® ÆÄÀÏ¿¡ Ãß°¡ ÇÊ¿ä.

	int		iGuildID;				// Guild ID
*/

const BYTE GUILD_ADD_RESULT			= 174;
/*
	(C <- S)						// ÇØ´ç ¿äÃ»À» º¸³»ÁØ´Ù.

	BYTE		tType;				// 00 : ½ÇÆÐ
									// 01 : ¼º°ø

	// 00 : ½ÇÆÐ
	BYTE		err					// 01 : ÇØ´ç ±æµå°¡ ¾ø´Ù
									// 255 : UnKnown Error
	// 01 : ÇØ´ç ±æµå Á¤º¸
	int			iSid;				// ±æµå ÀÎµ¦½º
	Varchar		strGuildName;		// ±æµå ÀÌ¸§
	short		sVersion;			// ±æµå¹®¾ç ¹öÁ¯
	VarBinary	GuildMark;			// ±æµå ¹®¾ç

*/

const BYTE GUILD_MARK_ADD_REQ		= 175;
/*
	(C -> S)						// »õ·Î¿î ±æµå ¹®¾ç¸¦ Ãß°¡ÇÑ´Ù.

	VarBinary	GuildMark;			// ±æµå ¹®¾ç
*/

const BYTE GUILD_MARK_ADD_RESULT	= 176;
/*
	(C <- S)						// ÇØ´ç ¿äÃ»À» º¸³»ÁØ´Ù.

	BYTE		tType;				// 00 : ±ÇÇÑÀÌ ¾ø´Ù.
									// 01 : ¼º°ø

	// 00 : ½ÇÆÐ
	BYTE		err					// 01 : ±æµåÂ¯ ±ÇÇÑÀÌ ¾ø´Ù.
									// 02 : ±æµå°¡ ¾ø´Ù.

	// 01 : µî·Ï ¼º°ø
	short		sVersion;
*/

const BYTE	GUILD_WAREHOUSE_OPEN	= 177;
/*
	(C <- S)							// ±æµåÃ¢°í¿¡ º¸°üµÈ ¾ÆÀÌÅÛ°ú µ·¸¦ º¸³½´Ù.

		DWORD		GuildDN;				// ±æµå Ã¢°í¿¡ º¸°üµÈ µ·

		BYTE		TotalSlotNum;		// ±æµåÃ¢°í¿¡ º¸°üµÈ ¾ÆÀÌÅÛ ½½·ÔÀÇ °¹¼ö

		typedef struct _tagItem
		{
			byte	bySlot;
			short	sLevel;				// ¾ÆÀÌÅÛ ÀåÂø ·¹º§@@@
			short	sSid;
			short	sDuration;
			short	sBullNum;
			short	sCount;
			BYTE	tMagic[6];
			BYTE	tIQ;
		} ItemInfo[TotalSlotNum];
*/

const BYTE	GUILD_ITEM_MOVE_REQ		= 178;
/*
	( C -> S )						// ¾ÆÀÌÅÛ ÀÌµ¿
	BYTE			tType;			// 1 : ±æµå Ã¢°í¿¡ ¾ÆÀÌÅÛ º¸°ü
									// 2 : ±æµå Ã¢°í¿¡¼­ ¾ÆÀÌÅÛ Ã£±â
									// 3 : DN ÀÔ±Ý
									// 4 : DN Ãâ±Ý

	// 1 : ±æµå Ã¢°í¿¡ ¾ÆÀÌÅÛ º¸°ü
		BYTE		tSourceSlot;	// »ç¿ëÀÚ ¾ÆÀÌÅÛÀÌ ÀÖ´ø ½½·Ô À§Ä¡
		short		sCount;

	// 2 : ±æµå Ã¢°í¿¡¼­ ¾ÆÀÌÅÛ Ã£±â
		BYTE		tSourceSlot;	// ±æµå Ã¢°í¿¡ ¾ÆÀÌÅÛÀÌ ÀÖ´ø ½½·Ô À§Ä¡
		short		sCount;

	// 3 : DN ÀÔ±Ý
	 	DWORD		dwDN;			// ¼ÒÁö±Ý
	// 4 : DN Ãâ±Ý
		DWORD		dwDN;			// ¼ÒÁö±Ý
*/

const BYTE	GUILD_ITEM_MOVE_RESULT	= 179;


const BYTE GUILD_INFO				= 180;	//@@@@@@@@@@@@ ÀÚ¸®¸¦ Info¿¡ ³Ö¾î¾ßÇÑ´Ù.


const BYTE GUILD_USER_INFO_REQ		= 181;
/*
	(C -> S)						// ÇöÀç Á¢¼ÓÁßÀÎ ±æµå¿ø Á¤º¸¸¦ ¿ä±¸ÇÑ´Ù.
*/

const BYTE GUILD_USER_INFO_RESULT	= 182;
/*
	(C <- S)						// ±æµå ¼Ò¼Ó¿ø Á¤º¸¸¦ º¸³½´Ù.

	BYTE		tType;				// 1 : ±æµå ÇÏ¿ì½º ÀÎ¿ø ½ÅÃ» Á¤º¸
									// 2 : ±æµå ¸¶½ºÅÍ°¡ º¸´Â ±æµå Á¤º¸
									// 3 : Æ÷Æ®¸®½º ±æ¸¶°¡ ±æµå¿ø ¼ÒÈ¯
									// else ÀÏ¹Ý ±æµå Á¤º¸

	// else ÀÏ¹Ý ±æµå Á¤º¸
	VarChar		strGuildMaster;		// ±æµå ¸¶½ºÅÍÀÌ¸§

	BYTE		tCount;				// ±æµå¿ø ¼ö

	typedef struct _tagGuild
	{
		Varchar		strGuilUser;	// ±æµå¿ø ÀÌ¸§
		short		sLevel;			// ·¹º§
	}Guild[tCount];

	// 1 : ±æµå ÇÏ¿ì½º ÀÎ¿ø ½ÅÃ» Á¤º¸
	VarChar		strGuildMaster;		// ±æµå ¸¶½ºÅÍÀÌ¸§
	BYTE		tCount;				// ±æµå¿ø ¼ö

	typedef struct _tagGuild
	{
		Varchar		strGuilUser;	// ±æµå¿ø ÀÌ¸§
		short		sLevel;			// ·¹º§
		int			uid;			// ±æµå À¯Àú

	}Guild[tCount];

  	short		sGuildHouseZone;	// Vertual Room Index
*/

const BYTE GUILD_WAREHOUSE_CLOSE	= 183;
/*	(C -> S)						// ±æµåÃ¢°í Ã¢À» ´Ý´Â´Ù°í Åëº¸ÇÑ´Ù.
*/

const BYTE GUILD_TAX_OPEN			= 184;
/*
	(C <- S)						// ±æµå»óÁ¡¿¡¼­ ¼¼±Ý Á¶Á¤ ¹®ÀÇ

	short	sStoreID;				// ½ÅÃ»ÇÏ·Á´Â »óÁ¡ ÀÎµ¦½º
	short	sCurTax;				// ÇöÀç ¼¼±Ý..
*/

const BYTE GUILD_TAX_REQ			= 185;
/*
	(C -> S)						// ±æµå»óÁ¡¿¡¼­ ¼¼±Ý Á¶Á¤

	short	sStoreID;
	short	sTax;					// 0 ~ 200%±îÁö
*/

const BYTE GUILD_WAR_OPEN			= 186;
/*
	(C <- S)						//

	short	sStoreID;				// ½ÅÃ»ÇÏ·Á´Â »óÁ¡ ÀÎµ¦½º
	short	sYear;					// ÇöÀç ¼­¹ö ½Ã°£À» ±âÁØÀ¸·Î ¸îÀÏÈÄ...
	BYTE	tMon;
	BYTE	tDay;
*/

const BYTE GUILD_WAR_REQ			= 187;
/*
	(C -> S)						// ±æµåÀü ³¯Â¥¸¦ °áÁ¤

	short	sStoreID;
	short	sYear;					//
	BYTE	tMon;
	BYTE	tDay;
	BYTE	tHour
*/

const BYTE GUILD_WAR_APPLY_OPEN		= 188;
/*
	(C <- S)						// ±æµåÀüÀ» ½ÅÃ»ÇÑ´Ù.

	short	sStoreID;
	VarChar	strGuildName;			// »ó´ë ±æµåÀÌ¸§
*/

const BYTE GUILD_WAR_APPLY_REQ		= 189;
/*
	(C -> S)						// ±æµåÀüÀ» ½ÅÃ»ÇÑ´Ù.

	short	sStoreID;
*/

const BYTE GUILD_WAR				= 190;
/*
	(C <- S)						// ±æµåÀü ½ÃÀÛÀ» ¾Ë¸°´Ù.

	BYTE	tType;					// 1 : ±æµåÀü ½ÃÀÛ..
									// 2 : ±æµåÀü Á¾·á...
									// 3 : ±æµåÀüÁß Á×¾î¼­ ¸¶À»·Î ºÎÈ°ÇÒ¶§...

	BYTE	tGuildWarType;			// 1 : ÇÊµåÀü
									// 2 : »óÁ¡Àü
									// 3 : °ø¼ºÀü

	short	sAreaIndex;				// ÀüÀïÁö¿ª(ÇÊµåÀüÀº = 0)

	// 1 : ±æµåÀü Á¾·á½Ã¿¡´Â ³»·Á°¡Áö ¾Ê´Â´Ù.
	TYPE	tGuildUser;				// 0 : ÀÏ¹ÝÀ¯Àú
									// 1 : ÇØ´ç ±æµå¿ø À¯Àú

	// 1 : ÇØ´ç ±æµå¿øÀÌ ±æµåÀü ½ÃÀÛÀÏ¶§...
	short	sCount;

	typedef struct _GuildList
	{
		int	iGuildID;				// ±æµåÀü¿¡ Âü°¡ÇÑ ¸®½ºÆ®

	}GuildList[sCount];

*/

const BYTE GUILD_FIELD_WAR_REQ		= 191;
/*
	(C -> S -> C)					// ÇØ´ç ±æ¸¶¿¡°Ô ÇÊµåÀü ¼ö¶ô¿©ºÎ¸¦ ¿ä±¸

	int		iGuildID;
	VarChar	strGuildName;			// ÇÊµåÀüÀ» ½ÅÃ»ÇÑ ±æµåÀÌ¸§
*/

const BYTE GUILD_FIELD_WAR_RESULT	= 192;
/*
	(C -> S -> C)						// ÇÊµåÀü ¼ö¶ô ¿©ºÎ¸¦ °áÁ¤

	BYTE	tType;					// ½Â³« : 1
									// °ÅÀý : 2
*/

const BYTE SIGHT_INFO_COMPRESSED	= 193;
/*
	²À ½á¾ßÇà?
*/

const BYTE GUILD_HOUSE_APPLY_REQ	= 194;
/*
	(C -> S)						//

	short	sCount;

	typedef struct _UserList
	{
		int uid;					// ±æµå ÇÏ¿ì½ººÐ¾ç¿¡ Âü°¡ÇÑ ¸®½ºÆ®

	}UserList[sCount];

  	short	sGuildHouseZone;		// Vertual Room Index
*/

const BYTE GUILD_FORTRESS_NCIRCLE	= 195;
/*
	(C <- S)						// °ø¼ºÀüÁß ÇØ´ç N_CircleÀÇ »öÀÌ ¹Ù²ñÀ» ¾Ë·Á..

	BYTE	tType;					// 0 : N_CircleÀÇ »ö º¯È­
									// 1 : ÇØ´ç Æ÷Æ®¸®½º ±æµå Á¡·É

	// 0 : N_CircleÀÇ »ö º¯È­
	int		Uid;
	BYTE	tColor;					// 0 : defalut(ÆÄ¶û)
									// 1 : Non_default(»¡°­);

	// 1 : ÇØ´ç Æ÷Æ®¸®½º ±æµå Á¡·É
	int		FortressUid;

	int		Uid1;
	int		Uid2;
	int		Uid3;
	int		Uid4;
*/

const BYTE DEL_GUILD_CHAR_REQ		= 196;
/*
	(S <- S)						//
*/

const BYTE GUILD_INVITE_USER_REQ	= 197;
/*
	(S <- S)						//
*/

const BYTE CHALLENGE_GUILD			= 198;
/*
	(C <- S)						// °ø¼ºÀüÀ» ½ÅÃ»ÇÑ ±æµåÀÌ¸§À» º¸³½´Ù.

	short	sCount;

	typedef	struct	_GUILD
	{
		Varchar	strGuildName;
	}GuildInfo[sCount];
*/
const BYTE GUILD_OFF_WITH_THREAD	= 199;		// ±æµå Å»Åð

const BYTE	NEW_CHAR_REQ_WITH_THREAD = 200;

const BYTE ADDRESS_WINDOW_OPEN		= 201;		// ÁÖ¼ÒÀÔ·ÂÃ¢

const BYTE AREA_ATTACK_RESULT		= 202;		// ¹üÀ§ °ø°Ý

/*
const BYTE GUILD_WAR_AREA			= 203;		// ¹üÀ§ °ø°Ý
/*
	(C <- S)						// ±æµåÀü Áö¿ªÀÓÀ» ¾Ë¸°´Ù.

	BYTE	tType;					// 1 : ±æµåÀüÀÌ ½ÃÀÛÇÏ´Â Àå¼ÒÁ¸Àç
									// 2 : ±æµåÀüÀÌ Á¾·áÇÏ´Â Àå¼ÒÁ¸Àç
    short	sCount;

	typedef	struct	_GUILD_AREA
	{
		short	Guild_Area_Index;
	}GuildArea[sCount];

*/

const BYTE DRESSING					= 204;		// ¼±±¤
/*
	(C -> S)
	
	(S -> C)

	BYTE	success/fail

	<success>
	short	slot					// Àç·á°¡ ÀÖ´ø ½½·Ô
	short	sLevel
	short	sSid
	short	sDuration
	short	sBullNum
	short	sCount
	BYTE	tMagic 6°³
	BYTE	tIQ

	<fail>
	none...

*/

const BYTE DRESSING_WINDOW_OPEN		= 205;		// ¼±±¤ Ã¢ ¿­±â

const BYTE ACCOUNT_BANK_OPEN		= 206;		// ÀºÇà ¿­±â
/*
	(C -> S)

	none...

	(S -> C)

	byte	ACCOUNT_BANK_OPEN;
	short	store_id;				// »óÁ¡ ¹øÈ£ - Ã¢°í ÀÌ¿ë½Ã ¼­¹ö·Î ¸®ÅÏ ½ÃÄÑ ÁÖ¾î¾ß ÇÔ
	dword	m_dwAccountBankDN;
	byte	count;

	;;count¸¸Å­
	{
		byte	slot;
		short	sLevel;
		short	sSid;
		short	sDuration;
		short	sBullNum;
		short	sCount;

		6°³
		{
			byte	tMagic;
		}

		byte	tIQ;

		DWORD	dwCost;				// ¹°°Ç °¢°¢ÀÇ Ã£À»¶§ ¼Ò¿äµÇ´Â ºñ¿ë
	}
*/

const BYTE ACCOUNT_BANK_ITEM_MOVE_RESULT = 207;
/*
	(S -> C)

	BYTE	ACCOUNT_BANK_ITEM_MOVE_RESULT;

	BYTE	result;

	
	if(result == 0)
	{
		return;		// fail
	}
	else if(result == 1 )
	{
		ÀºÇà¿¡ ¾ÆÀÌÅÛ ³Ö±â

		BYTE	bankslot;
		short	sLevel;
		short	sSid;
		short	sDuration;
		short	sBullNum;
		short	sCount;

		BYTE	tMagic * 6°³
		BYTE	tIQ;

		byte	userslot;
		short	sLevel;
		short	sSid;
		short	sDuration;
		short	sBullNum;
		short	sCount;

		byte	tMagic;
		byte	tIQ;
	}
	else if(result == 2 )
	{
		ÀºÇà¿¡¼­ ¾ÆÀÌÅÛ »©±â

		DWORD	m_dwDN
		DWORD	m_dwAccountBankDN

		short	bankchangeslotcount		// ¾ÆÀÌÅÛ»©±â·Î ÀºÇà¿¡¼­ º¯ÇÑ ½½·Ô °³¼ö

		bankchangeslotcount ¸¸Å­
		{
			BYTE	bankslot			// ¹ðÅ©¿¡¼­ º¯ÇÑ ½½·Ô¹øÈ£
			short	sLevel
			short	sSid
			short	sDuration
			short	sBullNum
			short	sCount
			
			BYTE	tMagic * 6°³
			BYTE	tIQ
		}

		short	myinvenchangecount		// ¾ÆÀÌÅÛÀ» ÀºÇà¿¡¼­ Ã£À¸¹Ç·Î¼­ ¹Ù²ï ³» ÀÎº¥ ½½·Ô °³¼ö

		myinvenchangecount ¸¸Å­
		{
			BYTE	myinvenslot			// ÀÎº¥¿¡¼­ º¯ÇÑ ½½·Ô¹øÈ£
			short	sLevel
			short	sSid
			short	sDuration
			short	sBullNum
			short	sCount
			
			BYTE	tMagic * 6°³
			BYTE	tIQ
		}
	}
	else if(result == 3 )
	{
		ÀºÇà¿¡ µ·³Ö±â

		DWORD	m_dwAccountBankDN		// ÀºÇà³» ÀÔ±ÝÇÑ ÃÑ±Ý¾×
		DWORD	m_dwDN					// ¼ÒÁöÇÑ ÃÑ±Ý¾×
	}
*/
const BYTE SELECT_OVER100_SKILL		= 208;
/*
	(C -> S)

	BYTE	selectedskill (20-21)
*/

const BYTE ACCOUNT_BANK_ITEM_MOVE_REQ = 209;
/*
	(C -> S)

	BYTE	type

	switch(type)
	{
	case BANK_ITEM_INPUT:
	{
		BYTE	tSourceSlot							// »ç¿ëÀÚ ¾ÆÀÌÅÛÀÌ ÀÖ´ø ½½·Ô À§Ä¡
		short	sCount								// ¾ó¸¶¸¸Å­ º¸°ü
	}
	case BANK_ITEM_OUTPUT:
	{
		short	store_id
		DWORD	outputdn							// ÀºÇà¿¡¼­ Ã£À» µ·
		short	slotcount							// ¸î°³ÀÇ ½½·Ô¿¡¼­ Ã£À» °ÇÁö

		slotcount¸¸Å­
		{
			short	slot							// ÀºÇà¿¡¼­ ½½·Ô ¹øÈ£
			short	count							// ¸î°³¸¦ Ã£À» °ÇÁö
		}
	}
	case BANK_DN_INPUT:
	{
		DWORD	InputDN								// ÀºÇà¿¡ ÀÔ±ÝÇÒ µ·
	}
*/

const BYTE PRE_REG_USER_EVENT_REQ	= 210;
/*
	(S -> C)										// ÇØ´ç Ã¢À» ¿¬´Ù

	(C -> S)										// ÀÔ·ÂµÈ°ªÀ» º¸³½´Ù.
	Varchar		strSerialNum;
	
*/

const BYTE PRE_REG_GAMEROOM_EVENT_REQ= 211;
/*
	(S -> C)										// ÇØ´ç Ã¢À» ¿¬´Ù

	(C -> S)										// ÀÔ·ÂµÈ°ªÀ» º¸³½´Ù.
	Varchar		strSerialNum;
	Varchar		strSocNum;							// µî·ÏÇÑ »ç¾÷ÁÖÀÇ ÁÖ¹Î¹øÈ£
	
*/

const BYTE ADDRESS_WINDOW_OPEN_NEW		= 212;			// ÁÖ¼ÒÀÔ·ÂÃ¢


// Áß±¹¾î ¹öÁ¯¿¡¼­ ÇÊ¿äÇÑ ÆÐÅ¶. 
const BYTE ENCRYPTION_START_REQ			= 213;
/*
	(C -> S)										// Before AccountLogin, Encryption Public_Key request;	

	BYTE	tType;									// Zone ServerÀÎÁö CharInfo¼­¹ö¿¡¼­ Á¢¼ÓÇÏ´Â°ÇÁö Client¿¡¼­ ±¸ºÐ ÇÊ¿ä
*/
const BYTE ENCRYPTION_START_RESULT		= 214;
/*
	(S -> C)

    Varchar		public_key;		// int64

	BYTE		tType;			//					0 : CharInfoServer, 1 : GameStartReq, 2 : ZonLoginReq
*/

const BYTE GUILD_INVITE_REQ				= 215;
/*
	(C -> S -> C)									// ±æµå °¡ÀÔ ¿©ºÎ¸¦ ¹°¾îº»´Ù.

	int			uid;								// ±æµå ¸¶½ºÅÍ
*/

const BYTE GUILD_INVITE_RESULT			= 216;
/*
	(C -> S -> C)									// ¼ö¶ô ¿©ºÎ¸¦ °áÁ¤

	BYTE	tType;									// ½Â³« : 1
													// °ÅÀý : °ÅÀý´çÇÏ´Â ÇÇÁÖÃ¼ 2
													// °ÅÀý : ÁÖÃ¼ÀÎ °æ¿ì 3
													// ¼­¹ö °ÅÀý : 4

	int			uid;								// »ó´ë¹æ uid

	// ¼­¹ö°¡ °ÅÀýÀÌ¸é  : 4
	BYTE		error_code;			// 03	: »ó´ë ¾øÀ½ (±Ó¸», »ç¶÷Ã£±âÀÇ °æ¿ì)
									// 06	: Àß¸øµÈ À¯Àú¾ÆÀÌµð(±Ó¸», »ç¶÷Ã£±âÀÇ °æ¿ì)
									// 07	: ÇØ´ç ±æµå°¡ Á¸ÀçÇÏÁö ¾Ê´Â´Ù.
									// 08	: ÇØ´ç ±æµå¿¡ °¡ÀÔÇÑ À¯Àú°¡ ¾Æ´Ï´Ù.
									// 09	: ÀÌ¹Ì ´Ù¸¥ ±æµå¿¡ °¡ÀÔÇÑ À¯Àú´Ù.
									// 10	: ±æµå Â¯	±ÇÇÑÀÌ ¾ø´Ù.
									// 11	: ÃÖ´ë ÀÎ¿ø¼ö¸¦ ³Ñ¾ú´Ù.
									// 12	: »ó´ë±æµå°¡ ÀüÀïÁßÀÌ´Ù.

*/

/////////////////////////////

//-----------------------------------------------------------------------------------------------------
//--yskang 0.1
//±æµå ±â´ÉÁß È£ÄªÀ» ºÎ¿©ÇÏ±â À§ÇÏ¿© Ãß°¡µÇ´Â ÆÐÅ¶
//-----------------------------------------------------------------------------------------------------
const BYTE LOVE_NAME_REQ		= 217;		//±æµå¿ø È£Äª[¾ÖÄª] Ãß°¡ ¶Ç´Â »èÁ¦ ÀÛ¾÷À» ÇÑ´Ù.
/*											//ÀÚ½ÅÀÇ È£Äª[¾ÖÄª] Ãß°¡ ¶Ç´Â »èÁ¦ ÀÛ¾÷À» ÇÑ´Ù.										
	(C -> S)
	BYTE	bCommand;				//1 ÀúÀå 0 »èÁ¦ 
	// 1 : ±æµå¿ø È£Äª[¾ÖÄª] ÀúÀå	//<±æ¸¶°¡ 50ÀÌÇÏ LevelÀÌ¸é º¸³»Áö ¾Ê´Â´Ù,ÀÚ½ÅÀÌ¸é 80ÀÌÇÏ...>
	int		uid;					//±æµå¿ø uid ¶Ç´Â ÀÚ½ÅÀÇ uid
	Varchar LoveName;				//È£Äª[¾ÖÄª]
	// 0 : »èÁ¦
	int		uid;
*/


const BYTE LOVE_NAME_RESULT		= 217;
/*
	(C <- S)

	BYTE	tSuccess;				// 1 : ¼º°ø
									// 0 : ½ÇÆÐ
	// 0 : ½ÇÆÐ
	BYTE	error_code;
	01 : ±ÇÇÑÀÌ ¾ø´Ù.
	02 : Ã£´Â ¾ÆÀÌµð°¡ ¾ø´Ù.
	03 : °°Àº ±æµå¿øÀÌ ¾Æ´Ï´Ù.
	04 : È£Äª[¾ÖÄª]ÀÌ Àß¸øµÇ¾ú´Ù.
	05 : µðºñ¿¡ ÀúÀå ÇÒ ¼ö ¾ø¾ú´Ù.
	06 : ¿åÀÌ´Ù.
*/
const BYTE LOVE_NAME			= 218; //µî·Ï½Ã °°Àº È­¸é¿¡ ÀÖ´Â »ç¶÷¿¡°Ô Àü´ÞÇÑ´Ù.
/*
	(C <- S)
	int		uid;
	varchar LoveName;
*/

//-----------------------------------------------------------------------------------------------------
// zinee 2002-11-07
// ¸ÞÀÏ ½Ã½ºÅÛ ÆÐÅ¶
//-----------------------------------------------------------------------------------------------------

const BYTE MAIL_INFO_REQ			= 219;	// Å¬¶óÀÌ¾ðÆ®¿¡¼­ ¸ÞÀÏ Á¤º¸ ¿äÃ»
/*
	(C -> S)
*/
const BYTE MAIL_INFO_RESULT			= 219;
/*
	(S -> C)
	int		nNewMail			// »õ ¸ÞÀÏ °¹¼ö
	int		nCurMail			// ÇöÀç ¸ÞÀÏ °¹¼ö
*/

const BYTE MAIL_SEND_REQ			= 220;	// Å¬¶óÀÌ¾ðÆ®¿¡¼­ ¸ÞÀÏ º¸³»±â ¿äÃ»
/*
	(C -> S)
	varchar		strRecvID		// ¼ö½ÅÀÚ ¾ÆÀÌµð
	varData		Contents		// ¸ÞÀÏ³»¿ë
*/
const BYTE MAIL_SEND_RESULT			= 220;
/*
	(S -> C)
	BYTE	error
	// 0 : ¼º°ø
	// 1 : ¿ìÇ¥¾øÀ½
	// 2 : Á¸ÀçÇÏÁö ¾Ê´Â ¾ÆÀÌµð
	// 3 : ¸ÞÀÏ ½Ã½ºÅÛ °úºÎÇÏ


	// ¼º°ø½Ã.
	typedef struct _tagMailItem		// ¿ìÇ¥ ¾ÆÀÌÅÛÀÇ Á¤º¸
	{
		short	sSlot;	// Item Slot number
		short	sNum;	// Item count
	};
*/
const BYTE MAIL_LIST_REQ			= 221;
/*
	(C -> S)
	int		nIndex		// ÀÐ°íÀÚ ÇÏ´Â ¸ÞÀÏÁ¤º¸ÀÇ ½ÃÀÛ¹øÈ£ (-1ÀÏ°æ¿ì Ã³À½)
*/
const BYTE MAIL_LIST_RESULT			= 221;
/*
	(S -> C)
	short	nCount			// ¸®½ºÆ® °¹¼ö( Á¸Àç½Ã ÇÑ¹ø¿äÃ»¿¡ ±âº» 20°³ )

	Data					// nCount °¹¼ö¸¸Å­ ¹Ýº¹ (ÀÎµ¦½º,º¸³½¾ÆÀÌµð,³¯Â¥,ÀÐÀ½¿©ºÎ)
	{
		int		nIndex
		varchar	strSendID
		varchar	Date		// ³¯Â¥Çü½Ä 02/11/23 14:34
		BYTE	bRead		// 0: »õÆíÁö 1: ÀÐÀº ÆíÁö
	}
*/
const BYTE MAIL_CONTENTS_REQ		= 222;
/*
	(C -> S)
	int		nIndex			// º¸°íÀÚ ÇÏ´Â ¸ÞÀÏÀÇ °íÀ¯¹øÈ£
*/
const BYTE MAIL_CONTENTS_RESULT		= 222;
/*
	(S -> C)
	BYTE	error
							// 0: ¼º°ø
							// 1: Ã£´ÂÆíÁö¾øÀ½
							// 2: ¿Ã¹Ù¸£Áö ¾ÊÀº ¿äÃ»(ÀÚ±âÆíÁö ¾Æ´Ô)

	Data					// ¿¡·¯°¡ ¾øÀ»°æ¿ì(error==0) µû¶ó¿À´Â µ¥ÀÌÅÍ
	{
		int		nIndex
		vardata	Contents
	}
*/
const BYTE MAIL_DELETE_REQ			= 223;
/*
	(C -> S)
	int		nIndex				// »èÁ¦¸ÞÀÏ ¹øÈ£
	
	int		nLastIndex			// ¸Ç¸¶Áö¸· Data¿äÃ» ÀÎµ¦½º.
*/
const BYTE MAIL_DELETE_RESULT		= 223;
/*
	int		error
								// 0: ¼º°ø
								// 1: Á¸ÀçÇÏÁö ¾Ê´Â ÆíÁö
								// 2: ¿Ã¹Ù¸£Áö ¾ÊÀº »èÁ¦( ÀÚ±â¸ÞÀÏÀÌ ¾Æ´Ò°æ¿ì )

	// ¼º°øÇßÀ»¶§.
	int		nIndex				// ¸ÞÀÏ ¹øÈ£

	int		nNewMail			// »õ ¸ÞÀÏ °¹¼ö
	int		nCurMail			// ÇöÀç ¸ÞÀÏ °¹¼ö

	int		nCount			// ¸¶Áö¸· ÆíÁö°¹¼ö ¾øÀ¸¸é 0 ÀÖÀ¸¸é 1
	Data					// 
	{
		int		nIndex		// DELETE_REQÀÇ  nLastIndex¿¡ ´ëÇÑ °á°ú Index
		varchar	strSendID
		varchar	Date		// ³¯Â¥Çü½Ä 02/11/23 14:34
		BYTE	bRead		// 0: »õÆíÁö 1: ÀÐÀº ÆíÁö
	}
*/

const BYTE REVIVAL_REQ				= 224;
/*
	(S -> C -> S)

	varchar strUserID		// ºÎÈ°½ÃÄÑÁÖ´Â À¯Àú ¾ÆÀÌµð

  	// ¼º°ø½Ã °á°ú°ªÀ» USER_INFO·Î ¾Ë·ÁÁØ´Ù. LIVE_REQ ÀÇ °á°ú°ª°ú µ¿ÀÏ.
	// ½ÇÆÐ½Ã REVIVAL_ERROR ÆÐÅ¶À» Àü¼ÛÇÑ´Ù.

*/

const BYTE REVIVAL_RESULT			= 225;
/*
	(S -> C)

	// Å¬¶óÀÌ¾ðÆ®ÀÇ REVIVAL_REQ ¿äÃ»¿¡ ½ÇÆÐÇÒ °æ¿ì¿¡¸¸ Àü¼ÛµÇ´Â ÆÐÅ¶

	BYTE	error			// 0: ¼º°ø -> ¼º°ø½Ã USER_INFO ÆÐÅ¶ÀÌ µû·Î µÚµû¶ó°£´Ù.
							// 1: ºÎÈ°ÇØÁÖ´Â À¯Àú°¡ ¾øÀ½(Á¢¼ÓÀÌ ²÷°å°Å³ª ±âÅ¸...ÀÌÀ¯·Î)
							// 2: ºÎÈ°ÇØÁÖ´Â À¯ÀúÀÇ ÀÎº¥¿¡ ¸®¹ö½º¸®¿Â ¾øÀ½.(´Ù¸¥ À¯ÀúºÎÈ°¿¡ »ç¿ëµÆ´Ù°Å³ª..)
							// 3: ¾Ë¼ö¾ø´Â¿¡·¯( À¯Àú°¡ ºÎÈ°ÇÒ Àå¼Ò¸¦ ¸øÃ£À» °æ¿ì µîµî.. )

	/////////////////////////////////////////////////////////////////////////////////////
	// error = 0  : ¼º°øÇÒ °æ¿ì USER_INFOÀÇ ÆÐÅ¶°ú Mode¸¦ Á¦¿ÜÇÑ µ¥ÀÌÅÍ°¡ µÚµû¶ó°£´Ù.

		int			uid;			// User index

		// Mode °¡ INFO_MODIFY ÀÎ °æ¿ì¸¸ ¾Æ·¡ÀÇ µ¥ÀÌÅÍ Àü¼Û
		VarChar		szName;			// User Name
		short		nPosX;			// X Position
		short		nPosY;			// Y Position

		DWORD		dwSkin;			// ÇÇºÎ»ö(COLORREF)
		DWORD		dwHair;			// ¸Ó¸®»ö(COLORREF)
		BYTE		byGender;		// ³², ¿©

		char		szFace[10];		// ¾ó±¼¸ð¾ç
									// 1 : ¸Ó¸®
									// 2 : ¾ó±¼
									// 3 : ´«
									// 4 : ÄÚ
									// 5 : ÀÔ
									// 6 : ¾Ç¼¼»ç¸® 1
									// 7 : ¾Ç¼¼»ç¸® 2
									// 8~10 : ¿¹ºñ

		short		sEquipItem[10];	// 0 = Çï¸ä
									// 1 = °©¿Ê
									// 2 = ¸Ó¸® ¾×¼¼¼­¸®
									// 3 = ¸ö ¾×¼¼¼­¸®
									// 4 = ¿À¸¥ÂÊ ¼Õ ¹«±â
									// 5 = ¿ÞÂÊ   ¼Õ ¹«±â
									// 6 = ¿À¸¥ÂÊ ¾Ç¼¼»ç¸®
									// 7 = ¿ÞÂÊ   ¾Ç¼¼»ç¸®
									// 8 = ¹ÙÁö
									// 9 = ½Å¹ß
									// ¾ÆÀÌÅÛÀ» ÀåÂøÇÏÁö ¾ÊÀº ½½·ÔÀº id == -1

		short		sHP;
		short		sMaxHP;

		BYTE		tDir;			// º¸°íÀÖ´Â ¹æÇâ

		DWORD		dwAbnormal		// »óÅÂÀÌ»óÁ¤º¸(Damaged)

									// const DWORD ABNORMAL_NONE			=	0;
									// const DWORD ABNORMAL_POISON			=	1;
									// const DWORD ABNORMAL_CONFUSION		=	1 << 2;
									// const DWORD ABNORMAL_PARALYSIS		=	1 << 3;
									// const DWORD ABNORMAL_BLIND			=	1 << 4;
									// const DWORD ABNORMAL_LIGHT			=	1 << 5;
									// const DWORD ABNORMAL_FIRE			=	1 << 6;
									// const DWORD ABNORMAL_COLD			=	1 << 7;
									// const DWORD ABNORMAL_HASTE			=	1 << 8;
									// const DWORD ABNORMAL_SHIELD			=	1 << 9;
									// const DWORD ABNORMAL_HIDE			=	1 << 10;
									// const DWORD ABNORMAL_DEX_UP			=	1 << 11;
									// const DWORD ABNORMAL_MAX_HP_UP		=	1 << 12;
									// const DWORD ABNORMAL_ADAMANTINE		=	1 << 13;
									// const DWORD ABNORMAL_MIGHTYWEAPON	=	1 << 14;
									// const DWORD ABNORMAL_BERSERKER		=	1 << 15;
									// const DWORD ABNORMAL_HIEXP			=	1 << 16;
									// const DWORD ABNORMAL_MAGICFIND		=	1 << 17;
									// const DWORD ABNORMAL_NOCHAT			=	1 << 18;

									// const DWORD ABNORMAL_FASTRUN			=	1 << 19;
									// const DWORD ABNORMAL_MINDSHOCK		=	1 << 20;
									// const DWORD ABNORMAL_MINDGUARD		=	1 << 21;
									// const DWORD ABNORMAL_PSISHIELD		=	1 << 22;
									// const DWORD ABNORMAL_PIERCING_SHIELD	=	1 << 23;

									// const DWORD OPERATION_MODE			=	1 << 31;	// ¿î¿µÀÚ Åõ¸í ¿É¼Ç

		short		sCityRank;		// ½Ã¹Îµî±Þ

		int			iGuildIndex;	// ±æµå ÀÎµ¦½º
		VarChar		szGuildName;	// ±æµå ÀÌ¸§
		short		sVersion;		// ±æµå ¹®¾ç ¹öÁ¯
		BYTE		tClass;			// Å¬·¡½º Á¤º¸
		BYTE		tKillState;		// º¸¶óµ¹ÀÌÀÎÁö ÆÇ´Ü
									// 0 : Á¤»óÀûÀÎ À¯Àú
									// 1 : º¸¶óµ¹ÀÌ À¯Àú
		
		VarChar		strLoveName		//È£Äª[¾ÖÄª] //-- yskang 0.1	
*/		

//-----------------------------------------------------------------------------------------------------
// Finito
// 226 - Expression Packet - Client packet asking to send expression info
// 255 - Screen capture packet - Sends server date and time to client.
//-----------------------------------------------------------------------------------------------------

const BYTE EXPRESSION_REQ			= 226;
/*
		short		sExpression	
*/
const BYTE EXPRESSION_OPEN			= 230;
const BYTE TRANSFORM_REQ			= 234;
const BYTE PERSONAL_SHOP_REQ		= 239;
const BYTE SCREEN_CAPTURE_REQ		= 245;
const BYTE PALYER_HEARTBAET_REQ		= 252;//·â¼ÓËÙ
const BYTE PALYER_ONLINE_REQ		= 253;


#define		MAGIC_100_CONVERT_POS	4				//100×°±¸×ª»»Ìí¼ÓÊôÐÔÎ»ÖÃ
#define		MAGIC_100_ADD_POS		6				//100×°±¸ÊôÐÔÌí¼ÓÊ¹ÓÃ
#define		MAGIC_100_POS			7				//100¼¶ÏâÇ¶ÊôÐÔËùÔÚµÄ[11]ÌõÊôÐÔÖÐµÄÎ»ÖÃ

#define		MAGIC_BLUE_LOOP			2				//À¶É«
#define		MAGIC_GOLD_LOOP			4				//»ÆÉ«µÄ0,1,2,3,[4]¸ÄÊý
#define		MAGIC_GOLD_EX_LOOP		3				//»ÆÉ«×°±¸5,6,7,8
#define		MAGIC_GOLD_EX_POS		6				//

#endif
