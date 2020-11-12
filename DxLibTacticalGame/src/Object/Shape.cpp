#include "Rect.h"

namespace Entity
{
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
	 * サイズセット
	 */
	void Shape::setSize(int w, int h)
	{
		w_ = w;
		h_ = h;
	}
}