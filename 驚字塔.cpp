//rotateblt ,Level20���Ǫ��ƶq���p�� 
#include <stdio.h>
#include <windows.h>
#include <string.h>  
#include <stdlib.h>   // rand() srand()  system ("cls")
#include <time.h>    // time()
#include <conio.h> 
#include <math.h>

int sw=1920;
int sh=1080;
int  sx = 27;
int  sy = 37;
int pixel= 40;
#define HP 999
int mn =20; //1~100
#define bn 20  //bomb number
#define spacetime 0.2

struct mover   //struct�G�ŧi�N�ϥε��c   mover �G���a+���~   mover[0]�����~���;���   mover[1]�����s�� 
{
	int tp;  //type==1 �N���ҥ� �F type==0 �N���ҥ�
	int nx;  //new x �y��  
	int ny;  //new y �y�� 
	int ox;  //old x �y�� 
	int oy;  //old y �y��  
	int ws;  //WalkingState
	int d;  //Direction�G���a+���~ �����ʤ�V 
	int e;  //evermove�G�b�����j�餤�O�_���ʹL 
	int sl;  //space_lock�G�O�_�Q�ɶ���� 
	double st;  //space_time
	LARGE_INTEGER t1; 
	LARGE_INTEGER t2; 
};
struct weapon
{                  
	int x;  //weapon  x �y�� 
	int y;  //weapon  y �y��
	int tp ;  //weapon ����(type) 
	int tm;   //time
	int e;  //evermove�G�b�����j�餤�O�_���ʹL 
	int sl;  //space_lock�G�O�_�Q�ɶ���� 
	double st;  //space_time
	LARGE_INTEGER t1; 
	LARGE_INTEGER t2; 
};

struct mover mover[130];     //�ŧi�}�Cmover[30] �����cmover���κA   
struct weapon w[bn+1];
int i,j,k,l,o,bx,by,nmn,sec,hp,Map_1[100][100],Map_2[100][100],temp_Map_1[100][100],BreakWall[100][100],levelkey[10],directionorder[]={1,2,3,4};
int Level,fromlevel,viewmove_x=0,viewmove_y=0,weapontype,thesame,levelever[31]={0};
int greetlist[10]={0};
float fi,fj;
char display[100],errs[1024];  
RECT rect;
//�ɶ� 
LARGE_INTEGER  frequency;
clock_t clock1,clock2,tclock1,tclock2;
//�r�� 
HFONT font1,font2,font3,font4;
//�ƹ� 
POINT mouse1,mouse2,mouse3;
COLORREF colorref;
BYTE   bRed,bGreen,bBlue;
//���� 
MCIERROR music_err;
MCI_OPEN_PARMS music_open[200];
MCI_PLAY_PARMS music_play[200];
int music_total_length[200],music_current_position[200];
MCI_STATUS_PARMS music_status[200]; // �x�s���֪��A���ܼ�


///////////////////////////�H�U�K��+�}����///////////////////////////////////// 

typedef struct pic{  //�C�@�i�ϳ��n����������T
	HDC dc;
	HBITMAP hbm;
	int width;
	int height;	
}picture ;
HWND hwnd;
HDC  dc1,dc2,dc3,dc4; 
HBITMAP hbm,hbm3,hbm4;
picture p[200];
MSG msg;
int client_sw,client_sh;
HBRUSH tempBrush = CreateSolidBrush( RGB(255, 0, 0) );
picture readBMP(const char *fname, HDC dc1)//��ݬۮe��dc1
	{
	 	picture tp;
		tp.dc =CreateCompatibleDC(dc1) ;
		tp.hbm=(HBITMAP)LoadImage(NULL,fname,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		
		//�������L���TŪ�� {1 
		BITMAP temp ;
		GetObject(tp.hbm,sizeof(BITMAP), &temp) ; //�NŪ�쪺�F���� temp�̭� 
		if((temp.bmWidth<=0)||(temp.bmHeight<=0)) //�Y��Ū��F��>�Ϥ����e<=0 
		{
			sprintf(display,"�L�kŪ��%c",*fname);
			MessageBox(NULL,fname,display,MB_ICONERROR);
			MessageBox(NULL,fname,"�L�kŪ��",MB_ICONERROR);
		}
		tp.width=temp.bmWidth;
		tp.height=temp.bmHeight;
		SelectObject(tp.dc,tp.hbm);	
		return tp;	
	}
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) {
	if( message == WM_DESTROY ) {
		PostQuitMessage( 0 );
		return 0;
	}
	else if( message == WM_CLOSE ) {
		DestroyWindow( hwnd );
		return 0;
	}
	else
		return DefWindowProc( hwnd, message, wparam, lparam );
}
void reg_class( const char *cname ) {
	WNDCLASS wnd;
	ZeroMemory( &wnd, sizeof( WNDCLASS ) );
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = WndProc;
	wnd.hIcon = (HICON)LoadImage( NULL, "pic\\���r��.ico" ,IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE );
	wnd.hbrBackground = ( HBRUSH )COLOR_BACKGROUND;
	wnd.lpszClassName = cname;
	
	if( RegisterClass( &wnd ) == 0 ){
		MessageBox( NULL, "�L�k���U", "In reg_class", MB_ICONERROR );
	}
}
///////////////////////////�H�W�K��+�}����///////////////////////////////////// 


//////////�H�U�g�c�۰ʥͦ�////////////
int g[100][100] ,used[100][100],ret[100][100],n=sx/2, m=sy/2,dx[] = {0,0,1,-1},dy[] = {1,-1,0,0};
void dfs(int x, int y, int px, int py) {
    n=sx/2;
	m=sy/2;
	if(x < 0 || y < 0 || x >= n || y >= m)
        return;
    if(used[x][y])
        return;
    used[x][y] = 1;
    if(px >= 0) {
        ret[(    (x*2+1)+(px*2+1)   )/2 ][((y*2+1)+(py*2+1))/2] = 1;
    }
    ret[x*2+1][y*2+1] = 1;
    int cnt = 0, dir = 0;
    if(x == n-1 && y == m-1)    return;
    while(cnt < 10) {
        dir = rand()%4;
        dfs(x+dx[dir], y+dy[dir], x, y);
        cnt++;
    }
}
//////////�H�W�g�c�۰ʥͦ�////////////







void Boundary()
{
	for(i=0;i<sx;i+=1)    //�ഫ�H���g�c��Map  
	{
		for(j=0;j<sy;j+=1)
		{
			if (ret[i][j]==0)
				Map_1[i][j]=11;
			else if (ret[i][j]==1)
				Map_1[i][j]=0;
		}
	}
	
	
		
	for(i=(sx+1 )/2 -2;i<=(sx+1 )/2 +2;i++)   //Level���ഫ�I 
	{
		for(j=(sy+1 )/2 -2;j<=(sy+1 )/2 +2;j++)
		{
			Map_1[i][j]=0;  
		}
	}
	for(i=(sx+1 )/2 -1;i<=(sx+1 )/2 +1;i++)   
	{
		for(j=(sy+1 )/2 -1;j<=(sy+1 )/2 +1;j++)
		{
			Map_1[i][j]=12;  
		}
	}
	
	for(i=sx-6;i<=sx-2;i++)   //�i�J�p�C�����ഫ�I 1
	{
		for(j=1;j<=5;j++)
		{
			Map_1[i][j]=0;  
		}
	}
	for(i=sx-5;i<=sx-3;i++)   
	{
		for(j=2;j<=4;j++)
		{
			Map_1[i][j]=13;  
		}
	}
	
	
	for(i=sx-6;i<=sx-2;i++)   //�i�J�p�C�����ഫ�I 2
	{
		for(j=sy-6;j<=sy-2;j++)
		{
			Map_1[i][j]=0;  
		}
	}
	for(i=sx-5;i<=sx-3;i++)   
	{
		for(j=sy-5;j<=sy-3;j++)
		{
			Map_1[i][j]=14;  
		}
	}
	
	
	for(i=1;i<=5;i++)   //�i�J�p�C�����ഫ�I 3
	{
		for(j=sy-6;j<=sy-2;j++)
		{
			Map_1[i][j]=0;  
		}
	}
	for(i=2;i<=4;i++)   
	{
		for(j=sy-5;j<=sy-3;j++)
		{
			Map_1[i][j]=15;  
		}
	}
	
	

	
	for(i=1;i<=5;i++)   //�i�J�p�C�����ഫ�I 4
	{
		for(j=1;j<=5;j++)
		{
			Map_1[i][j]=0;  
		}
	}
	for(i=2;i<=4;i++)   
	{
		for(j=2;j<=4;j++)
		{
			Map_1[i][j]=16;  
		}
	}
	
	
	
	
	for(i=0;i<sx;i+=1)    //�ƻs�g�c�� temp_Map_1
	{
		for(j=0;j<sy;j+=1)
		{
			if (Map_1[i][j]==11)
				temp_Map_1[i][j]=11;
			else if (Map_1[i][j]==0)
				temp_Map_1[i][j]=0;
		}
	}
	
	
	for(i=0;i<sx;i+=1)     //�ø� 
	{
		for(j=0;j<sy;j+=1)
		{
			if (Map_1[i][j]==0)
			{
				if(temp_Map_1[i+1][j]==0)
					Map_1[i][j]++;
				if(temp_Map_1[i][j+1]==0)
					Map_1[i][j]++;
				if(temp_Map_1[i-1][j]==0)
					Map_1[i][j]++;
				if(temp_Map_1[i][j-1]==0)
					Map_1[i][j]++;
			}
		}
	}
	for(i=0;i<sx;i+=1)    //�ƻs�g�c�� Map_2
	{
		for(j=0;j<sy;j+=1)
		{
			Map_2[i][j]=Map_1[i][j]; 
		}
	}
	
	for(i=0;i<sx;i+=1)     //�}�� 
	{
		for(j=0;j<sy;j+=1)
		{
			if(BreakWall[i][j]==1)
				Map_1[i][j]=10;		
		}
	}
};

