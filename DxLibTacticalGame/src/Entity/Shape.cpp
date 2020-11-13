#include "Shape.h"

namespace Entity
{
	/**
	 * @fn
	 * 当たり判定（主にマウスイベンWト用に使用）
	 */
	bool Shape::isHit(int x, int y) const
	{
		if (type_ == RECT) 
		{
			return x_ <= x && x <= (x_ + w_) && y_ <= y && y <= y_ + h_;
		}
		return false;
	}

	/**
	 * @fn
	 * 位置セット
	 */
	void Shape::set(int x, int y)
	{
		x_ = x;
		y_ = y;
	}

	/**
	 * @fn
	 * 位置とサイズセット
	 */
	void Shape::set(int x, int y, int w, int h)
	{
		x_ = x;
		y_ = y;
		w_ = w;
		h_ = h;
	}

	/**
	 * @fn
	 * 位置とサイズと形状セット
	 */
	void Shape::set(int x, int y, int w, int h, int type)
	{
		x_ = x;
		y_ = y;
		w_ = w;
		h_ = h;
		type_ = type;
	}

	/**
	 * @fn
	 * サイズセット
	 */
	void Shape::setSize(int w, int h)
	{
		w_ = w;
		h_ = h;
	}

	/**
	 * @fn
	 * 形状セット
	 */
	void Shape::setType(int type)
	{
		type_ = type;
	}
}