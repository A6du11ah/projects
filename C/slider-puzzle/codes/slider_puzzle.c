#include "main_screen_utility.c"

///additional utility functions---------------------------------------------------------------

struct
{
    int key_Enter;
    int key_Escape;
    int key_UArrow;
    int key_DArrow;
    int key_LArrow;
    int key_RArrow;
} CurKeyState = {0, 0, 0, 0, 0};

void checkKeyState(void)
{
    //resetting state
    CurKeyState.key_Enter = 0;
    CurKeyState.key_Escape = 0;
    CurKeyState.key_UArrow = 0;
    CurKeyState.key_DArrow = 0;
    CurKeyState.key_LArrow = 0;
    CurKeyState.key_RArrow = 0;

    //checking state
    if(GetKeyState(VK_UP) & 0x8000) CurKeyState.key_UArrow = 1;
    else if(GetKeyState(VK_DOWN) & 0x8000) CurKeyState.key_DArrow = 1;
    else if(GetKeyState(VK_LEFT) & 0x8000) CurKeyState.key_LArrow = 1;
    else if(GetKeyState(VK_RIGHT) & 0x8000) CurKeyState.key_RArrow = 1;
    else if(GetKeyState(VK_RETURN) & 0x8000) CurKeyState.key_Enter = 1;
    else if(GetKeyState(VK_ESCAPE) & 0x8000) CurKeyState.key_Escape = 1;
}

void keepFixedWindowSize(void)
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
}
///------------------------------------------------------------------------------------------

///feedback functions------------------------------------------------------------------------

int retryPuzzle(void)
{
    char text[3][43] =
    {"          !!! Congratulations !!!         ",
     "     - - - You solved the puzzle - - -    ",
     "[ Press ENTER to retry or ESCAPE to quit ]"};

    clear_main_screen(23, 20, 46, 7);
    setMessageBox(23, 20, 44, 5);

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 42; j++)
        {
            main_screen[22 + (2 * i)][26 + j] = text[i][j];
        }
    }

    while(1)
    {
        keepFixedWindowSize();
        show_main_screen();

        checkKeyState();

        if(CurKeyState.key_Enter) return 1;
        else if(CurKeyState.key_Escape) return 0;
    }

    return 0;
}
///------------------------------------------------------------------------------------------

///puzzle handling functions-----------------------------------------------------------------

int getPuzzleSize(void)
{
    FILE* fp = fopen("readme.txt", "r");

    if(fp == NULL) return 3;

    int puzzle_size;

    fscanf(fp, "puzzle_size = %d", &puzzle_size);

    fclose(fp);

    if(puzzle_size < 2 || puzzle_size > 10) return 3;

    return puzzle_size;
}

void setPuzzleBlock(int puzzle_size, int puzzle[][puzzle_size], int pbsi[])
{
    for(int i = 0; i < puzzle_size; i++)
    {
        for(int j = 0; j < puzzle_size; j++)
        {
            if(puzzle[i][j] != 0)
            {
                for(int l = 0; l < 3; l++)
                {
                    for(int k = 0; k < 3; k++)
                    {
                        if(l == 1 && k == 1)
                        {
                            main_screen[4 + pbsi[1] + 4 * i + l][6 + pbsi[0] + 8 * j + 2 * k] = '0' + (puzzle[i][j] / 10);
                            main_screen[4 + pbsi[1] + 4 * i + l][6 + pbsi[0] + 8 * j + 2 * k + 1] = '0' + (puzzle[i][j] % 10);
                        }
                        else
                        {
                            main_screen[4 + pbsi[1] + 4 * i + l][6 + pbsi[0] + 8 * j + 2 * k] = 219;
                            main_screen[4 + pbsi[1] + 4 * i + l][6 + pbsi[0] + 8 * j + 2 * k + 1] = 219;
                        }
                    }
                }
            }
        }
    }
}

