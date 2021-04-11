#include "Panel.h"

namespace Entity {

	/**
	 * @fn
	 * 座標セット
	 * @param (x) x座標
	 * @param (y) y座標
	 */
	void Panel::setPos(int x, int y)
	{
		shape_.x = x;
		shape_.y = y;
	}

	/**
	 * @fn
	 * サイズセット
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void Panel::setSize(int w, int h)
	{
		shape_.w = w;
		shape_.h = h;
	}

	/**
	 * @fn
	 * 矩形座標セット
	 * @param (x) x座標
	 * @param (y) y座標
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void Panel::setShape(int x, int y, int w, int h)
	{
		shape_.set(x, y, w, h);
	}

	/**
	 * @fn
	 * 色セット
	 * @param (colorType) カラータイプ
	 */
	void Panel::setColor(int colorType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		color_ = rm.getColor(colorType);
	}

	/**
	 * @fn
	 * 描画処理
	 */
	void Panel::render() const
	{
		if (color_ == -1) // 色未指定
		{
			return;
		}

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), color_, TRUE);
	}
}