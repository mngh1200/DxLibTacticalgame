#pragma once
#include <climits>
#include <string>
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

/**
 * @file Figure.h
 * @brief ��ʓ��v�f�̒��ۃN���X�i�}�E�X�C�x���g�����m���Ȃ��v�f��Figure�N���X�𒼐ڌp���j
 */

namespace FrameWork
{
	class Game;
}

namespace Entity
{
	class Figure
	{
		friend class ObjectsControl;
	public:
		/**
		 * @enum ObjectType
		 * �I�u�W�F�N�g�̎��
		 */
		enum Type : int {
			FIGURE,
			BACKGROUND,
			BUTTON,
			UNIT,
			MAP,
			TEXT,
			CONTEXT_MENU,
			RESULT_SCENE,
			OVERLAY
		};

		Figure() :
			type_(Type::FIGURE), layerId_(-1), objectId_(-1),
			isReserveDelete_(false), animationId_(-1), isView_(true) {};
		virtual ~Figure() {};

		virtual void init(int layerId, int objectId);

		virtual void render() const = 0;

		/**
		 * @fn
		 * �P�t���[�����̃A�j���[�V�������s
		 * @return true�̂Ƃ��A�A�j���[�V�����I��
		 */
		virtual bool animationUpdate() { return true; };

		bool isAnimation() const;

		void destroy();

		bool isDelete() const;

		int getLayerId() const;

		int getObjectId() const;

		int getType() const;

		int getX() const { return shape_.x; }; // X���W�擾
		int getY() const { return shape_.y; }; // Y���W�擾

	protected:
		/**
		 * @fn
		 * �A�j���[�V�����쐬(ObjectsControl::addAnimationObj���\�b�h��p�ŌĂяo��)
		 * @return �A�j���[�V�����쐬�\�ȏꍇtrue��Ԃ�
		 */
		virtual bool createAnimation(int animationId) { return false; };

		void joinAnimationList(int animationId = 0);
		void changeAnimation(int animationId);

		//! Object�̎��
		int type_;

		//! �I�u�W�F�N�g�̌`��(�ʒu��T�C�Y)
		Shape shape_;

		//! �A�j���[�V����ID�i���w��̂Ƃ���-1�j
		int animationId_;

		//! �����ڂ����̗v�f�̏ꍇ true�A�}�E�X�C�x���g�����m����v�f�̏ꍇ false
		bool isView_;

	private:

		//! �������C���[ID
		int layerId_;

		//! �I�u�W�F�N�gID
		int objectId_;

		//! �폜�\��
		bool isReserveDelete_;
	};
}