// MathClientGUI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "MathClientGUI.h"
#include <Windows.h>
#include <utility>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

static HWND hwndStartButton;
static HWND hwndClearButton;
static HWND hwndEdit;
static HFONT s_hFont = NULL;
static TCHAR text[] = TEXT("");

COLORREF crText = RGB(51, 255, 51); // text color
COLORREF crBg = RGB(40, 40, 40); // edit control color
static HBRUSH hbrushBg = NULL;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void AppendText(const HWND& hwnd, TCHAR* newText);
void StartButtonClick(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MATHCLIENTGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MATHCLIENTGUI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MATHCLIENTGUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MATHCLIENTGUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            hwndStartButton = CreateWindow(
                TEXT("BUTTON"),  // Predefined class; Unicode assumed 
                TEXT("Start"),      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
                5,         // x position 
                5,         // y position 
                100,        // Button width
                30,        // Button height
                hWnd,     // Parent window
                (HMENU)IDB_START,       // No menu.
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed

            hwndClearButton = CreateWindow(
                TEXT("BUTTON"),  // Predefined class; Unicode assumed 
                TEXT("Clear"),      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD,  // Styles 
                110,         // x position 
                5,         // y position 
                100,        // Button width
                30,        // Button height
                hWnd,     // Parent window
                (HMENU)IDB_CLEAR,       // No menu.
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed

            hwndEdit = CreateWindowEx(
                0,
                TEXT("EDIT"),   // predefined class 
                NULL,         // no window title 
                WS_BORDER| WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                0, 0, 0, 0,   // set size in WM_SIZE message 
                hWnd,         // parent window 
                (HMENU)IDC_TEXT,   // edit control ID 
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);        // pointer not needed
        
            //Font for edit control
            const TCHAR* fontName = TEXT("Courier New");
            const long nFontSize = 10;
            HDC hdc = GetDC(hWnd);
            LOGFONT logFont = { 0 };
            logFont.lfHeight = -MulDiv(nFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            logFont.lfWeight = FW_BOLD;
            _tcscpy_s(logFont.lfFaceName, fontName);
            s_hFont = CreateFontIndirect(&logFont);
            ReleaseDC(hWnd, hdc);
            //

            hbrushBg = CreateSolidBrush(crBg);

            SendMessage(hwndEdit, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDB_START:
                {
                    //SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)text);
                    StartButtonClick(hWnd);
                }
            break;
            case IDB_CLEAR:
                {
                    //SendMessage(hwndEdit, WM_CLEAR, 0, 0);
                    SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)TEXT(""));
                }
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CTLCOLOREDIT:
        {
            HDC hdc = (HDC)wParam;
            HWND hwndCtrl = (HWND)lParam;
            if (GetDlgItem(hWnd, IDC_TEXT) == hwndCtrl) {
                //SetBkColor(hdc, TRANSPARENT);
                SetBkColor(hdc, crBg);
                SetTextColor(hdc, crText);
                //return (LRESULT)GetStockObject(DC_BRUSH); // return a DC brush.
                //return (LRESULT)((HBRUSH)GetStockObject(BLACK_BRUSH));
                return (LRESULT)hbrushBg;
            }
            else
                return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DeleteObject(s_hFont);
        DeleteObject(hbrushBg);
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        {
            MoveWindow(hwndEdit,
                5, 40,                  // starting x- and y-coordinates 
                LOWORD(lParam) - 10,        // width of client area 
                HIWORD(lParam) - 45,        // height of client area 
                TRUE);                 // repaint window 
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void AppendText(const HWND& hwnd, TCHAR* newText)
{
    // get edit control from dialog
    HWND hwndOutput = GetDlgItem(hwnd, IDC_TEXT);

    // get new length to determine buffer size
    int outLength = GetWindowTextLength(hwndOutput) + lstrlen(newText) + 1;

    // create buffer to hold current and new text
    TCHAR* buf = (TCHAR*)GlobalAlloc(GPTR, outLength * sizeof(TCHAR));
    if (!buf) return;

    // get existing text from edit control and put into buffer
    GetWindowText(hwndOutput, buf, outLength);

    // append the newText to the buffer
    _tcscat_s(buf, outLength, newText);

    // Set the text in the edit control
    SetWindowText(hwndOutput, buf);

    // free the buffer
    GlobalFree(buf);
}

//

static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;               // Current seq. position

// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
void fibonacci_init(
    const unsigned long long a,
    const unsigned long long b)
{
    index_ = 0;
    current_ = a;
    previous_ = b; // see special case when initialized
}

// Produce the next value in the sequence.
// Returns true on success, false on overflow.
bool fibonacci_next()
{
    // check to see if we'd overflow result or position
    if ((ULLONG_MAX - previous_ < current_) ||
        (UINT_MAX == index_))
    {
        return false;
    }

    // Special case when index == 0, just return b value
    if (index_ > 0)
    {
        // otherwise, calculate next sequence value
        previous_ += current_;
    }
    std::swap(current_, previous_);
    ++index_;
    return true;
}

// Get the current value in the sequence.
unsigned long long fibonacci_current()
{
    return current_;
}

// Get the current index position in the sequence.
unsigned fibonacci_index()
{
    return index_;
}

//

void StartButtonClick(HWND hWnd)
{
    TCHAR t[] = TEXT("newText\r\n");
    for (int i = 0; i < 5; i++)
    {
        AppendText(hWnd, t);
        //Sleep(500);
    }

/*
    fibonacci_init(1, 1);
    do {
        std::string

        std::cout << fibonacci_index() << ": "
            << fibonacci_current() << std::endl;
    } while (fibonacci_next());
    // Report count of values written before overflow.
    std::cout << fibonacci_index() + 1 <<
        " Fibonacci sequence values fit in an " <<
        "unsigned 64-bit integer." << std::endl;
    */
}