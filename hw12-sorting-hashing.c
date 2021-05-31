/*
 * hw12-sorting-hashing.c
 *
 *  Created on: May 22, 2021
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */	
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE	//충돌을 줄이기 위해, hash table의 크기를 소수로 설정 

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a);
int freeArray(int *a);
void printArray(int *a);

int selectionSort(int *a);
int insertionSort(int *a);
int bubbleSort(int *a);
int shellSort(int *a);
/* recursive function으로 구현 */
int quickSort(int *a, int n);


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);

/* array a에 대한 hash table을 만든다. */
int hashing(int *a, int **ht);

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key);


int main()
{
	char command;
	int *array = NULL;
	int *hashtable = NULL;
	int key = -1;
	int index = -1;

	srand(time(NULL));
	printf("[----- [jinyounglee] [2020039063] -----]\n");

	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array);	//single pointer의 주소를 인자로 사용
			break;
		case 'q': case 'Q':
			freeArray(array);
			break;
		case 's': case 'S':
			selectionSort(array);
			break;
		case 'i': case 'I':
			insertionSort(array);
			break;
		case 'b': case 'B':
			bubbleSort(array);
			break;
		case 'l': case 'L':
			shellSort(array);
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);	//정렬 전 배열array 출력 
			quickSort(array, MAX_ARRAY_SIZE);
			printf("----------------------------------------------------------------\n");
			printArray(array);	//정렬 후 배열array 출력

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);			//배열 출력	
			hashing(array, &hashtable);	//hashing 함수 호출 
			printArray(hashtable); 		//hashtable 출력
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key);		//key 입력
			printArray(hashtable);	//hashtable 출력
			index = search(hashtable, key);	//hashtable에서 입력한 key에 대한 주소를 index에 대입
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);	//관련 정보 출력
			break;

		case 'p': case 'P':
			printArray(array);		//배열 출력
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

/* 배열 array 초기화 */
int initialize(int** a)	//double pointer 사용-> 배열 array의 주소, 즉 *a를 변경하겠다는 의미 
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a;	// array가 NULL이 아닌 경우 temp가 array를 가리키도록 설정 

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;

	return 0;
}

/* 배열 메모리 해제 */
int freeArray(int *a) 
{
	if(a != NULL)	//array가 NULL이 아닌 경우 메모리 해제
		free(a);
	return 0;
}

/* 배열 출력 함수 */
void printArray(int *a)	
{
	if (a == NULL) {	//array가 NULL인 경우 
		printf("nothing to print.\n");
		return;
	}
	//array가 NULL이 아닌 경우
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("a[%02d] ", i);		//배열의 어떤 인덱스인지 출력 
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]);		//배열의 해당 인덱스 값 출력
	printf("\n");
}

/* 선택 정렬 
	-> 정렬되어 있지 않은 부분에서 최솟값을 찾는 과정을 반복하여 정렬-> 앞에서부터 정렬*/
int selectionSort(int *a)
{
	int min;
	int minindex;
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);	//정렬 전 배열 array 출력

	for (i = 0; i < MAX_ARRAY_SIZE; i++)	//정렬할 위치 제시
	{
		minindex = i;	//가장 작은 배열 값의 인덱스를 i로 설정 
		min = a[i];		//가장 작은 배열 값을 a[i]로 설정 
		for(j = i+1; j < MAX_ARRAY_SIZE; j++)	//비교할원소 제시
		{
			if (min > a[j])	//min값보다 a[j]값이 더 작으면 -> 해당과정에서 a[j]가 가장 작은 값으로, min과 minindex 재설정
			{	
				min = a[j];		//min은 a[j]로 재설정
				minindex = j;	//minindex는 j로 재설정 
			}
		}
		a[minindex] = a[i];		//배열의 인덱스i에 대한 값과 최소값 교환 -> 교환할 때 minindex와 min를 이용하여, 교환과정의 코드 줄임 
		a[i] = min;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);	//정렬 후 배열 array 출력
	return 0;
}

