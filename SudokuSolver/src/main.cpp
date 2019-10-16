#include <stdio.h>
#include <AnsiColors.h>

const uint8_t UNSOLVED_SYMBOL = '*';

const char *Colors[15] =
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


class Cell
{
public:
    Cell(char Val, uint8_t Block) : Value(Val), BlockNo(Block), x(255), y(255) {}
    Cell(char Val, uint8_t X, uint8_t Y, uint8_t Block) : Value(Val), x(X), y(Y), BlockNo(Block) {}
    
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


class Block
{
public:
private:
    uint8_t *Elements;
};


class SudokuTable
{
    const uint8_t MAX_SIZE = 15;

};

typedef struct
{
    char val;
    const uint8_t BlockNo;
}cell_short_t;

cell_short_t CellsMatrix[13][13]  =
        {
            /*Line 0*/  { {'*', 0}, {'a', 0}, {'*', 1}, {'*', 1}, {'d', 1}, {'3', 1}, {'*', 2}, {'*', 2}, {'7', 2}, {'*', 2}, {'*', 3}, {'9', 3}, {'*', 3} },
            /*Line 1*/  { {'4', 0}, {'*', 0}, {'*', 0}, {'*', 1}, {'*', 1}, {'*', 1}, {'*', 2}, {'c', 2}, {'*', 2}, {'7', 3}, {'b', 3}, {'*', 3}, {'*', 3} },
            /*Line 2*/  { {'6', 0}, {'*', 0}, {'*', 0}, {'d', 0}, {'*', 1}, {'*', 1}, {'c', 1}, {'*', 2}, {'3', 3}, {'*', 3}, {'8', 3}, {'*', 3}, {'*', 3} },
            /*Line 3*/  { {'*', 0}, {'b', 0}, {'c', 0}, {'2', 1}, {'*', 1}, {'*', 1}, {'4', 2}, {'*', 2}, {'*', 2}, {'*', 3}, {'*', 7}, {'*', 7}, {'*', 7} },
            /*Line 4*/  { {'*', 4}, {'*', 0}, {'4', 4}, {'*', 4}, {'*', 5}, {'*', 5}, {'*', 2}, {'*', 6}, {'*', 2}, {'*', 7}, {'*', 7}, {'*', 7}, {'*', 7} },
            /*Line 5*/  { {'*', 4}, {'7', 4}, {'*', 4}, {'5', 5}, {'8', 5}, {'*', 5}, {'3', 6}, {'*', 6}, {'*', 6}, {'*', 7}, {'*', 7}, {'*', 7}, {'*', 7} },
            /*Line 6*/  { {'*', 4}, {'5', 4}, {'b', 4}, {'*', 5}, {'*', 5}, {'*', 5}, {'2', 6}, {'*', 6}, {'*', 6}, {'*', 7}, {'*', 12}, {'*', 12}, {'*', 7} },
            /*Line 7*/  { {'*', 4}, {'3', 4}, {'*', 4}, {'*', 5}, {'*', 5}, {'*', 6}, {'*', 6}, {'5', 6}, {'*', 6}, {'2', 12}, {'*', 12}, {'*', 12}, {'8', 12} },
            /*Line 8*/  { {'*', 4}, {'*', 8}, {'*', 8}, {'*', 9}, {'a', 5}, {'*', 5}, {'*', 6}, {'*', 6}, {'4', 11}, {'5', 11}, {'*', 11}, {'*', 12}, {'*', 12} },
            /*Line 9*/  { {'*', 8}, {'c', 8}, {'*', 8}, {'*', 9}, {'*', 9}, {'*', 5}, {'*', 10}, {'*', 10}, {'*', 11}, {'d', 11}, {'*', 11}, {'3', 12}, {'*', 12} },
            /*Line 10*/ { {'b', 8}, {'*', 8}, {'*', 9}, {'*', 9}, {'*', 9}, {'9', 9}, {'*', 10}, {'*', 10}, {'*', 10}, {'*', 10}, {'*', 11}, {'*', 11}, {'*', 12} },
            /*Line 11*/ { {'*', 8}, {'4', 8}, {'*', 8}, {'*', 9}, {'*', 9}, {'2', 9}, {'d', 10}, {'*', 10}, {'8', 10}, {'*', 11}, {'*', 11}, {'*', 12}, {'*', 12} },
            /*Line 12*/ { {'*', 8}, {'*', 8}, {'*', 8}, {'*', 9}, {'5', 9}, {'6', 9}, {'a', 10}, {'*', 10}, {'*', 10}, {'*', 10}, {'*', 11}, {'*', 11}, {'*', 11} },
            
            };


int main()
{
    printf("Inputs:\n");
    
    for(int i = 0; i < 13; i++)
    {
        for(int j = 0; j < 13; j++)
        {
            printf( "%s%c " ANSI_COLOR_RESET, Colors[CellsMatrix[i][j].BlockNo], CellsMatrix[i][j].val);
        }
        printf("\n");
    }
    
}