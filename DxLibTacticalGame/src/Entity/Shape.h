#pragma once

/**
 * @file Shape.h
 * @brief 画面内要素の位置や形状を管理するクラス
 */

namespace Entity
{
	class Shape
	{
	public:
		Shape() : x(0), y(0), w(0), h(0), type(RECT), disabledHit(false) {};
		Shape(int x, int y, int w, int h) : x(x), y(y), w(w), h(h), type(RECT), disabledHit(false) {};
		Shape(int x, int y, int w, int h, int type) : x(x), y(y), w(w), h(h), type(type), disabledHit(false) {};
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

		//! イベント無効化
		bool disabledHit;

	private:

	};
}