void NextStep (int k)
{
	mover[k].ox=mover[k].nx;
	mover[k].oy=mover[k].ny;
	switch (mover[k].d)
	{
		case 37: 
			mover[k].ny--;
			mover[k].e=1; 
			break;
		case 38: 
			mover[k].nx--;
			mover[k].e=1;
			break;
		case 39: 
			mover[k].ny++;
			mover[k].e=1;
			break;
		case 40: 
			mover[k].nx++;
			mover[k].e=1;
			break;
		default:
			break;
	}
};

void BoundaryCondition(int bc_k)
{
	
	if(bc_k<=30)  //���a
	{
		if (Map_1[mover[bc_k].nx][mover[bc_k].ny]>=12 && Map_1[mover[bc_k].nx][mover[bc_k].ny]<=17) //�p�G���a�i�J�p�C�� 
			{
				switch (Map_1[mover[bc_k].nx][mover[bc_k].ny])
				{
					case 12: 
						if(Level==10)
							Level=16;
						else if(Level==20)
							Level=26;
						else if(Level==30)
							Level=36;
						break;
					case 13: 
						if(Level==10)
							Level=11;
						else if(Level==20)
							Level=21;
						else if(Level==30)
							Level=31;
						break;
					case 14: 
						if(Level==10)
							Level=12;
						else if(Level==20)
							Level=22;
						else if(Level==30)
							Level=32;
						break;
					case 15: 
						if(Level==10)
							Level=13;
						else if(Level==20)
							Level=23;
						else if(Level==30)
							Level=33;
						break;
					case 16: 
						if(Level==10)
							Level=14;
						else if(Level==20)
							Level=24;
						else if(Level==30)
							Level=34;
						break;
					case 17: 
						if(Level==10)
							Level=15;
						else if(Level==20)
							Level=25;
						else if(Level==30)
							Level=35;
						break;
					default:
						break;
				}
				
				mover[bc_k].nx=mover[bc_k].ox;
				mover[bc_k].ny=mover[bc_k].oy;
				mover[bc_k].d=0;          //�ð���� 
			}
		else if (Map_1[mover[bc_k].nx][mover[bc_k].ny]>=11 && Map_1[mover[bc_k].nx][mover[bc_k].ny]<=20) //�p�G���a�]��������A�Y�h�^���e����m 
			{
				mover[bc_k].nx=mover[bc_k].ox;
				mover[bc_k].ny=mover[bc_k].oy;
				mover[bc_k].d=0;          //�ð���� 
			}
		
	}
	else if(bc_k<=130)   //���~ 
	{
	 	
		if (Map_2[mover[bc_k].nx][mover[bc_k].ny]==3  || Map_2[mover[bc_k].nx][mover[bc_k].ny]==4)
		{
				mover[bc_k].d=37 +rand()%4 ;
		}
		
		if (Map_1[mover[bc_k].nx][mover[bc_k].ny]>=10 && Map_1[mover[bc_k].nx][mover[bc_k].ny]<=20)
		{
			mover[bc_k].nx=mover[bc_k].ox;
			mover[bc_k].ny=mover[bc_k].oy;
			//mover[bc_k].d=37 +rand()%4 ;
			//NextStep (bc_k);
			
			srand((unsigned)time(NULL));
			
			/*	*/
			for(o=0;o<=3;o++)
			{			
				j=rand()%4;				
				l=directionorder[o];				
				directionorder[o]=directionorder[j];				
				directionorder[j]=l;
			}
			
			
			for(o=0;o<=3;o++)
			{
				switch (directionorder[o])
				{	 
					case 1:	
						if(Map_1[mover[bc_k].nx][mover[bc_k].ny-1]>=0  &&  Map_1[mover[bc_k].nx][mover[bc_k].ny-1]<=4 )
							mover[bc_k].d=37;
						break;	
					case 2:	
						if(Map_1[mover[bc_k].nx-1][mover[bc_k].ny]>=0  &&  Map_1[mover[bc_k].nx-1][mover[bc_k].ny]<=4 )
							mover[bc_k].d=38;
						break;
					case 3:	
						if(Map_1[mover[bc_k].nx][mover[bc_k].ny+1]>=0  &&  Map_1[mover[bc_k].nx][mover[bc_k].ny+1]<=4)
							mover[bc_k].d=39;
						break;
					case 4:	
						if(Map_1[mover[bc_k].nx+1][mover[bc_k].ny]>=0  &&  Map_1[mover[bc_k].nx+1][mover[bc_k].ny]<=4)
							mover[bc_k].d=40;
						break;
					default:
							break;
				} 
			}
		}
	}
};

void Sound(int Tracks)
{
	
	if(Tracks==0)   //��l�� 
	{
	
		
		music_open[1].lpstrDeviceType="mpegvideo";
		music_open[1].lpstrElementName="mp3//�宪�j�v�ưϦҧJ�q�v���n�a���@.mp3";
		
		
		music_open[2].lpstrDeviceType="mpegvideo";
		music_open[2].lpstrElementName="mp3/Town Theme.mp3";
		
		music_open[3].lpstrDeviceType="mpegvideo";
		music_open[3].lpstrElementName="mp3//Move1.mp3";
		
		music_open[4].lpstrDeviceType="mpegvideo";
		music_open[4].lpstrElementName="mp3//Battle 1.mp3";
		
		music_open[5].lpstrDeviceType="mpegvideo";
		music_open[5].lpstrElementName="mp3//Cursor1.mp3";
		
		music_open[6].lpstrDeviceType="mpegvideo";
		music_open[6].lpstrElementName="mp3//Crossbow.mp3";
		
		//�}�ҭ��� 
		for (i=1;i<=6;i++)
		{
			music_err=mciSendCommand( 0,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_OPEN_TYPE,(DWORD_PTR)&music_open[i]); 
			if(music_err)
			{
				mciGetErrorString(music_err,errs,sizeof(errs));
				MessageBox(NULL, errs, "����", MB_OK );
			} 
		}
	}	
	
	if(Tracks!=999)
	{
		if (Tracks==1 || Tracks==2)
		{
			// �]�w���֪���
			music_status[Tracks].dwItem = MCI_STATUS_LENGTH;
			// ���o�����`����
			mciSendCommand( music_open[Tracks].wDeviceID, MCI_STATUS, MCI_STATUS_ITEM,
			(DWORD_PTR)&music_status[Tracks] );
			// �N���֤j�p�s�J total_length
			music_total_length[Tracks]= music_status[Tracks].dwReturn;

			// �]�w���֦�m
			music_status[Tracks].dwItem = MCI_STATUS_POSITION;
			// ���o�ثe���֦�m 
			mciSendCommand( music_open[Tracks].wDeviceID, MCI_STATUS, MCI_STATUS_ITEM,(DWORD_PTR)&music_status[Tracks] );
			
			// �N�ثe��m�s�J current_position
			music_current_position[Tracks] = music_status[Tracks].dwReturn;
			
			if( music_total_length[Tracks] <= music_current_position[Tracks] )  //���ּ����K�`������ 
			{
				//�����
				mciSendCommand(music_open[Tracks].wDeviceID,MCI_STOP,MCI_WAIT,(DWORD_PTR)&music_play[Tracks]);
				//�^��̶}�l
				mciSendCommand(music_open[Tracks].wDeviceID,MCI_SEEK,MCI_SEEK_TO_START,0);
			}
			 
			else if(  music_current_position[Tracks]==0 )  //���֥����K�}�l���� 
			{
				music_err=mciSendCommand(music_open[Tracks].wDeviceID,MCI_PLAY,0,(DWORD_PTR)&music_play[Tracks]);
				if(music_err)
				{
					mciGetErrorString(music_err,errs,sizeof(errs));
					MessageBox(NULL, errs, "����", MB_OK ); 
				} 
			}
		}
		
		if (Tracks>=3  && Tracks<=6)
		{
			//�����
			mciSendCommand(music_open[Tracks].wDeviceID,MCI_STOP,MCI_WAIT,(DWORD_PTR)&music_play[Tracks]);
			//�^��̶}�l
			mciSendCommand(music_open[Tracks].wDeviceID,MCI_SEEK,MCI_SEEK_TO_START,0);
			
			//���񭵼� 
			music_err=mciSendCommand(music_open[Tracks].wDeviceID,MCI_PLAY,0,(DWORD_PTR)&music_play[Tracks]);
			if(music_err)
			{
				mciGetErrorString(music_err,errs,sizeof(errs));
				MessageBox(NULL, errs, "����", MB_OK ); 
			} 
		}
		if (Tracks==10)
		{
			
				//�����
				mciSendCommand(music_open[1].wDeviceID,MCI_STOP,MCI_WAIT,(DWORD_PTR)&music_play[1]);
				//�^��̶}�l
				mciSendCommand(music_open[1].wDeviceID,MCI_SEEK,MCI_SEEK_TO_START,0);
		}
		
		if (Tracks==20)
		{
			
				//�����
				mciSendCommand(music_open[2].wDeviceID,MCI_STOP,MCI_WAIT,(DWORD_PTR)&music_play[2]);
				//�^��̶}�l
				mciSendCommand(music_open[2].wDeviceID,MCI_SEEK,MCI_SEEK_TO_START,0);
		}
		
	}
		
		
	if(Tracks==999)   //�������� 
	{
		//�b�̫ᶷ�[�W�H�U���e�A�~��b����������A����񭵼�
		for (i=1;i<=6;i++)
		{
			mciSendCommand(music_open[i].wDeviceID,MCI_STOP,0,(DWORD_PTR)&music_play[i]);
			mciSendCommand(music_open[i].wDeviceID,MCI_CLOSE,0,(DWORD_PTR)&music_play[i]);
		}
	} 
};

