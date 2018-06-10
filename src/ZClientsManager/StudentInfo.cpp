#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "StudentInfo.h"

#include "ZFixApi.h"

#include "ZUtility.h"

#include "ZComments.h"


int ZStuInfoFixString(const ZStudentInfo* apStuInfo, char* apBuffer, uint32_t aPrePaddingSize)
{
	ZFixApi lFixApi;
	lFixApi.SetBuffer(apBuffer + aPrePaddingSize);

	std::string lName   = ZConvDataToBase64(apStuInfo->Name, strlen(apStuInfo->Name), ZSTU_SimpleChange);
	std::string lTel    = ZConvDataToBase64(apStuInfo->Telehone, strlen(apStuInfo->Telehone), ZSTU_SimpleChange);
	std::string lQQ     = ZConvDataToBase64(apStuInfo->QQ, strlen(apStuInfo->QQ), ZSTU_SimpleChange);
	std::string lIDNum  = ZConvDataToBase64(apStuInfo->IDNumber, strlen(apStuInfo->IDNumber), ZSTU_SimpleChange);
	std::string lEMail  = ZConvDataToBase64(apStuInfo->EMail, strlen(apStuInfo->EMail), ZSTU_SimpleChange);

	// comments not use simple password change
	std::string lComments = ZConvDataToBase64(apStuInfo->Comments, strlen(apStuInfo->Comments), false);

	lFixApi.SetItem(ZFD_Version,		ZSTU_Version);

	lFixApi.SetItem(ZFD_Number,         apStuInfo->Number);
	lFixApi.SetItem(ZFD_Name,           lName);
	lFixApi.SetItem(ZFD_Telephone,		lTel);
	lFixApi.SetItem(ZFD_QQ,             lQQ);
	lFixApi.SetItem(ZFD_Class,          apStuInfo->Class);
	lFixApi.SetItem(ZFD_CollegeFrom,    apStuInfo->CollegeFrom);
	lFixApi.SetItem(ZFD_CollegeTo,      apStuInfo->CollegeTo);
	lFixApi.SetItem(ZFD_MajorFrom,      apStuInfo->MajorFrom);
	lFixApi.SetItem(ZFD_MajorTo,        apStuInfo->MajorTo);
	lFixApi.SetItem(ZFD_Country,        apStuInfo->Country);
	lFixApi.SetItem(ZFD_Source,         apStuInfo->Source);
	lFixApi.SetItem(ZFD_Status,         apStuInfo->Status);
	lFixApi.SetItem(ZFD_Recorder,       apStuInfo->Recorder);
	lFixApi.SetItem(ZFD_IDNumber,       lIDNum);
	lFixApi.SetItem(ZFD_LanScore,       apStuInfo->LanguageScore);
	lFixApi.SetItem(ZFD_Flag,           apStuInfo->Flag);
	lFixApi.SetItem(ZFD_Sex,            apStuInfo->Sex);
	lFixApi.SetItem(ZFD_ImportantLevel, apStuInfo->ImportantLevel);
	lFixApi.SetItem(ZFD_InsertTime,     apStuInfo->InsertTime);
	lFixApi.SetItem(ZFD_UpdateTime,     apStuInfo->UpdateTime);
	lFixApi.SetItem(ZFD_NextVisitTime,  apStuInfo->NextVisitTime);
	lFixApi.SetItem(ZFD_EMail,          apStuInfo->EMail);
	lFixApi.SetItem(ZFD_Comment,        lComments);

	return lFixApi.Length() + aPrePaddingSize;
}

static std::string _ZGetRawData(ZFixApi& aFixApi, uint32_t aFixID, bool aDoSimpleChange)
{
	std::string lValue;
	aFixApi.GetItem(aFixID, lValue);
	lValue = ZConvBase64ToData(lValue.c_str(), lValue.length(), aDoSimpleChange);
	return lValue;
}

