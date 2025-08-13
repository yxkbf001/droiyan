#ifndef _DEFINE_H_
#define _DEFINE_H_

//
// 25饭骇 OR 何盒 劝悼力茄 急琶
//
#define _ACTIVE_USER		// 何盒 劝悼

//
//
//
//#define KOR_VER
  #define CHN_VER
//  #define ENG_VER

//
//	瘤开 弥碍 捞亥飘阑困秦 肺锯烦喉 版扁厘阑 烙矫肺 静霸茄促.	
//
//#define _EVENT_RR

//
//	MAX VALUE DEFINE
//
#define _MAX_DWORD				0xFFFFFFFF
#define _MAX_INT				0x7FFFFFFF
#define _MAX_SHORT				0x7FFF

//
//	Defines About Communication
//
//#define USER_SOCKET_PORT		12000 // 烙矫内靛

#define OVL_RECEIVE				0X01
#define OVL_SEND				0X02
#define OVL_CLOSE				0X03

#define PACKET_START1			0XAA
#define PACKET_START2			0X55
#define PACKET_END1				0X55
#define PACKET_END2				0XAA

//#define RECV_BUF_SIZE			8192 * 5
//#define SEND_BUF_SIZE			8192 * 5
#define RECV_BUF_SIZE			10000
#define SEND_BUF_SIZE			10000


//
//	Define CriticalSection Spin Count
//
#define SPIN_COUNT				4000

//
//	About City
//
#define SANAD					0
//VIP天数
#define MAX_VIP_DAY                 20000
//守护天使天数
#define MAX_TianShi_DAY             20000
//
//
//	About USER
//
#define USER_DEAD				0X00
#define USER_LIVE				0X01

//
//	About GUILD
//
#define MAX_GUILD				30000
#define MAX_GUILD_USER			50
#define GUILD_WAR_PREPARE		0
#define GUILD_WAR_DECISION		1
#define GUILD_MAX_TAX			30 //添加税收
#define MAX_GUILD_HOUSE_USER	20

#define GUILD_WAR_READY		0		// 辨靛傈 官肺 5盒傈俊 惯积窍绰 霖厚 窜拌
#define GUILD_WARRING		1		// 瘤陛 辨靛傈吝烙
#define GUILD_WAR_AFFTER	2		// 辨靛傈捞 场抄饶

#define FORTRESS_VIOLENCE_WARRING	1	// 刮缴捞 馆鄂吝
#define FORTRESS_VIOLENCE_AFFTER	0	// End

#define GUILD_WAR_MAX_MSG_TIMER	6		// 傍己傈吝 巢篮 矫埃阑 舅妨霖促.

#define GUILD_GUARD_MAX_NUM		6	// 夸货甫 瘤虐绰 版厚捍栏肺 弥措 6疙鳖瘤 绊侩且荐乐促.
#define _GUILDNPCLIST_DB		24	// 4 * 6

#define GUILD_REPAIR_MAX_NUM	5	// 弥措 5俺 鳖瘤 荐府且 NPC啊 乐促.
#define _GUILDREPAIRLIST_DB		30 // 5 * 6

#define FORTRESS_TARGET_MAX_NUM	4	// 弥措 4俺 鳖瘤 坷胆肺啊 乐促.
#define GUILD_ATTACK_MAX_NUM	20	// 弥措 20俺鳖瘤 辨靛傈 脚没阑 罐阑荐 乐促
#define _GUILDLIST_DB			80	// 4 * 20

#define GUILDFORTRESS_ATTACK_MAX_NUM	20	// 弥措 50俺鳖瘤 辨靛傈 脚没阑 罐阑荐 乐促
#define _GUILDFORTRESSLIST_DB			200	// 4 * 50

#define GUILD_VIRTUAL_WAR_START	16
#define GUILD_VIRTUAL_WAR_END	17
#define GUILD_HOUSE_DAY			18

#define GUILD_FIELD_WARRING		1
#define GUILD_STORE_WARRING		2
#define GUILD_FOTRESS_WARRING	3		
//
//	About NPC
//
#define NPC_NUM					200
#define NPC_DEAD				0X00
#define NPC_LIVE				0X01
#define NPC_ATTACKING			0X02
#define NPC_ATTACKED			0X03
#define NPC_ESCAPE				0X04
#define NPC_STANDING			0X05
#define NPC_MOVING				0X06
#define NPC_TRACING				0X07
#define NPC_FIGHTING			0X08
#define NPC_STRATEGY			0x09
#define NPC_BACK				0x10
#define NPC_WAIT				0x11	// 啊惑 傍埃俊辑 积己窍绰 各篮 老雀己 捞骨肺

#define NPC_PASSIVE				150
#define NPC_ACTIVE				100

//
//	About Map Object
//
#define USER_BAND				10000		// Map 困俊 蜡历啊 乐促.
#define NPC_BAND				20000		// Map 困俊 NPC(各器窃)啊 乐促.
#define INVALID_BAND			30000		// 肋给等 ID BAND
#define USER_HUFA_BAND			20000
//
//	About Big Mop Size In CELL
//
#define NPC_MAP_SIZE			20

//
//	Defines About Max Value
//
#define MAX_BASIC_ITEM			1000
#define MAX_EVENT				3000  //最大事件event编号 原始1000

//
//	To Who ???
//
#define TO_ALL					0X01
#define TO_ZONE					0X02
#define TO_ME					0X03
#define TO_INSIGHT				0X04

#define SEND_USER				0
#define SEND_INSIGHT			1
#define SEND_ZONE				2
#define SEND_ALL				3
#define SEND_RANGE				4
#define SEND_SCREEN				5

//
//	矫具贸府
//
#define	SCREEN_X				15
#define	SCREEN_Y				19

//#define SIGHT_SIZE_X			10
//#define SIGHT_SIZE_Y			10
#define SIGHT_SIZE_X			12
#define SIGHT_SIZE_Y			12

#define DIR_H					1
#define DIR_L					-1
#define DIR_OUTSIDE				3

//
//	State Value
//
#define STATE_ACCEPTED			0X01   //接受
#define STATE_CONNECTED			0X02   //连接
#define STATE_DISCONNECTED		0X03   //断开
#define STATE_GAMESTARTED		0X04   //游戏开始
#define STATE_INITED			0X05   //邀请
#define STATE_LOGOUT			0X06   //注销
#define STATE_GAMERESTART		0X07   //重启

//
//	Event
//
#define MAX_LOGIC_INT			5
#define MAX_EXEC_INT			3
#define MAX_LOGIC_ELSE_INT		20
#define MAX_LOGIC_CHAR_LEN		128
#define MAX_EXEC_CHAR_LEN		128
#define MAX_LOGIC_ELSE_CHAR_LEN 128

#define MONSTER_ATTACK_EVENT_BAND		10000		// 10000锅 捞惑狼 捞亥飘 锅龋绰 阁胶磐啊 傍拜寸沁阑锭 惯积茄促.

//
//	PSI limit Number
//
#define BRAWL_PSI_NUM			2
#define STAFF_PSI_NUM			7
#define	EDGED_PSI_NUM			2
#define FIREARMS_PSI_NUM		2
#define NOCLASS_PSI_NUM			3

#define	DEFAULT_PSI_DELAY		30000

//
//  Item
//
#define TYPE_MONEY				0
#define TYPE_ITEM				1

