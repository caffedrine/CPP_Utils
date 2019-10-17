/*  
 *  File      : SudokuBoard.hpp
 *  Created on: 16.10.2019
 *      Author: curiosul
 */

#ifndef _SUDOKUBOARD_H_
#define _SUDOKUBOARD_H_

#include <cstdio>
#include <cstdint>
#include <cstdlib>

#include <AnsiColors.h>
#include <TimeUtils.h>

#define UNSOLVED_SYMBOL     '*'
#define SUDOKU_MAX_SIZE     15

/** Struct to define a basic cell used by user to input it's data */
typedef struct
{
    char val;
    uint8_t BlockNo;
}cell_base_t;

class SudokuBoard
{
    const char *Charset = "123456789abcdef\0";
    const char *AsciiColors[SUDOKU_MAX_SIZE] =
            {
                    ANSI_COLOR_RED_BOLD,
                    ANSI_COLOR_GREEN_BOLD,
                    ANSI_COLOR_BLUE_BOLD,
                    ANSI_COLOR_MAGENTA_BOLD,
                    ANSI_COLOR_YELLOW_BOLD,
                    ANSI_COLOR_CYAN_BOLD,
                    ANSI_COLOR_WHITE_BOLD,
                    ANSI_COLOR_RED_BOLD,
                    ANSI_COLOR_GREEN_BOLD,
                    ANSI_COLOR_BLUE_BOLD,
                    ANSI_COLOR_MAGENTA_BOLD,
                    ANSI_COLOR_YELLOW_BOLD,
                    ANSI_COLOR_CYAN_BOLD,
                    ANSI_COLOR_WHITE_BOLD,
                    ANSI_COLOR_RED_BOLD,
            };
public:
    SudokuBoard(uint8_t Size_, cell_base_t *Cells) : Size(Size_)
    {
        /* Dynamic allocate memory for cells matrix */
        this->CellsMatrix = (cell_base_t **)malloc(this->Size * sizeof(cell_base_t *));
        for(int i = 0; i < this->Size; i++)
        {
            this->CellsMatrix[i] = (cell_base_t *)malloc(this->Size * sizeof(cell_base_t));
        }
        
        /* Copy matrix to internal matrix buffer */
        for(int i = 0; i < this->Size; i++)
        {
            for(int j = 0; j <= this->Size; j++)
            {
                this->CellsMatrix[i][j] = *((Cells+i*this->Size) + j);
            }
        }
        
    }
    
    ~SudokuBoard()
    {
        /* Make clean */
        for(int i = 0; i < this->Size; i++)
        {
            delete this->CellsMatrix[i];
        }
        delete[] this->CellsMatrix;
    }
    
    
    void PrintTimeElapsed()
    {
        static bool FirstPrint = true;
        if(FirstPrint == false)
        {
            /* Move cursor up to the first line */
            for( int i = 0; i < this->Size + 1; i++ )
            {
                printf("\33[A\r"); // Move cursor up one line
            }
            printf("\33[2K\r"); // Erase current line
        }
        
        /* Print time again */
        printf("Time elapsed: %s\n", this->TimeCounter.ElapsedTimeString().c_str());

        /* Send to the output */
        fflush(stdout);
    
        /* Move cursor back to the end */
        if(FirstPrint == false)
        {
            /* Move cursor up to the first line */
            for( int i = 0; i < this->Size; i++ )
            {
                printf("\33[B\r"); // Move cursor down one line
            }
        }
        
        FirstPrint = false;
    }
    
    void PrintBoard()
    {
        static bool FirstPrint = true;
        if(FirstPrint == false)
        {
            /* Display can be cleared as it is not first time matrix is being printed */
            for( int i = 0; i < this->Size; i++ )
            {
                printf("\33[2K\r"); // Erase current line
                printf("\33[A\r"); // Move cursor up one line
            }
        }
        
        for( int i = 0; i < 13; i++ )
        {
            for( int j = 0; j < 13; j++ )
            {
//                if( CellsMatrix[i][j].BlockNo >= 14 )
//                {
//                    CellsMatrix[i][j].BlockNo = 0;
//                }
            
                printf("%s%c " ANSI_COLOR_RESET, AsciiColors[CellsMatrix[i][j].BlockNo], CellsMatrix[i][j].val);
            }
            printf("\n");
        }
        fflush(stdout); // Will now print everything in the stdout buffer
        FirstPrint = false;
    }
    
