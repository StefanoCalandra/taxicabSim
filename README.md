#+title: Taxicab
[[file:20201205215419-project.org][#project]]
[[file:#universita.org][#universita]]

- [[file:/home/dan/Documents/UNI/II/SO/PROGETTO.pdf][Documento di Presentazione del Progetto]]

  + Opzioni di compilazione
    - gcc -std=c89 -pedantic

  + utilizzo di make

- [[file:home/dan/Documents/UNI/II/SO/taxicab-overview.pdf][Overview]]

* Struttura della Simulazione

La simulazione è strutturata in diversi processi. 

Master

- Si occupa della raccolta e stampa dei dati durante l’esecuzione del programma.
1.	Crea tre strutture, i quali si occuperanno di raccogliere tutti ciò che avviene durante la simulazione.
  + taxiData, che rappresenta i taxi nella mappa.
  + dataMessage, che rappresenta i messaggi creati.
  + simData, che rappresenta tutti i dati da stampare alla fine della compilazione. 
2.	Crea vari code per comunicare con i vari moduli in master.
3.	Verifica se la mappa iniziale sia impostato correttamente prima di iniziare la simulazione.
4.	Avvia il generator così da stampare continuamente gli stati di occupazione delle celle durante la simulazione, continuando fino al termine della simulazione.
5.	Al termine stamperà per un’ultima volta i seguenti dati finali sopracitati.
  + Numero di viaggi effettuati:
    - Con successo
    - Inevasi
    - Abortiti
  + La mappa dove saranno evidenziati:
    - Numero di punti sulla mappa di origine dei clienti (SO_SOURCE)
    - Punti inaccessibili hai taxi sulla mappa (SO_HOLES)
    - Il numero di celle maggiormente attraversate dai taxi
  + I processi taxi che hanno:
    - Attraversato più strada
    - Servito il maggior numero di richieste/clienti

void printMap(Cell (*map)[][SO_HEIGHT])
•	Stampa la mappa nel seguente formato leggibile:
1.	Celle FREE sono stampate come [ ]   
2.	Celle SOURCE sono stampate come [S] 
3.	Celle HOLE sono stampate come [#]

•	void handler(int sig)
   - Si occupa della interruzione di tutti i processi rimanenti al termine della simulazione.

•	void logmsg(char *message, enum Level l)
   - Si occupa della trasmissione dei messaggi fra tutte le funzioni all’interno del master durante la simulazione.

•	void updateData(long pid, taxiData *data)
   - Si occupa di costantemente aggiornare il master, così che le stampe della mappa siano sempre aggiornate mostrano tutti i nuovi eventi avvenuti in ogni istante.

•	void printReport()
   - Si occupa della stampa finale al termine della simulazione, mostrando le varie statistiche richieste dal progetto (Viaggi, Mappa e Taxi).

Generator

•	Si occupa della creazione della coda, utilizzando source e taxi per scambiarsi le destinazioni, alla predisposizione delle aree di memoria ed ipc, oltre ad occuparsi della creazione dei semafori, message queue, della mappa e l’array di punti che indica che sorgente sta in che punto.

1.	Genera la mappa, rappresentato da una griglia a quadretti, il quale avrà un tot numero di celle inaccessibili disposte casualmente ogni qualvolta si compila il processo, esso sarà composto da:
  + cell, i quadretti nella griglia, tramite una typedef struct, costituiti da variabili int capacity, traffic, visits ed enum type state (typedef struct Cell), i quali saranno raggruppati in tre tipologie:
     + Celle dove le richieste dei clienti partono (SO_SOURCE).
     + Celle inaccessibili hai taxi (SO_HOLE).
     + Celle vuote dove i taxi possono muoversi liberamente.
  + point, la posizione dei taxi all’interno della mappa, costituito dalle variabili int x e y, i quali rappresentano la linea orizzontale e linea verticale (typedef struct Point).
  + Stabilire la capacità minima e massima delle celle, così da evitare che non superino il limite prestabilito o che siano vuote.
     + SO_CAP_MIN <= capacity <= SO_CAP_MAX.
  + Verificare continuamente che i taxi non intasino le celle durante il processo, assicurandosi che sia sempre inferiore o uguale alla capacità della cella corrente.
     + traffic <= capacity.
  + n_visite - int
     + Il valore verrà sempre incrementato da ogni taxi di passaggio.

2.	Genera i processi figli (SO_TAXI), i taxi, che si muoveranno all’interno della mappa.
  + Esegue il programma taxi.c, il quale si occupa della creazione dei taxi.
     + Il punto di spawn di ogni taxi sarà sempre random, ogni volta che il programma verrà compilato, tranne che:
     + Alle celle inaccessibili hai taxi (SO_HOLE)
     + Alle celle che hanno ecceduto la loro capacità massima (SO_CAP_MAX)

3.	Predispone il Msg Queue, stabilendo così il tempo di durata massima che la simulazione ha a disposizione.
  + msg
     + Crea una destinazione (x,y)
  + Dimensione

4.	Genera le richieste taxi (SO_SOURCES), i quali rappresentano i clienti, ognuna legata ad una cella della mappa dove la richiesta veniva generata.

5.	Inserisce le richieste taxi, facendo così partire la simulazione.

6.	Una volta terminato il tempo di durata del processo (SO_DURATION), la simulazione e tutto ciò che sta svolgendo dovrà terminare all’istante (SIGINThandler).

•	Ma prima di terminare i processi si segnalerà al master che la simulazione è terminato (SIGINT), così che possa stampare le statistiche prima della terminazione.

•	void unblock(int sem)

•	void parseConf(Config *conf)
   - Analizza il process taxicab.conf all’interno della directory, per poi popolare il Config struct con i paramentri in taxicab.

•	int checkNoAdiacentHoles(Cell (*matrix)[][SO_HEIGHT], int x, int y)
   - Controlla le celle adiacenti nella matrice [x] [y] contrassegnati come HOLE, restituendo 0 se il controllo ha avuto successo.

•	void generateMap(Cell (*matrix)[][SO_HEIGHT], Config *conf)
   - Popola la matrice [x] [y] segnando lo stato delle celle se sono FREE, SOURCE oppure HOLE.

•	void printMap(Cell (*map)[][SO_HEIGHT])
   - Stampa la mappa nel seguente formato leggibile:
     1.	Celle FREE sono stampate come [ ]   
     2.	Celle SOURCE sono stampate come [S] 
     3.	Celle HOLE sono stampate come [#]

•	void logmsg(char *message, enum Level l)
   - Si occupa della trasmissione dei messaggi fra tutte le funzioni all’interno del generator durante la simulazione.

•	void execTaxi()
   - Invoca taxi.c per generare i taxi.

•	void execSource(int arg)
   - Invoca source.c per generare i punti d’origine dei clienti.

•	void handler(int sig)
   - Si occupa della interruzione di tutti i processi al termine della simulazione.

Taxi

•	Si occupa della generazione dei taxi e del loro comportamento seguente durante l’esecuzione della simulazione.
1.	Condivide la memoria.
2.	Apre una coda per trasmettere i messaggi al master.
3.	Semafori.
4.	Si sposta verso la sorgente, ovvero al punto d’origine del cliente, più vicino al taxi selezionato, incrementando il traffico della.
  + Questo in modo che i taxi non possano rubare il posto a vicenda.
  + Nel caso sia possibili, si riprova spostandosi verso un’altra cella con sorgente più vicina.
  
5.	Dopodiché il processo continuerà a ripetersi fino al termine della simulazione.

•	void moveTo(Point dest)
   - Si occupa dello spostamento dei taxi fra le varie celle della mappa durante la compilazione.

•	int canTransist(Point p)
   - Verifica se sia possibile per il taxi di spostarsi alla cella seguente.

•	void incTrafficAt(Point p)
   - Aumenta il traffico all’interno della cella.

•	void decTrafficAt(Point p)
   - Diminuisce il traffico all’interno della cella.

•	void logmsg(char *message, enum Level l)
   - Trasmette e sposta i messaggi fra le varie funzione durante la simulazione.

•	Point getNearSource(int *source_id)
   - Sposta verso il punto d’origine delle richieste più vicine.

•	void checkTimeout()
   - Verifica se il tempo a disposizione sia scaduto.

•	void handler(int sig)
   - Si occupa della interruzione di tutti i processi al termine della simulazione.

General

•	Si occupa di impostare tutte le strutture principali che gli altri processi avranno bisogno di usare, così che non sia necessario riscriverle tutte all’interno di ogni processo, ciò include:
1.	Cell, le celle che formano la mappa.
2.	Point, i punti dove un taxi si è fermato durante la compilazione.
3.	Message, i messaggi che vengono usati così che i processi possano comunicare a vicenda.
4.	Config, i parametri che i processi dovranno utilizzare.

Functions
•	Si occupa della generazione dei segnali dei semafori.

•	int isFree(Cell (*map)[][SO_HEIGHT], Point p)
   - Verifica se il Point sia libero.

•	void semWait(Point p, int sem)
   - Genera un segnale di waiting per i semafori.

•	void semSignal(Point p, int sem)
   - Generat un segnale di partenza per i semafori.

•	void semSync(int sem)
   - Crea un segnale di sincronizzazione per i semafori alla loro partenza.

Source

•	Si occupa della generazione delle sorgenti all’interno nella mappa.
1.	Apre una coda per trasmettere i messaggi far i vari taxi.
2.	Apre una coda per trasmettere i messaggi al master.
3.	Semafori
4.	Avvia il ciclo di esecuzione, il quale selezionerà le celle che diventeranno i punti d’origine durante la simulazione, e controllerà anche se le celle selezionate non siano già occupati da altri punti d’origine o dai taxi.

•	void logmsg(char *message, enum Level l)
   - Trasmette e sposta i messaggi all’interno del source durante la simulazione.

•	Void handler(int sig)
   - Si occupa della interruzione di tutti i processi al termine della simulazione.

Taxifunc

•	Si occupa della configurazione dei semafori all’interno della mappa.

+ int leggi(Point p)
   - Verifica se c’è uno scrittore all’interno del semaforo e dico quello che sto scrivendo.

+ int scrivi(Point p)
   - Verifica se c’è uno scrittore e se vi sono lettori, dopodiché blocco i prossimi scrittori e lettori.

+ int releaseW(Point p)
   - Invio la nuova scrittura hai semafori della mappa.

+ int releaseR(Point p)
   - Invio la nuova lettura hai semafori della mappa.

Configurazione

Il programma utilizza undici parametri letti a tempo di esecuzione, da file, da variabili di ambiente, oppure da stdin.

•	SO_TAXI
   - Numero di taxi presenti.

•	SO_SOURCE
   - Numero di punti sulla mappa di origine dei clienti (si immagini che siano le stazioni, gli aeroporti, etc.).

•	SO_HOLES
   - Numero di celle della mappa inaccessibili. Si ricordi: ogni cella inaccessibile non può averne altre nelle otto celle che la circondano.

•	SO_CAP_MIN
   - Capacità minima di ogni cella. Ogni cella ha capacità casuale.

•	SO_CAP_MAX
   - Capacità massima di ogni cella. Ogni cella ha capacità casuale.

•	SO_TIMENSEC_MIN
   - Tempo minimo necessario per l'attraversamento di ogni cella (in nanosecondi).

•	SO_TIMENSEC_MAX
   - Tempo massimo necessario per l'attraversamento di ogni cella (in nanosecondi).

•	SO_TIMEOUT
   - Tempo di inattività massiva di ogni taxi, dopo il quale il taxi chiude.

•	SO_DURATION
   - Durata della simulazione. Dopo questo tempo, il processo Generator invia SIGINT ai figli.

•	SO_WIDTH
   - Larghezza della mappa.

•	SO_HEIGHT
   - Altezza della mappa.
