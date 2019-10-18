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

/** Struct to define a basic cell used by user to input it's data */
typedef struct
{
    char val;
    uint8_t BlockNo;
    uint8_t IsAvailable;
}cell_extended_t;

class SudokuBoard
{
    const char *Charset = "123456789abcdef\0";
    const char *AsciiColors[SUDOKU_MAX_SIZE] =
            {
                    ANSI_COLOR_RED_BG,
                    ANSI_COLOR_GREEN_BG,
                    ANSI_COLOR_BLUE_BG,
                    ANSI_COLOR_MAGENTA_BG,
                    ANSI_COLOR_YELLOW_BG,
                    ANSI_COLOR_CYAN_BG,
                    ANSI_COLOR_RED_BG,
                    ANSI_COLOR_GREEN_BG,
                    ANSI_COLOR_BLUE_BG,
                    ANSI_COLOR_MAGENTA_BG,
                    ANSI_COLOR_YELLOW_BG,
                    ANSI_COLOR_CYAN_BG,
                    ANSI_COLOR_RED_BG,
                    ANSI_COLOR_GREEN_BG,
                    ANSI_COLOR_BLUE_BG,
            };
public:
    SudokuBoard(uint8_t Size_, cell_base_t *Cells) : Size(Size_)
    {
        /* Dynamic allocate memory for cells matrix */
//        this->CellsMatrix = (cell_extended_t **)malloc(this->Size * sizeof(cell_extended_t *));
//        for(int i = 0; i < this->Size; i++)
//        {
//            this->CellsMatrix[i] = (cell_extended_t *)malloc(this->Size * sizeof(cell_extended_t));
//        }
        
        /* Copy matrix to internal matrix buffer */
        for(int i = 0; i < this->Size; i++)
        {
            for(int j = 0; j <= this->Size; j++)
            {
                this->CellsMatrix[i][j].val = (*((Cells+i*this->Size) + j)).val;
                this->CellsMatrix[i][j].BlockNo = (*((Cells+i*this->Size) + j)).BlockNo;
            }
        }
    }
    
    ~SudokuBoard()
    {
        /* Make clean */
//        for(int i = 0; i < this->Size; i++)
//        {
//            delete this->CellsMatrix[i];
//        }
//        delete[] this->CellsMatrix;
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
        /* Reset time counter */
        TimeCounter.Restart();
        this->PrintTimeElapsed();
        this->PrintBoard();
        TimeUtils::Timer displayTimeTimer;
        
        while( !this->IsSolved() )
        {
            uint16_t StartProgress = this->CellsSolved;
            
            Algo_SafeMoves();
            if(this->IsSolved()) break;
            
            Algo_CheckIntersections();
            if(this->IsSolved()) break;
    
            Algo_Blocks();
            if(this->IsSolved()) break;
            
            /* Update time elapsed */
            if( displayTimeTimer.ElapsedMs() >= 28 )
            {
                displayTimeTimer.Restart();
                this->PrintTimeElapsed();
            }
            
            /* Algorithms are blocked...fins something better :) */
            if( this->CellsSolved == StartProgress )
            {
                break;
            }
            
        }/*while*/
        
        this->PrintTimeElapsed();
        PrintBoard();
        if(!this->IsSolved())
        {
            printf("Cells solved: %d. Can's solve from this point...\n", this->CellsSolved);
            Print_Info();
        }
    } /* Solve() */
    
protected:

private:
    const uint8_t Size = 0;
    cell_extended_t CellsMatrix[13][13];//**CellsMatrix;
    TimeUtils::Timer TimeCounter;
    uint16_t CellsSolved = 0;
    
