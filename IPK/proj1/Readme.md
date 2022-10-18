# Jednoduchý server
Cieľom projektu bolo implementovať jedodnuchý server v jazyku c/c++, ktorý je schopný spracovávať malý počet GET požiadaviek. Server vie poskytnúť zákládné informácie o systéme, na ktorom je spustení.

## Spustenie aplikácie
Pomocou príkazu make v terminálovom okne sa vytvorí spustiteľný súbor hinfosvc. Tento súbor sa potom spúšťa pomocou príkazu **./hinfosvc** a očakáva jeden vstupný parameter (číslo), ktorý bude označovať lokálny port na ktorom má server načúvať.

## Použitie
Po spustení aplikácie je možné použiť napríklad nástroj **curl** alebo poslať požiadavku rovno pomocou preferovaného webového prehliadača.

### curl
```
    ./hinfosvc 12345 &
    curl http://localhost:12345/hostname
    curl http://localhost:12345/cpu-name
    curl http://localhost:12345/load
```
### webový prehliadač
```
    http://localhost:12345/hostname
    http://localhost:12345/cpu-name
    http://localhost:12345/load
```
## Autor
Krištof Šiška (xsiska16)