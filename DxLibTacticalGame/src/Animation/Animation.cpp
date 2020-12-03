#include "Animation.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 * @param (timeMs) �A�j���[�V��������
	 * @param (direction) �Đ������̎��
	 * @param (repeat) ���s�[�g��
	 * @param (delayMs) �J�n�܂ł̒x������
	 * @param (func) ���s���x�i�C�[�W���O�֐��w��j< Easing.h�̊֐�����I��
	 */
	Animation::Animation(int timeMs, int direction, int repeat, int delayMs,
		function<float(float, float, float, float)> func)
		: direction_(direction), isNormalOrder_(true), repeat_(repeat), frameCount_(0)
	{
		delayFrame_ = (delayMs - ONE_FRAME_MS + 1) / ONE_FRAME_MS;
		frameMax_ = (float)((timeMs - ONE_FRAME_MS + 1) / ONE_FRAME_MS);
		calcFunk_ = func;

		if (direction_ == Direction::REVERSE || direction_ == Direction::AlTERNATE_REVERSE)
		{
			// �t������n�܂�ꍇ
			isNormalOrder_ = false;
			frameCount_ = frameMax_;
		}
	}

	/**
	 * @fn
	 * �t���[���J�E���g
	 * @return �I������ true: �I��
	 */
	bool Animation::increaseFrame()
	{
		// �x�����ԏ���
		if (delayFrame_ > 0)
		{
			delayFrame_--;
		}
		// �t���[���J�E���g���ő�t���[���J�E���g�܂Ői��ł����ꍇ
		else if ((isNormalOrder_ && frameCount_ >= frameMax_) ||
					(!isNormalOrder_ && frameCount_ <= 0))
		{
			// �������[�v�̏ꍇ�͏I�����肵�Ȃ�
			if (repeat_ != REPEAT_INFINITE)
			{
				--repeat_; // ���s�[�g�񐔏���
				if (repeat_ == 0) // ���s�[�g�񐔂�0�ɂȂ������_�ŏI��
				{
					// �I������
					return true;
				}
			}

			if (direction_ == Direction::AlTERNATE || direction_ == Direction::AlTERNATE_REVERSE)
			{
				// �t���ɂ���
				isNormalOrder_ = !isNormalOrder_;
			}

			// �t���[���J�E���g���Z�b�g
			if (isNormalOrder_)
			{
				frameCount_ = 0;
			}
			else
			{
				frameCount_ = frameMax_;
			}
		}
		else if (isNormalOrder_) // �t���[���J�E���g
		{
			++frameCount_;
		}
		else // �t���[���J�E���g�t��
		{
			--frameCount_;
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V�������X�V�\��
	 * @return true: �X�V�\
	 */
	inline bool Animation::getAbleUpdate() const
	{
		return delayFrame_ <= 0 && repeat_ != 0;
	}

	/**
	 * @fn
	 * float�l���������ω�����
	 * @param (y) �ω��l
	 * @param (y0) �����l
	 * @param (y1) �I���l
	 * @param (isIncreaseFrame) �t���[���J�E���g�����s���邩
	 * @return true: �I������
	 */
	bool Animation::update(float* y, float y0, float y1, bool isIncreaseFrame)
	{
		bool isFin = false;

		// �t���[���J�E���g
		if (isIncreaseFrame)
		{
			isFin = increaseFrame();
		}
		
		if (getAbleUpdate())
		{
			*y = calcFunk_(frameCount_, frameMax_, y1, y0);
		}
		return isFin;
	}

	/**
	 * @fn
	 * float�l���������ω�����
	 * @param (y) �ω��l
	 * @param (y0) �����l
	 * @param (y1) �I���l
	 * @param (isIncreaseFrame) �t���[���J�E���g�����s���邩
	 * @return true: �I������
	 */
	bool Animation::update(int* y, int y0, int y1, bool isIncreaseFrame)
	{
		bool isFin = false;

		// �t���[���J�E���g
		if (isIncreaseFrame)
		{
			isFin = increaseFrame();
		}

		if (getAbleUpdate())
		{
			*y = (int)calcFunk_(frameCount_, frameMax_, (float)y1, (float)y0);
		}
		return isFin;
	}

	/**
	 * @fn
	 * 2��int�l���������ω�����
	 * @param (nowX) �ω��lx
	 * @param (nowY) �ω��ly
	 * @param (x0) x�����l
	 * @param (y0) y�����l
	 * @param (x1) x�I���l
	 * @param (y1) y�I���l
	 * @param (isIncreaseFrame) �t���[���J�E���g�����s���邩
	 * @return true: �I������
	 */
	bool Animation::update(int* nowX, int* nowY, int x0, int y0, int x1, int y1, bool isIncreaseFrame)
	{
		bool isFin = false;

		// �t���[���J�E���g
		if (isIncreaseFrame)
		{
			isFin = increaseFrame();
		}

		if (getAbleUpdate())
		{
			float y = calcFunk_(frameCount_, frameMax_, 1.0, 0.0);
			*nowX = x0 + (int)(x1 * y);
			*nowY = y0 + (int)(y1 * y);
		}
		return isFin;
	}

	/**
	 * @fn
	 * Shape�N���X�̃T�C�Y��ω�������
	 * @param (nowShape) ����p
	 * @param (baseShape) ��̑傫���i�����l�j
	 * @param (scale) �{��
	 * @param (isIncreaseFrame) �t���[���J�E���g�����s���邩
	 * @return true: �I������
	 */
	bool Animation::update(Shape* nowShape, Shape baseShape, float scale, bool isIncreaseFrame)
	{
		bool isFin = false;

		// �t���[���J�E���g
		if (isIncreaseFrame)
		{
			isFin = increaseFrame();
		}

		if (getAbleUpdate())
		{
			float y = calcFunk_(frameCount_, frameMax_, scale, 1.0);
			float positionAdjustScale = (y - 1.0F) / 2.0F;
			nowShape->x = (int)(baseShape.x - (positionAdjustScale * baseShape.w));
			nowShape->y = (int)(baseShape.y - (positionAdjustScale * baseShape.h));
			nowShape->w = (int)(baseShape.w * y);
			nowShape->h = (int)(baseShape.h * y);
		}
		return isFin;
	}

	/**
	 * @fn
	 * �F��ω�������
	 * @param (nowColor) ����p
	 * @param (r0) r�l�̏����l
	 * @param (g0) g�l�̏����l
	 * @param (b0) b�l�̏����l
	 * @param (r1) r�l�̏I���l
	 * @param (g1) g�l�̏I���l
	 * @param (b1) b�l�̏I���l
	 * @param (isIncreaseFrame) �t���[���J�E���g�����s���邩
	 * @return true: �I������
	 */
	bool Animation::update(int* nowColor, int r0, int g0, int b0, int r1, int g1, int b1, bool isIncreaseFrame)
	{
		bool isFin = false;

		// �t���[���J�E���g
		if (isIncreaseFrame)
		{
			isFin = increaseFrame();
		}

		if (getAbleUpdate())
		{
			int r, g, b;
			float y = calcFunk_(frameCount_, frameMax_, 1.0, 0.0);
			r = r0 + (int)((r1 - r0) * y);
			g = g0 + (int)((g1 - g0) * y);
			b = b0 + (int)((b1 - b0) * y);
			*nowColor = DxLib::GetColor(r, g, b);
		}
		return isFin;
	}


	/**
	 * @fn
	 * ���ݒl����Ƀt���[���l�𒲐�����
	 * @param (y) ���ݒl
	 * @param (y0) �����l
	 * @param (y1) �I���l
	 */
	void Animation::adjustFrame(float y, float y0, float y1)
	{
		// �l���������Ă�����(�ʏ�Đ��̏ꍇ��)
		bool isIncrease = y0 < y1;

		for (int i = 0; i <= frameMax_; ++i)
		{
			float tmpY = calcFunk_((float)i, frameMax_, y1, y0);

			if ((isIncrease && tmpY >= y) || 
				(!isIncrease && tmpY <= y))
			{
				frameCount_ = (float)i;
				if (isNormalOrder_)
				{
					frameCount_--;
				}
				return;
			}
		}
	}

	/**
	 * @fn
	 * ���ݒl����Ƀt���[���l�𒲐�����
	 * @param (nowShape) ���݃T�C�Y
	 * @param (baseShape) ��T�C�Y
	 * @param (scale) �ύX�{��
	 */
	void Animation::adjustFrame(Shape nowShape, Shape baseShape, float scale)
	{
		adjustFrame((float)nowShape.w / (float)baseShape.w, 1.0, scale);
	}
}