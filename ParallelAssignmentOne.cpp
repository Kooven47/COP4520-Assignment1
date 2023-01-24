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

	// Create a list of 1s and 0s to represent if a number is prime or not, initialized to 0 for prime
	// Size is 1 greater than limit to account for including limit in list
	std::vector<int> isPrimeList(LIMIT + 1);
	// Mark all odd numbers as prime
	for (int i = 1; i <= LIMIT; i += 2)
	{
		isPrimeList[i] = 1;
	}
	// Mark 2 as prime
	isPrimeList[2] = 1;

	// Thread-local storage for time taken by each thread
	std::vector<double> threadTimes(NUM_THREADS);

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
				// Use OpenMP's built in omp_get_wtime() to get time taken by each thread
				double startTime = omp_get_wtime();
				isPrimeList[j] = 0;
				double endTime = omp_get_wtime();
				// Add time taken by each thread to thread-local storage
				threadTimes[omp_get_thread_num()] += endTime - startTime;
			}
		}
    }

	long long sumOfPrimes = 0;
	int numPrimes = 0;
	int topTenPrimes[10];

	// Iterate backwards through list to start at greatest primes for top ten primes
	for (int i = LIMIT - 1; i >= 2; i--)
	{
		if (isPrimeList[i] == 1)
		{
			sumOfPrimes += i;
			numPrimes++;
			if (numPrimes <= 10)
			{
				topTenPrimes[numPrimes - 1] = i;
			}
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::ofstream myFile("primes.txt");
	myFile << duration.count() << "ms ";
	myFile << numPrimes << " ";
	myFile << sumOfPrimes << std::endl;
	// Iterate backwards through top ten primes to go from lowest to highest
	for (int i = 9; i >= 0; i--)
	{
		myFile << topTenPrimes[i] << " ";
	}
	myFile << std::endl;
	// Print out thread times
	for (int i = 0; i < NUM_THREADS; i++)
	{
		myFile << "Thread " << i << " took " << threadTimes[i] << " seconds." << std::endl;
	}
	myFile.close();	
}