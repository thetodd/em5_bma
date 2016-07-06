# Emergency 5 BMA Plugin
Das Plugin fügt verschiedene neue Elemente zum Thema Brandmeldeanlage (BMA) zu [Emergency 5](http://emergency5.de/) hinzu.

### Neuer Einsatz
Ein neuer Einsatz wurde erstellt. Der Einsatz kann im Eventpool über den Namen `flo11:BMAFireEventFactory` hinzugefügt werden.
Eine Beispiel Event-Definition ist im Repository beigelegt und kann in die eigene Modifikation kopiert werden.

Die EventFactory sucht auf der Spielkarte nach der BMAComponent. Aus allen BMAComponent-Entities wird eines per Zufall ausgesucht
und ein Event darauf ausgelöst. Das Event löst dann auch im entsprechenden TargetEntity einen Brand aus und setzt die BMA in den
"Ausgelöst"-Zustand.

### Neue Commands
Natürlich will eine Brandmeldeanlage auch zurückgesetzt werden. Dazu wurde ein neuer Command `flo11::ResetBMACommand` eingeführt.
Dieses Command kann jeder Person gegeben werden. Im Repository liegt ein Beispiel-Prefab für einen Gruppenführer, der das neue Command
enthält. Das Command ist erst verfügbar, wenn die BMA "ausgelöst" hat und die Brände im TargetEntity gelöscht sind.

### Neue Komponente
Die `flo11:BMAComponent` kennzeichnet Entities, die durch das `BMAFireEvent` ausgelöst werden können. Das Repository enthält ein Prefab,
das eine BMA darstellen soll. Das Prefab enthält bereits die BMA-Komponente. Wird das BMA-Prefab auf die Karte gezogen muss die Komponente
noch bearbeitet werden. Die Einstellungen `AlarmLights` und `TargetEntity` müssen noch angepasst werden.

#### AlarmLights
In die Einstellung `AlarmLights` kommt die EntityId eines `Lights`, das zwingend eine `LightAnimationComponent` haben muss.
Die LightAnimation sollte standardmäßig deaktiviert werden, die BMA-Komponente aktiviert sie automatisch.

#### TargetEntity
In die Einstellung `TargetEntity` kommt die EntityId des Objektes, dass durch das Event einen Brand erhalten soll. Damit ein Brand
ausgelöst werden kann muss das Objekt eine em5::FireComponent oder eine em5::ComplexFireComponent enthalten. Die meisten Gebäude aus
dem Originalspiel besitzen die notwendigen Komponenten bereits.

#### CanTrigger
Diese Einstellung deaktiviert die BMA, sodass sie kein Event auslösen kann. Ist das Häkchen gesetzt, kann also ein Brand in dem Gebäude
entstehen, der auch von der BMA erkannt wird. Vergleichbar ist die Funktion mit der deaktivierung der Übertragungseinheit der BMA.
