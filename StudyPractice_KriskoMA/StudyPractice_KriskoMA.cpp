// StudyPractice_KriskoMA.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "StudyPractice_KriskoMA.h"
#include "sort_str.h"
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <Windows.h>
#include <mutex>
#include <cstdio>
#include <WinBase.h>

#define MAX_LOADSTRING 200

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
WCHAR button[MAX_LOADSTRING];
int num_proc;
int mas[10][10];
std::mutex mut;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STUDYPRACTICEKRISKOMA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STUDYPRACTICEKRISKOMA));

    MSG msg;

    // Цикл основного сообщения:
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

DWORD WINAPI sort_elem(LPVOID lpParameter)
{
    CreateNamedPipeA("\\.\pipe\piper", PIPE_ACCESS_OUTBOUND, PIPE_NOWAIT, PIPE_UNLIMITED_INSTANCES, 2, 0, 0, NULL);
    sort(mas[num_proc], mas[num_proc] + 10, std::greater<int>());
    return(0);
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STUDYPRACTICEKRISKOMA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STUDYPRACTICEKRISKOMA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

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
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND but1, but2, text1, text2;
    switch (message)
    {
    case WM_CREATE:
    {
        hInst = ((LPCREATESTRUCT)lParam)->hInstance;
        but1 = CreateWindow(L"BUTTON", L"Вывести начальные цифры", WS_VISIBLE | WS_CHILDWINDOW, 50, 50, 400, 30, hWnd, (HMENU)"button1", hInst, NULL);
        ShowWindow(but1, SW_NORMAL);
        but2 = CreateWindow(L"BUTTON", L"Отсортировать и вывести результат", WS_VISIBLE | WS_CHILDWINDOW, 50, 250, 400, 30, hWnd, (HMENU)"button2", hInst, NULL);
        ShowWindow(but2, SW_NORMAL);
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int flag;
            if (lParam == (LPARAM)but1)
            {
                std::string str;
                std::string temp;
                std::ifstream file;
                file.open("numbers.txt",std::ios::in);

                if (file.is_open())
                {
                    file.seekg(std::ios::beg);
                    file.clear();
                    for (int i = 0; i < 10; i++)
                    {
                        for (int j = 0; j < 10; j++)
                        {
                            file >> mas[i][j];
                        }
                    }
                    flag = 1;
                }

                else {
                    flag = 0;
                }
                //забрать цифры из файла и закинуть их в массив
                //из цифр сформировать строку и вывести ее в текст
                text1 = CreateWindow(L"STATIC", (flag == 1 ? L"Данные успешно считаны" : L"Ошибка открытия файла"), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CHILDWINDOW, 50, 80, 400, 170, hWnd, (HMENU)IDC_STATIC, hInst, NULL);
            }
            if (lParam == (LPARAM)but2)
            {
                HANDLE threads[10];
                std::ofstream file;
                int flag;
                WCHAR content2[MAX_LOADSTRING];
                for (num_proc = 0; num_proc < 10; num_proc++)
                {
                    mut.lock();
                    threads[num_proc] = CreateThread(NULL, 0, sort_elem, NULL, 0, NULL);
                    Sleep(100);
                    mut.unlock();
                }
                sort_str(mas);
                file.open("changed.txt");
                if (file.is_open())
                {
                    for (int i = 0; i < 10; i++)
                    {
                        for (int j = 0; j < 10; j++)
                        {
                            file << mas[i][j] << " ";
                        }
                        file << std::endl;
                    }
                    flag = 1;
                }
                else {
                    flag = 0;
                }
                text2 = CreateWindow(L"STATIC", (flag == 1 ? L"Данные записаны в файл \"changed.txt\"" : L"Ошибка открытия файла"), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CHILDWINDOW, 50, 280, 400, 170, hWnd, (HMENU)IDC_STATIC, hInst, NULL);
            }
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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
