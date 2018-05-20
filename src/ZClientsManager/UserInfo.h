#pragma once

#include <stdint.h>


#define ZUSER_ROLE_Visitor				0           // 访客
#define ZUSER_ROLE_Normal				1           // 普通用户（仅可浏览数据）
#define ZUSER_ROLE_Admin				2           // 普通管理员（可添加、删除客户信息）
#define ZUSER_ROLE_Root					4           // 超级管理员（可管理用户和客户信息）

#define ZUSER_CIPHER_None				0           // 未加密
#define ZUSER_CIPHER_Simple				1           // 简单加密
#define ZUSER_CIPHER_AES				2           // AES加密

#define ZUSER_FLAG_Deleted				2

#define ZUSER_DEFAULT_RootName			"root"
#define ZUSER_DEFAULT_RootPasswd		"111111"

#define ZUSER_INFO_Version				10000

#define ZUSER_SimpleChange              true


typedef struct tagZUserInfo
{
	char        UserName[24];           // 名字
	char        Password[24];           // 密码
	char        Telephone[16];          // 电话
	char        QQ[16];                 // QQ
	char		IDNumber[20];			// 身份证号
	uint32_t    Number;                 // 编号
	uint32_t    Role;                   // 角色
	uint32_t	Department;             // 部门
	int32_t     Cipher;                 // 加密方式
	int32_t		ExpireDate;				// 结束日期
	int64_t     InsertTime;             // 插入时间
	int64_t     UpdateTime;             // 更新时间
	uint32_t    Flags;                  // 预留字段
	char        Padding1[32];           //
	char        Padding2[128];          //
	char        Comment[256];           // 备注
}ZUserInfo;


extern int ZUserInfoFixString(const ZUserInfo* apUserInfo, char* apBuffer, uint32_t aPrePaddingSize);

extern int ZFixString2UserInfo(char* apString, uint32_t aLength, uint32_t aPrePaddingSize, ZUserInfo* apUserInfo);

extern void ZUserInfoCopy(ZUserInfo* apDstInfo, const ZUserInfo* apSrcInfo);