    void Solve()
    {
        /* counter indicating how many cells were solved */
        uint16_t cellsSolved = 0;
        
        /* Reset time counter */
        TimeCounter.Restart();
        this->PrintTimeElapsed();
        this->PrintBoard();
        TimeUtils::Timer displayTimeTimer;
    
        bool SolutionPossible = false;
        while( !this->IsSolved() )
        {
            SolutionPossible = false;
            for( int y = 0; y < this->Size; y++ )
            {
                for( int x = 0; x < this->Size; x++ )
                {
                    /* Only loop through unknown symbols */
                    if( this->GetPossibleSolutions(x, y) == 1 )
                    {
                        /* Set a flag indicating that all cells shall be checked again before concluding that sudoku is not possible */
                        SolutionPossible = true;
                        
                        /* We have a winner, increment to know hoq many cells we solved */
                        cellsSolved++;
                        
                        /* Write sollution to corresponding cell */
                        this->CellsMatrix[x][y].val = this->GetNthSolution(x, y);
    
                        /* Update visual board containing last solution */
                        PrintBoard();
                        TimeUtils::SleepMs(10);
                    }
                    
                    /* Update time elapsed */
                    if(displayTimeTimer.ElapsedMs() >= 28)
                    {
                        displayTimeTimer.Restart();
                        this->PrintTimeElapsed();
                    }
                }/*X*/
            } /*Y*/
            if(!SolutionPossible)
            {
                uint8_t TwoSolutionsCells = 0;
                for( int y = 0; y < this->Size; y++ )
                {
                    for( int x = 0; x < this->Size; x++ )
                    {
                        uint8_t SolutionsNumber = this->GetPossibleSolutions(x, y);
                        if(SolutionsNumber > 0 && SolutionsNumber <= 3)
                        {
                            printf("[%d][%d] could have only %d solutions\n", x, y, SolutionsNumber);
                        }
                    }
                }
                
                break;
            }
        }/*while*/
        
//        this->PrintTimeElapsed();
//        PrintBoard();
        if(SolutionPossible == false)
        {
            printf("Cells solved: %d. Can's solve from this point...\n", cellsSolved);
        }
    }
    
protected:

private:
    const uint8_t Size = 0;
    cell_base_t **CellsMatrix;
    TimeUtils::Timer TimeCounter;
    
    /* Basic solving when for 1 possibility in a cell */
    bool IsSolved()
    {
        for(int i = 0; i < this->Size; i++)
        {
            for( int j = 0; j < this->Size; j++ )
            {
                if( this->CellsMatrix[i][j].val == UNSOLVED_SYMBOL )
                {
                    return false;
                }
            }
        }
        return true;
    }
    bool AlreadyOnBlock(uint8_t BlockId, char TargetChar)
    {
        for(int i = 0; i < this->Size; i++)
        {
            for( int j = 0; j < this->Size; j++ )
            {
                if( this->CellsMatrix[i][j].BlockNo == BlockId && this->CellsMatrix[i][j].val == TargetChar)
                {
                    return true;
                }
            }
        }
        return false;
    }
    bool AlreadyOnX(uint8_t CurrY, char TargetChar)
    {
        for(int i = 0; i < this->Size; i++)
        {
            /* Skip current element */
            if( this->CellsMatrix[i][CurrY].val == TargetChar )
            {
                return true;
            }
        }
        return false;
    }
    bool AlreadyOnY(uint8_t CurrX, char TargetChar)
    {
        for(int i = 0; i < this->Size; i++)
        {
            /* Skip current element */
            if( this->CellsMatrix[CurrX][i].val == TargetChar )
            {
                return true;
            }
        }
        return false;
    }
    uint8_t GetPossibleSolutions(uint8_t x, uint8_t y)
    {
        uint8_t PossibleSollutions = 0;
        if( this->CellsMatrix[x][y].val == UNSOLVED_SYMBOL )
        {
            for( int TryCharIdx = 0; TryCharIdx < this->Size; TryCharIdx++ )
            {
                /* Variable to store current char to be stored */
                char CurrentTryChar = Charset[TryCharIdx];
                /* Check whether it's safe to place this symbol on this vox */
                if( !this->AlreadyOnX(y, CurrentTryChar) &&
                    !this->AlreadyOnY(x, CurrentTryChar) &&
                    !this->AlreadyOnBlock(this->CellsMatrix[x][y].BlockNo, CurrentTryChar) )
                {
                    PossibleSollutions++;
                }
            }
        }
        return PossibleSollutions;
    }
    char GetNthSolution(uint8_t x, uint8_t y, uint8_t NTh = 0)
    {
        if( this->CellsMatrix[x][y].val == UNSOLVED_SYMBOL )    /* If cell is still unsolved */
        {
            uint8_t PossibleSollutions = 0;
            for( int TryCharIdx = 0; TryCharIdx < this->Size; TryCharIdx++ )
            {
                /* Variable to store current char to be stored */
                char CurrentTryChar = Charset[TryCharIdx];
                /* Check whether it's safe to place this symbol on this vox */
                if( !this->AlreadyOnX(y, CurrentTryChar) &&
                    !this->AlreadyOnY(x, CurrentTryChar) &&
                    !this->AlreadyOnBlock(this->CellsMatrix[x][y].BlockNo, CurrentTryChar) )
                {
                    if( PossibleSollutions == NTh )
                    {
                        return CurrentTryChar;
                    }
            
                    PossibleSollutions++;
                }
            }
        }
        return this->CellsMatrix[x][y].val;
    }
    /* Advanced solving */
    

};

#endif // _SUDOKUBOARD_H_