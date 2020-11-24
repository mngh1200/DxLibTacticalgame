#pragma once
#include <climits>
#include <string>
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class Figure
	{
	public:
		/**
		 * @enum ObjectType
		 * �I�u�W�F�N�g�̎��
		 */
		enum Type {
			FIGURE,
			BACKGROUND,
			BUTTON,
			UNIT,
			MASS,
			TEXT
		};

		Figure() :
			type_(FIGURE),	isTrans_(false), imagePath_(-1), layerId_(-1), objectId_(-1),
			isReserveDelete_(false), animationId_(-1) {};
		virtual ~Figure() {};

		virtual void init(int layerId, int objectId);

		virtual void render() const;

		/**
		 * @fn
		 * �P�t���[�����̃A�j���[�V�������s
		 * @return true�̂Ƃ��A�A�j���[�V�����I��
		 */
		virtual bool animationUpdate() { return false; };

		bool setAnimationId(int animationId);

		bool isAnimation();

		void destroy();

		bool isDelete() const;

		int getLayerId() const;

		int getObjectId() const;

		int getType() const;

	protected:

		void renderExtend() const;

		//! Object�̎��
		Type type_;

		//! �I�u�W�F�N�g�̌`��(�ʒu��T�C�Y)
		Shape shape_;

		//! �摜�𓧉߂����邩
		bool isTrans_;

		//! DxLib�œǂݍ��񂾉摜�̃p�X
		int imagePath_;

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