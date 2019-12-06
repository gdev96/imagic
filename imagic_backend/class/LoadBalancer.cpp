#include "LoadBalancer.h"

//1.Preleva il messaggio dalla coda
//2.Legge l'header e ne controlla il tipo
//3.Se necessario calcola il server a cui inviare la richiesta
//4.Copia ordinatamente l'header prelevato e il payload nel buffer del/i connettore/i [1,4,4,p_l] big endian

