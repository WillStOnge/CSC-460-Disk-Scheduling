#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

/* Assume no more than 1000 requests in the input file */
#define BUFSIZE 1000

/* Assume 5000 cylinders */
#define CYL 5000

/* Assume no requests will be longer than 80 characterse */
#define LINELEN 80

// Boolean type.
typedef enum { false, true } bool;

// Sorts an array, uses selection sort.
void sort(int arr[], int len, bool reverse)
{
	int i, j, min, temp, start, end;

	// Selection sort.
	for (i = 0; i < len; i++)
	{
		min = i;
		for (j = i + 1; j < len; j++)
			if (arr[j] < arr[min])
				min = j;
		temp = arr[min];
		arr[min] = arr[i];
		arr[i] = temp;
	}

	// Reverse the array if needed.
	if (reverse)
	{
		start = 0;
		end = len - 1;
		while (start < end)
		{
			temp = arr[start];
			arr[start] = arr[end];
			arr[end] = temp;
			start++;
			end--;
		}
	}
}

// Calculates the order of the requests using the SSTF algorithm. Modifies the order parameter to return it.
void findSSTFOrder(int requests[], int initpos, int len, int order[])
{
	int i, j, distance[len], current = 0;
	bool used[len];

	memset(used, false, sizeof(bool) * len);

 	for (i = 0; i < len; i++)
	{
		// Calculate distance (Used elements will have 'infinite' distance.
		for (j = 0; j < len; j++)
			distance[j] = used[j] ? INT_MAX : abs(requests[j] - initpos);

		// Find closest.
		for (j = 0; j < len; j++)
			if (distance[current] > distance[j])
				current = j;

		// Add the current element to the order array and mark that element as used.
		order[i] = requests[current];
		used[current] = true;
	}
}

// Calculates the order of the requests using the SCAN algorithm. Modifies the order parameter to return it.
void findScanOrder(int requests[], int initpos, int len, int order[])
{
	int i, indexl = 0, indexr = 0, right[len];

	// Seperates the numbers into 2 seperate arrays for the 2 parts of the SCAN.
	for (i = 0; i < len; i++)
	{
		if (requests[i] > initpos)
			right[indexr++] = requests[i];
		else
			order[indexl++] = requests[i];
	}

	// Sorts the left side in reverse order and the right in normal order.
	sort(order, indexl, true);
	sort(right, indexr, false);

	// Add 0 in since we have to go all the way to cylinder 0.
	order[indexl++] = 0;

	// Add the right array on top of the left array to get the proper sequence, then return it.
	for (i = indexl; i < len + 1; i++)
		order[i] = right[i - indexl];
}

// Calculates the order of the requests using the C-SCAN algorithm. Modifies the order parameter to return it.
void findCScanOrder(int requests[], int initpos, int len, int order[])
{
	int i, indexl = 0, indexr = 0, right[len];

	// Seperates the numbers into 2 seperate arrays for the 2 parts of the SCAN.
	for (i = 0; i < len; i++)
	{
		if (requests[i] > initpos)
			order[indexl++] = requests[i];
		else
			right[indexr++] = requests[i];
	}

	// Sorts the 2 lists in ascending order.
	sort(order, indexl, false);
	sort(right, indexr, false);

	// Need to go to the last cylinder and then return to 0 before servicing more requests.
	order[indexl++] = CYL - 1;
	order[indexl++] = 0;

	// Add the right array on top of the left array to get the proper sequence, then return it.
	for (i = indexl; i < len + 2; i++)
		order[i] = right[i - indexl];
}

// Calculates the order of the requests using the LOOK algorithm. Modifies the order parameter to return it.
void findLookOrder(int requests[], int initpos, int len, int order[])
{
	int i, indexl = 0, indexr = 0, right[len];

	// Seperates the numbers into 2 seperate arrays for the 2 parts of the SCAN.
	for (i = 0; i < len; i++)
	{
		if (requests[i] > initpos)
			right[indexr++] = requests[i];
		else
			order[indexl++] = requests[i];
	}

	// Sorts the left side in reverse order and the right in normal order.
	sort(order, indexl, true);
	sort(right, indexr, false);

	// Add the right array on top of the left array to get the proper sequence, then return it.
	for (i = indexl; i < len; i++)
		order[i] = right[i - indexl];
}

