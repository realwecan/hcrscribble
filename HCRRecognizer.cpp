// HCRRecognizer.cpp: implementation of the CHCRRecognizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HCRScribble.h"
#include "HCRRecognizer.h"

#include "math.h" //sqrt function
#include "stdlib.h" //quick-sort function qsort()
#define PI 3.14159265359f

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHCRRecognizer::CHCRRecognizer()
{
	GenGTable();
}

CHCRRecognizer::~CHCRRecognizer()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CHCRRecognizer::InitWithStrokeList(CTypedPtrList<CObList,CStroke*>& strokeList)
{
	
	POSITION pos = strokeList.GetHeadPosition();
	int ptCnt = 0;
	while (pos != NULL)
	{
		CStroke* pStroke = strokeList.GetNext(pos);
		
		for (int i=0; i < ( pStroke->m_pointArray.GetSize() ); i++ )
		{
			m_points[ptCnt++] = pStroke->m_pointArray.GetAt(i);
			if (ptCnt >= MAX_POINTS) { MessageBox(0,"Too many points", 0, MB_OK); ptnum = ptCnt; return; }
		}
	}
	
	ptnum = ptCnt;
	
}

CPoint CHCRRecognizer::RebuildCoordinate()
{
	CPoint leftupper = m_points[0];
	CPoint rightlower = m_points[0];
	for (int i=1; i < ptnum; i++)
	{
		if ( m_points[i].x < leftupper.x )
			leftupper.x = m_points[i].x;
		if ( m_points[i].y < leftupper.y )
			leftupper.y = m_points[i].y;
		if ( m_points[i].x > rightlower.x )
			rightlower.x = m_points[i].x;
		if ( m_points[i].y > rightlower.y )
			rightlower.y = m_points[i].y;
	}
	// find the leftupper and rightlower points

	for (i=0; i < ptnum; i++)
	{
		m_points[i] -= leftupper; // move the points, establish new coordinates
	}
	
	return (rightlower - leftupper);
}

void CHCRRecognizer::WritePointsToFileImage(CPoint* ptPoints, CString filename)
{
	char fileImage[SIDE_LENGTH][SIDE_LENGTH];
	for (int i=0; i < SIDE_LENGTH; i++)
		for (int j=0; j < SIDE_LENGTH; j++)
			fileImage[i][j] = '-';

	for (i=0; i < ptnum; i++)
	{
		fileImage[ptPoints[i].y][ptPoints[i].x] = '*';
		// NOTE: a point with coordinate [x,y] should be placed into the fileImage
		//       with index [y,x]. It's because the fileImage array stores data
		//       line-by-line. Use specific examples to examine this.
	}

	//CFile file(_T("c:\\myfile.txt"),CFile::modeCreate);
	FILE *fp;
	fp = fopen(filename,"wb");

	for (i=0; i < SIDE_LENGTH; i++)
	{
		fwrite(fileImage[i],sizeof(char),SIDE_LENGTH,fp);
		fputc('\n',fp);
	}
	fclose(fp);
}

void CHCRRecognizer::WriteMatrixToFileImage(float feaPattern[PATTERN_SIZE][PATTERN_SIZE], CString filename)
{
	FILE *fp;
	fp = fopen(filename,"wb");

	for (int i=0; i < PATTERN_SIZE; i++)
	{
		for (int j=0; j < PATTERN_SIZE; j++)
		{
			//fputc(feaPattern[j][i]+'0', fp);
			fprintf(fp, "%.1f", feaPattern[j][i]);
			// NOTE: a point with coordinate [x,y] should be placed into the fileImage
			//       with index [y,x].
			fputc(' ',fp);
		}
		fputc('\n',fp);
	}
	fclose(fp);
}

void CHCRRecognizer::WriteMatrixToFileImage8x8(float feaPattern[GF_PATTERN_SIZE][GF_PATTERN_SIZE], CString filename)
{
	FILE *fp;
	fp = fopen(filename,"wb");
	
	for (int i=0; i < GF_PATTERN_SIZE; i++)
	{
		for (int j=0; j < GF_PATTERN_SIZE; j++)
		{
			//fputc(feaPattern[j][i]+'0', fp);
			fprintf(fp, "%.1f", feaPattern[j][i]);
			// NOTE: a point with coordinate [x,y] should be placed into the fileImage
			//       with index [y,x].
			fputc(' ',fp);
		}
		fputc('\n',fp);
	}
	fclose(fp);
}

void CHCRRecognizer::LinearNormalize(CPoint rightlower) //要横坐标和纵坐标分开归一化
{
	//int charSize = max(rightlower.x,rightlower.y);
	float xzoomScale = (float)rightlower.x / (float)(SIDE_LENGTH-1); // eg. charSize=30, SIDE_LENGTH=16(-1=15);
											   //     then zoomScale = 2
	float yzoomScale = (float)rightlower.y / (float)(SIDE_LENGTH-1);

	for (int i=0; i < ptnum; i++)
	{
		//m_points[i].x = int ( 0.5 + float(m_points[i].x) / zoomScale + float(SIDE_LENGTH-1) / 2 - float(rightlower.x) / 2 );
		m_points[i].x = int ( 0.5f + ( float(m_points[i].x) - float(rightlower.x) / 2.0f ) / xzoomScale
							+ float(SIDE_LENGTH-1) / 2.0f );
		m_points[i].y = int ( 0.5f + ( float(m_points[i].y) - float(rightlower.y) / 2.0f ) / yzoomScale
							+ float(SIDE_LENGTH-1) / 2.0f );
		//m_points[i].y = int ( 0.5 + float(m_points[i].y) / zoomScale + float(SIDE_LENGTH-1) / 2 - float(rightlower.y) / 2 );
	}   // to put the new normalized character in the center of a pattern
		// +0.5 at first is a way to implement the pascal round() func

	//WritePointsToFileImage(m_points,"c:\\linenorm.txt");

}

