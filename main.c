/******************************************************************************

							Online C Compiler. 
				Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void initializeTheBoard(char **, int, int);
void printScreen(char **, int, int);
void play(char **, int *);
int canMove(char **, int, int, int, int, int);
int pawnMove(char **, int, int, int, int, int);
int destinationCheck(char, int);
int isGameOver(char **, int, int);
int isAvailable(char **, int, int, int);


#define HEIGTH 8
#define WIDTH 8

int main()
{
	char **arr;
	int  i = 0, turn = 0, gameOver = 0;

	//hafýzadan alan al
	arr = malloc(sizeof(char *) * HEIGTH);
	for (; i < HEIGTH; i++)
	{
		arr[i] = malloc(sizeof(char) * WIDTH);
	}
	//tahtaya ilk halini ver
	initializeTheBoard(arr, HEIGTH, WIDTH);


	do
	{
		printScreen(arr, HEIGTH, WIDTH);
		play(arr, &turn);
	} while ((gameOver = isGameOver(arr, HEIGTH, WIDTH)) == 0);


	printf("%d player wins!", gameOver);

	//hafizayi geri býrak
	for (i = 0; i < 5; i++)
	{
		free(arr[i]);
	}
	free(arr);

	return 0;
}

//kullanýcýlarýn ikisinin de sah tasý mevcut mu
//eger oyleyse return 0
//eger 0. oyuncunun tasi yoksa 1 dondur
//eger 1. oyuncunun tasi yoksa 2 dondur
int isGameOver(char **arr, int nRows, int nCols)
{
	int i, j, result, result2, blk = 0, wht = 0;
	char piece;

	for (i = 0; i < nRows; i++)
	{
		for (j = 0; j < nRows; j++)
		{
			piece = arr[i][j];
			if (piece == 'S')
			{
				wht = 1;
			}
			else if (piece == 's')
			{
				blk = 1;
			}
		}
	}
	if (blk == 1 && wht == 1)
	{
		return 0;
	}
	else if (wht == 1 && blk == 0)
	{
		return 1;
	}
	else if (wht == 0 && blk == 1)
	{
		return 2;
	}
}

//hareket edilecek hedef noktasindaki tas kimin
//eger siranin kendisindeki oyuncunun ise 0
//degilse 1 dondur
int destinationCheck(char destPiece, int turn)
{
	//turn 0 ise
	if ((destPiece >= 'A' && destPiece <= 'Z') && turn == 0)
	{
		return 0;
	}
	//turn 1 ise
	else if ((destPiece >= 'a' && destPiece <= 'z') && turn == 1)
	{
		return 0;
	}

	/**** Rakibin tasinin ustunden atlama ****/
	else if ((destPiece >= 'A' && destPiece <= 'Z') && turn == 1)
	{
		return 2;
	}
	else if ((destPiece >= 'a' && destPiece <= 'z') && turn == 0)
	{
		return 2;
	}

	return 1;
}
//piyon hareketi. hareket gecerliyse 1 degilse 0 dondurur
int pawnMove(char **arr, int sRow, int sCol, int eRow, int eCol, int direction)
{
	if (sRow == 6 || sRow == 1)
	{
		if (eRow == sRow + (direction * 2))
		{
			return 1;
		}
	}
	//hareket duz ise ilgili alan bos mu?
	if (eRow == sRow + direction && sCol == eCol)
	{
		if (arr[eRow][eCol] == ' ')
		{
			return 1;
		}
	}
	//hareket capraz ise hedef alan bos olmamalý
	else if (eRow == sRow + direction && ((sCol == eCol + 1) || (sCol == eCol - 1)))
	{
		if (arr[eRow][eCol] != ' ')
		{
			return 1;
		}
	}
	//buraya ulastýysa yukaridaki returnlere ulasamamistir o yuzden 0 dondur
	return 0;
}