void sePuzzleBlockPtr(int puzzleBlockPtr[], int pbsi[])
{
    int x = puzzleBlockPtr[0], y = puzzleBlockPtr[1];

    main_screen[3 + pbsi[1] + 4 * y][4 + pbsi[0] + 8 * x] = 219;
    main_screen[3 + pbsi[1] + 4 * y][4 + pbsi[0] + 8 * x + 1] = 223;

    main_screen[3 + pbsi[1] + 4 * y][4 + pbsi[0] + 8 * x + 8] = 223;
    main_screen[3 + pbsi[1] + 4 * y][4 + pbsi[0] + 8 * x + 9] = 219;

    main_screen[3 + pbsi[1] + 4 * y + 4][4 + pbsi[0] + 8 * x] = 219;
    main_screen[3 + pbsi[1] + 4 * y + 4][4 + pbsi[0] + 8 * x + 1] = 220;

    main_screen[3 + pbsi[1] + 4 * y + 4][4 + pbsi[0] + 8 * x + 8] = 220;
    main_screen[3 + pbsi[1] + 4 * y + 4][4 + pbsi[0] + 8 * x + 9] = 219;

    if(puzzleBlockPtr[2])
    {
        for(int k = 1; k < 4; k++)
        {
            main_screen[3 + pbsi[1] + 4 * y + k][4 + pbsi[0] + 8 * x] = 219;
            main_screen[3 + pbsi[1] + 4 * y + k][4 + pbsi[0] + 8 * x + 9] = 219;
        }

        for(int k = 2; k < 8; k++)
        {
            main_screen[3 + pbsi[1] + 4 * y][4 + pbsi[0] + 8 * x + k] = 223;
            main_screen[3 + pbsi[1] + 4 * y + 4][4 + pbsi[0] + 8 * x + k] = 220;
        }
    }
}

void puzzleNavigation(int puzzle_size, int puzzle[][puzzle_size], int puzzleBlockPtr[])
{
    //additional conditions to avoid continuous keypress
    if(CurKeyState.key_UArrow) puzzleBlockPtr[3] = 1 + 5 * puzzleBlockPtr[2];
    else if(CurKeyState.key_DArrow) puzzleBlockPtr[3] = 2 + 5 * puzzleBlockPtr[2];
    else if(CurKeyState.key_LArrow) puzzleBlockPtr[3] = 3 + 5 * puzzleBlockPtr[2];
    else if(CurKeyState.key_RArrow) puzzleBlockPtr[3] = 4 + 5 * puzzleBlockPtr[2];
    else if(CurKeyState.key_Enter) puzzleBlockPtr[3] = 5;
    else if(CurKeyState.key_Escape) exit(0);

    //navigation condition checking
    if(!CurKeyState.key_UArrow && puzzleBlockPtr[3] == 1)
    {
        puzzleBlockPtr[1] = (puzzle_size + puzzleBlockPtr[1] - 1) % puzzle_size;
        puzzleBlockPtr[3] = 0;
    }
    else if(!CurKeyState.key_DArrow && puzzleBlockPtr[3] == 2)
    {
        puzzleBlockPtr[1] = (puzzleBlockPtr[1] + 1) % puzzle_size;
        puzzleBlockPtr[3] = 0;
    }
    else if(!CurKeyState.key_LArrow && puzzleBlockPtr[3] == 3)
    {
        puzzleBlockPtr[0] = (puzzle_size + puzzleBlockPtr[0] - 1) % puzzle_size;
        puzzleBlockPtr[3] = 0;
    }
    else if(!CurKeyState.key_RArrow && puzzleBlockPtr[3] == 4)
    {
        puzzleBlockPtr[0] = (puzzleBlockPtr[0] + 1) % puzzle_size;
        puzzleBlockPtr[3] = 0;
    }
    else if(!CurKeyState.key_Enter && puzzleBlockPtr[3] == 5 && puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0]] != 0)
    {
        puzzleBlockPtr[2] = !puzzleBlockPtr[2];
        puzzleBlockPtr[3] = 0;
    }
    else if(!CurKeyState.key_UArrow && puzzleBlockPtr[3] == 6)
    {
        if(puzzleBlockPtr[1] - 1 >= 0 && puzzle[puzzleBlockPtr[1] - 1][puzzleBlockPtr[0]] == 0)
        {
            puzzle[puzzleBlockPtr[1] - 1][puzzleBlockPtr[0]] = puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0]];
            puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0]] = 0;

            puzzleBlockPtr[1]--;
        }

        puzzleBlockPtr[3] = 0;
    }
    else if(!CurKeyState.key_DArrow && puzzleBlockPtr[3] == 7)
    {
        if(puzzleBlockPtr[1] + 1 < puzzle_size && puzzle[puzzleBlockPtr[1] + 1][puzzleBlockPtr[0]] == 0)
        {
            puzzle[puzzleBlockPtr[1] + 1][puzzleBlockPtr[0]] = puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0]];
            puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0]] = 0;

            puzzleBlockPtr[1]++;
        }

        puzzleBlockPtr[3] = 0;
    }
    else if(!CurKeyState.key_LArrow && puzzleBlockPtr[3] == 8)
    {
        if(puzzleBlockPtr[0] - 1 >= 0 && puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0] - 1] == 0)
        {
            puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0] - 1] = puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0]];
            puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0]] = 0;

            puzzleBlockPtr[0]--;
        }

        puzzleBlockPtr[3] = 0;
    }
    else if(!CurKeyState.key_RArrow && puzzleBlockPtr[3] == 9)
    {
        if(puzzleBlockPtr[0] + 1 < puzzle_size && puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0] + 1] == 0)
        {
            puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0] + 1] = puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0]];
            puzzle[puzzleBlockPtr[1]][puzzleBlockPtr[0]] = 0;

            puzzleBlockPtr[0]++;
        }

        puzzleBlockPtr[3] = 0;
    }
}

