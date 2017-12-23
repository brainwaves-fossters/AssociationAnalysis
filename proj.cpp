//Program to demonstrate Association Analysis
//Add any number of items to the "char items[][20]" which is declared as global
//Depending on the number of items items added update global variable "int num" which stores the total count
//ndx stands for index
//since integer array is used, -1 is taken as end of the array

#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

using namespace std;

void copy( int[1000][100], char[], int& );	
//converts entered string to number for storing 

void copy( int[100][100], int[100][100], int&, int& );		
//copies old frequent item set to new frequent item set

void combine( int[100][100], int[100][100], int[1000][100], int, int, int&, int, int );	
//combines each frequent item set with other frequent item sets

int check( int[200], int[1000][100], int, int, int);	
//checks if the two rows are same

int compare( int[100][100], int[100][100], int, int );	
//compares old frequent item set to new frequent item set

void eliminate_dupli( int[100][100], int& );	
//eliminates duplicate rows in the frequent item sets

int compare_rows( int[100][100], int[100][100], int, int);	
//compares if two rows are same

void copy_rows( int[100][100], int[100][100], int&, int );
//copies non subset rows of old frequent item set to new frequent item set

int length( int[100][100], int );
//returns length of the array i.e until -1 is found

int check_subset( int[100][100], int[100][100], int, int);
//while combining old frequent item set to new frequent item set if no subset is found in the old fre_item_set it is copied as it is to the new one..returns 1 if no subset is found

void num_binary(int[], int, int );
//converts number to its binary equivalent

void power_set(int[], int, int[1000][100], int );
//generates all power sets for Rule Generation


char items[][20] = {"Beer","Bread","Cola","Diapers","Milk","Eggs","Badam","Pista","Ice-cream","diary milk"};
//Add items here

int num = 10;
//total count of items

int main()
{
	int shop[1000][100], shop_ndx = -1;		//stores all transactions based on their id's
	int old_fre_item_set[100][100], old_fre_ndx = -1;	//stores old frequent item set and its index
	
	int fre_item_set[100][100], fre_ndx = -1;	//stores frequent item sets and its index
	int choice, count;
	
	int i, j, k;
	int min_sup, level;		//minimum support count

	char str[200];		//to scan the string entered
	char c, n;

	system("clear");

	while( 1 )
	{
		
		cout << "\n\n\n1.Shopping\n2.Show all Transactions\n3.Compute Association Analysis\n4.Exit\nEnter your chocie : ";
		cin >> choice;
		fflush(stdin);		//to clear the input buffer
		cout << "\n\n";

		switch( choice )
		{
		case 1:
			do
			{
				cout << "\n\n";
				for(i = 0; i < num; i++)	//displays list of items
					cout << i+1 << "." << items[i] << endl;
				cout << "\nEnter the item numbers to be purchased seperated by spaces :\n";
		
				n = getchar();
				cin.getline( str, 200 );	//scans the entered string

				copy( shop, str , shop_ndx );	//converts string to number for storing
	
				cout << "\nShop again? Type [y] or [n] and press enter \n";
				cin >> c;	//y for yes n for no
	
			}
			while( c == 'y' );
			
			break;

		case 2:
			if( shop_ndx < 0 )
			{
				cout << "\nNo Transactions done\n";
				break;
			}

			for( i = 0; i <= shop_ndx; i++ )
			{
				for( j = 0; shop[i][j] != -1; j++)
					cout << items[shop[i][j]-1] << ", ";
				cout << "\b\b  \n";
			}
			break;

		case 3:
			if( shop_ndx < 1 )
			{
				cout << "\nMore Transactions needed for Analysis\n";
				break;
			}

			min_sup = (60 * (shop_ndx+1) ) / 100;	//computes minimum support
			level = 1;		//for Frequent 1-itemsets
			
			for( i = 0; i < num; i++ )	//generates Frequent 1-itemsets
			{
				count = 0;	
				for( k = 0; k <= shop_ndx; k++ )	//searches all transcations
				{
					for( j = 0; shop[k][j] != -1; j++)
						if( shop[k][j] == i+1 )
							count++;
				}
				if( count >= min_sup )		//if minimum support is greater than threshold value it is added to the frequent item set
				{
					fre_item_set[++fre_ndx][0] = i+1;
					fre_item_set[fre_ndx][1] = -1;
				}
			}
			
			
			copy( fre_item_set, old_fre_item_set, fre_ndx, old_fre_ndx );
			level++;
			
			
			while(1)		//starts combining Frequent itemsets
			{
				for( i = 0; i < old_fre_ndx; i++ )	
				{
					for( j = i+1; j <= old_fre_ndx; j++ )	//combines each frequent item set with other frequent item sets
					{
						combine( fre_item_set, old_fre_item_set, shop, i, j, fre_ndx, level, shop_ndx );
					}
				}
				if( fre_ndx == -1 )		//No combination is formed
				{
					fre_ndx = old_fre_ndx;
					break;
				}
				if( compare( fre_item_set, old_fre_item_set, fre_ndx, old_fre_ndx ) )	//Same combinatation persists
				{
					break;
				}
				
				eliminate_dupli( fre_item_set, fre_ndx );	//eliminates duplicate rows in the frequent item sets
				
				for( i = 0; i <= old_fre_ndx; i++ )
				{
					if( check_subset(fre_item_set, old_fre_item_set, fre_ndx, i ) )
						copy_rows( fre_item_set, old_fre_item_set, fre_ndx, i );
				}

				copy( fre_item_set, old_fre_item_set, fre_ndx, old_fre_ndx );
				level++;
			}
			
			cout << "\nThe various Frequent item-sets are\n\n";
			for( i = 0; i <= fre_ndx; i++ )
			{
				for( j = 0; fre_item_set[i][j] != -1; j++ )
					cout << items[fre_item_set[i][j]-1] << ", ";
				cout << "\b\b  \n";
			}
			
			//RULE GENERATION
			cout << endl;
			
			cout << "\n\nProducts depeneded on each other are\n\n";
			for( i = 0; i <= fre_ndx; i++ )
			{
				power_set( fre_item_set[i], length( fre_item_set, i), shop, shop_ndx );
				
			}
			


			break;

		case 4:
			exit( 0 );
		}
	}
	return 0;
}


