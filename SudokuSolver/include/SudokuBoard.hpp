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
#include <iostream>

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
} cell_base_t;

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
    
    typedef struct coord_t
    {
        uint8_t x;
        uint8_t y;
    
        bool operator==(coord_t const  &f) const
        {
           bool Result = false;
           if( (x == f.x) && (y == f.y) )
           {
                Result = true;
           }
           return Result;
        }
    
        bool operator!=(coord_t const  &f) const
        {
            bool Result = false;
            if( (x != f.x) || (y != f.y) )
            {
                Result = true;
            }
            return Result;
        }
    } coord_t;
    
    typedef struct
    {
        char Val;
        coord_t Coord;
    } possibility_t;
    
    /** Struct to define a basic cell used by user to input it's data */
    typedef struct
    {
        char val;
        uint8_t BlockNo;
        uint8_t IsAvailable;
        possibility_t PossibleSolutions[SUDOKU_MAX_SIZE];
        coord_t Coord;
    } cell_extended_t;
    
    typedef struct
    {
        cell_extended_t Cell1;
        cell_extended_t Cell2;
    }pair_t;
    
    typedef struct
    {
        coord_t element1;
        coord_t element2;
        char Val;
    } small_pair_t;
    
    typedef struct
    {
        coord_t element1;
        coord_t element2;
        char Val1;
        char Val2;
    } naked_pair_t;
    
    typedef struct
    {
        coord_t element1;
        coord_t element2;
        coord_t element3;
        char Val1;
        char Val2;
        char Val3;
    } naked_triplets_t;

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
        for( int i = 0; i < this->Size; i++ )
        {
            for( int j = 0; j <= this->Size; j++ )
            {
                this->CellsMatrix[i][j].val = (*((Cells + i * this->Size) + j)).val;
                this->CellsMatrix[i][j].BlockNo = (*((Cells + i * this->Size) + j)).BlockNo;
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
        if( FirstPrint == false )
        {
            /* Move cursor up to the first line */
            for( int i = 0; i < this->Size + 1; i++ )
            {
                printf("\33[A\r"); // Move cursor up one line
            }
            printf("\33[2K\r"); // Erase current line
        }
        /* Send to the output */
        fflush(stdout);
        
        std::string elapsedTimeStr = this->TimeCounter.ElapsedTimeString();
        /* Print time again */
        std::cout << elapsedTimeStr << std::endl;
        
        /* Send to the output */
        fflush(stdout);
        
        /* Move cursor back to the end */
        if( FirstPrint == false )
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
        if( FirstPrint == false )
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
    
            Algo_UpdatePossibilitiesTable();
            if( this->IsSolved() ) break;
            
            Algo_SafeMoves();
            if( this->IsSolved() ) break;
            
            Algo_CheckIntersections();
            if( this->IsSolved() ) break;
            
            Algo_ColumnsAndRowsPossibilities();
            if( this->IsSolved() ) break;
            
            Algo_NakedSingles();
            if( this->IsSolved() ) break;
            
            Algo_NextStrategy();
            if( this->IsSolved() ) break;
            
            /* Update time elapsed */
            this->PrintTimeElapsed();
            
            /* Algorithms are blocked...find something better :) */
            if( this->CellsSolved == StartProgress )
            {
                break;
            }
            
        }/*while*/
        
        PrintBoard();
        this->PrintTimeElapsed();
        if( !this->IsSolved() )
        {
            printf("Cells solved: %d. Can't solve from this point...\n\n", this->CellsSolved);
            Print_Info();
        }
    } /* Solve() */

protected:

