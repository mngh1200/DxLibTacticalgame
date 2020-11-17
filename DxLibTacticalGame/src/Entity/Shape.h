#pragma once

namespace Entity
{
	class Shape
	{
	public:
		Shape() : x(0), y(0), w(0), h(0), type(RECT) {};
		Shape(int x, int y, int w, int h) : x(x), y(y), w(w), h(h), type(RECT) {};
		Shape(int x, int y, int w, int h, int type) : x(x), y(y), w(w), h(h), type(type) {};
		~Shape() {};

		bool isHit(int x, int y) const;

		int getX2() const;
		int getY2() const;

		void set(int x, int y);
		void set(int x, int y, int w, int h);
		void set(int x, int y, int w, int h, int type);
		void setSize(int w, int h);

		enum 
		{
			RECT
		};

		//! ¶À•W
		int x;
		//! ãÀ•W
		int y;
		//! •
		int w;
		//! ‚‚³
		int h;
		//! Œ`ó
		int type;

	private:

	};
}