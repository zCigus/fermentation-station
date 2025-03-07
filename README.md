# Fermantation Station
## Descrizione
Il progetto si propone di realizzare una "Fermentation Station" ovvero una cella adibita alla fermentazione/lievitazione di prodotti da forno, che permetta di impostare un obiettivo per temperatura e umidità interna, da mantenere costanti attraverso un feedback loop gestito con la tecnica di controllo PID (proporzionale-integrale-derivativo). Il sistema è gestito da un Elegoo UNO R3, la misurazione di umidità e temperatura viene effettuata con un sensore DHT11. Per modificare l'umidità e la temperatura interna alla cella sono presenti un tappetino riscaldante corporeo controllato da un relay, ed un modulo Groove "Water Atomization" che permette di vaporizzare l'acqua attraverso ultrasuoni. Sono presenti inoltre un display LCD I2C, pulsanti fisici per il controllo e l'impostazione degli obiettivi ed una ventola per rendere uniforme la distribuzione di umidità e temperatura.

## Librerie Utilizzate
- [DHT sensor library](https://docs.arduino.cc/libraries/dht-sensor-library/)
- [LiquidCrystal I2C](https://docs.arduino.cc/libraries/liquidcrystal-i2c/)
- [Wire](https://docs.arduino.cc/language-reference/en/functions/communication/wire/)
- [PID](https://docs.arduino.cc/libraries/pid/)