void CHCRRecognizer::FilterOverSampling()
{
	CPoint lastPt;
	lastPt = m_points[0];
	BOOL ptToDel[MAX_POINTS];
	ptToDel[0] = FALSE;
	int newPtnum = ptnum;
	for (int i=1; i < ptnum; i++)
	{
		if ( m_points[i] == lastPt )
		{
			ptToDel[i] = TRUE;
			newPtnum--;
		}
		else
		{
			ptToDel[i] = FALSE;
			lastPt = m_points[i];
		}
	}
	int j=0;
	for (i=0; i < ptnum; i++)
	{
		if ( !ptToDel[i] )
		{
			m_points[j] = m_points[i];
			j++;
		}
	}
	ptnum = newPtnum;

	//WritePointsToFileImage(m_points,"c:\\filterover.txt");
}

double CHCRRecognizer::TwoPtDist(CPoint pt1, CPoint pt2)
{
	return sqrt((double)( (pt2.x - pt1.x)*(pt2.x - pt1.x)+(pt2.y - pt1.y)*(pt2.y - pt1.y) ));
}

BOOL CHCRRecognizer::IsClockwise(CPoint pt3, CPoint pt2, CPoint pt1)
{
	// NOTE: The three points are loaded in inversed order into this func.
	//       This is because the coordinate system on the computer screen
	//       is a mirror to the traditional mathematic coordinate system.

	/*E1x = P1x-P2x 
	E1y = P1y-P2y 
	E2x = P3x-P2x 
	E2y = P3y-P2y
	if ((E1x * E2y - E1y * E2x) >= 0) clockwise = TRUE; 
	else clockwise = FALSE;*/

	int E1x = pt1.x - pt2.x;
	int E1y = pt1.y - pt2.y;
	int E2x = pt3.x - pt2.x;
	int E2y = pt3.y - pt2.y;

	if ((E1x * E2y - E1y * E2x) >= 0) return TRUE;
	else return FALSE;

}

int CHCRRecognizer::IsOnSingleLine(CPoint pt1, CPoint pt2, CPoint pt3)
{
	int x1 = pt2.x - pt1.x;
	int y1 = pt2.y - pt1.y;
	int x2 = pt3.x - pt2.x;
	int y2 = pt3.y - pt2.y;

	if ( x1*y2 == x2*y1 ) // three points are on a single line
	{
		if ( x1 == 0 ) // because two successive points cannot be of same coordinate, deduce y1!=0.
					   // then under that x1==0, the only possibility is x2==0.
		{
			if ( y1*y2 > 0 ) // y1,y2 are both positive or both negative. Then pt2 is in the middle of
							 // pt1 and pt3.
				return 0;
			else             // y1,y2 are not both positive or both negative.
				return 1;    // Then pt2 is a point where the stroke have a 180 deg rewind.
		}
		else // x1!=0
		{
			if ( x1*x2 > 0 ) // x1,x2 are both positive or both negative.
				return 0;
			else             // x1,x2 are not both positive or both negative.
				return 1;    // Then pt2 is a point where the stroke have a 180 deg rewind.
		}
	}
	else return -1; // three points are NOT on a single line
}

int CHCRRecognizer::CalcDegCh(CPoint ptPrev, CPoint ptCur, CPoint ptNext)
{

	// STEP1. IF TWO SUCCESSIVE POINTS ARE OF SAME COORDINATE
	//-------------------------------------------------------------------------------------------------
	//if ( ( ptPrev == ptCur ) || ( ptCur == ptNext ) ) //if two successive points are of same coordinate
	//{
	//	return 0;
	//}
	
	// STEP2. IF THREE POINTS ARE ON A SINGLE LINE
	//-------------------------------------------------------------------------------------------------
	int singleLine = IsOnSingleLine(ptPrev, ptCur, ptNext);
	if ( singleLine >= 0 ) // three points are on a single line
	{
		if ( singleLine == 0 ) return 0;
		else return 180;
	}

	// STEP3. IF THREE POINTS ARE CLOCKWISE OR COUNTER-CLOCKWISE
	//-------------------------------------------------------------------------------------------------
	double ab,bc,ac;
	ab = TwoPtDist(ptPrev,ptCur);
	bc = TwoPtDist(ptCur,ptNext);
	ac = TwoPtDist(ptPrev,ptNext);
	
	if ( IsClockwise(ptPrev, ptCur, ptNext) )
	{
		return int( -180 + ( ( acos( (ab*ab+bc*bc-ac*ac) / (2*ab*bc) ) ) * 180.0f / PI ) );
		// acos() returns value in [0,PI] radian, conversion to degree is required.
	}
	else
	{
		return int( 180 - ( ( acos( (ab*ab+bc*bc-ac*ac) / (2*ab*bc) ) ) * 180.0f / PI ) );
		// acos() returns value in [0,PI] radian, conversion to degree is required.
	}
	
}

