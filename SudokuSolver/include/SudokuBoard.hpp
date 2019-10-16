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

class Cell
{
public:
    Cell(char Val, uint8_t Block) : Value(static_cast<uint8_t>(Val)), BlockNo(Block), x(255), y(255) {}
    Cell(char Val, uint8_t X, uint8_t Y, uint8_t Block) : Value(static_cast<uint8_t>(Val)), x(X), y(Y), BlockNo(Block) {}
    
    bool IsSolved()
    {
        return this->Value != UNSOLVED_SYMBOL;
    }
    
    uint8_t GetBlockNo()
    {
        return BlockNo;
    }
    
    void SetValue(uint8_t NewVal)
    {
        this->Value = NewVal;
    }


private:
    uint8_t Value;
    const uint8_t x;
    const uint8_t y;
    const uint8_t BlockNo;
};

class SudokuBoard
{
    const char *Charset = "123456789abcdef\0";
    const char *AsciiColors[SUDOKU_MAX_SIZE] =
            {
                    ANSI_COLOR_RED_BOLD,
                    ANSI_COLOR_GREEN_BOLD,
                    ANSI_COLOR_YELLOW_BOLD,
                    ANSI_COLOR_BLUE_BOLD,
                    ANSI_COLOR_MAGENTA_BOLD,
                    ANSI_COLOR_CYAN_BOLD,
                    ANSI_COLOR_WHITE_BOLD,
                    ANSI_COLOR_RED_BOLD,
                    ANSI_COLOR_GREEN_BOLD,
                    ANSI_COLOR_YELLOW_BOLD,
                    ANSI_COLOR_BLUE_BOLD,
                    ANSI_COLOR_MAGENTA_BOLD,
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

    void ClearDisplay()
    {
        for( int i = 0; i < this->Size; i++ )
        {
            printf("\33[2K\r"); // Erase current line
            printf("\33[A\r"); // Move cursor up one line
        }
        fflush(stdout); // Will now print everything in the stdout buffer
    }
    void PrintBoard()
    {
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
    }
    
    void Solve()
    {
        while(!this->IsSolved())
        for(int y = 0; y < this->Size; y++)
        {
            for(int x = 0; x < this->Size; x++)
            {
                /* Only loop through unknown symbols */
                if(this->CellsMatrix[x][y].val == UNSOLVED_SYMBOL)
                {
                    for(int TryCharIdx = 0; TryCharIdx < this->Size; TryCharIdx++)
                    {
                        /* Variable to store current char to be stored */
                        char CurrentTryChar = Charset[TryCharIdx];
                        /* Check whether it's safe to place this symbol on this vox */
                        if( !this->AlreadyOnX(y, CurrentTryChar) || !this->AlreadyOnY(x, CurrentTryChar) || !this->AlreadyOnBlock(this->CellsMatrix[x][y].BlockNo, CurrentTryChar) )
                        {
                            // Check whether is safe to fill this car on ths line
                            this->CellsMatrix[x][y].val = CurrentTryChar;
                        
                            /* Update table */
                            ClearDisplay();
                            PrintBoard();
                            TimeUtils::SleepMs(500);
                        
                            /* Go to next cel as this one was already solved */
                            break;
                        }
                    }
                }
            }
        }
    }
    
protected:

private:
    const uint8_t Size = 0;
    cell_base_t **CellsMatrix;

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
    char GetNext(char CurrentChar)
    {
        int Idx = -1;
        for( int i = 0; i < this->Size; i++ )
        {
            if( Charset[i] == CurrentChar )
            {
                Idx = i;
                if( Idx + 1 >= this->Size)
                {
                    /* Return first element indicating that there was a complete shift */
                    return '1';
                }
                return Charset[Idx+1];
            }
        }
        
    }
};

#endif // _SUDOKUBOARD_H_