# COP4520 Assignment 1

This project is a C++ implementation of a parallel primality tester using OpenMP.
It tests numbers from 1 to 10^8 and uses 8 threads.

## Problem 1 (100 points) 
Your non-technical manager assigns you the task to find all primes between 1 and 
10^8.  The assumption is that your company is going to use a parallel machine that 
supports eight concurrent threads. Thus, in your design you should plan to spawn 8 
threads that will perform the necessary computation. Your boss does not have a strong 
technical background but she is a reasonable person. Therefore, she expects to see that 
the work is distributed such that the computational execution time is approximately 
equivalent among the threads. Finally, you need to provide a brief summary of your 
approach and an informal statement reasoning about the correctness and efficiency of 
your design. Provide a summary of the experimental evaluation of your approach. 
Remember, that your company cannot afford a supercomputer and rents a machine by 
the minute, so the longer your program takes, the more it costs. Feel free to use any 
programming language of your choice that supports multi-threading as long as you 
provide a ReadMe file with instructions for your manager explaining how to compile and 
run your program from the command prompt.

## Output
Please print the following output to a file named primes.txt: 
<br> \<execution time>  \<total number of primes found>  \<sum of all primes found> 
<br> \<top ten maximum primes, listed in order from lowest to highest>

## Notes on Output: 
1. Zero and one are neither prime nor composite, so please don't include them in 
the total number of primes found and the sum of all primes found. 
2. The execution time should start prior to spawning the threads and end after all 
threads complete.

## Compilation Instructions
g++ -o ParallelAssignmentOne -fopenmp ParallelAssignmentOne.cpp
<br> ./ParallelAssignmentOne

## Approach and Experimental Evaluation
The first approach I had was the [primality test on Wikipedia](https://en.wikipedia.org/wiki/Primality_test#C,_C++,_C#_&_D) for every integer to check if it was prime. This was EXTREMELY slow, especially since it considered even numbers and other numbers that are "obviously" prime. I was using 8 threads for the for loop of the isPrime function I implemented, if it was reached.
<br> The execution time for this method was about 19 minutes on my computer.

Then, I searched for better primality testing and came across the Sieve of Eratosthenes and decided to implement it. I filled a bool vector with size 10^8 with trues, with each index representing the primality of that number, marking them as prime. I loop from 2 to the square root of 10^8. In that loop, if the current number is marked as prime, I mark all of its multiples as nonprime, going from that number squared up to 10^8, incrementing by that number every time. This inner loop is what I focused on parallelizing, utilizing all 8 threads equally.
<br> The execution time for this method was about 2.6 seconds on my computer.

Next, I switched from a bool vector to store everything to an int vector. I thought the bool would be faster originally due to less memory usage, but I found it was slower and not thread-safe.
<br> The execution time for this method was about 1.7 seconds on my computer.

Afterwards, I decided to only consider odd numbers. I initially mark 2 and all odd numbers except for 1 as prime. I loop from 3 to the square root of 10^8, incrementing by 2 each time.
<br> The execution time for this method was about 1.2 seconds on my computer.

Then, I decided to implement multithreading for the initialization of the odd numbers as prime, which was only a slight improvement.
<br> The execution time for this method was about 1.1 seconds on my computer.

Finally, I also implemented multithreading for the final iterating, which sums up everything and keeps track of the number of primes found. I made sure to use a reduction clause for this due to the shared data, and that kept me safe from race conditions. I did have to move my top ten prime counter outside of the multithreaded for loop to completely avoid race conditions, however.
<br> The execution time for this method is about 0.8 seconds on my computer.

## Proof of Correctness and Equal Utilization
The Sieve of Eratosthenes is a well-known and proven method for finding prime numbers. My odd only variation is trivially correct because every even number is divisible by 2, and therefore not prime (except for 2).
<br> I will still give a simple proof of its correctness:
<br> The fundamental theorem of arithmetic states that every integer greater than 1 is either a prime number or can be written as a product of its prime factors.
<br> If any integer n is composite, then n has a prime divisor less than or equal to sqrt(n).
- If n is composite, then it has a positive integer factor a with 1 < a < n by definition. 
- This means that n = ab, where b is an integer greater than 1.
- Assume a > sqrt(n) and b > sqrt(n). 
- Then ab > sqrt(n) * sqrt(n) = n, which is a contradiction. 
- So either a <= sqrt(n) or b <= sqrt(n).
- Thus, n has a divisor less than sqrt(n).

Since for any number n in the list, we are looking all prime numbers up to sqrt(n), we are indeed separating all composite numbers. Hence, Sieve of Eratosthenes generates all primes numbers less than the upper limit.

<br> My proof of equal utilization can be found in the Thread-Timing branch, where I use OpenMP's built in functions to time and get the current thread being used to store the total time taken from each thread in thread-local storage. Here is a picture from one of the runs showing how close the execution times are from each thread: (Note that the overall execution time is greatly increased due to the constant timing for each thread. The key point is that the difference between the lowest execution time and highest execution time is only 0.68% in this example, highlighting how equally spread out the work is between the 8 threads).
![image](https://user-images.githubusercontent.com/74631846/215229890-5dcad054-c40b-4c04-a44d-5ec094f8054f.png)
