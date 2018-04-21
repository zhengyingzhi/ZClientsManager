#pragma once


#include <stdint.h>


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
	char            CollegeFrom[64];            // 现就读大学
	char            CollegeTo[64];              // 意向大学
	char            MajorFrom[64];              // 现就读专业
	char            MajorTo[64];                // 意向专业
	char            Country[64];                // 意向国家：可多个国家
	char            Source[32];                 // 学生来源：活动/推荐等
	char            Status[32];                 // 目前状态（由于用户自定义描述）
	char            Recorder[12];               // 谁记录
	char			IDNumber[20];               // 身份证号

	uint32_t        Number;                     // 编号
	uint32_t        LanguageScore;              // 托福/雅思分数(* 10)
	uint32_t        Flag;                       // 数据标志
	StuImportant    ImportantLevel;             // 重要级别
	StudentSex      Sex;                        // 性别 1-boy 2-girl
	uint32_t        Reserve1;                   // 预留字段1
	uint32_t        Reserve2;                   // 预留字段2
	uint32_t        Reserve3;                   // 预留字段3
	int64_t         InsertTime;                 // 录入时间
	int64_t         UpdateTime;                 // 更新时间
	int64_t         NextVisitTime;              // 下次回访时间

	char            Comments[1024];             // 备注
};
typedef struct stZStudentInfo ZStudentInfo;