//tasýn cinsine göre her tasi kontrol et fonksiyonlarýný yaz
//destinationCheck fonksiyonu yukarýda yazýlmýs ve hedef olarak gideceginiz
//noktadaki tasin kimin olduðuna bakar. Eðer sýranýn kendindeki oyuncunun tasý
//ise 0 dondurur. Deðilse 1 dondurur. Boylece hem hareket fonksyionunda
//gelen deger 1 hem de destinationCheck fonksiyonundan donen deger 1 ise
//canMove fonksiyonu 1 dondurur. && ile bu durum kontrol edilebilir
int canMove(char **arr, int turn, int sRow, int sCol, int eRow, int eCol)
{
	char piece = arr[sRow][sCol];
	int result;

	if (piece == 'P')
	{
		result = pawnMove(arr, sRow, sCol, eRow, eCol, 1) && destinationCheck(arr[eRow][eCol], turn);

		if (eRow == 7 && result == 1)
		{
			return 4;
		}
		else if (result == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (piece == 'p')
	{
		result = pawnMove(arr, sRow, sCol, eRow, eCol, -1) && destinationCheck(arr[eRow][eCol], turn);

		if (eRow == 0 && result == 1)
		{
			return 3;
		}
		else if (result == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	//buradan sonrasý her tas icin fonksiyon yazacaksýnýz
	//fil hareketi icin yaz
	else if (piece == 'F' || piece == 'f')
	{
		if ((eRow - sRow == eCol - sCol) || ((eRow - sRow) == -(eCol - sCol)))
		{
			while (eRow != sRow && eCol != sCol)
			{

				if (eRow - sRow > 0 && eCol - sCol > 0)
				{
					sRow++;
					sCol++;
					result = destinationCheck(arr[sRow][sCol], turn);
					if (result == 1)
					{
						if (eRow == sRow && eCol == sCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (eRow != sRow && eCol != sCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
				else if (eRow - sRow < 0 && eCol - sCol < 0)
				{
					sRow--;
					sCol--;
					result = destinationCheck(arr[sRow][sCol], turn);
					if (result == 1)
					{
						if (eRow == sRow && eCol == sCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (eRow != sRow && eCol != sCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
				else if (eRow - sRow < 0 && eCol - sCol > 0)
				{
					sRow--;
					sCol++;
					result = destinationCheck(arr[sRow][sCol], turn);
					if (result == 1)
					{
						if (eRow == sRow && eCol == sCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (eRow != sRow && eCol != sCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
				else if (eRow - sRow > 0 && eCol - sCol < 0)
				{
					sRow++;
					sCol--;
					result = destinationCheck(arr[sRow][sCol], turn);
					if (result == 1)
					{
						if (eRow == sRow && eCol == sCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (eRow != sRow && eCol != sCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
			}
		}
		else
		{
			return 0;
		}
	}
	//at hareketi icin yaz
	else if (piece == 'A' || piece == 'a')
	{
		if ((eRow == sRow + 1 && eCol == sCol + 2) ||
			(eRow == sRow + 1 && eCol == sCol - 2) ||
			(eRow == sRow - 1 && eCol == sCol + 2) ||
			(eRow == sRow - 1 && eCol == sCol - 2) ||
			(eRow == sRow + 2 && eCol == sCol + 1) ||
			(eRow == sRow - 2 && eCol == sCol + 1) ||
			(eRow == sRow + 2 && eCol == sCol - 1) ||
			(eRow == sRow - 2 && eCol == sCol - 1))
		{
			result = destinationCheck(arr[eRow][eCol], turn);
			if (result == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 0;
		}
	}
	//kale icin yaz
	else if (piece == 'K' || piece == 'k')
	{
		if (sRow == eRow)
		{
			while (sCol != eCol)
			{
				if (sCol < eCol)
				{
					sCol++;
					result = destinationCheck(arr[sRow][sCol], turn);

					if (result == 1)
					{
						if (sCol == eCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (sCol != eCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
				else
				{
					sCol--;
					result = destinationCheck(arr[sRow][sCol], turn);

					if (result == 1)
					{
						if (sCol == eCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (sCol != eCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
			}

		}
		else if (sCol == eCol)
		{
			while (sRow != eRow)
			{
				if (sRow < eRow)
				{
					sRow++;
					result = destinationCheck(arr[sRow][sCol], turn);

					if (result == 1)
					{
						if (sRow == eRow)
						{
							return 1;
						}

					}
					else if (result == 2)
					{
						if (sRow != eRow)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
				else
				{
					sRow--;
					result = destinationCheck(arr[sRow][sCol], turn);

					if (result == 1)
					{
						if (sRow == eRow)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (sRow != eRow)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
			}
		}
		else
		{
			return 0;
		}
	}
	//vezir icin yaz
	else if (piece == 'V' || piece == 'v')
	{
		if ((eRow - sRow == eCol - sCol) || ((eRow - sRow) == -(eCol - sCol)))
		{
			while (eRow != sRow && eCol != sCol)
			{

				if (eRow - sRow > 0 && eCol - sCol > 0)
				{
					sRow++;
					sCol++;
					result = destinationCheck(arr[sRow][sCol], turn);
					if (result == 1)
					{
						if (eRow == sRow && eCol == sCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (eRow != sRow && eCol != sCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
				else if (eRow - sRow < 0 && eCol - sCol < 0)
				{
					sRow--;
					sCol--;
					result = destinationCheck(arr[sRow][sCol], turn);
					if (result == 1)
					{
						if (eRow == sRow && eCol == sCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (eRow != sRow && eCol != sCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
				else if (eRow - sRow < 0 && eCol - sCol > 0)
				{
					sRow--;
					sCol++;
					result = destinationCheck(arr[sRow][sCol], turn);
					if (result == 1)
					{
						if (eRow == sRow && eCol == sCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (eRow != sRow && eCol != sCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
				else if (eRow - sRow > 0 && eCol - sCol < 0)
				{
					sRow++;
					sCol--;
					result = destinationCheck(arr[sRow][sCol], turn);
					if (result == 1)
					{
						if (eRow == sRow && eCol == sCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (eRow != sRow && eCol != sCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
			}
		}
		else if (sRow == eRow)
		{
			while (sCol != eCol)
			{
				if (sCol < eCol)
				{
					sCol++;
					result = destinationCheck(arr[sRow][sCol], turn);

					if (result == 1)
					{
						if (sCol == eCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (sCol != eCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
				else
				{
					sCol--;
					result = destinationCheck(arr[sRow][sCol], turn);

					if (result == 1)
					{
						if (sCol == eCol)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (sCol != eCol)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
			}

		}
		else if (sCol == eCol)
		{
			while (sRow != eRow)
			{
				if (sRow < eRow)
				{
					sRow++;
					result = destinationCheck(arr[sRow][sCol], turn);

					if (result == 1)
					{
						if (sRow == eRow)
						{
							return 1;
						}

					}
					else if (result == 2)
					{
						if (sRow != eRow)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
				else
				{
					sRow--;
					result = destinationCheck(arr[sRow][sCol], turn);

					if (result == 1)
					{
						if (sRow == eRow)
						{
							return 1;
						}
					}
					else if (result == 2)
					{
						if (sRow != eRow)
						{
							return 0;
						}
						else
						{
							return 1;
						}

					}
					else
					{
						return 0;
					}
				}
			}
		}
		else
		{
			return 0;
		}
	}
	//sah icin yaz
	else if (piece == 'S' || piece == 's')
	{
		int result2;
		if ((eRow == sRow || eRow == sRow + 1 || eRow == sRow - 1) && (eCol == sCol || eCol == sCol + 1 || eCol == sCol - 1))
		{
			result = destinationCheck(arr[eRow][eCol], turn);
			if (result == 1)
			{
				result2 = isAvailable(arr, turn, eRow, eCol);
				if (result2 == 0)
				{
					return 0;
				}
				return 1;
			}
			else
			{

				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	//yukarýdaki kodlar yazýldýðýnda buraya ulasamaz ama su anki halinde warning almamak icin yazýldý.
	//Tum kodu yazýnca silebilirsiniz.
	return 0;

}

void play(char **arr, int *turn)
{
	char *sMove, *eMove;
	int sRow, sCol, eRow, eCol;
	sMove = malloc(sizeof(char) * 3);
	eMove = malloc(sizeof(char) * 3);
	do
	{
		printf("%d. player turn: ", *turn + 1);
		scanf("%2s %2s", sMove, eMove);
		fflush(stdin);
		sRow = (int)(sMove[0] - 'A');
		sCol = (int)(sMove[1] - '1');
		eRow = (int)(eMove[0] - 'A');
		eCol = (int)(eMove[1] - '1');
		if (
			(sRow >= 0 && sRow <= 7) &&
			(sCol >= 0 && sCol <= 7) &&
			(eCol >= 0 && eCol <= 7) &&
			(eRow >= 0 && eRow <= 7)
			)
		{
			char piece = arr[sRow][sCol];
			//secilen tas sýrasý gelen kullanýcýnýn mý
			if (
				(*turn == 0 && (piece >= 'A' && piece <= 'Z'))
				||
				(*turn == 1 && (piece >= 'a' && piece <= 'z'))
				)
			{
				//secilen tas hedef konuma gidebilir mi
				if (canMove(arr, *turn, sRow, sCol, eRow, eCol) == 1)
				{

					arr[eRow][eCol] = piece;
					arr[sRow][sCol] = ' ';
					*turn = (*turn + 1) % 2;
					break;
				}
				else if (canMove(arr, *turn, sRow, sCol, eRow, eCol) == 3)
				{
					arr[eRow][eCol] = 'v';
					arr[sRow][sCol] = ' ';
					*turn = (*turn + 1) % 2;
					break;
				}
				else if (canMove(arr, *turn, sRow, sCol, eRow, eCol) == 4)
				{
					arr[eRow][eCol] = 'V';
					arr[sRow][sCol] = ' ';
					*turn = (*turn + 1) % 2;
					break;
				}
				//eger gidemiyorsa illegal move
				else
				{
					printf("Illegal move. Try again.\n");
				}

			}
			//secilen tas kullanýcýnýn degil
			else
			{
				printf("It's your opponent's piece\n");
			}
		}
		//istenen konumlar aralýkta girilmemis
		else
		{
			printf("Your move is out of boundary\n");
		}
	} while (1);//dogru hareket yapýlana kadar devam eder. Dogru hareket yapýldýysa
			  //yukarýdaki break bu while kosulunu kýrar ve cýkýlýr.
}
//dizi iceriklerini ekrana yaz.
//her bir tas bir karenin icine girsin diye
//bir kac duzenleme yapildi
void printScreen(char ** arr, int nRows, int nCols)
{

	//ekraný her seferinde silip tekrar yazmak isterseniz.
	//aþagýdaki komutu aciniz
	system("@cls||clear");

	int i, j;

	printf("%s", " |");
	for (j = 0; j < nCols;)
	{
		printf("  %d  |", ++j);
	}

	printf("\n--");

	for (j = 0; j < nCols; j++)
	{
		printf("%s", "------");
	}

	printf("\n");

	for (i = 0; i < nRows; i++)
	{
		printf("%c|", 'A' + i);

		for (j = 0; j < nCols; j++)
		{
			printf("  %c  |", arr[i][j]);
		}

		printf("\n--");

		for (j = 0; j < nCols; j++)
		{
			printf("%s", "------");
		}
		printf("\n");
	}
}
void initializeTheBoard(char **arr, int nRows, int nCols)
{
	int i, j;
	for (i = 0; i < nRows - 2; i++)
	{
		for (j = 0; j < nCols; j++)
		{
			if (i == 0)
			{
				if (j == 0 || j == nCols - 1)
				{
					arr[i][j] = 'K';
				}

				else if (j == 1 || j == nCols - 2)
				{
					arr[i][j] = 'A';
				}

				else if (j == 2 || j == nCols - 3)
				{
					arr[i][j] = 'F';
				}
				else if (j == 3)
				{
					arr[i][j] = 'V';
				}
				else
				{
					arr[i][j] = 'S';
				}
			}
			else if (i == 1)
			{
				arr[i][j] = 'P';
			}
			else
			{
				arr[i][j] = ' ';
			}
		}
	}
	//last 2 rows for opponent
	for (i = nRows - 2; i < nRows; i++)
	{
		for (j = 0; j < nCols; j++)
		{
			//add 32 to lower the opponent's character
			arr[i][j] = arr[nRows - i - 1][j] + 32;
		}
	}
}

int isAvailable(char **arr, int turn, int eRow, int eCol)
{
	int i, j, result, result2;

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			result = destinationCheck(arr[i][j], turn);
			if (result == 2)
			{
				result2 = canMove(arr, turn, i, j, eRow, eCol);
				if (result2 == 1)
				{
					printf("You can't make this move because the game ends.  ");
					return 0;
				}
			}
		}
	}
}

