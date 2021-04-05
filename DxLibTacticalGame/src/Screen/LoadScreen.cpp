#include "LoadScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * ��������
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
	 * �C�x���g�擾��̃X�N���[���X�V����
	*/
	void LoadScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (Utility::ResourceManager::getInstance().isLoaded())
		{
			// �I�[�o�[���C�����i��ʑJ�ڗ\��j
			createOverlay(false);
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void LoadScreen::updateByAnimation()
	{
		if (isCloseOverlayEnded())
		{
			// �I�[�o�[���C���������Ƃɉ�ʑJ��
			FrameWork::Game::getInstance().setScreen(new MenuScreen());
		}
		
	}
}