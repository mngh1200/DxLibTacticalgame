#include "Tooltip.h"

namespace Entity {

	/**
	 * @fn
	 * ツールチップの初期処理(基本はxy座標の右上に表示、画面外になる場合は調整)
	 */
	void Tooltip::show(int x, int y, string text)
	{
		text_ = text;

		// ■ 矩形座標計算
		shape_.set(x, y);
		
		// テキストの内容を元にサイズ取得
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		int lineCount = 1;

		DxLib::GetDrawStringSizeToHandle(&shape_.w, &shape_.h, &lineCount, text.c_str(), 
			DxLib::GetStringLength(text.c_str()), rm.getHdlFont(FONT_TYPE));

		shape_.w += PADDING * 2;
		shape_.h += PADDING * 2;

		shape_.y -= shape_.h; // 右上に表示するための調整

		// 画面外にはみ出す場合の調整
		if (shape_.getX2() > WIN_W)
		{
			shape_.x = WIN_W - shape_.w;
		}

		if (shape_.y < 0)
		{
			shape_.y = 0;
		}

		// アニメーション開始
		joinAnimationList(AnimationKind::SHOW);
	}

	/**
	 * @fn
	 * 非表示
	 */
	void Tooltip::hide()
	{
		joinAnimationList(AnimationKind::HIDE);
	}

	/**
	 * @fn
	 * 描画
	 */
	void Tooltip::render() const
	{
		if (alpha_ == 0)
		{
			return;
		}

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// 枠
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR_ON), TRUE);
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR_ON_DARK), FALSE);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// テキスト
		DxLib::DrawStringToHandle(shape_.x + PADDING, shape_.y + PADDING, text_.c_str(),
			rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S18));
	}

	/**
	 * @fn
	 * アニメーション更新
	 */
	bool Tooltip::animationUpdate()
	{
		if (animationId_ == AnimationKind::SHOW || animationId_ == AnimationKind::HIDE) // 非表示時アニメーション
		{
			if (animation_.update(&alpha_, 0, 230))
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * アニメーション作成
	 * @param (animationId) アニメーションの種類
	 */
	bool Tooltip::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::SHOW) // 表示時アニメーション
		{
			animation_ = Animation(ANIME_MS);
			return true;
		}
		else if (animationId == AnimationKind::HIDE) // 非表示時アニメーション
		{
			animation_ = Animation(ANIME_MS, Animation::Direction::REVERSE);
			return true;
		}
		return false;
	}
}