//最大装备改数
#define MAX_ITEM_UPGRADE_COUNT	o_yehuoini[0]->gaishu		// 泅犁 0~9鳖瘤 诀弊饭捞啊 啊瓷窍促

#define ITEM_NAME_LENGTH		20
#define MAX_THROW_ITEM			5000    //地面物品刷新最大数
#define ITEM_MAX_USE_WEAR		150		// 酒捞袍吝 镜荐乐绰 加己甸父 葛酒持扁困秦, 捞亥飘 酒捞袍苞 备盒
#define ITEM_USER_RIGHT_NUM		3		// 泅犁 3疙鳖瘤 酒捞袍俊措秦 鼻府甫 林厘茄促.

#define EVENT_ITEM_LENGTH		30		// 捞亥飘肺 惯积窍绰 酒捞袍狼 绊蜡锅龋甫 何咯且 磊府荐

#define EVENT_UNIQUE_ITEM_BAND	30000	// 弥绊乏 蜡历甫困秦 漂喊酒捞袍阑 嘎冕促.	
#define EVENT_RR_ITEM_BAND		30001

// Upgrade Item
#define BLESSING_WEAPONLESS_WEAR	23	// 绵 傀乔府胶 酒捞袍 傀绢 沥焊	
#define BLESSING_ARMORLESS_WEAR		24	// 绵 酒赣府胶 酒捞袍 傀绢 沥焊		

#define NORMAL_WEAPONLESS_WEAR		25	// 老馆 傀乔府胶 酒捞袍 傀绢 沥焊	
#define NORMAL_ARMORLESS_WEAR		26	// 老馆 酒赣府胶 酒捞袍 傀绢 沥焊	
	
//
//	Quick Item Use
//
#define QUICK_ITEM_SP			1
#define QUICK_ITEM_PP			2	
#define QUICK_ITEM_HP			4
#define QUICK_ITEM_ALLREPAIR	7
#define QUICK_ITEM_CONFUSE		8
#define QUICK_ITEM_PALSY		16
#define QUICK_ITEM_POISON		32
#define QUICK_ITEM_VISION		64
#define QUICK_ITEM_ALLCURE		127
#define QUICK_ITEM_BULL			100
#define QUICK_ITEM_POTAL		101
#define EVENT_ITEM_LOTTERY		102
#define QUICK_ITEM_TELEPORT		103
#define	EVENT_ITEM_DEADPOISON	104
#define QUICK_ITEM_SALUTE		113	

#define NORMAL_SALUTE_SID		850
#define XMAS_SALUTE_SID			874
#define YEAR_SALUTE_SID			875

//
//	Moon Event Item
//
#define EVENTITEM_WEAR_MOON		115

//
//	Invasion Event Item
//
#define EVENTITEM_WEAR_INVASION	120


//
//	Special Potion
//
#define SPECIAL_ITEM_BIANLANG	    	1028  //狼变身
#define SPECIAL_ITEM_BIANXIONG		    1029  //熊变身
#define SPECIAL_ITEM_TS			1044  //守护天使
#define SPECIAL_ITEM_HX_LS		1035//幻灵
#define SPECIAL_ITEM_HX_JS		1289//幻晶
#define SPECIAL_ITEM1441		1405  //20元宝券
#define SPECIAL_ITEM1442		1415  //50元宝券
#define SPECIAL_ITEM1443		1416  //100元宝券
#define SPECIAL_ITEM1444		1417  //500元宝券
#define SPECIAL_ITEM1445		1418  //1000元宝券
#define SPECIAL_POTION_WEAR		116
#define SPECIAL_ITEM_HIEXP		699
#define SPECIAL_ITEM_MAGICFIND	700
#define SPECIAL_ITEM_KILLREMOVE	701
#define SPECIAL_BIAOZHI		    724

// 傈磊快钎 酒捞袍
#define NORMAL_ITEM_STAMP		714					// zinee 02-11-18

// Revival Item
#define NORMAL_ITEM_REVERSE		807					// 府滚胶府柯 : 滚叼盔 何劝 酒捞袍

//
//	Psionic Stone
//
#define PSI_STONE_WEAR			117					// 荤捞坷葱 嚼垫备浇 傀绢 沥焊

#define PSI_STONE_FAST_RUN			706					
#define PSI_STONE_MIND_SHOCK		707
#define PSI_STONE_MIND_GUARD		708
#define PSI_STONE_PSI_SHIELD		709
#define PSI_STONE_PIERCING_SHIELD	710
#define PSI_STONE_BAO_YAN			1123
#define PSI_STONE_BA_QI				1124
#define PSI_STONE_CHAO_FAN_TAN		1549
#define PSI_STONE_CHAO_FENG_LI		1550
#define PSI_STONE_CHAO_YI_NENG		1551
#define PSI_STONE_CHAO_DUO_CHONG	1552
#define PSI_STONE_CHONG_RAN     	1737
#define PSI_STONE_LI_HU_SHUO_LIAN     	1739 


//
//	DB俊 历厘登绰 荤捞坷葱
//
#define PSIONIC_HASTE				1
#define PSIONIC_SHIELD				6
#define PSIONIC_DEXUP				11
#define PSIONIC_HPUP				12

#define PSIONIC_ADAMANTINE			13
#define PSIONIC_MIGHTYWEAPON		14
#define PSIONIC_BERSERKER			15

//
//	Quest Psionic
//
#define PSIONIC_FAST_RUN			16
#define PSIONIC_MIND_SHOCK			17
#define PSIONIC_MIND_GUARD			18
#define PSIONIC_PSI_SHIELD			19
#define PSIONIC_PIERCING_SHIELD		20
#define	PSIONIC_BAO_YAN				35
#define PSIONIC_BA_QI				36
#define PSIONIC_CHAO_FAN_TAN        64
#define PSIONIC_CHAO_FENG_LI        65
#define PSIONIC_CHAO_YI_NENG        66
#define PSIONIC_CHAO_DUO_CHONG        67
#define PSIONIC_CHONG_RAN           73
#define PSIONIC_LI_HU_SHUO_LIAN     	76 

const BYTE PSI_BISA_0			=	0x17;					//拳必杀
const BYTE PSI_BISA_1			=	0x18;					//法必杀
const BYTE PSI_BISA_2			=	0x19;					//剑必杀
const BYTE PSI_BISA_3			=	0x1a;					//枪必杀
const BYTE PSI_BISA_4			=	0x53;					//仲裁必杀

