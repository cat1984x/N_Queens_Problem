#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef WIN32

#define MAX_BOARDSIZE 21
typedef unsigned __int64 SOLUTIONTYPE;

#else

#define MAX_BOARDSIZE 18
typedef unsigned long SOLUTIONTYPE;

#endif // WIN32

#define MIN_BOARDSIZE 2

SOLUTIONTYPE g_numsolutions = 0;

void printtable(int boardsize, int* aQueenBitRes, SOLUTIONTYPE numSolution)
{
    int i, j, k, row;
    for(k = 0; k < 2; ++k)
    {
#ifdef WIN32
        printf("*** Solution #: %I64d ***\n", 2 * numSolution + k -1);
#else
        printf("*** Solution #: %d ***\n", 2 * numSolution + k -1);
#endif // WIN32
        for( i = 0; i < boardsize; i++)
        {
            unsigned int bitf;
            bitf = aQueenBitRes[i];
            row = bitf ^ (bitf & (bitf - 1));
            for(j = 0; j < boardsize; j++)
            {
                if(0 == k && ((row >> j) & 1))
                {
                    printf("Q");
                }
                else if(1 == k && (row & (1 << (boardsize - j -1))))
                {
                    printf("Q");
                }
                else
                {
                    printf(".");
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}

void Nqueen(int board_size)
{
    int aQueenBitRes[MAX_BOARDSIZE];
    int aQueenBitCol[MAX_BOARDSIZE];
    int aQueenBitPosDiag[MAX_BOARDSIZE];
    int aQueenBitNegDiag[MAX_BOARDSIZE];
    int aStack[MAX_BOARDSIZE + 2];
    register int* pnStack;
    register int numrows = 0;
    register unsigned int lsb;
    register unsigned int bitfield;
    int i;
    int odd = board_size & 1;
    int board_minus = board_size - 1;
    int mask = (1 << board_size) - 1;

    aStack[0] = -1;

    for(i = 0; i < (1 + odd); ++i)
    {
        //initialize
        bitfield = 0;
        if(0 == i)
        {
            int half = board_size >> 1;
            bitfield = (1 << half) - 1;
            pnStack = aStack + 1;
            aQueenBitRes[0] = 0;
            aQueenBitCol[0] = aQueenBitPosDiag[0] = aQueenBitNegDiag[0] = 0;
        }
        else
        {
            bitfield = 1 << (board_size >> 1);
            numrows = 1;
            aQueenBitRes[0] = bitfield;
            aQueenBitCol[0] = aQueenBitPosDiag[0] = aQueenBitNegDiag[0] = 0;
            aQueenBitCol[1] = bitfield;
            aQueenBitNegDiag[1] = (bitfield >> 1);
            aQueenBitPosDiag[1] = (bitfield << 1);
            pnStack = aStack + 1;
            *pnStack++ = 0;
            bitfield = (bitfield - 1) >> 1;
        }
        //find the position
        for(;;)
        {
            lsb = -((signed)bitfield) & bitfield;
            if(0 == bitfield)
            {
                bitfield = * --pnStack;
                if(pnStack == aStack)
                {
                    break;
                }
                --numrows;
                continue;
            }
            bitfield &= ~lsb;
            aQueenBitRes[numrows] = lsb;
            if(numrows < board_minus)
            {
                int n = numrows++;
                aQueenBitCol[numrows] = aQueenBitCol[n] | lsb;
                aQueenBitNegDiag[numrows] = (aQueenBitNegDiag[n] | lsb) >> 1;
                aQueenBitPosDiag[numrows] = (aQueenBitPosDiag[n] | lsb) << 1;
                *pnStack++ = bitfield;
                bitfield = mask & ~(aQueenBitCol[numrows]
                                    | aQueenBitNegDiag[numrows]
                                    | aQueenBitPosDiag[numrows]);
                continue;
            }
            else
            {
                printtable(board_size, aQueenBitRes, g_numsolutions + 1);
                ++g_numsolutions;
                bitfield = * --pnStack;
                --numrows;
                continue;
            }
        }
    }
    g_numsolutions *= 2;
}

void printResults(time_t* pt1, time_t* pt2)
{
    double secs;
    int hours, mins, intsecs;
    printf("End: \t%s", ctime(pt2));
    secs = difftime(*pt2, *pt1);
    intsecs = (int)secs;
    printf("Calculations took %d second%s.\n", intsecs, (intsecs == 1? " ":"s"));
    hours = intsecs / 3600;
    intsecs -= hours * 3600;
    mins = intsecs / 60;
    intsecs -= mins * 60;
    if(hours > 0 || mins > 0)
    {
        printf("Equals ");
        if(hours > 0)
        {
            printf("%d hour%s and ", hours, (hours == 1) ? " " : "s");
        }
        if(mins > 0)
        {
            printf("%d minute%s and ", mins, (mins == 1) ? " " : "s");
        }
        printf("%d second%s.\n", intsecs, (intsecs == 1) ? " " : "s");
    }
}
int main(int argc, char** argv)
{
    time_t t1,t2;
    int boardsize;
    if(argc != 2)
    {
        printf("N Queens program by chenyushen.\n");
        printf("chenyushen1989@163.com\n");
        printf("This program calculates the total number of solutions to the N Queens problem.\n");
        printf("Usage: NQueen<width of board>\n");
        return 0;
    }
    boardsize = atoi(argv[1]);
    if(MIN_BOARDSIZE > boardsize ||
       MAX_BOARDSIZE < boardsize)
    {
        printf("Width of board must be between %d and %d,inclusive.\n",
               MIN_BOARDSIZE, MAX_BOARDSIZE);
        return 0;
    }
    time(&t1);
    printf("N Queens program by chenyushen.\n");
    printf("chenyushen1989@163.com\n");
    printf("Start: \t %s", ctime(&t1));
    Nqueen(boardsize);
    time(&t2);
    printResults(&t1, &t2);
    if(g_numsolutions != 0)
    {
        #ifdef WIN32
            printf("For board size %d, %I64d solution%s found.\n",
                   boardsize, g_numsolutions, (g_numsolutions == 1 ? " ":"s"));
        #else
            printf("For board size %d, %d solutions%s found.\n",
                   boardsize, g_numsolutions, (g_numsolutions == 1 ? " ":"s"));
        #endif // WIN32
    }
    else
    {
        printf("No solutions found.\n");
    }
    return 0;
}
