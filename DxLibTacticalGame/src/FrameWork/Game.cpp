#include "Game.h"
#include "Screen/ScreenBase.h"

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
		// スクリーン初期処理
		if (!nowScreen_->isInited()) {
			nowScreen_->init(); // 初期処理
			nowScreen_->inited(); // 初期処理済みフラグセット
		}

		if (!isLock_)
		{
			// キーイベント管理
			Controller& controller = Controller::getInstance();
			if (controller.getAllEvents() != 0)
			{
				return -1;
			}

			// マウスイベント
			int x, y, button, eventType;
			weak_ptr<Entity::Object> hitObjWp = objectsControl.checkMouseEvent(&x, &y, &button, &eventType);

			// イベント取得後の画面更新処理
			nowScreen_->updateByEvents(hitObjWp, x, y, button, eventType);
		}


		// アニメーション
		objectsControl.updateAnimation();

		// アニメーション後の画面更新処理（特定の要素のアニメーションが終了しているかを基準に更新する場合に使用）
		nowScreen_->updateByAnimation();

		// 描画およびオブジェクト削除処理
		objectsControl.renderAndDelete();


		// 終了判定
		if (isFinish_)
		{
			return -1;
		}

		// FPS管理
		controlFps();
		increaseFrame();

		return 0;
	}

	/**
	 * @fn
	 * 画面遷移
	 * @param (newScreen) 遷移先のスクリーンクラス
	 */
	void Game::setScreen(Screen::ScreenBase* newScreen)
	{
		setScreenLock(false);
		objectsControl.clear();
		nowScreen_.reset(newScreen);
	}

	/**
	 * @fn
	 * イベントの有効/無効切り替え
	 * @param (isLock) true: 無効、false: 有効
	 */
	void Game::setScreenLock(bool isLock)
	{
		isLock_ = isLock;
	}

	/**
	 * @fn
	 * ゲーム終了
	 */
	void Game::finish()
	{
		isFinish_ = true;
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