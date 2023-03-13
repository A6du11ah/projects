#pragma once

#include"main_screen_utility.h"

void window_fullscreen(void)
{
    keybd_event(VK_MENU,0x38,0,0);
    keybd_event(VK_RETURN,0x1c,0,0);
    keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
    keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}

void customConsoleWindowSetup(void)
{
    //changing screen buffer size
    COORD curBuffSize;

    curBuffSize.X = MAIN_SCREEN_WIDTH + 1;
    curBuffSize.Y = MAIN_SCREEN_HEIGHT + 1;

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), curBuffSize);

    //changing window size
    SMALL_RECT curWinSize;

    curWinSize.Top = 0;
    curWinSize.Left = 0;
    curWinSize.Bottom = MAIN_SCREEN_HEIGHT;
    curWinSize.Right = MAIN_SCREEN_WIDTH;

    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &curWinSize);

    //getting computer monitor screen size
    const HWND deskHandle = GetDesktopWindow();

    RECT deskRect;
    GetWindowRect(deskHandle, &deskRect);

    //changing font size
    CONSOLE_FONT_INFOEX curConFontInfo;

    curConFontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    curConFontInfo.nFont = 0;
    curConFontInfo.dwFontSize.X = (deskRect.bottom / 128) - 1;
    curConFontInfo.dwFontSize.Y = (deskRect.bottom / 54) - 2;
    curConFontInfo.FontFamily = 54;
    curConFontInfo.FontWeight = FW_NORMAL;
    wcscpy(curConFontInfo.FaceName, L"Consolas");

    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &curConFontInfo);
}

void initialize_main_screen(void)
{
    for(int i = 0; i < MAIN_SCREEN_HEIGHT; i++)
    {
        for(int j = 0; j < MAIN_SCREEN_WIDTH; j++)
        {
            main_screen[i][j] = EMPTY_SPACE;
        }
    }
}

void clear_main_screen(int x, int y, int w, int h)
{
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            main_screen[y + i][x + j] = EMPTY_SPACE;
        }
    }
}

void setGameViewport(int x, int y, int w, int h)
{
    //outer layer
    main_screen[y][x] = 218;
    main_screen[y + ((h + 6) / 4) - 1][x] = 192;
    main_screen[y][x + ((w + 6) / 4) - 1] = 191;

    main_screen[y + h + 5][x + w + 5] = 217;
    main_screen[y + h - ((h + 6) / 4) + 6][x + w + 5] = 191;
    main_screen[y + h + 5][x + w - ((w + 6) / 4) + 6] = 192;

    for(int j = 1; j < ((w + 6) / 4) - 1; j++)
    {
        main_screen[y][x + j] = 196;
        main_screen[y + h + 5][x + w - ((w + 6) / 4) + 6 + j] = 196;
    }

    for(int i = 1; i < ((h + 6) / 4) - 1; i++)
    {
        main_screen[y + i][x] = 179;
        main_screen[y + h - ((h + 6) / 4) + 6 + i][x + w + 5] = 179;
    }

    main_screen[y + h + 5][x] = 178;
    main_screen[y + h - ((h + 6) / 3) + 6][x] = '/';
    main_screen[y + h + 5][x + ((w + 6) / 3) - 1] = '/';

    main_screen[y][x + w + 5] = 178;
    main_screen[y + ((h + 6) / 3) - 1][x + w + 5] = '/';
    main_screen[y][x + w - ((w + 6) / 3) + 6] = '/';

    for(int j = 1; j < ((w + 6) / 3) - 1; j++)
    {
        main_screen[y][x + w - ((w + 6) / 3) + 6 + j] = 223;
        main_screen[y + h + 5][x + j] = 220;
    }

    for(int i = 1; i < ((h + 6) / 3) - 1; i++)
    {
        main_screen[y + i][x + w + 5] = 222;
        main_screen[y + h - ((h + 6) / 3) + 6 + i][x] = 221;
    }

    //mid layer
    main_screen[y + 1][x + 1] = 201;
    main_screen[y + 1][x + w + 4] = 191;
    main_screen[y + h + 4][x + 1] = 192;
    main_screen[y + h + 4][x + w + 4] = 188;

    for(int i = y + 2; i < y + h + 4; i++)
    {
        main_screen[i][x + 1] = 178;
        main_screen[i][x + w + 4] = 178;
    }

    for(int j = x + 2; j < x + w + 4; j++)
    {
        main_screen[y + 1][j] = 177;
        main_screen[y + h + 4][j] = 177;
    }

    //inner layer
    main_screen[y + 2][x + 2] = 201;
    main_screen[y + 2][x + w + 3] = 187;
    main_screen[y + h + 3][x + 2] = 200;
    main_screen[y + h + 3][x + w + 3] = 188;

    for(int i = y + 3; i < y + h + 3; i++)
    {
        main_screen[i][x + 2] = 186;
        main_screen[i][x + w + 3] = 186;
    }

    for(int j = x + 3; j < x + w + 3; j++)
    {
        main_screen[y + 2][j] = 205;
        main_screen[y + h + 3][j] = 205;
    }
}

