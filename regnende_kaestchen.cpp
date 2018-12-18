#include <iostream>
#define CIMGGIP_MAIN
#include "CImgGIP05.h"

using namespace std;

struct Box
{
	int x; // x-Koordinate (Zeile)
	int y; // y-Koordinate (Spalte)
	int delta_y; // aktuelle Fallgeschwindigkeit des Kaestchens
};

const int box_max = 10 /*maximale Anzahl von Boxen*/, box_size = 40; // Box groesse


// draw_boxes():
// Die Anzahl der Kaestchen wird nicht als zweiter Parameter uebergeben,
// da diese Anzahl als globale Konstante box_max im gesamten Programm bekannt ist ...

// Die "+10" bewirken, dass alle Kaestchen leicht nach rechts versetzt werden
// und somit links vom linksten Kaestchen noch etwas Platz bleibt (dieses
// also nicht am Rand klebt).
// Die "+20" sorgen fuer einen Zwischenraum von 20 zwischen den Kaestchen ...

void prepare_box_at_top(Box &b, int i)
{
	b.x = i * (box_size + 20) + 10; // +10 = leicht nach rechts, +20 = zwischenraum
	b.y = 0;

	if (b.delta_y > 0) 
		b.delta_y += 10; // wenn die Startgeschwindigkeit gesetzt ist über gip_zufall, wird sie jedes Mal um 10 erhöht wenn ein Kästchen angeklickt wird
	else
	{
		b.delta_y = gip_zufall(10, 30); // Startgeschwindigkeit ist ganzzahlige Zufallszahl zwischen 10 und 30 ...
	}
}

void draw_boxes(Box boxes[])
{
	// Loesche den bisherigen Fensterinhalt komplett,
	// d.h. komplettes Neuzeichnen aller Kaestchen etc ...

	gip_white_background();

	for (int i = 0; i < box_max; i++)
	{
		gip_draw_rectangle(boxes[i].x, boxes[i].y, boxes[i].x + box_size, boxes[i].y + box_size, blue);
	}
}

// Und jetzt alle Kaestchen zeichnen mittels gip_draw_rectangle().
// Linke obere Ecke: (boxes[i].x, boxes[i].y)
// Groesse: box_size x box_size
// Farbe: blue
// TO DO

// update_boxes():
// 1. Berechne neue y-Koordinate (jedes Kaestchen boxes[i] faellt um boxes[i].delta_y)
// 2. Pruefe, ob ein Kaestchen das untere Fensterende gip_win_size_y (ist ein int)
// ueberschritten hat. Falls ja: gib sofort false zurueck
// 3. (Sonst:) gib true zurueck, wenn keines der Kaestchen den unteren Rand
// ueberschritten hatte
// Die Anzahl der Kaestchen wird nicht als zweiter Parameter uebergeben,
// da diese Anzahl als globale Konstante box_max im gesamten Programm bekannt ist ...

bool update_boxes(Box boxes[])
{
	bool windowEnd = false;
	for (int i = 0; i < box_max; i++)
	{
		boxes[i].y += boxes[i].delta_y;
		if (boxes[i].y > gip_win_size_y) // SOLANGE die Boxen oberhalb vom unteren Fenster Ende sind, ist Window End auf true
			windowEnd = true;

	}
	if (windowEnd)
		return false; // wenn eine Box das Ende erreicht, wird windowEnd auf false gesetzt das Programm beendet
	else
	{
		draw_boxes(boxes); // ANSONSTEN werden weiter neue Boxen gezeichnet
		return true;
	}
}

// handle_mouse_click():
// Annahme: Funktion wird nur aufgerufen, wenn die Maus wirklich
// geklickt wurde. Die Funktion braucht dies also nicht mehr zu pruefen.
//
// Nimm die Koordinaten der Mausposition und pruefe dann fuer jedes Kaestchen,
// ob die Koordinaten innerhalb des Kaestchens liegen.
// Falls ja:
// * Erhoehe die Fallgeschwindigkeit des Kaestchens um 10
// * Setze das Kaestchen an den oberen Rand

void handle_mouse_click(Box boxes[])
{
	for (int i = 0; i < box_max; i++)
	{
		if (gip_mouse_x() >= boxes[i].x && gip_mouse_x() <= boxes[i].x + box_size && 
			gip_mouse_y() >= boxes[i].y && gip_mouse_y() <= boxes[i].y + box_size) 
			prepare_box_at_top(boxes[i], i);
	}
}

int main()
{
// Die "+10" bewirken, dass alle Kaestchen leicht nach rechts versetzt werden
// und somit links vom linksten Kaestchen noch etwas Platz bleibt (dieses
// also nicht am Rand klebt).
// Die "+20" sorgen fuer einen Zwischenraum von 20 zwischen den Kaestchen ...
	Box boxes[box_max] = { 0 };
	bool keep_going = true;
	for (int i = 0; i < box_max; i++) {
		prepare_box_at_top(boxes[i], i);
		draw_boxes(boxes);
	}
	// Startgeschwindigkeit ist ganzzahlige Zufallszahl zwischen 10 und 30 ...
	while (keep_going && gip_window_not_closed())
	{
		for (int loop_count = 0; loop_count < 200; loop_count++) {
			gip_wait(5); // warte 5 Milli-Sekunden
			if (gip_mouse_button1_pressed())
				handle_mouse_click(boxes);
		}
		keep_going = update_boxes(boxes);
		// Berechne die neue Fall-Position aller Kaestchen und pruefe,
		// ob eines der Kaestchen unten aus dem Fenster "herausgefallen" ist
		// (falls ja, wird der Wert false zurueckgegeben) ...
	}
	return 0;
}