const BYTE PSI_ZHAN_SHEN_GONG		=	54;	   //战神的技能 黑暗灵蝠
//新职业魔法
const BYTE PSI_JUDGE_FU_SHE_LEN		=	71;					//辐射能 lv10
const BYTE PSI_JUDGE_CALL_YI_TU_LUO		=	72;				//召唤伊涂罗lv20
const BYTE PSI_JUDGE_CONG_RAN		=	73;					//重燃lv30
const BYTE PSI_JUDGE_ATTACK_UP		=	74;					//攻击力提升-敏捷lv50
const BYTE PSI_JUDGE_WIS_UP		=	75;						//攻击力提升-智力lv50
const BYTE PSI_JUDGE_LI_HU_SHUO_LIAN		=	76;			//灵魂锁链
const BYTE PSI_JUDGE_CALL_PU_LEI_TA		=	77;				//召唤普雷塔lv90
const BYTE PSI_JUDGE_QIANG_LI_CONG_RAN		=	78;			//强力重燃lv100
const BYTE PSI_JUDGE_XI_SHEN_HU_FA		=	79;				//牺牲护法lv115
const BYTE PSI_JUDGE_CALL_KE_LAO_SI		=	80;				//召唤克劳斯lv125
const BYTE PSI_JUDGE_LEI_XIAN_FENG_BAO		=	81;			//烈焰风暴lv130
const BYTE PSI_JUDGE_CALL_YI_XI_SI		=	82;				//召唤伊西斯lv150
const BYTE PSI_JUDGE_LU_SHA		=	83;						//怒杀
//
//	Psionic Success Rate
//
#define SUCCESS_RATE_MIND_SHOCK			25
#define SUCCESS_RATE_PIERCING_SHIELD	10

//
//	E-Body Magic Class
//

#define EBODY_NONE						0			// E-Body 可记绝澜
#define EBODY_MOVE_UP					1			// 捞悼加档 1窜拌 刘啊
#define EBODY_ATTACK_DELAY_DOWN			2			// 傍拜加档 1窜拌刘啊
#define EBODY_WEIGHT_UP					3			// 公霸 10% 歹 惦
#define EBODY_DEFENSE_UP				4			// 规绢仿 刘啊
#define EBODY_PSI_RESIST_UP				5			// 荤捞坷葱 规绢仿 刘啊

#define EBODY_VITAL_RESIST				6			// 官捞呕 农府萍拿 单固瘤 皑家			
#define EBODY_BACK_RESIST				7			// 馆拜 单固瘤 皑家
#define EBODY_WEAPON_BASE_UP			8			// 公扁狼 扁夯 单固瘤 刘啊
#define EBODY_ATTACK_RATING_UP			9			// 疙吝伏 刘啊
#define EBODY_PSI_ATTACK_UP				10			// 荤捞坷葱 单固瘤 刘啊

#define EBODY_HP_RECOVERY_UP			11			// HP 雀汗樊 吝啊
#define EBODY_AVOID_RATING_UP			12			// 雀乔啦 吝啊
#define EBODY_RECOVERY_TIME_UP			13			// 雀汗加档 刘啊
#define EBODY_CASTING_DELAY_DOWN		14			// 某胶泼 矫埃 皑家
#define EBODY_PP_RECOVERY_UP			15			// PP 雀汗樊 吝啊

#define EBODY_STR_TO_DAMAGE				16			// STR 阑 单固瘤肺 券魂
#define EBODY_STR_TO_WEIGHT				17			// STR 阑 公霸肺 券魂
#define EBODY_CON_TO_HP					18			// CON 阑 HP 肺 券魂
#define EBODY_CON_TO_DEFENSE			19			// CON 阑规绢仿栏肺
#define EBODY_CON_TO_BACK_RESIST		20			// CON 阑 馆拜 单固瘤 皑家肺

#define EBODY_CON_TO_VITAL_RESIST		21			// CON 阑 官捞呕 单固瘤 皑家肺
#define EBODY_DEX_TO_ATTACK_RATING		22			// DEX 甫 疙吝伏肺 
#define EBODY_DEX_TO_AVOID_RATING		23			// DEX 甫 雀乔啦肺
#define EBODY_DEX_TO_DAMAGE				24			// DEX 甫 单固瘤肺
#define EBODY_DEX_TO_BACK_RESIST		25			// DEX 甫 馆拜单固瘤 皑家肺

#define EBODY_DEX_TO_VITAL_RESIST		26			// DEX 甫 官捞呕 单固瘤 皑家肺
#define EBODY_VOL_TO_PSI_DAMAGE			27			// VOL 阑 荤捞坷葱 单固瘤肺
#define EBODY_VOL_TO_PSI_RESIST			28			// VOL 阑 荤捞坷葱 历亲仿栏肺
#define EBODY_WIS_TO_PP					29			// WIS 甫 PP肺 
#define EBODY_WIS_TO_CAST_DOWN			31			// WIS 甫 纳胶泼 矫埃 皑家肺

#define EBODY_WIS_TO_DAMAGE				32			// WIS智力变远程攻击力

#define EBODY_MAGIC_COUNT				33			//机械身体属性条目
///
///幻石///
#define HUANSHI_MAGIC_COUNT		8 
#define H_ZERO			0
#define H_DEFINE		1
#define H_PSI_DEFINE	2
#define H_HP			3
#define H_HUIBI			4
#define H_PSI_SUB_DAMAGE 5
#define H_END_SUB_DAMAGE 6
#define H_WL_SUB_DAMAGE 7

#define MS_MAGIC_COUNT 13 //程序卡属性统计
#define Q0_130_0	0
#define QS0_130_0	1
#define QS0_130_1	2
#define QS0_130_2	3
#define FS1_130_3	4
#define FS1_130_4	5
#define FS1_130_5	6
#define JS2_130_6	7
#define JS2_130_7	8
#define JS2_130_8	9
#define QS3_130_9	10
#define QS3_130_10	11
#define QS3_130_11	12
//
//	Magic Item
//
//#define MAGIC_COUNT						38			// 魔法属性类型
#define STATE_MAGIC_COUNT				8

#define MAGIC_NONE						0//无
#define MAGIC_STR_UP					1 //力量增加
#define MAGIC_CON_UP					2 //体质增加.
#define MAGIC_DEX_UP					3 //敏捷增加
#define MAGIC_VOL_UP					4 //智慧增加
#define MAGIC_WIS_UP					5 //智力增加.

#define MAGIC_BACK_ATTACK				6			//反击技术
#define MAGIC_EXCITE					7//兴奋技术
#define MAGIC_IRON_SKIN					8//钢铁皮肤
#define MAGIC_HP_RECOVER_UP				9//自我医疗

#define MAGIC_PSI_CONCENTRATION			10			// 魔法精修
#define MAGIC_CASTING_DELAY_DOWN		11//法术精修
#define MAGIC_PP_CONSUME_DOWN			12//精神强化
#define MAGIC_PP_RECOVER_UP				13//冥想技术

#define MAGIC_VITAL_CRITICAL			14			//穿刺技术
#define MAGIC_CRITICAL_GUARD			15    //闪避技术
#define MAGIC_EDGE_CONCENTRATION		16//精神集中
#define MAGIC_SP_RECOVER_UP				17//回复技术

#define MAGIC_ATTACK_ACCURACY_SKILL_UP	18			// 狙击技术
#define MAGIC_FIRE_DOUBLE_ATTACK		19//连射技术
#define MAGIC_AVOID_SKILL_UP			20//灵动技术
#define MAGIC_MIN_DAMAGE_UP				21//枪械大师

#define MAGIC_DAMAGE_UP					22  //损伤增加
#define MAGIC_STATE_ERROR_UP			23	//诱发各种状态
#define MAGIC_STATE_ERROR_DOWN			24			//防止各种异常状态
#define MAGIC_ALL_SKILL_UP				25//所有技术
#define MAGIC_RANGE_UP					26//射程增加 
#define MAGIC_ATTACK_DELAY_DOWN			27//攻击速度
#define MAGIC_ATTACK_RATING_UP			28//命中率
#define MAGIC_AVOID_RATING_UP			29//回避率
#define MAGIC_BROKEN_DOWN				30			// 故障率
#define MAGIC_DEFENSE_UP				31			// 防御增加
#define MAGIC_DURATION_UP				32			// 耐久性增加
#define MAGIC_MAX_HP_UP					33			// 生命值增加
#define MAGIC_MAX_PP_UP					34			//精神值增加
#define MAGIC_MAX_SP_UP					35			//体力值增加

