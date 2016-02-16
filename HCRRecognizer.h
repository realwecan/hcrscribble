// HCRRecognizer.h: interface for the CHCRRecognizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HCRRECOGNIZER_H__C9866CFA_057D_4D39_9786_53FA1E28418E__INCLUDED_)
#define AFX_HCRRECOGNIZER_H__C9866CFA_057D_4D39_9786_53FA1E28418E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "HCRScribbleDoc.h"						// Ϊ������AFX Template��CStroke��


#define MAX_POINTS 5000							// ����DDA�㷨���������ĵ�����һ����ԭ����������10��
#define SIDE_LENGTH 64							// ���Թ�һ����ı߳�
#define PATTERN_SIZE 64							// �����ı߳���δ��άǰ��һ���SIDE_LENGTH��ͬ��
#define GF_PATTERN_SIZE 8						// ͨ����˹�˲���ά���ģʽͼ��ı߳�
#define OVRALL_EMESH_PARAM 8					// ȫ�ֵ�������������������˸�˹�˲������˲��е�������

#define NUM_TRAIN 45							// ѵ���ֿ������
#define NUM_TEST 15								// �����ֿ������
#define NUM_KANJI 3755							// ���ݿ��еĺ�������



class CHCRRecognizer  
{

// �������������
public:
	CHCRRecognizer();							// �޲ι��캯���Ǳ����
	virtual ~CHCRRecognizer();					// ��������


// ��Ա����

public:
	int curfea;																	// ��ǰʹ�õ�������Ĭ��Ϊ0�����Ľ��ķ���任������
																				// 1��ʾ����任������2��ʾ8����������
	int result[10];																// ���ڴ洢ʶ���������ֵı�š�

protected:
	
	CPoint m_points[MAX_POINTS];												// ���ڴ洢�ּ�������
	int ptnum;																	// ����ָ��m_points[]�ּ��еĵ���
	//int GetPtNum() { return ptnum; }											// �õ�m_points[]�еĵ���
	float m_fea[8][PATTERN_SIZE][PATTERN_SIZE];									// ����
	float m_maxfea[8][PATTERN_SIZE][PATTERN_SIZE];								// ���ֵ�˲��������
	float m_gftfea[8][GF_PATTERN_SIZE][GF_PATTERN_SIZE];						// ��˹�˲��������
	float m_pfea[NUM_KANJI][8][GF_PATTERN_SIZE][GF_PATTERN_SIZE];				// ����ģ��

	float m_fGTable[625];														// ��ά��˹����ֵ��25*25�������Ϳ��Ϊ12��
	CPoint m_fPoints[OVRALL_EMESH_PARAM][OVRALL_EMESH_PARAM];					// ��˹�˲������ĵ����꣨���������е㣩


// ��Ա����
public:

	// ʵ�ú���
	// TODO: �ѱ���Ҫʶ�������еĺ������õĺ����������

	double CHCRRecognizer::TwoPtDist(CPoint pt1, CPoint pt2);
								// ���������ŷʽ����

	BOOL CHCRRecognizer::IsClockwise(CPoint pt1, CPoint pt2, CPoint pt3);
								// �ж�������pt1,pt2,pt3�Ƿ���˳ʱ�����У�������ʱ�����С�

	int CHCRRecognizer::IsOnSingleLine(CPoint pt1, CPoint pt2, CPoint pt3);
								// �ж�pt1,pt2,pt3�����Ƿ��һֱ�ߡ�������ǣ�����-1������ǵĻ�������0��1��
								// ���pt0---->pt1---->pt2������0��
								// ���pt0-----pt2<--->pt1������1��

	int CHCRRecognizer::CalcDegCh(CPoint ptPrev, CPoint ptCur, CPoint ptNext);
								// ���� Line(ptPrev,ptCur) �� Line(ptCur,ptNext)֮ǰ�ļнǡ�����ֵ�����Ǹ�ֵ��
								// ��ʱ��ļн�Ϊ����˳ʱ��Ϊ�����н�ȡֵ��Χ��(-180 deg, 180 deg]�� 

	int CHCRRecognizer::CalcDirection(CPoint ptCur, CPoint ptNext);
								// ���� Vector(ptCur, ptNext) �ķ��򡣷���ֵ��[0..7]�е�һ������������
								// �����8�����š�

	int CHCRRecognizer::CalcDirectionWithOffset(CPoint ptCur, CPoint ptNext);
								// ���� Vector(ptCur, ptNext) �ķ��򣬲��Ҵ��� 22.5deg �� offset��
								// ����ֵ��[0..7]�е�һ�����������ص�������8��������ı�š�

