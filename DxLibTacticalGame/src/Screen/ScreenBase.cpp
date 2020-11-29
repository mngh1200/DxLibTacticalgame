#include "ScreenBase.h"
#include "FrameWork/Game.h"

namespace Screen
{
	/**
	 * @fn
	 * ���������ς݂ł��邩����
	 * @detail ���������ς݂ł����true
	 */
	bool ScreenBase::isInited() const
	{
		return isInited_;
	}

	/**
	 * @fn
	 * ����������ɌĂяo���A���������ςݔ���ϐ���true�ɕύX
	 */
	void ScreenBase::inited()
	{
		isInited_ = true;
	}

	/**
	 * @fn
	 * ��ʑJ�ڎ��̃I�[�o�[���C���Z�b�g����
	 * @param (isOpen) true: ��ʕ\�����Afalse: ��ʏI����
	 */
	void ScreenBase::createOverlay(bool isOpen)
	{
		Entity::ObjectsControl& objectsControl = FrameWork::Game::getInstance().objectsControl;
		objectsControl.addObject(0, OVERLAY_ID, make_shared<Entity::Overlay>(isOpen));
		objectsControl.addAnimationObj(0, 0, OVERLAY_ID);

		if (isOpen)
		{
			nowScene_ = START_OVERLAY_SCENE;
		}
		else
		{
			nowScene_ = END_OVERLAY_SCENE;
		}
	}


	/**
	 * @fn
	 * �I�[�o�[���C�̃A�j���[�V�������I�����Ă��邩����
	 * @return �I�����Ă����true
	 */
	bool ScreenBase::isOpenOverlayEnded()
	{
		if (nowScene_ == START_OVERLAY_SCENE)
		{
			FrameWork::Game& game = FrameWork::Game::getInstance();
			shared_ptr<Entity::Object> overlay = game.objectsControl.getObjectWp(0, OVERLAY_ID).lock();
			if (overlay && !overlay->isAnimation())
			{
				game.objectsControl.removeObject(0, OVERLAY_ID);
				return true;
			}
		}
	}

	/**
	 * @fn
	 * �I�[�o�[���C�̃A�j���[�V�������I�����Ă��邩����
	 * @return �I�����Ă����true
	 */
	bool ScreenBase::isCloseOverlayEnded()
	{
		if (nowScene_ == END_OVERLAY_SCENE)
		{
			FrameWork::Game& game = FrameWork::Game::getInstance();
			shared_ptr<Entity::Object> overlay = game.objectsControl.getObjectWp(0, OVERLAY_ID).lock();
			if (overlay && !overlay->isAnimation())
			{
				return true;
			}
		}
		return false;
	}
}