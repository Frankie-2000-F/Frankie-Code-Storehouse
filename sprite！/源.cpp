#include"acllib.h"
#include<math.h>
#include<time.h>
#include<stdio.h>


class SpriteBase
{
protected:
	int x, y;
	int width, height;
	ACL_Image *img;
	rect r;
public:
	int dx, dy;
	SpriteBase(int x, int y, int width, int height, int dx, int dy, ACL_Image *img, rect r1);
	virtual ~SpriteBase();

	virtual void move(rect r) = 0;
	void Draw(int width, int height);
	void Draw();
	rect getRect();
};
SpriteBase::SpriteBase(int x, int y, int width, int height, int dx, int dy, ACL_Image *img, rect r1)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->dx = dx;
	this->dy = dy;
	this->img = img;
	r = r1;
}

SpriteBase::~SpriteBase()
{

}
void SpriteBase::Draw(int width, int height)
{
	putImageScale(img, x, y, width, height);
}
void SpriteBase::Draw()
{
	putImageScale(img, x, y, width, height);
}
rect SpriteBase::getRect()
{
	rect r = { x, y, width, height };
	return r;
}


class UsrSprite :public SpriteBase
{
	int score;
public:
	UsrSprite(int x, int y, int width, int height, int dx, int dy, ACL_Image *img, rect r1);

	~UsrSprite();

	void move(int key);
	void move(rect r);

	int collision(rect r1);
	int getScore();
	void addScore(int a);
	void setScore(int s);
	
};

UsrSprite::UsrSprite(int x, int y, int width, int height, int dx, int dy, ACL_Image *img, rect r1)
:SpriteBase(x, y, width, height, dx, dy, img, r1)
{

}
UsrSprite::~UsrSprite(){}
void UsrSprite::move(rect r1)
{
	x = r1.x;
	y = r1.y;
	if (x < r.x)x = r.x;
	if (x >(r.x + r.width - width))x = (r.x + r.width - width);
	if (y < r.y)y = r.y;
	if (y >(r.y + r.height - height))y = (r.y + r.height - height);

}
void UsrSprite::move(int key)
{
	switch (key)
	{
	case VK_UP:
		y -= dy;
		if (y < 0) y = 0;
		break;
	case VK_DOWN:
		y += dy;
		if (y >(r.height - height)) y = (r.height - height);

		break;
	case VK_LEFT:
		x -= dx;
		if (x < r.x)x = r.x;
		break;
	case VK_RIGHT:
		x += dx;
		if (x >(r.width - width)) x = (r.width - width);
		break;
	}
}
int UsrSprite::collision(rect r2)
{
	rect r1 = { x, y, width, height };
	int c = 1;
	if (r1.x < r2.x && r1.x + r1.width >r2.x) {
		if (r1.y > r2.y && r1.y<r2.y + r2.height)return c;
		if (r1.y <r2.y && r1.y + r1.height >r2.y)return c;
	}
	else {
		if (r1.x > r2.x && r2.x + r2.width > r1.x)
		{
			if (r1.y > r2.y && r1.y<r2.y + r2.height)return c;
			if (r1.y <r2.y && r1.y + r1.height >r2.y)return c;
		}
	}
	c = 0;
	return c;
}
int UsrSprite::getScore()
{
	return score;
}
void UsrSprite::addScore(int a)
{
	this->score += a;
}
void UsrSprite::setScore(int s)
{
	score = s;
}



class AutoSprite :
	public SpriteBase
{
	int score;
public:
	AutoSprite(int x, int y, int width, int height, int dx, int dy, ACL_Image *img, rect r1, int score);
	AutoSprite(AutoSprite &sprite);
	~AutoSprite();
	void doubleScore();

	void move(rect r);
	int getScore();
};

AutoSprite::AutoSprite(int x, int y, int width, int height, int dx, int dy, ACL_Image *img, rect r1, int score) :SpriteBase(x, y, width, height, dx, dy, img, r1)
{
	this->score = score;
}


AutoSprite::~AutoSprite()
{
}
void AutoSprite::move(rect r1)
{

	if (x>(r.width - width)||x<0) dx *= -1;
	if (y>(r.height - height) || y<0) dy *= -1;
	x += dx;
	y += dy;

}
int AutoSprite::getScore()
{
	return score;
}
void AutoSprite::doubleScore()
{
	this->score *= 2;
}


//define AvoidSprite
class Avoidsprite :public AutoSprite
{
	bool close(rect ur);
public:
	Avoidsprite(int x, int y, int width, int height, int dx, int dy, ACL_Image *img, rect r1, int score);
	Avoidsprite(Avoidsprite &sprite);
	virtual ~Avoidsprite();

	void move(rect ur);
};

