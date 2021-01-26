##############################Programma svolto da################################
#####Vendrame Alessandro, Collarini Marco, Maraspin Lorenzo, Apostu Gabriel######
###Dopo una ricerca abbiamo deciso di utilizzare l'algoritmo di Trial Division###
#################################################################################
import time

def trial_division(n: int):
    a = []
    while n % 2 == 0:
        a.append(2)
        n /= 2
    f = 3
    while f * f <= n:
        if n % f == 0:
            a.append(f)
            n /= f
        else:
            f += 2
    if n != 1: a.append(n)
    # Solo i numeri dispari sono primi :D
    return a

def calcola_ripetizioni(v : []):
    ripetizioni = []

    for x in v:
        trovato = False
        for couple in ripetizioni:
            if couple[0] == x:
                couple[1] += 1
                trovato = True

        if not trovato:
            ripetizioni.append([x,1])
    return ripetizioni

def stampa_fattoriale(v:[]):
    fattoriale = ""

    for x in range(0,len(v)):
        if v.index(v[x]) != len(v)-1:
            if(v[x][1] > 1):
                fattoriale += str(v[x][0]) + "^" + str(v[x][1]) + " * "
            else:
                fattoriale += str(v[x][0]) + " * "
        else:
            if(v[x][1] > 1):
                fattoriale += str(v[x][0]) + "^" + str(v[x][1])
            else:
                fattoriale += str(v[x][0])

    return fattoriale

n = int(input("Inserisci un numero: "))

start_time = time.time()

v = calcola_ripetizioni(trial_division(n))

tempo_impiegato = round(time.time() - start_time, 5)

print(stampa_fattoriale(v))
print("Tempo di esecuzione: " + str(tempo_impiegato))