#pragma once


#include <stdint.h>


#define ZSI_FLAG_None               0
#define ZSI_FLAG_Backup             1           // 备份数据标志志
#define ZSI_FLAG_NotPublish         2           // 还未广播数据
#define ZSI_FLAG_Deleted            4           // 删除标记

#define ZSTU_Version                10000
#define ZSTU_SimpleChange           true

typedef enum
{
	SSEX_Unknown    = 0,                        // 未知
	SSEX_Boy        = 1,                        // 男生
	SSEX_Girl       = 2                         // 女生
}StudentSex;

typedef enum
{
	SIM_Emergency   = 2,                        // 紧急
	SIM_Important   = 1,                        // 重要
	SIM_Normal      = 0                         // 普通
}StuImportant;

struct stZSumaryInfo
{
	int32_t         Version;                    // 版本
	uint32_t        Count;                      // 总个数
	int32_t         Cipher;                     // 加密方式
	char            Padding[52];
};

/* todo: add GPA field */

struct stZStudentInfo
{
	char            Name[16];                   // 姓名
	char            Telehone[16];               // 电话
	char            QQ[16];                     // QQ
	char            Class[16];                  // 年级
	char            CollegeFrom[64];            // 现就读大学
	char            CollegeTo[64];              // 意向大学
	char            MajorFrom[64];              // 现就读专业
	char            MajorTo[64];                // 意向专业
	char            Country[64];                // 意向国家：可多个国家
	char            Source[32];                 // 学生来源：活动/推荐等
	char            Status[32];                 // 目前状态（由于用户自定义描述）
	char            Recorder[12];               // 谁记录
	char            IDNumber[20];               // 身份证号
	char            EMail[32];                  // 邮箱地址

	uint32_t        Number;                     // 编号
	uint32_t        LanguageScore;              // 托福/雅思分数(* 10)
	uint32_t        Flag;                       // 数据标志
	StuImportant    ImportantLevel;             // 重要级别
	StudentSex      Sex;                        // 性别 1-boy 2-girl
	uint32_t        GPA;                        // 绩点（* 10）
	uint32_t        Reserve2;                   // 预留字段1
	uint32_t        Reserve3;                   // 预留字段2
	int64_t         InsertTime;                 // 录入时间 20180601143025
	int64_t         UpdateTime;                 // 更新时间
	int64_t         NextVisitTime;              // 下次回访时间(已废弃)

	char            Comments[1024];             // 备注
};
typedef struct stZStudentInfo ZStudentInfo;


extern int ZStuInfoFixString(const ZStudentInfo* apStuInfo, char* apBuffer, uint32_t aPrePaddingSize);

extern int ZFixString2StuInfo(char* apString, uint32_t aLength, uint32_t aPrePaddingSize, ZStudentInfo* apStuInfo);

extern void ZStuInfoCopy(ZStudentInfo* apDstInfo, const ZStudentInfo* apSrcInfo);

extern void ZMergeComments(ZStudentInfo* apDstInfo, const ZStudentInfo* apSrcInfo);

extern bool ZStuInfoEqual(ZStudentInfo* apStuInfoA, ZStudentInfo* apStuInfoB);