int ZFixString2StuInfo(char* apString, uint32_t aLength, uint32_t aPrePaddingSize, ZStudentInfo* apStuInfo)
{
	ZFixApi lFixApi;
	lFixApi.SetBuffer(apString + aPrePaddingSize);
	lFixApi.SetLength(aLength - aPrePaddingSize);

	std::string lName   = _ZGetRawData(lFixApi, ZFD_Name, ZSTU_SimpleChange);
	std::string lTel    = _ZGetRawData(lFixApi, ZFD_Telephone, ZSTU_SimpleChange);
	std::string lQQ     = _ZGetRawData(lFixApi, ZFD_QQ, ZSTU_SimpleChange);
	std::string lIDNum  = _ZGetRawData(lFixApi, ZFD_IDNumber, ZSTU_SimpleChange);
	std::string lEMail  = _ZGetRawData(lFixApi, ZFD_EMail, ZSTU_SimpleChange);
	// comments not use simple password change
	std::string lComments = _ZGetRawData(lFixApi, ZFD_Comment, false);


	if (!lFixApi.GetItem(ZFD_Number, apStuInfo->Number)) {
		return -1;
	}

	strncpy(apStuInfo->Name, lName.c_str(), sizeof(apStuInfo->Name) - 1);
	strncpy(apStuInfo->Telehone, lTel.c_str(), sizeof(apStuInfo->Telehone) - 1);
	strncpy(apStuInfo->QQ, lQQ.c_str(), sizeof(apStuInfo->QQ) - 1);
	strncpy(apStuInfo->IDNumber, lIDNum.c_str(), sizeof(apStuInfo->IDNumber) - 1);
	strncpy(apStuInfo->EMail, lEMail.c_str(), sizeof(apStuInfo->EMail) - 1);

	lFixApi.GetItem(ZFD_Class,          apStuInfo->Class);
	lFixApi.GetItem(ZFD_CollegeFrom,    apStuInfo->CollegeFrom);
	lFixApi.GetItem(ZFD_CollegeTo,      apStuInfo->CollegeTo);
	lFixApi.GetItem(ZFD_MajorFrom,      apStuInfo->MajorFrom);
	lFixApi.GetItem(ZFD_MajorTo,        apStuInfo->MajorTo);
	lFixApi.GetItem(ZFD_Country,        apStuInfo->Country);
	lFixApi.GetItem(ZFD_Source,         apStuInfo->Source);
	lFixApi.GetItem(ZFD_Status,         apStuInfo->Status);
	lFixApi.GetItem(ZFD_Recorder,       apStuInfo->Recorder);
	lFixApi.GetItem(ZFD_LanScore,       apStuInfo->LanguageScore);
	lFixApi.GetItem(ZFD_Flag,           apStuInfo->Flag);
	lFixApi.GetItem(ZFD_InsertTime,     apStuInfo->InsertTime);
	lFixApi.GetItem(ZFD_UpdateTime,     apStuInfo->UpdateTime);
	lFixApi.GetItem(ZFD_NextVisitTime,  apStuInfo->NextVisitTime);

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
	int64_t lOldInsertTime = apDstInfo->InsertTime;
	std::string lOldComments = apDstInfo->Comments;

	memcpy(apDstInfo, apSrcInfo, sizeof(ZStudentInfo));
	apDstInfo->Number = lOldNum;

	if (lOldNum != 0)
		apDstInfo->Number = lOldNum;
	if (lOldInsertTime != 0)
		apDstInfo->InsertTime = lOldInsertTime;
	if (lOldComments.compare(apSrcInfo->Comments) != 0)
	{
		strncpy(apDstInfo->Comments, lOldComments.c_str(), sizeof(apDstInfo->Comments) - 1);
		ZMergeComments(apDstInfo, apSrcInfo);
	}
}

