#include "Rect.h"

namespace Entity
{
	Rect::Rect(int x, int y, int w, int h) {
		x_ = x;
		y_ = y;
		w_ = w;
		h_ = h;
	}

	/**
	 * @fn
	 * 当たり判定（主にマウスイベンWト用に使用）
	 */
	bool Rect::isHit(int x, int y) const
	{
		return x_ <= x && x <= (x_ + w_) && y_ <= y && y <= y_ + h_;
	}
}