int CHCRRecognizer::CalcDirectionWithOffset(CPoint ptCur, CPoint ptNext)
{
	// This func returns an integer in [0..7] on a 8-directional basis
	// An offset of 22.5 deg is presented here to implement HUO Qiang's method.
	
	CPoint ptPrev; // imagine a new point to simplify the calculation 
	ptPrev.x = 0;
	ptPrev.y = ptCur.y;
	float newdeg = float(CalcDegCh(ptPrev, ptCur, ptNext));
	if ( newdeg < 0 ) newdeg += 360.0f;
	if ( ( newdeg >= 0 ) && ( newdeg < 45 ) ) return 0;
	else if ( ( newdeg >= 45 ) && ( newdeg < 90 ) ) return 1;
	else if ( ( newdeg >= 90 ) && ( newdeg < 135 ) ) return 2;
	else if ( ( newdeg >= 135 ) && ( newdeg < 180 ) ) return 3;
	else if ( ( newdeg >= 180 ) && ( newdeg < 225 ) ) return 4;
	else if ( ( newdeg >= 225 ) && ( newdeg < 270 ) ) return 5;
	else if ( ( newdeg >= 270 ) && ( newdeg < 315 ) ) return 6;
	else return 7;
}

int CHCRRecognizer::CalcDirection(CPoint ptCur, CPoint ptNext)
{
	// This func returns an integer in [0..7] on a 8-directional basis

	CPoint ptPrev; // imagine a new point to simplify the calculation 
	ptPrev.x = 0;
	ptPrev.y = ptCur.y;
	float newdeg = float(CalcDegCh(ptPrev, ptCur, ptNext));
	if ( newdeg < 0 ) newdeg += 360.0f;
	if ( ( newdeg >= 23 ) && ( newdeg < 68 ) ) return 1;
	else if ( ( newdeg >= 68 ) && ( newdeg < 113 ) ) return 2;
	else if ( ( newdeg >= 113 ) && ( newdeg < 158 ) ) return 3;
	else if ( ( newdeg >= 158 ) && ( newdeg < 203 ) ) return 4;
	else if ( ( newdeg >= 203 ) && ( newdeg < 248 ) ) return 5;
	else if ( ( newdeg >= 248 ) && ( newdeg < 293 ) ) return 6;
	else if ( ( newdeg >= 293 ) && ( newdeg < 338 ) ) return 7;
	else return 0;
}

void CHCRRecognizer::DirChFeaExtract()
{

	// Clear Direction-change feature matrixes
	for (int i=0; i<8; i++)
	{
		for (int j=0;j<PATTERN_SIZE;j++)
		{
			for (int k=0;k<PATTERN_SIZE;k++)
			{
				m_fea[i][j][k] = 0;
			}
		}
	}
	
	// Too few points cannot extract direction-change feature
	if (ptnum < 3)
	{
		//MessageBox(0,"Too few points, Direction-change feature will be set to all one (default value).",0,MB_OK);
	}
	else // Start to extract direction-change feature
	{
		for (i=1; i < ptnum-1; i++)
		{
			int dirch = CalcDegCh(m_points[i-1],m_points[i],m_points[i+1]);
			float Fdc = 1.0f;
			Fdc = float(abs(dirch)) / 60.0f + 1.0f;
			int curDirection = CalcDirectionWithOffset(m_points[i],m_points[i+1]);
			int dx = abs( m_points[i+1].x - m_points[i-1].x );
			int dy = abs( m_points[i+1].y - m_points[i-1].y );
			float s = Fdc;
			if (s!=0) //if s==0 then ptPrev==ptNext, the m_dirchfea at this point should be all zero
			{
				if ( curDirection % 2 == 0 ) // even
				{
					m_fea[curDirection][m_points[i].x][m_points[i].y] = (abs( dx - dy )) / s;
					m_fea[curDirection+1][m_points[i].x][m_points[i].y] = (min(dx,dy)*1.41421356f) / s;
				}
				else //odd
				{
					m_fea[(curDirection+1)%8][m_points[i].x][m_points[i].y] = (abs( dx - dy )) / s;
					m_fea[curDirection][m_points[i].x][m_points[i].y] = (min(dx,dy)*1.41421356f) / s;
				}
			}
		}
	}

	//WriteMatrixToFileImage(m_dirchfea[0],"c:\\dirchfea.txt");
}


void CHCRRecognizer::GenGTable()//波长取12 宽度取12
{
	short x,y;
	for(y = -12 ;y <=12; ++y)
	{
		for (x =-12; x <=12; ++x)
		{
			m_fGTable[(y+12)*25+x+12] = (float)(8192*0.081633f*exp( -0.040816f*(float)((x+0.5)*(x+0.5)+(y+0.5)*(y+0.5)) ));
		}
	}
}

