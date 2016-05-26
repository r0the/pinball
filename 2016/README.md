# Ausgabemodul für Flipperautomaten 2016

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

## Ereignisse

| Name   | Beschreibung                                 |
| ------ | -------------------------------------------- |
| `init` | Der Mikrocontroller ist zurückgesetzt worden |
| `io-a` | Anschluss IO-A ist aktiviert worden          |
| `io-b` | Anschluss IO-B ist aktiviert worden          |
| `io-c` | Anschluss IO-C ist aktiviert worden          |
| `io-d` | Anschluss IO-D ist aktiviert worden          |
| `io-e` | Anschluss IO-E ist aktiviert worden          |
| `io-f` | Anschluss IO-F ist aktiviert worden          |
| `io-g` | Anschluss IO-G ist aktiviert worden          |
| `io-h` | Anschluss IO-H ist aktiviert worden          |
| `io-i` | Anschluss IO-I ist aktiviert worden          |
| `io-j` | Anschluss IO-J ist aktiviert worden          |
| `io-k` | Anschluss IO-K ist aktiviert worden          |
| `high` | Ein neuer Punkterekord ist erreicht worden   |
| `over` | Der letzte Ball ist verloren gegangen        |

## Konfiguration von Audioeffekten

### Dateiformat

Das Steuerungsmodul kann nur PCM-Audiodaten (*Puls-Code-Modulation*) abspielen. Die Dateien müssen
im WAV-Dateiformat (*Waveform Audio File Format*) mit einem Kanal (Mono), einer Bit-Tiefe von 8
und einer Abtastfrequenz von 16000 Hz vorliegen.

Mit dem frei verfügbaren Programm Audacity kann jede Audiodatei mit kleinem Aufwand in das geeignete
Format umgewandelt werden:

1. Audiodatei öffnen.
2. Menüpunkt Spuren / Stereosput in Mono umwandeln auswählen.
3. Projektfrequenz auf 16000 Hz stellen (unten links).
4. Exportieren als *Andere unkomprimierte Dateien* mit den folgenden Optionen:
    − Header: WAV (Microsoft)
    - Codec: Unsigned 8 bit PCM

### Dateiname

Der Dateiname muss mit dem Namen eines *Ereignisses* übereinstimmen. Die Dateiendung muss '.wav'
lauten. Der ganze Dateiname muss in Kleinbuchstaben geschrieben werden.

## Konfiguration der Ein- und Ausgängen

Die Anschlüsse *IO-A* bis *IO-K* können entweder als Eingang oder als Ausgang verwendet werden.
In der Datei `io.txt` auf der SD-Karte wird festgelegt, in welchem Modus jeder Anschluss betrieben
werden soll.

Die Textdatei enthält auf jeder Zeile den Namen eines Anschlusses in Kleinbuchstaben, gefolgt von
einem Leerzeichen und dem Buchstaben `i` (für *input*) oder `o` (für *output*):

```
io-a i
io-b i
io-c o
```

## Konfiguration von Aktionen

Aktionen werden in der Datei `actions.txt` auf der SD-Karte muss festgelegt.

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

**Konfiguration:** In der Konfigurationsdatei `config.txt` auf der SD-Karte muss festgelegt werden,
dass die Anschlüsse *IO-A*, *IO-B* und *IO-C* als Eingänge zu behandeln sind. Ausserdem wird die zu
zählende Punktzahl pro Eingang festgelegt:

```
io-a:in s+50
io-b:in s+100
io-c:in s+1000
```

Um den einen Audioeffekt abzuspielen, muss ausserdem die gewünschte Audiodatei in das korrekte
Format konvertiert und untern dem Namen `io-a.wav` auf der SD-Karte gespeichert werden.

### Spielfeld mit drei Targets

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

**Konfiguration:** In der Konfigurationsdatei `config.txt` auf der SD-Karte wird festgelegt, dass
der Anschluss *IO-A* als Eingang zu behandeln sind und dass bei dessen Aktivierung die Anzahl Kugeln
um eins zu reduzieren ist:

```
io-a:in z-1
```

### Zeitschaltung


### Steuerung eines LED-Streifens

tbd.

### Zurücksetzen mechanischer Zählräder
