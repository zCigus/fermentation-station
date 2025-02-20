# Fermantation Station
## Descrizione
Il progetto si propone di realizzare una "Fermentation Station" ovvero una cella adibita alla fermentazione/lievitazione che permetta di impostare un obiettivo per temperatura e umidità interna da mantenere costanti attraverso un feedback loop. Il sistema sarà gestito da un Elegoo UNO R3, la misurazione di umidità e temperatura viene effettuata con l'utilizzo di un sensore DHT11, sono presenti un display e pulsanti fisici per il controllo e l'impostazione degli obiettivi di temperatura e umidità. Il feedback loop inoltre viene gestito con una tecnica di controllo PID (proporzionale-integrale-derivativo).
Il PID è un meccanismo che permette di controllare con precisione un sistema fisico cercando di mantenere una variabile (detta "process variable") il più vicino possibile ad un valore desiderato (setpoint).

## Librerie utilizzate
- DHT sensor library
- Adafruit Unified Sensor (necessaria per DHT sensor library)
