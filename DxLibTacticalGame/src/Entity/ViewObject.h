#pragma once
#include <climits>
#include <string>
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class ViewObject
	{
	public:
		/**
		 * @enum ObjectType
		 * �I�u�W�F�N�g�̎��
		 */
		enum ObjectType {
			OBJECT,
			BACKGROUND,
			BUTTON,
			UNIT,
			MASS,
			TEXT
		};

		ViewObject() :
			objectType(ObjectType::OBJECT),	isTrans_(false), imagePath_(-1), layerId_(-1), objectId_(-1),
			isReserveDelete_(false), animationId(-1) {};
		virtual ~ViewObject() {};

		virtual void init(int layerId, int objectId, Shape shape);

		virtual void render() const;

		void destroy();

		bool isDelete() const;

		int getLayerId() const;

		int getObjectId() const;

		//! Object�̎��
		ObjectType objectType;

	protected:
		/**
		 * @fn
		 * �P�t���[�����̃A�j���[�V�������s
		 * @return true�̂Ƃ��A�A�j���[�V�����I��
		 */
		virtual bool animationUpdate() { return false; };

		void renderExtend() const;

		//! �I�u�W�F�N�g�̌`��(�ʒu��T�C�Y)
		Shape shape_;

		//! ���{���̃A�j���[�V����ID�i-1�̂Ƃ��͖����{�j
		int animationId;

		//! �摜�𓧉߂����邩
		bool isTrans_;

		//! DxLib�œǂݍ��񂾉摜�̃p�X
		int imagePath_;

	private:

		//! �������C���[ID
		int layerId_;

		//! �I�u�W�F�N�gID
		int objectId_;

		//! �폜�\��
		bool isReserveDelete_;
	};
}