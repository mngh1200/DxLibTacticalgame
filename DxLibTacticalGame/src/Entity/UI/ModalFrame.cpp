#include "ModalFrame.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	ModalFrame::ModalFrame() : color_(-1), isOverlay_(true), title_("")
	{
		setValidOverlay(isOverlay_);
	}

	/**
	 * @fn
	 * ボタン位置とサイズ指定
	 * @param (x) X座標
	 * @param (y) Y座標
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void ModalFrame::setShape(int x, int y, int w, int h)
	{
		setPos(x, y);
		setSize(w, h);
	}

	/**
	 * @fn
	 * ボタン位置指定
	 * @param (x) X座標
	 * @param (y) Y座標
	 */
	void ModalFrame::setPos(int x, int y)
	{
		// テキストとボタンの相対座標を計算
		frameBodyShape_.x = x;
		frameBodyShape_.y = y;

		if (!isOverlay_)
		{
			shape_ = frameBodyShape_;
		}
	}

	/**
	 * @fn
	 * ボタンサイズ指定
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void ModalFrame::setSize(int w, int h)
	{
		frameBodyShape_.w = w;
		frameBodyShape_.h = h;

		if (!isOverlay_)
		{
			shape_ = frameBodyShape_;
		}
	}

	/**
	 * @fn
	 * タイトルセット
	 * @param (title) タイトル文言
	 */
	void ModalFrame::setTitle(string title)
	{
		title_ = title;
	}

	/**
	 * @fn
	 * オーバーレイの有効無効セット
	 * @param (isVald) 有効の場合 true、無効の場合 false
	 */
	void ModalFrame::setValidOverlay(bool isValid)
	{
		isOverlay_ = isValid;

		if (isValid)
		{
			shape_ = Shape(0, 0, WIN_W, WIN_H); // オーバーレイがある場合は画面全体に当たり判定を広げる
		}
		else
		{
			shape_ = frameBodyShape_;
		}
	}

	/**
	 * @fn
	 * 色指定
	 * @param (colorType) カラータイプ 
	 */
	void ModalFrame::setColor(int colorType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		color_ = rm.getColor(colorType);
	}
	/**
	 * @fn
	 * 描画
	 */
	void ModalFrame::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// オーバーレイ
		if (isOverlay_)
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, OVERLAY_ALPHA);
			DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), 0, TRUE);
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// フレーム本体
		int x1 = frameBodyShape_.x;
		int x2 = frameBodyShape_.getX2();
		int y1 = frameBodyShape_.y;
		int y2 = frameBodyShape_.getY2();

		// ヘッダータイトル
		if (title_ != "")
		{
			DxLib::DrawBox(x1, y1, x2, y1 + HEAD_H, rm.getColor(ColorType::SUB_COLOR), TRUE);
			DxLib::DrawStringToHandle(x1 + HEAD_PADDING, y1 + HEAD_PADDING, title_.c_str(), rm.getColor(ColorType::MAIN_COLOR), rm.getHdlFont(TITLE_FONT_TYPE));

			// フレーム位置をずらす
			y1 += HEAD_H;
		}

		DxLib::DrawBox(x1, y1, x2, y2, color_, TRUE);
	}
}