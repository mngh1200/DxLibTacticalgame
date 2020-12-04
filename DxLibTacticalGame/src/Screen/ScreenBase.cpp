#include "ScreenBase.h"
#include "FrameWork/Game.h"

namespace Screen
{
	/**
	 * @fn
	 * 初期処理済みであるか判定
	 * @detail 初期処理済みであればtrue
	 */
	bool ScreenBase::isInited() const
	{
		return isInited_;
	}

	/**
	 * @fn
	 * 初期処理後に呼び出し、初期処理済み判定変数をtrueに変更
	 */
	void ScreenBase::inited()
	{
		isInited_ = true;
	}

	/**
	 * @fn
	 * 画面遷移時のオーバーレイをセットする
	 * @param (isOpen) true: 画面表示時、false: 画面終了時
	 */
	void ScreenBase::createOverlay(bool isOpen)
	{
		Entity::ObjectsControl& objectsControl = FrameWork::Game::getInstance().objectsControl;
		objectsControl.addObject(0, OVERLAY_ID, make_shared<Entity::Overlay>(isOpen));
		objectsControl.addAnimationObj(0, 0, OVERLAY_ID);

		if (isOpen)
		{
			nowScene_ = START_OVERLAY_SCENE;
		}
		else
		{
			nowScene_ = END_OVERLAY_SCENE;
		}
	}


	/**
	 * @fn
	 * オーバーレイのアニメーションが終了しているか判定
	 * @return 終了していればtrue
	 */
	bool ScreenBase::isOpenOverlayEnded()
	{
		if (nowScene_ == START_OVERLAY_SCENE)
		{
			FrameWork::Game& game = FrameWork::Game::getInstance();
			shared_ptr<Entity::Object> overlay = game.objectsControl.getObjectWp(0, OVERLAY_ID).lock();
			if (overlay && !overlay->isAnimation())
			{
				game.objectsControl.removeObject(0, OVERLAY_ID);
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * オーバーレイのアニメーションが終了しているか判定
	 * @return 終了していればtrue
	 */
	bool ScreenBase::isCloseOverlayEnded()
	{
		if (nowScene_ == END_OVERLAY_SCENE)
		{
			FrameWork::Game& game = FrameWork::Game::getInstance();
			shared_ptr<Entity::Object> overlay = game.objectsControl.getObjectWp(0, OVERLAY_ID).lock();
			if (overlay && !overlay->isAnimation())
			{
				return true;
			}
		}
		return false;
	}
}