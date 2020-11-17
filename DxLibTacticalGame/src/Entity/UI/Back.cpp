#include "Back.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	Back::Back()
	{
		shape_ = Shape(0, 0, WIN_W, WIN_H);
		objectType = BACKGROUND;
		isTrans_ = false;
		imagePath_ = -1;
	}

	/**
	 * @fn
	 * コンストラクタ
	 */
	Back::Back(Shape shape)
	{
		shape_ = Shape(0, 0, WIN_W, WIN_H);
		objectType = BACKGROUND;
		isTrans_ = false;
		imagePath_ = -1;
	}


	/**
	 * @fn
	 * 描画処理
	 */
	void Back::render() const
	{
		// テスト用
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), DxLib::GetColor(247, 241, 230), TRUE);
	}
}