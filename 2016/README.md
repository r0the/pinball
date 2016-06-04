# Mikrocontroller-Modul für Flipperautomaten 2016

## Anschlüsse

| Nr. | Bezeichnung | Arduino |
| --- | ----------- | ------- |
|  1  | PWM         | D10     |
|  2  | IO-A        | D2      |
|  3  | IO-B        | D3      |
|  4  | IO-C        | D4      |
|  5  | IO-D        | D5      |
|  6  | IO-E        | D6      |
|  7  | IO-F        | A5      |
|  8  | IO-G        | A4      |
|  9  | IO-H        | A3      |
| 10  | IO-I        | A2      |
| 11  | IO-J        | A1      |
| 12  | IO-K        | A0      |
| 13  | VIN         | VIN     |
| 14  | GND         | GND     |
| 15  | 5V          | 5V      |
| 16  | RESET       | RESET   |

## Preis

| Komponente            | Einzelpreis | Anzahl | Total |
| --------------------- | ----------- | ------ | ----- |
| Siebensegmentanzeige  |  2.90       | 5      | 14.50 |
| Schieberegister       |  0.50       | 5      |  2.50 |
| Klinkensteckverbinder |  1.25       | 1      |  1.25 |
| IC-Fassung 16 Pol     |  0.30       | 6      |  1.80 |
| Puffer-Treiber        |  0.40       | 1      |  0.40 |
| SD-Kartensockel       |  3.00       | 1      |  3.00 |
| SD-Karte              |  8.95       | 1      |  8.95 |
| Arduino               | 20.00       | 1      | 20.00 |
| Platine               |  5.00       | 1      |  5.00 |
| USB-Ladegerät         |  9.95       | 1      |  9.95 |
| Lautsprecherboxen     | 13.95       | 1      | 13.95 |
| Verbrauchsmaterial    |  2.60       |        |  2.60 |
| **Total**             |             |        | 81.30 |

## Komponenten