#define MAGIC_PSI_RESIST_UP				36			// 魔法损伤增加
#define MAGIC_PSI_ATTACK_UP				37			// 魔法抗斥增加

#define MAGIC_ALL_POINT_UP				38			// 所有技能点增加
#define MAGIC_HP_RATING_UP				39			// 血量增加百分比

#define MAGIC_ZI_DONG_FANG_HU			41			// 自动防护
#define MAGIC_WU_SHI_CHUA_CHI			42			// 无视穿刺

#define MAGIC_PSI_ATTACK_DOWN			43			// 魔法损伤减少
#define MAGIC_FINALLY_ATTACK_DOWN		44			// 最终损伤减少
#define MAGIC_PHY_ATTACK_DOWN			45			// 物理损伤减少

#define MAGIC_EXP_3BEI_UP				46			// 获得经验值增加30%

#define MAGIC_FINALLY_ATTACK_UP			48			// 最终损伤增加 

#define MAGIC_QIANG_HUA_HUA_FA_UP		49			// 强化生命
#define MAGIC_REN_MI_BU_XI_UP			50			// 生活不息
#define MAGIC_BAO_FA_UP					51			// 爆发增加
#define MAGIC_JIN_ZHUN_UP				52			// 精准增加

#define MAGIC_COUNT						53			//魔法属性类型
//
//	Magic Item STATE_ERROR 惑怕捞惑 曼炼
//
#define MAGIC_STATE_NORMAL				0 
#define MAGIC_STATE_ELECTRIC			1 
#define MAGIC_STATE_COLD				2 
#define MAGIC_STATE_FIRE				4 
#define MAGIC_STATE_CONFUSION			8
#define MAGIC_STATE_PARALISYS			16
#define MAGIC_STATE_POINSON				32  
#define MAGIC_STATE_BLIND				64
#define MAGIC_STATE_ALL					127

//
//	SKILL
//
#define SKILL_IRON						0//钢铁皮肤
#define SKILL_EXCITE					1//兴奋
#define SKILL_BACK_ATTACK				2//反击技能
#define SKILL_HP_RECOVER_UP				3//自我医疗

#define SKILL_PP_RECOVER_UP				5//冥思
#define SKILL_PP_DOWN					6//精神强化
#define SKILL_PSI_SUCCESS_UP			7//魔法精修
#define SKILL_CASTING_DOWN				8//法师大师

#define SKILL_CONCENTRATION				10//精神集中
#define SKILL_CRITICAL_GUARD			11//闪避
#define SKILL_VITAL_CRITICAL			12//穿刺
#define SKILL_SP_RECOVER_UP				13//回复

#define SKILL_MIN_DAMAGE_UP				15//枪械法师
#define SKILL_RE_FIRE_UP				16//连射
#define SKILL_ATTACK_RATING_UP			17//狙击
#define SKILL_AVOID_RATING_UP			18//灵动

#define SKILL_ASSAULT					20//怒击
#define SKILL_ABSOLUTE_DEFENSE			21//绝对防御
#define SKILL_PSYCHIC_RESIST			22//魔法抗斥
#define SKILL_DEFENSE_UP				23//超级防御

#define SKILL_JUDGE_QIANGHUAHUFA		25				//强化护法护法,67%起点智力变成护法攻击力1-2级9点,3-8级8点,9-14级7点,15-20级6点,
#define SKILL_JUDGE_RENMINBUXI			26				//生命不息 起点78%体质转成生命1-6级以下每级增加12%,6级以后每级增加11%
#define SKILL_JUDGE_BAOFA				27				//爆发 起点10攻击力增加10%
#define SKILL_JUDGE_JINGZHUN			28				//精准 起点10命中1-6级以每级增加2点。6级以后每级增加命中1 

#define SKILL_COUNT						29				//普通技能总数


#define SKILL_130_TI_ZHI_QIANG_HUA		16				//仲裁体质强化 15体质为起点,每级加二点体质 
#define SKILL_130_REN_MI_FU_HA			17				//仲裁生命护法 仲裁血量起点 17%,每级增加2%
#define SKILL_130_MO_FU_KANG_CHI_4		18				//仲裁抗斥 37魔法抗斥 每级加4点
#define SKILL_130_COUNT					19				//130技能总数

#define SKILL_140_ZHONGJI_FANGYU		12				//140终极防御
#define SKILL_140_WUDI_LIESHA			13				//140无敌猎杀
#define SKILL_140_ZHI_DONG		        14				//140制动


////////////////////////////////////////////////////(官操扁 傈 单捞磐)
#define MAGIC_BROKEN_DOWN_5				5
#define MAGIC_BROKEN_DOWN_11_TO_15		67

