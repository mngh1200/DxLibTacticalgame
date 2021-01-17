#pragma once
#include <climits>
#include <string>
#include <memory>
#include "Utility/ResourceManager.h"
#include "Entity/Object.h"
#include "TerrainEffectDisplay.h"
#include "Entity/Unit/Unit.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "BattleUIDefine.h"

using namespace std;

/**
 * @file UnitStatusDisplay.h
 * @brief ���j�b�g�X�e�[�^�X�\����
 */

namespace Entity
{
	// �X�L�����̓���X�e�[�^�X�̕\�����e���Ǘ�����N���X
	struct ExtraStatus
	{
		string label; // ���x��
		string description; // ������
		Shape shape; // ��`���W
	};

	class UnitStatusDisplay : public Object
	{
	public:
		UnitStatusDisplay();
		~UnitStatusDisplay() {};

		void render() const override;
		bool animationUpdate() override;

		void updateByEvents(int x, int y);

		void setTargetUnit(shared_ptr<Unit> unit);
		void clear();

	protected:
		bool createAnimation(int animationId) override;

	private:
		constexpr static int WIDTH = 450; //! ��
		constexpr static int X = TerrainEffectDisplay::WIDTH + 40; //! X���W

		constexpr static int NAME_W = BUI_ZEN_W * 3; //! ���O���̕�
		constexpr static int EXTRA_STATUS_MARGIN = 6; //! ����X�e�[�^�X���̗]��

		constexpr static int ANIMATION_MS = 500;	//! �A�j���[�V��������
		constexpr static int ANIMATION_Y0 = WIN_H;  //! �A�j���[�V�����J�n�ʒu

		constexpr static int EXTRA_STATUS_ID_NONE = -1; //! ����X�e�[�^�X���w�莞��ID

		// �A�j���[�V�������
		enum AnimationKind
		{
			DISPLAY
		};

		//! �Ώۃ}�X
		shared_ptr<Unit> targetUnit_;

		//! �A�j���[�V�����p�ϐ�
		Animation animation_;

		//! ����X�e�[�^�X�̃��X�g
		vector<ExtraStatus> extraStatusList_;

		//! �z�o�[���̓���X�e�[�^�X
		int extraStatusHoverId_;
	};
}