- Siebensegmentanzeige: [Kingbright SC10-21HWA](http://www.conrad.ch/ce/de/product/160040/)
- Klinkensteckverbinder 3.5mm: [BKL Electronic 1109050](http://www.conrad.ch/ce/de/product/730294/)
- Schieberegister: [Texas Instruments SN74HC595N PDIP-16](http://www.conrad.ch/ce/de/product/1016774/)
- Schraubklemmblock: [AKZ692/8-2.54-V-GRÜN](http://www.conrad.ch/ce/de/product/567844/)
- SD-Karte: [2 GB Transcend](https://www.conrad.ch/ce/de/product/992826/)
- SD-Kartensockel: [Attend 104H-TDA0-R01 1](http://www.conrad.ch/ce/de/product/1308325/)
- Puffer-Treiber: [Texas Instruments CD74HC4050E PDIP-16](https://www.conrad.ch/ce/de/product/164895/)
- USB-Ladegerät: [Goobay 42663](http://www.conrad.ch/ce/de/product/393438/)
- Lautsprecherboxen: [Logitech Z120 2.0](http://www.conrad.ch/ce/de/product/917126/)

## Ereignisse

| Name | Beschreibung                                                    |
| ---- | --------------------------------------------------------------- |
| `on` | Der Mikrocontroller ist eingeschaltet oder zurückgesetzt worden |
| `ia` | Anschluss IO-A ist aktiviert worden                             |
| `ib` | Anschluss IO-B ist aktiviert worden                             |
| `ic` | Anschluss IO-C ist aktiviert worden                             |
| `id` | Anschluss IO-D ist aktiviert worden                             |
| `ie` | Anschluss IO-E ist aktiviert worden                             |
| `if` | Anschluss IO-F ist aktiviert worden                             |
| `ig` | Anschluss IO-G ist aktiviert worden                             |
| `ih` | Anschluss IO-H ist aktiviert worden                             |
| `ii` | Anschluss IO-I ist aktiviert worden                             |
| `ij` | Anschluss IO-J ist aktiviert worden                             |
| `ik` | Anschluss IO-K ist aktiviert worden                             |
| `hi` | Ein neuer Punkterekord ist erreicht worden                      |
| `go` | Der letzte Ball ist verloren gegangen                           |
| `c1` | Countdown 1 hat bis Null gezählt                                |
| `c2` | Countdown 2 hat bis Null gezählt                                |
| `c3` | Countdown 3 hat bis Null gezählt                                |
| `c4` | Countdown 4 hat bis Null gezählt                                |

## Konfiguration von Audioeffekten

### Dateiformat

Das Steuerungsmodul kann nur PCM-Audiodaten (*Puls-Code-Modulation*) abspielen. Die Dateien müssen
im WAV-Dateiformat (*Waveform Audio File Format*) mit einem Kanal (Mono), einer Bit-Tiefe von 8
und einer Abtastfrequenz von 16000 Hz vorliegen.

Mit dem frei verfügbaren Programm Audacity kann jede Audiodatei mit kleinem Aufwand in das geeignete
Format umgewandelt werden:

1. Audiodatei öffnen.
2. Spur auswählen.
3. Menüpunkt Spuren / Stereosput in Mono umwandeln auswählen.
4. Projektfrequenz auf 16000 Hz stellen (unten links).
5. Exportieren als *Andere unkomprimierte Dateien* mit den folgenden Optionen:

    - Header: WAV (Microsoft)
    - Codec: Unsigned 8 bit PCM

### Dateiname

Der Dateiname muss mit dem Namen eines *Ereignisses* übereinstimmen. Die Dateiendung muss '.wav'
lauten. Der ganze Dateiname muss in Kleinbuchstaben geschrieben werden.

## Konfiguration der Anschlüsse

Die Anschlüsse *IO-A* bis *IO-K* können entweder als Eingang oder als Ausgang verwendet werden.
In der Datei `io.txt` auf der SD-Karte wird festgelegt, in welchem Modus jeder Anschluss betrieben
werden soll.

Die Textdatei enthält auf jeder Zeile den Namen eines Anschlusses in Kleinbuchstaben, gefolgt von
einem Leerzeichen und dem Buchstaben `i` (für *input*) oder `o` (für *output*):

```
ia i
ib i
ic o
```

## Konfiguration von Aktionen

Aktionen werden in der Datei `action.txt` auf der SD-Karte definiert.

## Anwendungsbeispiele

### Spielfeld mit drei Targets

Auf einem Spielfeld sind drei Targets vorhanden. Das erste Target hat einen Wert von 50 Punkten,
das zweite 100 Punkte und für einen Treffer des dritten Targets erhält man 1000 Punkte. Bei einem
Treffer des dritten Targets soll ausserdem ein Audioeffekt ausgegeben werden.

**Verdrahtung:** Vom Masse-Ausgang des Ausgabemoduls (*GND*) wird ein Kabel zum einen Kontakt des
ersten Targets gezogen. Anschliessend wird von diesem Kontakt in Kabel weiter zum einen Kontakt des
zweiten und schliesslich zur dritten Target weitergezogen. Nun ist der eine Kontakt jedes Targets
mit der Masse des Ausgabemoduls verbunden. Nun wird der andere Kontakt des ersten Targets mit dem
Eingang *IO-A* des Moduls verbunden, anschliessend der andere Kontakt des zweiten Targets mit dem
Eingang *IO-B* und schliesslich der andere Kontakt des dritten Targets mit dem Eingang *IO-C*.

![](example-target.png)

Wenn nun ein Target getroffen wird, so wird der Stromkreis geschlossen und am entsprechenden
Eingang liegt eine Spannung von 0 Volt an und das Ausgabemodul erkennt den Eingang als "aktiv".

**Konfiguration:** In der Konfigurationsdatei `action.txt` auf der SD-Karte muss festgelegt werden,
dass die Anschlüsse *IO-A*, *IO-B* und *IO-C* als Eingänge zu behandeln sind. Ausserdem wird die zu
zählende Punktzahl pro Eingang festgelegt:

```
ia s+50
ib s+100
ic s+1000
```

Um den einen Audioeffekt abzuspielen, muss ausserdem die gewünschte Audiodatei in das korrekte
Format konvertiert und untern dem Namen `ia.wav` auf der SD-Karte gespeichert werden.

### Kugelverlust und Game Over

Bei einem Flipperautomaten soll maximal mit drei Kugeln gespielt werden können. Anschliessend muss
ein neues Spiel gestartet werden.

Damit ein Kugelverlust festgestellt werden kann, muss ein Kontakt angebracht werden, der durch
jeden möglichen Kugelverlust ausgelöst wird. Um ein neue Spiel zu starten, muss zusätzlich ein
Taster am Automaten angebracht werden.

**Verdrahtung:** Der eine Kontakt des Tasters und der Ballverlust-Kontakt wird mit der Masse des
Ausgabemodus verbunden. Der andere Kontakt des Ballverlust-Kontakts wird mit einem Eingang, z.B.
*IO-A* verbunden. Der andere Kontakt des Tasters wird mit dem *RESET*-Eingang des Moduls verbunden.

![](example-game-over.png)

**Konfiguration:** In der Konfigurationsdatei `action.txt` auf der SD-Karte wird festgelegt, dass
der Anschluss *IO-A* als Eingang zu behandeln sind und dass bei dessen Aktivierung die Anzahl Kugeln
um eins zu reduzieren ist:

```
ia z-1
```

### Zeitschaltung


### Steuerung eines LED-Streifens

tbd.

### Zurücksetzen mechanischer Zählräder
