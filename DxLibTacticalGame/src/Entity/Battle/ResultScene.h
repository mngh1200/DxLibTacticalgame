#pragma once
#include <climits>
#include <string>
#include "FrameWork/Game.h"
#include "Entity/Object.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Utility/ResourceManager.h"
#include "Define.h"

/**
 * @file ResultScene.h
 * @brief ���s���ʉ��
 */

namespace Entity
{
	class ResultScene : public Object
	{
	public:
		ResultScene() : 
			isPlayerWin_(true),	alpha_(0), 
			textAlpha_(0), text(""), textX_(0) {};
		~ResultScene() {};

		static void makeResultScene(bool isPlayerWin);

		void init(bool isPlayerWin);

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:
		bool isPlayerWin_;

		Animation anim_; //! �A�j���[�V�����N���X

		// �A�j���[�V�������
		enum AnimKind
		{
			DISPLAY
		};

		int alpha_;		//! �s�����x
		int textAlpha_; //! �e�L�X�g�̕s�����x

		string text; //! ����/�s�k�e�L�X�g

		int textX_;  //! ����/�s�k�e�L�X�g��X���W
	};
}