void CHCRRecognizer::DrawOverallElasticMesh()
{
	// STEP1. DRAW POINTS COORDINATES ONTO OFFLINE BITMAP
	//-------------------------------------------------------------------------------------------------
	bool char_image[PATTERN_SIZE+1][PATTERN_SIZE+1];
	int i,j;

	for(i=0; i< PATTERN_SIZE+1; i++)
		for(j=0; j< PATTERN_SIZE+1; j++)
			char_image[i][j] = false;

	for(i=0; i< ptnum;i++)
		char_image[m_points[i].x][m_points[i].y] = true;

	// STEP2. MAXIMUM VALUE FILTERING
	//-------------------------------------------------------------------------------------------------
	bool max_char_image[PATTERN_SIZE+1][PATTERN_SIZE+1];

	for(i=0; i< PATTERN_SIZE+1; i++)
		for(j=0; j< PATTERN_SIZE+1; j++)
			max_char_image[i][j] = false;

	for(i=0; i< PATTERN_SIZE; i++)
		for(j=0; j< PATTERN_SIZE; j++)
		{
			if (char_image[i][j]) max_char_image[i][j] = true;
			if (char_image[i+1][j]) max_char_image[i][j] = true;
			if (char_image[i][j+1]) max_char_image[i][j] = true;
			if (char_image[i+1][j+1]) max_char_image[i][j] = true;
			if ( (i>0) && (j>0) )
				if (char_image[i-1][j-1]) max_char_image[i][j] = true;
			if ( i>0 )
			{
				if (char_image[i-1][j]) max_char_image[i][j] = true;
				if (char_image[i-1][j+1]) max_char_image[i][j] = true;
			}
			if ( j>0 )
			{
				if (char_image[i][j-1]) max_char_image[i][j] = true;
				if (char_image[i+1][j-1]) max_char_image[i][j] = true;
			}
		}

	int max_ptnum = 0;
	for(i=0; i< PATTERN_SIZE; i++)
		for(j=0; j< PATTERN_SIZE; j++)
		{
			if (max_char_image[i][j]) max_ptnum++;
		}


	// STEP3. FIND EDGES OF ELASTIC MESHES
	//-------------------------------------------------------------------------------------------------
	CPoint edge[8]; //edge of elastic meshes

	int accu_value = 0; //accumulated value of points scanned
	int mesh_id=0; //mesh_id=0..7

	for(i=0; i< PATTERN_SIZE; i++)
		for(j=0; j< PATTERN_SIZE; j++)
		{
			if (max_char_image[i][j]) accu_value++;
			if ( float(accu_value) >= float(max_ptnum)/8.0f*float(mesh_id+1) )
			{
				edge[mesh_id].y = i;
				mesh_id++;
			}
		}
	
	accu_value = 0;
	mesh_id = 0;
	for(j=0; j< PATTERN_SIZE; j++)
		for(i=0; i< PATTERN_SIZE; i++)
		{
			if (max_char_image[i][j]) accu_value++;
			if ( float(accu_value) >= float(max_ptnum)/8.0f*float(mesh_id+1) )
			{
				edge[mesh_id].x = j;
				mesh_id++;
			}
		}
	//edge[7].x = 63; edge[7].y = 63; // the last edge must lies at the bottom of pattern image

	// STEP4. WRITE RESULTS TO m_fPoints[8][8]
	//-------------------------------------------------------------------------------------------------
	CPoint tempPt;
	for (i=0; i<8; i++ )
	{
		if (i==0) tempPt.y = int( float(edge[0].y) / 2.0f + 0.5f );
		else tempPt.y = int(float(edge[i].y + edge[i-1].y) / 2.0f + 0.5);

		for(j=0; j<8; j++ )
		{
			if (j==0) tempPt.x = int( float(edge[0].x) / 2.0f + 0.5f );
			else tempPt.x = int(float(edge[j].x + edge[j-1].x) / 2.0f + 0.5);

			m_fPoints[i][j] = tempPt; //1st dimension y; 2nd dimension x
		}
	}

}

void CHCRRecognizer::GaussianFilter()
{

	DrawOverallElasticMesh();

	for (int i=0; i<8; i++)
	{
		for (int j=0;j<GF_PATTERN_SIZE;j++)
		{
			for (int k=0;k<GF_PATTERN_SIZE;k++)
			{
				double thisValue;
				CPoint centerPt;
				centerPt = m_fPoints[k][j];
				thisValue = 0;
				for (int y=-12; y<=12; ++y)
					for (int x=-12; x<=12; ++x)
					{
						if ( ( (y+centerPt.y)>=0 ) && ( (x+centerPt.x)>=0 ) )
							if ( ( (y+centerPt.y)<PATTERN_SIZE ) && ( (x+centerPt.x)<PATTERN_SIZE ) )
							{
								thisValue += m_maxfea[i][y+centerPt.y][x+centerPt.x]*m_fGTable[(y+12)*25+x+12];
							}
					}
				m_gftfea[i][j][k] = float(thisValue);
			}
		}
	}

}


WORD ChangeByteOrder(WORD Char)
{
	int low = Char & 0x00ff;
	int high = Char & 0xff00;
	low = low << 8;
	high = high >> 8;
	return low|high;
}