Avoidsprite::~Avoidsprite()
{
}
Avoidsprite::Avoidsprite(int x, int y, int width, int height, int dx, int dy, ACL_Image *img, rect r1, int score)
:AutoSprite(x, y, width, height, dx, dy, img, r1, score){}
void Avoidsprite::move(rect ur)
{
	if (close(ur))
	{
		if (x < ur.x) dx = -abs(dx);
		else dx = abs(dx);

		if (y < ur.y) dy = -abs(dy);
		else dy = abs(dy);

		x += 1.5*dx;
		y += 1.5*dy;
	}
	else
	{
		x += dx;
		y += dy;
	}

	if ((dx<0) && (x<-width / 2)) x = r.width - width / 2;
	if ((dx>0) && (x>(r.width - width / 2))) x = -width / 2;
	if ((dy<0) && (y<-height / 2)) y = r.height - height / 2;
	if ((dy>0) && (y>(r.height - height / 2))) y = -height / 2;

}
bool Avoidsprite::close(rect ur)
{
	int cx = x + width / 2;
	int cy = y + height / 2;
	int ux = ur.x + ur.width / 2;
	int uy = ur.y + ur.height / 2;
	float dist = sqrt(float((cx - ux)*(cx - ux) + (cy - uy)*(cy - uy)));
	if (dist < 150) return true;
	else return false;
}


class Tool :public AutoSprite
{
public:
	Tool(int x, int y, int width, int height, int dx, int dy, ACL_Image *img, rect r1, int score);
	void SpeedUp(UsrSprite *usr);
	void DoubleScore(int nowNum, AutoSprite *autosprite[]);
	virtual ~Tool();

	void move(rect ur);
};
Tool::~Tool()
{
}
Tool::Tool(int x, int y, int width, int height, int dx, int dy, ACL_Image *img, rect r1, int score)
:AutoSprite(x, y, width, height, dx, dy, img, r1, score)
{
}
void Tool::move(rect ur) 
{
	if (x<0 || x>(r.width - width)) dx *= -1;
	if (y<0 || y>(r.height - height)) dy *= -1;
    x += dx;
	y += dy;
}
void Tool::SpeedUp(UsrSprite *usr)
{
	usr->dx *= 2;
	usr->dy *=2;
	startTimer(3, 10000);
}
void Tool::DoubleScore(int nowNum,AutoSprite *autosprite[])
{
	for (int i = 0; i < nowNum; ++i)
	{
		if (autosprite[i])
		{
			autosprite[i]->doubleScore();
		}
	}
}

const int maxNum = 100;
const int winW = 800, winH = 600;
AutoSprite *autosprite[maxNum] = { 0 };
UsrSprite *usr = NULL;
Tool *tools[2] = {0};
int autoW = 100, autoH = 100;
int usrW = 100, usrH = 100;
int remainTime = 120;
ACL_Image img, imgCat, imgDuck,imgsUp,imgDS;
rect winr;
void timerEvent(int id);
void newData(AutoSprite **autosprite);
void newData(UsrSprite **usr);
void newData(Tool **tools);
void getHighScore(int* highscore);
void storeHighScore(int highscore);
void keyEvent(int key, int event);
void paint();
void resetUsr();
int nowNum = 0;
int end = 0;
int highscore;
int caughtNum=0;



