#include <stdio.h>
#include <windows.h>
#include <string.h>  
#include <stdlib.h>   // rand() srand()  system ("cls")
#include <time.h>    // time()
#include <conio.h> 
#define sw 1216 -20
#define sh 684+38-8
#define sx 20
#define sy 34
#define HP 200
#define mn 10
#define bn 2    //bomb number
#define spacetime 0.1
#define pixel 34
///////////////////////////�H�U�K��+�}����///////////////////////////////////// 





typedef struct pic{  //�C�@�i�ϳ��n����������T
	HDC dc;
	HBITMAP hbm;
	int width;
	int height;	
}picture ;
HWND hwnd;
HDC  dc1,dc2; 
HBITMAP hbm;
picture p[200];
MSG msg;

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
	wnd.hbrBackground = ( HBRUSH )COLOR_BACKGROUND;
	wnd.lpszClassName = cname;
	
	if( RegisterClass( &wnd ) == 0 ){
		MessageBox( NULL, "�L�k���U", "In reg_class", MB_ICONERROR );
	}
}

struct mover   //struct�G�ŧi�N�ϥε��c   mover �G���a+���~ 
	{
		int t;  //type�G 21~30�N���a   31~40�N��U�ة��~
		int d;  //Direction�G���a+���~ �����ʤ�V 
		int ox;  //old x �y�� 
		int oy;  //old y �y��  
		int nx;  //new x �y��  
		int ny;  //new y �y��  
	};
struct mover mover[40];     //�ŧi�}�Cmover[40] �����cmover���κA   
	//mover[21~30]�N���a   mover[31~40]�N��U�ة��~
int i,j,Map_1[sx][sy],space_lock=1, evermove, k,l,W;
double space_time=0.15;                      
LARGE_INTEGER t1, t2, frequency;
	
void B3_Level_2();     		//�i�J�C���Ĥ@�h 
void B3_Boundary();
void B3_BoundaryCondition();   //�]�w���+�I����ɪ��B�z 
void B3_GetKey();				//��o�ϥΪ̫���			 		
void B3_NextStep();   			//�P�_�U�@�B����m 
void B3_Output();       		//��X�e����©����� 
void B3_TimeControl();	 		//����ɶ����j 

void B3_Boundary()
{
	for(j=1;j<20;j++)      //�W�U��� 
	{
		Map_1[1][j]=11;
		Map_1[19][j]=11;
	}
	
	for(j=1;j<10;j++) 
		Map_1[10][j]=11;   //������� 
	for(j=11;j<20;j++)
		Map_1[10][j]=11;   //������� 
		
	Map_1[0][10]=80;      //���P$ 
	Map_1[1][10]=70;
	Map_1[10][10]=70;
	
	for(i=1;i<20;i++)     //���k��� 
	{
		Map_1[i][1]=11;
		Map_1[i][19]=11;
	}
	Map_1[15][5]=11;
	Map_1[15][6]=11;
	Map_1[16][5]=11;
	Map_1[16][6]=11;
	Map_1[16][7]=11;
	Map_1[17][6]=11;
	Map_1[17][7]=11;
	Map_1[15][15]=11;
	Map_1[14][15]=11;
	Map_1[15][14]=11;
	Map_1[14][14]=11;
	Map_1[13][15]=11;
	Map_1[13][14]=11;
	Map_1[13][16]=11;
	Map_1[13][17]=11;
	Map_1[14][17]=11;
	Map_1[15][17]=11;
	Map_1[13][16]=11;
	Map_1[14][16]=11;
	Map_1[15][16]=11;
	Map_1[13][18]=11;
	Map_1[14][18]=11;
	Map_1[15][18]=11;
	Map_1[16][4]=11;
	Map_1[16][12]=11;
	

	Map_1[3][5]=11;
	Map_1[4][6]=11;
	Map_1[5][7]=11;
	Map_1[6][8]=11;
	Map_1[7][9]=11;
	Map_1[8][10]=11;

	Map_1[8][4]=11;
	Map_1[7][5]=11;
	Map_1[6][6]=11;
	Map_1[5][7]=11;
	Map_1[4][8]=11;

	
	Map_1[13][11]=11;
	Map_1[14][11]=11;

	Map_1[9][8]=11;
	Map_1[8][5]=11;
	Map_1[7][2]=11;
	Map_1[3][4]=11;
	Map_1[3][6]=11;
	Map_1[4][7]=11;
	Map_1[6][7]=11;
	Map_1[5][6]=11;
	Map_1[5][8]=11;
	

	Map_1[4][17]=11;
	Map_1[4][18]=11;
	
	Map_1[5][16]=11;
	Map_1[5][17]=11;
	Map_1[5][18]=11;
	
	Map_1[2][11]=11;
	Map_1[2][14]=11;
	
	/*Map_1[1][5]=20;
	Map_1[5][5]=20;*/
	
}

