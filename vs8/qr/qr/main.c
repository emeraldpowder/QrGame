#include <windows.h>

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 640

/*int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MessageBox(NULL, "Goodbye, cruel world!", "Note", MB_OK);
	return 0;
}*/

extern int _fltused = 0;

static int pos_x=10;
static int pos_y=10;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawPixels(HWND hwnd);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PWSTR lpCmdLine, int nCmdShow) {

	MSG  msg;
	WNDCLASSW wc = { 0 };

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"o";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetStockObject(BLACK_BRUSH);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

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

        SetTimer(hwnd, 1, 10, NULL); 
		break;

	case WM_PAINT:

		DrawPixels(hwnd);
		break;

    case WM_ERASEBKGND:
        return (LRESULT)1; // Say we handled it.
 
    case WM_TIMER: 

		pos_x += 1;
		RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void DrawPixels(HWND hwnd) {

	PAINTSTRUCT ps;
	RECT r;
	int i;
	HDC hdc;
    HBITMAP hbmMem, hbmOld;

	GetClientRect(hwnd, &r);

	if (r.bottom == 0) {

		return;
	}

	hdc = BeginPaint(hwnd, &ps);

	for (i = 0; i < 1000; i++) {

		int x = (i*451613) % r.right;
		int y = (i*116843) % r.bottom;
		SetPixel(hdc, x, y, RGB(255, 0, 0));
	}

    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH)); 
    SelectObject(hdc, GetStockObject(WHITE_PEN)); 
	Ellipse(hdc, pos_x, pos_y, pos_x+10, pos_y+10);

	EndPaint(hwnd, &ps);
}