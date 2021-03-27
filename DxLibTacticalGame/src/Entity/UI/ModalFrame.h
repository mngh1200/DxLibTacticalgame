#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file ModalFrame.h
 * @brief ���[�_���̃t���[���v�f�N���X
 */

namespace Entity
{
	class ModalFrame : public Object
	{
	public:
		ModalFrame();
		~ModalFrame() {};

		constexpr static int HEAD_H = 46; //! �w�b�_�[�̍���

		void setShape(int x, int y, int w, int h);
		void setPos(int x, int y);
		void setSize(int w, int h);

		void setTitle(string title);

		void setValidOverlay(bool isValid);

		void setColor(int colorType);

		void render() const override;

		const Shape& getFrameShape() const { return frameBodyShape_; }; // ��`���W�擾

	private:
		constexpr static int OVERLAY_ALPHA = 100; //! �I�[�o���C�̕s�����x

		constexpr static int TITLE_FONT_TYPE = FontType::NORMAL_S24;
		constexpr static int HEAD_PADDING = 10; //! �w�b�_�[�]��

		Shape frameBodyShape_; //! �t���[���{�̂̋�`���W�i�I�[�o�[���C�l���j

		int color_; //! �F

		bool isOverlay_; //! �I�[�o�[���C�̗L��

		string title_; //! �w�b�_�[�^�C�g��
	};
}