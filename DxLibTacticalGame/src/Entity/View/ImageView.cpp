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
		x_ = x;
		y_ = y;
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
		DxLib::DrawGraph(x_, y_, imageHandle_, TRUE);
	}
}