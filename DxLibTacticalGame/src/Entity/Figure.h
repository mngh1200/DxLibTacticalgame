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

namespace Entity
{
	class Figure
	{
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
			MASS,
			TEXT,
			OVERLAY
		};

		Figure() :
			type_(Type::FIGURE), layerId_(-1), objectId_(-1),
			isReserveDelete_(false), animationId_(-1) {};
		virtual ~Figure() {};

		virtual void init(int layerId, int objectId);

		virtual void render() const = 0;

		/**
		 * @fn
		 * �P�t���[�����̃A�j���[�V�������s
		 * @return true�̂Ƃ��A�A�j���[�V�����I��
		 */
		virtual bool animationUpdate() { return true; };

		bool setAnimationId(int animationId);

		bool isAnimation();

		void destroy();

		bool isDelete() const;

		int getLayerId() const;

		int getObjectId() const;

		int getType() const;

	protected:

		//! Object�̎��
		int type_;

		//! �I�u�W�F�N�g�̌`��(�ʒu��T�C�Y)
		Shape shape_;

		//! �A�j���[�V����ID�i���w��̂Ƃ���-1�j
		int animationId_;

	private:

		//! �������C���[ID
		int layerId_;

		//! �I�u�W�F�N�gID
		int objectId_;

		//! �폜�\��
		bool isReserveDelete_;
	};
}