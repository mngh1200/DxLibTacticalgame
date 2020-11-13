#pragma once

namespace Entity
{
	class Shape
	{
	public:
		Shape() : x_(0), y_(0), w_(0), h_(0), type_(RECT) {};
		Shape(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h), type_(RECT) {};
		Shape(int x, int y, int w, int h, int type) : x_(x), y_(y), w_(w), h_(h), type_(type) {};
		~Shape() {};

		bool isHit(int x, int y) const;

		void set(int x, int y);
		void set(int x, int y, int w, int h);
		void set(int x, int y, int w, int h, int type);
		void setSize(int w, int h);
		void setType(int type);

		enum 
		{
			RECT
		};

	private:
		//! ¶À•W
		int x_;
		//! ãÀ•W
		int y_;
		//! •
		int w_;
		//! ‚‚³
		int h_;

		//! Œ`ó
		int type_;
	};
}