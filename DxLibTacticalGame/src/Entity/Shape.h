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

		void set(int x, int y);
		void set(int x, int y, int w, int h);
		void set(int x, int y, int w, int h, int type);
		void setSize(int w, int h);

		enum 
		{
			RECT
		};

		//! 左座標
		int x;
		//! 上座標
		int y;
		//! 幅
		int w;
		//! 高さ
		int h;
		//! 形状
		int type;

	private:

	};
}