void copy( int shop[1000][100], char str[], int &shop_ndx )	//converts entered string to number for storing
{
	int i = 0, len = strlen( str );
	char num[10], num_ndx;
	int n = 0;

	if( strlen(str) != 0 )
	{
		shop_ndx++;
		
		while( i < len )
		{
			if( !(str[i] >= '0' && str[i] <= '9') )
			{
				i++;
				continue;
			}
			num_ndx = 0;
			while( str[i] >= '0' && str[i] <= '9' )
				num[num_ndx++] = str[i++];
			shop[shop_ndx][n] = atoi( num );
			n++;
		}
	}
	shop[shop_ndx][n] = -1;
}


void copy( int fre_item_set[100][100], int old_fre_item_set[100][100], int &fre_ndx, int &old_fre_ndx )
//copies old frequent item set to new frequent item set
{
	int i, j;
	for(i = 0; i <= fre_ndx; i++)
	{
		for(j = 0; fre_item_set[i][j] != -1; j++)
			old_fre_item_set[i][j] = fre_item_set[i][j];
		old_fre_item_set[i][j] = -1;
	}
	old_fre_ndx = fre_ndx;
	fre_ndx = -1;
}


void combine( int fre_item_set[100][100], int old_fre_item_set[100][100], int shop[1000][100], int row1, int row2, int &fre_ndx, int level, int shop_ndx )	//combines each frequent item set with other frequent item sets
{
	int combn_row[200], flag;
	int i, j, k;

	
	for( i = 0; old_fre_item_set[row1][i] != -1; i++)
	{
		combn_row[i] = old_fre_item_set[row1][i];	
	}


	for( j = 0; old_fre_item_set[row2][j] != -1; j++)
	{
		flag = 1;
		for( k = 0; k < i; k++ )
		{
			if( combn_row[k] == old_fre_item_set[row2][j] )
			{
				flag = 0;
				break;
			}
		}
		if( flag == 1 )
		{
			combn_row[i++] = old_fre_item_set[row2][j];
		}
	}
	
	combn_row[i] = -1;
	if( i == level )
	{
		if( check( combn_row, shop, shop_ndx, i, 1) )
		{
			fre_ndx++;
			for( j = 0; j <= i; j++ )
				fre_item_set[fre_ndx][j] = combn_row[j];
		}
	}
}


int check( int combn_row[200], int shop[1000][100], int shop_ndx, int col, int call_flag)	//checks if the two rows are same

{
	int count = 0, item_count = 0;
	int min_sup = (60 * (shop_ndx+1) ) / 100;
	int i, j, k, flag;
	for( i = 0; i <= shop_ndx; i++ )
	{
		item_count = 0;
		for( j = 0; shop[i][j] != -1; j++)
		{
			flag = 0;
			for( k = 0; k < col; k++ )
			{
				if( shop[i][j] == combn_row[k] )
				{
					flag = 1;
					break;
				}
			}
			if( flag == 1 )
				item_count++;
		}
		if( item_count == col )
			count++;
	}
	if( call_flag == 1 )
	{
		if( count >= min_sup )
			return 1;
		else
			return 0;
	}
	else
		return count;		
}


int compare( int fre_item_set[100][100], int old_fre_item_set[100][100], int fre_ndx, int old_fre_ndx )	//compares old frequent item set to new frequent item set
{
	int i, j;
	if( fre_ndx != old_fre_ndx )
		return 0;
	
	for( i = 0; i <= old_fre_ndx; i++)
	{
		for( j = 0; fre_item_set[i][j] != -1; j++ )
		{
			if( fre_item_set[i][j] != old_fre_item_set[i][j] )
				return 0;
		}
		if( old_fre_item_set[i][j] != -1 )
			return 0;
	}
	
	return 1;
}
	

