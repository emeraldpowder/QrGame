#include <windows.h>
#include <stdlib.h>

//#pragma comment(linker,"/merge:.rdata=.data")
//#pragma comment(linker,"/merge:.text=.data")

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 640

/*int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpCmdLine, int nCmdShow)
{
   MessageBox(NULL, "Goodbye, cruel world!", "Note", MB_OK);
   return 0;
}*/

extern int _fltused = 0;

struct point {int x; int y;};

struct point pos = {WINDOW_WIDTH/2*100,WINDOW_HEIGHT/2*100};
struct point vel = {0,0};

struct point obstacles[10];
int obstacles_count = 1;

struct point collectibles[10];
int collectibles_count = 1;

int scroll=0;
int score=0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Update();
void DrawPixels(HWND hwnd);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
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

   o.x=100;
   o.y=100;
   obstacles[0] = o;

   o.x=200;
   o.y=200;
   collectibles[0] = o;

   RegisterClassW(&wc);
   CreateWindowW(wc.lpszClassName, L"o",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

   while (GetMessage(&msg, NULL, 0, 0)) {

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
      vel.y = -500;
      //vel.y = -500*(WINDOW_HEIGHT-pos.y/100)*100/WINDOW_HEIGHT;

      break;

   case WM_DESTROY:

      PostQuitMessage(0);
      return 0;
   }

   return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void Update()
{
   pos.x += vel.x;
   pos.y += vel.y;
   vel.y += 10;
   vel.x = vel.x*99/100;

   scroll += 1;

}

int distance_squared(struct point a, struct point b)
{
   return (a.x-b.x/100)*(a.x-b.x/100)+(a.y+scroll-b.y/100)*(a.y+scroll-b.y/100);
}

void DrawPixels(HWND hwnd) {

   PAINTSTRUCT ps;
   RECT r;
   int i;
   HDC hdc;
    HBRUSH hbrBkGnd;
    HBITMAP hbmMem, hbmOld;
    char score_text[3];

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

    SelectObject(hdc, CreatePen(PS_DOT,2,RGB(255,255,255)));
   Ellipse(hdc, pos.x/100, pos.y/100, pos.x/100+10, pos.y/100+10);

    SelectObject(hdc, CreatePen(PS_DOT,2,RGB(255,0,0)));
   for (i = 0; i < obstacles_count; i++)
   {
      Rectangle(hdc, obstacles[i].x, obstacles[i].y+scroll, obstacles[i].x+10, obstacles[i].y+scroll+10);
      if (distance_squared(obstacles[i], pos) < 20*20)
      {
         PostQuitMessage(0);
      }
   }

    SelectObject(hdc, CreatePen(PS_DOT,2,RGB(0,255,0)));
   for (i = 0; i < collectibles_count; i++)
   {
      Ellipse(hdc, collectibles[i].x, collectibles[i].y+scroll, collectibles[i].x+10, collectibles[i].y+scroll+10);
      if (distance_squared(collectibles[i], pos) < 20*20)
      {
         score++;
         collectibles[i].x = -100;
      }
   }

   score_text[0] = score/100%10 + '0';
   score_text[1] = score/10%10 + '0';
   score_text[2] = score%10 + '0';
   
   SetBkColor(hdc, RGB(0,0,0));
   SetTextColor(hdc, RGB(0,255,0));
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