    /* Algorithms to be applied */
    void Algo_SafeMoves()
    {
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                /* Only loop through unknown symbols */
                if( this->GetPossibleSolutions(x, y) == 1 )
                {
                    /* Update status */
                    this->IncCellsSolved();
                
                    /* Write sollution to corresponding cell */
                    this->CellsMatrix[y][x].val = this->GetNthSolution(x, y);
                    
                }
                
            }/*X*/
        } /*Y*/
    }
    
    void Algo_CheckIntersections()
    {
        /* Itterate through all symbols */
        for(int ChrIdx = 0; ChrIdx < this->Size; ChrIdx++)
        {
            /* Current symbol */
            char CurrentTryChar = this->Charset[ChrIdx];
    
            /* Make all cells as valid and itterate thru' them and mark those which are invalid */
            this->SetAvailableOnAllCells(1);
        
            /* Invalidate all lines and columns if already contain our symbol */
            for( int y = 0; y < this->Size; y++ )
            {
                for( int x = 0; x < this->Size; x++ )
                {
                    if(y == x)
                    {
                        if( AlreadyOnX(y, CurrentTryChar) )
                        {
                            /* Mark all elements on that column as unavailable */
                            for( int xIdx = 0; xIdx < this->Size; xIdx++ )
                            {
                                this->CellsMatrix[y][xIdx].IsAvailable = 0;
                            }
                        }
                        if( AlreadyOnY(x, CurrentTryChar) )
                        {
                            /* Mark all elements on that line as unavailable */
                            for( int yIdx = 0; yIdx < this->Size; yIdx++ )
                            {
                                this->CellsMatrix[yIdx][x].IsAvailable = 0;
                            }
                        }
                    }/*x==y*/
                }/* for x */
            }/* for y */

            
//            /* Invalidate all elements from a cell if current char is already contained */
//            for(int blockIdx = 0; blockIdx < this->Size; blockIdx++)
//            {
//                if(AlreadyOnBlock(blockIdx, CurrentTryChar))
//                {
//                    for( int y = 0; y < this->Size; y++ )
//                    {
//                        for( int x = 0; x < this->Size; x++ )
//                        {
//                            if( this->CellsMatrix[y][x].BlockNo == blockIdx )
//                            {
//                                this->CellsMatrix[y][x].IsAvailable = 0;
//                            }
//                        }
//                    }
//                }
//            }
        
            /* Now loop through all block and check whether there is only one element available - that would be an solution */
            for(int blockIdx = 0; blockIdx < this->Size; blockIdx++)
            {
                /* After all intersections there is only one cell remaining. If Currentchar is not conteinted by the block, then we have a solution */
                if( GetAvailableCellsOnBlock(blockIdx) == 1  && !AlreadyOnBlock(blockIdx, CurrentTryChar) )
                {
                    this->IncCellsSolved();

                    for( int y = 0; y < this->Size; y++ )
                    {
                        for( int x = 0; x < this->Size; x++ )
                        {
                            if( this->CellsMatrix[x][y].BlockNo == blockIdx )
                            {
                                if( this->CellsMatrix[x][y].IsAvailable == 1 )
                                {
                                    this->CellsMatrix[x][y].val = CurrentTryChar;
                                    this->CellsMatrix[x][y].IsAvailable = 0;
                                }
                            }
                        }
                    }
                    break;
                }
            }
            
            
//            /* Print debugging matrix */
//            printf("\n Current character: '%c' - available pozitions\n", CurrentTryChar);
//            for(int y = 0; y < this->Size; y++)
//            {
//                for( int x = 0; x < this->Size; x++)
//                {
//                    printf("%s%d " ANSI_COLOR_RESET, AsciiColors[CellsMatrix[y][x].BlockNo], this->CellsMatrix[y][x].IsAvailable);
//                }
//                printf("\n");
//            }
//            printf("\n");
//            fflush(stdout);
            
        
        } /* For every char in charset */
        int dummyBreak = 0;
    }
    
    void Algo_Blocks()
    {
        /* Itterate through all symbols */
        for(int ChrIdx = 0; ChrIdx < this->Size; ChrIdx++)
        {
            /* Current symbol */
            char CurrentTryChar = this->Charset[ChrIdx];
        
            /* Make all cells as valid and itterate thru' them and mark those which are invalid */
            this->SetAvailableOnAllCells(1);
        
            /* Invalidate all lines and columns if already contain our symbol */
            for( int y = 0; y < this->Size; y++ )
            {
                for( int x = 0; x < this->Size; x++ )
                {
                    if(y == x)
                    {
                        if( AlreadyOnX(y, CurrentTryChar) )
                        {
                            /* Mark all elements on that column as unavailable */
                            for( int xIdx = 0; xIdx < this->Size; xIdx++ )
                            {
                                this->CellsMatrix[y][xIdx].IsAvailable = 0;
                            }
                        }
                        if( AlreadyOnY(x, CurrentTryChar) )
                        {
                            /* Mark all elements on that line as unavailable */
                            for( int yIdx = 0; yIdx < this->Size; yIdx++ )
                            {
                                this->CellsMatrix[yIdx][x].IsAvailable = 0;
                            }
                        }
                    }/*x==y*/
                }/* for x */
            }/* for y */


//            /* Invalidate all elements from a cell if current char is already contained */
//            for(int blockIdx = 0; blockIdx < this->Size; blockIdx++)
//            {
//                if(AlreadyOnBlock(blockIdx, CurrentTryChar))
//                {
//                    for( int y = 0; y < this->Size; y++ )
//                    {
//                        for( int x = 0; x < this->Size; x++ )
//                        {
//                            if( this->CellsMatrix[y][x].BlockNo == blockIdx )
//                            {
//                                this->CellsMatrix[y][x].IsAvailable = 0;
//                            }
//                        }
//                    }
//                }
//            }
        
            /* Now loop through all block and check whether there is only one element available - that would be an solution */
            for(int blockIdx = 0; blockIdx < this->Size; blockIdx++)
            {
                /* After all intersections there is only one cell remaining. If Currentchar is not conteinted by the block, then we have a solution */
                if( GetAvailableCellsOnBlock(blockIdx) == 1  && !AlreadyOnBlock(blockIdx, CurrentTryChar) )
                {
                    this->IncCellsSolved();
                
                    for( int y = 0; y < this->Size; y++ )
                    {
                        for( int x = 0; x < this->Size; x++ )
                        {
                            if( this->CellsMatrix[x][y].BlockNo == blockIdx )
                            {
                                if( this->CellsMatrix[x][y].IsAvailable == 1 )
                                {
                                    this->CellsMatrix[x][y].val = CurrentTryChar;
                                    this->CellsMatrix[x][y].IsAvailable = 0;
                                }
                            }
                        }
                    }
                    break;
                }
            }


//            /* Print debugging matrix */
//            printf("\n Current character: '%c' - available pozitions\n", CurrentTryChar);
//            for(int y = 0; y < this->Size; y++)
//            {
//                for( int x = 0; x < this->Size; x++)
//                {
//                    printf("%s%d " ANSI_COLOR_RESET, AsciiColors[CellsMatrix[y][x].BlockNo], this->CellsMatrix[y][x].IsAvailable);
//                }
//                printf("\n");
//            }
//            printf("\n");
//            fflush(stdout);
        
        
        } /* For every char in charset */
        int dummyBreak = 0;
    }
    
    
    void Print_Info()
    {
        uint8_t TwoSolutionsCells = 0;
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                uint8_t SolutionsNumber = this->GetPossibleSolutions(x, y);
                if(SolutionsNumber > 0 && SolutionsNumber <= 3 )
                {
                    printf("[%d][%d] could have only %d solutions\n", y, x, SolutionsNumber);
                }
            }
        }
    }
    
    /* Basic solving when for 1 possibility in a cell */
    void IncCellsSolved()
    {
        this->CellsSolved++;
        /* Update visual board containing last solution */
        PrintBoard();
    }
    bool IsSolved()
    {
        for(int y = 0; y < this->Size; y++)
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL )
                {
                    return false;
                }
            }
        }
        return true;
    }
    bool IsPossibleSolution(uint8_t x, uint8_t y, char Solution)
    {
        /* Check whether it's safe to place this symbol on this vox */
        if( !this->AlreadyOnX(y, Solution) &&
            !this->AlreadyOnY(x, Solution) &&
            !this->AlreadyOnBlock(this->CellsMatrix[x][y].BlockNo, Solution) &&
            this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL)
        {
            return true;
        }
        return false;
    }
    bool AlreadyOnBlock(uint8_t BlockId, char TargetChar)
    {
        for(int y = 0; y < this->Size; y++)
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].BlockNo == BlockId && this->CellsMatrix[y][x].val == TargetChar)
                {
                    return true;
                }
            }
        }
        return false;
    }
    bool AlreadyOnX(uint8_t CurrY, char TargetChar)
    {
        for(int x = 0; x < this->Size; x++)
        {
            /* Skip current element */
            if( this->CellsMatrix[CurrY][x].val == TargetChar )
            {
                return true;
            }
        }
        return false;
    }
    bool AlreadyOnY(uint8_t CurrX, char TargetChar)
    {
        for(int y = 0; y < this->Size; y++)
        {
            /* Skip current element */
            if( this->CellsMatrix[y][CurrX].val == TargetChar )
            {
                return true;
            }
        }
        return false;
    }
    uint8_t GetPossibleSolutions(uint8_t x, uint8_t y)
    {
        uint8_t PossibleSollutions = 0;
        if( this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL )
        {
            for( int TryCharIdx = 0; TryCharIdx < this->Size; TryCharIdx++ )
            {
                /* Variable to store current char to be stored */
                char CurrentTryChar = Charset[TryCharIdx];
                /* Check whether it's safe to place this symbol on this vox */
                if( !this->AlreadyOnX(y, CurrentTryChar) &&
                    !this->AlreadyOnY(x, CurrentTryChar) &&
                    !this->AlreadyOnBlock(this->CellsMatrix[y][x].BlockNo, CurrentTryChar) )
                {
                    PossibleSollutions++;
                }
            }
        }
        return PossibleSollutions;
    }
    char GetNthSolution(uint8_t x, uint8_t y, uint8_t NTh = 0)
    {
        if( this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL )    /* If cell is still unsolved */
        {
            uint8_t PossibleSollutions = 0;
            for( int TryCharIdx = 0; TryCharIdx < this->Size; TryCharIdx++ )
            {
                /* Variable to store current char to be stored */
                char CurrentTryChar = Charset[TryCharIdx];
                /* Check whether it's safe to place this symbol on this vox */
                if( !this->AlreadyOnX(y, CurrentTryChar) &&
                    !this->AlreadyOnY(x, CurrentTryChar) &&
                    !this->AlreadyOnBlock(this->CellsMatrix[y][x].BlockNo, CurrentTryChar) )
                {
                    if( PossibleSollutions == NTh )
                    {
                        return CurrentTryChar;
                    }
            
                    PossibleSollutions++;
                }
            }
        }
        return this->CellsMatrix[y][x].val;
    }
    /* Advanced solving */
    uint8_t GetAvailableCellsOnBlock(uint8_t BlockId)
    {
        uint8_t Result = 0;
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].BlockNo == BlockId )
                {
                    if(this->CellsMatrix[y][x].IsAvailable == 1)
                    {
                        Result++;
                    }
                }
            }
        }
        return Result;
    }
    uint8_t SetAvailableOnAllCells(uint8_t available)
    {
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL )
                {
                    this->CellsMatrix[y][x].IsAvailable = 0;
                }
                else
                {
                    this->CellsMatrix[y][x].IsAvailable = available;
                }
            }
        }
    }
};

#endif // _SUDOKUBOARD_H_