private:
    const uint8_t Size = 0;
    cell_extended_t CellsMatrix[13][13];//**CellsMatrix;
    TimeUtils::Timer TimeCounter;
    uint16_t CellsSolved = 0;
    /* Flag indicating that possibilities table is updated or not */
    bool PossibilityTableUpdated = false;
    /* Flag indicating whether possibility table is fully updated or is updating */
    bool PossibilityTableUpdateRequest = true;
    
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
        for( int ChrIdx = 0; ChrIdx < this->Size; ChrIdx++ )
        {
            /* Current symbol */
            char CurrentTryChar = this->Charset[ChrIdx];
            
            /* Make all cells as valid and itterate thru' them and mark those which are invalid */
            this->SetAvailableOnAllCells(0);
            
            /* Get all cells that can have CurrentTryChar as solution */
            for( int y = 0; y < this->Size; y++ )
            {
                for( int x = 0; x < this->Size; x++ )
                {
                    if( IsInPossibleSolutionsList(x, y, CurrentTryChar) )
                    {
                        this->CellsMatrix[y][x].IsAvailable = 1;
                    }
                }
            }
            
            /* Now loop through all block and check whether there is only one element available - that would be an solution */
            for( int blockIdx = 0; blockIdx < this->Size; blockIdx++ )
            {
                /* After all intersections there is only one cell remaining. If Current char is not contained by the block, then we have a solution */
                if( GetAvailableCellsOnBlock(blockIdx) == 1 && !AlreadyOnBlock(blockIdx, CurrentTryChar) )
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
                                    int dummy = 0;
                                }
                            }
                        }
                    }
                    break;
                }
            }


//            /* Print debugging matrix */
//            printf("Current character: '%c' - available pozitions\n", CurrentTryChar);
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
        
        /** Get each cell from each line */
        for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
        {
            char CurrentChar = this->Charset[chrIdx];
            for( int y = 0; y < this->Size; y++ )
            {
                uint8_t Appearances = 0;
                for( int x = 0; x < this->Size; x++ )
                {
                    if( IsInPossibleSolutionsList(x, y, CurrentChar) )
                        Appearances++;
                }/*x*/
                if( Appearances == 1 ) /* 1 appearance = 1 solution */
                {
                    /* Go to the location of the single solution */
                    for( int x = 0; x < this->Size; x++ )
                    {
                        if( IsInPossibleSolutionsList(x, y, CurrentChar) )
                        {
                            this->CellsMatrix[y][x].val = CurrentChar;
                            this->IncCellsSolved();
                        }
                    }/*x*/
                }
            }/*y*/
        }/*chrIdx */
        
        /** Get each cell from each column */
        for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
        {
            char CurrentChar = this->Charset[chrIdx];
            for( int x = 0; x < this->Size; x++ )
            {
                uint8_t Appearances = 0;
                for( int y = 0; y < this->Size; y++ )
                {
                    if( IsInPossibleSolutionsList(x, y, CurrentChar) )
                        Appearances++;
                }/*x*/
                if( Appearances == 1 ) /* 1 appearance = 1 solution */
                {
                    /* Go to the location of the single solution */
                    for( int y = 0; y < this->Size; y++ )
                    {
                        if( IsInPossibleSolutionsList(x, y, CurrentChar) )
                        {
                            this->CellsMatrix[y][x].val = CurrentChar;
                            this->IncCellsSolved();
                        }
                    }/*x*/
                }
            }/*y*/
        }/*chrIdx */

