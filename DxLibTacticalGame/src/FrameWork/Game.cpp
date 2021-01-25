#include "Game.h"
#include "Screen/ScreenBase.h"

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
		// �X�N���[����������
		if (!nowScreen_->isInited()) {
			nowScreen_->init(); // ��������
			nowScreen_->inited(); // ���������ς݃t���O�Z�b�g
		}

		if (!isLock_)
		{
			// �L�[�C�x���g�Ǘ�
			Controller& controller = Controller::getInstance();
			if (controller.getAllEvents() != 0)
			{
				return -1;
			}

			// �}�E�X�C�x���g
			int x, y, button, eventType;
			weak_ptr<Entity::Object> hitObjWp = objectsControl.checkMouseEvent(&x, &y, &button, &eventType);

			// �C�x���g�擾��̉�ʍX�V����
			nowScreen_->updateByEvents(hitObjWp, x, y, button, eventType);
		}


		// �A�j���[�V����
		objectsControl.updateAnimation();

		// �A�j���[�V������̉�ʍX�V�����i����̗v�f�̃A�j���[�V�������I�����Ă��邩����ɍX�V����ꍇ�Ɏg�p�j
		nowScreen_->updateByAnimation();

		// �`�您��уI�u�W�F�N�g�폜����
		objectsControl.renderAndDelete();


		// �I������
		if (isFinish_)
		{
			return -1;
		}

		// FPS�Ǘ�
		controlFps();
		increaseFrame();

		return 0;
	}

	/**
	 * @fn
	 * ��ʑJ��
	 * @param (newScreen) �J�ڐ�̃X�N���[���N���X
	 */
	void Game::setScreen(Screen::ScreenBase* newScreen)
	{
		setScreenLock(false);
		objectsControl.clear();
		nowScreen_.reset(newScreen);
	}

	/**
	 * @fn
	 * �C�x���g�̗L��/�����؂�ւ�
	 * @param (isLock) true: �����Afalse: �L��
	 */
	void Game::setScreenLock(bool isLock)
	{
		isLock_ = isLock;
	}

	/**
	 * @fn
	 * �Q�[���I��
	 */
	void Game::finish()
	{
		isFinish_ = true;
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