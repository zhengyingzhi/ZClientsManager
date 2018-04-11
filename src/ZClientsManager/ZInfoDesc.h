#pragma once

#include "UserInfo.h"
#include "StudentInfo.h"


/* User role desc */
const char* ZUserRoleDesc(uint32_t aUserRole)
{
	switch (aUserRole)
	{
	case ZUSER_ROLE_Visitor:    return "访客";
	case ZUSER_ROLE_Normal:     return "普通用户";
	case ZUSER_ROLE_Admin:      return "普通管理员";
	case ZUSER_ROLE_Root:       return "超级管理员";
	default:
		break;
	}
	return "";
}

/* 性别 */
const char* ZStuSexDesc(uint32_t aSex)
{
	switch (aSex)
	{
	case SSEX_Boy:    return "男";
	case SSEX_Girl:   return "女";
	default:
		break;
	}
	return "未知";
}

/* 重要级别 */
const char* ZStuImportantDesc(uint32_t aImportant)
{
	switch (aImportant)
	{
	case SIM_Normal:    return "普通";
	case SIM_Important: return "重要";
	case SIM_Emergency: return "紧急";
	default:
		break;
	}
	return "普通";
}