// Calculates the order of the requests using the C-LOOK algorithm. Modifies the order parameter to return it.
void findCLookOrder(int requests[], int initpos, int len, int order[])
{
	int i, indexl = 0, indexr = 0, right[len];

	// Seperates the numbers into 2 seperate arrays for the 2 parts of the SCAN.
	for (i = 0; i < len; i++)
	{
		if (requests[i] > initpos)
			order[indexl++] = requests[i];
		else
			right[indexr++] = requests[i];
	}

	// Sorts the 2 lists in ascending order.
	sort(order, indexl, false);
	sort(right, indexr, false);;

	// Add the right array on top of the left array to get the proper sequence, then return it.
	for (i = indexl; i < len; i++)
		order[i] = right[i - indexl];
}

// Returns how far the disk head moved for a series of requests using the FCFS algorithm.
int fcfs(int initpos, int requests[], int len)
{
	int i, total = 0, current = initpos;

	// Loops through each element in the array and adds the difference between the current one and where it is moving to.
	for (i = 0; i < len; i++)
	{
		total += abs(requests[i] - current);
		current = requests[i];
	}

	return total;
}

// Returns how far the disk head moved for a series of requests using the SSTF algorithm.
int sstf(int initpos, int requests[], int len)
{
	int i, total = 0, current = initpos, order[len];

	findSSTFOrder(requests, initpos, len, order);

	// Iterates through the properly ordered requests and adds the head movement to total.
	for (i = 0; i < len; i++)
	{
		total += abs(order[i] - current);
		current = order[i];
		
		printf("%d\n", total);
	}

	return total;
}

// Returns how far the disk head moved for a series of requests using the SCAN algorithm.
int scan(int initpos, int requests[], int len)	
{
	int i, total = 0, current = initpos, order[len + 1];

	findScanOrder(requests, initpos, len, order);

	// Iterates through the properly ordered requests and adds the head movement to total.
	for (i = 0; i < len + 1; i++)
	{	
		total += abs(order[i] - current);
		current = order[i];
	}

	return total;
}

// Returns how far the disk head moved for a series of requests using the C-SCAN algorithm.
int cscan(int initpos, int requests[], int len)
{
	int i, total = 0, current = initpos, order[len + 2];

	findCScanOrder(requests, initpos, len, order);

	// Loops through each element in the array and adds the difference between the current one and where it is moving to.
	for (i = 0; i < len + 2; i++)
	{
		total += abs(order[i] - current);
		current = order[i];
	}

	return total;
}

// Returns how far the disk head moved for a series of requests using the LOOK algorithm.
int look(int initpos, int requests[], int len)
{
	int i, total = 0, current = initpos, order[len];

	findLookOrder(requests, initpos, len, order);

	// Iterates through the properly ordered requests and adds the head movement to total.
	for (i = 0; i < len; i++)
	{
		total += abs(order[i] - current);
		current = order[i];
	}

	return total;
}

// Returns how far the disk head moved for a series of requests using the C-LOOK algorithm.
int clook(int initpos, int requests[], int len)
{
	int i, total = 0, current = initpos, order[len];

	findCLookOrder(requests, initpos, len, order);

	// Loops through each element in the array and adds the difference between the current one and where it is moving to.
	for (i = 0; i < len; i++)
	{
		total += abs(order[i] - current);
		current = order[i];
	}

	return total;
}

main(int argc, char* argv[])
{
	char s[LINELEN];
	int requests[BUFSIZE];
	int count;
	int initpos;

	if (argc < 2)
	{
		printf("Usage: ./a.out initpos\n");
		exit(1);
	}

	initpos = atoi(argv[1]);

	count = 0;
	while (fgets(s, LINELEN, stdin))
		requests[count++] = atoi(s);

	printf("FCFS: %d\n", fcfs(initpos, requests, count));
	printf("SSTF: %d\n", sstf(initpos, requests, count));
	printf("SCAN: %d\n", scan(initpos, requests, count));
	printf("C-SCAN: %d\n", cscan(initpos, requests, count));
	printf("LOOK: %d\n", look(initpos, requests, count));
	printf("C-LOOK: %d\n", clook(initpos, requests, count));

}
