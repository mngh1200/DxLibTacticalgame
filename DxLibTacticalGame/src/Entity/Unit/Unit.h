#pragma once
#include <climits>
#include "Entity/Object.h"
#include "UnitInfo.h"
#include "Utility/ResourceManager.h"
#include "Animation/Animation.h"
#include "Entity/Battle/Map.h"
#include "Entity/Effect/DamageEffect.h"
#include "Battle/BUI/EnemyTurnCont.h"

using namespace std;

/**
 * @file Unit.h
 * @brief ���j�b�g�N���X
 */

namespace Entity
{
	class Unit : public Object
	{
	public:
		Unit() : x_(0), y_(0), baseX_(0), baseY_(0), targetRealX_(0), targetRealY_(0), 
			imageId_(0), isEnemy_(false), state_(State::NORMAL), isActed_(false), closeAttackedLogs_(Direction::NONE_DIRECTION),
			animation_{}, animationSub_{}, viewHp_(0), prevHp_(0), alpha_(255), predictHp_(-1)  {};
		virtual ~Unit() {};

		//! �A�j���[�V�����X�s�[�h�̔{��
		static float animatinTimeRate;

		// �󋵂̎��
		enum class State
		{
			NORMAL,
			MOVE,
			SELECTED
		};

		void init(int x, int y, int kind, bool isEnemy = false);

		void render() const override;
		bool animationUpdate() override;

		void move(int x, int y);

		void setPos(int x, int y);

		void back();

		bool damage(int damage, int direction, bool isCloseAtack);

		void forceDecreaseHp(int decrease);

		void avoid();

		bool checkDead();

		void atack(int targetRealX, int targetRealY);

		bool select(bool isSelect);

		void turnEnd(bool isOwnEnd);

		void endAction();

		void getExtraStatusList(vector<pair<string, string>>& list) const;

		void setPredictDamage(int predictDamage);
		void clearPredictDamage();

		int getMassX() const { return x_; }; // x���W��Ԃ�
		int getMassY() const { return y_; }; // y���W��Ԃ�
		int getBaseX() const { return baseX_; }; // �ړ���x���W��Ԃ�
		int getBaseY() const { return baseY_; }; // �ړ���y���W��Ԃ�
		
		
		const UnitInfo& getInfo() const { return info_; } // ���j�b�g����X�e�[�^�X�̏���Ԃ�

		virtual bool isAtackable() const { return true; }; // �U���\�ł��邩
		int getCloseAttackLogs() const { return closeAttackedLogs_; } // �ߐڍU�����ꂽ������Ԃ�
		bool isHorse() const { return info_.kind == UnitKey::CAVALRY; } // �R���ł����true��Ԃ�
		bool isEnemy() const { return isEnemy_; } // �G���j�b�g�ł��邩��Ԃ�
		bool isActed() const { return isActed_; } // �s���I���ς݂ł��邩
		bool isStricken() const { return info_.hpm > info_.hp; } // �_���[�W���󂯂����Ƃ����邩

	protected:
		constexpr static int HP_PADDING = 5; //! HP�o�[�̗]��
		constexpr static int HP_Y = 55; //! HP�o�[�̑��Έʒuy
		constexpr static int HP_W = CHIP_SIZE - HP_PADDING * 2; //! HP�o�[�̕�
		constexpr static int HP_H = 5;  //! HP�o�[�̍���

		constexpr static int ANIME_ATACK_MS = 400;	  //! �U���A�j���[�V�����̎���
		constexpr static int ANIME_DAMAGE_MOVE = 10;  //! �_���[�W�A�j���V�����̓���͈�
		constexpr static int ANIME_DAMAGE_REPEAT = 4; //! �_���[�W�A�j���V�����̃��s�[�g��

		bool createAnimation(int animationId) override;
		virtual void createAtackAnimation();
		virtual bool updateAtackAnimation();

		virtual void renderExtra() const {}; // ���ʃN���X�Œǉ�����`�揈��
		virtual void turnEndExtra(bool isOwnEnd) {};      // ���ʃN���X�p�̃^�[���G���h����
		virtual void getExtraStatusListExtra(vector<pair<string, string>>& list) const {}; // 

		//! �}�X���W
		int x_, y_;

		//! �U����̐����̍��W
		int targetRealX_, targetRealY_;

		//! �A�j���[�V�����N���X
		Animation animation_;
	
	private:
		void setKind(int kind);
		void setImage(int state);

		//! �摜
		int imageId_;

		//! �ړ����}�X���W
		int baseX_, baseY_;

		// ���j�b�g�̏��
		UnitInfo info_;

		//! ��
		State state_;

		//! �s���I���ς݂ł��邩
		bool isActed_;

		//! �G���ǂ���
		bool isEnemy_;

		//! ���̃^�[�����ɋߐڍU�����󂯂������̃��O
		int closeAttackedLogs_;

		// �A�j���[�V�����̎��
		enum AnimationKind
		{
			MOVE,
			DAMAGE,
			AVOID,
			ATACK,
			FIRE,
			DESTROY
		};

		//! �A�j���[�V�����N���X�i�����K�v�������ꍇ�̃T�u�j
		Animation animationSub_;

		//! �A�j���[�V�����p�ϐ�
		int viewHp_, prevHp_, alpha_, predictHp_;

	public:
		constexpr static int ANIME_DAMAGE_MS = 400;	//! �_���[�W�A�j���V�����̎���
	};


}