int CHCRRecognizer::ReadOne863OnlineCharcter(FILE *fpc,int i_wordindex)
{

	i_wordindex += 63; //the first KANJI is no.63, "a- for surprise".

	WORD w_wordsize; //字体所占用的字节数
	WORD w_strokenum; //字体的笔画数
	WORD w_indexsize; //记录所需要的字之前的每个字所占有的字节数 用于搜索所需要的汉字的位置
	
	WORD x,y;
	
	int i_offset=0; //在字的起始位置相对于文件头的偏置
	short i_potnum; //形成该字的采样点的数目
	int i,countindex=1; //用于计数，判读是否到达所需要的字的序号
	
	DWORD d_code;
	
	rewind(fpc);
	
	if(countindex>i_wordindex)
	{
		return -1;
	}
	
	while(countindex<i_wordindex)
	{
		fread(&w_indexsize,1,sizeof(WORD),fpc);
		
		i_offset+=w_indexsize;
		
		fseek(fpc,i_offset,SEEK_SET);
		
		countindex++;
	}
	
	fread(&w_wordsize,1,sizeof(WORD),fpc);
	//fseek(fpc,sizeof(DWORD),SEEK_CUR);
	fread(&d_code,sizeof(DWORD),1,fpc);
	
	d_code=ChangeByteOrder((WORD)d_code);
	fread(&w_strokenum,1,sizeof(WORD),fpc);
	
	i_potnum=(w_wordsize-4*sizeof(WORD)-(w_strokenum+1)*sizeof(DWORD))/4;

	ptnum = 0;
	
	for(i=0;i<i_potnum;i++)
	{
		fread(&x,sizeof(WORD),1,fpc);
		fread(&y,sizeof(WORD),1,fpc);

		if (i==30)
			i=i;
		
		if(x==0xFFFF && y==0x0000)
		{
			--i;
		}
		else
		{
			m_points[ptnum].x=x;
			m_points[ptnum].y=y;
			ptnum++;
		}
		
	}

	
	return i_potnum;
	
}

void CHCRRecognizer::ReadAll863OnlineCharacter(CString filename, int dataset_counter)
{

	FILE *fp;
	
	fp = fopen(filename,"rb");

	for (int i=0; i<3755; i++)
	{

		if (i==10)
			i=10;

		ReadOne863OnlineCharcter(fp,i);
		
		Preprocess();

		for (int j=0; j<8; j++)
			for (int k=0; k<GF_PATTERN_SIZE; k++)
				for (int l=0; l<GF_PATTERN_SIZE; l++)
				{
					m_pfea[i][j][k][l] += ( m_gftfea[j][k][l] / float(NUM_TRAIN) );
				}
				
		CString status_bar_msg;
		status_bar_msg.Format("正在训练第%d个数据集的第%d个汉字。",dataset_counter,i+1);
		CStatusBar* pStatus = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
		pStatus->SetPaneText(0,status_bar_msg);
	}
	
	fclose(fp);

}

void CHCRRecognizer::TestAll863OnlineCharacter(CString filename,int dataset_counter)
{
	FILE *fp;

	fp = fopen(filename,"rb");

	int err_cnt[4]; //four err_cnt[] elements indicates error rate for 1,2,5,10 candidates respectively.
	for (int i=0; i<4; i++)
		err_cnt[i]=0;

	for (i=0; i<3755; i++)
	{

		ReadOne863OnlineCharcter(fp,i);

		Preprocess();
		
		Recognize();

		if (result[0]!=i)
		{
			err_cnt[0]++;
			if (result[1]!=i)
			{
				err_cnt[1]++;
				if ( (result[2]!=i) && (result[3]!=i) && (result[4]!=i) )
				{	
					err_cnt[2]++;
					if ( (result[5]!=i) && (result[6]!=i) && (result[7]!=i) && (result[8]!=i) && (result[9]!=i) ) err_cnt[3]++;
				}
			}
		}

		/*if (result!=i)
		{

			CString tempfile;		
			tempfile.Format("C:\\char%d.txt",i);
			WritePointsToFileImage(m_points,tempfile);

			ReadOne863OnlineCharcter(fp,result);
		
			CPoint rightlower = RebuildCoordinate();
		
			LinearNormalize(rightlower);
		
			tempfile.Format("C:\\char%d.txt",result);
		
			WritePointsToFileImage(m_points,tempfile);

			CString tempmsg;

			tempmsg.Format("error pattern %d, recognized as %d",i,result);

			MessageBox(0,tempmsg,0,MB_OK);
		}
		*/
		float recog_rate[4];
		recog_rate[0] = float( float(i+1-err_cnt[0]) / float(i+1) * 100.0f );
		recog_rate[1] = float( float(i+1-err_cnt[1]) / float(i+1) * 100.0f );
		recog_rate[2] = float( float(i+1-err_cnt[2]) / float(i+1) * 100.0f );
		recog_rate[3] = float( float(i+1-err_cnt[3]) / float(i+1) * 100.0f );
		
		CString status_bar_msg;
		status_bar_msg.Format("正在测试第%d个数据集的第%d个汉字。前一、二、五、十候选字识别率分别为%.2f%%、%.2f%%、%.2f%%、%.2f%%。",
							  dataset_counter,i+1,recog_rate[0],recog_rate[1],recog_rate[2],recog_rate[3]);
		CStatusBar* pStatus = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
		pStatus->SetPaneText(0,status_bar_msg);

	}

	fclose(fp);

}