void setGameInstructionTab(int x, int y, int w, int h)
{
    //outer layer
    main_screen[y][x] = 177;
    main_screen[y][x + w + 3] = 191;
    main_screen[y + h + 3][x] = 192;
    main_screen[y + h + 3][x + w + 3] = 177;

    for(int i = 1; i < h + 3; i++)
    {
        if(i <((h + 4) / 3))
        {
            main_screen[y + i][x] = 177;
            main_screen[y + i][x + w + 3] = 221;
        }
        else if(i > (h + 4) - ((h + 4) / 3) - 1)
        {
            main_screen[y + i][x] = 221;
            main_screen[y + i][x + w + 3] = 177;
        }
        else
        {
            main_screen[y + i][x] = 221;
            main_screen[y + i][x + w + 3] = 221;
        }
    }

    for(int j = 1; j < w + 3; j++)
    {
        if(j <((w + 4) / 3))
        {
            main_screen[y][x + j] = 177;
            main_screen[y + h + 3][x + j] = 220;
        }
        else if(j > (w + 4) - ((w + 4) / 3) - 1)
        {
            main_screen[y][x + j] = 220;
            main_screen[y + h + 3][x + j] = 177;
        }
        else
        {
            main_screen[y][x + j] = 220;
            main_screen[y + h + 3][x + j] = 220;
        }
    }

    //inner layer
    main_screen[y + 1][x + 1] = 218;
    main_screen[y + 1][x + w + 2] = 191;
    main_screen[y + h + 2][x + 1] = 192;
    main_screen[y + h + 2][x + w + 2] = 217;

    for(int i = y + 2; i < y + h + 2; i++)
    {
        main_screen[i][x + 1] = 179;
        main_screen[i][x + w + 2] = 179;
    }

    for(int j = x + 2; j < x + w + 2; j++)
    {
        main_screen[y + 1][j] = 196;
        main_screen[y + h + 2][j] = 196;
    }
}

void setGameAccessoryTab(int x, int y, int w, int h)
{
    //top and bottom
    main_screen[y][x] = 218;
    main_screen[y][x + w + 3] = 191;
    main_screen[y + h + 5][x] = 192;
    main_screen[y + h + 5][x + w + 3] = 217;

    for(int j = x + 1; j < x + w + 3; j++)
    {
        main_screen[y][j] = 220;
        main_screen[y + h + 5][j] = 223;
    }

    //outer layer
    main_screen[y + 1][x] = 177;
    main_screen[y + 1][x + w + 3] = 177;
    main_screen[y + h + 4][x] = 177;
    main_screen[y + h + 4][x + w + 3] = 177;

    for(int i = 0; i < h + 2; i++)
    {
        if(i > ((h + 2) / 4) && i < h + 1 - ((h + 2) / 4))
        {
            main_screen[y + 2 + i][x] = 222;
            main_screen[y + 2 + i][x + w + 3] = 221;
        }
        else if(i == ((h + 2) / 4))
        {
            main_screen[y + 2 + i][x] = '\\';
            main_screen[y + 2 + i][x + w + 3] = '/';
        }
        else if(i == h + 1 - ((h + 2) / 4))
        {
            main_screen[y + 2 + i][x] = '/';
            main_screen[y + 2 + i][x + w + 3] = '\\';
        }
        else
        {
            main_screen[y + 2 + i][x] = 177;
            main_screen[y + 2 + i][x + w + 3] = 177;
        }
    }

    for(int j = x + 1; j < x + w + 3; j++)
    {
        main_screen[y + 1][j] = 177;
        main_screen[y + h + 4][j] = 177;
    }

    //inner layer
    main_screen[y + 2][x + 1] = 218;
    main_screen[y + 2][x + w + 2] = 191;
    main_screen[y + h + 3][x + 1] = 192;
    main_screen[y + h + 3][x + w + 2] = 217;

    for(int i = y + 3; i < y + h + 3; i++)
    {
        main_screen[i][x + 1] = 179;
        main_screen[i][x + w + 2] = 179;
    }

    for(int j = x + 2; j < x + w + 2; j++)
    {
        main_screen[y + 2][j] = 196;
        main_screen[y + h + 3][j] = 196;
    }
}

void setMessageBox(int x, int y, int w, int h)
{
    //outer layer
    main_screen[y][x] = 218;
    main_screen[y][x + w + 3] = 191;
    main_screen[y + h + 3][x] = 192;
    main_screen[y + h + 3][x + w + 3] = 217;

    for(int i = y + 1; i < y + h + 3; i++)
    {
        main_screen[i][x] = 179;
        main_screen[i][x + w + 3] = 179;
    }

    for(int j = x + 1; j < x + w + 3; j++)
    {
        main_screen[y][j] = 196;
        main_screen[y + h + 3][j] = 196;
    }

    //inner layer
    for(int i = y + 1 + ((h + 2) / 3); i < y + 1 + (2 * (h + 2) / 3); i++)
    {
        main_screen[i][x + 1] = 179;
        main_screen[i][x + w + 2] = 179;
    }

    for(int j = x + 2; j < x + w + 2; j++)
    {
        main_screen[y + 1][j] = 196;
        main_screen[y + h + 2][j] = 196;
    }
}

void show_main_screen(void)
{
    resetCursorPosition();

    for(int i = 0; i < MAIN_SCREEN_HEIGHT; i++)
    {
        printf("%s\n", main_screen[i]);
    }
}

void resetCursorPosition(void)
{
    COORD cursorPosition;

    cursorPosition.X = 0;
    cursorPosition.Y = 0;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void delay(int delay_length)
{
    while(delay_length--);
}

void pressEnterToContinue(void)
{
    while(1)
    {
        if(GetAsyncKeyState(VK_RETURN) & 0x8000) break;
    }
}