void WalkingState(int ws_k)  //�V���V�k+���}�k�} 
{
	if(1) 	
	{
		if( mover[ws_k].d==40)      //�V�U 
		{
			if (mover[ws_k].ws==21)
			{
				mover[ws_k].ws=22;
			}
			else if (mover[ws_k].ws==22)
			{
				mover[ws_k].ws=23;
			}
			else if (mover[ws_k].ws==23)
			{
				mover[ws_k].ws=22;
			}
			else 
			{
				mover[ws_k].ws=22;
			}				
		}
		else if(mover[ws_k].d==37)    	  //�V�� 
		{
			if (mover[ws_k].ws==24)
			{
				mover[ws_k].ws=25;
			}
			else if (mover[ws_k].ws==25)
			{
				mover[ws_k].ws=26;
			}
			else if (mover[ws_k].ws==26)
			{
				mover[ws_k].ws=25;
			}
			else 
			{
				mover[ws_k].ws=25;
			}			
		}
		else if(mover[ws_k].d==39)    	  //�V�k 
		{
			if (mover[ws_k].ws==27)
			{
				mover[ws_k].ws=28;
			}
			else if (mover[ws_k].ws==28)
			{
				mover[ws_k].ws=29;
			}
			else if (mover[ws_k].ws==29)
			{
				mover[ws_k].ws=28;
			}
			else 
			{
				mover[ws_k].ws=28;
			}
			
		}
		else if(mover[ws_k].d==38)  	 //�V�W 
		{
			if (mover[ws_k].ws==30)
			{
				mover[ws_k].ws=31;
			}
			else if (mover[ws_k].ws==31)
			{
				mover[ws_k].ws=32;
			}
			else if (mover[ws_k].ws==32)
			{
				mover[ws_k].ws=31;
			}
			else 
			{
				mover[ws_k].ws=31;
			}
		}
		else if(mover[ws_k].d==38) 
		{
			if (mover[ws_k].ws>=21 && mover[ws_k].ws<=23)
				mover[ws_k].ws=21;
			if (mover[ws_k].ws>=24 && mover[ws_k].ws<=26)
				mover[ws_k].ws=24;
			if (mover[ws_k].ws>=27 && mover[ws_k].ws<=29)
				mover[ws_k].ws=27;
			if (mover[ws_k].ws>=30 && mover[ws_k].ws<=32)
				mover[ws_k].ws=30;
		}
	}
};


void GameClock(int gameclock)
{
	if (gameclock==1)
	{
		clock1 = clock();
		
	}
	if (gameclock==2)
	{
		clock2 = clock();
	    
	    sec=(clock2-clock1)/(double)(CLOCKS_PER_SEC);
	    
	    SelectObject(dc3,font2);  //dc2�e�����r�鳣�ܬ�font1    
	    
		sprintf(display,"%d:%2d:%2d",sec/3600%60,sec/60%60,sec%60);
		TextOut(dc3,1500,1045,display,strlen(display));
	}
    
    
};

void InitialSetting(int initial)
{	
	if (initial==1)
	{
		font1=CreateFont(40,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,TEXT("�з���"));  
		font2=CreateFont(25,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,TEXT("�з���"));  
		font3=CreateFont(20,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,TEXT("�з���"));  
		font4=CreateFont(10,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,TEXT("�з���"));  
		//dc2�e�����r�鳣�ܬ�font1 
		SelectObject(dc3,font1); 
		SelectObject(dc2,font1); 
		SelectObject(dc1,font1); 
		//���ܦr���C��
		SetTextColor(dc3,0x00FFFFFF);
		SetTextColor(dc2,0x00FFFFFF);
		SetTextColor(dc1,0x00FFFFFF);
		// ���ܦr��I���C��
		SetBkColor(dc3,0x00000000);
		SetBkColor(dc2,0x00000000);
		SetBkColor(dc1,0x00000000);
		//TextOut ���Ҧ��GOPAQUE���   /  TRANSPARENT�z�� 
		SetBkMode( dc3,  TRANSPARENT );
		SetBkMode( dc2, TRANSPARENT );
		SetBkMode( dc1, TRANSPARENT );
		
		SetStretchBltMode(dc1, HALFTONE); //����StretchBlt���t��k 
		SetBrushOrgEx(dc1, 0, 0, NULL);
		
	}
	
	if (initial==2)  //�j�r 
	{
		
		SelectObject(dc3,font1);  //dc2�e�����r�鳣�ܬ�font1 		
		SetTextColor(dc3,0x00FFFFFF);  //���ܦr���C��
		SetBkColor(dc3,0x00000000);  // ���ܦr��I���C��
		SetBkMode( dc3,  TRANSPARENT ); //TextOut ���Ҧ��GOPAQUE���   /  TRANSPARENT�z�� 
		
		SelectObject(dc2,font1);  //dc2�e�����r�鳣�ܬ�font1 		
		SetTextColor(dc2,0x00FFFFFF);  //���ܦr���C��
		SetBkColor(dc2,0x00000000);  // ���ܦr��I���C��
		SetBkMode( dc2,  TRANSPARENT ); //TextOut ���Ҧ��GOPAQUE���   /  TRANSPARENT�z��
		
		SelectObject(dc1,font1);  //dc2�e�����r�鳣�ܬ�font1 		
		SetTextColor(dc1,0x00FFFFFF);  //���ܦr���C��
		SetBkColor(dc1,0x00000000);  // ���ܦr��I���C��
		SetBkMode( dc1,  TRANSPARENT ); //TextOut ���Ҧ��GOPAQUE���   /  TRANSPARENT�z��
	}
	if (initial==3)  //�_�l��m
	{
		if( Level==10 )
		{
			GameClock(1);
			sx = 27;
			sy = 37;
			mn =20;
			hp=999;
			levelever[10]=1;
		}
		if( Level==20)
		{
		 	sx = 37;
			sy = 47;
			mn =50;
			hp=999;
			levelever[20]=1;
		}
		if( Level==30)
		{
		 	sx = 47;
			sy = 57;
			mn =100;
			hp=999;
			levelever[30]=1;
		}
		
		
		
		
		
		memset(g, 0, sizeof(g)); 
		memset(used, 0, sizeof(used));
		memset(ret, 0, sizeof(ret));
		srand((unsigned)time(NULL));
		dfs(0,0, -1,-1);
		Boundary();
		
		mover[21].tp=1;	//���a�_�l��m
		mover[21].st=0.01;
		mover[21].ws=21;
	   
        mover[21].nx=(sx+1)/2;   
		mover[21].ny=(sy+1)/2+2; 
		Map_1[mover[21].nx][mover[21].ny]=21; 
		hp=HP;
		
		for (i=31;i<=30+5;i++) //���~�_�l��m  
		{
			mover[i].tp=1;	
			mover[i].st=0.3;
			mover[i].ws=131;
			do{
				mover[i].nx=rand()%sx; 
				mover[i].ny=rand()%sy ;
			}while(Map_1[mover[i].nx][mover[i].ny]>=11 && Map_1[mover[i].nx][mover[i].ny]<=20);
			Map_1[mover[i].nx][mover[i].ny]=31; 
			mover[i].d=37+rand()%4;
		}
		for (i=0;i<=bn;i++) //���u�_�l  
		{
			w[i].st=0.5;
			w[i].e=0;
			w[i].tp=0;
			
		}	
	}
};

void Editor()
{
	//���X�ֳt���ʱ���O 
	if (GetAsyncKeyState(16)<0 && GetAsyncKeyState(17)<0 && GetAsyncKeyState(18)<0  )   //�� Shift +  Ctrl + Alt 
	{
		//MessageBox(NULL,"�� �FShift +  Ctrl + Alt ","����",MB_OK);
		SelectObject(dc3,font1); 
		InitialSetting(2);
		
		i=Level;
		while(1)
		{
			StretchBlt(dc3,370,270,500,200,p[81].dc,0,0,p[80].width,p[80].height,SRCCOPY); 
			
			sprintf(display,"�����ܦ�h?");
			TextOut(dc3,400,300,display,strlen(display));
			sprintf(display,"10~16�B20~26�B30~36");
			TextOut(dc3,400,350,display,strlen(display));
			
			if ( GetAsyncKeyState(40)<0  )
				i--;
			if ( GetAsyncKeyState(38)<0  )
				i++;
			if ( GetAsyncKeyState(37)<0  )
				i-=10;
			if ( GetAsyncKeyState(39)<0  )
				i+=10;
			Sleep(100); 
								
			
			sprintf(display,"%d",i);
			TextOut(dc3,400,400,display,strlen(display));
			
			StretchBlt(dc1,370,270,500,200,dc3,370,270,500,200,SRCCOPY);  //��i��  dc3  ���  dc1 
			
			if(GetAsyncKeyState(27)<0  && GetAsyncKeyState(17)<0)
				break;
				
			if(GetAsyncKeyState(13)<0 )
			{
				if (i==1||i==10||i==11||i==12||i==13||i==14||i==16||i==20||i==21||i==22||i==23||i==24||i==26||i==30||i==31||i==32||i==33||i==34||i==36||i==40 )
				Level=i;
				break;
			}
				
			while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0)
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
	}
	
};

