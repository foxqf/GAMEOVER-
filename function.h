HDC hdc,hdcmem,hdcm,bufdc;
HWND hwnd;
HFONT hf,hftxt;
HBITMAP hBm,hpr,Chpr,hbk;
DWORD t1,t2;
int BKmove,TFlag,htest,H,level;
POINT broke[10],gray[5];
int Num,Bn,Gn,Mn,Tn,BTn,Ln;//计数器
struct Doodle
{
	POINT pt;
	int a0,v0,a1,v1;
	int Dn;
	int sf,pn,fy;
	int time,stime,ptime;
};
Doodle duck;
struct BULLET
{
	POINT pt;
	int v,m,kind;
};
BULLET bt[20];
bool Menu,atk,life;//玩家生命
struct Block
{
	POINT pt;
	WORD kind,prop;//砖块和道具种类
	bool b;//道具状态
	double n;
	int broke;//砖块破碎标识
	int OP,yh,yl;//砖块移动坐标
};
Block block[50];
struct Hbitmap
{
	HBITMAP hbm;
	HBITMAP chm;
	BITMAP bm;
};
Hbitmap Bkg[3],Green,Blue,Gray,White,Brown[4],Time[8],Duck[5][2],Attack,Bullet,Number[10],Prop_take[5],Prop_help[2][2],Monster[7];
struct MONSTER
{
	POINT pt;
	int kind;//怪兽种类
	int life;//怪兽生命值
};
MONSTER mstr[40];
double M[40]={0};
//UI接口
class UI
{
private:
	int kind;//接口种类
	bool show;//是否显示UI
	bool color;
	char *str;
	POINT pt1,pt2;
public:
	UI(int k,bool s,bool c,char *st):kind(k),show(s),color(c),str(st){};
	void SetPoint(int);
	void SetShow(bool);
	bool TestShow();
	void SetColor(bool);
	bool TestColor();
	void PaintUI();
	bool MouseInUI(POINT);
	void Respond();
};
UI U[19]={
	UI(0,0,0,"Heaven mode"),
	UI(1,0,0,"Earth mode"),
	UI(2,0,0,"Hell mode"),
	UI(3,0,0,"豆腐渣版"),
	UI(4,0,0,"急救版"),
	UI(5,0,0,"360卫士版"),
	UI(6,0,0,"哆啦A梦版"),
	UI(7,0,0,"宇宙版"),
	UI(8,0,0,"胡克定律版"),
	UI(9,0,0,"无尽怪物版"),
	UI(10,0,0,"普通版"),
	UI(11,0,0,"BT版"),
	UI(12,1,0,"Play"),
	UI(13,1,0,"Record"),
	UI(14,1,0,"Help"),
	UI(15,1,0,"Exit"),
	UI(16,0,0,"Again"),
	UI(17,0,0,"Menu"),
	UI(18,0,0,"OK"),
};
FILE *fp;
DWORD Score,show,mode[4]={0};
bool help=0,record=0;
char *helptxt[6]={"← →控制移动，空格发射子",
                  "弹。普通版有3种模式，越往",
				  "下的模式中,道具数量变少，",
				  "怪物数量增多,难度更大！BT",
				  "版则为娱乐之用",
				  "             ——by.foxqf"};
char *scoretxt[4]={"Heaven mode: ",
				   "Earth mode:  ",
				   "Hell mode:   ",
				   "BT mode:     "};
