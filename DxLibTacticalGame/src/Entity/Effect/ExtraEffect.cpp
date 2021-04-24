#include "ExtraEffect.h"
#include "Screen/BattleScreen.h"

namespace Entity {

	/**
	 * @fn
	 * �G�t�F�N�g����
	 * @param (atkUnit) �U�������j�b�g
	 * @param (defUnit) �h�䑤���j�b�g
	 * @param (text) �e�L�X�g
	 * @param (num) �A��
	 */
	void ExtraEffect::makeExtraEffect(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, const char* text, int num)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		shared_ptr<ExtraEffect> objSp = make_shared<ExtraEffect>();
		objectsControl.addFigure(Screen::BattleScreen::Layer::EFFECT, objSp);

		objSp->initExtraEffect(atkUnit, defUnit, text, num);
	}

	/**
	 * @fn
	 * ��������
	 * �_���[�W�G�t�F�N�g�𐶐�
	 * @param (atkUnit) �U�������j�b�g
	 * @param (defUnit) �h�䑤���j�b�g
	 * @param (text) �e�L�X�g
	 * @param (num) �A��
	 */
	void ExtraEffect::initExtraEffect(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, const char* text, int num)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		shape_.set(-W, BASE_Y + (H + MARGIN) * num);
		shape_.setSize(W, H);

		// �U�����܂��́A�h�䑤���j�b�g���\���ʒu�Əd�Ȃ�\��������ꍇ�A�ʒu����
		if (atkUnit->getX() < W || defUnit->getX() < W)
		{
			shape_.x = WIN_W;
			isLeft_ = false;
		}

		joinAnimationList(AnimationKind::SHOW);
	}

	/**
	 * @fn
	 * ��\��
	 */
	void ExtraEffect::hide()
	{
		joinAnimationList(AnimationKind::HIDE);
	}

	/**
	 * @fn
	 * �`��
	 */
	void ExtraEffect::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);
		DxLib::DrawStringToHandle(shape_.x + PADDING, shape_.y + PADDING, text_.c_str(), rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S24));
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool ExtraEffect::animationUpdate()
	{
		if (animationId_ == AnimationKind::SHOW || animationId_ == AnimationKind::HIDE) // �\��
		{
			bool isFin = false;
			if (isLeft_)
			{
				isFin = animation_.update(&shape_.x, -W, 0);
			}
			else
			{
				isFin = animation_.update(&shape_.x, WIN_W, WIN_W - W);
			}

			if (isFin && animationId_ == AnimationKind::HIDE)
			{
				destroy(); // ��\���A�j���[�V�����I�����A�v�f����
			}

			return isFin;
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V�����쐬
	 * @param (animationId) �A�j���[�V�����̎��
	 */
	bool ExtraEffect::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::SHOW)
		{
			animation_ = Animation(ANIMATION_TIME, 0, 1, NUM_DELAY * num_);
			return true;
		}
		else if (animationId == AnimationKind::HIDE)
		{
			animation_ = Animation(ANIMATION_TIME, Animation::Direction::REVERSE);
			return true;
		}
		return false;
	}
}