/* 삽입 정렬 
	-> 정렬되어 있는 부분집합에 새로운 원소의 위치를 찾아 삽입 */
int insertionSort(int *a)
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);	//정렬 전 배열 array 출력

	for(i = 1; i < MAX_ARRAY_SIZE; i++)	//배열의 첫번째 원소, a[0]은 정렬되어 있다고 본다 -> 비정렬된 원소의 위치는 인덱스 1번부터 본다
	{
		t = a[i];			//정렬할 배열값(비정렬된 집합의 첫번째 원소)를 t에 복사 
		j = i;
		while (a[j-1] > t && j > 0)		//정렬된 집합의 원소가 비정렬된 원소t보다 크고, j>0일 때 반복된다.
		{
			a[j] = a[j-1];	//a[j]에 a[j-1] 복사 -> a[j-1]위치가 비어있다고 생각한다
			j--;			//j를 1씩 감소하며, 삽입할 위치를 찾는다
		}
		a[j] = t;			//삽입할 위치를 찾았으므로, a[j]에 t를 삽입한다
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);	//정렬 후 배열 array 출력

	return 0;
}

/* 버블 정렬 
	-> 두 인접원소를 검사하여 정렬하는 방법으로, 최대값을 비정렬된 부분 중 제일 뒤로 보내는 것을 반복한다(뒤에서부 정렬)*/
int bubbleSort(int *a)
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);	//정렬 전 배열 array 출력

	for(i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)
		{
			if (a[j-1] > a[j])	//인접한 두 원소 중 앞의 원소가 클 때 -> 교환
			{
				t = a[j-1];		//원소를 교환하기 위해, 변수 t를 이용한다
				a[j-1] = a[j];
				a[j] = t;
			}
			//인접한 두 원소 중 뒤의 원소가 클 때 -> 교환x(이미 정렬되어 있기 때문)
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);	//정렬 후 배열 array 출력

	return 0;
}

/* 쉘 정렬 
	-> insertion sort의 문제점 보완하기 위한 정렬로, h만큼의 간격으로 떨어진 레코드를 삽입 정렬하는 정렬
	insertion sort의 문제점: 정렬되기 위해 비교와 교환횟수가 많음*/
int shellSort(int *a)
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);	//정렬 전 배열 array 출력

	/* h는 부분집합의 기준이 되는 간격을 의미한다 */
	//h를 배열크기의 1/2로 초기화하고, 단계가 수행될 때마다 h를 1/2씩 줄이고, h가 1이 될 때까지 반복한다.
	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2)	
	{
		for (i = 0; i < h; i++)	//h번 반복 
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h)	//비교할 원소 지정(i+h)
			{
				v = a[j];	
				k = j;
				while (k > h-1 && a[k-h] > v)	//k>h-1이고 k와 h간격만큼 앞에 위치한 배열값과 v 비교시, v가 더 작은 경우 -> a[k-h]가 v(=a[k])로 이동 
				{
					a[k] = a[k-h];	
					k -= h;		//k를 h간격만큼 왼쪽으로 이동 
				}
				//k<=h-1 || a[k-h]<=v 일 때, a[k]를 v로 설정 
				a[k] = v;
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);	//정렬 후 배열 array 출력

	return 0;
}

/* 퀵 정렬
	-> 기준값(pivot)을 중심으로 왼쪽 부분집합과 오른쪽 부분집합으로 분할하며 정렬 */
