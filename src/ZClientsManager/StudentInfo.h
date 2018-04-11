#pragma once


#include <stdint.h>


#define MAINLIST_COL_Row            0           // �����б���
#define MAINLIST_COL_Name           1           // ����
#define MAINLIST_COL_Telephone      2           // �绰
#define MAINLIST_COL_Country        3           // �������
#define MAINLIST_COL_College        4           // ��ѧ
#define MAINLIST_COL_Major          5           // רҵ
#define MAINLIST_COL_Class          6           // �꼶
#define MAINLIST_COL_LangScore      7           // ����
#define MAINLIST_COL_Sex            8           // �Ա�
#define MAINLIST_COL_QQ             9           // QQ
#define MAINLIST_COL_Source         10          // ��Դ
#define MAINLIST_COL_Status         11          // ״̬
#define MAINLIST_COL_InsertTime     12          // ����ʱ��
#define MAINLIST_COL_UpdateTime     13          // ����ʱ��
#define MAINLIST_COL_NextVisitTime  14          // �´λط�ʱ��
#define MAINLIST_COL_Important      15          // ��Ҫ����

#define ZSI_FLAG_None               0
#define ZSI_FLAG_Backup             1	        // �������ݱ�־
#define ZSI_FLAG_Deleted            2	        // ɾ�����

typedef enum
{
	SSEX_Unknown    = 0,                        // δ֪
	SSEX_Boy        = 1,                        // ����
	SSEX_Girl       = 2                         // Ů��
}StudentSex;

typedef enum
{
	SIM_Normal		= 0,                        // ��ͨ
	SIM_Important   = 1,                        // ��Ҫ
	SIM_Emergency   = 2                         // ����
}StuImportant;

struct stZSumaryInfo
{
	int32_t         Version;                    // �汾
	uint32_t        Count;                      // �ܸ���
	int32_t         Cipher;                     // ���ܷ�ʽ
	char            Padding[52];
};

struct stZStudentInfo
{
	char            Name[16];                   // ����
	char            Telehone[16];               // �绰
	char            QQ[16];                     // QQ
	char            Class[16];                  // �꼶
	char            College[64];                // ��ѧ
	char            Major[64];                  // רҵ
	char            Country[64];                // ������ң��ɶ������
	char            Source[32];                 // ѧ����Դ���/�Ƽ���
	char            Status[32];                 // Ŀǰ״̬�������û��Զ���������
	char            Recorder[32];               // ˭��¼

	uint32_t        Number;                     // ���
	float           LanguageScore;              // �и�/��˼����
	uint32_t        Flag;                       // ���ݱ�־
	StuImportant    ImportantLevel;             // ��Ҫ����
	StudentSex      Sex;                        // �Ա� 1-boy 2-girl
	uint32_t        Reserve1;                   // Ԥ���ֶ�1
	uint32_t        Reserve2;                   // Ԥ���ֶ�2
	uint32_t        Reserve3;                   // Ԥ���ֶ�3
	uint64_t        InsertTime;                 // ¼��ʱ��
	uint64_t        UpdateTime;                 // ����ʱ��
	uint64_t        NextVisitTime;              // �´λط�ʱ��

	char            Comments[1024];             // ��ע
};
typedef struct stZStudentInfo ZStudentInfo;

