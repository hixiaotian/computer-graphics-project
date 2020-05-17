
#include "stdafx.h"
#include "canvasr.h"
#include "canvasFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// canvasFrame

CDC *mdc,*Tmpmdc;
CBitmap *bgbitmap,*Tmpbitmap,*p1,*p2,*p3,*p4,*p5,*p6;
int i=0;
int sudu=20;
int DirtInt=1;
int count=0;
int ix,iy;



int randKeyUD,randKeyLR;

struct snow
{
  int x;
  int y;
  BOOL exist;
  int vx,vy;
 };

snow flakes[50];

//¶¨Òå¹«¹²±äÁ¿


IMPLEMENT_DYNCREATE(canvasFrame, CFrameWnd)

canvasFrame::canvasFrame()
{
	RECT rect;
	Create(NULL,"»æÍ¼´°¿Ú");
	CClientDC dc(this);
	int width = dc.GetDeviceCaps(HORZRES);
	int height = dc.GetDeviceCaps(VERTRES);
	GetWindowRect( &rect );
	width = ( width -  640 )/2 ;
	height = (height - 480)/2 ;
	MoveWindow( width , height , 640 , 480 ,true);


	mdc=new CDC;
	Tmpmdc=new CDC;
	p1=new CBitmap;
	p2=new CBitmap;
	p3=new CBitmap;
	p4=new CBitmap;
	p5=new CBitmap;
	p6=new CBitmap;
	bgbitmap=new CBitmap;
	Tmpbitmap=new CBitmap;


   
	mdc->CreateCompatibleDC(&dc);//½¨Á¢ÓëAppµÄdc¼æÈÝµÄmdc
	Tmpmdc->CreateCompatibleDC(&dc);

		//¼ÓÔØ±³¾°Í¼Æ¬
	bgbitmap->m_hObject=(HBITMAP)::LoadImage(NULL,
		                          "BGROUND.BMP",
								  IMAGE_BITMAP,
								  0,
								  0,
								  LR_LOADFROMFILE);


	//¼ÓÔØp1Í¼Æ¬
	p1->m_hObject=(HBITMAP)::LoadImage(NULL,
		                         "1.bmp",
								 IMAGE_BITMAP,
								 10,
								 10,
								 LR_LOADFROMFILE);

	//¼ÓÔØp2Í¼Æ¬
	p2->m_hObject=(HBITMAP)::LoadImage(NULL,
		                         "2.bmp",
								 IMAGE_BITMAP,
							     10,
								 10,
								 LR_LOADFROMFILE);

	//¼ÓÔØp3Í¼Æ¬
	p3->m_hObject=(HBITMAP)::LoadImage(NULL,
		                         "3.bmp",
								 IMAGE_BITMAP,
								 10,
								 10,
								 LR_LOADFROMFILE);

	//¼ÓÔØp4Í¼Æ¬
	p4->m_hObject=(HBITMAP)::LoadImage(NULL,
		                         "4.bmp",
								 IMAGE_BITMAP,
								 10,
								 10,
								 LR_LOADFROMFILE);

	//¼ÓÔØp5Í¼Æ¬
	p5->m_hObject=(HBITMAP)::LoadImage(NULL,
		                         "5.bmp",
								 IMAGE_BITMAP,
								 10,
								 10,
								 LR_LOADFROMFILE);

	//¼ÓÔØp6Í¼Æ¬
	p6->m_hObject=(HBITMAP)::LoadImage(NULL,
		                         "6.bmp",
								 IMAGE_BITMAP,
								 10,
								 10,
								 LR_LOADFROMFILE);

	Tmpbitmap->CreateCompatibleBitmap(&dc,rect.right,rect.bottom);
	mdc->SelectObject(Tmpbitmap);
}


canvasFrame::~canvasFrame()
{
	
	delete mdc;
	delete Tmpmdc;
	delete p1;
	delete p2;
	delete p3;
	delete p4;
	delete p5;
	delete p6;
	delete bgbitmap;

	KillTimer(1);
}


