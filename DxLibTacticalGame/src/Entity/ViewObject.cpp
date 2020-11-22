#include "ViewObject.h"

namespace Entity {
	/**
	 * @fn
	 * 初期化処理
	 * @param (layerId) 所属レイヤー
	 * @param (objectId) 所属オブジェクト
	 * @param (shape) 位置、サイズ
	 */
	void ViewObject::init(int layerId, int objectId, Shape shape)
	{
		layerId_ = layerId;
		objectId_ = objectId;
		shape_ = shape;
	}

	/**
	 * @fn
	 * 描画処理
	 */
	void ViewObject::render() const
	{
		DxLib::DrawGraph(shape_.x, shape_.y, imagePath_, isTrans_);
	}

	/**
	 * @fn
	 * オブジェクト削除(予約)
	 */
	void ViewObject::destroy()
	{
		isReserveDelete_ = true;
	}

	/**
	 * @fn
	 * オブジェクトが削除予約されているか判定
	 * @return 削除する場合true
	 */
	bool ViewObject::isDelete() const
	{
		return isReserveDelete_;
	}

	/**
	 * @fn
	 * 所属レイヤーIDを取得
	 * @return 所属レイヤーID
	 */
	int ViewObject::getLayerId() const
	{
		return layerId_;
	}

	/**
	 * @fn
	 * オブジェクトIDを取得
	 * @return オブジェクトID
	 */
	int ViewObject::getObjectId() const
	{
		return objectId_;
	}

	/**
	 * @fn
	 * shape_のサイズに合わせた描画処理
	 */
	void ViewObject::renderExtend() const
	{
		DxLib::DrawExtendGraph(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), imagePath_, isTrans_);
	}
}