void CHCRRecognizer::LoadClassifier(CString filename)
{

	FILE *fp;
	int i,j,k,l;
	fp = fopen(filename,"rb");
	for (i=0; i<3755; i++)
	{
		for (j=0; j<8; j++)
			for (k=0; k<GF_PATTERN_SIZE; k++)
				for (l=0; l<GF_PATTERN_SIZE; l++)
				{
					fread(&(m_pfea[i][j][k][l]),sizeof(float),1,fp);
				}
				CString status_bar_msg;
				status_bar_msg.Format("正在加载第%d个汉字的特征模板。",i+1);
				CStatusBar* pStatus = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
				pStatus->SetPaneText(0,status_bar_msg);
	}
	fclose(fp);

}

void CHCRRecognizer::TrainClassifier()
{
	int i,j,k,l;

	for (i=0; i<3755; i++)
		for (j=0; j<8; j++)
			for (k=0; k<GF_PATTERN_SIZE; k++)
				for (l=0; l<GF_PATTERN_SIZE; l++)
				{
					m_pfea[i][j][k][l] = 0;
				}
	MessageBox(0,"Success upon initialization.", 0, MB_OK);

	ReadAll863OnlineCharacter("C:\\Check001.Pot",1);
	//MessageBox(0,"1 finished", 0, MB_OK);
	ReadAll863OnlineCharacter("C:\\Check002.Pot",2);
	ReadAll863OnlineCharacter("C:\\Check004.Pot",3);
	ReadAll863OnlineCharacter("C:\\Check005.Pot",4);
	ReadAll863OnlineCharacter("C:\\Check006.Pot",5);
	//MessageBox(0,"5 finished", 0, MB_OK);
	ReadAll863OnlineCharacter("C:\\Check007.Pot",6);
	ReadAll863OnlineCharacter("C:\\Check009.Pot",7);
	ReadAll863OnlineCharacter("C:\\Check010.Pot",8);
	ReadAll863OnlineCharacter("C:\\Check011.Pot",9);
	ReadAll863OnlineCharacter("C:\\Check012.Pot",10);
	//MessageBox(0,"10 finished", 0, MB_OK);
	ReadAll863OnlineCharacter("C:\\Check014.Pot",11);
	ReadAll863OnlineCharacter("C:\\Check015.Pot",12);
	ReadAll863OnlineCharacter("C:\\Check016.Pot",13);
	ReadAll863OnlineCharacter("C:\\Check017.Pot",14);
	ReadAll863OnlineCharacter("C:\\Check019.Pot",15);
	ReadAll863OnlineCharacter("C:\\Check020.Pot",16);
	ReadAll863OnlineCharacter("C:\\Check021.Pot",17);
	ReadAll863OnlineCharacter("C:\\Check022.Pot",18);
	ReadAll863OnlineCharacter("C:\\Check024.Pot",19);
	ReadAll863OnlineCharacter("C:\\Check025.Pot",20);
	//MessageBox(0,"20 finished", 0, MB_OK);
	ReadAll863OnlineCharacter("C:\\Check026.Pot",21);
	ReadAll863OnlineCharacter("C:\\Check027.Pot",22);
	ReadAll863OnlineCharacter("C:\\Check029.Pot",23);
	ReadAll863OnlineCharacter("C:\\Check030.Pot",24);
	ReadAll863OnlineCharacter("C:\\Check031.Pot",25);
	ReadAll863OnlineCharacter("C:\\Check032.Pot",26);
	ReadAll863OnlineCharacter("C:\\Check034.Pot",27);
	ReadAll863OnlineCharacter("C:\\Check035.Pot",28);
	ReadAll863OnlineCharacter("C:\\Check036.Pot",29);
	ReadAll863OnlineCharacter("C:\\Check037.Pot",30);
	//MessageBox(0,"30 finished", 0, MB_OK);
	ReadAll863OnlineCharacter("C:\\Check039.Pot",31);
	ReadAll863OnlineCharacter("C:\\Check040.Pot",32);
	ReadAll863OnlineCharacter("C:\\Check041.Pot",33);
	ReadAll863OnlineCharacter("C:\\Check042.Pot",34);
	ReadAll863OnlineCharacter("C:\\Check044.Pot",35);
	ReadAll863OnlineCharacter("C:\\Check045.Pot",36);
	ReadAll863OnlineCharacter("C:\\Check046.Pot",37);
	ReadAll863OnlineCharacter("C:\\Check050.Pot",38);
	ReadAll863OnlineCharacter("C:\\Check051.Pot",39);
	ReadAll863OnlineCharacter("C:\\Check052.Pot",40);
	ReadAll863OnlineCharacter("C:\\Check054.Pot",41);
	ReadAll863OnlineCharacter("C:\\Check055.Pot",42);
	ReadAll863OnlineCharacter("C:\\Check058.Pot",43);
	ReadAll863OnlineCharacter("C:\\Check059.Pot",44);
	ReadAll863OnlineCharacter("C:\\Check060.Pot",45);
	//MessageBox(0,"all finished, start to write file.", 0, MB_OK);

	FILE *fp;
	fp = fopen("c:\\classifier4.fea","wb");
	for (i=0; i<3755; i++)
	{
		for (j=0; j<8; j++)
			for (k=0; k<GF_PATTERN_SIZE; k++)
				for (l=0; l<GF_PATTERN_SIZE; l++)
				{
					fwrite(&(m_pfea[i][j][k][l]),sizeof(float),1,fp);
				}
		CString status_bar_msg;
		int tmpAllKanji = NUM_KANJI;
		status_bar_msg.Format("正在写第%d个汉字的模板。一共%d个。",i+1,tmpAllKanji);
		CStatusBar* pStatus = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
		pStatus->SetPaneText(0,status_bar_msg);
	}
	fclose(fp);

}

