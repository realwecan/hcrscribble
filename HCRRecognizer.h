// HCRRecognizer.h: interface for the CHCRRecognizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HCRRECOGNIZER_H__C9866CFA_057D_4D39_9786_53FA1E28418E__INCLUDED_)
#define AFX_HCRRECOGNIZER_H__C9866CFA_057D_4D39_9786_53FA1E28418E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "HCRScribbleDoc.h"						// 为了引用AFX Template和CStroke类


#define MAX_POINTS 5000							// 经过DDA算法后最多允许的点数，一般是原采样点数的10倍
#define SIDE_LENGTH 64							// 线性归一化后的边长
#define PATTERN_SIZE 64							// 特征的边长（未降维前，一般和SIDE_LENGTH相同）
#define GF_PATTERN_SIZE 8						// 通过高斯滤波降维后的模式图像的边长
#define OVRALL_EMESH_PARAM 8					// 全局弹性网格的数量（决定了高斯滤波器的滤波中点数量）

#define NUM_TRAIN 45							// 训练字库的数量
#define NUM_TEST 15								// 测试字库的数量
#define NUM_KANJI 3755							// 数据库中的汉字数量



class CHCRRecognizer  
{

// 构造和析构函数
public:
	CHCRRecognizer();							// 无参构造函数是必须的
	virtual ~CHCRRecognizer();					// 析构函数


// 成员变量

public:
	int curfea;																	// 当前使用的特征。默认为0，即改进的方向变换特征。
																				// 1表示方向变换特征；2表示8方向特征。
	int result[10];																// 用于存储识别结果，汉字的编号。

protected:
	
	CPoint m_points[MAX_POINTS];												// 用于存储字迹的数组
	int ptnum;																	// 总是指出m_points[]字迹中的点数
	//int GetPtNum() { return ptnum; }											// 得到m_points[]中的点数
	float m_fea[8][PATTERN_SIZE][PATTERN_SIZE];									// 特征
	float m_maxfea[8][PATTERN_SIZE][PATTERN_SIZE];								// 最大值滤波后的特征
	float m_gftfea[8][GF_PATTERN_SIZE][GF_PATTERN_SIZE];						// 高斯滤波后的特征
	float m_pfea[NUM_KANJI][8][GF_PATTERN_SIZE][GF_PATTERN_SIZE];				// 特征模板

	float m_fGTable[625];														// 二维高斯函数值表（25*25，波长和宽度为12）
	CPoint m_fPoints[OVRALL_EMESH_PARAM][OVRALL_EMESH_PARAM];					// 高斯滤波的中心点坐标（弹性网格中点）


// 成员函数
public:

	// 实用函数
	// TODO: 把被主要识别流程中的函数引用的函数放在这里。

	double CHCRRecognizer::TwoPtDist(CPoint pt1, CPoint pt2);
								// 计算两点的欧式距离

	BOOL CHCRRecognizer::IsClockwise(CPoint pt1, CPoint pt2, CPoint pt3);
								// 判断三个点pt1,pt2,pt3是否是顺时针排列，还是逆时针排列。

	int CHCRRecognizer::IsOnSingleLine(CPoint pt1, CPoint pt2, CPoint pt3);
								// 判断pt1,pt2,pt3三点是否成一直线。如果不是，返回-1。如果是的话，返回0或1。
								// 如果pt0---->pt1---->pt2，返回0。
								// 如果pt0-----pt2<--->pt1，返回1。

	int CHCRRecognizer::CalcDegCh(CPoint ptPrev, CPoint ptCur, CPoint ptNext);
								// 计算 Line(ptPrev,ptCur) 和 Line(ptCur,ptNext)之前的夹角。返回值可能是负值。
								// 逆时针的夹角为正，顺时针为负。夹角取值范围是(-180 deg, 180 deg]。 

	int CHCRRecognizer::CalcDirection(CPoint ptCur, CPoint ptNext);
								// 计算 Vector(ptCur, ptNext) 的方向。返回值是[0..7]中的一个整数，基于
								// 最靠近的8方向编号。

	int CHCRRecognizer::CalcDirectionWithOffset(CPoint ptCur, CPoint ptNext);
								// 计算 Vector(ptCur, ptNext) 的方向，并且带有 22.5deg 的 offset。
								// 返回值是[0..7]中的一个整数，返回的是所在8方向区间的编号。

