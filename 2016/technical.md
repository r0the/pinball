# Technische Hintergründe

## Audioausgabe

### Puls-Code-Modulation

Um Schall digital zu speichern, muss ein analoges elektrisches Signal (welches vom Mikrofon
aus den Schallwellen erzeugt wird) digitalisiert werden, d.h. in eine Folge von Bits (oder Bytes)
umgewandelt werden.

Die Puls-Code-Modulation (PCM) macht genau das. Mit einem konstanten zeitlichen
Abstand wird die Amplitude des Signals gemessen und durch eine Zahl in einem
bestimmten Bereich codiert. Dieser Zahlenbereich wird in Bit angegeben. Je mehr Bit
pro Abtastung (*Sample*) zu Verfügung stehen, desto besser ist die Qualität der
digitalisierten Daten.

In folgendem Beispiel wird eine Sinuswelle durch die Zahlen 0 bis 15, also mit 4 Bit codiert:

![](images/pcm.png)

Das codierte Signal sieht also folgendermassen aus:

8, 9, 11, 13, 14, 15, 15, 15, 14, 13, 12, 10, 8, 7, 5, 3, 2, 1, 0, 0, 0, 1, 2, 4, 6, 7

Damit die Zahlenfolge wieder in ein Audiosignal umgewandelt werden kann, müssen auch
die Anzahl Bit pro Abtastung sowie die *Abtastrate* bekannt sein.

#### Abtastrate

Die Frequenz, mit welcher das Signal gemessen wird, heisst *Abtastrate*. Sie
muss gemäss dem [Nyquist-Shannon-Abtasttheorem](https://de.wikipedia.org/wiki/Nyquist-Shannon-Abtasttheorem) mindestens doppelt so gross wie die grösste zu codierende Frequenz sein, damit kein Qualitätsverlust auftritt.

Da die Musikwahrnehmbarkeit beim Menschen bis ca. 8 kHz reicht, beträgt die minimale Abstastrate 16 kHz:

![](images/hoerflaeche.png)

#### Abwägungen

Eine hohe Anzahl Bit pro Abtastung und eine hohe Abtastrate führen zu einer besseren
Qualität, aber auch zu einer viel grösseren Datenmenge.

Heute werden Audiodaten mit Hilfe von komplexen Algorithmen (z.B. MP3) komprimiert, um
die Datenmenge zu reduzieren. Diese Algorithmen benötigen aber eine grosse
Rechenleistung, die auf dem Arduino nicht zu Verfügung steht.


### Pulsweitenmodulation

Tbd.