void B3_BoundaryCondition()
{
	
	for (i=21;i<=40;i++)
	{
		if (Map_1[mover[i].nx][mover[i].ny]==11) //�p�G���a�]��������A�Y�h�^���e����m 
		{
			mover[i].nx=mover[i].ox;
			mover[i].ny=mover[i].oy;
			mover[i].d=0;          //�ð���� 
		}
		if (Map_1[mover[i].nx][mover[i].ny]==70) //�p�G���a�]��������A�Y�h�^���e����m 
		{
			mover[i].nx=mover[i].ox;
			mover[i].ny=mover[i].oy;
			mover[i].d=0;          //�ð���� 
		}
		if (Map_1[mover[i].nx][mover[i].ny]==20) //�p�G���a�]��������A�Y�h�^���e����m 
		{
			mover[i].nx=mover[i].ox;
			mover[i].ny=mover[i].oy;
			mover[i].d=0;          //�ð���� 
		}
		if (Map_1[mover[i].nx][mover[i].ny]==80) //�p�G���a�]��������A�Y�h�^���e����m 
		{
			mover[i].nx=mover[i].ox;
			mover[i].ny=mover[i].oy;
			mover[i].d=0;          //�ð���� 
		}
 
 		if (Map_1[mover[i].nx][mover[i].ny]==50)
 		{
			mover[i].nx=mover[i].ox;
			mover[i].ny=mover[i].oy;
			mover[i].d=0;		
		}
	 }
	
	 

}



void B3_GetKey()
{
	for(mover[21].d=0,i=37;i<=40;i++)
	{
		if(GetAsyncKeyState(i)<0)
		mover[21].d=i;
	}
} 

void B3_NextStep ()
{
	evermove=0;
	if(space_lock==0)   //���Q���~��� 
	{					   
		for (i=21;i<=40;i++)
		{
			mover[i].ox=mover[i].nx;
			mover[i].oy=mover[i].ny;
			switch (mover[i].d)
			{
				case 37: 
					mover[i].ny--;
					evermove=1; 
					break;
				case 38: 
					mover[i].nx--;
					evermove=1;
					break;
				case 39: 
					mover[i].ny++;
					evermove=1;
					break;
				case 40: 
					mover[i].nx++;
					evermove=1;
					break;
				default:
					break;
			}
		}
		
	}
}



void B3_TimeControl()
{
	if(evermove==1)
	{
		space_lock=1 ;								
		QueryPerformanceCounter(&t1);
	}
	if(space_lock==1) 						     //�Y�O��W�A�ݬO�_�i�H���� 
	{
		QueryPerformanceCounter(&t2);          //�����p�� 
		QueryPerformanceFrequency(&frequency);	//���o�q�����檺�W�v ex:3.5GHz 										  //���D�����ɶ�
		if((double)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart>=space_time)  //(�����ɶ�-�}�l�ɶ�)/�W�v 
		{
			space_lock=0; 						//���� 
		} 
	}
}


void B3_Window_Picture()
{
	reg_class( "classname" );
	HWND hwnd = CreateWindow( "classname", "�Ĥ���", WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE, 0, 0, sw, sh, NULL, NULL, NULL, NULL );
	dc1=GetDC(hwnd);    //�K�Ϩ�dc1�A�N�|��ܦb�����W(hwnd�֦������v������)
	hbm=CreateCompatibleBitmap(dc1,sw,sh) ;
	dc2=CreateCompatibleDC(dc1);
	SelectObject(dc2,hbm);//�V�t��hbm���Udc2���ϥ��v(���G�@��hbm�u����U�@��dc2)

	p[0]=readBMP("pic//�n�a�O.bmp", dc1); 
	p[11]=readBMP("pic//���.bmp", dc1);   
	p[20]=readBMP("pic//���.bmp", dc1);
	p[40]=readBMP("pic//���a.bmp", dc1);
	p[50]=readBMP("pic//�H�a�O.bmp", dc1);
	p[70]=readBMP("pic//�K��.bmp", dc1);
	p[80]=readBMP("pic//�_��.bmp", dc1);
	p[90]=readBMP("pic//���}.bmp", dc1); 
	p[100]=readBMP("pic//KEY.bmp", dc1);   

	

	
};
		