struct recogRes
{
	int index;
	float dist;
}tmpRes[10];

int compare (const void *a, const void *b)
{
	float val= (*(recogRes*)a).dist - (*(recogRes*)b).dist;
	if (val>0) return 1;
	if (val==0) return 0;
	return -1;
	
}

void CHCRRecognizer::Recognize()
{
	int i,j,k,l,m,n;
	float dist;

	for(i=0; i<10; i++)
	{
		tmpRes[i].index = i;
		tmpRes[i].dist = 0;
		for (j=0; j<8; j++)
			for(k=0; k<GF_PATTERN_SIZE; k++)
				for(l=0; l<GF_PATTERN_SIZE; l++)
				{
					tmpRes[i].dist += ((m_pfea[i][j][k][l]-m_gftfea[j][k][l])*((m_pfea[i][j][k][l]-m_gftfea[j][k][l])));
				}
	}

	qsort(tmpRes, 10, sizeof(tmpRes[0]), compare);

	for(i=10; i<NUM_KANJI; i++)
	{
		dist = 0;
		for (j=0; j<8; j++)
			for (k=0; k<GF_PATTERN_SIZE; k++)
				for (l=0; l<GF_PATTERN_SIZE; l++)
				{
					dist += ((m_pfea[i][j][k][l]-m_gftfea[j][k][l])*((m_pfea[i][j][k][l]-m_gftfea[j][k][l])));
				}
		if ( dist < tmpRes[9].dist )
		{
			m = 9;
			while ( (m>=0) && (dist < tmpRes[m].dist) ) // m>=0 must come first
			{
				m--;
			}
			m++;
			for (n=9; n>m; n--)
			{
				tmpRes[n] = tmpRes[n-1];
			}
			tmpRes[m].index = i;
			tmpRes[m].dist = dist;
		}
	}

	for(i=0; i<10; i++)
	{
		result[i] = tmpRes[i].index;
	}

	/*
	int i,j,k,l,min_kanji;
	float min_dist,dist;
	min_dist=0;
	float result_dist[10];
	for(j=0; j<8; j++)
		for(k=0;k<GF_PATTERN_SIZE;k++)
			for(l=0;l<GF_PATTERN_SIZE;l++)
			{
				min_dist += ((m_pfea[0][j][k][l]-m_gftfea[j][k][l])*((m_pfea[0][j][k][l]-m_gftfea[j][k][l])));
			}
	min_kanji=0;
	for (i=1; i<3755; i++)
	{
		dist = 0;
		for(j=0; j<8; j++)
			for(k=0; k<GF_PATTERN_SIZE; k++)
				for(l=0; l<GF_PATTERN_SIZE; l++)
				{
					dist += ((m_pfea[i][j][k][l]-m_gftfea[j][k][l])*((m_pfea[i][j][k][l]-m_gftfea[j][k][l])));
				}
		if (dist<min_dist)
		{
			min_dist = dist;
			min_kanji = i;
		}
	}
	*/

}

void CHCRRecognizer::TestRecognizer()
{
	TCHAR path[1024];
	GetModuleFileName(NULL, path, 1024);
	*_tcsrchr(path,'\\')=0;
	CString filename;
	filename.Format("%s\\Check057.pot",path);
	//MessageBox(0,filename,0,MB_OK);
	TestAll863OnlineCharacter("C:\\Check057.pot",1);
}

void CHCRRecognizer::DDALine()
{
	float length;
	float dx,dy;
	float x,y;
	int newptnum = 0;
	CPoint new_points[MAX_POINTS];

	// DDA for every pair of two points
	for (int i=0; i<ptnum-1; i++)
	{
		
		if ( (abs(m_points[i].x - m_points[i+1].x)) >= (abs(m_points[i].y - m_points[i+1].y)) )
			length = float(abs(m_points[i].x - m_points[i+1].x));
		else
			length = float(abs(m_points[i].y - m_points[i+1].y));
		
		dx = float(m_points[i+1].x - m_points[i].x) / length;
		dy = float(m_points[i+1].y - m_points[i].y) / length;
		
		x=float(m_points[i].x + 0.5f); y=float(m_points[i].y + 0.5f);
		
		int j=1;
		while (j<=length)
		{
			new_points[newptnum].x = int(x);
			new_points[newptnum].y = int(y);
			newptnum++;
			x += dx; y += dy;
			j++;
		}
	}

	// Update m_points[]
	for (i=0; i<newptnum; i++)
	{
		m_points[i] = new_points[i];
	}

	ptnum = newptnum;

	if (ptnum==0) ptnum=1; // Prevent if all points of a character is on
						   // a same coordinate. 

	//WritePointsToFileImage(m_points,"c:\\mychar.txt");
	//WritePointsToFileImage(m_points,"c:\\ddaline.txt");

}

