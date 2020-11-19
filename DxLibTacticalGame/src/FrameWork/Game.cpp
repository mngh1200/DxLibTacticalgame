#include "Game.h"

namespace FrameWork
{
	/**
	 * @fn
	 * シングルトンにするためのインスタンス関数
	 */
	Game& Game::getInstance()
	{
		static Game instance;
		return instance;
	}

	/**
	 * @fn
	 * フレーム管理用にループで呼ばれる処理
	 * @return 終了時0を返す
	 */
	int Game::process()
	{
		// テスト用コード
		if (isInit) {
			// 初期処理
			Entity::Shape shape(0, 0, 100, 100);
			// Entity::Object(&shape);

			layerObjList.clear();

			// レイヤー追加
			for (int i = 0; i < InitLayer::LEN; i++)
			{
				layerObjList.push_back(map<int,shared_ptr<Entity::Object>>());
			}

			// 背景追加
			layerObjList.at(InitLayer::BACK)[0] = make_shared<Entity::Back>();

			// ボタン追加
			layerObjList.at(InitLayer::BUTTON)[0] = make_shared<Entity::Button>(Entity::Shape(WIN_W/2 - 50, WIN_H / 2 - 15, 100, 30));

			isInit = false;
		}

		// コントロール管理
		Controller& controller = Controller::getInstance();
		if (controller.getAllEvents() != 0)
		{
			return -1;
		}

		// 描画処理(逆ループ)
		for (auto layerItr = rbegin(layerObjList); layerItr != rend(layerObjList); ++layerItr)
		{
			for (auto objMapItr = layerItr->rbegin(); objMapItr != layerItr->rend(); objMapItr++)
			{
				shared_ptr<Entity::Object> obj = (*objMapItr).second;
				obj->render();
			}
		}


		// FPS管理
		controlFps();
		increaseFrame();

		return 0;
	}

	/**
	 * @fn
	 * FPS管理用
	 */
	void Game::controlFps()
	{
		waitTime_ = DxLib::GetNowCount() - prevTime_; // calculate 1 loop time
		if (prevTime_ == 0)
		{
			waitTime_ = ONE_FRAME_MS;
		}
		prevTime_ = DxLib::GetNowCount();  // set current time
		// adjust to 60 fps
		if (ONE_FRAME_MS > waitTime_)
		{
			Sleep(static_cast<DWORD>(ONE_FRAME_MS - waitTime_));
		}
	}

	/**
	 * @fn
	 * フレーム数カウント(int最大値でループ)
	 */
	void Game::increaseFrame()
	{
		frame_++;
		if (frame_ >= INT_MAX)
		{
			frame_ = 0;
		}
	}

	/**
	 * @fn
	 * フレームカウント数を取得
	 * @return フレームカウント数
	 */
	int Game::getFrame() const
	{
		return frame_;
	}
}