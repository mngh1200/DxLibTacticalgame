#include "ImageView.h"

namespace Entity {

	/**
	 * @fn
	 * 座標セット
	 * @param (x) x座標
	 * @param (y) y座標
	 */
	void ImageView::setPos(int x, int y)
	{
		type_ = IMAGE_VIEW;
		shape_.x = x;
		shape_.y = y;
	}

	/**
	 * @fn
	 * 座標セット
	 * @param (imageId) イメージIDセット 
	 */
	void ImageView::setImageId(int imageHandle)
	{
		imageHandle_ = imageHandle;
	}

	/**
	 * @fn
	 * 描画処理
	 */
	void ImageView::render() const
	{
		if (imageHandle_ == -1) // イメージハンドルが未設定の場合
		{
			return;
		}

		DxLib::DrawGraph(shape_.x, shape_.y, imageHandle_, TRUE);
	}
}