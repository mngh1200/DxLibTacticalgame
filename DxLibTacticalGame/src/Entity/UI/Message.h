#pragma once
#include <climits>
#include <string>
#include "Utility/ResourceManager.h"
#include "Entity/Object.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Define.h"
#include "Battle/BUI/BattleUIDefine.h"
#include "Utility/Timer.h"

using namespace std;

/**
 * @file Message.h
 * @brief ���b�Z�[�W�v�f
 */

namespace Entity
{
	class Message : public Object
	{
	public:
		Message();
		~Message() {};

		void show(string text, bool isSound, int displayTimeMs = -1);
		void hide();

		void onMouseClick(int x, int y) override;

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:
		void getClosePos(int* x, int* y) const;

		constexpr static int MARGIN = 10; //! �]��
		constexpr static int PADDING = 12; //! �]��
		constexpr static int PADDING_TOP = 6; //! �]��(�㑤)
		const static int FONT_TYPE = FontType::NORMAL_S24;  //! �t�H���g�^�C�v

		constexpr static int TEXT_PADDING_RIGHT = PADDING + 12; //! �e�L�X�g�݂̂̉E���]��

		constexpr static int CLOSE_SIZE = 18; //! ����{�^���̃T�C�Y
		constexpr static int CLOSE_MARGIN_BOTTOM = 6; //! ����{�^���̗]��
		constexpr static int CLOSE_H = CLOSE_SIZE + CLOSE_MARGIN_BOTTOM; //! ����{�^���̗]���܂߂�����

		constexpr static int ALPHA = 210; //! �s�����x
		constexpr static int ANIME_MS = 200; //! �A�j���[�V��������

		Shape shapeClose_; //! ����{�^���̍��W

		string text_; //! �e�L�X�g

		int displayTimeMs_; //! �\������

		Animation animation_; //! �A�j���[�V����

		// �A�j���[�V�����̎��
		enum AnimationKind
		{
			SHOW, // �\����
			DELAY_HIDE // �x����\��
		};
	};
}