BEGIN_MESSAGE_MAP(canvasFrame, CFrameWnd)
	//{{AFX_MSG_MAP(canvasFrame)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// canvasFrame message handlers

void canvasFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	RECT rect;
	GetClientRect(&rect);
	
	//±³¾°Í¼Æ¬
	Tmpmdc->SelectObject(bgbitmap);
	mdc->BitBlt(0,0,rect.right,rect.bottom,Tmpmdc,0,0,SRCCOPY);

   //Á£×ÓµÄÍ¼Æ¬
    //Tmpmdc->SelectObject(p1);
	//mdc->BitBlt(0,0,10,10,Tmpmdc,0,0,SRCCOPY);


	dc.BitBlt(0,0,rect.right,rect.bottom,mdc,0,0,SRCCOPY);


	ix=rand()%rect.right ;
	iy=rand()%rect.bottom;

	for(i=1;i<50;i++)
	{
          flakes[i].x=ix;
          flakes[i].y=iy;

	      flakes[i].exist=true;

	       if (i%4==0)
			{
			   flakes[i].vx=rand()%40;
			   flakes[i].vy=rand()%40;
			}
		    else
		    if (i%4==1)
			{
			   flakes[i].vx=-rand()%40;
			   flakes[i].vy=rand()%40;
			}
		    else
		    if (i%4==2)
			{
			   flakes[i].vx=-rand()%40;
			   flakes[i].vy=-rand()%40;
			}
		    else
		    if (i%4==3)
			{
			   flakes[i].vx=rand()%40;
			   flakes[i].vy=-rand()%40;
			}

	        count=50;
	}


}

void canvasFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default


	RECT rect;
	GetClientRect(&rect);
	CClientDC dc(this);

	//±³¾°Í¼Æ¬
	Tmpmdc->SelectObject(bgbitmap);
	mdc->BitBlt(0,0,rect.right,rect.bottom,Tmpmdc,0,0,SRCCOPY);

 	ix=rand()%(rect.right-150)+50;
	iy=rand()%(rect.bottom-150)+50;
    

	//±¬Õ¨µã------------------------------------------------------------------------------------------------
	if (count<5)                           
	{
		for(i=1;i<50;i++)
		{
			flakes[i].x=ix;
			flakes[i].y=iy;
			flakes[i].exist=true;
			
            if (i%4==0)
			{
			   flakes[i].vx=rand()%30+2;
			   flakes[i].vy=rand()%30+2;
			}
		    else
		    if (i%4==1)
			{
			   flakes[i].vx=-rand()%30-2;
			   flakes[i].vy=rand()%30+2;
			}
		    else
		    if (i%4==2)
			{
			   flakes[i].vx=-rand()%30+2;
			   flakes[i].vy=-rand()%30-2;
			}
		    else
		    if (i%4==3)
			{
			   flakes[i].vx=rand()%30+2;
			   flakes[i].vy=-rand()%30-2;
			}
		}
		count=50;
	}
    //±¬Õ¨µã------------------------------------------------------------------------------------------------


    //´¦Àí±¬Õ¨------------------------------------------------------------------------------------------------
	for(i=1;i<50;i++)
	{

      if(flakes[i].exist)
	  {
  


		  flakes[i].x=flakes[i].x+flakes[i].vx;
		  flakes[i].y=flakes[i].y+flakes[i].vy+15;


	      if (flakes[i].x>rect.right||flakes[i].x<-10||flakes[i].y>rect.bottom||flakes[i].y<-10)
		  {
			flakes[i].exist=false;
			count--;
		  }


	   }

	    

	  Tmpmdc->SelectObject(p1);
      mdc->BitBlt(flakes[i].x,flakes[i].y,10,10,Tmpmdc,0,0,SRCCOPY);

	}

      
    dc.BitBlt(0,0,rect.right,rect.bottom,mdc,0,0,SRCCOPY);
	CFrameWnd::OnTimer(nIDEvent);

	CString s;
	s.Format("%d",count);
	dc.TextOut(0,0,s);
}





int canvasFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetTimer(1,50,NULL);
// TODO: Add your specialized creation code here

	return 0;
}





void canvasFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
