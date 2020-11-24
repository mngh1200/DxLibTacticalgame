#include "Figure.h"

namespace Entity {
	/**
	 * @fn
	 * 初期化処理
	 * @param (layerId) 所属レイヤー
	 * @param (objectId) 所属オブジェクト
	 * @param (shape) 位置、サイズ
	 */
	void Figure::init(int layerId, int objectId)
	{
		layerId_ = layerId;
		objectId_ = objectId;
	}

	/**
	 * @fn
	 * 描画処理
	 */
	void Figure::render() const
	{
		DxLib::DrawGraph(shape_.x, shape_.y, imagePath_, isTrans_);
	}

	/**
	 * @fn
	 * shape_のサイズに合わせた描画処理
	 */
	void Figure::renderExtend() const
	{
		DxLib::DrawExtendGraph(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), imagePath_, isTrans_);
	}

	/**
	 * @fn
	 * アニメーションIDがセットされていないときのみセット可能
	 * @return セットできた場合はtrueを返す
	 */
	bool Figure::setAnimationId(int animationId)
	{
		if (!isAnimation())
		{
			animationId_ = animationId;
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * アニメーションの追加可能か
	 * @return 追加済みの場合 false
	 */
	bool Figure::isAnimation()
	{
		return animationId_ != -1;
	}

	/**
	 * @fn
	 * オブジェクト削除(予約)
	 */
	void Figure::destroy()
	{
		isReserveDelete_ = true;
	}

	/**
	 * @fn
	 * オブジェクトが削除予約されているか判定
	 * @return 削除する場合true
	 */
	bool Figure::isDelete() const
	{
		return isReserveDelete_;
	}

	/**
	 * @fn
	 * 所属レイヤーIDを取得
	 * @return 所属レイヤーID
	 */
	int Figure::getLayerId() const
	{
		return layerId_;
	}

	/**
	 * @fn
	 * オブジェクトIDを取得
	 * @return オブジェクトID
	 */
	int Figure::getObjectId() const
	{
		return objectId_;
	}

	/**
	 * @fn
	 * オブジェクトIDを取得
	 * @return オブジェクトID
	 */
	int Figure::getType() const
	{
		return type_;
	}
}