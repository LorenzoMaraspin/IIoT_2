import time
"""					
2)data la seguente equazione 
4x + 7y = n
trovare l'insieme di coppie x e y che danno come soluzione n numero intero
"""
solutions = [] 
def executeEquation():
	#print ("for equation of form '4x + 7y = n") 
	a = int(4)
	b = int(7)
	
	#print ("The Equation : %sx + %sy = %s"% (a,b,n)) 
	
	div = int(n/b)
	for y in range(0,div + 1): 
		x = (n - b*y)/float(a) 
		if(x.is_integer()): 
			solutions.append((int(x),y)) 
def main():
	start_time = time.time()
	executeEquation()
	tempo_impiegato = round(time.time() - start_time, 5)
	print("Tempo di esecuzione: " + str(tempo_impiegato))


n = int(input("Enter n: "))
main()
print(solutions)