void eliminate_dupli( int fre_item_set[100][100], int &fre_ndx ) 	//eliminates duplicate rows in the frequent item sets
{
	int i, j, flag;
	int temp[100][100], temp_ndx;

	temp_ndx = fre_ndx;	
	for( i = 0; i <= temp_ndx; i++ )		//copying it to temporary array
	{
		for( j = 0; fre_item_set[i][j] != -1; j++ )
		{
			temp[i][j] = fre_item_set[i][j];
		}
		temp[i][j] = -1;
	}
	fre_ndx = -1;

	for( i = 0; i <= temp_ndx; i++ )		//for the temp array
	{
		flag = 1;
		for( j = 0; j <= fre_ndx; j++ )
		{
			if( compare_rows( fre_item_set, temp, i, j ) )
			{
				flag = 0;
				break;
			}
		}
		if( flag == 0 )
			continue;
		else
			copy_rows( fre_item_set, temp, fre_ndx, i );
	}
}



int compare_rows( int fre_item_set[100][100], int temp[100][100], int temp_row, int fre_row )	
//compares if two rows are same
{
	int i, j;

	if( length( fre_item_set, fre_row ) != length( temp, temp_row ) )
		return 0;
	for( i = 0; temp[temp_row][i] != -1; i++)
	{
		for( j = 0; fre_item_set[fre_row][j] != -1; j++)
		{
			if( temp[temp_row][i] == fre_item_set[fre_row][j] )
			{
				break;
			}
		}
		if( fre_item_set[fre_row][j] == -1 )
			return 0;
	}
	return 1;
}

		


void copy_rows( int fre_item_set[100][100], int temp[100][100], int &fre_ndx, int temp_row )
//copies non subset rows of old frequent item set to new frequent item set
{
	int i;
	fre_ndx++;

	for( i = 0; temp[temp_row][i] != -1; i++ )
		fre_item_set[fre_ndx][i] = temp[temp_row][i];
	fre_item_set[fre_ndx][i] = -1;
}


int length( int t[100][100], int r )
{
	int i, count = 0;
	for( i = 0; t[r][i] != -1; i++ )	
		count++;
	return count;
}
		

int check_subset( int fre_item_set[100][100], int old_fre_item_set[100][100], int fre_ndx, int row)
//while combining old frequent item set to new frequent item set if no subset is found in the old fre_item_set it is copied as it is to the new one..returns 1 if no subset is found
{
	int i, j, k, count = 0;
	int actual_count = length( old_fre_item_set, row );

	for( k = 0; k <= fre_ndx; k++ )
	{ 
		count = 0;
		for( i = 0; fre_item_set[k][i] != -1; i++)
		{
			
			for( j = 0; old_fre_item_set[row][j] != -1; j++)
			{
				if( fre_item_set[k][i] == old_fre_item_set[row][j] )
				{
					count++;
					break;
				}
			}
			
		}
		if( count == actual_count )
			return 0;
	}
		return 1;
}









void num_binary(int binary[], int num, int count)
{
	int i, j, a[100], bin[100];
	for(i = 0; num != 0; i++)
	{
		a[i] = num % 2;	/*its binary form gets stored in reverse order*/
		num = num / 2;
	}
	i--;
	for(j = 0; i >= 0; i--,j++)
		bin[j] = a[i];	/*reverse the array a to get the actual order*/
	j--;
	for(i = 0; j >= 0; j--, i++)
		binary[count-i-1] = bin[j];     /*shifting postions to get the truth table*/
	
	while((count-i-1) >= 0)		
	{
		binary[count-i-1] = 0;	/*Initializing unfilled elements to 0*/
		i++;
	}
}

void power_set(int a[], int count, int shop[1000][100], int shop_ndx)
{
	int num, i, j, binary[100], k;
	int sup_count_uni = check( a, shop, shop_ndx, count, 0);
	int sup_count_sub;
	float confi;
	

	char c;
	
	for(i = 1; i < pow(2,count)-1; i++)      /*to get all combinations*/
	{
		int left_row[200], left_ndx = 0 ;
		num_binary(binary, i, count);
		
		for(j = 0; j < count; j++)
		{
			if(binary[j] == 1)
				left_row[left_ndx++] = a[j];
		}

		sup_count_sub = check( left_row, shop, shop_ndx, left_ndx, 0);
		
		confi = (float)sup_count_uni / sup_count_sub;		//computes confidence
		
		if( confi >= 0.9 )
		{
			for(j = 0; j < left_ndx; j++)
				cout << items[ left_row[j]-1 ] << ",";

			cout << "\b   ->    ";
			
			for(j = 0; j < count; j++)
			{
				if(binary[j] == 0)
					cout << items[ a[j]-1 ] << ",";
			}
			
			cout << "\b " << endl;
		}
		
		for(k = 0; k  <100; k++)
			binary[i] = 0;
		
	}
	
}
