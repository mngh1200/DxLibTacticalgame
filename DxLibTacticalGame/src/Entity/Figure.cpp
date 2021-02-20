#include "Figure.h"
#include "FrameWork/Game.h"

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
	 * アニメーションの追加可能か
	 * @return 追加済みの場合 false
	 */
	bool Figure::isAnimation() const
	{
		return animationId_ != -1;
	}

	/**
	 * @fn
	 * オブジェクト自身をObjectsControlクラスのアニメーションオブジェクトリストに追加する
	 * @param (animationId) セットするアニメーションID
	 */
	void Figure::joinAnimationList(int animationId)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		game.objectsControl.addAnimationObj(animationId, getLayerId(), getObjectId(), isView_);
	}

	/**
	 * @fn
	 * アニメーションを変更する
	 * @param (animationId) セットするアニメーションID
	 */
	void Figure::changeAnimation(int animationId)
	{
		if (isAnimation())
		{
			animationId_ = animationId;
			createAnimation(animationId_);
		}
		else
		{
			joinAnimationList(animationId);
		}
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