	void CHCRRecognizer::GenGTable();
								// 生成二维高斯函数值表。

	int CHCRRecognizer::ReadOne863OnlineCharcter(FILE *fpc,int i_wordindex);
								// 从863Online数据库中读取一个汉字，汉字的编号在[0..3754]之间。

	void CHCRRecognizer::ReadAll863OnlineCharacter(CString filename, int dataset_counter);
								// 读取整个863Online数据库。dataset_counter只是用于写状态栏的信息，没有其他作用。
								// 结果将被写入m_pfea[][][][]数组中。

	void CHCRRecognizer::TrainClassifier();
								// 训练分类器，数据集的数量由NUM_TRAIN指出。

	void CHCRRecognizer::LoadClassifier(CString filename);
								// 从文件中恢复分类器的特征模板。

	void CHCRRecognizer::TestAll863OnlineCharacter(CString filename,int dataset_counter);
								// 测试一个863Online数据库中的所有汉字。
								// 返回一个整数，指出有多少汉字被错误地识别。
								// 识别率是：(return value / 3755 * 100%).

	void CHCRRecognizer::TestRecognizer();
								// 测试分类器，数据集的数量有NUM_TEST指出。

	void CHCRRecognizer::DrawOverallElasticMesh();
								// 画全局弹性网格，将弹性网格的中点写入到 m_fPoints[OVRALL_EMESH_PARAM][OVRALL_EMESH_PARAM]。
								// 后者是高斯滤波的中点。


	// 主函数
	// TODO: 将主要识别流程中出现的函数写在这里。

	void CHCRRecognizer::InitWithStrokeList(CTypedPtrList<CObList,CStroke*>& strokeList);
								// 从strokeList中读出点到m_points[]。

	CPoint RebuildCoordinate();
								// 把左上角的点坐标变成(0,0)，并且返回右下角的点坐标。

	void CHCRRecognizer::LinearNormalize(CPoint rightlower);
								// 线性归一化到SIDE_LENGTH*SIDE_LENGTH。

	void CHCRRecognizer::DDALine();
								// 用DDA算法进行重采样。

	void CHCRRecognizer::FilterOverSampling();
								// 去掉相同坐标的连续点。


	void CHCRRecognizer::DirChFeaExtract();
								// 提取改进的方向变换特征到8张模式图像，存入数组m_fea[8][PATTERN_SIZE][PATTERN_SIZE]

	void CHCRRecognizer::DirChFeaExtract1();
								// 提取日本的方向变换特征到8张模式图像，存入数组m_fea[8][PATTERN_SIZE][PATTERN_SIZE]

	void CHCRRecognizer::DirFeaExtract();
								// 提取霍强的8方向特征到8张模式图像，存入数组m_fea[8][PATTERN_SIZE][PATTERN_SIZE]

	void CHCRRecognizer::MaxValueFilter3x3();
								// 对m_fea[][][]做3*3最大值滤波，存入m_maxfea[][][]。

	void CHCRRecognizer::GaussianFilter();
								// 高斯滤波，首先要通过调用DrawOverallElasticMesh()来确定滤波中点。
								// 把m_maxfea[][][]中的数据滤波（降维）后存到m_gft[][][]。

	void CHCRRecognizer::Preprocess();
								// 除了识别之外的所有预处理步骤的结合。包括从RebuildCoordinate()开始到
								// GaussianFilter()为止的所有步骤。

	void CHCRRecognizer::Recognize();
								// 识别汉字，使用最小欧式距离分类器。
								// 识别结果存储在result[10]中，是十个整数，其中每个的范围是：[0..3754]。



// 用于 DEBUG 的成员函数
public:
	void CHCRRecognizer::WritePointsToFileImage(CPoint* ptPoints, CString filename);
	void CHCRRecognizer::WriteMatrixToFileImage(float feaPattern[PATTERN_SIZE][PATTERN_SIZE], CString filename);
	void CHCRRecognizer::WriteMatrixToFileImage8x8(float feaPattern[GF_PATTERN_SIZE][GF_PATTERN_SIZE], CString filename);
	


};

#endif // !defined(AFX_HCRRECOGNIZER_H__C9866CFA_057D_4D39_9786_53FA1E28418E__INCLUDED_)
