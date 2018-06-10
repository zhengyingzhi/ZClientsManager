#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

#define COMMENTSLIST_COL_Row        0
#define COMMENTSLIST_COL_ByName     1
#define COMMENTSLIST_COL_Time       2
#define COMMENTSLIST_COL_Content    3

class ZCommentLine
{
public:
	ZCommentLine(const std::string& aCommentLine, char aDelemeter = '|');
	~ZCommentLine();

public:
	void UpdateNumber(std::string& aNumberStr);
	void UpdateNumber(int aNumber);

	std::string GetValue(int aIndex);

	std::string GetCommentLine();

	size_t GetSize();

	std::string operator [](int aIndex) {
		return GetValue(aIndex);
	}

private:
	std::string m_CommentLine;
	std::vector<std::string> m_CommentVec;
	char m_Delemeter;
};

class ZComments
{
public:
	ZComments(const std::string& aComments, const char* apDelemeter = "\r\n");
	~ZComments();

	ZCommentLine* GetCommentLine(int aIndex);

	ZCommentLine* operator [](int aIndex) {
		return GetCommentLine(aIndex);
	}

	std::string GetComments();

	int GetCount();

	void Reset(const std::string& aComments, const char* apDelemeter);

private:
	std::string m_Comments;
	vector<ZCommentLine*> m_CommentLines;
};