void GetKey()
{	
	thesame=0;	//���a21�����񬵼u
	if(GetAsyncKeyState(32)<0)
	{	        
		
		for(i=2;w[i].tp==1;i++)
		{
			if (i>bn)
				thesame=1;	
		}
		for(j=1;j<=bn;j++)
		{
			if(mover[21].nx==w[j].x  &&  mover[21].ny==w[j].y)
			{
				thesame=1;
			}			
		}
		if(thesame==0)
		{
			w[i].x=mover[21].nx;
			w[i].y=mover[21].ny; 
			w[i].tp=1;
			w[i].tm=5;	
		}
	} 
	 
	for(i=37,mover[21].d=0;i<=40;i++)     //���a��V 
	{
		if(GetAsyncKeyState(i)<0)
		{
			mover[21].d=i;
		}
	}
	if(GetAsyncKeyState(65)<0)  //A
	{
		mover[21].d=37;
		
	}
	if(GetAsyncKeyState(87)<0)  //W
	{
		mover[21].d=38;
	}
	if(GetAsyncKeyState(68)<0)  //D
	{
		mover[21].d=39;

	}
	
	if(GetAsyncKeyState(83)<0)  //S
	{
		mover[21].d=40;
	}
	
	
	//�Y����� 
	if(GetAsyncKeyState(17)<0)  //Ctrl
	{
		if(GetAsyncKeyState(109)<0)  //+
		{
			pixel--;
		}
		if(GetAsyncKeyState(107)<0)  //-
		{
			pixel++;
		}
	}
	
	

	
} ;

void TimeControl(int tp,int tm_k)
{
	if (GetAsyncKeyState(192)<0)   //��~ 
	{
		MessageBox(NULL,"�bTimeControl���F~","����",MB_OK);
	}
	if(tp==1)
	{
		if(mover[tm_k].e==1)
		{
			mover[tm_k].sl=1 ;								
			QueryPerformanceCounter(&(mover[tm_k].t1));
		}
		if(mover[tm_k].sl==1) 						     //�Y�O��W�A�ݬO�_�i�H���� 
		{
			QueryPerformanceCounter(&(mover[tm_k].t2));          //�����p�� 
			QueryPerformanceFrequency(&frequency);	//���o�q�����檺�W�v ex:3.5GHz 										  //���D�����ɶ�
			if((double)((mover[tm_k].t2).QuadPart-(mover[tm_k].t1).QuadPart)/frequency.QuadPart>=mover[tm_k].st)  //(�����ɶ�-�}�l�ɶ�)/�W�v 
			{
				mover[tm_k].sl=0; 						//���� 
			} 
		}
	}
	else if (tp==2)
	{
		if(w[tm_k].e==1)
		{
			w[tm_k].sl=1 ;								
			QueryPerformanceCounter(&(w[tm_k].t1));
		}
		if(w[tm_k].sl==1) 						     //�Y�O��W�A�ݬO�_�i�H���� 
		{
			QueryPerformanceCounter(&(w[tm_k].t2));          //�����p�� 
			QueryPerformanceFrequency(&frequency);	//���o�q�����檺�W�v ex:3.5GHz 										  //���D�����ɶ�
			if((double)((w[tm_k].t2).QuadPart-(w[tm_k].t1).QuadPart)/frequency.QuadPart>=w[tm_k].st)  //(�����ɶ�-�}�l�ɶ�)/�W�v 
			{
				w[tm_k].sl=0; 						//���� 
			} 
		}
	}
	else if (tp==3)
	{
		if(	mover[tm_k].e==1)
		{
			mover[tm_k].sl=1 ;								
			QueryPerformanceCounter(&(mover[tm_k].t1));
		}
		if(	mover[tm_k].sl==1) 						     //�Y�O��W�A�ݬO�_�i�H���� 
		{
			QueryPerformanceCounter(&(	mover[tm_k].t2));          //�����p�� 
			QueryPerformanceFrequency(&frequency);	//���o�q�����檺�W�v ex:3.5GHz 										  //���D�����ɶ�
			if((double)((mover[tm_k].t2).QuadPart-(mover[tm_k].t1).QuadPart)/frequency.QuadPart>=	mover[tm_k].st)  //(�����ɶ�-�}�l�ɶ�)/�W�v 
			{
				mover[tm_k].sl=0; 						//���� 
			} 
		}
	}
};

void Arms() 
{
	/*i=1;
	sprintf(display,"%d�G(%2d,%2d)",i,w[i].x,w[i].y);
	TextOut(dc3,1490,950,display,strlen(display));
	i=2;
	sprintf(display,"%d�G(%2d,%2d)",i,w[i].x,w[i].y);
	TextOut(dc3,1490,1000,display,strlen(display));*/
	
	for(i=0;i<sx;i++)   //�M�Ŭ��u���� 
	{
		for(j=0;j<sw;j++)
		{
			if(Map_1[i][j]==51)
			  Map_1[i][j]=0;
		}
	}
	
	for(i=2;i<=bn;i++)
	{
		w[i].e=0;
		if (w[i].tp==1)
		{
			sprintf(display,"%2d�G(%2d,%2d)",i,w[i].x,w[i].y);
			TextOut(dc3,1490,900,display,strlen(display));
			
			Map_1[w[i].x][w[i].y]=56;   								//Problem
			  	
			if (w[i].sl==0)
			{
				if (w[i].tm>0)
				{
					w[i].tm--;	
				}
				 if (w[i].tm==0)
				{
					bx=w[i].x;						//�z�� 
					by=w[i].y;
					Map_1[bx][by]=51;
					for (l=0;  (l<5) &&!(Map_1[bx+l][by] >=11  &&Map_1[bx+l][by]<=20)  ;l++)
					{
						Map_1[bx+l][by]=51;
					}
					for (l=0;(l<5) &&!(Map_1[bx][by+l]>=11 &&Map_1[bx][by+l]<=20)    ;l++)
					{
						Map_1[bx][by+l]=51;
					}
					for (l=0;(l<5) &&!(Map_1[bx-l][by]>=11   &&Map_1[bx-l][by]<=20)   ;l++)
					{
						Map_1[bx-l][by]=51;
					}
					for (l=0;(l<5) &&!(Map_1[bx][by-l]>=11  &&Map_1[bx][by-l]<=20)   ;l++)
					{
						Map_1[bx][by-l]=51;
					}									
					for (k=31;k<=130;k++)
					{
						if(   Map_1[mover[k].nx][mover[k].ny]==51 )
						{	
							Map_1[mover[k].nx][mover[k].ny]=0;
							mover[k].tp=0;
							mover[k].nx=0;
							mover[k].ny=0;
							mover[k].ox=0;
							mover[k].oy=0;
						}
					}
					
					w[i].x=0;
					w[i].y=0;
					w[i].tp=0;
				}
				w[i].e=1;
			}
		}
		TimeControl(2,i);
	}		
	
	
} ;
void CleanArray()
{
	memset(Map_1,0,sizeof(Map_1));    //�M�Ű}�C Map_1
	memset(BreakWall,0,sizeof(BreakWall));
	memset(Map_2,0,sizeof(Map_2)); 
	memset(levelkey,0,sizeof(levelkey)); 
	for (i=0;i<=130;i++)				  //�M�ŵ��c�}�Cmover[40] 
	{
		memset(&mover[i],0,sizeof(struct mover)); 
	} 
	for (i=0;i<=20;i++)	   //�M�ŵ��c�}�Cw[20] 
	{
		memset(&w[i],0,sizeof(struct weapon)); 
	}     
};

