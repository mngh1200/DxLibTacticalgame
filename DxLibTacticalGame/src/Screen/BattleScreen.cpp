#include "BattleScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * ��������
	*/
	void BattleScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;
		objectsControl.setLayer(Layer::LEN);

		// �}�b�v�i�}�X�j
		objectsControl.addObject(Layer::MAP, 0, make_shared<Entity::Map>());

		// ���j�b�g�ݒu(�e�X�g)
		objectsControl.addObject(Layer::PLAYER_UNIT, 0, make_shared<Entity::Unit>());

		// �I�[�o�[���C�Z�b�g
		createOverlay(true);
	}

	/**
	 * @fn
	 * �C�x���g�擾��̃X�N���[���X�V����
	*/
	void BattleScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();


		if (hitObjSp)
		{
			Entity::ObjectsControl& objCont = FrameWork::Game::getInstance().objectsControl;

			/*
			if (hitObjSp->getLayerId() == Layer::COURSE_BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
			}
			*/

		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void BattleScreen::updateByAnimation()
	{
		isOpenOverlayEnded();
		isCloseOverlayEnded();
	}

}