void GameInit();
void UI::SetPoint(int h)
{
	SIZE size;
	pt1.y=h;
	pt2.y=h+50;
	GetTextExtentPoint32(hdcmem,str,strlen(str),&size);
	pt1.x=(500-size.cx-40)/2;
	pt2.x=500-pt1.x;
}
void UI::SetShow(bool s)
{
	show=s;
}
bool UI::TestShow()
{
	return show;
}
void UI::SetColor(bool c)
{
	color=c;
}
bool UI::TestColor()
{
	return color;
}
void UI::PaintUI()
{
	HPEN hp;
	HBRUSH hbr;
	hp=CreatePen(PS_SOLID,0,RGB(0,0,0));
	if(color)
		hbr=CreateSolidBrush(RGB(95,165,70));
	else
		hbr=CreateSolidBrush(RGB(160,215,100));
	SelectObject(hdcmem,hp);
	SelectObject(hdcmem,hbr);
	RoundRect(hdcmem,pt1.x,pt1.y,pt2.x,pt2.y,10,10);
	SetBkMode(hdcmem,TRANSPARENT);
	SetTextColor(hdcmem,RGB(0,0,0));
	TextOut(hdcmem,pt1.x+20,pt1.y+15,str,strlen(str));
	DeleteObject(hp);
	DeleteObject(hbr);
}
bool UI::MouseInUI(POINT p)
{
	if(p.x>=pt1.x&&p.x<=pt2.x&&p.y>=pt1.y&&p.y<=pt2.y)
		return true;
	else
		return false;
}
void UI::Respond()
{
	int i;
	if(kind<10)
	{
		Menu=0;
		GameInit();
		for(i=0;i<19;i++)
			U[i].SetShow(0);
	}
	switch(kind)
	{
	case 16:
		Menu=0;
		GameInit();
		for(i=0;i<19;i++)
			U[i].SetShow(0);
		break;
	case 10:
		U[10].SetShow(0);
		U[11].SetShow(0);
		U[13].SetShow(0);
		U[14].SetShow(0);
		U[0].SetShow(1);
		U[1].SetShow(1);
		U[2].SetShow(1);
		break;
	case 11:
		U[10].SetShow(0);
		U[11].SetShow(0);
		U[13].SetShow(0);
		U[14].SetShow(0);
		U[3].SetShow(1);
		U[4].SetShow(1);
		U[5].SetShow(1);
		U[6].SetShow(1);
		U[7].SetShow(1);
		U[8].SetShow(1);
		U[9].SetShow(1);
		break;
	case 12:
		U[10].SetShow(1);
		U[11].SetShow(1);
		U[12].SetShow(0);
		break;
	case 13:
		record=1;
		U[18].SetPoint(300);
		U[18].SetShow(1);
		break;
	case 14:
		help=1;	
		U[18].SetPoint(300);
		U[18].SetShow(1);
		break;
	case 15:
		SendMessage(hwnd,WM_DESTROY,0,0);
		break;
	case 17:
		Menu=1;
		BTn=0;
		Tn=1;
		for(i=0;i<19;i++)
			U[i].SetShow(0);
		U[12].SetShow(1);
		U[13].SetShow(1);
		U[14].SetShow(1);
		U[15].SetShow(1);
		break;
	case 18:
		help=0;
		record=0;
		U[18].SetShow(0);
		break;
	}
}
void GameInit()
{
	Score=0;
	show=0;
	level=530;
	BKmove=0;
	TFlag=0;
	htest=2;
	Num=0;
	Bn=0;
	Gn=0;
	Mn=0;
	Ln=0;
	duck.pt.x=190;
	duck.pt.y=500;
	duck.a0=5;
	duck.v0=55;
	duck.a1=0;
	duck.v1=0;
	duck.Dn=0;
	duck.time=0;
	duck.sf=0;
	duck.stime=0;
	duck.fy=0;
	duck.pn=1;
	duck.ptime=80;
	atk=0;
	life=1;
}
void Loadmap(HINSTANCE hinstance)
{
	int i,j,k;
	char s[10];
	HPEN hp;
	HBRUSH hbr;
	hBm=CreateCompatibleBitmap(hdc,500,1000);
	hbk=CreateCompatibleBitmap(hdc,120,120);

	hpr=CreateCompatibleBitmap(hdc,120,120);
	hp=CreatePen(PS_SOLID,0,RGB(0,0,0));
	hbr=CreateSolidBrush(RGB(0,0,0));
	SelectObject(hdcm,hpr);
	SelectObject(hdcm,hp);
	SelectObject(hdcm,hbr);
	Rectangle(hdcm,0,0,120,120);
	DeleteObject(hp);
	DeleteObject(hbr);
	hp=CreatePen(PS_SOLID,0,RGB(160,215,100));
	hbr=CreateSolidBrush(RGB(160,215,100));
	SelectObject(hdcm,hp);
	SelectObject(hdcm,hbr);
	Ellipse(hdcm,0,0,120,120);
	DeleteObject(hp);
	DeleteObject(hbr);

	Chpr=CreateCompatibleBitmap(hdc,120,120);
	hp=CreatePen(PS_SOLID,0,RGB(255,255,255));
	hbr=CreateSolidBrush(RGB(255,255,255));
	SelectObject(hdcm,Chpr);
	SelectObject(hdcm,hp);
	SelectObject(hdcm,hbr);
	Rectangle(hdcm,0,0,120,120);
	DeleteObject(hp);
	DeleteObject(hbr);
	hp=CreatePen(PS_SOLID,0,RGB(0,0,0));
	hbr=CreateSolidBrush(RGB(0,0,0));
	SelectObject(hdcm,hp);
	SelectObject(hdcm,hbr);
	Ellipse(hdcm,0,0,120,120);
	DeleteObject(hp);
	DeleteObject(hbr);

	//背景
	for(i=0;i<3;i++)
	{
		sprintf(s,"BK%d",i);
		Bkg[i].hbm=LoadBitmap(hinstance,s);
		GetObject(Bkg[i].hbm,sizeof(BITMAP),(LPVOID)&Bkg[i].bm);
	}
	//绿色方块
	Green.hbm=LoadBitmap(hinstance,"GREEN");
	Green.chm=LoadBitmap(hinstance,"CGREEN");
	GetObject(Green.hbm,sizeof(BITMAP),(LPVOID)&Green.bm);
	//蓝色方块
	Blue.hbm=LoadBitmap(hinstance,"BLUE");
	Blue.chm=LoadBitmap(hinstance,"CBLUE");
	GetObject(Blue.hbm,sizeof(BITMAP),(LPVOID)&Blue.bm);
	//灰色方块
	Gray.hbm=LoadBitmap(hinstance,"GRAY");
	Gray.chm=LoadBitmap(hinstance,"CGRAY");
	GetObject(Gray.hbm,sizeof(BITMAP),(LPVOID)&Gray.bm);
	//白色方块
	White.hbm=LoadBitmap(hinstance,"WHITE");
	White.chm=LoadBitmap(hinstance,"CWHITE");
	GetObject(White.hbm,sizeof(BITMAP),(LPVOID)&White.bm);
	//脆弱方块
	for(i=0;i<4;i++)
	{
		sprintf(s,"BROWN%d",i);
		Brown[i].hbm=LoadBitmap(hinstance,s);
		sprintf(s,"CBROWN%d",i);
		Brown[i].chm=LoadBitmap(hinstance,s);
		GetObject(Brown[i].hbm,sizeof(BITMAP),(LPVOID)&Brown[i].bm);
	}
	//时限方块
	for(i=0;i<8;i++)
	{
		sprintf(s,"TIME%d",i);
		Time[i].hbm=LoadBitmap(hinstance,s);
		sprintf(s,"CTIME%d",i);
		Time[i].chm=LoadBitmap(hinstance,s);
		GetObject(Time[i].hbm,sizeof(BITMAP),(LPVOID)&Time[i].bm);
	}
	//主角
	for(i=0;i<5;i++)
		for(j=0;j<2;j++)
		{
			sprintf(s,"DUCK%d%d",i,j);
			Duck[i][j].hbm=LoadBitmap(hinstance,s);
			sprintf(s,"CDUCK%d%d",i,j);
			Duck[i][j].chm=LoadBitmap(hinstance,s);
			GetObject(Duck[i][j].hbm,sizeof(BITMAP),(LPVOID)&Duck[i][j].bm);
		}
	Attack.hbm=LoadBitmap(hinstance,"ATTACK");
	Attack.chm=LoadBitmap(hinstance,"CATTACK");
	GetObject(Attack.hbm,sizeof(BITMAP),(LPVOID)&Attack.bm);
	Bullet.hbm=LoadBitmap(hinstance,"BULLET");
	Bullet.chm=LoadBitmap(hinstance,"CBULLET");
	GetObject(Bullet.hbm,sizeof(BITMAP),(LPVOID)&Bullet.bm);
	//数字
	for(i=0;i<10;i++)
	{
		sprintf(s,"NUMBER%d",i);
		Number[i].hbm=LoadBitmap(hinstance,s);
		sprintf(s,"CNUMBER%d",i);
		Number[i].chm=LoadBitmap(hinstance,s);
		GetObject(Number[i].hbm,sizeof(BITMAP),(LPVOID)&Number[i].bm);
	}
	//道具
	for(i=0;i<5;i++)
	{
		sprintf(s,"TPROP%d",i);
		Prop_take[i].hbm=LoadBitmap(hinstance,s);
		sprintf(s,"CTPROP%d",i);
		Prop_take[i].chm=LoadBitmap(hinstance,s);
		GetObject(Prop_take[i].hbm,sizeof(BITMAP),(LPVOID)&Prop_take[i].bm);
		if(i==4)
			break;
		j=i%2;
		k=i/2;
		sprintf(s,"HPROP%d%d",k,j);
		Prop_help[k][j].hbm=LoadBitmap(hinstance,s);
		sprintf(s,"CHPROP%d%d",k,j);
		Prop_help[k][j].chm=LoadBitmap(hinstance,s);
		GetObject(Prop_help[k][j].hbm,sizeof(BITMAP),(LPVOID)&Prop_help[k][j].bm);
	}
	//怪兽
	for(i=0;i<7;i++)
	{
		sprintf(s,"MONSTER%d",i);
		Monster[i].hbm=LoadBitmap(hinstance,s);
		sprintf(s,"CMONSTER%d",i);
		Monster[i].chm=LoadBitmap(hinstance,s);
		GetObject(Monster[i].hbm,sizeof(BITMAP),(LPVOID)&Monster[i].bm);
	}
	//字体创建
	hf=CreateFont(25,0,0,0,600,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"楷体_GB2312");
	hftxt=CreateFont(20,0,0,0,600,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"楷体_GB2312");
}
void Move()
{
	POINT p;
	int i,j,k;
	bool h;
	//快速向上飞行时背景拉动的处理代码
	if(duck.time)
	{
		k=duck.v0;
		for(i=0;i<Num;i++)
		{
			block[i].pt.y+=k;
			if(block[i].kind==1)
			{
				block[i].yh+=k;
				block[i].yl+=k;
			}
			if(block[i].pt.y>510)
			{
				for(j=i;j<Num-1;j++)
					block[j]=block[j+1];
				Num--;
				i--;
			}
		}
		H+=k;
		Score+=k;
		for(i=0;i<Ln;i++)
			bt[i].pt.y+=k;
		for(i=0;i<Mn;i++)
		{
			mstr[i].pt.y+=k;
			if(mstr[i].pt.y>500)
			{
				for(j=i;j<Mn-1;j++)
					mstr[j]=mstr[j+1];
				Mn--;
				i--;
			}
		}
		for(i=0;i<Bn;i++)
		{
			broke[i].y+=k;
			if(broke[i].y>520)
			{
				for(j=i;j<Bn-1;j++)
					broke[j]=broke[j+1];
				Bn--;
				i--;
			}
		}
		for(i=0;i<Gn;i++)
			gray[i].y+=k;
		duck.time--;
		if(duck.time==0)
		{
			duck.fy=0;
			duck.v0=40;
		}
	}
	//跳动时背景拉动的处理代码
	if(BKmove)
	{
		for(i=0;i<Num;i++)
		{
			block[i].pt.y+=20;
			if(block[i].kind==1)
			{
				block[i].yh+=20;
				block[i].yl+=20;
			}
			if(block[i].pt.y>510)
			{
				for(j=i;j<Num-1;j++)
					block[j]=block[j+1];
				Num--;
				i--;
			}
		}
		H+=20;
		duck.pt.y+=20;
		level+=20;
		for(i=0;i<Ln;i++)
			bt[i].pt.y+=20;
		for(i=0;i<Mn;i++)
		{
			mstr[i].pt.y+=20;
			if(mstr[i].pt.y>500)
			{
				for(j=i;j<Mn-1;j++)
					mstr[j]=mstr[j+1];
				Mn--;
				i--;
			}
		}
		for(i=0;i<Bn;i++)
		{
			broke[i].y+=20;
			if(broke[i].y>520)
			{
				for(j=i;j<Bn-1;j++)
					broke[j]=broke[j+1];
				Bn--;
				i--;
			}
		}
		for(i=0;i<Gn;i++)
			gray[i].y+=20;
		if(duck.pt.y>=250)
			BKmove=0;
	}
	//子弹运动代码
	for(i=0;i<Ln;i++)
	{
		bt[i].pt.y-=60;	
		bt[i].pt.x+=bt[i].v;
		if(bt[i].kind!=-1)
		{
			bt[i].m--;
			if(!bt[i].m)
			{
				k=bt[i].kind;
				mstr[k].life--;
				if(!mstr[k].life)
				{
					for(j=k;j<Mn-1;j++)
						mstr[j]=mstr[j+1];
					Mn--;
					for(j=0;j<Ln;j++)
					{
						if(k==bt[j].kind)
							bt[j].kind=-1;
					}
				}
				for(j=i;j<Ln-1;j++)
					bt[j]=bt[j+1];
				Ln--;
				i--;
			}	
		}
		else if(bt[i].pt.y<30)
		{
			for(j=i;j<Ln-1;j++)
				bt[j]=bt[j+1];
			Ln--;
			i--;
		}
	}
	//砖块产生代码段
	if(Num)
		p.y=H;
	else
		p.y=500;
	k=Score/6000+4;
	if(k>25)
		k=25;
	while(p.y>=40)
	{
		p.x=50;
		h=1;
		while(p.x<=370)
		{
			for(i=0;i<Gn;i++)
			{
				if(p.y<gray[i].y)
				{
					for(j=i;j<Gn-1;j++)
						gray[j]=gray[j+1];
					Gn--;
					i--;
				}
			}
			if(!BTn&&p.x==50&&p.y-70<40)
			{
				i=rand()%(100*(3-Tn));
				if(i==0)
				{
					p.x=(5+rand()%31)*10;
					mstr[Mn].pt=p;
					mstr[Mn].kind=0;
					Mn++;
					break;
				}
				else if(i<4)
				{
					p.x=(5+rand()%31)*10;
					mstr[Mn].pt=p;
					mstr[Mn].kind=1;
					mstr[Mn].life=1;
					Mn++;
					break;
				}
			}
			if(rand()%k==0)
			{
				if(Gn)
				{
					j=0;
					while(k<6)
					{
						p.x=(5+rand()%32)*10;
						for(i=0;i<Gn;i++)
							if(p.x+80>=gray[i].x&&p.x<=gray[i].x+80)
								break;
						if(i==Gn)
							break;
						j++;
					}
				}
				if(!Gn||j<6)
				{
					if(BTn!=0&&BTn!=9)
					{
						switch(BTn)
						{
						case 3:
						case 4:
							block[Num].kind=BTn;
							block[Num].n=0;
							block[Num].prop=0;
							break;
						case 5:
							block[Num].kind=2;
							block[Num].prop=5;
							break;
						case 6:
							block[Num].kind=2;
							block[Num].prop=4;
							break;
						case 7:
							block[Num].kind=2;
							block[Num].prop=3;
							break;
						case 8:
							block[Num].kind=2;
							block[Num].prop=1;
							break;
						}
						block[Num].pt=p;
					}
					else
					{
						if(TFlag)
						{
							block[Num].kind=4;
							block[Num].n=0;
							TFlag--;
						}
						else
						{
							i=rand()%60;
							if(i>62-k/3)
							{
								block[Num].kind=1;
								block[Num].yh=p.y-210;
								block[Num].yl=p.y;
								block[Num].OP=-1;
								gray[Gn].x=p.x;
								gray[Gn].y=p.y-210;
								Gn++;
							}
							else if(i>59-k/3)
							{
								block[Num].kind=4;
								block[Num].n=0;
								TFlag=8;
							}
							else if(i>50-k/3)
								block[Num].kind=3;
							else if(i>43-k/3)
							{
								block[Num].n=0;
								block[Num].broke=0;
								block[Num].kind=5;
							}
							else
								block[Num].kind=2;
						}
						block[Num].pt=p;
						block[Num].prop=0;
					}
					if(block[Num].kind!=5)
					{
						if(block[Num].kind<=2)
						{
							if(!BTn)
							{
								if(rand()%(Tn*10+5)==0)
									block[Num].prop=rand()%6+1;
								else if(block[Num].kind==2&&rand()%(60-k*Tn)==0)
								{
									mstr[Mn].kind=rand()%5+2;
									switch(mstr[Mn].kind)
									{
									case 2:
									case 6:
										mstr[Mn].life=1;
										break;
									case 3:
										mstr[Mn].life=3;
										break;
									case 4:
										mstr[Mn].life=2;
										break;
									case 5:
										mstr[Mn].life=2;
										break;
									}
									j=mstr[Mn].kind;
									mstr[Mn].pt.x=block[Num].pt.x+(80-Monster[j].bm.bmWidth)/2;
									mstr[Mn].pt.y=block[Num].pt.y-Monster[j].bm.bmHeight+5;
									Mn++;
								}
							}
							else if(BTn==9)
							{
								mstr[Mn].kind=rand()%5+2;
								mstr[Mn].life=1;
								j=mstr[Mn].kind;
								mstr[Mn].pt.x=block[Num].pt.x+(80-Monster[j].bm.bmWidth)/2;
								mstr[Mn].pt.y=block[Num].pt.y-Monster[j].bm.bmHeight+5;
								Mn++;
							}
							block[Num].b=0;
						}
						if(block[Num].prop==1)
						{
							h=0;
							htest=5;
						}
						if(htest<=4&&(block[Num].prop==2||block[Num].kind==1))
						{
							h=0;
							htest=4;
						}
						else if(htest<=2)
						{
							htest=2;
							h=0;
						}
					}
					p.x+=80;
					Num++;
				}
			}
			if(Gn)
				break;
			p.x+=rand()%3*30;
		}
		if(h)
			htest--;
		if(!htest)
		{
			p.x=(5+rand()%32)*10;
			if(BTn>=3&&BTn<=4)
			{
				block[Num].kind=BTn;
				block[Num].n=0;
				block[Num].pt=p;
				block[Num].prop=0;
			}
			else
			{
				for(i=Num-1;i>=0;i--)
				{
					if(block[i].pt.y==p.y)
						Num--;
					else
						break;
				}
				p.x=(5+rand()%32)*10;
				block[Num].kind=0;
				block[Num].pt=p;
				block[Num].OP=1;
				block[Num].prop=0;
				if(rand()%(Tn*10+5)==0)
					block[Num].prop=rand()%6+1;
				switch(BTn)
				{
				case 5:
					block[Num].prop=5;
					break;
				case 6:
					block[Num].prop=4;
					break;
				case 7:
					block[Num].prop=3;
					break;
				case 8:
					block[Num].prop=1;
					break;
				}
				block[Num].b=0;
			}
			if(block[Num].prop==1)
				htest=5;
			else if(block[Num].prop==2)
				htest=4;
			else
				htest=2;
			Num++;
		}
		p.y-=70;
		H=p.y;
	}
	//玩家移动代码段
	if(life)
	{
		if(duck.v1<30&&duck.v1>-30)
			duck.v1+=duck.a1;
		duck.pt.x+=duck.v1;
		if(duck.pt.x<20)
			duck.pt.x=390;
		else if(duck.pt.x>390)
			duck.pt.x=20;
	}
	if(life&&duck.v0<=0)
	{
		if(duck.v0==0)
		{
			Score+=level-duck.pt.y;
			level=duck.pt.y;
			for(i=0;i<Num;i++)
				if(block[i].prop)
					block[i].b=0;
		}
		for(i=Mn-1;i>=0;i--)
		{
			j=mstr[i].kind;
			if(j==1)
				k=Monster[1].bm.bmWidth/3;
			else
				k=Monster[j].bm.bmWidth;
			if(j&&duck.pt.x>=mstr[i].pt.x-50&&duck.pt.x<=mstr[i].pt.x+k-35&&duck.pt.y+85<=mstr[i].pt.y+7&&duck.pt.y-duck.v0+85>=mstr[i].pt.y)
			{
				duck.pt.y=mstr[i].pt.y-85;
				for(j=i;j<Mn-1;j++)
						mstr[j]=mstr[j+1];
				Mn--;
				break;
			}
		}
		k=-1;
		if(i==-1)
		{
			for(k=Num-1;k>=0;k--)
				if(duck.pt.x>=block[k].pt.x-50&&duck.pt.x<=block[k].pt.x+45&&duck.pt.y+85<=block[k].pt.y+7&&duck.pt.y-duck.v0+85>=block[k].pt.y)
				{
					duck.pt.y=block[k].pt.y-85;
					break;
				}
		}
		if(i==-1&&k==-1)
		{
			duck.pt.y-=duck.v0;
			duck.v0-=duck.a0;
		}
		else if(k!=-1)
		{
			switch(block[k].kind)
			{
			case 0:
			case 1:
			case 2:
				if(block[k].prop&&block[k].prop<=2&&duck.pt.x>=block[k].pt.x-30&&duck.pt.x<=block[k].pt.x+20)
				{
					block[k].b=1;
					duck.time=6-block[k].prop;
					duck.v0=50;
				}
				else
					duck.v0=40;
				break;
			case 4:
				duck.v0=40;
				break;
			case 3:
				duck.v0=40;
				for(j=k;j<Num-1;j++)
					block[j]=block[j+1];
				Num--;
				break;
			case 5:
				block[k].broke=1;
				duck.v0=-duck.a0;
				duck.pt.y-=2*duck.v0;
				break;
			}
			if(block[k].kind!=5&&duck.sf)
			{
				duck.time+=4;
				duck.v0=50;
			}
			if(duck.pt.y<400)
				BKmove=1;
		}
		else
		{
			duck.v0=40;
			if(duck.pt.y<400)
				BKmove=1;
		}
	}
	else if(duck.pt.y<600&&!duck.time)
	{
		duck.pt.y-=duck.v0;
		duck.v0-=duck.a0;
	}
	if(duck.pt.y>=600)
		life=0;
	//检测道具是否拾取
	for(i=0;life&&i<Num&&!duck.fy;i++)
	{
		if(block[i].prop>2)
		{
			k=block[i].prop-3;
			if(duck.pt.x>=block[i].pt.x-30&&duck.pt.x<=block[i].pt.x+20&&duck.pt.y<=block[i].pt.y-40&&duck.pt.y>=block[i].pt.y-Prop_take[k].bm.bmHeight-70)
			{
				switch(k)
				{
				case 0:
					duck.fy=3;
					duck.time=100;
					duck.v0=60;
					duck.sf=0;
					duck.pn=0;
					break;
				case 1:
					duck.fy=1;
					duck.time=80;
					duck.v0=50;
					duck.sf=0;
					duck.pn=0;
					break;
				case 2:
					duck.pn=1;
					duck.ptime=80;
					break;
				case 3:
					duck.sf=1;
					duck.stime=80;
					break;
				}
				block[i].prop=0;
				break;
			}
		}
	}
	//检测是否碰撞怪物
	for(i=0;life&&i<Mn&&!duck.fy;i++)
	{
		j=mstr[i].kind;
		if(j==1)
			k=Monster[1].bm.bmWidth/3;
		else if(j==0)
			k=Monster[0].bm.bmWidth+20;
		else
			k=Monster[j].bm.bmWidth;
		if(duck.pt.x>=mstr[i].pt.x-55&&duck.pt.x<=mstr[i].pt.x+k-45&&duck.pt.y<=mstr[i].pt.y+Monster[j].bm.bmHeight-50&&duck.pt.y>=mstr[i].pt.y-70)
		{
			if(j==0)
			{
				duck.pt.y=560;
				duck.v0=0;
				duck.time=0;
				life=0;
			}
			else if(!duck.pn)
			{
				duck.v0=0;
				duck.time=0;
				life=0;
			}
			else
			{
				for(j=i;j<Mn-1;j++)
					mstr[j]=mstr[j+1];
				Mn--;
			}
		}
	}
	if(duck.sf)
	{
		duck.stime--;
		if(!duck.stime)
			duck.sf=0;
	}
	if(duck.pn)
	{
		duck.ptime--;
		if(!duck.ptime)
			duck.pn=0;
	}
}
void Paint()
{
	int i,j,k,m;
	char s[40];
	POINT Bpt[5];
	HPEN hp;
	HBRUSH hbr;
	SelectObject(hdcm,Bkg[Tn].hbm);
	BitBlt(hdcmem,0,0,Bkg[Tn].bm.bmWidth,Bkg[Tn].bm.bmHeight,hdcm,0,0,SRCCOPY);
	hp=CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdcmem,hp);
	Bpt[0].x=50;
	Bpt[0].y=40;
	Bpt[1].x=450;
	Bpt[1].y=40;
	Bpt[2].x=450;
	Bpt[2].y=530;
	Bpt[3].x=50;
	Bpt[3].y=530;
	Bpt[4].x=50;
	Bpt[4].y=40;
	Polyline(hdcmem,Bpt,5);
	DeleteObject(hp);
	for(i=0;i<Mn;i++)
	{
		k=mstr[i].kind;
		if(k==1)
		{
			m=Monster[1].bm.bmWidth/3;
			j=int(M[i])%3*m;
			M[i]+=0.6;
		}
		else
		{
			m=0;
			j=0;
		}
		SelectObject(hdcm,Monster[k].chm);
		BitBlt(hdcmem,mstr[i].pt.x,mstr[i].pt.y,Monster[k].bm.bmWidth-2*m,Monster[k].bm.bmHeight,hdcm,j,0,SRCAND);
		SelectObject(hdcm,Monster[k].hbm);
		BitBlt(hdcmem,mstr[i].pt.x,mstr[i].pt.y,Monster[k].bm.bmWidth-2*m,Monster[k].bm.bmHeight,hdcm,j,0,SRCPAINT);
	}
	for(i=0;i<Num;i++)
	{
		if(block[i].kind<=2&&block[i].prop)
		{
			k=block[i].prop-1;
			if(k<=1)
			{
				j=block[i].b;
				SelectObject(hdcm,Prop_help[k][j].chm);
				BitBlt(hdcmem,block[i].pt.x+20,block[i].pt.y-Prop_help[k][j].bm.bmHeight+5,Prop_help[k][j].bm.bmWidth,Prop_help[k][j].bm.bmHeight,hdcm,0,0,SRCAND);
				SelectObject(hdcm,Prop_help[k][j].hbm);
				BitBlt(hdcmem,block[i].pt.x+20,block[i].pt.y-Prop_help[k][j].bm.bmHeight+5,Prop_help[k][j].bm.bmWidth,Prop_help[k][j].bm.bmHeight,hdcm,0,0,SRCPAINT);
			}
			else
			{
				k-=2;
				SelectObject(hdcm,Prop_take[k].chm);
				BitBlt(hdcmem,block[i].pt.x+20,block[i].pt.y-Prop_take[k].bm.bmHeight+5,Prop_take[k].bm.bmWidth,Prop_take[k].bm.bmHeight,hdcm,0,0,SRCAND);
				SelectObject(hdcm,Prop_take[k].hbm);
				BitBlt(hdcmem,block[i].pt.x+20,block[i].pt.y-Prop_take[k].bm.bmHeight+5,Prop_take[k].bm.bmWidth,Prop_take[k].bm.bmHeight,hdcm,0,0,SRCPAINT);
			}
		}
		switch(block[i].kind)
		{
		case 0:
			SelectObject(hdcm,Blue.chm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,Blue.bm.bmWidth,Blue.bm.bmHeight,hdcm,0,0,SRCAND);
			SelectObject(hdcm,Blue.hbm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,Blue.bm.bmWidth,Blue.bm.bmHeight,hdcm,0,0,SRCPAINT);
			if(block[i].pt.x==50)
				block[i].OP=1;
			else if(block[i].pt.x==370)
				block[i].OP=-1;
			block[i].pt.x+=block[i].OP*5;
			break;
		case 1:
			SelectObject(hdcm,Gray.chm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,Gray.bm.bmWidth,Gray.bm.bmHeight,hdcm,0,0,SRCAND);
			SelectObject(hdcm,Gray.hbm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,Gray.bm.bmWidth,Gray.bm.bmHeight,hdcm,0,0,SRCPAINT);
			if(block[i].pt.y==block[i].yl)
				block[i].OP=-1;
			else if(block[i].pt.y==block[i].yh)
				block[i].OP=1;
			block[i].pt.y+=block[i].OP*5;
			if(block[i].pt.y>510)
			{
				for(j=i;j<Num-1;j++)
					block[j]=block[j+1];
				Num--;
				i--;
			}
			break;
		case 2:
			SelectObject(hdcm,Green.chm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,Green.bm.bmWidth,Green.bm.bmHeight,hdcm,0,0,SRCAND);
			SelectObject(hdcm,Green.hbm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,Green.bm.bmWidth,Green.bm.bmHeight,hdcm,0,0,SRCPAINT);
			break;
		case 3:
			SelectObject(hdcm,White.chm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,White.bm.bmWidth,White.bm.bmHeight,hdcm,0,0,SRCAND);
			SelectObject(hdcm,White.hbm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,White.bm.bmWidth,White.bm.bmHeight,hdcm,0,0,SRCPAINT);
			break;
		case 4:
			k=(int)block[i].n;
			SelectObject(hdcm,Time[k].chm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,Time[k].bm.bmWidth,Time[k].bm.bmHeight,hdcm,0,0,SRCAND);
			SelectObject(hdcm,Time[k].hbm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,Time[k].bm.bmWidth,Time[k].bm.bmHeight,hdcm,0,0,SRCPAINT);
			if(block[i].pt.y>220)
				block[i].n+=0.2;
			if(k==7)
			{
				for(j=i;j<Num-1;j++)
					block[j]=block[j+1];
				Num--;
				i--;
			}
			break;
		case 5:
			k=(int)block[i].n;
			SelectObject(hdcm,Brown[k].chm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,Brown[k].bm.bmWidth,Brown[k].bm.bmHeight,hdcm,0,0,SRCAND);
			SelectObject(hdcm,Brown[k].hbm);
			BitBlt(hdcmem,block[i].pt.x,block[i].pt.y,Brown[k].bm.bmWidth,Brown[k].bm.bmHeight,hdcm,0,0,SRCPAINT);
			if(block[i].broke)
			{
				block[i].n+=0.4;
				if(k==3)
				{
					broke[Bn]=block[i].pt;
					Bn++;
					for(j=i;j<Num-1;j++)
						block[j]=block[j+1];
					Num--;
					i--;
				}
			}
			break;
		}
	}
	for(i=0;i<Bn;i++)
	{
		SelectObject(hdcm,Brown[3].chm);
		BitBlt(hdcmem,broke[i].x,broke[i].y,Brown[3].bm.bmWidth,Brown[3].bm.bmHeight,hdcm,0,0,SRCAND);
		SelectObject(hdcm,Brown[3].hbm);
		BitBlt(hdcmem,broke[i].x,broke[i].y,Brown[3].bm.bmWidth,Brown[3].bm.bmHeight,hdcm,0,0,SRCPAINT);
		broke[i].y+=15;
		if(broke[i].y>520)
		{
			for(j=i;j<Bn-1;j++)
				broke[j]=broke[j+1];
			Bn--;
			i--;
		}
	}
	if(duck.pn)
	{
		//透明半透明贴图处理
		int xstart,ystart,xend,yend,y=0;
		xstart=duck.pt.x-18;
		if(duck.sf)
			ystart=duck.pt.y-30;
		else
			ystart=duck.pt.y-18;
		if(ystart<0)
		{
			y=-ystart;
			ystart=0;
		}
		BitBlt(bufdc,0,y,120,120-y,hdcmem,xstart,ystart,SRCCOPY);
		SelectObject(hdcm,Chpr);
		BitBlt(bufdc,0,y,120,120,hdcm,0,y,SRCAND);
		SelectObject(hdcm,hpr);
		BitBlt(bufdc,0,y,120,120,hdcm,0,y,SRCPAINT);
		BITMAP bm1,bm2;
		unsigned char *px1,*px2;
		int pxbit1,pxbit2;
		DWORD rgb;
		GetObject(hBm,sizeof(BITMAP),(LPVOID)&bm1);
		pxbit1=bm1.bmBitsPixel/8;
		rgb=bm1.bmHeight*bm1.bmWidthBytes;
		px1=new unsigned char[rgb];
		GetBitmapBits(hBm,rgb,px1);
		xend=xstart+120;
		yend=ystart+120-y;
		for(i=ystart;i<yend;i++)
			for(j=xstart;j<xend;j++)
			{
				rgb=(i*bm1.bmWidth+j)*pxbit1;
				px1[rgb]=px1[rgb]/2;
				px1[rgb+1]=px1[rgb+1]/2;
				px1[rgb+2]=px1[rgb+2]/2;
			}
		GetObject(hbk,sizeof(BITMAP),(LPVOID)&bm2);
		pxbit2=bm2.bmBitsPixel/8;
		rgb=bm2.bmHeight*bm2.bmWidthBytes;
		px2=new unsigned char[rgb];
		GetBitmapBits(hbk,rgb,px2);
		for(i=y;i<120;i++)
			for(j=0;j<120;j++)
			{
				rgb=(i*bm2.bmWidth+j)*pxbit2;
				k=((ystart+i-y)*bm1.bmWidth+xstart+j)*pxbit1;
				px2[rgb]=px2[rgb]/2+px1[k];
				px2[rgb+1]=px2[rgb+1]/2+px1[k+1];
				px2[rgb+2]=px2[rgb+2]/2+px1[k+2];
			}
		SetBitmapBits(hbk,bm2.bmHeight*bm2.bmWidthBytes,px2);
		BitBlt(hdcmem,xstart,ystart,120,120-y,bufdc,0,y,SRCCOPY);
		delete []px1;
		delete []px2;
	}
	k=duck.pt.y-25*duck.sf;
	if(life&&atk&&!duck.fy)
	{
		SelectObject(hdcm,Attack.chm);
		BitBlt(hdcmem,duck.pt.x,k,Attack.bm.bmWidth,Attack.bm.bmHeight,hdcm,0,0,SRCAND);
		SelectObject(hdcm,Attack.hbm);
		BitBlt(hdcmem,duck.pt.x,k,Attack.bm.bmWidth,Attack.bm.bmHeight,hdcm,0,0,SRCPAINT);
		for(i=0;i<Mn;i++)
			if(mstr[i].kind&&k>mstr[i].pt.y+20)
			{
				bt[Ln].kind=i;
				break;
			}
		bt[Ln].pt.x=duck.pt.x+35;
		bt[Ln].pt.y=k-20;
		if(i<Mn)
		{
			bt[Ln].m=(k-mstr[i].pt.y-20)/60+1;
			j=mstr[i].pt.x+20-bt[Ln].pt.x;
			bt[Ln].v=j/bt[Ln].m;
			if(j>0)
				bt[Ln].v++;
			else if(j<0)
				bt[Ln].v--;
		}
		else
		{
			bt[Ln].m=0;
			bt[Ln].kind=-1;
			bt[Ln].v=0;
		}
		Ln++;
	}
	else
	{
		SelectObject(hdcm,Duck[duck.fy][duck.Dn].chm);
		BitBlt(hdcmem,duck.pt.x,k,Duck[duck.fy][duck.Dn].bm.bmWidth,Duck[duck.fy][duck.Dn].bm.bmHeight,hdcm,0,0,SRCAND);
		SelectObject(hdcm,Duck[duck.fy][duck.Dn].hbm);
		BitBlt(hdcmem,duck.pt.x,k,Duck[duck.fy][duck.Dn].bm.bmWidth,Duck[duck.fy][duck.Dn].bm.bmHeight,hdcm,0,0,SRCPAINT);
	}
	if(atk)
		atk=0;
	//画子弹
	for(i=0;i<Ln;i++)
	{
		SelectObject(hdcm,Bullet.chm);
		BitBlt(hdcmem,bt[i].pt.x,bt[i].pt.y,Bullet.bm.bmWidth,Bullet.bm.bmHeight,hdcm,0,0,SRCAND);
		SelectObject(hdcm,Bullet.hbm);
		BitBlt(hdcmem,bt[i].pt.x,bt[i].pt.y,Bullet.bm.bmWidth,Bullet.bm.bmHeight,hdcm,0,0,SRCPAINT);
	}
	if(duck.sf)
	{
		k+=75;
		SelectObject(hdcm,Prop_take[3+duck.Dn].chm);
		BitBlt(hdcmem,duck.pt.x+22,k,Prop_take[3].bm.bmWidth,Prop_take[3].bm.bmHeight,hdcm,0,0,SRCAND);
		SelectObject(hdcm,Prop_take[3+duck.Dn].hbm);
		BitBlt(hdcmem,duck.pt.x+22,k,Prop_take[3].bm.bmWidth,Prop_take[3].bm.bmHeight,hdcm,0,0,SRCPAINT);
	}
    if(duck.fy>=3)
		duck.fy=duck.fy%2+3;
	else if(duck.fy>=1)
		duck.fy=duck.fy%2+1;
	if(show<Score)
	{
		if(duck.time)
			show+=(duck.v0-1);
		else
			show+=19;
	}
	sprintf(s,"%d",show);
	j=52;
	for(i=0;s[i]!='\0';i++)
	{
		k=s[i]-48;
		SelectObject(hdcm,Number[k].chm);
		BitBlt(hdcmem,j,40,Number[k].bm.bmWidth,Number[k].bm.bmHeight,hdcm,0,0,SRCAND);
		SelectObject(hdcm,Number[k].hbm);
		BitBlt(hdcmem,j,40,Number[k].bm.bmWidth,Number[k].bm.bmHeight,hdcm,0,0,SRCPAINT);
		j+=Number[k].bm.bmWidth;
	}
	if(duck.pt.y>=600)
	{
		if(BTn)
			m=3;
		else
			m=Tn;
		if(mode[m]<show)
		{
			mode[m]=show;
			fp=fopen("Record","wb");
			fwrite(mode,sizeof(int),4,fp);//写记录
			fclose(fp);
		}
		hp=CreatePen(PS_SOLID,0,RGB(0,0,0));
		hbr=CreateSolidBrush(RGB(160,215,100));
		SelectObject(hdcmem,hp);
		SelectObject(hdcmem,hbr);
		RoundRect(hdcmem,130,120,380,360,10,10);
		DeleteObject(hp);
		DeleteObject(hbr);
		SelectObject(hdcmem,hf);
		sprintf(s,"Score:  %d",show);
		TextOut(hdcmem,150,140,s,strlen(s));
		sprintf(s,"Record: %d",mode[m]);
		TextOut(hdcmem,150,180,s,strlen(s));
		U[16].SetShow(1);
		U[17].SetShow(1);
		for(i=16,k=0;i<=17;i++)
		{
			U[i].SetPoint(230+k*70);
			U[i].PaintUI();
			k++;
		}
	}
	BitBlt(hdc,0,0,500,1000,hdcmem,0,0,SRCCOPY);
}