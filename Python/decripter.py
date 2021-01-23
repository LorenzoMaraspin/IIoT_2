# DATA LA STRINGA SCRITTA ALLA LAVAGNA, SCRIVERE UN CODICE
# CHE DECIFRI LA STRINGA E LA STAMPI A VIDEO

#-----------------------------------------------------------------------------------------------
stringa_codificata = "VCZJCZIZJMZJLXYHMJYHEMCWSJM\\WZNCXWJSYQXWZCOXJHMCJDVSOJ\\HYHWWZOH@"
Alfabeto_criptato =   ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","[","\\","@"]
Alfabeto_latino =     ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","[","\\","."]
Alfabeto_decriptato = []
stringa_decodificata = []
frase = ""
#variabile che contiene tutte le lettere tranne i caratteri J \
tot_lettere_senza_spazio = 55
tot_lettere = 64

v = []
statistica_lettere = {}
#lista che contiene ogni singola parola del testo cifrato
WORDS = []

#-----------------------------------------------------------------------------------------------
def setAlfabetoDecifrato():
    for i in Alfabeto_criptato:
        posizione = Alfabeto_criptato.index(i) + 1
        calcolo = (posizione * 19) % 29
        value = Alfabeto_latino[calcolo - 1]
        if(value == "J"):
            Alfabeto_decriptato.append(" ")
        else:
            Alfabeto_decriptato.append(value)
#-----------------------------------------------------------------------------------------------
def comparaTraAlfabeti():
    for i in stringa_codificata:
        if(i == "J"):
            stringa_decodificata.append(" ")
        for x in Alfabeto_decriptato:
            if(i == x):
                pos = Alfabeto_decriptato.index(x)
                var = Alfabeto_criptato[pos]
                stringa_decodificata.append(var)
                
#SEGNARSI QUANTE SONO LE LETTERE, CONFRONTI LA FREQUENZA DELLE LETTERE CON LA FREQUENZA DELLE LETTERE LATINE
#---------------------------------CONTA LETTERE-------------------------------------------------
def contaRipetizioneLettere():
    for letter in stringa_codificata:
        trovato = False
        for x in v:
            if x[1] == letter:
                x[0] += 1
                trovato = True

        if trovato == False:
            v.append([1,letter])
    v.sort()
#-----------------------------------------------------------------------------------------------
def dividiParola(frase):                                        #Funzione che si occupa di dividere la frase in parole tramite dei separatori
    parola = []
    for x in frase:
        if x == "J" or x == "@":
            WORDS.append(parola)
            parola = []
        else:
            parola.append(x)
#-------------------------CALCOLA STATISTICA RICORRENZA LETTERE---------------------------------
def getStatistica():
    for i in v:
        calc = ((i[0]* 100) / tot_lettere)                      #calcolo la percentuale di frequenza di ogni singola lettera nel testo
        toConvert = f"{calc:.2f}"                               #imposto il troncamento del float ricevuto alla seconda cifra decimale
        percent = float(toConvert)                              #eseguo il troncamento
        key = "Lettera " + i[1]                                 #setto la key del dizionario in base alla lettere
        statistica_lettere[key] = str(percent) + "%"            #assegno la percentuale delle singole lettere
#-----------------------------------------------------------------------------------------------
contaRipetizioneLettere()
dividiParola(stringa_codificata)
getStatistica()
setAlfabetoDecifrato()
comparaTraAlfabeti()
#-----------------------------------------------------------------------------------------------

print("DECIFRARE IL SEGUENTE MESSAGGIO\n\n        " + stringa_codificata + "\n")
print("Numero totale di lettere nel messaggio -> " + str(tot_lettere) + "\n")
print("Numero totale di lettere nel messaggio senza gli spazi -> " + str(tot_lettere_senza_spazio) + "\n")
print("PAROLE CODIFICATE")
for i in WORDS:
    print(i)
print()
print("Statistica della frequenza di ripetizione di ogni singola lettera")
for i in statistica_lettere:
    print(i,statistica_lettere[i])
print()
print("La codifica utilizzata per la decifratura è questa")
print(Alfabeto_decriptato)
for i in stringa_decodificata:
    frase += i
print("\n\n        " + frase)