/*
/////捞傈 累诀
#define MAGIC_DAMAGE_Y_UP				1
#define MAGIC_DAMAGE_Z_UP				2
#define MAGIC_ACCURACY_UP_5				3
#define MAGIC_CRITICAL_UP_4				4
#define MAGIC_BROKEN_DOWN_5				5
#define MAGIC_MAKE_POISON				6
#define MAGIC_MAKE_PARALYSIS			7
#define MAGIC_MAKE_CONFUSION			8
#define MAGIC_MAKE_BLIND				9
#define MAGIC_MAKE_FIRE					10
#define MAGIC_MAKE_ICE					11
#define MAGIC_MAKE_ELECTRIC				12
#define MAGIC_DURATION_UP_200			13
#define MAGIC_STAFF_GUARD_1				14
#define MAGIC_BRAWL_GUARD_1				15
#define MAGIC_EDGE_GUARD_1				16
#define MAGIC_STAFF_CONCENTRTE_UP_1		17
#define MAGIC_STAFF_PSY_ACCLIMATE_UP_1	18
#define MAGIC_STAFF_FASTCAST_UP_1		19
#define MAGIC_EDGE_WPN_ACCLIMATE_UP_1	20
#define MAGIC_EDGE_FASTATTACK_UP_1		21
#define MAGIC_FIREARM_OUTRANGE_UP_1		22
#define MAGIC_FIREARM_STRAFING_UP_1		23
#define MAGIC_FRIEARM_MECHANIC_UP_1		24
#define MAGIC_BRAWL_FASTATTCK_UP_1		25
#define MAGIC_BRAWL_OVERHEAT_UP_1		26
#define MAGIC_AMMO_UP_4					27
#define MAGIC_ARMORPOINT_UP_1_TO_7		28
#define MAGIC_RANGE_UP_1				29
#define MAGIC_ATTACKDELAY_DN_1_TO_5		30
#define MAGIC_AVOID_POINSON				31
#define MAGIC_AVOID_PARALISYS			32
#define MAGIC_AVOID_CONFUSION			33
#define MAGIC_AVOID_BLIND				34
#define MAGIC_AVOID_FIRE				35
#define MAGIC_AVOID_ICE					36
#define MAGIC_AVOID_ELECTRIC			37
#define MAGIC_AVOID_ALL					38
#define MAGIC_USING_SP_DOWN_1			39
#define MAGIC_STR_UP_1_TO_2				40
#define MAGIC_CON_UP_1_TO_2				41
#define MAGIC_DEX_UP_1_TO_2				42
#define MAGIC_INT_UP_1_TO_2				43
#define MAGIC_VOL_UP_1_TO_2				44
#define MAGIC_WIS_UP_1_TO_2				45
#define MAGIC_EVADE_UP_10_TO_20			46
#define MAGIC_RECOVER_PP_UP_1			47
#define MAGIC_RECOVER_SP_UP_1			48
#define MAGIC_DURATION_UP_100			49
#define MAGIC_DURATION_UP_300			50
#define MAGIC_ARMORPOINT_UP_8_TO_15		51
#define MAGIC_ACCURACY_UP_11_TO_15		52
#define MAGIC_CRITICAL_UP_11_TO_15		53
#define MAGIC_ATTACKDELAY_DN_11_TO_15	54
#define MAGIC_STAFF_ACCURACY_UP_1		55
#define MAGIC_BRAWL_ACCURACY_UP_1		56
#define MAGIC_EDGE_ACCURACY_UP_1		57
#define MAGIC_FIREARM_ACCURACY_UP_1		58
#define MAGIC_EDGE_VITAL_CRITICAL_UP_1	59
#define MAGIC_BRAWL_DEALY_CRITICAL_UP_1	60
#define MAGIC_ALL_SKILL_UP_1			61
#define MAGIC_ALL_SKILL_UP_2			62
#define MAGIC_ATTACKDELAY_DN_6_TO_10	63
#define MAGIC_DURATION_UP_500			64
#define MAGIC_DURATION_UP_700			65
#define MAGIC_CRITICAL_UP_16_TO_20		66
#define MAGIC_BROKEN_DOWN_11_TO_15		67
#define MAGIC_STAFF_ACCURACY_UP_2		68
#define MAGIC_BRAWL_ACCURACY_UP_2		69
#define MAGIC_EDGE_ACCURACY_UP_2		70
#define MAGIC_FIREARM_ACCURACY_UP_2		71
#define MAGIC_STAFF_GUARD_2				72
#define MAGIC_BRAWL_GUARD_2				73
#define MAGIC_EDGE_GUARD_2				74
#define MAGIC_USING_SP_DOWN_2			75
#define MAGIC_STR_UP_3_TO_4				76
#define MAGIC_CON_UP_3_TO_4				77
#define MAGIC_DEX_UP_3_TO_4				78
#define MAGIC_INT_UP_3_TO_4				79
#define MAGIC_VOL_UP_3_TO_4				80
#define MAGIC_WIS_UP_3_TO_4				81
#define MAGIC_EVADE_UP_30_TO_40			82
#define MAGIC_ATTACKDELAY_DN_10			83
#define MAGIC_RECOVER_PP_UP_2			84
#define MAGIC_RECOVER_SP_UP_2			85
#define MAGIC_RANGE_UP_2				86
#define MAGIC_EDGE_VITAL_CRITICAL_UP_2	87
#define MAGIC_BRAWL_DEALY_CRITICAL_UP_2	88
#define MAGIC_BRAWL_FASTATTACK_UP_2		89
#define MAGIC_BRAWL_OVERHEAT_UP_2		90
#define MAGIC_DAMAGE_X_UP_1				91
#define MAGIC_DAMAGE_Y_UP_4_TO_6		92
#define MAGIC_DAMAGE_Z_UP_6_TO_10		93
*/
//
//	Attack
//
#define		DEFAULT_AT_DELAY	600  //物理职业攻速延迟
#define		DEFAULT_SP_DEC		1

const BYTE	ATTACK_FAIL				=	0;
const BYTE	ATTACK_SUCCESS			=	1;
const BYTE	ATTACK_MISS				=	2;
const BYTE	CRITICAL_ATTACK_SUCCESS =	3;//yskang 0.3
const BYTE	CRITICAL_ATTACK_MISS	=	4;//yskang 0.3

const BYTE	ATTACK_MIN		=	15;//20;
const BYTE	ATTACK_MAX		=	90;//80;

#define		DAMAGE_DELAY_C_TIME	2000

//
//	LevelUP Init Value
//
#define SKILL_INIT_VALUE		300
#define PSY_INIT_VALUE			300
#define LEVEL_INIT_VALUE		600

//	Logic Define...
#define LOGIC_POSITION			0X01
#define LOGIC_MEET_NPC			0X02

