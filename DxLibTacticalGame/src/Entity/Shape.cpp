#include "Shape.h"

namespace Entity
{
	/**
	 * @fn
	 * 当たり判定（主にマウスイベンWト用に使用）
	 * @param (mouseX) マウスのx座標
	 * @param (mouseY) マウスのy座標
	 */
	bool Shape::isHit(int x, int y) const
	{
		if (disabledHit) // イベント無効
		{
			return false;
		}

		if (type == RECT) 
		{
			return this->x <= x && x <= this->getX2() && this->y <= y && y <= this->getY2();
		}
		return false;
	}

	/**
	 * @fn
	 * 右側のX座標取得
	 * @return 右側のX座標
	 */
	int Shape::getX2() const
	{
		return x + w;
	}

	/**
	 * @fn
	 * 下側のY座標取得
	 * @return 下側のY座標
	 */
	int Shape::getY2() const
	{
		return y + h;
	}

	/**
	 * @fn
	 * float型でx座標取得
	 * @return x座標
	 */
	float Shape::getXf() const
	{
		return (float)x;
	}

	/**
	 * @fn
	 * float型でy座標取得
	 * @return y座標
	 */
	float Shape::getYf() const
	{
		return (float)y;
	}

	/**
	 * @fn
	 * float型で右側のx座標取得
	 * @return 右側のx座標
	 */
	float Shape::getX2f() const
	{
		return (float)(x + w);
	}

	/**
	 * @fn
	 * float型で下側のy座標取得
	 * @return 下側のy座標
	 */
	float Shape::getY2f() const
	{
		return (float)(y + h);
	}

	/**
	 * @fn
	 * 位置セット
	 * @param (x) x座標
	 * @param (y) y座標
	 */
	void Shape::set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	/**
	 * @fn
	 * 位置とサイズセット
	 * @param (x) x座標
	 * @param (y) y座標
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void Shape::set(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	/**
	 * @fn
	 * 位置とサイズと形状セット
	 * @param (x) x座標
	 * @param (y) y座標
	 * @param (w) 幅
	 * @param (h) 高さ
	 * @param (type) 形状
	 */
	void Shape::set(int x, int y, int w, int h, int type)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->type = type;
	}

	/**
	 * @fn
	 * サイズセット
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void Shape::setSize(int w, int h)
	{
		this->w = w;
		this->h = h;
	}
}