# COP4520 Assignment 1

This project is a C++ implementation of a parallel primality tester using OpenMP.
It tests numbers from 1 to 10^8 and uses 8 threads.

## Problem 1 (100 points) 
Your non-technical manager assigns you the task to find all primes between 1 and 
108.  The assumption is that your company is going to use a parallel machine that 
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
g++ -o ParallelAssignmentOne -std=c++2a -fopenmp ParallelAssignmentOne.cpp
<br> ./ParallelAssignmentOne

## Approach and Experimental Evaluation
My current approach involves using the Sieve of Eratosthenes, but optimized for only considering odd numbers.
I fill an array with size 10^8 with 0s, with each index representing the primality of that number, marking them as not prime.
I then mark every odd number, as well as 2, with 1 (prime).
I loop from 3, incrementing by 2 to only consider odd numbers, up to the square root of 10^8. In that loop, if the current number is marked as prime, I mark all of its multiples as nonprime, going from that number squared up to 10^8, incrementing by that number times 2 every time. This inner loop is what I focused on parallelizing, utilizing all 8 threads equally. 
<br> The execution time for this method is about 1.2 seconds on my computer

Previously, I had the same approach but checking all numbers rather than just odds.
<br> The execution time for this method was about 1.4 seconds on my computer.

This was the first time where I searced for better primality testing and came across the Sieve of Eratosthenes and decided to implement it. Before, I would always execute the inner loop and then check with an if statement for the inner number being prime. This led to a lot of unnecessary work/looping.
<br> The execution time for this method was about 3.0 seconds on my computer.

The first approach I had was the classic looping until sqrt(n) for every integer to check if it was prime. This was EXTREMELY slow, especially since it considered even numbers and other numbers that are "obviously" prime.
<br> The execution time for this method was about 19 minutes on my computer.

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

<br> My proof of equal utilization can be found in the Thread-Timing branch, where I use OpenMP's built in functions to time and get the current thread being used to store the total time taken from each thread in thread-local storage. Here is a picture from one of the runs showing how close the execution times are from each thread: (In this example, the difference between the lowest execution time and highest execution time is only 1.57%).
![image](https://user-images.githubusercontent.com/74631846/214456841-3d38f1bb-b713-4574-91a2-27f4f7316e89.png)