//	Execution Define...
#define EXEC_LINK				0X01
#define EXEC_SAY				0X02
#define EXEC_SAY_IN				0x03
#define EXEC_GIVE_ITEM			0X04
#define EXEC_ROB_ITEM			0X05
#define EXEC_SDROB_ITEM			0X6F//优惠券
#define EXEC_GIVE_MAGIC			0X06
#define EXEC_ROB_MAGIC			0X07
#define EXEC_BBS_OPEN			0X08
#define EXEC_DISAPPEAR			0X09
#define EXEC_RUN_MAGIC			0X0A
#define EXEC_DEAD				0X0B
#define EXEC_CHANGE_LEVEL		0X0C
#define EXEC_CHANGE_GENDER		0X0D
#define EXEC_CHANGE_HP			0X0E
#define EXEC_CHANGE_MP			0X0F
#define EXEC_CHANGE_STR			0X10
#define EXEC_CHANGE_DEX			0X11
#define EXEC_CHANGE_INT			0X12
#define EXEC_CHANGE_WIS			0X13
#define EXEC_CHANGE_CHR			0X14
#define EXEC_CHANGE_CON			0X15
#define EXEC_CHANGE_DMAGIC		0X16
#define EXEC_CHANGE_WMAGIC		0X17
#define EXEC_CHANGE_BMAGIC		0X18
#define EXEC_CHANGE_SWORD		0X19
#define EXEC_CHANGE_SPEAR		0X1A
#define EXEC_CHANGE_AXE			0X1B
#define EXEC_CHANGE_BOW			0X1C
#define EXEC_CHANGE_KNUCKLE		0X1D
#define EXEC_CHANGE_WEAPONMAKE	0X1E
#define EXEC_CHANGE_ARMORMAKE	0X1F
#define EXEC_CHANGE_ACCMAKE		0X20
#define EXEC_CHANGE_POTIONMAKE	0X21
#define EXEC_CHANGE_COOKING		0X22
#define EXEC_SHOW_MAGIC			0X23
#define EXEC_MAKER_OPEN			0X24
#define EXEC_STORAGE_OPEN		0X25
#define EXEC_AUCTION_SELL_OPEN	0X26
#define EXEC_AUCTION_BUY_OPEN	0X27
#define EXEC_SELECT_MSG			0X28
//#define EXEC_MESSAGE_OK			0X29
#define EXEC_KILL_BABY			0x29			//卖掉宠物
//#define EXEC_MESSAGE_NONE		0X2A
#define	EXEC_GIVE_YB			0x2A//给元宝
#define EXEC_GIVE_GOLD			0X2B
#define EXEC_ROB_DN				0X2C
#define EXEC_ROB_LING			0X1A
#define EXEC_RUN_EVENT			0X2D
#define EXEC_ADD_EVENT			0X2E
#define EXEC_DEL_EVENT			0X2F
#define EXEC_RAND_SAY			0X30
#define EXEC_RETURN				0X31
#define EXEC_CHANGE_MAGETYPE	0X32
#define EXEC_GUILD_OPEN			0X33 // 辨靛汲赋甫 楷促.
#define EXEC_ADD_SKILL			0X80
#define EXEC_GIVE_ITEM_ALL		0X81
/********************************************************/
// 2001.5.7 HyperText... 
#define EXEC_STORE_OPEN			0X03 // 酒捞袍 魄概 NPC
#define EXEC_REPAIR_OPEN		0X34 // 酒捞袍 荐府 NPC 		
#define EXEC_SELL_OPEN			0X35 // 酒捞袍 混 NPC 
#define EXEC_ZONE_MOVE			0X36 // 秦寸 粮栏肺 角青
#define EXEC_MOVE				0X41 // 鞍篮 粮俊辑 捞悼 角青
#define EXEC_FT_MOVE			0X3A // 夸货救栏肺 捞悼	
#define EXEC_FT_PEOPLE			0x3B // 刮缴 荐摹甫 钎泅
#define EXEC_FT_REPAIR			0x3C // 刮缴 荐摹甫 钎泅
#define EXEC_PSI_STORE_OPEN		0X37 // 秦寸 荤捞坷葱 魄概
#define EXEC_ALL_CURE			0X38 // 葛电 HP, PP, SP甫 MAX肺 父惦
//#define EXEC_CARE				0X39 // 葛电 惑怕捞惑甫 摹丰
#define EXEC_SC_CHENGXU_KA      0X39//删除机甲程序卡
#define EXEC_BANK_OPEN			0x40 // 篮青 芒备甫 楷促. 
#define EXEC_GUILD_MARK			0x42 // 辨靛 巩剧阑 殿废
#define EXEC_GUILD_WAREHOUSE_OPEN 0x43 // 辨靛 芒绊甫 楷促.
#define EXEC_EBODY_OPEN			0x44 // E_Body 惑痢阑 坷锹茄促.	
#define EXEC_UPGRADE_OPEN		0x45
#define EXEC_FIELD_STORE_OPEN	0x46 // 鞘靛惑牢老锭绰 盒扁秦具 窍骨肺..蝶肺 沥狼茄促.
#define EXEC_GUILD_SAY			0x47 // 辨靛俊 加茄 惑牢捞 惑措祈 脚盒俊 蝶扼 盒扁茄促...
#define EXEC_GUILD_TAX			0x48 // 辨付啊 秦寸 惑痢俊 技陛阑 炼沥茄促.
#define EXEC_GUILD_SCHEME		0x49 // 辨付啊 秦寸 惑痢俊 措茄 辨靛傈阑 汲沥茄促.
#define EXEC_GUILD_APPLY		0x4A // 促弗 辨付啊 辨靛傈阑 脚没茄促.
#define EXEC_XP_UP				0x4B // XP甫 棵妨淋
#define EXEC_CHECK_GUILD_USER	0x4C // 立加吝牢 辨靛蜡历甫 焊辰促.	
#define EXEC_CHECK_GUILD_RANK	0x4D // 辨靛 窍快胶盒剧甫 困秦 鉴困甫 舅赴促.
#define EXEC_GUARD_BOW			0x4E // 版厚捍捞 蜡历俊霸 牢荤窍搁辑 捞亥飘 措荤甫 茄促.
#define EXEC_GIVE_EVENT_ITEM	0x4F // 捞亥飘 惑前鼻苞 酒捞袍阑 背券茄促. 
#define EXEC_ADDRESS_WINDOW		0x50 // 惑前鼻 林家 涝仿芒 剁快扁.
#define EXEC_EVENT_ROB_ITEM		0x51 // 惑前鼻 雀荐 捞亥飘
#define EXEC_DRESSING			0x52 // 急堡 芒 剁快扁
#define EXEC_ACCOUNT_BANK_OPEN	0x53 // 拌沥 篮青 凯扁
#define	EXEC_GIVE_DN			0x54 // 蜡历俊霸 捣 林扁
#define EXEC_SEND_MSG			0x55 // 矫胶袍 皋技瘤 焊郴扁
#define EXEC_GIVE_MAGIC_ITEM	0x56 // 蜡历俊霸 概流 酒捞袍阑 霖促. item 抛捞喉俊辑 bySpecial亲格阑 概流可记俊 悸泼窍绊, iq甫 概流栏肺 官层辑 霖促.
#define EXEC_INPUT_WINDOW		0x57 // 抗距啊涝磊俊霸 矫府倔 锅龋 涝仿芒阑... 
#define EXEC_INPUTGAME_WINDOW	0x58 // 抗距啊涝 霸烙规俊霸 矫府倔 锅龋 涝仿芒阑... 	
//#define EXEC_ADD_GAMETIME_ITEM	0x59 // 惑前鼻阑 爱绊乐绰 沥咀鼻甫 雀荐茄促.
#define EXEC_SC_FUSHU_PIN	0x59    //删除机甲附属品
#define EXEC_CHANGE_UPGRADE_ACC 0x5A // 扁粮狼 咀技辑府甫 诀弊饭捞靛 啊瓷茄 咀技辑府肺 背券秦霖促.
#define EXEC_APPLY_GUILD_RUN	0x5B // 辨靛付扼沛 脚没
#define EXEC_RR_ENTER			0x5C // 肺锯烦喉 涝厘
#define EXEC_MON_SAY			0x5D // 涅胶飘甫困秦 各 say甫... 	
#define EXEC_SUMMON				0x5E // 涅胶飘 各阑 家券茄促.
#define EXEC_EBODY_DEFINE		0x5F // EBODY 犬牢 
#define EXEC_HELPERCURE			0x60 // Helper npc
#define EXEC_GIVE_PSI			0x61 // Gives user PSI
#define EXEC_TRANSFORM			0x62 // lvl 20 quest transform window
#define EXEC_100_1		0x63 // 100级防具转换
#define	EXEC_GIVE_HUFA			0x64        //领取护法
#define EXEC_100_3		0x65 // 添加及变更属性
#define EXEC_100_4		0x66 // 属性升级
#define EXEC_100_5		0x67 //防具材料化
#define EXEC_100_6		0x68  //解除材料化
#define RUN_SKILL		0x69  //解除材料化
#define EXEC_100_2		0x70 //解除属性限制
#define EXEC_GO_EMO				0x71	//进入恶魔广场
#define EXEC_NPC_SAYING			0x72    //抽奖
#define EXEC_OPEN_YUANBAO		0x73 //打开属性元宝商店
#define EXEC_JUANKUAN           0x74 //捐款3倍经验
#define EXEC_BAOLV              0x75 //捐款3倍幸运
#define EXCE_GOPK				0x76 //进入PK广场
//#define EXCE_ON_WEB	            0x77 //游戏中的网页
#define EXEC_PKSAY				0X78 //PK大塞对话框
#define	EXEC_SAYSHAREN			0x79	//杀人狂对话框
#define EXCE_GOSHAREN			0x83	//杀人狂大赛脚本
#define EXEC_NPC_DA				0X84	//赌博大
#define EXEC_NPC_XIAO			0X85	//赌博小
#define EXEC_CHANGE_JOB			0X86			//转职
#define EXEC_STUDY_NEW_JOB_PSI		0X87			//学新职业魔法
#define EXEC_CAN_GIVE_NEWER_ITEM	0X88			//给允许给新手装
#define EXEC_GIVE_NEWER_ITEM		0X89			//给新手装
#define EXEC_GIVE_NEWER_ITEM_FINISH	0X8a			//刷新显示
#define EXEC_ONLINE_EXCHANGE		0X95			//RMB兑换
#define EXCE_ON_SHOP	0x96 //在线商店
#define EXEC_CHANGE_ATTRIBUTE_ITEM		0X97 //白浩换有属性浩
#define	EXEC_OFF_JJ			0x82        //卸下机甲
#define EXEC_SEND_TOU			0x99 // NPC说话


