#include "Back.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	Back::Back()
	{
		objectType = BACKGROUND;
	}

	/**
	 * @fn
	 * 初期化処理（サイズは固定）
	 */
	void Back::init(int layerId, int objectId, Shape shape)
	{
		Object::init(layerId, objectId, Shape(0, 0, WIN_W, WIN_H));
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