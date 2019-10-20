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
#include <cstring>
#include <Exception.h>

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
    const char *Charset = "123456789abcdef\0"; //!< Must NOT contain 0 (zero)
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
    
    typedef struct
    {
        uint8_t x;
        uint8_t y;
    }coord_t;
    
    typedef struct
    {
        coord_t element1;
        coord_t element2;
        char Val;
    }small_pair_t;
    
    typedef struct
    {
        char Val;
        coord_t Coord;
    }possibility_t;

    /** Struct to define a basic cell used by user to input it's data */
    typedef struct
    {
        char val;
        uint8_t BlockNo;
        uint8_t IsAvailable;
        possibility_t PossibleSolutions[SUDOKU_MAX_SIZE];
        coord_t Coord;
    }cell_extended_t;
    
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
                this->CellsMatrix[i][j].Coord.y = i;
                this->CellsMatrix[i][j].Coord.x = j;
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
    
            Algo_UpdateCellPossibilities();
            if(this->IsSolved()) break;
            
//            Algo_SafeMoves();
//            if(this->IsSolved()) break;
            
//            Algo_CheckIntersections();
//            if(this->IsSolved()) break;
    
//            Algo_ColumnsAndRowsPossibilities();
//            if(this->IsSolved()) break;
            
            /* Update time elapsed */
            if( displayTimeTimer.ElapsedMs() >= 28 )
            {
                displayTimeTimer.Restart();
                this->PrintTimeElapsed();
            }
            
            /* Algorithms are blocked...find something better :) */
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
    /* Flag indicating thet possibilities table is updated or not */
    bool PossibilityTableUpdated = false;
    
    /* Algorithms to be applied */
    void Algo_SafeMoves()
    {
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                /* Only loop through unknown symbols */
                if( this->GetAllPossibleSolutions(x, y) == 1 )
                {
                    /* Write solution to corresponding cell */
                    this->CellsMatrix[y][x].val = this->GetNthSolution(x, y);
    
                    /* Update status */
                    this->IncCellsSolved();
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
                                    this->IncCellsSolved();
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
    
    void Algo_ColumnsAndRowsPossibilities()
    {
        /* Based on this method:
         * https://www.youtube.com/watch?v=ld0hChtBLno&t=592s
         * */
        
        /* Array to store number of possibilities for each cell */
        char Possibilities[SUDOKU_MAX_SIZE][SUDOKU_MAX_SIZE] = {0};
        
        /** *********************** */
        /* Checking the lines first */
        for( int y = 0; y < this->Size; y++ ) /* Each line */
        {
            /* Initialize array which will remember all possibilities */
            memset(Possibilities, 0, sizeof(Possibilities));
            
            for( int x = 0; x < this->Size; x++ ) /* Each element from line */
            {
                int PossibilitiesIndex = 0;
                
                /* If is already solved then go to next cell */
                if(this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL)
                    continue;
                
                /* Try each possible symbol and see how many of them could be here */
                for(int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
                {
                    char CurrentSymbol = this->Charset[chrIdx];
                    if( IsInPossibleSolutionsList(x, y, CurrentSymbol) )
                    {
                        Possibilities[x][PossibilitiesIndex++] = CurrentSymbol;
                    }
                }
            }
            
            /* Next thing to check is to identify the number that is not repeating. */
            char Unique = '-';
            uint8_t Apparitions[SUDOKU_MAX_SIZE] = {0};
            
            for( int x = 0; x < this->Size; x++ )
            {
                if(this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL)
                    continue;
                
                for(int i = 0; i < SUDOKU_MAX_SIZE; i++)
                {
                    if( Possibilities[x][i] == 0 ) /* Last element or no elements */
                        break;
                        
                    char currSymbol = Possibilities[x][i];
                    uint8_t currSymbolIndex = this->CharFromCharsetToIndex(currSymbol);
                    Apparitions[currSymbolIndex]++;
                }
            }
            
            /* Check for possible solutions */
            for(int apparitionsIdx = 0; apparitionsIdx < this->Size; apparitionsIdx++)
            {
                if( Apparitions[apparitionsIdx] == 1 )
                {
                    Unique = this->Charset[apparitionsIdx];
                    
                    for( int x = 0; x < this->Size; x++ )
                    {
                        /* Fill the table with new discovered solution and propagate result*/
                        if( this->IsInPossibleSolutionsList(x, y, Unique) )
                        {
                            this->CellsMatrix[y][x].val = Unique;
                            
                            this->IncCellsSolved();
                            
                            return; // Leave the function to reset algo
                        }
        
                    }/* each x */
                }
            }
            
            
            /* At this point, we have an array with all possibilities for all cells on the current row */
//            printf("Line: %d\n", y);
//            for( int x = 0; x < this->Size; x++ )
//            {
//                if(this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL)
//                    continue;
//
//                printf("\tRow %d: ", x);
//                for(int i = 0; i < SUDOKU_MAX_SIZE; i++)
//                {
//                    if( Possibilities[x][i] == 0 ) /* Last element or no elements */
//                        break;
//                    printf( "%c ", Possibilities[x][i] );
//                }
//                printf("\n");
//            }
//
//            printf("\tUnique solution: '%c'\n", Unique);
//            printf("\tAparitions: \n");
//            for(int i = 0; i < this->Size; i++)
//            {
//                if( Apparitions[i] > 0 )
//                {
//                    printf("\t\t'%c' = %d\n", this->Charset[i], Apparitions[i] );
//                }
//            }
            printf("\n");
            fflush(stdout);
        }
    
        int dummyBreakpoint2 = 0;
        
        /** *******************  */
        /* Checking the collumns */
        for( int x = 0; x < this->Size; x++ ) /* Each column */
        {
            /* Initialize array which will remember all possibilities */
            memset(Possibilities, 0, sizeof(Possibilities));
        
            for( int y = 0; y < this->Size; y++ ) /* Each element from line */
            {
                int PossibilitiesIndex = 0;
            
                /* If is already solved then go to next cell */
                if(this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL)
                    continue;
            
                /* Try each possible symbol and see how many of them could be here */
                for(int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
                {
                    char CurrentSymbol = this->Charset[chrIdx];
                    if( IsInPossibleSolutionsList(x, y, CurrentSymbol) )
                    {
                        Possibilities[y][PossibilitiesIndex++] = CurrentSymbol;
                    }
                }
            }
    
            /* Next thing to check is to identify the number that is not repeating. */
            char Unique = '-';
            uint8_t Apparitions[SUDOKU_MAX_SIZE] = {0};
    
            for( int y = 0; y < this->Size; y++ )
            {
                if(this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL)
                    continue;
        
                for(int i = 0; i < SUDOKU_MAX_SIZE; i++)
                {
                    if( Possibilities[y][i] == 0 ) /* Last element or no elements */
                        break;
            
                    char currSymbol = Possibilities[y][i];
                    uint8_t currSymbolIndex = this->CharFromCharsetToIndex(currSymbol);
                    Apparitions[currSymbolIndex]++;
                }
            }
            for(int i = 0; i < this->Size; i++)
            {
                if( Apparitions[i] == 1 )
                {
                    Unique = this->Charset[i];
                    
                    for( int y = 0; y < this->Size; y++ )
                    {
                        if( this->IsInPossibleSolutionsList(x, y, Unique) )
                        {
                            this->CellsMatrix[y][x].val = Unique;
                            /* Fill the table with new discovered solution and propagate result*/
                            this->IncCellsSolved();
                            return; // Leave the function to reset algo
                        }
        
                    }/* each x */
                }
            }
        
//            /* At this point, we have an array with all posibilities for all cells on the current column */
//            printf("Col: %d\n", x);
//            for( int y = 0; y < this->Size; y++ )
//            {
//                if(this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL)
//                    continue;
//
//                printf("\tRow %d: ", y);
//                for(int i = 0; i < SUDOKU_MAX_SIZE; i++)
//                {
//                    if( Possibilities[y][i] == 0 ) /* Last element or no elements */
//                        break;
//                    printf( "%c ", Possibilities[y][i] );
//                }
//                printf("\n");
//            }
//
//            printf("\tUnique solution: '%c'\n", Unique);
//            printf("\tAparitions: \n");
//            for(int i = 0; i < this->Size; i++)
//            {
//                if( Apparitions[i] > 0 )
//                {
//                    printf("\t\t'%c' = %d\n", this->Charset[i], Apparitions[i] );
//                }
//            }

            printf("\n");
            fflush(stdout);
        }
        
        int dummyBreakpoint = 0;
    }
    
    void Algo_UpdateCellPossibilities()
    {
        /* Update all possibilities on all cells without cross check. Some of them will be removed after crosscheck. */
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++)
            {
                ClearPossibleSolutions(x, y);
                GetAllPossibleSolutionsNoCrossCheck(x, y, this->CellsMatrix[y][x].PossibleSolutions);
            }
        }
        
        int PairsDetectionIterations = 5;
        while( PairsDetectionIterations > 0 ) // Just to be sure that all blocks gets updated
        {
            printf("Pairs detected on iteration: %d:\n", 5 - PairsDetectionIterations);
            for( int BlockId = 0; BlockId < this->Size; BlockId++ ) /* Loop through all blocks */
            {
                /* Matrix containing pairs of cells that are on the same line/row */
                small_pair_t SmallPairsFound[SUDOKU_MAX_SIZE] = {{0, 0}, {0, 0}, 0};
                this->GetSmallPairs(BlockId, SmallPairsFound);
                
                for( int pairIdx = 0; pairIdx < this->Size; pairIdx++ )
                {
                    if( SmallPairsFound[pairIdx].Val == 0) break;
                    printf("Block: %d, '%c' = [%d][%d] and [%d][%d]\n",BlockId, SmallPairsFound[pairIdx].Val, SmallPairsFound[pairIdx].element1.y, SmallPairsFound[pairIdx].element1.x, SmallPairsFound[pairIdx].element2.y, SmallPairsFound[pairIdx].element2.x);
                }
                
                /* Propagate restrictions which come by the pairs */
                for( int pairIdx = 0; pairIdx < this->Size; pairIdx++ )
                {
                    small_pair_t currPair = SmallPairsFound[pairIdx];
                    if( currPair.Val == 0 ) break; /* Break the loop if no more pairs there */

                    if( currPair.element1.x == currPair.element2.x ) /* Restrictions to be propagated over Y axis */
                    {
                        for( int y = 0; y < this->Size; y++ )
                        {
                            cell_extended_t &cell = this->CellsMatrix[y][currPair.element1.x];
                            if( cell.BlockNo == BlockId ) continue;
                            this->RemovePossibility(cell.Coord.x, cell.Coord.y, currPair.Val);
                        }
                    }
                    else if( currPair.element1.y == currPair.element2.y ) /* Restrictions to be propagated over X axis */
                    {
                        for( int x = 0; x < this->Size; x++ )
                        {
                            cell_extended_t &cell = this->CellsMatrix[currPair.element1.y][x];
                            if( cell.BlockNo == BlockId ) continue;
                            this->RemovePossibility(cell.Coord.x, cell.Coord.y, currPair.Val);
                        }
                    }
                }
            }/* for each block */
            printf("\n");
            
            PairsDetectionIterations--;
        }
        
        /* Validate changes on possibilities table for each cell */
        PossibilityTableUpdated = true;
        
        /* Display symbols for debugging*/
        printf("Matrix cells possiblities:\n");
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if(this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL) continue;
                
                printf("[%d][%d] = ", y, x);
                for( int psIdx = 0; psIdx < this->Size; psIdx++ )
                {
                    if( this->CellsMatrix[y][x].PossibleSolutions[psIdx].Val == 0 )
                        break;
                    
                    printf("%c ", this->CellsMatrix[y][x].PossibleSolutions[psIdx].Val);
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    
        
        int dummy = 0;
    
    }/*void Algo_UpdateCellPossibilities()*/
    
    void Print_Info()
    {
        uint8_t TwoSolutionsCells = 0;
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                uint8_t SolutionsNumber = this->GetAllPossibleSolutions(x, y);
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
        
        /* Update board possibilities every time a solution is being found */
        this->Algo_UpdateCellPossibilities();
        
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
    void RemovePossibility(uint8_t x, uint8_t y, char possibilityToRemove)
    {
        for( int pIdx = 0; pIdx < this->Size; pIdx++ )
        {
            possibility_t &posibility = this->CellsMatrix[y][x].PossibleSolutions[pIdx];
            if( posibility.Val == 0 )
                break;
            
            if( posibility.Val == possibilityToRemove)
            {
                /* Invalidate the possibility */
                posibility.Val = 0;
                
                /* Shift the other is end of array not reached */
                for(int i = pIdx; i < this->Size - 1; i++)
                {
                    this->CellsMatrix[y][x].PossibleSolutions[i] = this->CellsMatrix[y][x].PossibleSolutions[i + 1];
                }
                this->CellsMatrix[y][x].PossibleSolutions[this->Size-1].Val = 0;
                
            }
        }
    }
    bool IsInPossibleSolutionNoCrossCheck(uint8_t x, uint8_t y, char SolutionToCheck)
    {
        bool Result = false;
        if( !PossibilityTableUpdated )
        {
            /* Check whether it's safe to place this symbol on this vox */
            if( !this->AlreadyOnX(y, SolutionToCheck) &&
                !this->AlreadyOnY(x, SolutionToCheck) &&
                !this->AlreadyOnBlock(this->CellsMatrix[y][x].BlockNo, SolutionToCheck) &&
                this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL )
            {
                Result = true;
            }
        }
        return Result;
    }
    uint8_t GetAllPossibleSolutionsNoCrossCheck(uint8_t x, uint8_t y, possibility_t *PossibleSolutionsBuffer = nullptr)
    {
        uint8_t PossibleSollutions = 0;
        if( this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL )
        {
            for( int TryCharIdx = 0; TryCharIdx < this->Size; TryCharIdx++ )
            {
                /* Variable to store current char to be stored */
                char CurrentTryChar = Charset[TryCharIdx];
                /* Check whether it's safe to place this symbol on this vox */
                if( this->IsInPossibleSolutionNoCrossCheck(x, y, CurrentTryChar) )
                {
                    if( PossibleSolutionsBuffer != nullptr )
                    {
                        possibility_t possibility;
                        possibility.Val = CurrentTryChar;
                        possibility.Coord.y = y;
                        possibility.Coord.x = x;
                        PossibleSolutionsBuffer[ PossibleSollutions ++ ] = possibility;
                    }
                    else
                    {
                        PossibleSollutions++;
                    }
                }
                
            }
        }
        return PossibleSollutions;
    }
    
    bool IsInPossibleSolutionsList(uint8_t x, uint8_t y, char SolutionToCheck)
    {
        bool Result = false;
        for( int solIdx = 0; solIdx < this->Size; solIdx++ )
        {
            if( this->CellsMatrix[y][x].PossibleSolutions[solIdx].Val == SolutionToCheck )
            {
                Result = true;
            }
        }
        return Result;
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
    uint8_t GetAllPossibleSolutions(uint8_t x, uint8_t y, possibility_t *PossibleSolutionsBuffer = nullptr)
    {
        uint8_t PossibleSollutions = 0;
        if( this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL )
        {
            for( int TryCharIdx = 0; TryCharIdx < this->Size; TryCharIdx++ )
            {
                /* Variable to store current char to be stored */
                char CurrentTryChar = Charset[TryCharIdx];
                /* Check whether it's safe to place this symbol on this vox */
                if( this->IsInPossibleSolutionsList(x, y, CurrentTryChar) )
                {
                    if( PossibleSolutionsBuffer != nullptr )
                    {
                        possibility_t possibility;
                        possibility.Val = CurrentTryChar;
                        possibility.Coord.y = y;
                        possibility.Coord.x = x;
                        PossibleSolutionsBuffer[ PossibleSollutions ++ ] = possibility;
                    }
                    else
                    {
                        PossibleSollutions++;
                    }
                }
                
                
            }
        }
        return PossibleSollutions;
    }
    char GetNthSolution(uint8_t x, uint8_t y, uint8_t NTh = 0)
    {
        if( this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL || NTh > this->Size)    /* If cell is still unsolved */
        {
            if( this->CellsMatrix[y][x].PossibleSolutions[NTh].Val != 0 )
            {
                return this->CellsMatrix[y][x].PossibleSolutions[NTh].Val;
            }
        }
        throw Exception("GetNthSolution", "NTh solution does not exist!");
    }
    uint8_t CharFromCharsetToIndex(char c)
    {
        for( int index = 0; index < this->Size; index++)
        {
            if( this->Charset[index] == c)
            {
                return index;
            }
        }
        throw Exception("CharFromCharsetToIndex", "Char not present into array");
    }
    void GetBlockElements(uint8_t in_GroupId, coord_t *out_Elements)
    {
        uint8_t ElementsIndex = 0;
        coord_t tmpCoord;
        
        for( int y = 0; y < this-> Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if(this->CellsMatrix[y][x].BlockNo == in_GroupId)
                {
                    tmpCoord.x = x;
                    tmpCoord.y = y;
                    out_Elements[ElementsIndex++] = tmpCoord;
                }
            }
        }
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
    coord_t GetCoordByVal(uint8_t BlockID, char Val)
    {
        coord_t Result;
        
        for( int y = 0; y < this->Size; y++ )
        {
            for(int x = 0; x < this->Size; x++)
            {
                if( this->CellsMatrix[y][x].BlockNo == BlockID)
                {
                    if( this->CellsMatrix[y][x].val == Val )
                    {
                        Result.x = x;
                        Result.y = y;
                        return Result;
                    }
                }
            }
        }
        throw Exception("GetCoordByVal", "Value not present in selected group!");
    }
    void ClearPossibleSolutions(uint8_t X, uint8_t Y)
    {
        for( uint8_t idx = 0; idx < this->Size; idx++ )
        {
            this->CellsMatrix[Y][X].PossibleSolutions[idx].Val = 0;
        }
    }
    uint8_t GetSmallPairs(uint8_t GroupId, small_pair_t *Pairs)
    {
        uint8_t PairsFound = 0;
        
        /* Fetch all elements from current block */
        coord_t BlockCells[SUDOKU_MAX_SIZE];
        (void)this->GetBlockElements(GroupId, BlockCells);
        
        /* Look for pairs of two at least on the same line/column */
        for( uint8_t blockCellIdx = 0; blockCellIdx < this->Size; blockCellIdx++ )
        {
            uint8_t x1 = BlockCells[blockCellIdx].x;
            uint8_t y1 = BlockCells[blockCellIdx].y;
            /* Block cell already solved? */
            if( this->CellsMatrix[y1][x1].val != UNSOLVED_SYMBOL )
                continue;
            
            for( uint8_t blockCellIdx2 = blockCellIdx; blockCellIdx2 < this->Size; blockCellIdx2++ )
            {
                if( blockCellIdx == blockCellIdx2 ) // don't compare element with itself
                    continue;
                
                /* Coordinates of the current block */
                uint8_t x2 = BlockCells[blockCellIdx2].x;
                uint8_t y2 = BlockCells[blockCellIdx2].y;
                
                if( this->CellsMatrix[y2][x2].val != UNSOLVED_SYMBOL ) // also ignore already solved solutions
                    continue;
    
                /** Compare each possibility with each possibilities */
                {
                    /* Check how many time can this a possibility appear */
                    
                    for( int i = 0; i < this->Size; i++ )
                    {
                        if( this->CellsMatrix[y1][x1].PossibleSolutions[i].Val == 0 ) break;
                        
                        for( int j = 0; j < this->Size; j++ )
                        {
                            if( this->CellsMatrix[y2][x2].PossibleSolutions[j].Val == 0 ) break;
                            
                            char possibleSolution1 = this->CellsMatrix[y1][x1].PossibleSolutions[i].Val;
                            char possibleSolution2 = this->CellsMatrix[y2][x2].PossibleSolutions[j].Val;
    
                            if( (possibleSolution1 == possibleSolution2)  && ( (x1 == x2) || (y1==y2) ) )
                            {
                                small_pair_t potentialSmallPair;
                                potentialSmallPair.Val = possibleSolution1;
                                potentialSmallPair.element1.x = x1;
                                potentialSmallPair.element1.y = y1;
                                potentialSmallPair.element2.x = x2;
                                potentialSmallPair.element2.y = y2;
                                
                                bool ThirdElementOnDifferentLinColFound = false;
                                
                                /** If there ISN'T a third one on a different line/column then the solution is accepted. Otherwise solution is dropped */
                                for( int cellIdx = 0; cellIdx < this->Size; cellIdx++ )
                                {
                                    uint8_t x = BlockCells[cellIdx].x;
                                    uint8_t y = BlockCells[cellIdx].y;
                                    /* Block cell already solved? */
                                    if( this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL )
                                        continue;
                                    
                                    /* Ignore the cell if is the same as the other two already found */
                                    if( ((x == x1) && (y == y1)) || ((x==x2) && (y == y2)) )
                                        continue;
                                    
                                    for( int ps = 0; ps < this->Size; ps++ )
                                    {
                                        /* A third element with the same solution was found */
                                        if(this->CellsMatrix[y][x].PossibleSolutions[ps].Val == possibleSolution1)
                                        {
                                            /* Check whether it is in the same row */
                                            if(x1==x2)
                                            {
                                                if( x != x1 )
                                                {
                                                    ThirdElementOnDifferentLinColFound = true;
                                                    break;
                                                }
                                            }
                                            else if(y1 == y2)
                                            {
                                                if( y != y1 )
                                                {
                                                    ThirdElementOnDifferentLinColFound = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }/*each cell checking for third solution */
                                
                                if( !ThirdElementOnDifferentLinColFound )
                                {
                                    /* If the solution was already found already, just ignore this one to keep things simple */
                                    bool SolutionAlreadyFound = false;
                                    for( int sol = 0; sol < PairsFound; sol++ )
                                    {
                                       if( Pairs[sol].Val == potentialSmallPair.Val )
                                       {
                                           SolutionAlreadyFound = true;
                                       }
                                    }
                                    if( !SolutionAlreadyFound )
                                    {
                                        Pairs[PairsFound++] = potentialSmallPair;
                                    }
                                }
                            }/* two cells with the same solution? */
                        }/* j */
                    }/* i */
                    
                    
                }
                /** **/
            }
        }
        return PairsFound;
    }
};

#endif // _SUDOKUBOARD_H_