void ZMergeComments(ZStudentInfo* apDstInfo, const ZStudentInfo* apSrcInfo)
{
	// split comments by \r\n
	ZComments lDstVec(apDstInfo->Comments);
	ZComments lSrcVec(apSrcInfo->Comments);

	// remove the repeat records
	std::map <std::string, std::string> lMap;
	for (int i = 0; i < lSrcVec.GetCount(); ++i)
	{
		ZCommentLine& lSrcLine = *lSrcVec[i];
		if (lSrcLine[COMMENTSLIST_COL_Time].empty() || lSrcLine[COMMENTSLIST_COL_Content].empty())
		{
			ZDebug("ZMergeComments empty src string data %s", lSrcLine[i].c_str());
			continue;
		}

		lMap.insert(std::make_pair(lSrcLine[COMMENTSLIST_COL_Time], lSrcLine.GetCommentLine()));
	}
	for (int i = 0; i < lDstVec.GetCount(); ++i)
	{
		ZCommentLine& lDstLine = *lDstVec[i];
		if (lDstLine[COMMENTSLIST_COL_Time].empty() || lDstLine[COMMENTSLIST_COL_Content].empty())
		{
			ZDebug("ZMergeComments empty dst string data %s", lDstLine[i].c_str());
			continue;
		}

		lMap.insert(std::make_pair(lDstLine[COMMENTSLIST_COL_Time], lDstLine.GetCommentLine()));
	}

	// sort them
	vector<string> lVecKey;
	map<string, string>::iterator iter = lMap.begin();
	for (; iter != lMap.end(); ++iter)
		lVecKey.push_back(iter->first);
	std::sort(lVecKey.begin(), lVecKey.end());

	vector<string> lVecString;
	for (size_t i = 0; i < lVecKey.size(); ++i)
	{
		std::string lKey = lVecKey[i];
		std::string lData = lMap[lKey];
		if (lData.empty())
		{
			ZDebug("ZMergeComments got unexpected empty string data for %s", lKey.c_str());
			continue;
		}

		lVecString.push_back(lData);
	}

	// update each line's new number
	for (size_t i = 0; i < lVecString.size(); ++i)
	{
		// 将 lVecString 的所有字符串拼接为一个字符串，且编号需要重新组织
		ZCommentLine lCL(lVecString[i]);
		lCL.UpdateNumber(i + 1);
		lVecString[i] = lCL.GetCommentLine();
	}

	// make a new comments
	int  lLength = 0;
	char lBuffer[4096] = "";
	for (int i = (int)lVecString.size() - 1; i >= 0; --i)
	{
		lLength += sprintf(lBuffer + lLength, "%s", lVecString[i].c_str());
	}

	if (lLength > 0)
	{
		strncpy(apDstInfo->Comments, lBuffer, sizeof(apDstInfo->Comments) - 1);
	}
}

bool ZStuInfoEqual(ZStudentInfo* apStuInfoA, ZStudentInfo* apStuInfoB)
{
	if (strcmp(apStuInfoA->Name, apStuInfoB->Name) == 0 &&
		strcmp(apStuInfoA->Telehone, apStuInfoB->Telehone) == 0 &&
		strcmp(apStuInfoA->QQ, apStuInfoB->QQ) == 0 &&
		strcmp(apStuInfoA->Class, apStuInfoB->Class) == 0 &&
		strcmp(apStuInfoA->CollegeFrom, apStuInfoB->CollegeFrom) == 0 &&
		strcmp(apStuInfoA->CollegeTo, apStuInfoB->CollegeTo) == 0 &&
		strcmp(apStuInfoA->MajorFrom, apStuInfoB->MajorFrom) == 0 &&
		strcmp(apStuInfoA->MajorTo, apStuInfoB->MajorTo) == 0 &&
		strcmp(apStuInfoA->Country, apStuInfoB->Country) == 0 &&
		strcmp(apStuInfoA->Source, apStuInfoB->Source) == 0 &&
		strcmp(apStuInfoA->IDNumber, apStuInfoB->IDNumber) == 0 &&
		strcmp(apStuInfoA->EMail, apStuInfoB->EMail) == 0 &&
		strcmp(apStuInfoA->Comments, apStuInfoB->Comments) == 0 &&
		apStuInfoA->LanguageScore == apStuInfoB->LanguageScore &&
		apStuInfoA->Flag == apStuInfoB->Flag &&
		apStuInfoA->ImportantLevel == apStuInfoB->ImportantLevel &&
		apStuInfoA->GPA == apStuInfoB->GPA &&
		apStuInfoA->Sex == apStuInfoB->Sex
		) {
		return true;
	}

	return false;
}
