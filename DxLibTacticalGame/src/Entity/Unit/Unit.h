#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"
#include "Animation/Animation.h"
#include "Entity/Battle/Map.h"
#include "Entity/Effect/DamageEffect.h"

using namespace std;

/**
 * @file Unit.h
 * @brief ���j�b�g�̃C���^�[�t�F�[�X�N���X
 */

namespace Entity
{
	struct UnitInfo
	{
		//! ���O
		string name = "";

		//! ���j�b�g���
		int kind = UnitKey::LANCER;

		//! �e��X�e�[�^�X
		int hp = 0;
		int hpm = 0;
		int atk = 0;
		int def = 0;
		int mov = 0;
		int len = 0;
		int range = 1;
	};

	class Unit : public Object
	{
	public:
		Unit() : x_(0), y_(0), baseX_(0), baseY_(0), targetRealX_(0), targetRealY_(0), 
			imageId_(0), isEnemy_(false), state_(State::NORMAL), isActed_(false),
			animation_{}, animationSub_{}, viewHp_(0), prevHp_(0), alpha_(255)  {};
		~Unit() {};

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

		bool damage(int damage);

		void avoid();

		bool checkDead();

		void atack(int targetRealX, int targetRealY);

		bool select(bool isSelect);

		void turnEnd();

		void endAction();

		string getLenText() const;

		int getMassX() const { return x_; }; // x���W��Ԃ�
		int getMassY() const { return y_; }; // y���W��Ԃ�
		int getBaseX() const { return baseX_; }; // �ړ���x���W��Ԃ�
		int getBaseY() const { return baseY_; }; // �ړ���y���W��Ԃ�
		
		
		UnitInfo getInfo() const { return info_; } // ���j�b�g����X�e�[�^�X�̏���Ԃ�

		bool isHorse() const { return info_.kind == UnitKey::CAVALRY; } // �R���ł����true��Ԃ�
		bool isEnemy() const { return isEnemy_; } // �G���j�b�g�ł��邩��Ԃ�
		bool isActed() const { return isActed_; } // �s���I���ς݂ł��邩

	protected:
		bool createAnimation(int animationId);
	
	private:
		constexpr static int HP_PADDING = 5; //! HP�o�[�̗]��
		constexpr static int HP_Y = 55; //! HP�o�[�̑��Έʒuy
		constexpr static int HP_H = 5;  //! HP�o�[�̍���

		const static vector<string> LEN_TEXT; //! �˒��������e�L�X�g

		constexpr static int ANIME_ATACK_MS = 400;	  //! �U���A�j���[�V�����̎���
		constexpr static int ANIME_DAMAGE_MOVE = 10;  //! �_���[�W�A�j���V�����̓���͈�
		constexpr static int ANIME_DAMAGE_REPEAT = 4; //! �_���[�W�A�j���V�����̃��s�[�g��

		void setKind(int kind);

		//! �摜
		int imageId_;

		//! �ʒux
		int x_;

		//! �ړ���x
		int baseX_;

		//! �ʒuy
		int y_;

		//! �ړ���y
		int baseY_;

		//! �U�����x���W
		int targetRealX_;

		//! �U�����y���W
		int targetRealY_;

		// ���j�b�g�̏��
		UnitInfo info_;

		//! ��
		State state_;

		//! �s���I���ς݂ł��邩
		bool isActed_;

		//! �G���ǂ���
		bool isEnemy_;

		//! �A�j���[�V�����N���X
		Animation animation_;

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
		int viewHp_, prevHp_, alpha_;

	public:
		constexpr static int ANIME_DAMAGE_MS = 400;	//! �_���[�W�A�j���V�����̎���
	};


}