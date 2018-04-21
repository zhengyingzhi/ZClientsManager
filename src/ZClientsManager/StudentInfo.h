#pragma once


#include <stdint.h>


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
	char            CollegeFrom[64];            // �־Ͷ���ѧ
	char            CollegeTo[64];              // �����ѧ
	char            MajorFrom[64];              // �־Ͷ�רҵ
	char            MajorTo[64];                // ����רҵ
	char            Country[64];                // ������ң��ɶ������
	char            Source[32];                 // ѧ����Դ���/�Ƽ���
	char            Status[32];                 // Ŀǰ״̬�������û��Զ���������
	char            Recorder[12];               // ˭��¼
	char			IDNumber[20];               // ���֤��

	uint32_t        Number;                     // ���
	uint32_t        LanguageScore;              // �и�/��˼����(* 10)
	uint32_t        Flag;                       // ���ݱ�־
	StuImportant    ImportantLevel;             // ��Ҫ����
	StudentSex      Sex;                        // �Ա� 1-boy 2-girl
	uint32_t        Reserve1;                   // Ԥ���ֶ�1
	uint32_t        Reserve2;                   // Ԥ���ֶ�2
	uint32_t        Reserve3;                   // Ԥ���ֶ�3
	int64_t         InsertTime;                 // ¼��ʱ��
	int64_t         UpdateTime;                 // ����ʱ��
	int64_t         NextVisitTime;              // �´λط�ʱ��

	char            Comments[1024];             // ��ע
};
typedef struct stZStudentInfo ZStudentInfo;