void CHCRRecognizer::MaxValueFilter3x3()
{
	for (int i=0; i<8; i++)
		for (int j=0; j<PATTERN_SIZE; j++)
			for (int k=0; k<PATTERN_SIZE; k++)
			{
				float maxvalue = m_fea[i][j][k];
				if (j>0)
				{
					if (k>0) maxvalue = max(maxvalue,m_fea[i][j-1][k-1]);
					maxvalue = max(maxvalue,m_fea[i][j-1][k]);
					if (k<PATTERN_SIZE-1) maxvalue = max(maxvalue,m_fea[i][j-1][k+1]);
				}
				if (j<PATTERN_SIZE-1)
				{
					if (k>0) maxvalue = max(maxvalue,m_fea[i][j+1][k-1]);
					maxvalue = max(maxvalue,m_fea[i][j+1][k]);
					if (k<PATTERN_SIZE) maxvalue = max(maxvalue,m_fea[i][j+1][k+1]);
				}
				if (k>0) maxvalue = max(maxvalue,m_fea[i][j][k-1]);
				if (k<PATTERN_SIZE) maxvalue = max(maxvalue,m_fea[i][j][k+1]);
				m_maxfea[i][j][k] = maxvalue;
			}
	//WriteMatrixToFileImage(m_mdirfea[0],"c:\\maxvalue.txt");
}

void CHCRRecognizer::Preprocess()
{

	CPoint rightlower = RebuildCoordinate();

	LinearNormalize(rightlower);
	
	FilterOverSampling();
	
	DDALine();

	switch (curfea)
	{
		
		case 0: DirChFeaExtract(); break;
		case 1: DirChFeaExtract1(); break;
		case 2: DirFeaExtract(); break;
	}
	
	MaxValueFilter3x3();
	
	GaussianFilter();

}

void CHCRRecognizer::DirChFeaExtract1()
{
	// Clear Direction-change feature matrixes
	for (int i=0; i<8; i++)
	{
		for (int j=0;j<PATTERN_SIZE;j++)
		{
			for (int k=0;k<PATTERN_SIZE;k++)
			{
				m_fea[i][j][k] = 0;
			}
		}
	}
	
	// Too few points cannot extract direction-change feature
	if (ptnum < 3)
	{
		//MessageBox(0,"Too few points, Direction-change feature will be set to all zero (default value).",0,MB_OK);
	}
	else // Start to extract direction-change feature
	{
		for (i=1; i < ptnum-1; i++)
		{
			int dirch = CalcDegCh(m_points[i-1],m_points[i],m_points[i+1]);
			float Fdc = 1.0f;
			Fdc = float(abs(dirch)) / 60.0f + 1.0f;
			int newDirection = CalcDirection(m_points[i],m_points[i+1]);
			m_fea[newDirection][m_points[i].x][m_points[i].y] = Fdc;
		}
	}
}

void CHCRRecognizer::DirFeaExtract()
{
	// Clear Directional feature matrixes
	for (int i=0; i<8; i++)
	{
		for (int j=0;j<PATTERN_SIZE;j++)
		{
			for (int k=0;k<PATTERN_SIZE;k++)
			{
				m_fea[i][j][k] = 0;
			}
		}
	}
	
	// Too few points cannot extract directional feature
	if (ptnum < 2) { /*MessageBox(0,"Too few points, Directional feature will be set to all zero (default value).",0,MB_OK);*/ }
	else // Start to extract direction-change feature
	{
		for (i=0; i < ptnum; i++)
		{
			CPoint ptPrev,ptNext;
			
			if (i==0)
			{
				ptPrev = m_points[0];
				ptNext = m_points[1];
			}
			else if (i==ptnum-1)
			{
				ptPrev = m_points[ptnum-2];
				ptNext = m_points[ptnum-1];
			}
			else
			{
				ptPrev = m_points[i-1];
				ptNext = m_points[i+1];
			}
			
			int curDirection = CalcDirectionWithOffset(ptPrev,ptNext);
			int dx = abs( ptNext.x - ptPrev.x );
			int dy = abs( ptNext.y - ptPrev.y );
			float s = float(sqrt( dx*dx + dy*dy ));
			if (s!=0) //if s==0 then ptPrev==ptNext, the m_dirfea at this point should be all zero
			{
				if ( curDirection % 2 == 0 ) // even
				{
					m_fea[curDirection][m_points[i].x][m_points[i].y] = (abs( dx - dy )) / s;
					m_fea[curDirection+1][m_points[i].x][m_points[i].y] = (min(dx,dy)*1.41421356f) / s;
				}
				else //odd
				{
					m_fea[(curDirection+1)%8][m_points[i].x][m_points[i].y] = (abs( dx - dy )) / s;
					m_fea[curDirection][m_points[i].x][m_points[i].y] = (min(dx,dy)*1.41421356f) / s;
				}
			}
		}
	}
}