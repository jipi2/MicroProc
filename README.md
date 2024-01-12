# MicroProc

## Structura Proiect

### Folder __doc__
Acest folder conține documentația și cerințele proiectului.

### Folderul __img__
Acest folder conține imagini prezente în documentație.

### Folderul __GUI__
Acest folder conține aplicația python pentru reprezentarea graficului, recepționarea valorilor și transmiterea comenzilor.

### Folderul __src__
Acest folder conține aplicația dezvoltată în Keil.

## Setup

Pentru a rula aplicația este nevoie de: 
- Realizarea circuitului
- Conectarea plăcuței la pc
- Deschiderea aplicației dezvoltată în Keil
- Buil
- Load
- Modificarea portului __COMx__ în funcție de portul pe care se regăsește plăcuța conectată (în fișierul __mcn.py__ se face modificarea)
- Instalarea pachetelor în python: ```pip install -r requirements.txt```
- Rularea aplicației: ```python ./main.py```

