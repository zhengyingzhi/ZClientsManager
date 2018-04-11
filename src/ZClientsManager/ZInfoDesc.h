#pragma once

#include "UserInfo.h"
#include "StudentInfo.h"


/* User role desc */
const char* ZUserRoleDesc(uint32_t aUserRole)
{
	switch (aUserRole)
	{
	case ZUSER_ROLE_Visitor:    return "�ÿ�";
	case ZUSER_ROLE_Normal:     return "��ͨ�û�";
	case ZUSER_ROLE_Admin:      return "��ͨ����Ա";
	case ZUSER_ROLE_Root:       return "��������Ա";
	default:
		break;
	}
	return "";
}

/* �Ա� */
const char* ZStuSexDesc(uint32_t aSex)
{
	switch (aSex)
	{
	case SSEX_Boy:    return "��";
	case SSEX_Girl:   return "Ů";
	default:
		break;
	}
	return "δ֪";
}

/* ��Ҫ���� */
const char* ZStuImportantDesc(uint32_t aImportant)
{
	switch (aImportant)
	{
	case SIM_Normal:    return "��ͨ";
	case SIM_Important: return "��Ҫ";
	case SIM_Emergency: return "����";
	default:
		break;
	}
	return "��ͨ";
}

