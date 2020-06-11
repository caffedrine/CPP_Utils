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
    } possibility_t;
    
    /** Struct to define a basic cell used by user to input it's data */
    typedef struct
    {
        char val;
        uint8_t BlockNo;
        possibility_t PossibleSolutions[SUDOKU_MAX_SIZE];
        coord_t Coord;
        
        uint8_t GetPossibleSolutionsNumber()
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
        bool ContainPossibleSolution(char Solution)
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
        bool ContainPossibleSolutions(char *Solutions, uint8_t SolutionsNumber)
        {
            bool Result = false;
            if( val == UNSOLVED_SYMBOL )
            {
                for( int solIdx = 0; solIdx < SolutionsNumber; solIdx++ )
                {
                    if( !ContainPossibleSolution(Solutions[solIdx]) )
                    {
                        Result = false;
                        break;
                    }
                }
            }
            return Result;
        }
        uint8_t GetPossibleSolutions(char *Solutions)
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
    } cell_extended_t;
    
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
        this->CellsMatrix = (cell_extended_t **) malloc(this->Size * sizeof(cell_extended_t *));
        for( int i = 0; i < this->Size; i++ )
        {
            this->CellsMatrix[i] = (cell_extended_t *) malloc(this->Size * sizeof(cell_extended_t));
        }
        
        /* Copy matrix to internal matrix buffer */
        for( int i = 0; i < this->Size; i++ )
        {
            for( int j = 0; j <= this->Size; j++ )
            {
                this->CellsMatrix[i][j].val = (*((Cells + i * this->Size) + j)).val;
                this->CellsMatrix[i][j].BlockNo = (*((Cells + i * this->Size) + j)).BlockNo;
                this->CellsMatrix[i][j].Coord.y = i;
                this->CellsMatrix[i][j].Coord.x = j;
                
                this->CellsMatrixAsStrings[i][j] = this->CellsMatrix[i][j].val;
            }
        }
    }
    
    ~SudokuBoard()
    {
        /* Make clean */
        for( int i = 0; i < this->Size; i++ )
        {
            delete this->CellsMatrix[i];
        }
        delete[] this->CellsMatrix;
    }
    
    void PrintTimeElapsed()
    {
        static bool FirstPrint = true;
        if( !FirstPrint )
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
        if( !FirstPrint )
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
//        static bool FirstPrint = true;
//        if( ! FirstPrint )
//        {
//            /* Display can be cleared as it is not first time matrix is being printed */
//            for( int i = 0; i < this->Size; i++ )
//            {
//                printf("\33[2K\r"); // Erase current line
//                printf("\33[A\r"); // Move cursor up one line
//            }
//        }
        
        /* Print first line */
        printf("  ");
        for(int i = 0; i < this->Size; i++)
            printf("%x ", i);
        printf("\n");
        
        
        for( int i = 0; i < this->Size; i++ )
        {
            printf("%x ", i);
            for( int j = 0; j < this->Size; j++ )
            {
                printf("%s%c " ANSI_COLOR_RESET, AsciiColors[CellsMatrix[i][j].BlockNo], CellsMatrix[i][j].val);
                //printf(ANSI_COLOR_BLACK "%c" ANSI_COLOR_RESET, CellsMatrix[i][j].val);
            }
            printf("\n");
        }
        fflush(stdout); // Will now print everything in the stdout buffer
//        FirstPrint = false;
    }
    
    void PrintAllPossibilities()
    {
        uint8_t TwoSolutionsCells = 0;
        
        printf("%s\n", this->CellsMatrixAsONEstringColor.c_str());
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
            
            //Algo_NakedSingles();
            //if( this->IsSolved() ) break;
            
            /* Update time elapsed */
            //this->PrintTimeElapsed();
            
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
            PrintAllPossibilities();
        }
        else
        {
            printf("SUCCESS! Puzzle solved!\n");
        }
    } /* Solve() */

protected:

