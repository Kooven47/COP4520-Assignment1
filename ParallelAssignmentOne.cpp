// Kevin Alfonso
// COP4520

#include <iostream>
#include <omp.h>
#include <chrono>
#include <vector>
#include <fstream>

// Limited to 8 threads
#define NUM_THREADS 8
// Limit of 10^8
#define LIMIT 100000000

int main(int argc, char *argv[])
{
	// Start timer
	auto start = std::chrono::high_resolution_clock::now();

	// Create a list of 1s and 0s to represent if a number is prime or not, initialized to 0 for not prime
	// Size is 1 greater than limit to account for including limit in list
	std::vector<int> isPrimeList(LIMIT + 1);
	
	// Mark all odd numbers (except for 1) as prime to start off with using 8 threads
	#pragma omp parallel for num_threads(NUM_THREADS)
	for (int i = 3; i <= LIMIT; i += 2)
	{
		isPrimeList[i] = 1;
	}
	// Mark 2 as prime
	isPrimeList[2] = 1;

	// Only go up to sqrt of limit
	// Only check odds since even numbers are not prime
	for (int i = 3; i * i <= LIMIT; i += 2)
	{
		// Use parallel for loop to check if each number is prime with necessary 8 threads
		// Mark each multiple as non-prime
		if (isPrimeList[i] == 1)
		{
			#pragma omp parallel for num_threads(NUM_THREADS)
			for (int j = i * i; j <= LIMIT; j += i * 2)
			{
				isPrimeList[j] = 0;
			}
		}
	}

	long long sumOfPrimes = 0;
	int numPrimes = 0;
	int topTenPrimes[10];

	// Reduction clause due to shared data, don't want race conditions
	#pragma omp parallel for num_threads(NUM_THREADS) reduction(+:sumOfPrimes, numPrimes)
	for (int i = 2; i <= LIMIT; i++)
	{
		if (isPrimeList[i] == 1)
		{
			sumOfPrimes += i;
			numPrimes++;
		}
	}

	// Get top ten elements from the array, which is already sorted
	// Had to keep separate from previous loop since it was not possible to use reduction clause
	// Occurs very fast, so no need for multithreading
	// Iterate backwards through list to start at greatest primes for top ten primes
	for (int i = LIMIT, n = 10; i >= 2 && n > 0; i--)
	{
		if (isPrimeList[i] == 1)
		{
			topTenPrimes[n - 1] = i;
			n--;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::ofstream myFile("primes.txt");

	myFile << duration.count() << "ms ";
	myFile << numPrimes << " ";
	myFile << sumOfPrimes << std::endl;
	for (int i = 0; i < 10; i++)
	{
		myFile << topTenPrimes[i] << " ";
	}
	
	myFile.close();

	return 0;	
}
