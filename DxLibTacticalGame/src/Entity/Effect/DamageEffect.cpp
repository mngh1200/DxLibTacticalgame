#include "DamageEffect.h"
#include "Screen/BattleScreen.h"

namespace Entity {

	/**
	 * @fn
	 * �_���[�W�G�t�F�N�g�𐶐�
	 * @param (unitX) ��_���[�W�������j�b�g��X���W
	 * @param (unitY) ��_���[�W�������j�b�g��Y���W
	 * @param (damage) ��_���[�W��
	 */
	void DamageEffect::makeDamageEffect(int unitX, int unitY, int damage)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		shared_ptr<DamageEffect> objSp = make_shared<DamageEffect>();
		objectsControl.addFigure(Screen::BattleScreen::Layer::EFFECT, objSp);

		objSp->init(unitX, unitY, damage);
	}

	/**
	 * @fn
	 * ��������
	 * @param (unitX) ��_���[�W�������j�b�g��X���W
	 * @param (unitY) ��_���[�W�������j�b�g��Y���W
	 * @param (damage) ��_���[�W��
	 */
	void DamageEffect::init(int unitX, int unitY, int damage)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		shape_.y = unitY - H;
		shape_.h = H;

		if (damage == MISS) // MISS�\��
		{
			shape_.w = MISS_W;
			imageIds_.push_back(rm.getImage(ImageType::EFFECT, EffectId::MISS));
		}
		else // �_���[�W�\��
		{
			string damageStr = to_string(damage);
			int size = damageStr.length(); // ����

			shape_.w = size * DAMAGE_W;

			for (int i = 0; i < size; ++i )
			{
				// ����̌��̐��l�擾
				char s = damageStr.at(i);
				int num = s - '0';

				imageIds_.push_back(rm.getImage(ImageType::EFFECT, EffectId::DAMAGE, num));
			}
		}

		shape_.x = unitX + CHIP_SIZE / 2 - shape_.w / 2; // ��������

		if (shape_.y < 0) // ��ʊO�̏ꍇ
		{
			shape_.y = unitY;
		}

		joinAnimationList(AnimationKind::DISPLAY);
	}

	/**
	 * @fn
	 * �`��
	 */
	void DamageEffect::render() const
	{
		if (!isDisplay_)
		{
			return;
		}

		int count = 0;
		for (auto itr = imageIds_.begin(); itr != imageIds_.end(); ++itr)
		{
			DxLib::DrawGraph(shape_.x + DAMAGE_W * count, shape_.y, *itr, TRUE);
			++count;
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool DamageEffect::animationUpdate()
	{
		if (animationId_ == AnimationKind::DISPLAY)
		{
			
			if (animation_.increaseFrame())
			{
				// �\���I��
				destroy();
				return true;
			}
			else if (animation_.getDelayFinishLog_()) // �x���J�n���ԏI����
			{
				isDisplay_ = true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V�����쐬
	 * @param (animationId) �A�j���[�V�����̎��
	 */
	bool DamageEffect::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::DISPLAY)
		{
			animation_ = Animation(Unit::ANIME_DAMAGE_MS, 0, 1, Unit::ANIME_DAMAGE_MS);
			return true;
		}
		return false;
	}
}