private:
    const uint8_t Size = 0;
    cell_extended_t **CellsMatrix; //CellsMatrix[13][13];
    TimeUtils::Timer TimeCounter;
    /* Increment every time a cell was solved */
    uint16_t CellsSolved = 0;
    /* Flag indicating whether possibility table is fully updated or is updating */
    bool PossibilityTableUpdateRequest = true;
    /* Full table of solutions as a string representation for debugging */
    std::string CellsMatrixAsStrings[13][13] = {""};
    std::string CellsMatrixAsONEstring = "";
    std::string CellsMatrixAsONEstringColor = "";
    
    void Algo_UpdatePossibilitiesTable()
    {
        /* Update all possibilities on all cells for the first time without cross check. Some of them will be removed after crosscheck. */
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                ClearPossibleSolutions(x, y);
                GetAllPossibleSolutionsNoCrossCheck(x, y, this->CellsMatrix[y][x].PossibleSolutions);
            }
        }
        
        /* Update string representation */
        UpdateCellsStringRepresentation();
        
        /* While there are possibilities being removed, keep updating table */
        while( PossibilityTableUpdateRequest )
        {
            /* As long as table update process is not finished this will be set to true by algorithms */
            PossibilityTableUpdateRequest = false;
            
            /** Confirmed */
            Algo_LockedCandidates_Type1_Pointing();
            Algo_NakedPairs();
            Algo_LockedCandidates_Type2_Claiming();
            Algo_HiddenPairs();
            
            /** Unconfirmed */
            Algo_NakedTriplets();
        }
        
        /* To make sure next time will try to update as well */
        PossibilityTableUpdateRequest = true;
    }
    
    void Algo_FullHouse_LastDigit()
    {
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                /* Only loop through unknown symbols */
                if( this->CellsMatrix[y][x].GetPossibleSolutionsNumber() == 1 )
                {
                    /* Propagate solution */
                    this->FoundSolution(__func__,  &this->CellsMatrix[y][x].Coord, this->CellsMatrix[y][x].PossibleSolutions[0].Val);
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
                    /* Propagate solution */
                    this->FoundSolution(std::string(__func__) + " (lines)",  &this->CellsMatrix[y][CellsContainingSymbol[0].Coord.x].Coord, CurrentTryChar);
                }
            }
            
            /** Columns */
            for( int x = 0; x < this->Size; x++ )
            {
                cell_extended_t CellsContainingSymbol[SUDOKU_MAX_SIZE] = {0};
                uint8_t CellsContainingSymbolNo = this->GetApparitionsOnY(x, CurrentTryChar, CellsContainingSymbol);
                if( CellsContainingSymbolNo == 1 ) /* If only one cell in this house contains this symbol, the solution must be this symbol */
                {
                    /* Propagate solution */
                    this->FoundSolution(std::string(__func__) + " (columns)",  &this->CellsMatrix[CellsContainingSymbol[0].Coord.y][x].Coord, CurrentTryChar);
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
                    /* Propagate solution */
                    this->FoundSolution(std::string(__func__) + " (blocks)",  &this->CellsMatrix[GroupCellsContainingSymbol[0].Coord.y][GroupCellsContainingSymbol[0].Coord.x].Coord, CurrentTryChar);
                }
            }
            
        } /*ChrIdx*/
    }
    
    void Algo_NakedSingles()
    {
        /*
         * http://hodoku.sourceforge.net/en/tech_singles.php
         * If there is a cell with only one possibility then that is the solution.
         */
        
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].GetPossibleSolutionsNumber() == 1 )
                {
                    /* Propagate solution */
                    this->FoundSolution(std::string(__func__),  &this->CellsMatrix[y][x].Coord, this->CellsMatrix[y][x].PossibleSolutions[0].Val);
                    
                }
            }/*x*/
        }/*y*/
        
    }
    
    void Algo_LockedCandidates_Type1_Pointing()
    {
        int PairsDetected = 0, PreviousPairsDetected = 0;
        
        while( true )
        {
            PairsDetected = 0;
            
            /* Every char */
            for( int chrIdx = 0; chrIdx < this->Size; chrIdx++ )
            {
                char CurrChar = this->Charset[chrIdx];
                /* Every block */
                for( int blockIdx = 0; blockIdx < this->Size; blockIdx++ )
                {
                    /* Get all elements within this block that contain 'CurrChar' as solution */
                    cell_extended_t CellsFound[SUDOKU_MAX_SIZE] = {0};
                    uint8_t CellsFoundNo = 0;
                    for( int y = 0; y < this->Size; y++ )
                        for( int x = 0; x < this->Size; x++)
                        {
                            if( (this->CellsMatrix[y][x].BlockNo == blockIdx) && (this->CellsMatrix[y][x].ContainPossibleSolution(CurrChar)) )
                                CellsFound[CellsFoundNo++] = this->CellsMatrix[y][x];
                        }
                    
                    /* If block contain multiple cells with given solutions -> go check further whether cells are on the same x or on the same y */
                    if( CellsFoundNo > 1 )
                    {
                        /* Check whether all cell have the same x or the same y */
                        bool HaveSameX = true, HaveSameY = true;
                        for( int cellIdx = 1; cellIdx < CellsFoundNo; cellIdx++ )
                        {
                            if( CellsFound[0].Coord.x != CellsFound[cellIdx].Coord.x )
                                HaveSameX = false;
                            if( CellsFound[0].Coord.y != CellsFound[cellIdx].Coord.y )
                                HaveSameY = false;
                        }
                        
                        if( HaveSameX )
                        {
                            int x = CellsFound[0].Coord.x;
                            for( int y = 0; y < this->Size; y++ )
                            {
                                if( (this->CellsMatrix[y][x].BlockNo != blockIdx) && (this->CellsMatrix[y][x].ContainPossibleSolution(CurrChar)))
                                {
                                    //printf("Removed possibility '%c' from [x:%x][y:%x] as it is on the same X in block %d\n", CurrChar, x, y, blockIdx);
                                    this->RemovePossibility(x, y, CurrChar);
                                }
                            }
                            PairsDetected++;
                        }
                        else if( HaveSameY)
                        {
                            int y = CellsFound[0].Coord.y;
                            for( int x = 0; x < this->Size; x++ )
                            {
                                if( (this->CellsMatrix[y][x].BlockNo != blockIdx) && (this->CellsMatrix[y][x].ContainPossibleSolution(CurrChar)))
                                {
                                    //printf("Removed possibility '%c' from [x:%x][y:%x] as it is on the same Y in block %d\n", CurrChar, x, y, blockIdx);
                                    this->RemovePossibility(x, y, CurrChar);
                                }
                            }
                            PairsDetected++;
                        }
                    }
                }/*BlockId*/
            }/*chrIdx*/
            
            /* Stay in the loop until no more pairs are found */
            if( PreviousPairsDetected == PairsDetected )
                break;
            else
                PreviousPairsDetected = PairsDetected;
        }
    }
    
    void Algo_LockedCandidates_Type2_Claiming()
    {
        /** http://hodoku.sourceforge.net/en/tech_intersections.php */
        
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
    
                                // Only remove possibility if already contained
                                if( (currentCell.Coord.y != y) && (currentCell.ContainPossibleSolution(CurrentChar)) )
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
                                
                                // Only remove posibility if it is contained by the cell
                                if( (currentCell.Coord.x != x) && (currentCell.ContainPossibleSolution(CurrentChar)) )
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
                if( this->CellsMatrix[y][x].GetPossibleSolutionsNumber() == 2 )
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
                        /* Remove cell1.val and cell2.val from the rest of cells */
                        for( int x = 0; x < this->Size; x++ )
                        {
                            if( (this->CellsMatrix[y][x].Coord == cell1.Coord) || (this->CellsMatrix[y][x].Coord == cell2.Coord) ) /* Ignore cell1 and cell2 */
                            {
                                continue;
                            }
                            this->RemovePossibility(x, y, cell1.PossibleSolutions[0].Val);
                            this->RemovePossibility(x, y, cell1.PossibleSolutions[1].Val);
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
                if( this->CellsMatrix[y][x].GetPossibleSolutionsNumber() == 2 )
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
                        /* Remove cell1.val and cell2.val from the rest of cells */
                        for( int y = 0; y < this->Size; y++ )
                        {
                            if( (this->CellsMatrix[y][x].Coord == cell1.Coord) || (this->CellsMatrix[y][x].Coord == cell2.Coord) ) /* Ignore cell1 and cell2 */
                            {
                                continue;
                            }
                            this->RemovePossibility(x, y, cell1.PossibleSolutions[0].Val);
                            this->RemovePossibility(x, y, cell1.PossibleSolutions[1].Val);
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
                if( BlockCells[blockCellIdx].GetPossibleSolutionsNumber() == 2 )
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
                        /* Remove cell1.val and cell2.val from the rest of cells */
                        for( int blockCellIdx = 0; blockCellIdx < BlockCellsNo; blockCellIdx++ )
                        {
                            if( (BlockCells[blockCellIdx].Coord == cell1.Coord) || (BlockCells[blockCellIdx].Coord == cell2.Coord) )
                            {
                                continue;
                            }
                            this->RemovePossibility(BlockCells[blockCellIdx].Coord.x, BlockCells[blockCellIdx].Coord.y, cell1.PossibleSolutions[0].Val);
                            this->RemovePossibility(BlockCells[blockCellIdx].Coord.x, BlockCells[blockCellIdx].Coord.y, cell1.PossibleSolutions[1].Val);
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
        
        int PairsFound = 0, PreviousPairsFound = 0;
        while(true)
        {
            PairsFound = 0;
            
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
                            PairsFound++;
                        }
                    }
                }
            }/* Lines */
    
            /** Columns */
            for( int x = 0; x < this->Size; x++ )
            {
                /* Keep only candidates which appear twice */
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
                            PairsFound++;
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
                            PairsFound++;
                        }
                    }
                }
            }/*CurrentBlock*/
            
            if( PairsFound == PreviousPairsFound )
                break;
            else
                PreviousPairsFound = PairsFound;
            
        }/* while */
    }
    
    void Algo_NakedTriplets()
    {
        /**
         * http://hodoku.sourceforge.net/en/tech_naked.php
         */
        
        /** Blocks */
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
                if( BlockCells[blockCellIdx].GetPossibleSolutionsNumber() == 3 )
                {
                    CellsWithThreeSolutions[CellsWithThreeSolutionsNo++] = BlockCells[blockCellIdx];
                }
            }
            
            /* Keep only keep only which have 2 possibilities */
            int CellsWithTwoSolutionsNo = 0;
            cell_extended_t CellsWithTwoSolutions[SUDOKU_MAX_SIZE] = {0};
            for( int blockCellIdx = 0; blockCellIdx < BlockCellsNo; blockCellIdx++ )
            {
                if( BlockCells[blockCellIdx].GetPossibleSolutionsNumber() == 2 )
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
            if( CellsFilledNo > 0 && CellsFilledNo < 3 )
            {
                for( int cellIdx = 0; cellIdx < CellsWithTwoSolutionsNo; cellIdx++ )
                {
                    cell_extended_t currCell = CellsWithTwoSolutions[cellIdx];
                    char currCellSolutions[2] = {0};
                    currCellSolutions[0] = currCell.PossibleSolutions[0].Val;
                    currCellSolutions[1] = currCell.PossibleSolutions[1].Val;
                    
                    if( CellsFilled[0].ContainPossibleSolutions(currCellSolutions, 2) )
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

//                    this->PrintAllPossibilities();
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
    
    void UpdateCellsStringRepresentation()
    {
        /* Get the the number of most solutions to know how many should be between columns */
        
        int MostSolutions = 0;
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( this->CellsMatrix[y][x].GetPossibleSolutionsNumber() > MostSolutions )
                {
                    MostSolutions = this->CellsMatrix[y][x].GetPossibleSolutionsNumber();
                }
            }
        }
        if( MostSolutions < 3 )
        {
            MostSolutions = 3;
        }
        
        this->CellsMatrixAsONEstring = "     ";
        /* Append header indicating X on matrix string */
        for( int i = 0; i  < this->Size; i++ )
        {
            CellsMatrixAsONEstring += std::to_string( i );
            for( int j = 0; j < MostSolutions - (i > 9?1:0); j++ )
            {
                CellsMatrixAsONEstring += " ";
            }
        }
        CellsMatrixAsONEstring += "\n\n";
        CellsMatrixAsONEstringColor = CellsMatrixAsONEstring;
        
        
        for( int y = 0; y < this->Size; y++ )
        {
            CellsMatrixAsONEstring += (std::to_string(y) + (y>9?"   ":"    ") );
            this->CellsMatrixAsONEstringColor += (std::to_string(y) + (y>9?"   ":"    ") );
    
    
            for( int x = 0; x < this->Size; x++ )
            {
                this->CellsMatrixAsStrings[y][x] = "";
                
                /* fill solutions */
                if( this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL )
                {
                    this->CellsMatrixAsStrings[y][x] += this->CellsMatrix[y][x].val;
    
                    /* Fill with empty spaces */
                    for( int k = 0; k < (MostSolutions - 1); k++ )
                    {
                        this->CellsMatrixAsStrings[y][x] += " ";
                    }
                }
                else
                {
                    for( int solIdx = 0; solIdx < this->CellsMatrix[y][x].GetPossibleSolutionsNumber(); solIdx++ )
                    {
                        char currSol = this->CellsMatrix[y][x].PossibleSolutions[solIdx].Val;
                        this->CellsMatrixAsStrings[y][x] += currSol;
                    }
    
                    /* Fill with empty spaces */
                    for( int k = 0; k < (MostSolutions - this->CellsMatrix[y][x].GetPossibleSolutionsNumber()); k++ )
                    {
                        this->CellsMatrixAsStrings[y][x] += " ";
                    }
                }
                
                this->CellsMatrixAsONEstring += (this->CellsMatrixAsStrings[y][x] + " ");
                this->CellsMatrixAsONEstringColor += (AsciiColors[CellsMatrix[y][x].BlockNo]+ (this->CellsMatrixAsStrings[y][x] + " ") + std::string(ANSI_COLOR_RESET) );
    
            }
            this->CellsMatrixAsONEstring += "\n";
            this->CellsMatrixAsONEstringColor += "\n";
        }
    }
    
    /** **/
    void FoundSolution(const std::string& AlgoApplied, coord_t *Coords, char solution)
    {
        printf("\nFound a solution using '%s': [%x][%x] = %c:\n", AlgoApplied.c_str(), Coords->x, Coords->y, solution );
        printf("BEFORE:\n");
        this->PrintAllPossibilities();
        
        /* Update matrix with given solution */
        this->CellsMatrix[Coords->y][Coords->x].val = solution;
    
        /* Count how many cells were solved */
        this->CellsSolved++;
        
        /* Update board possibilities every time a solution is being found */
        this->Algo_UpdatePossibilitiesTable();
        
        /* Update visual board containing last solution */
        //this->PrintBoard();
        printf("AFTER:\n");
        this->PrintAllPossibilities();
    
        // Wait for user to continue
        printf("Press a key to continue solving...");
        fflush(stdin);
        fflush(stdout);
        getchar();
        fflush(stdin);
        fflush(stdout);
    }
    
    void IncCellsSolved()
    {

    
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
        if( (x==1) && (y==4) && (possibilityToRemove == '8') )
        {
            int dummy = 0;
        }
        
        for( int pIdx = 0; pIdx < this->Size; pIdx++ )
        {
            possibility_t posibility = this->CellsMatrix[y][x].PossibleSolutions[pIdx];
            if( posibility.Val == 0 )
                break;
            
            if( posibility.Val == possibilityToRemove )
            {
                /* Indicate that the rest of algorithms needs to check with this update */
                this->PossibilityTableUpdateRequest = true;
                
                /* Invalidate the possibility */
                posibility.Val = 0;
                
                /* Shift the other if end of array not reached */
                for( int i = pIdx; i < this->Size - 1; i++ )
                {
                    this->CellsMatrix[y][x].PossibleSolutions[i] = this->CellsMatrix[y][x].PossibleSolutions[i + 1];
                }
                this->CellsMatrix[y][x].PossibleSolutions[this->Size - 1].Val = 0;
                
                /* Update string representation of cells */
                UpdateCellsStringRepresentation();
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
    void RemoveAllPosibilitiesExcept(uint8_t x, uint8_t y, const char *Exceptions, uint8_t ExceptionsNo)
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
            if( (!IsOnExceptionList) && (this->CellsMatrix[y][x].ContainPossibleSolution(currChr)))
            {
                this->RemovePossibility(x, y, currChr);
            }
        }
    }
    bool IsInPossibleSolutionNoCrossCheck(uint8_t x, uint8_t y, char SolutionToCheck)
    {
        bool Result = false;
        /* Check whether it's safe to place this symbol on this box */
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
                /* Check whether it's safe to place this symbol on this box */
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
                if( (ExcludeSolved) && (this->CellsMatrix[y][x].val != UNSOLVED_SYMBOL) ) /* Exclude already solved if specified */
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
        if( Cell1->GetPossibleSolutionsNumber() != Cell2->GetPossibleSolutionsNumber() )
        {
            Result = false;
        }
        else
        {
            for( int solutionsCell1 = 0; solutionsCell1 < this->Size; solutionsCell1++ )
            {
                if( Cell1->PossibleSolutions[solutionsCell1].Val == 0 )
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
    uint8_t GetApparitionsOnBlock(uint8_t GroupId, char Solution, cell_extended_t *ApparitionsArray = nullptr)
    {
        uint8_t Ret = 0;
        
        /* Get all cells within block */
        for( int y = 0; y < this->Size; y++ )
        {
            for( int x = 0; x < this->Size; x++ )
            {
                if( (this->CellsMatrix[y][x].BlockNo == GroupId) && (this->CellsMatrix[y][x].val == UNSOLVED_SYMBOL) )
                {
                    if( this->CellsMatrix[y][x].ContainPossibleSolution(Solution) )
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
            if( (this->CellsMatrix[y][X].val == UNSOLVED_SYMBOL) && (this->CellsMatrix[y][X].ContainPossibleSolution(Solution)) )
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
            if( (this->CellsMatrix[Y][x].val == UNSOLVED_SYMBOL) && (this->CellsMatrix[Y][x].ContainPossibleSolution(Solution)) )
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