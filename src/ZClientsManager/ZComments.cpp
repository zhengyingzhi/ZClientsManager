#include <assert.h>
#include "ZComments.h"

#include "ZUtility.h"


ZCommentLine::ZCommentLine(const std::string& aCommentLine, char aDelemeter)
{
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