void EndBreak()
{	

	if(hp<=0)             //�Yhp==0�A�h���X�j��  
	{
		Level=0;
		MessageBox(NULL,"�w�L�ͩR��","����",MB_OK);
	}	
		
	for(i=31,nmn=0;i<=130;i++)   //�Y�����Ҧ��Ǫ��A�h���X�j��  
	{ 
		if( mover[i].tp==1)
		{
			
			nmn++;
		}
	}
	if( nmn==mn)
	{
		MessageBox(NULL,"�Ǫ��F��W���A�A��F","����",MB_OK);
		Level=0;
	}
	
	if (GetAsyncKeyState(17)<0 && GetAsyncKeyState(27)<0)   //�� ctrl + esc���� 
	{
		Level=1; 
	}
	if (GetAsyncKeyState(192)<0)   //��~ 
	{
		MessageBox(NULL,"�b endbreak ���F~","����",MB_OK);
		printf("�b endbreak ���F~");
		getch();
		system("pause");
	}
	
	while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0)
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
};
void MoverBehavior()
{
	
	for (i=31;i<=130;i++)  //���~ 
	{
		mover[i].e=0;
		if(mover[i].sl==0 && mover[i].tp==1)//���Q���~��� 
		{
			if (i>=31)      //���a�Y�Q���~�I�� 
			{
				
				if(mover[21].nx==mover[i].nx && mover[21].ny==mover[i].ny)
				{
					hp--;	
				}
			}
			NextStep(i);
			BoundaryCondition(i);
			WalkingState(i);
		}
		if(mover[i].tp==1)
		{
			Map_1[mover[i].ox][mover[i].oy]=0;   //���~�¦�m=�D�� 
			Map_1[mover[i].nx][mover[i].ny]=31;   //���~�s��m=�ۤv���� 
		}
		TimeControl(1,i);
		while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0)
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
		i=21;
		mover[i].e=0;
		if(mover[i].sl==0) //���Q���~��� 
		{
			NextStep(i);
			BoundaryCondition(i);	
			if (mover[i].d!=0)   //�}�B�n 
			{
				WalkingState(21);
				Sound(3) ;
			}
			
		}
		
		Map_1[mover[i].ox][mover[i].oy]=0;   //���a�¦�m=�D�� 
		Map_1[mover[i].nx][mover[i].ny]=i;   //���a�s��m=�ۤv���� 
		TimeControl(1,i);
		
		
	
	
	
	
	while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0)
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
};
;
void Output(int Output_k );
void MousePoint(int mousepoint_k)
{
	if(mousepoint_k==1)  //�ƹ���m�P�C�� 
	{
		GetCursorPos(&mouse1);   
		mouse2.x=mouse1.x*1920/client_sw;  //�ഫ�ƹ��@�� 	mouse1�GClient���y�� --> mouse2�Gdc3�W���y�� 
		mouse2.y=mouse1.y*1080/client_sh;
		colorref=GetPixel(dc3,mouse2.x,mouse2.y);  //�ƹ���m���C�� 
		bRed=GetRValue(colorref);
		bGreen=GetGValue(colorref);
		bBlue=GetBValue(colorref);
		Output(4);
	
	}
	
	if (mousepoint_k==1)  //�Z�����s 
	{
		//�Z�����s 
		TransparentBlt(dc3,1520,504,84,84,p[13].dc,0,0,p[13].width,p[13].height,RGB(255,255,255));// �}�� 
		TransparentBlt(dc3,1520,415,84,79,p[54].dc,0,0,p[54].width,p[54].height,RGB(255,255,255)); // bomp
		

		
		
		if ( mouse2.x>=1520 && mouse2.x<=1604 && mouse2.y>=504 && mouse2.y<=588)    //�Z�����s   �}�� 
		{
			TransparentBlt(dc3,1520,504,84,84,p[14].dc,0,0,p[14].width,p[14].height,RGB(255,255,255));
			if(GetAsyncKeyState(1)<0)
			{
				weapontype=2;
			}
			
		}
		else	if ( mouse2.x>=1520 && mouse2.x<=1604 && mouse2.y>=415 && mouse2.y<=494)    //�Z�����s   bomp
		{
			TransparentBlt(dc3,1520,415,84,79,p[55].dc,0,0,p[55].width,p[55].height,RGB(255,255,255)); // bomp
			if(GetAsyncKeyState(1)<0)
			{
				weapontype=1;
			}
		}
	}
	
	if (mousepoint_k==2)  //��m�Z��
	{
		 //�ഫ�ƹ��@�� 	  mouse2�Gdc3�W���y�� -->  mouse3�Gmap����m 		
		if(   ( sy*pixel-   (mover[21].ny+viewmove_y)*pixel  )  <740)
			mouse3.x=(   sy*pixel-1480  + mouse2.x)/pixel; 
		else if(   (mover[21].ny+viewmove_y)*pixel <740  )
			mouse3.x= mouse2.x/pixel;	
		else	
			mouse3.x=(   (mover[21].ny)*pixel -740 + mouse2.x)/pixel; //
		
		
		if((sx*pixel-   (mover[21].nx+viewmove_x)*pixel)    <540)
			mouse3.y=(	 sx*pixel-1080 + mouse2.y)/pixel;
		else if(   (mover[21].nx+viewmove_x)*pixel <540  )
			mouse3.y= mouse2.y/pixel;	
		else
			mouse3.y=(	 (mover[21].nx)*pixel -540 + mouse2.y)/pixel; //
			
			
			
		if (mouse3.x>=(mover[21].ny-2 -viewmove_y) && mouse3.x<=(mover[21].ny+2-viewmove_y) && mouse3.y>=(mover[21].nx-2-viewmove_x) && mouse3.y<=(mover[21].nx+2-viewmove_x))
		 {
			mouse3.x+=viewmove_y;
			mouse3.y+=viewmove_x;
			
			
			if (weapontype==1)	//���u
			{
				 //��ܬ��u�i��m����m 
				TransparentBlt(dc4, (mouse3.x)*pixel , (mouse3.y)*pixel ,pixel,pixel,p[54].dc,0,0,p[54].width,p[54].height,RGB(255,255,255));
				//TransparentBlt(dc3,1520,415,84,79,p[54].dc,0,0,p[54].width,p[54].height,RGB(255,255,255)); // bomp
				
				int thesame=0;	                     //���񬵼u
				if(GetAsyncKeyState(1)<0)
				{	        
					for(k=2;w[k].tp==1&&k>=bn;k++)
					{
						if (k>bn)
							thesame=1;	
					}
					for(l=2;l<=bn;l++)
					{
						if(mover[21].nx==w[l].x  &&  mover[21].ny==w[l].y)
						{
							thesame=1;
						}			
					}
					if(thesame==0)
					{
						
						w[k].x=mouse3.y;
						w[k].y=mouse3.x; 
						w[k].tp=1;
						w[k].tm=5;	
					}
					weapontype=0; 
				} 
			}
		 	
		 	else if (weapontype==2)	// �}��
			{
			
				if (Map_1[mouse3.y][mouse3.x]<31 || Map_1[mouse3.y][mouse3.x]>40) 
				 {
				 	TransparentBlt(dc4, (mouse3.x)*pixel , (mouse3.y)*pixel  ,pixel,pixel,p[13].dc,0,0,p[13].width,p[13].height,RGB(255,255,255));
					if(GetAsyncKeyState(1)<0)
					{
						//mouse3.x+=viewmove_y;
						//mouse3.y+=viewmove_x;
						BreakWall[mouse3.y][mouse3.x]=1;	
						Map_1[mouse3.y][mouse3.x]=10;
						weapontype=0;
					}
				 }
				
				
			}
		 
		 }
	}
	
	
	
	if (mousepoint_k==3)   //��ɲ��� 
	{	

		TransparentBlt(dc3,5,440,30,200,p[71].dc,0,0,p[71].width,p[71].height,RGB(255,255,255)); //��ɲ��� �� 
		TransparentBlt(dc3,640,5,200,30,p[72].dc,0,0,p[72].width,p[72].height,RGB(255,255,255)); //��ɲ��� �W 
		TransparentBlt(dc3,1445,440,30,200,p[73].dc,0,0,p[73].width,p[73].height,RGB(255,255,255)); //��ɲ��� �k 
		TransparentBlt(dc3,640,1045,200,30,p[74].dc,0,0,p[74].width,p[74].height,RGB(255,255,255)); //��ɲ��� �U 
		
		mover[1].e=0;
		mover[1].st=0.1;	
		if(mover[1].sl==0)//���Q���~���                     
		{
			//�ƹ��I����s������ 
			if ((mover[21].ny+viewmove_y)*pixel >=740   && mouse2.x>=5 && mouse2.x<=35 && mouse2.y>=440 && mouse2.y<=640)    //���� 
			{
				TransparentBlt(dc3,5,440,30,200,p[75].dc,0,0,p[75].width,p[75].height,RGB(255,255,255));  
				if(GetAsyncKeyState(1)<0)
				{
					mover[1].e=1;
					viewmove_y-=1;
				} 
				
			}
			if ((mover[21].nx+viewmove_x)*pixel >=540 && mouse2.x>=640 && mouse2.x<=840 && mouse2.y>=5 && mouse2.y<=35)    //���W
			{
				TransparentBlt(dc3,640,5,200,30,p[76].dc,0,0,p[76].width,p[76].height,RGB(255,255,255));  
				if(GetAsyncKeyState(1)<0)
				{
					mover[1].e=1;
					viewmove_x-=1;
				} 
			}
			if ( ( sy*pixel-   (mover[21].ny+viewmove_y)*pixel  )  >=740	 && mouse2.x>=1445 && mouse2.x<=1475 && mouse2.y>=440 && mouse2.y<=640)//���k 
			{ 
				TransparentBlt(dc3,1445,440,30,200,p[77].dc,0,0,p[77].width,p[77].height,RGB(255,255,255));  
				if(GetAsyncKeyState(1)<0)
				{
					mover[1].e=1;
					viewmove_y+=1;
				} 
			}
			if (sx*pixel-   (mover[21].nx+viewmove_x)*pixel    >=540 && mouse2.x>=640 && mouse2.x<=840 && mouse2.y>=1045 && mouse2.y<=1075) //���U 
			{
				TransparentBlt(dc3,640,1045,200,30,p[78].dc,0,0,p[78].width,p[78].height,RGB(255,255,255));  
				if(GetAsyncKeyState(1)<0)
				{
					mover[1].e=1;
					viewmove_x+=1;
				} 
			}
		}
		TimeControl(1,1);
	}
	
	
		

	
	while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0)
 	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
};


