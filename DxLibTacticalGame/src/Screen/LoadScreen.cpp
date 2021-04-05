#include "LoadScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * 初期処理
	*/
	void LoadScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		Utility::ResourceManager::getInstance().load();

		Utility::SaveManager& saveManager = Utility::SaveManager::getInstance();
		saveManager.load();
	}

	/**
	 * @fn
	 * イベント取得後のスクリーン更新処理
	*/
	void LoadScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (Utility::ResourceManager::getInstance().isLoaded())
		{
			// オーバーレイ生成（画面遷移予約）
			createOverlay(false);
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void LoadScreen::updateByAnimation()
	{
		if (isCloseOverlayEnded())
		{
			// オーバーレイが閉じたあとに画面遷移
			FrameWork::Game::getInstance().setScreen(new MenuScreen());
		}
		
	}
}