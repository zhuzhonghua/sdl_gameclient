#pragma once

#include "define.h"

class Level;
class Room;

class Painter{
public:
	static void set(Level* level, int cell, int value);
	static void set(Level* level, int x, int y, int value);
	static void set(Level* level, Point p, int value);
	static void fill(Level* level, int x, int y, int w, int h, int value);
	static void fill(Level* level, Rect rect, int value);
	static void fill(Level* level, Rect rect, int m, int value);
	static void fill(Level* level, Rect rect, int l, int t, int r, int b, int value);
	static Point drawInside(Level* level, Room* room, Point from, int n, int value);
};

class EntrancePainter :public Painter{
public:
	static void paint(Level* level, Room* room);
};

class ExitPainter :public Painter{
public:
	static void paint(Level* level, Room* room);
};