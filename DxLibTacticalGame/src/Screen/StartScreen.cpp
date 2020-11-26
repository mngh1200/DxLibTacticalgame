#include "StartScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * ��������
	*/
	void StartScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addFigure(Layer::TEXT, 0, make_shared<Entity::Text>("GAME TITLE", WIN_W / 2, 200, DxLib::CreateFontToHandle("���C���I", 64, 3, DX_FONTTYPE_NORMAL) , Entity::Text::Align::CENTER, DxLib::GetColor(124, 104, 95)));
		objectsControl.addFigure(Layer::TEXT, 1, make_shared<Entity::Text>("PRESS START", WIN_W / 2, 300, DxLib::CreateFontToHandle("�l�r ����", 9, 3, DX_FONTTYPE_ANTIALIASING), Entity::Text::Align::CENTER, DxLib::GetColor(211, 181, 155)));
	}

	/**
	 * @fn
	 * �C�x���g�擾��̃X�N���[���X�V����
	*/
	void StartScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp && hitObjSp->getLayerId() == Layer::BACK && eventType == MOUSE_INPUT_LOG_CLICK)
		{
			// �I�[�o�[���C�����i��ʑJ�ڗ\��j
			createOverlay(false);
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void StartScreen::updateByAnimation()
	{
		if (isCloseOverlayEnded())
		{
			// �I�[�o�[���C���������Ƃɉ�ʑJ��
			FrameWork::Game::getInstance().setScreen(new MenuScreen());
		}
		
	}
}