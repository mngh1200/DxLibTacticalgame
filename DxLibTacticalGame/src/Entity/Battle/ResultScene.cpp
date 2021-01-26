#include "ResultScene.h"
#include "Screen/BattleScreen.h"

namespace Entity {
	/**
	 * @fn
	 * ���U���g��ʂ̐����֐�
	 * @param(isPlayerWin) �v���C���[������: true
	 */
	void ResultScene::makeResultScene(bool isPlayerWin)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		shared_ptr<ResultScene> objSp = make_shared<ResultScene>();
		objectsControl.addObject(Screen::BattleScreen::Layer::TOP_UI, Screen::BattleScreen::TopUiId::RESULT_SCENE, objSp);

		objSp->init(isPlayerWin);
	}

	/**
	 * @fn
	 * ��������
	 * @param(isPlayerWin) �v���C���[������: true
	 */
	void ResultScene::init(bool isPlayerWin)
	{
		isPlayerWin_ = isPlayerWin;

		type_ = Figure::RESULT_SCENE;

		if (isPlayerWin)
		{
			text = "WIN!";
			Utility::ResourceManager::playSound(SoundKind::WIN);
		}
		else
		{
			text = "LOSE...";
			Utility::ResourceManager::playSound(SoundKind::LOSE);
		}
		
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		int width = DxLib::GetDrawFormatStringWidthToHandle(rm.getHdlFont(FontType::BLACK_S64), text.c_str());
		textX_ = (WIN_W - width) / 2;

		joinAnimationList(AnimKind::DISPLAY);
		
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void ResultScene::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		// �I�[�o�[���C
		DxLib::DrawBox(0, 0, WIN_W, WIN_H, 0x000, TRUE);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, textAlpha_);

		// ���s�e�L�X�g
		if (isPlayerWin_)
		{
			DxLib::DrawStringFToHandle(textX_, 100, text.c_str(), rm.getColor(ColorType::PLAYER_COLOR), rm.getHdlFont(FontType::BLACK_S64));
		}
		else
		{
			DxLib::DrawStringFToHandle(textX_, 100, text.c_str(), rm.getColor(ColorType::ENEMY_COLOR), rm.getHdlFont(FontType::BLACK_S64));
		}
		

		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool ResultScene::animationUpdate()
	{
		if (animationId_ == AnimKind::DISPLAY) // �\���A�j���[�V����
		{
			bool isEnd = anim_.increaseFrame();

			anim_.update(&alpha_, 0, 160, false);
			anim_.update(&textAlpha_, 0, 255, false);

			return isEnd;
		}
		return false;
	}


	/**
	 * @fn
	 * �A�j���[�V�����쐬
	 * @param (animationId) �A�j���[�V�����̎��
	 */
	bool ResultScene::createAnimation(int animationId)
	{
		if (animationId == AnimKind::DISPLAY) // �\���A�j���[�V����
		{
			anim_ = Animation(1000);
			return true;
		}
		return false;
	}
}