void Output(int Output_k )
{
	if (Output_k==0)  //�ŧi 
	{	
		
	
		p[0]=readBMP("pic//�a�O.bmp", dc1);  
		p[11]=readBMP("pic//���.bmp", dc1);
		p[13]=readBMP("pic//�}��.bmp", dc1);
		p[14]=readBMP("pic//�}��(��).bmp", dc1);
		p[19]=readBMP("pic//�p�C���ಾ�I1.bmp", dc1);
		p[20]=readBMP("pic//�Ǫ����;�.bmp", dc1);
		for(i=21;i<=32;i++)   //���a
		{
			sprintf(display,"pic//%d.bmp",i);
			p[i]=readBMP(display, dc1);	
		}
		p[30]=readBMP("pic//���a�p��.bmp", dc1);
		p[131]=readBMP("pic//�Ǫ��������B.bmp", dc1);
		p[40]=readBMP("pic//�Ǫ��p��.bmp", dc1);
		p[50]=readBMP("pic//�d��.bmp", dc1);
		p[51]=readBMP("pic//�z��.bmp", dc1);
		p[53]=readBMP("pic//�z��(��).bmp", dc1);
		p[54]=readBMP("pic//���u.bmp", dc1);
		p[55]=readBMP("pic//���u(��).bmp", dc1);
		p[52]=readBMP("pic//�I��1.bmp", dc1); 
		p[71]=readBMP("pic//��ɥ�.bmp", dc1); 
		p[72]=readBMP("pic//��ɤW.bmp", dc1); 
		p[73]=readBMP("pic//��ɥk.bmp", dc1); 
		p[74]=readBMP("pic//��ɤU.bmp", dc1); 
		p[75]=readBMP("pic//��ɥ�(��).bmp", dc1); 
		p[76]=readBMP("pic//��ɤW(��).bmp", dc1); 
		p[77]=readBMP("pic//��ɥk(��).bmp", dc1); 
		p[78]=readBMP("pic//��ɤU(��).bmp", dc1);
		p[80]=readBMP("pic//��r����.bmp", dc1);
		p[81]=readBMP("pic//�©�.bmp", dc1);
		p[82]=readBMP("pic//�u��C2.bmp", dc1);
		p[91]=readBMP("pic//���~�@.bmp", dc1);
		p[92]=readBMP("pic//���~�G.bmp", dc1);
		p[93]=readBMP("pic//���~�T.bmp", dc1);
		p[94]=readBMP("pic//�ʦL2.bmp", dc1);
		p[101]=readBMP("pic//A6.bmp", dc1);  
		p[102]=readBMP("pic//A6�}.bmp", dc1);  
		p[103]=readBMP("pic//���I��.bmp", dc1);  
		p[104]=readBMP("pic//�����s.bmp", dc1);  
		p[105]=readBMP("pic//�����s(��).bmp", dc1);
		for(i=121;i<=132;i++)   //�Ǫ�
		{
			sprintf(display,"pic//%d.bmp",i);
			p[i]=readBMP(display, dc1);	
		}
	}

	if (Output_k==1)
	{
		StretchBlt(dc4,0,0,sy*pixel*5,sx*pixel*5,p[81].dc,0,0,1480,1080,SRCCOPY);  //dc4 ���K�©� 
		StretchBlt(dc3,1481,0,440,1080,p[82].dc,0,0,p[82].width,p[82].height,SRCCOPY);
	
	
		//�_�� 
		if(levelkey[1]	==1)
			TransparentBlt(dc3,1813,412,94,52,p[91].dc,0,0,p[91].width,p[91].height,RGB(255,255,255));
		if(levelkey[2]	==1)
			TransparentBlt(dc3,1813,471,94,52,p[92].dc,0,0,p[92].width,p[92].height,RGB(255,255,255));
		if(levelkey[3]	==1)
			TransparentBlt(dc3,1813,523,94,52,p[93].dc,0,0,p[93].width,p[93].height,RGB(255,255,255));
		if(levelkey[4]	==1)
			{
				TransparentBlt(dc3,1813,579,94,52,p[94].dc,0,0,p[94].width,p[94].height,RGB(255,255,255));
				SelectObject(dc3,font1);  
				sprintf(display,"�w�ʦL���~�J�f");  //���a��m 
				TextOut(dc3,1508,736,display,strlen(display));
			}


		InitialSetting(2);  //�H�U�j�r 

	
		//�H�U�p�r 
		SelectObject(dc3,font2);  //dc2�e�����r�鳣�ܬ�font1 	    
		sprintf(display,"|   %2d,%2d",mover[21].nx,mover[21].ny);  //���a��m 
		TextOut(dc3,1620,1045,display,strlen(display));
		
		sprintf(display,"Level:%d From:%d",Level,fromlevel);  //Level
		TextOut(dc3,1496,17,display,strlen(display));
		
		
		//�H�U�W�p�r      �ͩR�� ���~�ƶq 
		SelectObject(dc3,font3);  //dc2�e�����r�鳣�ܬ�font1 
		sprintf(display,"Life  Monster",hp,nmn);
		TextOut(dc3,1663,383,display,strlen(display));
		sprintf(display,"%3d    %3d",hp,nmn);
		TextOut(dc3,1668,402,display,strlen(display));
		
		for(i=31,nmn=0;i<=130;i++)   //�Ǫ��ƶq  
		{ 
			if( mover[i].tp==1)
				nmn++;	
		}
		if( nmn==mn)
		{
			MessageBox(NULL,"�Ǫ��F��W���A�A��F","����",MB_OK);
			Level=0;
		}
	

		fi=(hp* 287/999 )  ; //���� 
		fj=(nmn * 287/mn );  
		i = round(fi);
		j = round(fj);
		StretchBlt(dc3,1657,711-i,58,i,p[30].dc,0,0,p[30].width,p[30].height,SRCCOPY);  
		StretchBlt(dc3,1728,711-j,58,j,p[40].dc,0,0,p[40].width,p[40].height,SRCCOPY);  

		
		
	}
		
	
	else if (Output_k==2)   //�p��   //�j�� 
	{
		for(i=0;i<sx ;i+=1)   //�p��
		{
			for(j=0;j<sy;j+=1)
			{
				if (Map_2[i][j]>=0&&Map_2[i][j]<=4)    
					TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[0].dc,0,0,p[0].width,p[0].height,RGB(255,255,255));
				else if ((Map_2[i][j]<=20)&&(Map_2[i][j]>=11))
					TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[11].dc,0,0,p[11].width,p[11].height,RGB(255,255,255));
				 if (Map_1[i][j]==21)
					TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[30].dc,0,0,p[30].width,p[30].height,RGB(255,255,255));
				else if ((Map_1[i][j]<=40)&&(Map_1[i][j]>=31))
					TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[40].dc,0,0,p[40].width,p[40].height,RGB(255,255,255));
				if (BreakWall[i][j]==1)     //�}��
					TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[13].dc,0,0,p[13].width,p[13].height,RGB(255,255,255));
			}
		}
		
		//���d�K�� 
		TransparentBlt(dc4,((sy+1)/2 -1)*pixel,((sx+1 )/2-1)*pixel,pixel*3,pixel*3,p[19].dc,0,0,p[19].width,p[19].height,RGB(255,255,255));
		TransparentBlt(dc4,2*pixel,(sx-5)*pixel,pixel*3,pixel*3,p[19].dc,0,0,p[19].width,p[19].height,RGB(255,255,255));
		TransparentBlt(dc4,(sy-5)*pixel,(sx-5)*pixel,pixel*3,pixel*3,p[19].dc,0,0,p[19].width,p[19].height,RGB(255,255,255));
		TransparentBlt(dc4,(sy-5)*pixel,2*pixel,pixel*3,pixel*3,p[19].dc,0,0,p[19].width,p[19].height,RGB(255,255,255));
		//TransparentBlt(dc4,((sy+1)/2 -1)*pixel,2*pixel,pixel*3,pixel*3,p[19].dc,0,0,p[19].width,p[19].height,RGB(255,255,255));
		TransparentBlt(dc4,2*pixel,2*pixel,pixel*3,pixel*3,p[19].dc,0,0,p[19].width,p[19].height,RGB(255,255,255));
		//TransparentBlt(dc4,2*pixel,(sx-5)*pixel,pixel*3,pixel*3,p[20].dc,0,0,p[20].width,p[20].height,RGB(255,255,255));
		
		StretchBlt(dc3,1536,98,326,239,dc4,0,0,sy*pixel,sx*pixel,SRCCOPY);   //�p��  dc4  ���  dc3 
		
		 
		for(i=0;i<sx ;i+=1)  //�j�� 
		{
			for(j=0;j<sy;j+=1)
			{
			   if (Map_1[i][j]==21)
				{
					TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[0].dc,0,0,p[0].width,p[0].height,RGB(255,255,255));
					if (BreakWall[i][j]==1)//�}��
						TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[13].dc,0,0,p[13].width,p[13].height,RGB(255,255,255));
					TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[mover[21].ws].dc,0,0,p[mover[21].ws].width,p[mover[21].ws].height,RGB(255,255,255));
				}
				else if ((Map_1[i][j]<=40)&&(Map_1[i][j]>=31)) //�Ǫ�
				{
					for(l=31;l<=130;l++)   
					{
						if(mover[l].nx==i && mover[l].ny==j)
						{
							TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[0].dc,0,0,p[0].width,p[0].height,RGB(255,255,255));
							TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[mover[l].ws+100].dc,0,0,p[mover[l].ws+100].width,p[mover[l].ws+100].height,RGB(255,255,255));
						}
						
					}
				}
				else if(Map_1[i][j]==51)
					TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[51].dc,0,0,p[51].width,p[51].height,RGB(255,255,255));
				else if(Map_1[i][j]>=52 && Map_1[i][j]<=56)
					TransparentBlt(dc4,j*pixel,i*pixel,pixel,pixel,p[54].dc,0,0,p[54].width,p[54].height,RGB(255,255,255));
					
			}
		}
	}
	
	else if (Output_k==3)  //�����H�۪��a����   //�����d��  // �Z�����s 
	{
		StretchBlt(dc3,0,0,1480,1080,p[81].dc,0,0,1480,1080,SRCCOPY);//dc3 ���K�©� 
		//�����d�� 
		TransparentBlt(dc4,(mover[21].ny-2)*pixel,(mover[21].nx-2)*pixel,pixel*5,pixel*5,p[50].dc,0,0,p[50].width,p[50].height,RGB(255,255,255));
		
		
		//�����H�۪��a���� 
		i=(mover[21].ny+viewmove_y)*pixel -740;
		
	    if(   ( sy*pixel-   (mover[21].ny+viewmove_y)*pixel  )  <740)
		{
			i=sy*pixel-1480;
		}
		
		if(   (mover[21].ny+viewmove_y)*pixel <740  )
		{
			i=0;
		}
		
		j=(mover[21].nx+viewmove_x)*pixel -540;
		
		if(sx*pixel-   (mover[21].nx+viewmove_x)*pixel    <540)
		{
			j=sx*pixel-1080;
		}
		if(   (mover[21].nx+viewmove_x)*pixel <540  )
		{
			j=0;
		}
		
		MousePoint(1);	// �Z�����s 
		MousePoint(2);	//��m�Z��
		
	}
		
	if (Output_k==4)  //�ƹ���m 
	{
	
		SelectObject(dc3,font2); 		
		sprintf(display,"|%4d,%4d",mouse2.x,mouse2.y);
		TextOut(dc3,1768,1045,display,strlen(display));
		
		//sprintf(display,"�C��(%3d,%3d,%3d)",bRed,bGreen,bBlue);
		//TextOut(dc3,1508,936,display,strlen(display));
	}
	
		
	else if (Output_k==43)
	{
		BitBlt(dc3,0,0,1480,1080,dc4,i,j,SRCCOPY);  //�j�� dc4  ���  dc3  
		
		MousePoint(3);	//��ɲ��� 
		
		StretchBlt(dc1,0,0,client_sw,client_sh,dc3,0,0,sw,sh,SRCCOPY);  //��i��  dc3  ���  dc1 
	}
		
		
	
	
	while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0)
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

