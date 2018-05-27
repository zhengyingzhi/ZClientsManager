#include "UserInfo.h"

#include "ZFixApi.h"

#include "ZUtility.h"


int ZUserInfoFixString(const ZUserInfo* apUserInfo, char* apBuffer, uint32_t aPrePaddingSize)
{
	ZFixApi lFixApi;
	lFixApi.SetBuffer(apBuffer + aPrePaddingSize);

	std::string lPassword = ZConvDataToBase64(apUserInfo->Password, strlen(apUserInfo->Password), ZUSER_SimpleChange);

	lFixApi.SetItem(ZFD_Version,    ZUSER_INFO_Version);
	lFixApi.SetItem(ZFD_Number,     apUserInfo->Number);
	lFixApi.SetItem(ZFD_Name,       apUserInfo->UserName);
	lFixApi.SetItem(ZFD_Password,   lPassword);
	lFixApi.SetItem(ZFD_Cipher,     apUserInfo->Cipher);
	lFixApi.SetItem(ZFD_Telephone,  apUserInfo->Telephone);
	lFixApi.SetItem(ZFD_QQ,         apUserInfo->QQ);
	lFixApi.SetItem(ZFD_IDNumber,   apUserInfo->IDNumber);
	lFixApi.SetItem(ZFD_Role,       apUserInfo->Role);
	lFixApi.SetItem(ZFD_Department, apUserInfo->Department);
	lFixApi.SetItem(ZFD_ExpireDate, apUserInfo->ExpireDate);
	lFixApi.SetItem(ZFD_Flag,       apUserInfo->Flags);
	lFixApi.SetItem(ZFD_Comment,    apUserInfo->Comment);

	return lFixApi.Length() + aPrePaddingSize;
}

int ZFixString2UserInfo(char* apString, uint32_t aLength, uint32_t aPrePaddingSize, ZUserInfo* apUserInfo)
{
	ZFixApi lFixApi;
	lFixApi.SetBuffer(apString + aPrePaddingSize);
	lFixApi.SetLength(aLength - aPrePaddingSize);

	if (!lFixApi.GetItem(ZFD_Number, apUserInfo->Number)) {
		return -1;
	}

	std::string lPassword;
	lFixApi.GetItem(ZFD_Password, lPassword);
	lPassword = ZConvBase64ToData(lPassword.c_str(), lPassword.length(), ZUSER_SimpleChange);
	strncpy(apUserInfo->Password, lPassword.c_str(), sizeof(apUserInfo->Password) - 1);

	lFixApi.GetItem(ZFD_Name,       apUserInfo->UserName);
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
