#include "FrameWork/Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	{
		DxLib::SetWindowTextA("DxLibTacticalGame");

		if (DxLib::ChangeWindowMode(true) != DX_CHANGESCREEN_OK
			|| DxLib::DxLib_Init() == -1
			|| DxLib::SetWindowSizeExtendRate(1.0) == -1
			)
		{
			return -1;
		}

		if (DxLib::SetDrawScreen(DX_SCREEN_BACK) == -1)
		{
			return -1;
		}

		FrameWork::Game& game = FrameWork::Game::getInstance();

		// int img = DxLib::LoadGraph("resource/test1.bmp");

		// エスケープキーが押されたら終了
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