//            printf("\tUnique solution: '%c'\n", Unique);
//            printf("\tAparitions: \n");
//            for(int i = 0; i < this->Size; i++)
//            {
//                if( Apparitions[i] > 0 )
//                {
//                    printf("\t\t'%c' = %d\n", this->Charset[i], Apparitions[i] );
//                }
//            }
        int dummyBreakpoint = 0;
    }
    
    void Algo_UpdatePossibilitiesTable()
    {
        /**
         * http://hodoku.sourceforge.net/en/tech_intersections.php#lc12
         *
         */
        
        /* Only to know when was called first time */
        PossibilityTableUpdated = true;
        
        /* Update all possibilities on all cells without cross check. Some of them will be removed after crosscheck. */
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                ClearPossibleSolutions(x, y);
                GetAllPossibleSolutionsNoCrossCheck(x, y, this->CellsMatrix[y][x].PossibleSolutions);
            }
        }
        
        /* While there are possibilitie sbeing removed, keep updating table */
        while( PossibilityTableUpdateRequest )
        {
            /* As long as table update process is not finished this will be set to true by algorithms */
            PossibilityTableUpdateRequest = false;
            
            /** Confirmed */
            Algo_LockedCandidates_Type1_Pointing();
            Algo_NakedPairs();
            
            /** Unconfirmed */
            Algo_LockedCandidates_Type2_Claiming();
            Algo_HiddenPairs();
        }
        
        /* To make sure next time will try to update as well */
        PossibilityTableUpdateRequest = true;
        
        int dummy = 0;
        
    }/*void Algo_UpdatePossibilitiesTable()*/
    
    void Algo_LockedCandidates_Type1_Pointing()
    {
        bool PrintLogs = false;
    
        for( int BlockId = 0; BlockId < this->Size; BlockId++ ) /* Loop through all blocks */
        {
            /* Matrix containing pairs of cells that are on the same line/row */
            small_pair_t SmallPairsFound[SUDOKU_MAX_SIZE] = {{0, 0}, {0, 0}, 0};
            this->GetSmallPairs(BlockId, SmallPairsFound);
        
            if( PrintLogs )
                for( int pairIdx = 0; pairIdx < this->Size; pairIdx++ )
                {
                    if( SmallPairsFound[pairIdx].Val == 0 ) break;
                    printf("Block: %d, '%c' = [%d][%d] and [%d][%d]\n", BlockId, SmallPairsFound[pairIdx].Val, SmallPairsFound[pairIdx].element1.y, SmallPairsFound[pairIdx].element1.x, SmallPairsFound[pairIdx].element2.y, SmallPairsFound[pairIdx].element2.x);
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
        if( PrintLogs ) printf("\n");
    
        if( PrintLogs )
        {
            /* Display symbols for debugging*/
            printf("Matrix cells possiblities:\n");
            for( int y = 0; y < this->Size; y++ )
            {
                for( int x = 0; x < this->Size; x++ )
                {
                    if( this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL ) continue;
                
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
        }
    }
    
    void Algo_LockedCandidates_Type2_Claiming()
    {
        /**
         * https://www.learn-sudoku.com/omission.html
         *
         */
        
        /* Based on this method:
         * https://www.youtube.com/watch?v=ld0hChtBLno&t=592s
         * */
        
        /** Get each cell from each line */
        for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
        {
            char CurrentChar = this->Charset[chrIdx];
            for( int y = 0; y < this->Size; y++ )
            {
                uint8_t Appearances = 0;
                uint8_t InLastBlock = 0;
                bool OmissionPossible = false;
                
                for( int x = 0; x < this->Size; x++ )
                {
                    
                    if( IsInPossibleSolutionsList(x, y, CurrentChar) )
                    {
                        if( Appearances == 0 )
                        {
                            InLastBlock = this->CellsMatrix[y][x].BlockNo;
                            Appearances++;
                        }
                        else
                        {
                            if( this->CellsMatrix[y][x].BlockNo == InLastBlock )
                            {
                                OmissionPossible = true;
                            }
                            else
                            {
                                OmissionPossible = false;
                                break;
                            }
                        }
                    }
                    
                    /* When last column is reached, verify the results */
                    if( x == this->Size - 1 )
                    {
                        if( OmissionPossible == true )
                        {
                            /* Remove the the other elements from the current cell as solution must be on this line */
                            coord_t BlockElements[SUDOKU_MAX_SIZE];
                            this->GetBlockElements(InLastBlock, BlockElements);
                            for( int elemIdx = 0; elemIdx < this->Size; elemIdx++ )
                            {
                                cell_extended_t &currentCell = this->CellsMatrix[BlockElements[elemIdx].y][BlockElements[elemIdx].x];
                                
                                if( currentCell.val != UNSOLVED_SYMBOL )
                                    continue;
                                
                                if( currentCell.Coord.y != y )
                                {
                                    this->RemovePossibility(currentCell.Coord.x, currentCell.Coord.y, CurrentChar);
                                }
                            }
                        }
                    }/* is last column?*/
                }/*x*/
            }/*y*/
        }/*chrIdx */
        
        /** Get each cell from each column */
        for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
        {
            char CurrentChar = this->Charset[chrIdx];
            for( int x = 0; x < this->Size; x++ )
            {
                uint8_t Appearances = 0;
                uint8_t InLastBlock = 0;
                bool OmissionPossible = false;
                
                for( int y = 0; y < this->Size; y++ )
                {
                    
                    if( IsInPossibleSolutionsList(x, y, CurrentChar) )
                    {
                        if( Appearances == 0 )
                        {
                            InLastBlock = this->CellsMatrix[y][x].BlockNo;
                            Appearances++;
                        }
                        else
                        {
                            if( this->CellsMatrix[y][x].BlockNo == InLastBlock )
                            {
                                OmissionPossible = true;
                            }
                            else
                            {
                                OmissionPossible = false;
                                break;
                            }
                        }
                    }
                    
                    /* When last line is reached, verify the results */
                    if( y == this->Size - 1 )
                    {
                        if( OmissionPossible == true )
                        {
                            /* Remove the the other elements from the current cell as solution must be on this column */
                            coord_t BlockElements[SUDOKU_MAX_SIZE];
                            this->GetBlockElements(InLastBlock, BlockElements);
                            for( int elemIdx = 0; elemIdx < this->Size; elemIdx++ )
                            {
                                cell_extended_t &currentCell = this->CellsMatrix[BlockElements[elemIdx].y][BlockElements[elemIdx].x];
                                
                                if( currentCell.val != UNSOLVED_SYMBOL )
                                    continue;
                                
                                if( currentCell.Coord.x != x )
                                {
                                    this->RemovePossibility(currentCell.Coord.x, currentCell.Coord.y, CurrentChar);
                                }
                            }
                        }
                    }/* is last line?*/
                }/*x*/
            }/*y*/
        }/*chrIdx */

//            printf("\tUnique solution: '%c'\n", Unique);
//            printf("\tAparitions: \n");
//            for(int i = 0; i < this->Size; i++)
//            {
//                if( Apparitions[i] > 0 )
//                {
//                    printf("\t\t'%c' = %d\n", this->Charset[i], Apparitions[i] );
//                }
//            }
        int dummyBreakpoint = 0;
        
    }
    
    void Algo_NakedSingles()
    {
        /*
         * https://www.learn-sudoku.com/lone-singles.html
         *
         * If there is a cell in a block with only one possibility then that is the solution.
         */
        for( int blockId = 0; blockId < this->Size; blockId++ )
        {
            coord_t BlockElements[SUDOKU_MAX_SIZE];
            for( int elementIdx = 0; elementIdx < this->GetBlockElements(blockId, BlockElements); elementIdx++ )
            {
                coord_t CurrentCell = BlockElements[elementIdx];
                if( GetAllPossibleSolutions(CurrentCell.x, CurrentCell.y) == 1 )
                {
                    this->CellsMatrix[CurrentCell.y][CurrentCell.x].val = this->CellsMatrix[CurrentCell.y][CurrentCell.x].PossibleSolutions[0].Val;
                    this->IncCellsSolved();
                }
            }
        }
    }
    
    void Algo_NakedPairs()
    {
        /**
         * https://www.learn-sudoku.com/naked-pairs.html
         */
        for( int BlockIdx = 0; BlockIdx < this->Size; BlockIdx++ )
        {
            uint8_t ElementsWithTwoSmallSolutionsNo = 0;
            coord_t ElementsWithTwoSmallSolutions[SUDOKU_MAX_SIZE];
            
            /* Fetch all elements that have maximum two solutions possible */
            coord_t BlockElements[SUDOKU_MAX_SIZE];
            for( int BlockElementIdx = 0; BlockElementIdx < this->GetBlockElements(BlockIdx, BlockElements); BlockElementIdx++ )
            {
                cell_extended_t &CurrentCell = this->CellsMatrix[BlockElements[BlockElementIdx].y][BlockElements[BlockElementIdx].x];
                /* Keep only cells with two solutions and which are not solved */
                if( GetAllPossibleSolutions(CurrentCell.Coord.x, CurrentCell.Coord.y) != 2 || CurrentCell.val != UNSOLVED_SYMBOL )
                    continue;
                
                ElementsWithTwoSmallSolutions[ElementsWithTwoSmallSolutionsNo++] = CurrentCell.Coord;
            }
            
            
            /* Store naked pairs found */
            uint8_t NakedPairsNo = 0;
            naked_pair_t NakedPairs[SUDOKU_MAX_SIZE / 2 + 1] = {0};
            
            /* Check whether there are naked pairs */
            for( int elementIdx = 0; elementIdx < ElementsWithTwoSmallSolutionsNo; elementIdx++ )
            {
                for( int elementIdx2 = elementIdx + 1; elementIdx2 < ElementsWithTwoSmallSolutionsNo; elementIdx2++ )
                {
                    coord_t element1, element2;
                    element1 = ElementsWithTwoSmallSolutions[elementIdx];
                    element2 = ElementsWithTwoSmallSolutions[elementIdx2];
                    
                    if( this->CellsMatrix[element1.y][element1.x].PossibleSolutions[0].Val == this->CellsMatrix[element2.y][element2.x].PossibleSolutions[0].Val
                        && this->CellsMatrix[element1.y][element1.x].PossibleSolutions[1].Val == this->CellsMatrix[element2.y][element2.x].PossibleSolutions[1].Val )
                    {
                        naked_pair_t Pair;
                        Pair.element1 = element1;
                        Pair.element2 = element2;
                        Pair.Val1 = this->CellsMatrix[element1.y][element1.x].PossibleSolutions[0].Val;
                        Pair.Val2 = this->CellsMatrix[element1.y][element1.x].PossibleSolutions[1].Val;
                        
                        NakedPairs[NakedPairsNo++] = Pair;
                    }
                }
            }
            
            /* In case there were naked pairs, remove update possibilities table */
            for( int NakedPairIdx = 0; NakedPairIdx < NakedPairsNo; NakedPairIdx++ )
            {
                naked_pair_t CurrentPair = NakedPairs[NakedPairIdx];
                
                for( int BlockElementIdx = 0; BlockElementIdx < this->GetBlockElements(BlockIdx, BlockElements); BlockElementIdx++ )
                {
                    cell_extended_t &CurrentCell = this->CellsMatrix[BlockElements[BlockElementIdx].y][BlockElements[BlockElementIdx].x];
                    /* Ignore cells alread solved */
                    if( (CurrentCell.val != UNSOLVED_SYMBOL) ) continue;
                    
                    /* Keep only cells with two solutions and are different compared to pair cells already found naked*/
                    if( ((CurrentCell.Coord.x == CurrentPair.element1.x) && (CurrentCell.Coord.y == CurrentPair.element1.y)) ||
                        ((CurrentCell.Coord.x == CurrentPair.element2.x) && (CurrentCell.Coord.y == CurrentPair.element2.y)) )
                        continue;
                    
                    this->RemovePossibility(CurrentCell.Coord.x, CurrentCell.Coord.y, CurrentPair.Val1);
                    this->RemovePossibility(CurrentCell.Coord.x, CurrentCell.Coord.y, CurrentPair.Val2);
                }
            }
        }
    }
    
    void Algo_HiddenPairs()
    {
        /**
         * http://hodoku.sourceforge.net/en/tech_hidden.php
         */
        
        /** Check each line */
        for( int y = 0; y < this->Size; y++ )
        {
            int PairsFoundNo = 0;
            pair_t PairsFound[SUDOKU_MAX_SIZE];
            for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
            {
                char CurrentSymbol = this->Charset[chrIdx];
                cell_extended_t Cell1 = {0}, Cell2 = {0};
                int CellsFound = 0;
                
                for( int x = 0; x < this->Size; x++ )
                {
                    if( this->IsInPossibleSolutionsList(x, y, CurrentSymbol) )
                    {
                        if( CellsFound++ == 0 )
                        {
                            Cell1 = this->CellsMatrix[y][x];
                        }
                        else if ( CellsFound == 1 )
                        {
                            Cell2 = this->CellsMatrix[y][x];
                        }
                        else
                        {
                            break;
                        }
                    }
                }/*x*/
                
                if( CellsFound == 2)
                {
                    PairsFound[PairsFoundNo].Cell1 = Cell1;
                    PairsFound[PairsFoundNo].Cell2 = Cell2;
                    PairsFoundNo++;
                }
                
                /* Check the pairs found for current line */
                for( int pairIdx = 0; pairIdx < PairsFoundNo; pairIdx++ )
                {
                    pair_t pair1 = PairsFound[pairIdx];
                    for( int pairIdx2 = pairIdx; pairIdx2 < PairsFoundNo; pairIdx2++ )
                    {
                        pair_t pair2 = PairsFound[pairIdx2];
                        
                        /* If the pairs are on the same coordinates then we have a "hidden pair */
                        if(  ( (pair1.Cell1.Coord == pair2.Cell1.Coord) && (pair1.Cell2.Coord == pair2.Cell2.Coord) ) ||
                                ( (pair1.Cell1.Coord == pair2.Cell2.Coord) && (pair1.Cell2.Coord == pair2.Cell1.Coord) ))
                        {
                            /* A pair was found - delete those possibilities from other cells */
                            for( int x = 0; x < this->Size; x++ )
                            {
                                cell_extended_t CurrentCell = this->CellsMatrix[y][x];
                                if(  (CurrentCell.Coord != pair1.Cell1.Coord && CurrentCell.Coord != pair1.Cell2.Coord) &&
                                        (CurrentCell.Coord != pair2.Cell1.Coord && CurrentCell.Coord != pair2.Cell2.Coord))
                                {
                                    this->RemovePossibility( x, y, pair1.Cell1.val );
                                    this->RemovePossibility( x, y, pair2.Cell1.val );
                                }
                            }
                        }
                    }
                }
                
            }/*chrIdx*/
        }/*y*/
        
    }
    
    void Algo_NakedTriplets()
    {
        /**
         * https://www.learn-sudoku.com/naked-pairs.html
         */
        for( int BlockIdx = 0; BlockIdx < this->Size; BlockIdx++ )
        {
            uint8_t ElementsWithThreeSmallSolutionsNo = 0;
            coord_t ElementsWithThreeSmallSolutions[SUDOKU_MAX_SIZE];
            
            /* Fetch all elements that have maximum two solutions possible */
            coord_t BlockElements[SUDOKU_MAX_SIZE];
            for( int BlockElementIdx = 0; BlockElementIdx < this->GetBlockElements(BlockIdx, BlockElements); BlockElementIdx++ )
            {
                cell_extended_t &CurrentCell = this->CellsMatrix[BlockElements[BlockElementIdx].y][BlockElements[BlockElementIdx].x];
                /* Keep only cells with two solutions and which are not solved */
                if( GetAllPossibleSolutions(CurrentCell.Coord.x, CurrentCell.Coord.y) != 3 || CurrentCell.val != UNSOLVED_SYMBOL )
                    continue;
                
                ElementsWithThreeSmallSolutions[ElementsWithThreeSmallSolutionsNo++] = CurrentCell.Coord;
            }
            
            return;
            
            /* Store naked pairs found */
            uint8_t NakedTripletsNo = 0;
            naked_triplets_t NakedTriplets[SUDOKU_MAX_SIZE / 2 + 1] = {0};
            
            /* Check whether there are naked pairs */
            for( int elementIdx = 0; elementIdx < ElementsWithThreeSmallSolutionsNo; elementIdx++ )
            {
                for( int elementIdx2 = elementIdx + 1; elementIdx2 < ElementsWithThreeSmallSolutionsNo; elementIdx2++ )
                {
                    coord_t element1, element2;
                    element1 = ElementsWithThreeSmallSolutions[elementIdx];
                    element2 = ElementsWithThreeSmallSolutions[elementIdx2];
                    
                    if( this->CellsMatrix[element1.y][element1.x].PossibleSolutions[0].Val == this->CellsMatrix[element2.y][element2.x].PossibleSolutions[0].Val
                        && this->CellsMatrix[element1.y][element1.x].PossibleSolutions[1].Val == this->CellsMatrix[element2.y][element2.x].PossibleSolutions[1].Val )
                    {
                        naked_triplets_t Pair;
                        Pair.element1 = element1;
                        Pair.element2 = element2;
                        Pair.Val1 = this->CellsMatrix[element1.y][element1.x].PossibleSolutions[0].Val;
                        Pair.Val2 = this->CellsMatrix[element1.y][element1.x].PossibleSolutions[1].Val;
                        
                        NakedTriplets[NakedTripletsNo++] = Pair;
                    }
                }
            }
            
            /* In case there were naked triplets, remove update possibilities table */
            for( int NakedPairIdx = 0; NakedPairIdx < NakedTripletsNo; NakedPairIdx++ )
            {
                naked_triplets_t CurrentPair = NakedTriplets[NakedPairIdx];
                
                for( int BlockElementIdx = 0; BlockElementIdx < this->GetBlockElements(BlockIdx, BlockElements); BlockElementIdx++ )
                {
                    cell_extended_t &CurrentCell = this->CellsMatrix[BlockElements[BlockElementIdx].y][BlockElements[BlockElementIdx].x];
                    /* Ignore cells alread solved */
                    if( (CurrentCell.val != UNSOLVED_SYMBOL) ) continue;
                    
                    /* Keep only cells with two solutions and are different compared to pair cells already found naked*/
                    if( ((CurrentCell.Coord.x == CurrentPair.element1.x) && (CurrentCell.Coord.y == CurrentPair.element1.y)) ||
                        ((CurrentCell.Coord.x == CurrentPair.element2.x) && (CurrentCell.Coord.y == CurrentPair.element2.y)) )
                        continue;
                    
                    this->RemovePossibility(CurrentCell.Coord.x, CurrentCell.Coord.y, CurrentPair.Val1);
                    this->RemovePossibility(CurrentCell.Coord.x, CurrentCell.Coord.y, CurrentPair.Val2);
                }
            }
        }
    }
    
    void Algo_NextStrategy()
    {
        /**
         * https://www.youtube.com/watch?v=AwBdgHqUmMQ - 5:15
         *
         * https://www.conceptispuzzles.com/index.aspx?uri=puzzle/sudoku/techniquesif w
         *
         * Refference: https://www.learn-sudoku.com/hidden-singles.html
         *
         * Techniques: https://www.sudoklue.com/m4/
         *  1. Naked single
         *  2. Hidden Single
         *  3. Naked Pair
         *  4. Omission
         *  5. Naked triplet
         *  6. Hidden pair
         *  7. Naked Quad
         *  8. Hidden triplet
         *  9. Hidden quad
         *  10. BUG Type 1
         *  11. X - wing
         *  12. Swordfish
         *  13. Uniq Rectangle 1
         *  14. XY-Wing
         *  15. XYZ Wing
         *  16. BUG Type 2
         *  17. Forcing Chain1
         *  18. Forcing Chain2
         *  19. Forcing Chain3
         *
         */
        
        /**
         *
         * Implemented here: Check every block. Is there is any symbol repeating only once then that must be a solution.
         *
         */
    }
    
    void Print_Info()
    {
        uint8_t TwoSolutionsCells = 0;
        
        printf("BOARD INFO:\n");
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                possibility_t possibileSolutions[SUDOKU_MAX_SIZE];
                uint8_t SolutionsNumber = this->GetAllPossibleSolutions(x, y, possibileSolutions);
                if( SolutionsNumber > 0 )// && SolutionsNumber <= 3 )
                {
                    printf("[%d][%d] could have only %d solutions: ", y, x, SolutionsNumber);
                    for( int i = 0; i < SolutionsNumber; i++ )
                    {
                        {
                            printf("%c ", possibileSolutions[i].Val);
                        }
                    }
                    printf("\n");
                }
            }
        }
        printf("\n");
        
        /* Print pairs per block */
        for( int BlockId = 0; BlockId < this->Size; BlockId++ )
        {
            small_pair_t pairs[SUDOKU_MAX_SIZE];
            uint8_t pairsFound = this->GetSmallPairs(BlockId, pairs);
            
            printf("Block: %d have %d pairs:\n", BlockId, pairsFound);
            for( int pairIdx = 0; pairIdx < pairsFound; pairIdx++ )
            {
                printf("\t'%c' = [%d][%d] and [%d][%d]\n", pairs[pairIdx].Val, pairs[pairIdx].element1.y, pairs[pairIdx].element1.x, pairs[pairIdx].element2.y, pairs[pairIdx].element2.x);
            }
        }
        printf("\n");
    }

/* Basic solving when for 1 possibility in a cell */
    void IncCellsSolved()
    {
        this->CellsSolved++;
        
        /* Update board possibilities every time a solution is being found */
        this->Algo_UpdatePossibilitiesTable();
        
        /* Update visual board containing last solution */
        PrintBoard();
    }
    bool IsSolved()
    {
        for( int y = 0; y < this->Size; y++ )
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
            
            if( posibility.Val == possibilityToRemove )
            {
                /* Indicate that the rest of algorithms needs to check with this update */
                PossibilityTableUpdateRequest = true;
                
                /* Invalidate the possibility */
                posibility.Val = 0;
                
                /* Shift the other is end of array not reached */
                for( int i = pIdx; i < this->Size - 1; i++ )
                {
                    this->CellsMatrix[y][x].PossibleSolutions[i] = this->CellsMatrix[y][x].PossibleSolutions[i + 1];
                }
                this->CellsMatrix[y][x].PossibleSolutions[this->Size - 1].Val = 0;
                
            }
        }
    }
    bool IsInPossibleSolutionNoCrossCheck(uint8_t x, uint8_t y, char SolutionToCheck)
    {
        bool Result = false;
        /* Check whether it's safe to place this symbol on this vox */
        if( !this->AlreadyOnX(y, SolutionToCheck) &&
            !this->AlreadyOnY(x, SolutionToCheck) &&
            !this->AlreadyOnBlock(this->CellsMatrix[y][x].BlockNo, SolutionToCheck) &&
            this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL )
        {
            Result = true;
        }
        return Result;
    }
    bool AlreadyOnBlock(uint8_t BlockId, char TargetChar)
    {
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].BlockNo == BlockId && this->CellsMatrix[y][x].val == TargetChar )
                {
                    return true;
                }
            }
        }
        return false;
    }
    bool AlreadyOnX(uint8_t CurrY, char TargetChar)
    {
        for( int x = 0; x < this->Size; x++ )
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
        for( int y = 0; y < this->Size; y++ )
        {
            /* Skip current element */
            if( this->CellsMatrix[y][CurrX].val == TargetChar )
            {
                return true;
            }
        }
        return false;
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
                        PossibleSolutionsBuffer[PossibleSollutions++] = possibility;
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
        if( this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL )
        {
            for( int solIdx = 0; solIdx < this->Size; solIdx++ )
            {
                if( this->CellsMatrix[y][x].PossibleSolutions[solIdx].Val == 0 )
                {
                    break;
                }
                
                if( this->CellsMatrix[y][x].PossibleSolutions[solIdx].Val == SolutionToCheck )
                {
                    Result = true;
                    break;
                }
            }
        }
        return Result;
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
                        PossibleSolutionsBuffer[PossibleSollutions++] = possibility;
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
        if( this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL || NTh > this->Size )    /* If cell is still unsolved */
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
        for( int index = 0; index < this->Size; index++ )
        {
            if( this->Charset[index] == c )
            {
                return index;
            }
        }
        throw Exception("CharFromCharsetToIndex", "Char not present into array");
    }
    uint8_t GetBlockElements(uint8_t in_GroupId, coord_t *out_Elements)
    {
        uint8_t ElementsIndex = 0;
        coord_t tmpCoord;
        
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].BlockNo == in_GroupId )
                {
                    tmpCoord.x = x;
                    tmpCoord.y = y;
                    out_Elements[ElementsIndex++] = tmpCoord;
                }
            }
        }
        return ElementsIndex;
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
                    if( this->CellsMatrix[y][x].IsAvailable == 1 )
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
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].BlockNo == BlockID )
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
        (void) this->GetBlockElements(GroupId, BlockCells);
        
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
                            
                            if( (possibleSolution1 == possibleSolution2) && ((x1 == x2) || (y1 == y2)) )
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
                                    if( ((x == x1) && (y == y1)) || ((x == x2) && (y == y2)) )
                                        continue;
                                    
                                    for( int ps = 0; ps < this->Size; ps++ )
                                    {
                                        /* A third element with the same solution was found */
                                        if( this->CellsMatrix[y][x].PossibleSolutions[ps].Val == possibleSolution1 )
                                        {
                                            /* Check whether it is in the same row */
                                            if( x1 == x2 )
                                            {
                                                if( x != x1 )
                                                {
                                                    ThirdElementOnDifferentLinColFound = true;
                                                    break;
                                                }
                                            }
                                            else if( y1 == y2 )
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