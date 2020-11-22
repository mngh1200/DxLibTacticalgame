#include "Game.h"

namespace FrameWork
{
	/**
	 * @fn
	 * �V���O���g���ɂ��邽�߂̃C���X�^���X�֐�
	 */
	Game& Game::getInstance()
	{
		static Game instance;
		return instance;
	}

	/**
	 * @fn
	 * �t���[���Ǘ��p�Ƀ��[�v�ŌĂ΂�鏈��
	 * @return �I����0��Ԃ�
	 */
	int Game::process()
	{
		// �e�X�g�p�R�[�h
		if (isInit) {
			// ��������
			Entity::Shape shape(0, 0, 100, 100);
			// Entity::Object(&shape);

			// ���C���[�Z�b�g
			objectsControl.setLayer(InitLayer::LEN);

			// �w�i�ǉ�
			objectsControl.addObject(InitLayer::BACK, 0, make_shared<Entity::Back>());

			// �{�^���ǉ�
			objectsControl.addObject(InitLayer::BUTTON, 0, make_shared<Entity::Button>(), Entity::Shape(WIN_W / 2 - 50, WIN_H / 2 - 15, 100, 30));

			objectsControl.addObject(InitLayer::BUTTON, 1, make_shared<Entity::Button>(), Entity::Shape(WIN_W / 2 - 50, WIN_H / 2 - 15 + 50, 100, 30));

			isInit = false;
		}

		// �R���g���[���Ǘ�
		Controller& controller = Controller::getInstance();
		if (controller.getAllEvents() != 0)
		{
			return -1;
		}

		// �A�j���[�V����
		objectsControl.updateAnimation();

		// �`�您��уI�u�W�F�N�g�폜����
		objectsControl.renderAndDelete();


		// FPS�Ǘ�
		controlFps();
		increaseFrame();

		return 0;
	}

	/**
	 * @fn
	 * FPS�Ǘ��p
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
	 * �t���[�����J�E���g(int�ő�l�Ń��[�v)
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
	 * �t���[���J�E���g�����擾
	 * @return �t���[���J�E���g��
	 */
	int Game::getFrame() const
	{
		return frame_;
	}
}