int quickSort(int *a, int n)	//오름차순 
{
	int v, t;
	int i, j;

	if (n > 1)	//n<=1이면 정렬할 필요없음(이미 정렬된것과 마찬가지)
	{
		v = a[n-1];		//v는 기준값(pivot)으로, v를 가장 마지막 값(n-1번 원소)로 정함 
		//뒤의 반복문에서 i와 j를 각각 증가, 감소한 후에 기준값 v와 비교하므로 아래와 같이 i와 j를 설정함 
		i = -1;
		j = n - 1;

		while(1)
		{
			while(a[++i] < v);	//배열값이 v보다 작을때까지 i증가하기 -> 정렬할 위치 i 찾기
			while(a[--j] > v);	//배열값이 v보다 클때까지 j감소하기	-> 정렬할 위치 j 찾기 

			if (i >= j) break;	// i>=j인 경우 반복문 빠져나옴
			//i<j인 경우, a[i]와 a[j] 교환
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
		//i>=j인 경우, a[i]와 a[n-1] 교환(기준값과 a[i] 교환) 
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;

		// recursive 방식 이용 -> 기준값을 기준으로 왼쪽과 오른쪽 집합들은 독립적으로 정렬됨
		quickSort(a, i);			//왼쪽 부분 집합(기준값보다 작은 원소들의 집합) 정렬
		quickSort(a+i+1, n-i-1);	//오른쪽 부분 집합(기준값보다 큰 원소들의 집합) 정렬
	}


	return 0;
}

/* hash code generator, key % MAX_HASH_TABLE_SIZE */ 
/* 해시 함수로, division 함수 이용 
	-> division 함수: 키 값이 음이 아닌 정수라고 가정하고, 
	home bucket은 모드(%) 연산자에 의해 결정하여 key를 MAX_HASH_TABLE_SIZE로 나눈 나머지를 key의 home bucket으로 사용 */
int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE;	
}

/* array a에 대한 hash table을 만든다. */
int hashing(int *a, int **ht)
{
	int *hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);	//배열array의 크기만큼 메모리 할당 
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;	//hash table의 각각의 위치의 값을 -1로 초기화 
	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);	//hash table 초기값 출력  
	*/

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)	//배열의 크기만큼 반복
	{
		key = a[i];					//key를 a[i]로 설정 
		hashcode = hashCode(key);	//key에 대한 home bucket을 hashcode에 대입 
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		if (hashtable[hashcode] == -1)	//hashtable의 hashcode위치의 값이 -1로, 해당 home bucket에 값을 저장할 수 있는 경우(=home bucket이 비어있는 경우)-> key를 대입함
		{
			hashtable[hashcode] = key;	
		} else 	{						//hashtale의 hashcode위치의 값이 -1이 아닌, 즉 key에 대한 home bucket이 정해진 적이 있는 경우(collision & overflow 발생한 경우 )

			index = hashcode;			//index를 hashcode로 설정 

			while(hashtable[index] != -1)		//collision & overflow가 발생하는 동안 반복
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;	//index에 1 증가시킨 값을 나머지연산한 결과값을 index로 설정(빈 home bucket을 찾는 과정)
				/*
				printf("index = %d\n", index);
				*/
			}
			//hashtable의 index값이 -1인 경우(해당 home bucket이 비어있는 경우) -> hashtable의 index 위치의 값을 key로 설정 
			hashtable[index] = key;	
		}
	}

	return 0;
}


/* hash table에서 key를 찾아 hash table의 index return 
	-> 입력된 key의 위치를 hash table에서 찾아, 주소 return */
int search(int *ht, int key)
{
	int index = hashCode(key);	//index는 key의 home bucket 정보를 저장하는 변수

	if(ht[index] == key)	//해당 key값이 hash table의 index위치에 저장된 경우  -> key의 주소인 index 반환
		return index;

	// 해당 key값이 hash table의 index위치에 저장되지 않은  경우(collsion & overflow로 다른 위치에 저장됨) -> 해당 key가 저장된 곳을 찾아야 함 
	while(ht[++index] != key)	//index를 1증가한 위치에서의 hash table의 값이 key와 동일하지 않는 동안 반복
	{
		index = index % MAX_HASH_TABLE_SIZE;	//나머지 연산의 결과값으로 index 재설정 
	}
	//해당 key값이 저장된 home bucket의 주소 index 반환 
	return index;	
}