//	Logic Else Define...
#define LOGIC_LEVEL					0X01
#define LOGIC_EXIST_EVENT			0X02
#define LOGIC_ABSENT_EVENT			0X03
#define LOGIC_EXIST_ITEM			0X04
#define LOGIC_ABSENT_ITEM			0X05
#define LOGIC_CLASS					0x06
#define LOGIC_WEIGHT				0x07
#define LOGIC_CHECK_ITEMSLOT		0x08		
#define LOGIC_EXIST_GM				0x09 // 辨靛付胶磐老 版快 烹苞
#define LOGIC_ZONE_WHO				0x10 // 捞悼且 粮俊 蜡历啊 荤侩吝俊 乐绰瘤 眉农
#define LOGIC_KILL_GH				0x11 // 辨靛窍快胶 鼻府甫 器扁
#define LOGIC_MASS_ZONE_MOVE		0x12 // 
#define LOGIC_TERM					0x13 // 老沥 朝楼 裹困救俊辑 角青茄促.	
#define LOGIC_GUILD_HOUSE_NUMBER	0x14 // 辨靛窍快胶 家蜡辨靛啊 乐绰瘤 眉农茄促.
#define LOGIC_CHECK_CV				0x15 // 矫刮 殿鞭阑 眉农茄促.
#define LOGIC_EXIST_EVENT_ITEM		0X16 // 惑前鼻阑 眉农茄促.
#define LOGIC_MONEY					0X17 // 捣阑 眉农茄促
#define LOGIC_RAND					0X18 // 犬伏阑 倒妨辑 犬伏俊 吧府搁 烹苞
#define LOGIC_OVER_GUILD_USER_COUNT 0X19
#define LOGIC_RR_TIME				0X1A // 肺锯烦喉 涝厘 啊瓷茄 矫埃牢瘤 眉农
#define LOGIC_RR_MAX_USER			0X1B // 肺锯烦喉 弥措 涝厘荐啊 酒流 救灯绰瘤 眉农
#define LOGIC_EXIST_SKILL           0X1C //百级学习什么技能
#define LOGIC_VIP					0X1D // NPC检测会员
#define LOGIC_LING					0X1E 
#define	EXEC_HUISHOU		        0x3E  
//
//	User Define Struct
//
typedef union{
	short int	i;
	BYTE		b[2];
} MYSHORT;

typedef union{
	__int64		i;
	BYTE		b[8];
} MYINT64;


//
//	User Status Value
//
#define USER_ABILITY_NUM		5		// 扁夯 惑怕 瓷仿摹 辆幅

#define USER_STR				0
#define USER_CON				1
#define USER_DEX				2
#define USER_VOL				3
#define USER_WIS				4

//
//	User Guild Require
//
#define GUILD_MAKE_LEVEL		50
#define GUILD_MAKE_DN			100000	

//
//	Default Start Position
//
#define DEFAULT_START_X			200
#define DEFAULT_START_Y			550

//
//	Magic Slot Num
//
#define MAGIC_NUM				11		// 概流 加己 浇吩

//
//	Item Quality
//
const BYTE	NORMAL_ITEM			= 0;
const BYTE	REMODEL_ITEM		= 1;
const BYTE	MAGIC_ITEM			= 2;
const BYTE	RARE_ITEM			= 3;
const BYTE	UNIQUE_ITEM			= 4;
const BYTE	SET_ITEM			= 5;
const BYTE	UPGRADE_ITEM		= 6;
const BYTE	REMODEL_MAGIC_ITEM	= 8;
const BYTE	GUARDIAN_ITEM		= 9;
const BYTE	REMODEL2_ITEM		= 10;//粉百及装备
const BYTE	MAGIC2_ITEM			= 11;//蓝百级
const BYTE	RARE2_ITEM			= 12;//金百级
const BYTE	GUARTIANSHI_ITEM	= 14;
const BYTE  SUPER_EBODY_ITEM	= 15;				//超级机械	
const BYTE	RARE3_ITEM			= 18;//130

const BYTE	EVENT_SP1_ITEM	= 201;		// 捞亥飘甫 困秦 冻绢瘤绰 归拳痢 惑前鼻 酒捞袍	
const BYTE	EVENT_SP2_ITEM	= 202;		// 捞亥飘甫 困秦 冻绢瘤绰 巩拳 惑前鼻 酒捞袍	
const BYTE	EVENT_DEF_ITEM	= 243;		// 捞亥飘甫 困秦 冻绢瘤绰 规绢备 背券 酒捞袍	
const BYTE	EVENT_ATT_ITEM	= 242;		// 捞亥飘甫 困秦 冻绢瘤绰 公扁 背券 酒捞袍	
const BYTE	EVENT_POT_ITEM	= 205;		// 捞亥飘甫 困秦 冻绢瘤绰 拱距 背券 酒捞袍	

const BYTE	EVENT_ITEM_MOON		= 220;	// 焊抚崔
const BYTE	EVENT_ITEM_SONGPEON	= 221;	// 价祈
const BYTE	EVENT_ITEM_BOX		= 222;	// 急拱惑磊

const BYTE	EVENT_ATT7_ITEM	= 240;		// 汲朝 捞亥飘甫困秦 
const BYTE	EVENT_DEF7_ITEM	= 241;		// 
const BYTE	EVENT_ATT6_ITEM	= 244;		// 
const BYTE	EVENT_DEF6_ITEM	= 245;		// 
const BYTE	EVENT_ATT4_ITEM	= 246;		// 
const BYTE	EVENT_DEF4_ITEM	= 247;		// 
const BYTE	EVENT_ATT3_ITEM	= 248;		// 
const BYTE	EVENT_DEF3_ITEM	= 249;		// 
const BYTE  EVENT_INIT_STAT_ITEM = 250;	// 必距
const BYTE  EVENT_USER_GAME_TIME = 252; // 俺牢 沥咀力

#define EVENT_ITEM_BAND		  100		

const BYTE	BREAK_NORMAL	= 100;
const BYTE	BREAK_MAGIC		= 101;
const BYTE	BREAK_REMODEL	= 102;
const BYTE	BREAK_RARE		= 103;
const BYTE	BREAK_UNIQUE	= 104;
const BYTE	BREAK_SET		= 105;