int Setup()
{
	
		winr.y = DEFAULT;
		winr.x = DEFAULT;
		winr.height = winH;
		winr.width = winW;
		initWindow("æ´¡È”Œœ∑", winr.x, winr.y, winW, winH);
		srand((unsigned)time(NULL));
		getHighScore(&highscore);
		loadImage("tom.bmp", &imgCat);
		loadImage("jerry.bmp", &img);
		loadImage("duck.jpg", &imgDuck);
		loadImage("speedUp.jpg", &imgsUp);
		loadImage("doubleScore.jpg", &imgDS);

		newData(autosprite);
		newData(&usr);
		registerKeyboardEvent(keyEvent);
		registerTimerEvent(timerEvent);
		startTimer(1, 1000);
		startTimer(0, 40);
		startTimer(2, 120000);
	
	return 0;
}
void getHighScore(int * highscore)
{
	FILE * fp = NULL; 
	fopen_s(&fp, "highscore.txt", "r");

	if (fp == NULL)
	{
		fopen_s(&fp, "highscore.txt", "w");
		fprintf(fp, "%-10d", *highscore);
	}
	else
		fscanf_s(fp, "%d", highscore);

	fclose(fp);
}
void storeHighScore(int highscore)
{
	FILE * fp = NULL;
	errno_t err;
	err = fopen_s(&fp,"highscore.txt", "w");
	fprintf(fp, "%-10d", highscore);
	fclose(fp);
}
void timerEvent(int id)
{
	int i = 0;
	if (!end)
	{
		switch (id)
		{
		case 0:
			if (caughtNum == maxNum){ end = 1; break; }
			for (i = 0; i < nowNum; ++i)
			if (autosprite[i])
			{
				rect ur = usr->getRect();
				autosprite[i]->move(ur);

			}
			for (i = 0; i < 2; ++i)
			if (tools[i])
			{
				rect ur = usr->getRect();
				tools[i]->move(ur);

			}

			break;
		case 1:
			if (nowNum < maxNum)
			{
				newData(autosprite);
			}
			newData(tools);
			remainTime--;
			break;
		case 2:
			end = 1;
			break;
		case 3:
			resetUsr();
			break;
		}
		paint();
	}
}
void newData(AutoSprite **autosprite)
{
	int x = rand() % winW - autoW;
	if (x < 0)x = 0;
	int y = rand() % winH - autoH;
	if (y < 0)y = 0;
	int dx = rand() % 4 + 2;
	int dy = rand() % 4 + 2;
	int t = rand() % 100;
	if (t>=70)
		autosprite[nowNum++] = new Avoidsprite(x, y, autoW, autoH, dx, dy, &imgDuck, winr, 5);
	else
		autosprite[nowNum++] = new AutoSprite(x, y, autoW, autoH, dx, dy, &img, winr, 1);
}
void newData(UsrSprite **usr)
{
	int x = rand() % winW - autoW;
	int y = rand() % winH - autoH;
	if (x < 0)x = 0;
	if (y < 0)y = 0;
	int dx = 4;
	int dy = 4;
	*usr = new UsrSprite(x, y, usrW, usrH, dx, dy, &imgCat, winr);

}
void newData(Tool **tools)
{
	int x = rand() % winW - autoW;
	if (x < 0)x = 0;
	int y = rand() % winH - autoH;
	if (y < 0)y = 0;
	int dx = rand() % 5 + 1;
	int dy = rand() % 5 + 1;
	int t = rand() % 100;
	if (t<8)
		tools[0] = new Tool(x, y, autoW, autoH, dx, dy, &imgsUp, winr, 0);
	else if (t>92)
		tools[1] = new Tool(x, y, autoW, autoH, dx, dy, &imgDS, winr, 0);
}
void paint()
{
	beginPaint();
	clearDevice();
	int i = 0;
	if (end)
	{
		char go[11] = "Game Over!";
		setTextSize(100);
		setTextColor(255);
		paintText(160, 100, go);
		if (usr->getScore() < highscore)
		{
			char hs[4];
			char ms[4];
			char txt1[15] = "Highest Score:";
			char txt2[15] = "Your Score:";
			setTextSize(50);
			paintText(200, 350, txt1);
			paintText(200, 250, txt2);
			sprintf_s(hs, "%d", highscore);
			paintText(600, 350, hs);
			sprintf_s(ms, "%d", usr->getScore());
			paintText(500, 250, ms);
		}
		else
		{
			char txt[20] = "Record-breaking!";
			char ms[4];
			setTextSize(50);
			setTextColor(255);
			paintText(200, 280, txt);
			sprintf_s(ms, "%d", usr->getScore());
			paintText(380, 400, ms);
			storeHighScore(usr->getScore());
		}
	}
	else
	{
		for (i = 0; i < nowNum; ++i)
		{
			if (autosprite[i])
			{
				autosprite[i]->Draw();
			}
		}
		for (i = 0; i < 2; ++i)
		{
			if (tools[i])
			{
				tools[i]->Draw();
			}
		}
		if (usr)
		{
			usr->Draw();

			char txt[10];
			sprintf_s(txt, "%d", usr->getScore());
			setTextSize(20);
			paintText(10, 10, txt);
		}

		if (remainTime)
		{
			char rttxt[50] = "RemainTime:   s";
			paintText(630, 10, rttxt);
			char rt[10];
			sprintf_s(rt, "%d", remainTime);
			paintText(750, 10, rt);
		}
	}
	endPaint();
}
void keyEvent(int key, int event)
{
	if (event != KEY_DOWN)return;
	if (usr)usr->move(key);
	for (int i = 0; i < nowNum; ++i)
	{
		if (autosprite[i])
		{
			if (usr->collision(autosprite[i]->getRect()))
			{
				int s = autosprite[i]->getScore();
				if (usr)usr->addScore(s);
				delete(autosprite[i]);
				autosprite[i] = NULL;
				caughtNum++;
			}
		}
	}
	for (int i = 0; i < 2; ++i)
	{
		if (tools[i])
		{
			if (usr->collision(tools[i]->getRect()))
			{   
				if (i == 0)tools[i]->SpeedUp(usr);
				if (i == 1)tools[i]->DoubleScore(nowNum, autosprite);
				delete(tools[i]);
				tools[i] = NULL;
				
			}
		}
	}
	paint();
}
void resetUsr()
{
	usr->dx = 5;
	usr->dy = 5;
}