void B3_Output()
{
		
	for(i=0;i<21;i++)
	{
		for(j=0;j<21;j++)
		{
		    if (Map_1[i][j]==0)	
				TransparentBlt(dc2,j*pixel,i*pixel,pixel,pixel,p[0].dc,0,0,p[0].width,p[0].height,RGB(255,255,255));           //�}�C�����Ȭ�0~10 -->��X�D��        " " 
			else if (Map_1[i][j]==11)
				TransparentBlt(dc2,j*pixel,i*pixel,pixel,pixel,p[11].dc,0,0,p[11].width,p[11].height,RGB(255,255,255));		   //�}�C�����Ȭ�11~20 -->��X���        "o"
			else if (Map_1[i][j]==20)
				TransparentBlt(dc2,j*pixel,i*pixel,pixel,pixel,p[20].dc,0,0,p[20].width,p[20].height,RGB(255,255,255)); 		   //�}�C�����Ȭ�11~20 -->��X���        "o"
		    else if (Map_1[i][j]==40)
				TransparentBlt(dc2,j*pixel,i*pixel,pixel,pixel,p[40].dc,0,0,p[40].width,p[40].height,RGB(255,255,255));       //�}�C�����Ȭ�21~30 /31~40-->��X���a/�Ǫ�����m    "X" 
			else if (Map_1[i][j]==50)
				TransparentBlt(dc2,j*pixel,i*pixel,pixel,pixel,p[50].dc,0,0,p[50].width,p[50].height,RGB(255,255,255));
			else if (Map_1[i][j]==70)
				TransparentBlt(dc2,j*pixel,i*pixel,pixel,pixel,p[70].dc,0,0,p[70].width,p[70].height,RGB(255,255,255));
			else if (Map_1[i][j]==80)
				TransparentBlt(dc2,j*pixel,i*pixel,pixel,pixel,p[80].dc,0,0,p[80].width,p[80].height,RGB(255,255,255));
			else if (Map_1[i][j]==90)
				TransparentBlt(dc2,j*pixel,i*pixel,pixel,pixel,p[90].dc,0,0,p[90].width,p[90].height,RGB(255,255,255));
			else if (Map_1[i][j]==110)
				TransparentBlt(dc2,j*pixel,i*pixel,pixel,pixel,p[90].dc,0,0,p[90].width,p[90].height,RGB(255,255,255));
		
		}
	}
	
	/*
	if (W==1){
		TransparentBlt(dc2,j*200,i*200,200,200,p[100].dc,0,0,p[100].width,p[100].height,RGB(255,255,255));
	}
	*/
		
		BitBlt(dc1,0,0,sw,sh,dc2,0,0,SRCCOPY);
		
		
}


 
 void B3_Level_2()
{
	B3_Window_Picture();	

	B3_Output();
	
		 
		memset(Map_1,0,sizeof(int)*20*20);    //�M�Ű}�C Map_1
	for (i=21;i<=40;i++)				  //�M�ŵ��c�}�Cmover[40] 
	{
		mover[i].d=0;
		mover[i].nx=0;
		mover[i].ny=0;
		mover[i].ox=0;
		mover[i].oy=0;
	} 
	B3_Boundary();                  //���������m 
	mover[21].nx=11,mover[21].ny=11 ;    //�_�l��m�b(6,6) 
	Map_1[mover[21].nx][mover[21].ny]=40; //�}�C�������a��m���Ȭ�21  
	B3_Output();	
	//char pos[64]; 
	while(1)                                
	{
		B3_GetKey();							
		B3_NextStep();
		B3_BoundaryCondition();
		
		for (i=21;i<=40;i++)       
		{
			Map_1[mover[i].ox][mover[i].oy]=50;   //���a�¦�m=�} 
			Map_1[mover[i].nx][mover[i].ny]=40;   //���a��m= 40
			
			if (k==136){
			Map_1[10][10]=90;
			}
			if (l==119){
			Map_1[1][10]=90;
		}
		}	
		
		Map_1[1][1]=11;                      //�@��(1,1)����� 
		Map_1[0][0]=0;		
		 
		for(k=1,i=11;i<=18;i++)
	{
		for(j=2;j<=18;j++)
		{
			if (Map_1[i][j]>10)	
			k++;
		}
		
	}
	for(l=1,i=3;i<=9;i++)
	{
		for(j=2;j<=18;j++)
		{
			if (Map_1[i][j]>10)	
			l++;
		}
	}
		
		char display[100];  //���k.l 
		sprintf(display,"k=%d,l=%d",k,l);
		TextOut(dc1,425,0,display,strlen(display));
		
	
		
	/*	if (k>=19){        //�U����L19�ӡA���~�|�} 
			Map_1[5][5]=90;
		}
		if (l>=18){      //�W����L18�ӡA���~�|�}  
			Map_1[1][5]=110;
		}*/
		if (Map_1[mover[21].nx][mover[21].ny]==110) //�����I
			{
				MessageBox(NULL,"KEY","xD",MB_OK);
				break;    
			}

		
		
		
		B3_TimeControl();	
		
		
		if(WM_QUIT==msg.message)
			break;
			
			
		while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0)
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		
		
		if(GetAsyncKeyState(27)<0)
			break;
		
		
		
		if (evermove==1)                         //�����ʤ~�ݭn���sprint 
		{
			
			B3_Output();						
			Sleep(10);			
		}	
	/*	sprintf(pos, "x = %d, y = %d", mover[i].nx, mover[i].ny);
		TextOut( dc1, 500, 0, pos, strlen(pos));  */
	}					
	

	//�N�귽�ٵ��t��
	ReleaseDC(hwnd,dc1);
	DeleteDC(dc2);
	DeleteObject(hbm);
}
 
 

 
 


int main()
{
		B3_Level_2();
		
		
	//return 0;
}
