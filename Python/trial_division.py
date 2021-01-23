import time
# Trial Division Algorithm 
  
# Function to check if a number is 
# a prime number or not  
def TrialDivision(N): 
  
    # Initializing with the value 2  
    # from where the number is checked 
    i = 2
  
    # Computing the square root of  
    # the number N 
    k = int(N ** 0.5) 
  
    # While loop till the  
    # square root of N 
    while(i<= k): 
  
        # If any of the numbers between  
        # [2, sqrt(N)] is a factor of N  
        # Then the number is composite 
        if(N % i == 0): 
            return 0
        i += 1
  
    # If none of the numbers is a factor, 
    # then it is a prime number 
    return 1

#Secondo metodo trial division
def trial_division(n: int):
    a = []
    while n % 2 == 0:
        a.append(2)
        n /= 2
        print(a)
    f = 3
    while f * f <= n:
        if n % f == 0:
            a.append(f)
            n /= f
            print(a)
        else:
            f += 2
    if n != 1: 
        a.append(n)
        print(a)
    # Only odd number is possible
    return a
# To check if a number is a prime or not 
def main():
    print("Inserisci un numero da 2 a 10.000.000.000")
    number = input()
    number = int(number)
    """
    p = TrialDivision(number)
    if(p): 
        print("Prime") 
    else: 
        print("Composite")
    """ 
    trial_division(number)

start_time = time.time()
main()
print("--- %s seconds ---" % (time.time() - start_time))