	void CHCRRecognizer::GenGTable();
								// ���ɶ�ά��˹����ֵ��

	int CHCRRecognizer::ReadOne863OnlineCharcter(FILE *fpc,int i_wordindex);
								// ��863Online���ݿ��ж�ȡһ�����֣����ֵı����[0..3754]֮�䡣

	void CHCRRecognizer::ReadAll863OnlineCharacter(CString filename, int dataset_counter);
								// ��ȡ����863Online���ݿ⡣dataset_counterֻ������д״̬������Ϣ��û���������á�
								// �������д��m_pfea[][][][]�����С�

	void CHCRRecognizer::TrainClassifier();
								// ѵ�������������ݼ���������NUM_TRAINָ����

	void CHCRRecognizer::LoadClassifier(CString filename);
								// ���ļ��лָ�������������ģ�塣

	void CHCRRecognizer::TestAll863OnlineCharacter(CString filename,int dataset_counter);
								// ����һ��863Online���ݿ��е����к��֡�
								// ����һ��������ָ���ж��ٺ��ֱ������ʶ��
								// ʶ�����ǣ�(return value / 3755 * 100%).

	void CHCRRecognizer::TestRecognizer();
								// ���Է����������ݼ���������NUM_TESTָ����

	void CHCRRecognizer::DrawOverallElasticMesh();
								// ��ȫ�ֵ������񣬽�����������е�д�뵽 m_fPoints[OVRALL_EMESH_PARAM][OVRALL_EMESH_PARAM]��
								// �����Ǹ�˹�˲����е㡣


	// ������
	// TODO: ����Ҫʶ�������г��ֵĺ���д�����

	void CHCRRecognizer::InitWithStrokeList(CTypedPtrList<CObList,CStroke*>& strokeList);
								// ��strokeList�ж����㵽m_points[]��

	CPoint RebuildCoordinate();
								// �����Ͻǵĵ�������(0,0)�����ҷ������½ǵĵ����ꡣ

	void CHCRRecognizer::LinearNormalize(CPoint rightlower);
								// ���Թ�һ����SIDE_LENGTH*SIDE_LENGTH��

	void CHCRRecognizer::DDALine();
								// ��DDA�㷨�����ز�����

	void CHCRRecognizer::FilterOverSampling();
								// ȥ����ͬ����������㡣


	void CHCRRecognizer::DirChFeaExtract();
								// ��ȡ�Ľ��ķ���任������8��ģʽͼ�񣬴�������m_fea[8][PATTERN_SIZE][PATTERN_SIZE]

	void CHCRRecognizer::DirChFeaExtract1();
								// ��ȡ�ձ��ķ���任������8��ģʽͼ�񣬴�������m_fea[8][PATTERN_SIZE][PATTERN_SIZE]

	void CHCRRecognizer::DirFeaExtract();
								// ��ȡ��ǿ��8����������8��ģʽͼ�񣬴�������m_fea[8][PATTERN_SIZE][PATTERN_SIZE]

	void CHCRRecognizer::MaxValueFilter3x3();
								// ��m_fea[][][]��3*3���ֵ�˲�������m_maxfea[][][]��

	void CHCRRecognizer::GaussianFilter();
								// ��˹�˲�������Ҫͨ������DrawOverallElasticMesh()��ȷ���˲��е㡣
								// ��m_maxfea[][][]�е������˲�����ά����浽m_gft[][][]��

	void CHCRRecognizer::Preprocess();
								// ����ʶ��֮�������Ԥ������Ľ�ϡ�������RebuildCoordinate()��ʼ��
								// GaussianFilter()Ϊֹ�����в��衣

	void CHCRRecognizer::Recognize();
								// ʶ���֣�ʹ����Сŷʽ�����������
								// ʶ�����洢��result[10]�У���ʮ������������ÿ���ķ�Χ�ǣ�[0..3754]��



// ���� DEBUG �ĳ�Ա����
public:
	void CHCRRecognizer::WritePointsToFileImage(CPoint* ptPoints, CString filename);
	void CHCRRecognizer::WriteMatrixToFileImage(float feaPattern[PATTERN_SIZE][PATTERN_SIZE], CString filename);
	void CHCRRecognizer::WriteMatrixToFileImage8x8(float feaPattern[GF_PATTERN_SIZE][GF_PATTERN_SIZE], CString filename);
	


};

#endif // !defined(AFX_HCRRECOGNIZER_H__C9866CFA_057D_4D39_9786_53FA1E28418E__INCLUDED_)
