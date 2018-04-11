#pragma once


#include <stdint.h>


#define MAINLIST_COL_Row            0           // 仅是列表编号
#define MAINLIST_COL_Name           1           // 名字
#define MAINLIST_COL_Telephone      2           // 电话
#define MAINLIST_COL_Country        3           // 意向国家
#define MAINLIST_COL_College        4           // 大学
#define MAINLIST_COL_Major          5           // 专业
#define MAINLIST_COL_Class          6           // 年级
#define MAINLIST_COL_LangScore      7           // 分数
#define MAINLIST_COL_Sex            8           // 性别
#define MAINLIST_COL_QQ             9           // QQ
#define MAINLIST_COL_Source         10          // 来源
#define MAINLIST_COL_Status         11          // 状态
#define MAINLIST_COL_InsertTime     12          // 插入时间
#define MAINLIST_COL_UpdateTime     13          // 更新时间
#define MAINLIST_COL_NextVisitTime  14          // 下次回访时间
#define MAINLIST_COL_Important      15          // 重要级别

#define ZSI_FLAG_None               0
#define ZSI_FLAG_Backup             1	        // 备份数据标志
#define ZSI_FLAG_Deleted            2	        // 删除标记

typedef enum
{
	SSEX_Unknown    = 0,                        // 未知
	SSEX_Boy        = 1,                        // 男生
	SSEX_Girl       = 2                         // 女生
}StudentSex;

typedef enum
{
	SIM_Normal		= 0,                        // 普通
	SIM_Important   = 1,                        // 重要
	SIM_Emergency   = 2                         // 紧急
}StuImportant;

struct stZSumaryInfo
{
	int32_t         Version;                    // 版本
	uint32_t        Count;                      // 总个数
	int32_t         Cipher;                     // 加密方式
	char            Padding[52];
};

struct stZStudentInfo
{
	char            Name[16];                   // 姓名
	char            Telehone[16];               // 电话
	char            QQ[16];                     // QQ
	char            Class[16];                  // 年级
	char            College[64];                // 大学
	char            Major[64];                  // 专业
	char            Country[64];                // 意向国家：可多个国家
	char            Source[32];                 // 学生来源：活动/推荐等
	char            Status[32];                 // 目前状态（由于用户自定义描述）
	char            Recorder[32];               // 谁记录

	uint32_t        Number;                     // 编号
	float           LanguageScore;              // 托福/雅思分数
	uint32_t        Flag;                       // 数据标志
	StuImportant    ImportantLevel;             // 重要级别
	StudentSex      Sex;                        // 性别 1-boy 2-girl
	uint32_t        Reserve1;                   // 预留字段1
	uint32_t        Reserve2;                   // 预留字段2
	uint32_t        Reserve3;                   // 预留字段3
	uint64_t        InsertTime;                 // 录入时间
	uint64_t        UpdateTime;                 // 更新时间
	uint64_t        NextVisitTime;              // 下次回访时间

	char            Comments[1024];             // 备注
};
typedef struct stZStudentInfo ZStudentInfo;

