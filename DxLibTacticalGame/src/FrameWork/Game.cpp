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

	/**
	 * @fn
	 * フレーム管理用にループで呼ばれる処理
	 */
	void Game::control()
	{
		controlFps();  // keep 60 fps
		increaseFrame();
	}
}