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
        
        bool operator==(coord_t const &f) const
        {
            bool Result = false;
            if( (x == f.x) && (y == f.y) )
            {
                Result = true;
            }
            return Result;
        }
        
        bool operator!=(coord_t const &f) const
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
        possibility_t PossibleSolutions[SUDOKU_MAX_SIZE];
        coord_t Coord;
        
        uint8_t GetSolutionsNumber()
        {
            uint8_t Result = 0;
            if( val == UNSOLVED_SYMBOL )
            {
                for( uint8_t solIdx = 0; solIdx < SUDOKU_MAX_SIZE; solIdx++ )
                {
                    if( PossibleSolutions[solIdx].Val == 0 )
                        break;
                    Result++;
                }
            }
            
            return Result;
        }
        bool ContainSolution(char Solution)
        {
            bool Result = false;
            if( val == UNSOLVED_SYMBOL )
            {
                for( int solIdx = 0; solIdx < SUDOKU_MAX_SIZE; solIdx++ )
                {
                    if( PossibleSolutions[solIdx].Val == 0 )
                        break;
                    
                    if( PossibleSolutions[solIdx].Val == Solution )
                    {
                        Result = true;
                        break;
                    }
                }
            }
            return Result;
        }
        bool ContainSolutions(char *Solutions, uint8_t SolutionsNumber)
        {
            bool Result = false;
            if( val == UNSOLVED_SYMBOL )
            {
                for( int solIdx = 0; solIdx < SolutionsNumber; solIdx++ )
                {
                    if( !ContainSolution(Solutions[solIdx]) )
                    {
                        Result = false;
                        break;
                    }
                }
            }
            return Result;
        }
        uint8_t GetSolutions(char *Solutions)
        {
            uint8_t SolutionsNo = 0;
            if( val != UNSOLVED_SYMBOL )
            {
                for( int solIdx = 0; solIdx < SUDOKU_MAX_SIZE; solIdx++ )
                {
                    if( PossibleSolutions[solIdx].Val == 0 )
                        break;
                    
                    Solutions[SolutionsNo++] = PossibleSolutions[solIdx].Val;
                }
            }
            return SolutionsNo;
        }
        bool ContainExactSolutions(char *Solutions)
        {
        
        }
    } cell_extended_t;
    
    typedef struct
    {
        cell_extended_t Cell1;
        cell_extended_t Cell2;
    } pair_t;
    
    typedef struct
    {
        cell_extended_t Cell1;
        cell_extended_t Cell2;
        char CommonSolution;
    } possibility_pair_t;
    
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
            
            Algo_FullHouse_LastDigit();
            if( this->IsSolved() ) break;
            
            Algo_HiddenSingles();
            if( this->IsSolved() ) break;

            Algo_NakedSingles();
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
        else
        {
            printf("SUCCESS! Puzzle solved!\n");
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
//            Algo_NakedPairs();
            
            /** Unconfirmed */
            Algo_LockedCandidates_Type2_Claiming();
//            Algo_HiddenPairs();
//            Algo_NakedTriplets();
        }
        
        /* To make sure next time will try to update as well */
        PossibilityTableUpdateRequest = true;
        
        int dummy = 0;
        
    }/*void Algo_UpdatePossibilitiesTable()*/
    
    void Algo_FullHouse_LastDigit()
    {
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                /* Only loop through unknown symbols */
                if( this->CellsMatrix[y][x].GetSolutionsNumber() == 1 )
                {
                    /* Write solution to corresponding cell */
                    this->CellsMatrix[y][x].val = this->CellsMatrix[y][x].PossibleSolutions[0].Val;
                    
                    /* Update status */
                    this->IncCellsSolved();
                }
                
            }/*X*/
        } /*Y*/
    }
    
    void Algo_HiddenSingles()
    {
        for( int ChrIdx = 0; ChrIdx < this->Size; ChrIdx++ )
        {
            /* Current symbol */
            char CurrentTryChar = this->Charset[ChrIdx];
    
            /** Lines */
            for( int y = 0; y < this->Size; y++ )
            {
                cell_extended_t CellsContainingSymbol[SUDOKU_MAX_SIZE] = {0};
                uint8_t CellsContainingSymbolNo = this->GetApparitionsOnX(y, CurrentTryChar, CellsContainingSymbol);
                if( CellsContainingSymbolNo == 1 ) /* If only one cell in this house contains this symbol, the solution must be this symbol */
                {
                    /* Write solution in it's corresponding cell */
                    this->CellsMatrix[y][CellsContainingSymbol[0].Coord.x].val = CurrentTryChar;
                    
                    /* Notify the others about finding */
                    this->IncCellsSolved();
                }
            }
    
            /** Columns */
            for( int x = 0; x < this->Size; x++ )
            {
                cell_extended_t CellsContainingSymbol[SUDOKU_MAX_SIZE] = {0};
                uint8_t CellsContainingSymbolNo = this->GetApparitionsOnY(x, CurrentTryChar, CellsContainingSymbol);
                if( CellsContainingSymbolNo == 1 ) /* If only one cell in this house contains this symbol, the solution must be this symbol */
                {
                    /* Write solution in it's corresponding cell */
                    this->CellsMatrix[CellsContainingSymbol[0].Coord.y][x].val = CurrentTryChar;
            
                    /* Notify the others about finding */
                    this->IncCellsSolved();
                }
            }
            
            /** Blocks */
            for( int GroupId = 0; GroupId < this->Size; GroupId++ )
            {
                cell_extended_t GroupCellsContainingSymbol[SUDOKU_MAX_SIZE] = {0};
                uint8_t GroupCellsContainingSymbolNo = 0;
                GroupCellsContainingSymbolNo = this->GetApparitionsOnBlock(GroupId, CurrentTryChar, GroupCellsContainingSymbol);
                if( GroupCellsContainingSymbolNo == 1 ) /* If only one cell in this house contains this symbol, the solution must be this symbol */
                {
                    /* Write solution in it's corresponding cell */
                    this->CellsMatrix[GroupCellsContainingSymbol[0].Coord.y][GroupCellsContainingSymbol[0].Coord.x].val = CurrentTryChar;
        
                    /* Notify the others about finding */
                    this->IncCellsSolved();
                }
            }
            
            
        } /* For every char in charset */
    }
    
    void Algo_NakedSingles()
    {
        /*
         * https://www.learn-sudoku.com/lone-singles.html
         *
         * If there is a cell in a block with only one possibility then that is the solution.
         */
        
        /** Lines */
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
        
        /** Columns */
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
    
        /** Blocks */
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
        

//            printf("\tUnique solution: '%c'\n", Unique);
//            printf("\tAparitions: \n");
//            for(int i = 0; i < this->Size; i++)
//            {
//                if( Apparitions[i] > 0 )
//                {
//                    printf("\t\t'%c' = %d\n", this->Charset[i], Apparitions[i] );
//                }
//            }
    }
    
    void Algo_LockedCandidates_Type1_Pointing()
    {
        bool PrintLogs = false;
        
        for( int BlockId = 0; BlockId < this->Size; BlockId++ ) /* Loop through all blocks */
        {
            /* Matrix containing pairs of cells that are on the same line/row */
            possibility_pair_t SmallPairsFound[SUDOKU_MAX_SIZE] = {0};
            this->GetSmallPairs(BlockId, SmallPairsFound);
            
            if( PrintLogs )
                for( int pairIdx = 0; pairIdx < this->Size; pairIdx++ )
                {
                    if( SmallPairsFound[pairIdx].CommonSolution == 0 ) break;
                    printf("Block: %d, '%c' = [%d][%d] and [%d][%d]\n", BlockId, SmallPairsFound[pairIdx].CommonSolution, SmallPairsFound[pairIdx].Cell1.Coord.y,
                           SmallPairsFound[pairIdx].Cell1.Coord.x, SmallPairsFound[pairIdx].Cell2.Coord.y, SmallPairsFound[pairIdx].Cell2.Coord.x);
                }
            
            /* Propagate restrictions which come by the pairs */
            for( int pairIdx = 0; pairIdx < this->Size; pairIdx++ )
            {
                possibility_pair_t currPair = SmallPairsFound[pairIdx];
                if( currPair.CommonSolution == 0 ) break; /* Break the loop if no more pairs there */
                
                if( currPair.Cell1.Coord.x == currPair.Cell2.Coord.x ) /* Restrictions to be propagated over Y axis */
                {
                    for( int y = 0; y < this->Size; y++ )
                    {
                        cell_extended_t &cell = this->CellsMatrix[y][currPair.Cell1.Coord.x];
                        if( cell.BlockNo == BlockId ) continue;
                        this->RemovePossibility(cell.Coord.x, cell.Coord.y, currPair.CommonSolution);
                    }
                }
                else if( currPair.Cell1.Coord.y == currPair.Cell2.Coord.y ) /* Restrictions to be propagated over X axis */
                {
                    for( int x = 0; x < this->Size; x++ )
                    {
                        cell_extended_t &cell = this->CellsMatrix[currPair.Cell1.Coord.y][x];
                        if( cell.BlockNo == BlockId ) continue;
                        this->RemovePossibility(cell.Coord.x, cell.Coord.y, currPair.CommonSolution);
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
                        if( OmissionPossible )
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
                        if( OmissionPossible )
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
    
    void Algo_NakedPairs()
    {
        /**
         * http://hodoku.sourceforge.net/en/tech_naked.php
         */
        
        /** Lines */
        for( int y = 0; y < this->Size; y++ )
        {
            /* Keep only keep only which have 2 possibilities */
            int CellsWithTwoSolutionsNo = 0;
            cell_extended_t CellsWithTwoSolutions[SUDOKU_MAX_SIZE] = {0};
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].GetSolutionsNumber() == 2 )
                {
                    CellsWithTwoSolutions[CellsWithTwoSolutionsNo++] = this->CellsMatrix[y][x];
                }
            }
            
            /* Check whether we have solutions overlapping */
            for( int cell1Idx = 0; cell1Idx < CellsWithTwoSolutionsNo; cell1Idx++ )
            {
                cell_extended_t cell1 = CellsWithTwoSolutions[cell1Idx];
                for( int cell2Idx = cell1Idx + 1; cell2Idx < CellsWithTwoSolutionsNo; cell2Idx++ )
                {
                    cell_extended_t cell2 = CellsWithTwoSolutions[cell2Idx];
                    
                    if( HaveSameSolutions(&cell1, &cell2) )
                    {
                        /* Remove all possibilities os cell1.val and cell2.val from the rest of lines */
                        for( int x = 0; x < this->Size; x++ )
                        {
                            if( (this->CellsMatrix[y][x].Coord == cell1.Coord) || (this->CellsMatrix[y][x].Coord == cell2.Coord) ) /* Ignore cell1 and cell2 */
                            {
                                continue;
                            }
                            char exceptions[2];
                            exceptions[0] = cell1.PossibleSolutions[0].Val;
                            exceptions[1] = cell1.PossibleSolutions[1].Val;
                            this->RemoveAllPosibilitiesExcept(x, y, exceptions, 2);
                        }
                    }
                }
            }
        }/* Lines */
        
        /** Columns */
        for( int x = 0; x < this->Size; x++ )
        {
            /* Keep only keep only which have 2 possibilities */
            int CellsWithTwoSolutionsNo = 0;
            cell_extended_t CellsWithTwoSolutions[SUDOKU_MAX_SIZE] = {0};
            for( int y = 0; y < this->Size; y++ )
            {
                if( this->CellsMatrix[y][x].GetSolutionsNumber() == 2 )
                {
                    CellsWithTwoSolutions[CellsWithTwoSolutionsNo++] = this->CellsMatrix[y][x];
                }
            }
            
            /* Check whether we have solutions overlapping */
            for( int cell1Idx = 0; cell1Idx < CellsWithTwoSolutionsNo; cell1Idx++ )
            {
                cell_extended_t cell1 = CellsWithTwoSolutions[cell1Idx];
                for( int cell2Idx = cell1Idx + 1; cell2Idx < CellsWithTwoSolutionsNo; cell2Idx++ )
                {
                    cell_extended_t cell2 = CellsWithTwoSolutions[cell2Idx];
                    
                    if( HaveSameSolutions(&cell1, &cell2) )
                    {
                        /* Remove all possibilities except cell1.val and cell2.val from the rest of lines */
                        for( int y = 0; y < this->Size; y++ )
                        {
                            if( (this->CellsMatrix[y][x].Coord == cell1.Coord) || (this->CellsMatrix[y][x].Coord == cell2.Coord) ) /* Ignore cell1 and cell2 */
                            {
                                continue;
                            }
                            char exceptions[2];
                            exceptions[0] = cell1.PossibleSolutions[0].Val;
                            exceptions[1] = cell1.PossibleSolutions[1].Val;
                            this->RemoveAllPosibilitiesExcept(x, y, exceptions, 2);
                        }
                    }
                }
            }
        }/* Columns */
        
        /** Blocks */
        for( int blockId = 0; blockId < this->Size; blockId++ )
        {
            /* Get all cells within block */
            int BlockCellsNo = 0;
            cell_extended_t BlockCells[SUDOKU_MAX_SIZE] = {0};
            BlockCellsNo = this->GetBlockElements(blockId, BlockCells, true);
            
            /* Keep only keep only which have 2 possibilities */
            int CellsWithTwoSolutionsNo = 0;
            cell_extended_t CellsWithTwoSolutions[SUDOKU_MAX_SIZE] = {0};
            for( int blockCellIdx = 0; blockCellIdx < BlockCellsNo; blockCellIdx++ )
            {
                if( BlockCells[blockCellIdx].GetSolutionsNumber() == 2 )
                {
                    CellsWithTwoSolutions[CellsWithTwoSolutionsNo++] = BlockCells[blockCellIdx];
                }
            }
            
            /* Check whether we have solutions overlapping */
            for( int cell1Idx = 0; cell1Idx < CellsWithTwoSolutionsNo; cell1Idx++ )
            {
                cell_extended_t cell1 = CellsWithTwoSolutions[cell1Idx];
                for( int cell2Idx = cell1Idx + 1; cell2Idx < CellsWithTwoSolutionsNo; cell2Idx++ )
                {
                    cell_extended_t cell2 = CellsWithTwoSolutions[cell2Idx];
                    
                    if( HaveSameSolutions(&cell1, &cell2) )
                    {
                        /* Remove all possibilities except cell1.val and cell2.val from the rest of lines */
                        for( int blockCellIdx = 0; blockCellIdx < BlockCellsNo; blockCellIdx++ )
                        {
                            if( (BlockCells[blockCellIdx].Coord == cell1.Coord) || (BlockCells[blockCellIdx].Coord == cell2.Coord) )
                            {
                                continue;
                            }
                            char exceptions[2];
                            exceptions[0] = cell1.PossibleSolutions[0].Val;
                            exceptions[1] = cell1.PossibleSolutions[1].Val;
                            this->RemoveAllPosibilitiesExcept(BlockCells[blockCellIdx].Coord.x, BlockCells[blockCellIdx].Coord.y, exceptions, 2);
                        }
                    }
                }
            }
        }/* Blocks */
    }
    
    void Algo_HiddenPairs()
    {
        /**
         * http://hodoku.sourceforge.net/en/tech_hidden.php
         */
        
        /** Lines */
        for( int y = 0; y < this->Size; y++ )
        {
            /* Keep only keep only candidates which appear twice */
            int PossibilityPairsNo = 0;
            possibility_pair_t PossibilityPairs[SUDOKU_MAX_SIZE] = {0};
            for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
            {
                char CurrentChar = this->Charset[chrIdx];
                cell_extended_t CellsFound[SUDOKU_MAX_SIZE] = {0};
                
                if( this->GetApparitionsOnX(y, CurrentChar, CellsFound) == 2 )
                {
                    possibility_pair_t pair;
                    pair.Cell1 = CellsFound[0];
                    pair.Cell2 = CellsFound[1];
                    pair.CommonSolution = CurrentChar;
                    PossibilityPairs[PossibilityPairsNo++] = pair;
                }
            }
            
            /* Check whether we have solutions overlaping */
            for( int pairIdx = 0; pairIdx < PossibilityPairsNo; pairIdx++ )
            {
                possibility_pair_t pair1 = PossibilityPairs[pairIdx];
                for( int pairIdx2 = pairIdx + 1; pairIdx2 < PossibilityPairsNo; pairIdx2++ )
                {
                    possibility_pair_t pair2 = PossibilityPairs[pairIdx2];
                    if( ((pair1.Cell1.Coord == pair2.Cell1.Coord) && (pair1.Cell2.Coord == pair2.Cell2.Coord)) ||
                        ((pair1.Cell1.Coord == pair2.Cell2.Coord) && (pair1.Cell2.Coord == pair2.Cell1.Coord)) )
                    {
                        char ExceptionsList[2];
                        ExceptionsList[0] = pair1.CommonSolution;
                        ExceptionsList[1] = pair2.CommonSolution;
                        
                        this->RemoveAllPosibilitiesExcept(pair1.Cell1.Coord.x, pair1.Cell1.Coord.y, ExceptionsList, 2);
                        this->RemoveAllPosibilitiesExcept(pair1.Cell2.Coord.x, pair1.Cell2.Coord.y, ExceptionsList, 2);
                    }
                }
            }
        }/* Lines */
        
        /** Columns */
        for( int x = 0; x < this->Size; x++ )
        {
            /* Keep only keep only candidates which appear twice */
            int PossibilityPairsNo = 0;
            possibility_pair_t PossibilityPairs[SUDOKU_MAX_SIZE] = {0};
            for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
            {
                char CurrentChar = this->Charset[chrIdx];
                cell_extended_t CellsFound[SUDOKU_MAX_SIZE] = {0};
                
                if( this->GetApparitionsOnY(x, CurrentChar, CellsFound) == 2 )
                {
                    possibility_pair_t pair;
                    pair.Cell1 = CellsFound[0];
                    pair.Cell2 = CellsFound[1];
                    pair.CommonSolution = CurrentChar;
                    PossibilityPairs[PossibilityPairsNo++] = pair;
                }
            }
            
            /* Check whether we have solutions overlaping */
            for( int pairIdx = 0; pairIdx < PossibilityPairsNo; pairIdx++ )
            {
                possibility_pair_t pair1 = PossibilityPairs[pairIdx];
                for( int pairIdx2 = pairIdx + 1; pairIdx2 < PossibilityPairsNo; pairIdx2++ )
                {
                    possibility_pair_t pair2 = PossibilityPairs[pairIdx2];
                    if( ((pair1.Cell1.Coord == pair2.Cell1.Coord) && (pair1.Cell2.Coord == pair2.Cell2.Coord)) ||
                        ((pair1.Cell1.Coord == pair2.Cell2.Coord) && (pair1.Cell2.Coord == pair2.Cell1.Coord)) )
                    {
                        char ExceptionsList[2];
                        ExceptionsList[0] = pair1.CommonSolution;
                        ExceptionsList[1] = pair2.CommonSolution;
                        
                        this->RemoveAllPosibilitiesExcept(pair1.Cell1.Coord.x, pair1.Cell1.Coord.y, ExceptionsList, 2);
                        this->RemoveAllPosibilitiesExcept(pair1.Cell2.Coord.x, pair1.Cell2.Coord.y, ExceptionsList, 2);
                    }
                }
            }
        }/* Columns */
        
        /** Blocks */
        for( int CurrentBlock = 0; CurrentBlock < this->Size; CurrentBlock++ )
        {
            /* Keep only keep only candidates which appear twice */
            int PossibilityPairsNo = 0;
            possibility_pair_t PossibilityPairs[SUDOKU_MAX_SIZE] = {0};
            for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
            {
                
                char CurrentChar = this->Charset[chrIdx];
                cell_extended_t CellsFound[SUDOKU_MAX_SIZE] = {0};
                
                if( CurrentBlock == 1 && CurrentChar == '5' )
                {
                    int dummy = 0;
                }
                
                if( this->GetApparitionsOnBlock(CurrentBlock, CurrentChar, CellsFound) == 2 )
                {
                    possibility_pair_t pair;
                    pair.Cell1 = CellsFound[0];
                    pair.Cell2 = CellsFound[1];
                    pair.CommonSolution = CurrentChar;
                    PossibilityPairs[PossibilityPairsNo++] = pair;
                }
            }
            
            /* Check whether we have solutions overlapping */
            for( int pairIdx = 0; pairIdx < PossibilityPairsNo; pairIdx++ )
            {
                possibility_pair_t pair1 = PossibilityPairs[pairIdx];
                for( int pairIdx2 = pairIdx + 1; pairIdx2 < PossibilityPairsNo; pairIdx2++ )
                {
                    possibility_pair_t pair2 = PossibilityPairs[pairIdx2];
                    if( ((pair1.Cell1.Coord == pair2.Cell1.Coord) && (pair1.Cell2.Coord == pair2.Cell2.Coord)) ||
                        ((pair1.Cell1.Coord == pair2.Cell2.Coord) && (pair1.Cell2.Coord == pair2.Cell1.Coord)) )
                    {
                        char ExceptionsList[2];
                        ExceptionsList[0] = pair1.CommonSolution;
                        ExceptionsList[1] = pair2.CommonSolution;
                        
                        this->RemoveAllPosibilitiesExcept(pair1.Cell1.Coord.x, pair1.Cell1.Coord.y, ExceptionsList, 2);
                        this->RemoveAllPosibilitiesExcept(pair1.Cell2.Coord.x, pair1.Cell2.Coord.y, ExceptionsList, 2);
                    }
                }
            }
        }/*CurrentBlock*/
    }
    
    void Algo_NakedTriplets()
    {
        /**
         * http://hodoku.sourceforge.net/en/tech_naked.php
         */
        
        /** Blocks */
        
//        this->CellsMatrix[0][0].PossibleSolutions[0].Val = '1';
//        this->CellsMatrix[0][0].PossibleSolutions[1].Val = '2';
//        this->CellsMatrix[0][0].PossibleSolutions[2].Val = '5';
//        this->CellsMatrix[0][0].PossibleSolutions[3].Val = 0;
//
//        this->CellsMatrix[1][1].PossibleSolutions[0].Val = '1';
//        this->CellsMatrix[1][1].PossibleSolutions[1].Val = '2';
//        this->CellsMatrix[1][1].PossibleSolutions[2].Val = '5';
//        this->CellsMatrix[1][1].PossibleSolutions[3].Val = 0;
//
//        this->CellsMatrix[1][2].PossibleSolutions[0].Val = '1';
//        this->CellsMatrix[1][2].PossibleSolutions[1].Val = '2';
//        this->CellsMatrix[1][2].PossibleSolutions[2].Val = '5';
//        this->CellsMatrix[1][2].PossibleSolutions[3].Val = 0;
        
        for( int blockId = 0; blockId < this->Size; blockId++ )
        {
            /* Get all cells within block */
            int BlockCellsNo = 0;
            cell_extended_t BlockCells[SUDOKU_MAX_SIZE] = {0};
            BlockCellsNo = this->GetBlockElements(blockId, BlockCells, true);
            
            /* Keep only keep only which have 3 possibilities */
            int CellsWithThreeSolutionsNo = 0;
            cell_extended_t CellsWithThreeSolutions[SUDOKU_MAX_SIZE] = {0};
            for( int blockCellIdx = 0; blockCellIdx < BlockCellsNo; blockCellIdx++ )
            {
                if( BlockCells[blockCellIdx].GetSolutionsNumber() == 3 )
                {
                    CellsWithThreeSolutions[CellsWithThreeSolutionsNo++] = BlockCells[blockCellIdx];
                }
            }
            
            /* Keep only keep only which have 2 possibilities */
            int CellsWithTwoSolutionsNo = 0;
            cell_extended_t CellsWithTwoSolutions[SUDOKU_MAX_SIZE] = {0};
            for( int blockCellIdx = 0; blockCellIdx < BlockCellsNo; blockCellIdx++ )
            {
                if( BlockCells[blockCellIdx].GetSolutionsNumber() == 2 )
                {
                    CellsWithTwoSolutions[CellsWithTwoSolutionsNo++] = BlockCells[blockCellIdx];
                }
            }
            
            int CellsFilledNo = 0;
            cell_extended_t CellsFilled[3] = {0};
            
            /* Check whether we have solutions overlapping */
            for( int cell1Idx = 0; cell1Idx < CellsWithThreeSolutionsNo; cell1Idx++ )
            {
                CellsFilled[CellsFilledNo++] = CellsWithThreeSolutions[cell1Idx];
                
                /* Check if there are some other 3 solutions cells */
                for( int cell2Idx = cell1Idx + 1; cell2Idx < CellsWithThreeSolutionsNo; cell2Idx++ )
                {
                    if( HaveSameSolutions(&CellsFilled[0], &CellsWithThreeSolutions[cell2Idx]) )
                    {
                        CellsFilled[CellsFilledNo++] = CellsWithThreeSolutions[cell2Idx];
                    }
                    
                    if( CellsFilledNo == 3 )
                        break;
                }
                
                if( CellsFilledNo == 1 )
                {
                    CellsFilledNo = 0;
                }
                else if( CellsFilledNo == 3 )
                {
                    break;
                }
            }
            
            /* If there are less than three cells found, check for cells with two solutions */
            if( CellsFilledNo > 0 && CellsFilledNo < 3)
            {
                for( int cellIdx = 0; cellIdx < CellsWithTwoSolutionsNo; cellIdx++ )
                {
                    cell_extended_t currCell = CellsWithTwoSolutions[cellIdx];
                    char currCellSolutions[2] = {0};
                    currCellSolutions[0] = currCell.PossibleSolutions[0].Val;
                    currCellSolutions[1] = currCell.PossibleSolutions[1].Val;

                    if( CellsFilled[0].ContainSolutions(currCellSolutions, 2) )
                    {
                        CellsFilled[CellsFilledNo++] = currCell;
                    }

                    if( CellsFilledNo == 3 )
                        break;
                }
            }
            
            /* Remove solutions from other cells */
            if( CellsFilledNo == 3 )
            {
                /* Remove all possibilities except cell1.val and cell2.val from the rest of lines */
                for( int blockCellIdx = 0; blockCellIdx < BlockCellsNo; blockCellIdx++ )
                {
                    if( (BlockCells[blockCellIdx].Coord == CellsFilled[0].Coord) || (BlockCells[blockCellIdx].Coord == CellsFilled[1].Coord) || (BlockCells[blockCellIdx].Coord == CellsFilled[2].Coord) )
                    {
                        continue;
                    }
                    
//                    this->Print_Info();
//                    TimeUtils::SleepMs(100);
                    
                    char exceptions[3];
                    exceptions[0] = CellsFilled[0].PossibleSolutions[0].Val;
                    exceptions[1] = CellsFilled[0].PossibleSolutions[1].Val;
                    exceptions[2] = CellsFilled[0].PossibleSolutions[2].Val;
                    this->RemoveAllPosibilitiesExcept(BlockCells[blockCellIdx].Coord.x, BlockCells[blockCellIdx].Coord.y, exceptions, 3);
                }
            }
            
        }/* Blocks */
    }
    
    void Algo_HiddenTriplets()
    {
    
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
                if( SolutionsNumber > 0 )//&& SolutionsNumber <= 3 )
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
            possibility_pair_t pairs[SUDOKU_MAX_SIZE];
            uint8_t pairsFound = this->GetSmallPairs(BlockId, pairs);
            
            printf("Block: %d have %d pairs:\n", BlockId, pairsFound);
            for( int pairIdx = 0; pairIdx < pairsFound; pairIdx++ )
            {
                printf("\t'%c' = [%d][%d] and [%d][%d]\n", pairs[pairIdx].CommonSolution, pairs[pairIdx].Cell1.Coord.y, pairs[pairIdx].Cell1.Coord.x, pairs[pairIdx].Cell2.Coord.y, pairs[pairIdx].Cell2.Coord.x);
            }
        }
        printf("\n");
    }
    
    /** **/
    void IncCellsSolved()
    {
        this->CellsSolved++;
        
        /* Update board possibilities every time a solution is being found */
        this->Algo_UpdatePossibilitiesTable();
        
        /* Update visual board containing last solution */
        PrintBoard();
        
        /* Wait for board to be updated on screen */
//        TimeUtils::SleepMs(100);
    
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
            possibility_t posibility = this->CellsMatrix[y][x].PossibleSolutions[pIdx];
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
    void RemoveAllPosibilitiesExcept(uint8_t x, uint8_t y, char Exception)
    {
        for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
        {
            if( this->Charset[chrIdx] != Exception )
            {
                this->RemovePossibility(x, y, this->Charset[chrIdx]);
            }
        }
    }
    void RemoveAllPosibilitiesExcept(uint8_t x, uint8_t y, char *Exceptions, uint8_t ExceptionsNo)
    {
        for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
        {
            bool IsOnExceptionList = false;
            char currChr = this->Charset[chrIdx];
            for( int exceptionIdx = 0; exceptionIdx < ExceptionsNo; exceptionIdx++ )
            {
                if( currChr == Exceptions[exceptionIdx] )
                {
                    IsOnExceptionList = true;
                }
            }
            if( !IsOnExceptionList )
            {
                this->RemovePossibility(x, y, currChr);
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
    uint8_t GetBlockElements(uint8_t in_GroupId, coord_t *out_Elements, bool ExcludeSolved = false)
    {
        uint8_t ElementsIndex = 0;
        coord_t tmpCoord;
        
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( ExcludeSolved == true && this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL ) /* Exclude already solved if specified */
                {
                    continue;
                }
                
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
    uint8_t GetBlockElements(uint8_t in_GroupId, cell_extended_t *out_Elements, bool ExcludeSolved = false)
    {
        uint8_t ElementsIndex = 0;
        cell_extended_t tmpCell;
        
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( ExcludeSolved == true && this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL ) /* Exclude already solved if specified */
                {
                    continue;
                }
                
                if( this->CellsMatrix[y][x].BlockNo == in_GroupId )
                {
                    tmpCell = this->CellsMatrix[y][x];
                    out_Elements[ElementsIndex++] = tmpCell;
                }
            }
        }
        return ElementsIndex;
    }
    bool HaveSameSolutions(cell_extended_t *Cell1, cell_extended_t *Cell2)
    {
        bool Result = true;
        if( Cell1->GetSolutionsNumber() != Cell2->GetSolutionsNumber() )
        {
            Result = false;
        }
        else
        {
            for( int solutionsCell1 = 0; solutionsCell1 < this->Size; solutionsCell1++ )
            {
                if(Cell1->PossibleSolutions[solutionsCell1].Val == 0)
                {
                    break;
                }
                
                if( !this->IsInPossibleSolutionsList(Cell2->Coord.x, Cell2->Coord.y, Cell1->PossibleSolutions[solutionsCell1].Val) )
                {
                    Result = false;
                    break;
                }
            }
        }
        
        return Result;
    }
    void ClearPossibleSolutions(uint8_t X, uint8_t Y)
    {
        for( uint8_t idx = 0; idx < this->Size; idx++ )
        {
            this->CellsMatrix[Y][X].PossibleSolutions[idx].Val = 0;
        }
    }
    uint8_t GetSmallPairs(uint8_t GroupId, possibility_pair_t *Pairs)
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
                                possibility_pair_t potentialSmallPair;
                                potentialSmallPair.CommonSolution = possibleSolution1;
                                potentialSmallPair.Cell1.Coord.x = x1;
                                potentialSmallPair.Cell1.Coord.y = y1;
                                potentialSmallPair.Cell2.Coord.x = x2;
                                potentialSmallPair.Cell2.Coord.y = y2;
                                
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
                                        if( Pairs[sol].CommonSolution == potentialSmallPair.CommonSolution )
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
    uint8_t GetApparitionsOnBlock(uint8_t GroupId, char Solution, cell_extended_t *ApparitionsArray = nullptr)
    {
        uint8_t Ret = 0;
        
        /* Get all cells within block */
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].BlockNo == GroupId && this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL )
                {
                    if( this->CellsMatrix[y][x].ContainSolution(Solution) )
                    {
                        if( ApparitionsArray != nullptr )
                        {
                            ApparitionsArray[Ret] = this->CellsMatrix[y][x];
                        }
                        Ret++;
                    }
                }
            }
        }
        return Ret;
    }
    uint8_t GetApparitionsOnY(uint8_t X, char Solution, cell_extended_t *ApparitionsArray = nullptr)
    {
        uint8_t Ret = 0;
        
        /* Get all cells within Y */
        for( int y = 0; y < this->Size; y++ )
        {
            if( this->CellsMatrix[y][X].val == UNSOLVED_SYMBOL && this->CellsMatrix[y][X].ContainSolution(Solution) )
            {
                if( ApparitionsArray != nullptr )
                {
                    ApparitionsArray[Ret] = this->CellsMatrix[y][X];
                }
                Ret++;
            }
        }
        return Ret;
    }
    uint8_t GetApparitionsOnX(uint8_t Y, char Solution, cell_extended_t *ApparitionsArray = nullptr)
    {
        uint8_t Ret = 0;
        
        /* Get all cells within X */
        for( int x = 0; x < this->Size; x++ )
        {
            if( this->CellsMatrix[Y][x].val == UNSOLVED_SYMBOL && this->CellsMatrix[Y][x].ContainSolution(Solution) )
            {
                if( ApparitionsArray != nullptr )
                {
                    ApparitionsArray[Ret] = this->CellsMatrix[Y][x];
                }
                Ret++;
            }
        }
        return Ret;
    }
    
};

#endif // _SUDOKUBOARD_H_