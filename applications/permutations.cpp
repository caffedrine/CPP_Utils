#include <stdio.h>

void swap(int *x, int *y)
{
	*x ^= *y;
	*y ^= *x;
	*x ^= *y;
}

void print(int vector[], int len)
{
	for( int i = 0; i < len; i++ )
		printf("%d ", vector[i]);
	printf("\n");
}

void sort(int vector[], int len)
{
	for( int i = 0; i < len; i++ )
	{
		for( int j = 0; j < len; j++ )
		{
			if( vector[j] > vector[i] )
				swap(&vector[i], &vector[j]);
		}
	}
}

void reverse(int vector[], int startPosition, int endPosition)
{
	int i, j;
	for(i=startPosition, j=(endPosition); i<j; ++i, --j)
	{
		swap(&vector[i], &vector[j]);
	}
}

bool isPalindrom(int vector[], int len)
{
	int i, j;
	for(i=0, j=(len-1); i<j; ++i, --j)
	{
		if(vector[i] != vector[j])
			return false;
	}
	return true;
}

bool next_permutation(int vector[], int len)
{
	/* find pivot index */
	int PivotPosition = 0;
	for( int i = len - 2; i >= 0; i-- )
	{
		if( vector[i] < vector[i+1] )
		{
			PivotPosition = i;
			break;
		}
	}
	if (PivotPosition == 0 && vector[PivotPosition + 1] <= vector[PivotPosition])
	{
		return false;
	}
	
	/* find succesor index*/
	int SuccesorPosition = 0;
	for(int i = len - 1; i > PivotPosition; i--)
	{
		if( vector[i] > vector[PivotPosition])
		{
			SuccesorPosition = i;
			break;
		}
	}

	/* Swap pivot value with succesor value */
	swap(&vector[PivotPosition], &vector[SuccesorPosition]);
	
	/* Reverse elements after pivot */
	reverse(vector, PivotPosition + 1, len - 1);
	
	return true;
}

int main()
{
	const int len = 4;
	int vector[len] = {1, 0, 1, 0};
	sort(vector, len);
	
	do
	{
		if(isPalindrom( vector, len) )
			print(vector, len);
	} while( next_permutation(vector, len) );
	
	return 0;
}