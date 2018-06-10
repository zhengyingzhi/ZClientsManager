#include <assert.h>
#include "ZComments.h"

#include "ZUtility.h"


ZCommentLine::ZCommentLine(const std::string& aCommentLine, char aDelemeter)
{
	m_Delemeter = aDelemeter;
	m_CommentLine = aCommentLine;
	m_CommentVec = ZStringSplit(aCommentLine, aDelemeter);
}

ZCommentLine::~ZCommentLine()
{}

void ZCommentLine::UpdateNumber(std::string& aNumberStr)
{
	if (aNumberStr.empty() || m_CommentVec.empty()) {
		return;
	}

	m_CommentVec[COMMENTSLIST_COL_Row] = aNumberStr;

	// update new since we update
	int  lLength = 0;
	char lBuffer[4090] = "";
	for (size_t i = 0; i < m_CommentVec.size(); ++i)
	{
		lLength += sprintf(lBuffer + lLength, "%s", m_CommentVec[i].c_str());

		if (i != m_CommentVec.size() - 1)
			lBuffer[lLength++] = m_Delemeter;
	}

	m_CommentLine = lBuffer;
}

void ZCommentLine::UpdateNumber(int aNumber)
{
	char lBuffer[32] = "";
	sprintf(lBuffer, "%d", aNumber);
	UpdateNumber(std::string(lBuffer));
}

std::string ZCommentLine::GetValue(int aIndex)
{
	assert(aIndex >= 0);

	if (aIndex >= (int)m_CommentVec.size()) {
		return "";
	}

	return m_CommentVec[aIndex];
}

std::string ZCommentLine::GetCommentLine()
{
	return m_CommentLine;
}

size_t ZCommentLine::GetSize()
{
	return m_CommentVec.size();
}

//////////////////////////////////////////////////////////////////////////
ZComments::ZComments(const std::string& aComments, const char* apDelemeter)
{
	if (!aComments.empty())
	{
		m_Comments = aComments;
		Reset(aComments, apDelemeter);
	}
}

ZComments::~ZComments()
{
	for (size_t i = 0; i < m_CommentLines.size(); ++i)
	{
		delete m_CommentLines[i];
	}
}

void ZComments::Reset(const std::string& aComments, const char* apDelemeter)
{
	const char* lpCur = aComments.c_str();
	const char* lpEnd = strstr(lpCur, apDelemeter);
	while (lpEnd)
	{
		std::string lLine = std::string(lpCur, int(lpEnd - lpCur + 2));
		if (lLine.empty())
		{
			lpCur = lpEnd + 2;
			lpEnd = strstr(lpCur, apDelemeter);
			continue;
		}

		ZCommentLine* lpCommLine = new ZCommentLine(lLine);
		if (lpCommLine->GetSize() >= 4)
		{
			m_CommentLines.push_back(lpCommLine);
		}
		else
		{
			ZDebug("ZComments line invalid size %d", lLine.c_str());
			delete lpCommLine;
		}

		lpCur = lpEnd + 2;
		lpEnd = strstr(lpCur, apDelemeter);
	}
}

ZCommentLine* ZComments::GetCommentLine(int aIndex)
{
	if (aIndex < 0 || aIndex >= (int)m_CommentLines.size()) {
		return NULL;
	}

	return m_CommentLines[aIndex];
}

std::string ZComments::GetComments()
{
	return m_Comments;
}

int ZComments::GetCount()
{
	return (int)m_CommentLines.size();
}
