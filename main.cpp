#include<windows.h>
#include<stdio.h>
#include<time.h>
#include"function.h"
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,UINT wparam,UINT lparam);
int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hpre,LPSTR lPstring,int ncmd)
{
	MSG msg;
	WNDCLASS wnd;
	wnd.hIcon=LoadIcon(hinstance,"DICO");
	wnd.hCursor=LoadCursor(NULL,IDC_ARROW);
	wnd.cbClsExtra=0;
	wnd.cbWndExtra=0;
	wnd.hbrBackground=NULL;
	wnd.hInstance=hinstance;
	wnd.lpfnWndProc=(WNDPROC)WndProc;
	wnd.lpszClassName="Game";
	wnd.lpszMenuName=NULL;
	wnd.style=CS_HREDRAW|CS_VREDRAW;
	if(!RegisterClass(&wnd))
		return FALSE;
	hwnd=CreateWindow("Game","Ñ¼×ÓÌøÔ¾",WS_OVERLAPPED|WS_MINIMIZEBOX|WS_SYSMENU,400,50,500,580,NULL,NULL,hinstance,NULL);
	ShowWindow(hwnd,ncmd);
	UpdateWindow(hwnd);
	srand((unsigned)time(NULL));
	hdc=GetDC(hwnd);
	hdcmem=CreateCompatibleDC(hdc);
	hdcm=CreateCompatibleDC(hdc);
	bufdc=CreateCompatibleDC(hdc);
	Loadmap(hinstance);
	SelectObject(hdcmem,hBm);
	SelectObject(bufdc,hbk);
	fp=fopen("Record","rb");
	if(fp)
	{
		fread(mode,sizeof(int),4,fp);//¶ÁÈ¡¼ÇÂ¼
		fclose(fp);
	}
	Menu=1;
	Tn=1;
	BTn=0;
	t1=GetTickCount();
	int i,k,high;
	high=40;
	do
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			t2=GetTickCount();
			if(t2-t1>=50)
			{
				if(Menu)
				{
					SelectObject(hdcmem,hf);
					SelectObject(hdcm,Bkg[Tn].hbm);
					BitBlt(hdcmem,0,0,Bkg[Tn].bm.bmWidth,Bkg[Tn].bm.bmHeight,hdcm,0,0,SRCCOPY);
					for(i=0,k=0;i<18;i++)
					{
						if(U[i].TestShow())
						{
							U[i].SetPoint(high+k*60);
							U[i].PaintUI();
							k++;
						}
					}
					if(help||record)
					{
						HPEN hp;
						HBRUSH hbr;
						hp=CreatePen(PS_SOLID,0,RGB(0,0,0));
						hbr=CreateSolidBrush(RGB(160,215,100));
						SelectObject(hdcmem,hp);
						SelectObject(hdcmem,hbr);
						RoundRect(hdcmem,100,100,400,360,10,10);
						DeleteObject(hp);
						DeleteObject(hbr);
						SetBkMode(hdcmem,TRANSPARENT);
						SetTextColor(hdcmem,RGB(0,0,0));
						SelectObject(hdcmem,hftxt);
						if(help)
							for(i=0;i<6;i++)
								TextOut(hdcmem,110,120+i*30,helptxt[i],strlen(helptxt[i]));
						else
						{
							char txt[40];
							for(i=0;i<4;i++)
							{
								sprintf(txt,"%s%d",scoretxt[i],mode[i]);
								TextOut(hdcmem,130,140+i*35,txt,strlen(txt));
							}
						}
						SelectObject(hdcmem,hf);
						U[18].PaintUI();
					}
					SelectObject(hdcm,Duck[0][1].chm);
					BitBlt(hdcmem,70,320,Duck[0][1].bm.bmWidth,Duck[0][1].bm.bmHeight,hdcm,0,0,SRCAND);
					SelectObject(hdcm,Duck[0][1].hbm);
					BitBlt(hdcmem,70,320,Duck[0][1].bm.bmWidth,Duck[0][1].bm.bmHeight,hdcm,0,0,SRCPAINT);
					BitBlt(hdc,0,0,500,1000,hdcmem,0,0,SRCCOPY);
				}
				else
				{
					Move();
					Paint();
				}
				t1=t2;
			}
		}
	}while(msg.message!=WM_QUIT);
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,UINT wparam,UINT lparam)
{
	int i,j;
	POINT p;
	switch(msg)
	{
	case WM_MOUSEMOVE:
			p.x=LOWORD(lparam);
			p.y=HIWORD(lparam);
			i=19;
			if(record||help)
			{
				i--;
				if(U[i].MouseInUI(p))
					U[i].SetColor(1);
				else
					U[i].SetColor(0);
			}
			else
			{
				
				for(i--;i>=0;i--)
				{
					if(U[i].TestShow()&&U[i].MouseInUI(p))
					{
						U[i].SetColor(1);
						if(i<=2)
							Tn=i;
						else if(i<=9)
							BTn=i;
						break;
					}
					else
						U[i].SetColor(0);
				}
			}
			for(i--;i>=0;i--)
				U[i].SetColor(0);
		return 0;
	case WM_LBUTTONDOWN:
		for(i=18;i>=0;i--)
		{
			if(U[i].TestShow()&&U[i].TestColor())
			{
				U[i].Respond();
				U[i].SetColor(0);
				break;
			}
		}
		return 0;		
	case WM_KEYDOWN:
		switch(wparam)
		{
		case VK_LEFT:
			duck.Dn=0;
			duck.a1=-3;
			break;
		case VK_RIGHT:
			duck.Dn=1;
			duck.a1=3;
			break;
		case VK_SPACE:
			atk=1;
			break;
		}
		return 0;
	case WM_KEYUP:
		switch(wparam)
		{
		case VK_LEFT:
			duck.v1=0;
			duck.a1=0;
			break;
		case VK_RIGHT:
			duck.v1=0;
			duck.a1=0;
			break;
		}
		return 0;
	case WM_DESTROY:
		DeleteObject(hf);
		DeleteObject(hftxt);
		DeleteObject(hBm);
		DeleteObject(hpr);
		DeleteObject(Chpr);
		DeleteObject(hbk);
		DeleteObject(Green.hbm);
		DeleteObject(Green.chm);
		DeleteObject(Blue.hbm);
		DeleteObject(Blue.chm);
		DeleteObject(Gray.hbm);
		DeleteObject(Gray.chm);
		DeleteObject(White.hbm);
		DeleteObject(White.chm);
		DeleteObject(Bullet.hbm);
		DeleteObject(Bullet.chm);
		DeleteObject(Attack.hbm);
		DeleteObject(Attack.chm);	
		for(i=0;i<10;i++)
		{
			if(i<2)
			{
				for(j=0;j<2;j++)
				{
					DeleteObject(Prop_help[i][j].hbm);
					DeleteObject(Prop_help[i][j].chm);
				}
			}
			if(i<3)
			{
				DeleteObject(Bkg[i].hbm);
				DeleteObject(Bkg[i].chm);
			}
			if(i<4)
			{
				DeleteObject(Brown[i].hbm);
				DeleteObject(Brown[i].chm);
			}
			if(i<8)
			{
				DeleteObject(Time[i].hbm);
				DeleteObject(Time[i].chm);
			}
			if(i<5)
			{
				DeleteObject(Prop_take[i].hbm);
				DeleteObject(Prop_take[i].chm);
				for(j=0;j<2;j++)
				{
					DeleteObject(Duck[i][j].hbm);
					DeleteObject(Duck[i][j].chm);
				}
			}
			if(i<7)
			{
				DeleteObject(Monster[i].hbm);
				DeleteObject(Monster[i].chm);
			}
			DeleteObject(Number[i].hbm);
			DeleteObject(Number[i].chm);
		}
		DeleteDC(hdcmem);
		DeleteDC(hdcm);
		DeleteDC(bufdc);
		ReleaseDC(hwnd,hdc);
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd,msg,wparam,lparam);
	}
}