int solvablePuzzle(int puzzle_size, int puzzle[][puzzle_size])
{
    int emptyBlockIndex, inversionCount = 0;

    //counting inversions
    for(int i = 0; i < puzzle_size * puzzle_size; i++)
    {
        if(puzzle[i / puzzle_size][i % puzzle_size] != 0)
        {
            for(int j = i + 1; j < puzzle_size * puzzle_size; j++)
            {
                if(puzzle[j / puzzle_size][j % puzzle_size] != 0 && puzzle[i / puzzle_size][i % puzzle_size] > puzzle[j / puzzle_size][j % puzzle_size])
                {
                    inversionCount++;
                }
            }
        }
        else emptyBlockIndex = i;
    }

    //checking if solvable
    if(puzzle_size % 2 != 0) return (inversionCount % 2 == 0);
    else return (((puzzle_size - (emptyBlockIndex / puzzle_size)) % 2 != 0) + (inversionCount % 2 != 0) == 1);
}

void shufflePuzzle(int puzzle_size, int puzzle[][puzzle_size])
{
    for(int i = 0; i < puzzle_size; i++)
    {
        for(int j = 0; j < puzzle_size; j++)
        {
            puzzle[i][j] = (puzzle_size * i + j + 1) % (puzzle_size * puzzle_size);
        }
    }

    srand(time(0));

    //fisher-yates shuffling algorithm
    for(int i = 0, ri, temp; i < puzzle_size * puzzle_size; i++)
    {
        ri = i + (rand() % (puzzle_size * puzzle_size - i));

        temp = puzzle[ri / puzzle_size][ri % puzzle_size];
        puzzle[ri / puzzle_size][ri % puzzle_size] = puzzle[i / puzzle_size][i % puzzle_size];
        puzzle[i / puzzle_size][i % puzzle_size] = temp;
    }

    //making the puzzle solvable if not solvable
    if(solvablePuzzle(puzzle_size, puzzle) == 0)
    {
        int temp, si;

        if(puzzle[0][0] != 0 && puzzle[0][1] != 0) si = 0;
        else si = (puzzle_size * puzzle_size) - 2;

        temp = puzzle[si / puzzle_size][si % puzzle_size];
        puzzle[si / puzzle_size][si % puzzle_size] = puzzle[(si + 1) / puzzle_size][(si + 1) % puzzle_size];
        puzzle[(si + 1) / puzzle_size][(si + 1) % puzzle_size] = temp;
    }
}

int checkPuzzle(int puzzle_size, int puzzle[][puzzle_size])
{
    for(int i = 0; i < puzzle_size; i++)
    {
        for(int j = 0; j < puzzle_size; j++)
        {
            if(puzzle[i][j] != (puzzle_size * i + j + 1) % (puzzle_size * puzzle_size)) return 0;
        }
    }

    return 1;
}

void playPuzzle(void)
{
    int puzzle_size = getPuzzleSize();

    int puzzle[puzzle_size][puzzle_size];

    int puzzleBlockPtr[4] = {0, 0, 0, 0};

    shufflePuzzle(puzzle_size, puzzle);

    int pbsi[2] = {((MAIN_SCREEN_WIDTH - (8 * puzzle_size + 10)) / 2), ((MAIN_SCREEN_HEIGHT - 1 - (4 * puzzle_size + 5)) / 2)};

    setGameInstructionTab(2 + pbsi[0], 1 + pbsi[1], 8 * puzzle_size + 2, 4 * puzzle_size + 1);

    while(1)
    {
        keepFixedWindowSize();

        clear_main_screen(4 + pbsi[0], 3 + pbsi[1], 8 * puzzle_size + 2, 4 * puzzle_size + 1);

        setPuzzleBlock(puzzle_size, puzzle, pbsi);
        sePuzzleBlockPtr(puzzleBlockPtr, pbsi);

        show_main_screen();

        if(puzzleBlockPtr[2] != 1 && checkPuzzle(puzzle_size, puzzle) == 1) return;

        checkKeyState();
        puzzleNavigation(puzzle_size, puzzle, puzzleBlockPtr);
    }
}
///-------------------------------------------------------------------------------------------

///driver function----------------------------------------------------------------------------

int main(void)
{
    customConsoleWindowSetup();

    while(1)
    {
        initialize_main_screen();

        playPuzzle();
        if(retryPuzzle() == 0) break;
    }

    return 0;
}
///-------------------------------------------------------------------------------------------
