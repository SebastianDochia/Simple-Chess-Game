/*MIT License

Copyright(c) 2020 SebastianDochia

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <iostream>
#include <iomanip>
#include <malloc.h>


void printMatrix(int board[][9]);
void move(int board[][9]);
int selectPiece(int board[][9]);
void fillBoard(int board[][9]);
void main()
{
	int gameBoard[9][9];
	int selectedPosition;
	bool gameOver = false;
	int movesCounter = 0;

	fillBoard(gameBoard);

	printMatrix(gameBoard);

	while (!gameOver)
	{
		move(gameBoard);
		movesCounter++;
	}
}

void printMatrix(int board[][9])
{
	for (register int i = 0; i < 9; i++)
	{
		std::cout << std::endl;
		for (register int j = 0; j < 9; j++)
		{
			std::cout << std::setw(3) << board[i][j];
		}
	}
}

//creating the initial matrix
void fillBoard(int board[][9])
{
	//place markings
	board[0][0] = 9;
	for (register int i = 1, v = 0; i < 9; i++)
	{
		board[0][i] = ++v;
	}
	for (register int i = 1, v = 0; i < 9; i++)
	{
		board[i][0] = ++v;
	}

	//place pawns
	for (register int i = 1; i < 9; i++)
	{
		board[2][i] = 11;
	}
	for (int i = 1; i < 9; i++)
	{
		board[7][i] = 10;
	}

	//place empty
	for (register int i = 3; i < 7; i++)
	{
		for (register int j = 1; j < 9; j++)
		{
			board[i][j] = 0;
		}
	}

	//place big pieces
	board[1][1] = board[1][8] = 21;
	board[1][2] = board[1][7] = 31;
	board[1][3] = board[1][6] = 41;
	board[1][4] = 81;
	board[1][5] = 91;

	board[8][1] = board[8][8] = 20;
	board[8][2] = board[8][7] = 30;
	board[8][3] = board[8][6] = 40;
	board[8][4] = 80;
	board[8][5] = 90;
}

//##############################################//
//############## MOVING A PIECE ################//
//##############################################//

int selectPosition(int*, int);
int selectPiece(int[][9]);
void fail(int[][9], int*);
void getMoves(int, int, int, int[][9], int*, int&);
void move(int board[][9])
{
	int selectedPosition = 0;
	int selected = selectPiece(board);
	const int row = selected / 10;
	const int col = selected % 10;
	int piece = board[row][col];
	int movesCount = 0;

	int* moves = (int*)malloc(sizeof(int) * 32);
	//int* moves = new int;
	if (moves == NULL)
	{
		throw("Memory allocation failed");
		std::cout << std::endl << std::endl << std::endl << "Memory allocation failed";
	}


	getMoves(row, col, piece, board, moves, movesCount);
	std::cout << std::endl << "The piece can be moved here: ";
	for (register int i = 0; i < movesCount; i++)
	{
		std::cout << std::endl << moves[i] << " ";
	}

	selectedPosition = selectPosition(moves, movesCount);

	const int newRow = selectedPosition / 10;
	const int newCol = selectedPosition % 10;

	board[row][col] = 0;
	board[newRow][newCol] = piece;

	printMatrix(board);
	//delete moves;

	free(moves);
}

int selectPiece(int board[][9])
{
	int row, col, piece;

	std::cout << std::endl << "You play as white! Please select the row and the column of the piece you wish to move!" << std::endl;

	//enter row number
	while (true)
	{
		try
		{
			std::cout << "Ener the number of the selected row..." << std::endl;

			std::cin >> row;

			if (0 < row > 9)
			{
				throw 1001;
			}
			std::cout << std::endl;
			break;
		}
		catch (int e)
		{
			std::cout << std::endl << "That is number invalid, please select a number from 1 to 8" << std::endl;
		}
	}

	//enter column number
	while (true)
	{
		try
		{
			std::cout << "Ener the number of the selected column..." << std::endl;

			std::cin >> col;

			if (0 < col > 9)
			{
				throw 1001;
			}
			std::cout << std::endl;
			break;
		}
		catch (int e)
		{
			std::cout << std::endl << "That is number invalid, please select a number from 1 to 8" << std::endl;
		}
	}

	piece = board[row][col];

	if (piece != 0 && piece > 9)
	{
		return row * 10 + col;
	}
	else
	{
		std::cout << std::endl << "Invalide selection" << std::endl;
		move(board);
	}
}

void king(int, int, int*, int[][9], int&);
void queen(int, int, int*, int[][9], int&);
void bishop(int, int, int*, int[][9], int&);
void knight(int, int, int*, int[][9], int&);
void rook(int, int, int*, int[][9], int&);
void pawn(int, int, int*, int[][9], int&);
void getMoves(int x, int y, int type, int board[][9], int* moves, int& index)
{
	switch (type)
	{
	case 10:
		pawn(x, y, moves, board, index);
		break;
	case 20:
		rook(x, y, moves, board, index);
		break;
	case 30:
		knight(x, y, moves, board, index);
		break;
	case 40:
		bishop(x, y, moves, board, index);
		break;
	case 80:
		queen(x, y, moves, board, index);
		break;
	case 90:
		king(x, y, moves, board, index);
		break;
	default:
		fail(board, moves);
	}
}

//################ MOVEMENT PATERN ################//

void pawn(int x, int y, int* moves, int board[][9], int& index)
{
	if ((board[x - 2][y] == 0 or (board[x - 2][y] % 10 > 0)) && x == 7)
	{
		moves[index] = (x - 1) * 10 + y;
		index++;
		moves[index] = (x - 2) * 10 + y;
		index++;
	}
	else
	{
		if (board[x - 1][y] == 0 or board[x - 1][y] % 10 > 0)
		{
			moves[index] = (x - 1) * 10 + y;
			index++;
		}
	}

}

void rook(int x, int y, int* moves, int board[][9], int& index)
{
	for (register int i = x + 1; i < 8 - x; i++)
	{
		if (board[i][y] == 0 or (board[i][y] % 10 > 0 and board[i][y] >= 10))
		{
			moves[index] = i * 10 + y;
			index++;
		}
		else
			break;
	}
	for (register int i = x - 1; i < x; i--)
	{
		if (board[i][y] == 0 or (board[i][y] % 10 > 0 and board[i][y] >= 10))
		{
			moves[index] = i * 10 + y;
			index++;
		}
		else
			break;
	}
	for (register int i = y + 1; i < 8 - y; i++)
	{
		if (board[x][i] == 0 or (board[x][i] % 10 > 0 and board[x][i] >= 10))
		{
			moves[index] = x * 10 + i;
			index++;
		}
		else
			break;
	}
	for (register int i = y - 1; i < y; i--)
	{
		if (board[x][i] == 0 or (board[x][i] % 10 > 0 and board[x][i] >= 10))
		{
			moves[index] = x * 10 + i;
			index++;
		}
		else
			break;
	}

}

void knight(int x, int y, int* moves, int board[][9], int& index)
{
	if ((board[x + 2][y + 1] % 10 > 0 or board[x + 2][y + 1] == 0) && x < 7 && y > 1)
	{
		moves[index] = (x + 2) * 10 + (y + 1);
		index++;
	}
	if ((board[x + 2][y - 1] % 10 > 0 or board[x + 2][y - 1] == 0) && x < 7 && y < 8)
	{
		moves[index] = (x + 2) * 10 + (y - 1);
		index++;
	}
	if ((board[x - 2][y + 1] % 10 > 0 or board[x - 2][y + 1] == 0) && x > 2 && y > 1)
	{
		moves[index] = (x - 2) * 10 + (y + 1);
		index++;
	}
	if ((board[x - 2][y - 1] % 10 > 0 or board[x - 2][y - 1] == 0) && x > 2 && y < 8)
	{
		moves[index] = (x - 2) * 10 + (y - 1);
		index++;
	}
	if ((board[x + 1][y + 2] % 10 > 0 or board[x + 1][y + 2] == 0) && x < 8 && y < 7)
	{
		moves[index] = (x + 1) * 10 + (y + 2);
		index++;
	}
	if ((board[x + 1][y - 2] % 10 > 0 or board[x + 1][y - 2] == 0) && x < 8 && y > 2)
	{
		moves[index] = (x + 1) * 10 + (y - 2);
		index++;
	}
	if ((board[x - 1][y + 2] % 10 > 0 or board[x - 1][y + 2] == 0) && x > 1 && y < 7)
	{
		moves[index] = (x - 1) * 10 + (y + 2);
		index++;
	}
	if ((board[x - 1][y - 2] % 10 > 0 or board[x - 1][y - 2] == 0) && x > 1 && y > 2)
	{
		moves[index] = (x - 1) * 10 + (y - 2);
		index++;
	}
}

void bishop(int x, int y, int* moves, int board[][9], int& index)
{
	for (register int i = 1; i < x && i < 8 - y; i++)
	{
		if (board[x - i][y + i] == 0 or board[x - i][y + i] % 10 > 0)
		{
			moves[index] = (x - i) * 10 + y + i;
			index++;
		}
		else
			break;


	}
	for (register int i = 1; i < 8 - x && i < 8 - y; i++)
	{
		if (board[x + i][y + i] == 0 or board[x + i][y + i] % 10 > 0)
		{
			moves[index] = (x + i) * 10 + y + i;
			index++;
		}
		else
			break;
	}
	for (register int i = 1; i < 8 - x && i < y; i++)
	{
		if (board[x + i][y - i] == 0 or board[x + i][y - i] % 10 > 0)
		{
			moves[index] = (x + i) * 10 + y - i;
			index++;
		}
		else
			break;
	}
	for (register int i = 1; i < x && i < y; i++)
	{
		if (board[x - i][y - i] == 0 or board[x - i][y - i] % 10 > 0)
		{
			moves[index] = (x - i) * 10 + y - i;
			index++;
		}
		else
			break;
	}
}

void queen(int x, int y, int* moves, int board[][9], int& index)
{
	rook(x, y, moves, board, index);
	bishop(x, y, moves, board, index);
}

void king(int x, int y, int* moves, int board[][9], int& index)
{
	if (board[x - 1][y] == 0 && board[x - 1][y] % 10 > 1)
	{
		if (x - 1 > 1)
		{
			moves[index] = board[x - 1][y];
			index++;
		}
	}
	if (board[x + 1][y] == 0 && board[x + 1][y] % 10 > 1)
	{
		if (x + 1 > 1)
		{
			moves[index] = board[x + 1][y];
			index++;
		}
	}
	if (board[x][y - 1] == 0 && board[x][y - 1] % 10 > 1)
	{
		if (y - 1 > 1)
		{
			moves[index] = board[x][y - 1];
			index++;
		}
	}
	if (board[x][y + 1] == 0 && board[x][y + 1] % 10 > 1)
	{
		if (y + 1 > 1)
		{
			moves[index] = board[x][y + 1];
			index++;
		}
	}
}

//################################################//

void fail(int board[][9], int* moves)
{
	std::cout << std::endl << "This piece cannot be moved!" << std::endl;
	//free(moves);
	delete[] moves;
	move(board);
}

bool validation(int*, int, int);
int selectPosition(int* moves, int index)
{
	int choice;
	std::cout << std::endl << "Select a position:" << std::endl;
	std::cin >> choice;
	if (validation(moves, index, choice))
	{
		return choice;
	}
	else
	{
		std::cout << std::endl << "Position invalid" << std::endl;
		selectPosition(moves, index);
	}
}

bool validation(int* moves, int index, int choice)
{
	for (register int i = 0; i < index; i++)
	{
		if (moves[i] == choice)
		{
			return true;
		}
	}
	return false;
}