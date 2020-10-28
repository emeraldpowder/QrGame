#include <windows.h>

//#pragma comment(linker,"/merge:.rdata=.data")
//#pragma comment(linker,"/merge:.text=.data")

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 640

extern int _fltused = 0;

struct point {int x; int y;};

struct point pos = {WINDOW_WIDTH/2*100,WINDOW_HEIGHT/2*100};
struct point vel = {0,0};

struct point obstacles[1000];
int obstacles_count = 0;

struct point collectibles[1000];
int collectibles_count = 1;

int scroll=0;
int speed=1000;
int score=0;
int dead=0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Update();
void DrawPixels(HWND hwnd);

void Die()
{
	dead=1;
	MessageBoxA(NULL, "Game over", "gg", MB_OK);
	PostQuitMessage(0);
	ExitProcess(0);
}

#define FLIP(p) if(flip)(p).x=WINDOW_WIDTH-(p).x

void SpawnChunk()
{
	int i, index = (pos.x*5+pos.y*4+obstacles_count+scroll) % 9;
	int flip = (pos.x+pos.y*3+scroll*5+obstacles_count) % 2;
	int sc = scroll/10;
	struct point p;

	switch (index)
	{
	case 0:
		p.x = WINDOW_WIDTH/4;
		p.y = -sc;
		obstacles[obstacles_count++] = p;
		p.x = WINDOW_WIDTH*3/4;
		p.y = -sc;
		obstacles[obstacles_count++] = p;
		p.x = WINDOW_WIDTH/4;
		p.y = -sc-30;
		obstacles[obstacles_count++] = p;
		p.x = WINDOW_WIDTH*3/4;
		p.y = -sc-30;
		obstacles[obstacles_count++] = p;
		p.x = WINDOW_WIDTH/4;
		p.y = -sc-60;
		obstacles[obstacles_count++] = p;
		p.x = WINDOW_WIDTH*3/4;
		p.y = -sc-60;
		obstacles[obstacles_count++] = p;
		
		p.x = WINDOW_WIDTH/2;
		p.y = -sc-30;
		collectibles[collectibles_count++] = p;
		break;

	case 1:
		p.x = WINDOW_WIDTH/2;
		p.y = -sc;
		obstacles[obstacles_count++] = p;
		p.x = WINDOW_WIDTH/2;
		p.y = -sc-30;
		obstacles[obstacles_count++] = p;
		p.x = WINDOW_WIDTH/2;
		p.y = -sc-60;
		obstacles[obstacles_count++] = p;

		
		p.x = WINDOW_WIDTH*3/4;
		p.y = -sc-30;
		collectibles[collectibles_count++] = p;
		p.x = WINDOW_WIDTH/4;
		p.y = -sc-30;
		collectibles[collectibles_count++] = p;
		break;

	case 2:
		p.x = WINDOW_WIDTH/4;
		p.y = -sc;
		FLIP(p);
		obstacles[obstacles_count++] = p;
		p.x = WINDOW_WIDTH/2;
		p.y = -sc;
		FLIP(p);
		obstacles[obstacles_count++] = p;
		p.x = WINDOW_WIDTH*3/4;
		p.y = -sc;
		FLIP(p);
		obstacles[obstacles_count++] = p;
		break;

	case 3:
		for (i=0;i<8;i++)
		{
			p.x = WINDOW_WIDTH*(1+i)/10;
			p.y = -sc-i*20;
			obstacles[obstacles_count++] = p;
		}
		break;

	case 4:
		for (i=0;i<6;i++)
		{
			p.x = WINDOW_WIDTH*(1+i)/10;
			p.y = -sc;
			FLIP(p);
			obstacles[obstacles_count++] = p;
		}

		p.x = WINDOW_WIDTH*4/5;
		p.y = -sc;
		FLIP(p);
		collectibles[collectibles_count++] = p;
		break;

	case 5:
		for (i=0;i<5;i++)
		{
			p.x = WINDOW_WIDTH/4;
			p.y = -sc-20*i;
			FLIP(p);
			obstacles[obstacles_count++] = p;
		}

		p.x = WINDOW_WIDTH*3/4;
		p.y = -sc-50;
		FLIP(p);
		collectibles[collectibles_count++] = p;
		break;

	case 6:
		for (i=0;i<5;i++)
		{
			p.x = WINDOW_WIDTH*(1+i)/6;
			p.y = -sc;
			obstacles[obstacles_count++] = p;

			p.y -= 40;
			collectibles[collectibles_count++] = p;
		}
		break;

	case 7:
		for (i=0;i<6;i++)
		{
			p.x = WINDOW_WIDTH*(5+i)/15;
			p.y = -sc;
			obstacles[obstacles_count++] = p;

			p.y -= 180;
			obstacles[obstacles_count++] = p;

			p.y += 100;
			collectibles[collectibles_count++] = p;
		}
		break;

	case 8:
		for (i=0;i<16;i++)
		{
			p.x = WINDOW_WIDTH*(10+i*5)/100;
			p.y = -sc - (i%4)*40;
			FLIP(p);
			obstacles[obstacles_count++] = p;
		}

		p.x = WINDOW_WIDTH/2+10;
		p.y = -sc-60;
		FLIP(p);
		collectibles[collectibles_count++] = p;
		break;
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PWSTR lpCmdLine, int nCmdShow) {

	MSG  msg;
	struct point o;
	WNDCLASSW wc = { 0 };

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"o";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetStockObject(BLACK_BRUSH);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	o.x = WINDOW_WIDTH/2;
	o.y = 100;
	collectibles[0] = o;

	RegisterClassW(&wc);
	CreateWindowW(wc.lpszClassName, L"o",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	SpawnChunk();

	while (GetMessage(&msg, NULL, 0, 0) > 0) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	switch (msg) {
    case WM_CREATE: 

        SetTimer(hwnd, 1, 20, NULL); 
		break;

	case WM_PAINT:

		DrawPixels(hwnd);
		break;

    case WM_ERASEBKGND:
        return 1; // Say we handled it.
 
    case WM_TIMER: 

		Update();
		RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
		break;
 
    case WM_LBUTTONDOWN:

		vel.x = (int)((short)(lParam & 0xFFFF))*1000/WINDOW_WIDTH-500;
		vel.y = -700*(pos.y)/(WINDOW_HEIGHT*100);

		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		ExitProcess(0);
		pos.x=0;
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void Update()
{
	int oldScroll;
	if (dead) return;

	pos.x += vel.x;
	if(pos.x<0)pos.x=0;
	if(pos.x>WINDOW_WIDTH*100-1200)pos.x=WINDOW_WIDTH*100-1200;
	pos.y += vel.y;
	vel.y += 10;
	vel.x = vel.x*99/100;
	if (speed<4000) speed++;

	oldScroll = scroll;
	scroll += speed/100;
	//if(pos.y*100<WINDOW_HEIGHT/2)scroll += 
	if (scroll / 3000 != oldScroll / 3000) SpawnChunk();

	if (pos.y > WINDOW_HEIGHT*100) Die();
}

int distance_squared(struct point a, struct point b)
{
	return (a.x-b.x/100)*(a.x-b.x/100)+(a.y+scroll/10-b.y/100)*(a.y+scroll/10-b.y/100);
}

void DrawPixels(HWND hwnd)
{
	PAINTSTRUCT ps;
	RECT r;
	int i;
	HDC hdc;
    HBRUSH hbrBkGnd;
    HBITMAP hbmMem, hbmOld;
    char score_text[3];

	if (dead) return;

	GetClientRect(hwnd, &r);

	BeginPaint(hwnd, &ps);

    hdc = CreateCompatibleDC(ps.hdc);

	hbmMem = CreateCompatibleBitmap(ps.hdc, WINDOW_WIDTH, WINDOW_HEIGHT);

    //
    // Select the bitmap into the off-screen DC.
    //

    hbmOld = SelectObject(hdc, hbmMem);

    //
    // Erase the background.
    //

    hbrBkGnd = GetStockObject(BLACK_BRUSH);
    FillRect(hdc, &r, hbrBkGnd);
    DeleteObject(hbrBkGnd);

    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH)); 

    SelectObject(hdc, CreatePen(PS_DOT,1,RGB(255,255,224)));
	Ellipse(hdc, pos.x/100, pos.y/100, pos.x/100+12, pos.y/100+12);

    SelectObject(hdc, CreatePen(PS_DOT,2,RGB(255,69,0)));
	for (i = 0; i < obstacles_count; i++)
	{
		Rectangle(hdc, obstacles[i].x, obstacles[i].y+scroll/10, obstacles[i].x+10, obstacles[i].y+scroll/10+10);
		if (distance_squared(obstacles[i], pos) < 13*13)
		{
			Die();
		}
	}

    SelectObject(hdc, GetStockObject(NULL_PEN)); 
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(60,179,113));
	for (i = 0; i < collectibles_count; i++)
	{
		Ellipse(hdc, collectibles[i].x, collectibles[i].y+scroll/10, collectibles[i].x+12, collectibles[i].y+scroll/10+12);
		if (distance_squared(collectibles[i], pos) < 20*20)
		{
			score++;
			collectibles[i].x = -100;
		}
	}
	
    SetDCBrushColor(hdc, RGB(255,255,224));
	Ellipse(hdc, pos.x/100+3, pos.y/100+3, pos.x/100+10, pos.y/100+10);

	score_text[0] = score/100%10 + '0';
	if (score_text[0] == '0') score_text[0]=' ';
	score_text[1] = score/10%10 + '0';
	if (score_text[0] == ' ' && score_text[1] == '0') score_text[1]=' ';
	score_text[2] = score%10 + '0';
	
	SetBkColor(hdc, RGB(0,0,0));
	SetTextColor(hdc, RGB(60,179,113));
	TextOutA(hdc, 5,5,score_text,3);

    //
    // Blt the changes to the screen DC.
    //

    BitBlt(ps.hdc,
           0, 0,
           WINDOW_WIDTH, WINDOW_HEIGHT,
           hdc,
           0, 0,
           SRCCOPY);

    //
    // Done with off-screen bitmap and DC.
    //

    SelectObject(hdc, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdc);

	EndPaint(hwnd, &ps);
}