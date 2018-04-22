#include "StudentInfo.h"

#include "ZFixApi.h"

int ZStuInfoFixString(const ZStudentInfo* apStuInfo, char* apBuffer, uint32_t aPrePaddingSize)
{
	ZFixApi lFixApi;
	lFixApi.SetPrePaddingSize(aPrePaddingSize);
	lFixApi.SetBuffer(apBuffer);

	lFixApi.SetItem(ZFD_Number,         apStuInfo->Number);
	lFixApi.SetItem(ZFD_Name,           apStuInfo->Name);
	lFixApi.SetItem(ZFD_Telephone,      apStuInfo->Telehone);
	lFixApi.SetItem(ZFD_QQ,             apStuInfo->QQ);
	lFixApi.SetItem(ZFD_Class,          apStuInfo->Class);
	lFixApi.SetItem(ZFD_CollegeFrom,    apStuInfo->CollegeFrom);
	lFixApi.SetItem(ZFD_CollegeTo,      apStuInfo->CollegeTo);
	lFixApi.SetItem(ZFD_MajorFrom,      apStuInfo->MajorFrom);
	lFixApi.SetItem(ZFD_MajorTo,        apStuInfo->MajorTo);
	lFixApi.SetItem(ZFD_Country,        apStuInfo->Country);
	lFixApi.SetItem(ZFD_Source,         apStuInfo->Source);
	lFixApi.SetItem(ZFD_Status,         apStuInfo->Status);
	lFixApi.SetItem(ZFD_Recorder,       apStuInfo->Recorder);
	lFixApi.SetItem(ZFD_IDNumber,       apStuInfo->IDNumber);
	lFixApi.SetItem(ZFD_LanScore,       apStuInfo->LanguageScore);
	lFixApi.SetItem(ZFD_Flag,           apStuInfo->Flag);
	lFixApi.SetItem(ZFD_Sex,            apStuInfo->Sex);
	lFixApi.SetItem(ZFD_ImportantLevel, apStuInfo->ImportantLevel);
	lFixApi.SetItem(ZFD_InsertTime,     apStuInfo->InsertTime);
	lFixApi.SetItem(ZFD_UpdateTime,     apStuInfo->UpdateTime);
	lFixApi.SetItem(ZFD_NextVisitTime,  apStuInfo->NextVisitTime);
	lFixApi.SetItem(ZFD_Comment,        apStuInfo->Comments);

	return lFixApi.Length();
}


int ZFixString2StuInfo(char* apString, uint32_t aLength, uint32_t aPrePaddingSize, ZStudentInfo* apStuInfo)
{
	ZFixApi lFixApi;
	lFixApi.SetPrePaddingSize(aPrePaddingSize);
	lFixApi.SetBuffer(apString);

	if (!lFixApi.GetItem(ZFD_Number, apStuInfo->Number)) {
		return -1;
	}
	lFixApi.GetItem(ZFD_Name,           apStuInfo->Name);
	lFixApi.GetItem(ZFD_Telephone,      apStuInfo->Telehone);
	lFixApi.GetItem(ZFD_QQ,             apStuInfo->QQ);
	lFixApi.GetItem(ZFD_Class,          apStuInfo->Class);
	lFixApi.GetItem(ZFD_CollegeFrom,    apStuInfo->CollegeFrom);
	lFixApi.GetItem(ZFD_CollegeTo,      apStuInfo->CollegeTo);
	lFixApi.GetItem(ZFD_MajorFrom,      apStuInfo->MajorFrom);
	lFixApi.GetItem(ZFD_MajorTo,        apStuInfo->MajorTo);
	lFixApi.GetItem(ZFD_Country,        apStuInfo->Country);
	lFixApi.GetItem(ZFD_Source,         apStuInfo->Source);
	lFixApi.GetItem(ZFD_Status,         apStuInfo->Status);
	lFixApi.GetItem(ZFD_Recorder,       apStuInfo->Recorder);
	lFixApi.GetItem(ZFD_IDNumber,       apStuInfo->IDNumber);
	lFixApi.GetItem(ZFD_LanScore,       apStuInfo->LanguageScore);
	lFixApi.GetItem(ZFD_Flag,           apStuInfo->Flag);
	lFixApi.GetItem(ZFD_InsertTime,     apStuInfo->InsertTime);
	lFixApi.GetItem(ZFD_UpdateTime,     apStuInfo->UpdateTime);
	lFixApi.GetItem(ZFD_NextVisitTime,  apStuInfo->NextVisitTime);
	lFixApi.GetItem(ZFD_Comment,        apStuInfo->Comments);

	uint32_t lTempVal = 0;
	lFixApi.GetItem(ZFD_Sex, lTempVal);
	apStuInfo->Sex = (StudentSex)lTempVal;

	lTempVal = 0;
	lFixApi.GetItem(ZFD_ImportantLevel, lTempVal);
	apStuInfo->ImportantLevel = (StuImportant)lTempVal;

	return 0;
}

void ZStuInfoCopy(ZStudentInfo* apDstInfo, const ZStudentInfo* apSrcInfo)
{
	uint32_t lOldNum = apDstInfo->Number;
	memcpy(apDstInfo, apSrcInfo, sizeof(ZStudentInfo));
	apDstInfo->Number = lOldNum;
}