void Window_Picture()
{
	reg_class( "classname" );
	HWND hwnd = CreateWindow( "classname", "��11��-��r��", WS_POPUP|WS_MAXIMIZE | WS_VISIBLE, 0, 0, sw, sh, NULL, NULL, NULL, NULL );	
	
	dc1=GetDC(hwnd);    //�K�Ϩ�dc1�A�N�|��ܦb�����W(hwnd�֦������v������)
		
	hbm=CreateCompatibleBitmap(dc1,sw,sh) ;
	dc2=CreateCompatibleDC(dc1);
	
	hbm3=CreateCompatibleBitmap(dc1,sw,sh) ;
	dc3=CreateCompatibleDC(dc1);
	
	hbm4=CreateCompatibleBitmap(dc1,sy*pixel*5,sx*pixel*5) ;
	dc4=CreateCompatibleDC(dc1);
	
	SelectObject(dc2,hbm);//�V�t��hbm���Udc2���ϥ��v(���G�@��hbm�u����U�@��dc2)
	SelectObject(dc3,hbm3);
	SelectObject(dc4,hbm4);
	
	RECT window_rect,client_rect;
	GetWindowRect( hwnd, &window_rect );
	GetClientRect( hwnd, &client_rect );
	
	char information[100];	
	client_sw= client_rect.right-client_rect.left;
	client_sh= client_rect.bottom-window_rect.top;
	sprintf( information,"client �e��=%d\nclient ����=%d",client_sw,client_sh );
	//MessageBox( NULL, information, "client ��T", MB_OK );
}

void MonsterGenrator()
{
	mover[0].st=10;
	mover[0].e=0;
	if(mover[0].sl==0 && levelkey[4]==0)        //MonsterGenratorLock==0
	{	        
		for(i=31;mover[i].tp==1 && i<=31+mn ;i++)
		;
		if (i!=31+mn)
		{
			do{
				mover[i].nx=rand()%sx; 
				mover[i].ny=rand()%sy ;
			}while(Map_1[mover[i].nx][mover[i].ny]>=11 && Map_1[mover[i].nx][mover[i].ny]<=20);
			
			mover[i].tp=1;
			Map_1[mover[i].nx][mover[i].ny]=31; 
			mover[i].st=spacetime;
			mover[i].d=37+rand()%4;
			mover[0].e=1;
		}
		
	} 
	TimeControl(3,0);
};


void Level_A()
{	
	if( levelever[Level]==0 )
	{
		CleanArray();
		InitialSetting(1);
		InitialSetting(3);
		
	}	
	while(Level==10)                                
	{	
		
		Sound(1);	  //����I������  
		
		Output(1);	  //�g�c 
		
		GetKey();	
		Editor();
		MoverBehavior();	
		Arms();
		MonsterGenrator();
		
		
		Output(2);	  //�p�� ��j ��
		
		Output(3);	 //��L 
		GameClock(2);
		

		Output(43);
		EndBreak();					 
	}
	
}

#include "Header\\A1.h"
#include "Header\\A2.h"
#include "Header\\A3.h"
#include "Header\\A4.h"

#include "Header\\B2.h"
#include "Header\\B3.h"

#include "Header\\C2.h"
#include "Header\\C3.h"

#include "Header\\D.h"

