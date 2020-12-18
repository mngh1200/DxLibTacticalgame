#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Screen/StartScreen.h"

/**
 * @file Main.cpp
 * @brief ���C���֐�
 */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	{
		// Window�^�C�g��
		DxLib::SetWindowTextA("DxLibTacticalGame");

		// Window���[�h�ݒ�
		if (DxLib::ChangeWindowMode(true) != DX_CHANGESCREEN_OK
			|| DxLib::DxLib_Init() == -1
			|| SetGraphMode(WIN_W, WIN_H, 32) == -1
			|| DxLib::SetWindowSizeExtendRate(1.0) == -1
			)
		{
			return -1;
		}

		// ����ʐݒ�
		if (DxLib::SetDrawScreen(DX_SCREEN_BACK) == -1)
		{
			return -1;
		}

		// Game�C���X�^���X�ݒ�
		FrameWork::Game& game = FrameWork::Game::getInstance();

		// �X�^�[�g��ʃZ�b�g
		game.setScreen(new Screen::StartScreen());

		// ���\�[�X�Ǘ��N���X
		Utility::ResourceManager& resourceManager = Utility::ResourceManager::getInstance();

		// ���\�[�X���[�h
		resourceManager.load();

		while (DxLib::ProcessMessage() == 0
			&& DxLib::ClearDrawScreen() == 0)
		{
			// DxLib::DrawGraph (game.getFrame(), 0, img, TRUE);

			// ���C���̏���
			if (game.process() == -1) {
				break;
			}

			// ����ʂ�\�Ɏ����Ă���
			DxLib::ScreenFlip();
		}

		DxLib::DxLib_End();				// �c�w���C�u�����g�p�̏I������
	}

	return 0;					// �\�t�g�̏I��
}
