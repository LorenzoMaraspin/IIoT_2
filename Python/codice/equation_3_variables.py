import time
"""
1)data l'equazione 
(x/(y+z) + (y/(x+z)) + (z/(x+y))) = 4
trovare il valore delle incognite x,y,z [numeri interi]
"""
#METODO CHE RISOLVE LA SEGUENTE EQUAZIONE IN MODALITà BRUTE FORCE
#from fractions import Fraction as Frac
from math import gcd
n = 100
for x in range(-n, n + 1):														#CICLO PER CREARE DINAMICAMENTE DEI VALORI DI X DA -100 A 100
    for y in range(-n, n + 1):													#CICLO PER CREARE DINAMICAMENTE DEI VALORI DI Y DA -100 A 100
        for z in range(0, n + 1):												#CICLO PER CREARE DINAMICAMENTE DEI VALORI DI z DA -100 A 100
            if gcd(x, gcd(y, z)) == 1:											#CONTROLLO SE IL MCD DI (X, Y, Z) è UGUALE A 1 ALLORA PROVA A SOSTITUIRE I VALORE NELLA FUNZIONE
                if x**3 + y**3 + z**3 - 3 * x**2 * (y + z) - 3 * y**2 * (		#CHE è STATA DIMINUITA FINO A MINIMI TERMINI
                        z + x) - 3 * z**2 * (x + y) - 5 * x * y * z == 0:		#STAMPA TUTTE LE POSSIBILI COMBINAZIONI DI X, Y, Z CHE DANNO COME RISULTATO DELL'EQUAZIONE 4
                    print((x, y, z))