int Level_1()
{
	if (greetlist[5]==1)
	{
		Sound(2); 
		Sound(10); 
	}
	else
		Sound(1);  //�����1�� 
		
	InitialSetting(2);  //�H�U�j�r 

	while(Level==1)
	{
		Editor();

		
		StretchBlt(dc3,0,0,1920,1080,p[103].dc,0,0,p[103].width,p[103].height,SRCCOPY);  //�I��
		
		StretchBlt(dc3,350,100,420,100,p[104].dc,0,0,p[104].width,p[104].height,SRCCOPY);  
		StretchBlt(dc3,350,300,420,100,p[104].dc,0,0,p[104].width,p[104].height,SRCCOPY);  
		StretchBlt(dc3,350,500,420,100,p[104].dc,0,0,p[104].width,p[104].height,SRCCOPY);  
		StretchBlt(dc3,350,700,420,100,p[104].dc,0,0,p[104].width,p[104].height,SRCCOPY);  

		
		GetCursorPos(&mouse1);   
		mouse2.x=mouse1.x*1920/client_sw;  //�ഫ�ƹ��@�� 	mouse1�GClient���y�� --> mouse2�Gdc3�W���y�� 
		mouse2.y=mouse1.y*1080/client_sh;
		
		
		if ( mouse2.x>=350 && mouse2.x<=770 && mouse2.y>=100 && mouse2.y<=200)    //�ƹ����W   //�s�C��
		{
			StretchBlt(dc3,350,100,420,100,p[105].dc,0,0,p[105].width,p[105].height,SRCCOPY);    //�ƹ��I�� 
			if(GetAsyncKeyState(1)<0)
			{
				Level=10;
				
				Sound(5); 
			}
		}
		
		if ( mouse2.x>=350 && mouse2.x<=770 && mouse2.y>=300 && mouse2.y<=400)      //���} 
		{
			StretchBlt(dc3,350,300,420,100,p[105].dc,0,0,p[105].width,p[105].height,SRCCOPY);    
			if(GetAsyncKeyState(1)<0)
			{
				Level=0;
				greetlist[2]=1; 
				Sound(5); 
			}
		}
		
		if ( mouse2.x>=350 && mouse2.x<=770 && mouse2.y>=500 && mouse2.y<=600)       //��������
		{
			StretchBlt(dc3,350,500,420,100,p[105].dc,0,0,p[105].width,p[105].height,SRCCOPY);    
			if(GetAsyncKeyState(1)<0)
			{
				greetlist[3]=1; 
				Sound(5); 
			}
		}
		
		while (greetlist[3]==1)   //�������� 
		{
			GetCursorPos(&mouse1);   
			mouse2.x=mouse1.x*1920/client_sw;  //�ഫ�ƹ��@�� 	mouse1�GClient���y�� --> mouse2�Gdc3�W���y�� 
			mouse2.y=mouse1.y*1080/client_sh;
			
			StretchBlt(dc3,350,100,1220,880,p[81].dc,0,0,p[81].width,p[81].height,SRCCOPY);  //�©� 
			StretchBlt(dc3,1510,100,60,60,p[30].dc,0,0,p[30].width,p[30].height,SRCCOPY);    //�e�e 
			
			if ( mouse2.x>=1510 && mouse2.x<=1570 && mouse2.y>=100 && mouse2.y<=160)       //������������
			{
				StretchBlt(dc3,1510,100,60,60,p[40].dc,0,0,p[40].width,p[40].height,SRCCOPY); //�Ťe�e 
				if(GetAsyncKeyState(1)<0)
				{
					greetlist[3]=0; 
					Sound(5); 
				}
			}
			
			
			sprintf(display,"�C���L�{���i�� ctrl + esc ����");  //1�� 56�ӼƦr  31�Ӥ��� 
			TextOut(dc3,360,110,display,strlen(display));
			sprintf(display,"�D�a�Ϥ��ϥ� WASD �� �W�U���k ����a���� ");
			TextOut(dc3,360,160,display,strlen(display));
			sprintf(display,"�I��k���u��C�����u�ί}��u��A�A�I�����a�P��A�i����");
			TextOut(dc3,360,210,display,strlen(display));
			sprintf(display,"��ctrl + Shift + Alt �i�J�޲z���Ҧ��A�i�ֳt���h");
			TextOut(dc3,360,260,display,strlen(display));
			sprintf(display,"�o��3���_�ͥB�N���~�����M�ťi�i�J�U�@�h�D�a��");
			TextOut(dc3,360,310,display,strlen(display));
			sprintf(display,"�o��Ǫ��J�f���ʦL�ťH������~���_�W�[");
			TextOut(dc3,360,360,display,strlen(display));
			sprintf(display,"���~�ƶq�W�L����ΥͩR�Ȥ֩�0�Y��");
			TextOut(dc3,360,410,display,strlen(display));
			sprintf(display,"");
			TextOut(dc3,360,460,display,strlen(display));
			
			StretchBlt(dc1,0,0,client_sw,client_sh,dc3,0,0,sw,sh,SRCCOPY);  //��i��  dc3  ���  dc1
			
			
			
			
			
			if (GetAsyncKeyState(17)<0 && GetAsyncKeyState(27)<0)   //�� ctrl + esc���� 
			greetlist[3]=0; 
			
			while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0){
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		
		
		
		
		if ( mouse2.x>=350 && mouse2.x<=770 && mouse2.y>=700 && mouse2.y<=800)       //�Ʀ�]
		{
			StretchBlt(dc3,350,700,420,100,p[105].dc,0,0,p[105].width,p[105].height,SRCCOPY);    
			if(GetAsyncKeyState(1)<0)
			{
				greetlist[4]=1; 
				Sound(5); 
			}
		}
		
		
		
		while (greetlist[4]==1)   //�Ʀ�]
		{
			GetCursorPos(&mouse1);   
			mouse2.x=mouse1.x*1920/client_sw;  //�ഫ�ƹ��@�� 	mouse1�GClient���y�� --> mouse2�Gdc3�W���y�� 
			mouse2.y=mouse1.y*1080/client_sh;
			
			StretchBlt(dc3,350,100,1220,880,p[81].dc,0,0,p[81].width,p[81].height,SRCCOPY);  //�©� 
			StretchBlt(dc3,1510,100,60,60,p[30].dc,0,0,p[30].width,p[30].height,SRCCOPY);    //�e�e 
			
			if ( mouse2.x>=1510 && mouse2.x<=1570 && mouse2.y>=100 && mouse2.y<=160)       //������������
			{
				StretchBlt(dc3,1510,100,60,60,p[40].dc,0,0,p[40].width,p[40].height,SRCCOPY); //�Ťe�e 
				if(GetAsyncKeyState(1)<0)
				{
					greetlist[4]=0; 
					Sound(5); 
				}
			}
			
			
			sprintf(display,"1.");  //1�� 56�ӼƦr  31�Ӥ��� 
			TextOut(dc3,360,110,display,strlen(display));
			sprintf(display,"2. ");
			TextOut(dc3,360,160,display,strlen(display));
			sprintf(display,"3.");
			TextOut(dc3,360,210,display,strlen(display));
			sprintf(display,"4.");
			TextOut(dc3,360,260,display,strlen(display));
			sprintf(display,"5.");
			TextOut(dc3,360,310,display,strlen(display));
			sprintf(display,"6.");
			TextOut(dc3,360,360,display,strlen(display));
			sprintf(display,"7.");
			TextOut(dc3,360,410,display,strlen(display));
			sprintf(display,"8.");
			TextOut(dc3,360,460,display,strlen(display));
			sprintf(display,"9.");
			TextOut(dc3,360,510,display,strlen(display));
			sprintf(display,"10.");
			TextOut(dc3,360,560,display,strlen(display));
			
			StretchBlt(dc1,0,0,client_sw,client_sh,dc3,0,0,sw,sh,SRCCOPY);  //��i��  dc3  ���  dc1
			
			while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0){
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		
		
		while (greetlist[5]==1)   //�z�Ҫ᪺�ɶ� 
		{
		
			GetCursorPos(&mouse1);   
			mouse2.x=mouse1.x*1920/client_sw;  //�ഫ�ƹ��@�� 	mouse1�GClient���y�� --> mouse2�Gdc3�W���y�� 
			mouse2.y=mouse1.y*1080/client_sh;
			
			StretchBlt(dc3,350,100,1220,880,p[81].dc,0,0,p[81].width,p[81].height,SRCCOPY);  //�©� 
			StretchBlt(dc3,1510,100,60,60,p[30].dc,0,0,p[30].width,p[30].height,SRCCOPY);    //�e�e 
			
			if ( mouse2.x>=1510 && mouse2.x<=1570 && mouse2.y>=100 && mouse2.y<=160)       //������������
			{
				StretchBlt(dc3,1510,100,60,60,p[40].dc,0,0,p[40].width,p[40].height,SRCCOPY); //�Ťe�e 
				if(GetAsyncKeyState(1)<0)
				{
					greetlist[5]=0; 
					Sound(5); 
				}
			}
			
			
			sprintf(display,"�z�Ҫ᪺�ɶ�:%d:%2d:%2d",sec/3600%60,sec/60%60,sec%60);  //1�� 56�ӼƦr  31�Ӥ��� 
			TextOut(dc3,360,110,display,strlen(display));
			
			StretchBlt(dc1,0,0,client_sw,client_sh,dc3,0,0,sw,sh,SRCCOPY);  //��i��  dc3  ���  dc1
		
			
			if (GetAsyncKeyState(17)<0 && GetAsyncKeyState(27)<0)   //�� ctrl + esc���� 
				greetlist[4]=0; 
			
			while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0){
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		} 
		
		
		sprintf(display,"     �s�C��");
		TextOut(dc3,400,130,display,strlen(display));
		sprintf(display,"      ���}");
		TextOut(dc3,400,330,display,strlen(display));
		sprintf(display,"    �C������");
		TextOut(dc3,400,530,display,strlen(display));
		sprintf(display,"     �Ʀ�]");
		TextOut(dc3,400,730,display,strlen(display));
		
		if (GetAsyncKeyState(17)<0 && GetAsyncKeyState(27)<0)   //�� ctrl + esc���� 
			Level=0; 
		
		StretchBlt(dc1,0,0,client_sw,client_sh,dc3,0,0,sw,sh,SRCCOPY);  //��i��  dc3  ���  dc1
		
		while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0)
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}	
	}
	Sound(20);  //�����2�� 		
	 
};

int main()
{
	Window_Picture();
	Sound(0);	//�ŧi���� 
	Output(0);	 //�ŧi�Ϥ� 
	InitialSetting(1);
	InitialSetting(2); 
	Level=1;	
				
	while(Level!=0)                                
	{
		StretchBlt(dc1,0,0,1920,1080,p[81].dc,0,0,1480,1080,SRCCOPY);  //dc1 ���K�©�
		StretchBlt(dc2,0,0,1920,1080,p[81].dc,0,0,1480,1080,SRCCOPY);  //dc2 ���K�©�
		switch (Level)
		{
			case 1: 	//�}�l�e�� 
				Level_1();	
				fromlevel=1;
				break;
			case 10: 	
				Level_A(); 
				fromlevel=10;
				break;
			case 11: 
				Level_A1(); 
				fromlevel=11;
				break;
			case 12: 
				Level_A2(); 
				fromlevel=12;
				break;
			case 13: 
				Level_A3(); 
				fromlevel=13;
				break;
			case 14: 
				Level_A4(); 
				fromlevel=14;
				break;
			case 15: 
				Level_A5(); 
				fromlevel=15;
				break;
			case 16: 
				Level_A6(); 
				fromlevel=16;
				break;
			case 20: 
				Level_B(); 
				fromlevel=20;
				break;
			case 21: 
				Level_B1(); 
				fromlevel=21;
				break;
			case 22: 
				Level_B2(); 
				fromlevel=22;
				break;
			case 23: 
				Level_B3(); 
				fromlevel=23;
				break;
			case 24: 
				Level_B4(); 
				fromlevel=24;
				break;
			case 25: 
				Level_B5(); 
				fromlevel=25;
				break;
			case 26: 
				Level_B6(); 
				fromlevel=26;
				break;
			case 30: 
				Level_C(); 
				fromlevel=30;
				break;
			case 31: 
				Level_C1(); 
				fromlevel=31;
				break;
			case 32: 
				Level_C2(); 
				fromlevel=32;
				break;
			case 33: 
				Level_C3(); 
				fromlevel=33;
				break;
			case 34: 
				Level_C4(); 
				fromlevel=34;
				break;
			case 35: 
				Level_C5(); 
				fromlevel=35;
				break;
			case 36: 
				Level_C6(); 
				fromlevel=36;
				break;
			case 40:   //�A������ 
				greetlist[5]=1;		
				Level=1;
				Level_1();		
				break;			
			default: 
				break;
		};	
	} 
	
	Sound(999);  //�������� 
	
	//�N�귽�ٵ��t��
	ReleaseDC(hwnd,dc1);
	DeleteDC(dc2);
	DeleteDC(dc3);
	DeleteDC(dc4);
	DeleteObject(hbm);
	DeleteObject(hbm3);
	DeleteObject(hbm4);
}

//�˭ץΤu�� 
/*
tclock1 = clock();
		tclock2 = clock();
	    char display3[100];  
	    int sec=(tclock2-tclock1)/(double)(CLOCKS_PER_SEC);
		sprintf(display3,"�ɶ�: %2d�G%2d�G%2d",sec/3600%60,sec/60%60,sec%60);
		TextOut(dc1,960,660,display3,strlen(display3));
		
		while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	   	
		if (GetAsyncKeyState(27)<0)   //��esc���� 
		{
			break;
		}
		
*/
