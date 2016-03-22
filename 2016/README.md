# Ausgabemodul für Flipperautomaten 2016

## Anschlüsse

| Nr. | Bezeichnung | Arduino |
| --- | ----------- | ------- |
|  1  | PWM         | D10     |
|  2  | IOA         | D2      |
|  3  | IOB         | D3      |
|  4  | IOC         | D4      |
|  5  | IOD         | D5      |
|  6  | IOE         | D6      |
|  7  | IOF         | A5      |
|  8  | IOG         | A4      |
|  9  | IOH         | A3      |
| 10  | IOI         | A2      |
| 11  | IOJ         | A1      |
| 12  | IOK         | A0      |
| 13  | VIN         | VIN     |
| 14  | GND         | GND     |
| 15  | 5V          | 5V      |
| 16  | RESET       | RESET   |

## Anwendungsbeispiele

### Spielfeld mit drei Targets

Auf einem Spielfeld sind drei Targets vorhanden. Das erste Target hat einen Wert von 50 Punkten, das
zweite 100 Punkte und für einen Treffer des dritten Targets erhält man 1000 Punkte. Bei einem
Treffer des dritten Targets soll ausserdem ein Audioeffekt ausgegeben werden.

**Verdrahtung:** Vom Masse-Ausgang des Ausgabemoduls wird ein Kabel zum einen Kontakt des ersten
Targets gezogen. Anschliessend wird von diesem Kontakt in Kabel weiter zum einen Kontakt des zweiten
und schliesslich zur dritten Target weitergezogen. Nun ist der eine Kontakt jedes Targets mit der
Masse des Ausgabemoduls verbunden. Nun wird der andere Kontakt des ersten Targets mit dem Eingang
IOA des Moduls verbunden, anschliessend der andere Kontakt des zweiten Targets mit dem Eingang IOB
und schliesslich der andere Kontakt des dritten Targets mit dem Eingang IOC.

[](example-target.png)

Wenn nun ein Target getroffen wird, so wird der Stromkreis geschlossen und am entsprechenden Eingang
liegt eine Spannung von 0 Volt an und das Ausgabemodul erkennt den Eingang als "aktiv".

**Konfiguration:**

In der Konfigurationsdatei `config.txt` auf der SD-Karte muss festgelegt werden, dass die Anschlüsse
IOA, IOB und IOC als Eingänge zu behandeln sind. Ausserdem wird die zu zählende Punktzahl pro
Eingang festgelegt:

```
IOA:in s+50
IOB:in s+100
IOC:in s+1000
```

Um den einen Audioeffekt abzuspielen, muss ausserdem die gewünschte Audiodatei in das korrekte
Format konvertiert und untern dem Namen `ioa.wav` auf der SD-Karte gespeichert werden.

### Ballverlust und Game Over

tbd.

### Steuerung eines LED-Streifens

tbd.

### Steuerung mechanischer Zählräder
