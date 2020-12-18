#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Screen/StartScreen.h"

/**
 * @file Main.cpp
 * @brief メイン関数
 */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	{
		// Windowタイトル
		DxLib::SetWindowTextA("DxLibTacticalGame");

		// Windowモード設定
		if (DxLib::ChangeWindowMode(true) != DX_CHANGESCREEN_OK
			|| DxLib::DxLib_Init() == -1
			|| SetGraphMode(WIN_W, WIN_H, 32) == -1
			|| DxLib::SetWindowSizeExtendRate(1.0) == -1
			)
		{
			return -1;
		}

		// 裏画面設定
		if (DxLib::SetDrawScreen(DX_SCREEN_BACK) == -1)
		{
			return -1;
		}

		// Gameインスタンス設定
		FrameWork::Game& game = FrameWork::Game::getInstance();

		// スタート画面セット
		game.setScreen(new Screen::StartScreen());

		// リソース管理クラス
		Utility::ResourceManager& resourceManager = Utility::ResourceManager::getInstance();

		// リソースロード
		resourceManager.load();

		while (DxLib::ProcessMessage() == 0
			&& DxLib::ClearDrawScreen() == 0)
		{
			// DxLib::DrawGraph (game.getFrame(), 0, img, TRUE);

			// メインの処理
			if (game.process() == -1) {
				break;
			}

			// 裏画面を表に持ってくる
			DxLib::ScreenFlip();
		}

		DxLib::DxLib_End();				// ＤＸライブラリ使用の終了処理
	}

	return 0;					// ソフトの終了
}