//
//	Item Wear Class
//
const BYTE	ATTACK_ITEM				= 1;
const BYTE	MATERIAL_ITEM			= 16; //俺炼 酒捞袍阑 困茄 
const UINT	MATERIAL_MAGIC_UP_ITEM	= 625; //诀弊饭捞靛甫 困茄 概流 酒捞袍 = 磊宏费
const UINT	MATERIAL_RARE_UP_ITEM	= 626; //诀弊饭捞靛甫 困茄 饭绢 酒捞袍 = 漠肺捞父
const UINT	MATERIAL_REMODEL_UP_ITEM	= 861;//俺炼 诀弊饭捞靛甫 困茄 酒捞袍 = 鸥饭费
const UINT	MATERIAL_EBODY_UP_ITEM	= 1236;//俺炼 诀弊饭捞靛甫 困茄 酒捞袍 = 鸥饭费

//
//	Item Upgrade Cost
//
#define ATTACK_UPGRADE_COST		200000	//	Weapon Upgrade Cost
#define DEFENSE_UPGRADE_COST	100000	//	Defense Upgrade Cost
#define ACCESSORI_UPGRADE_COST	10000	//	Accessori Upgrade Cost
#define ITEM_REMODELING_COST	50000	//	Item Remodeling Cost
#define ITEM_REMAGIC_COST		200000	//	魔法圣装费用
#define EBODY_UPGRADE_COST		200000	//	EBody Upgrade Cost
#define EBODY_IDENTIFY_COST		100000	//	EBody Indentify Cost
#define ATTACKCHAO_UPGRADE_COST		1000000	//	超灵超晶升级费用
#define DEFENSEWANG_UPGRADE_COST	1000000	//	晶灵升级费用


//
//	EBody 包访 家葛 XP
//
#define EBODY_UPGRADE_XP	2000
#define EBODY_IDENTIFY_XP	100
#define FANGJUCAILIAOHUA_XP	100


#define UPDATE_TICKS		500000

//
//	City Level
//
#define USER_PK				1

const short CHAONISE_RANK	= 0;	
const short DEATH_RANK		= 1;	
const short EVIL_RANK		= 2;	
const short VILLAIN_RANK	= 3;	
const short CAIN_RANK		= 4;

const short COMMOM_RANK		= 5;

const short CITIZEN_RANK	= 6;	
const short VIUAL_RANK		= 7;	
const short WARRIOR_RANK	= 8;	
const short HERO_RANK		= 9;	
const short SAINT_RANK		= 10;
const short GUARDANT_RANT	= 11;  //圣贤
const short	ANGEL_RANK		= 12;	//天使

#define CITY_RANK_INTERVAL	5;

#define USER_PK_TICKS		25000	//紫名时间

#define GUILD_HOUSE_MAX_NUM	7		// 霉锅掳 0篮 力寇, 6俺 辨靛 窍快胶

#define UPDATE_GUILD_INVEN_TIME		12
#define UPDATE_GUILD_CITY_TIME		1

#define SYSTEM_GUILD_BAND	900000

//
// User Event Max Num
// 用户事件最大值
#define MAX_EVENT_NUM		350
//
//	g_DynamicSkillInfo
//
const BYTE	MAX_SKILL_MAGIC_NUM	= SKILL_COUNT;

//
//	Item Data Version
//
#define CURRENT_ITEM_VERSION	1

//
//	ItemLog Type
//
#define ITEMLOG_PICKUP					1
#define ITEMLOG_THROW					2
#define ITEMLOG_GIVE					3
#define ITEMLOG_RECEIVE					4
#define ITEMLOG_EXCHANGE_GIVE			5
#define ITEMLOG_EXCHANGE_RECEIVE		6
#define ITEMLOG_SELL					7
#define ITEMLOG_BUY						8
#define ITEMLOG_ACCOUNT_BANKIN			9
#define ITEMLOG_ACCOUNT_BANKOUT			10
#define ITEMLOG_GUILD_BANKIN			11
#define ITEMLOG_GUILD_BANKOUT			12
#define ITEMLOG_UPGRADE_SUCCESS			13
#define ITEMLOG_UPGRADE_FAIL			14

#define ITEMLOG_MONEY_LOTTERY			15
#define ITEMLOG_MONEY_GIVE				16
#define ITEMLOG_MONEY_THROW				17
#define ITEMLOG_MONEY_PICKUP			18
#define ITEMLOG_EXCHANGE_GIVE_MONEY		19
#define ITEMLOG_EXCHANGE_RECEIVE_MONEY	20
#define ITEMLOG_SELL_MONEY				21
#define ITEMLOG_BUY_MONEY				22
#define ITEMLOG_ACCOUNT_BANKIN_MONEY	23
#define ITEMLOG_ACCOUNT_BANKOUT_MONEY	24
#define ITEMLOG_GUILD_BANKIN_MONEY		25
#define ITEMLOG_GUILD_BANKOUT_MONEY		26

#define ITEMLOG_EVENT_GIVE				27

#define ITEMLOG_DEAD_THROW				28

#define ITEMLOG_BLESS_UPGRADE_SUCCESS	29
#define ITEMLOG_BLESS_UPGRADE_FAIL		30

#define ITEMLOG_REMODEL_SUCCESS			31
#define ITEMLOG_REMODEL_FAIL			32

#define ITEMLOG_BLESS_USE				33		// 绵 酒捞袍 厚胶公府茄 酒捞袍阑 荤侩且措
#define ITEMLOG_SPECIAL_USE_POTION		34		// 漂喊茄 家葛己 拱距阑 荤侩且锭

#define ITEMLOG_PSI_STONE				35		// 荤捞坷葱 嚼垫备浇 荤侩矫 肺弊锅龋

#define ITEMLOG_ACC_USE					36		// 家葛己 厩技荤府啊 荤扼龙锭
#define ITEMLOG_REMAGIC_DELETE			37		// 府概流 矫胶袍栏肺 酒捞袍 昏力

#define ITEMLOG_DEAD_DELETE				38		// PK 肺 牢秦 酒捞袍捞 荤扼龙锭

#define ITEMLOG_EBODY_ADDER_DELETE		39		// EBody 诀弊饭捞靛肺 俊捞歹甫 家厚茄促.
#define ITEMLOG_EBODY_BASE_DELETE		40		// EBody 诀弊饭捞靛 角菩肺 海捞胶甫 昏力茄促.
#define ITEMLOG_EBODY_UPGRADE_SUCCESS	41		// EBody 诀弊饭捞靛 己傍

//
//	Royal Rumble Define
//

#define RR_EMPTY						-1
#define RR_IDLE							0
#define RR_APPLY						1
#define RR_ENTERING						2
#define RR_ENTERING_END					3
#define RR_START						4
#define RR_END							5

//
//	to detect Event Hacking
//
#define BUY_BLOCK						1
#define SELL_BLOCK						2
#define REPAIR_BLOCK					3
#define CURE_BLOCK						4
#define TEL_BLOCK						5

#define ITEM_MAX_RANGE					16

// Special Accessori
#define MAX_ACCESSORI		4			// 弥措 厩技荤府 肮荐
#define	RANDOM_PROTECT		30			// 辑器飘 蓖吧捞狼 农府萍拿 公矫 犬啦
#define	GUARDIAN_SLOT		1


//yskang 0.4 款康磊 肺弊扁废 可记
//0:立加酒捞乔 1:措拳 2:酒捞袍 力傍沥焊 3:PK沥焊
#define CONNECT_IP		0
#define CHAT_LOG		1
#define ITEM_LOG		2
#define	PKINFO_LOG		3
#define CALL_USER_LOG	4

#endif