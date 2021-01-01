#include "BattleUIDefine.h"

namespace BUI
{
	/**
	 * @fn
	 * ���x���`��
	 * @return �E�[��x���W��Ԃ�
	 */
	int drawLabel(int x0, int y0, string text, int w, int backgroundColorType, int textColorType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		x0 += BUI_LINE_MARGIN;
		y0 += BUI_LINE_MARGIN;
		int x1 = x0 + w + BUI_LINE_PADDING * 2;
		int y1 = y0 + BUI_FONT_SIZE + BUI_LINE_PADDING * 2;

		DxLib::DrawBox(x0, y0, x1, y1, rm.getColor(backgroundColorType), TRUE);
		DxLib::DrawStringFToHandle((float)(x0 + BUI_LINE_PADDING), (float)(y0 + BUI_LINE_PADDING), text.c_str(), rm.getColor(textColorType), rm.getHdlFont(FontType::NORMAL_S18));

		return x1;
	}

	/**
	 * @fn
	 * �l�`��
	 * @return �E�[��x���W��Ԃ�
	 */
	int drawValue(int x0, int y0, string text, int w, int textColorType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		x0 += BUI_LINE_MARGIN + BUI_LINE_PADDING;
		y0 += BUI_LINE_MARGIN + BUI_LINE_PADDING;
		int x1 = x0 + w + BUI_LINE_PADDING * 2;

		DxLib::DrawStringFToHandle((float)(x0), (float)(y0), text.c_str(), rm.getColor(textColorType), rm.getHdlFont(FontType::NORMAL_S18));

		return x1;
	}


	/**
	 * @fn
	 * ���蕶�����̔��p�����\�����ɕK�v�ȕ����擾
	 * @param (textLen) ������
	 * @return ��
	 */
	int getHanW(const int textLen)
	{
		return BUI_HAN_W * textLen;
	}

	/**
	 * @fn
	 * ���蕶�����̑S�p�����\�����ɕK�v�ȕ����擾
	 * @param (textLen) ������
	 * @return ��
	 */
	int getZenW(const int textLen)
	{
		return BUI_ZEN_W * textLen;
	}
}