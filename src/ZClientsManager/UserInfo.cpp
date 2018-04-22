#include "UserInfo.h"

#include "ZFixApi.h"

int ZUserInfoFixString(const ZUserInfo* apUserInfo, char* apBuffer, uint32_t aPrePaddingSize)
{
	ZFixApi lFixApi;
	lFixApi.SetPrePaddingSize(aPrePaddingSize);
	lFixApi.SetBuffer(apBuffer);

	lFixApi.SetItem(ZFD_Number,     apUserInfo->Number);
	lFixApi.SetItem(ZFD_Name,       apUserInfo->UserName);
	lFixApi.SetItem(ZFD_Password,   apUserInfo->Password);
	lFixApi.SetItem(ZFD_Cipher,     apUserInfo->Cipher);
	lFixApi.SetItem(ZFD_Telephone,  apUserInfo->Telephone);
	lFixApi.SetItem(ZFD_QQ,         apUserInfo->QQ);
	lFixApi.SetItem(ZFD_IDNumber,   apUserInfo->IDNumber);
	lFixApi.SetItem(ZFD_Role,       apUserInfo->Role);
	lFixApi.SetItem(ZFD_Department, apUserInfo->Department);
	lFixApi.SetItem(ZFD_ExpireDate, apUserInfo->ExpireDate);
	lFixApi.SetItem(ZFD_Flag,       apUserInfo->Flags);
	lFixApi.SetItem(ZFD_Comment,    apUserInfo->Comment);

	return lFixApi.Length();
}

int ZFixString2UserInfo(char* apString, uint32_t aLength, uint32_t aPrePaddingSize, ZUserInfo* apUserInfo)
{
	ZFixApi lFixApi;
	lFixApi.SetPrePaddingSize(aPrePaddingSize);
	lFixApi.SetBuffer(apString);

	if (!lFixApi.GetItem(ZFD_Number, apUserInfo->Number)) {
		return -1;
	}

	lFixApi.GetItem(ZFD_Name,       apUserInfo->UserName);
	lFixApi.GetItem(ZFD_Password,   apUserInfo->Password);
	lFixApi.GetItem(ZFD_Cipher,     apUserInfo->Cipher);
	lFixApi.GetItem(ZFD_Telephone,  apUserInfo->Telephone);
	lFixApi.GetItem(ZFD_QQ,         apUserInfo->QQ);
	lFixApi.GetItem(ZFD_IDNumber,   apUserInfo->IDNumber);
	lFixApi.GetItem(ZFD_Role,       apUserInfo->Role);
	lFixApi.GetItem(ZFD_Department, apUserInfo->Department);
	lFixApi.GetItem(ZFD_ExpireDate, apUserInfo->ExpireDate);
	lFixApi.GetItem(ZFD_Flag,       apUserInfo->Flags);
	lFixApi.GetItem(ZFD_Comment,    apUserInfo->Comment);

	return 0;
}

void ZUserInfoCopy(ZUserInfo* apDstInfo, const ZUserInfo* apSrcInfo)
{
	// only copy some fields
	strcpy(apDstInfo->UserName, apSrcInfo->UserName);
	strcpy(apDstInfo->Password, apSrcInfo->Password);
	strcpy(apDstInfo->Telephone, apSrcInfo->Telephone);
	strcpy(apDstInfo->QQ, apSrcInfo->QQ);
	strcpy(apDstInfo->IDNumber, apSrcInfo->IDNumber);

	apDstInfo->Role = apSrcInfo->Role;
	apDstInfo->Department = apSrcInfo->Department;
	apDstInfo->Cipher = apSrcInfo->Cipher;
	apDstInfo->ExpireDate = apSrcInfo->ExpireDate;
	apDstInfo->InsertTime = apSrcInfo->InsertTime;
	apDstInfo->UpdateTime = apSrcInfo->UpdateTime;
	apDstInfo->Flags = apSrcInfo->Flags;
	strcpy(apDstInfo->Comment, apSrcInfo->Comment);
}
