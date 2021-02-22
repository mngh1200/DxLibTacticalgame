#include "Message.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	Message::Message() : text_(""), displayTimeMs_(-1), animation_{}
	{
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * メッセージ表示
	 * @param (text) 表示テキスト
	 * @param (isSound) 効果音を鳴らす場合はtrue
	 * @param (displayTimeMs) 表示時間 (デフォルトは無期限)
	 */
	void Message::show(string text, bool isSound, int displayTimeMs)
	{
		shape_.disabledHit = false;

		text_ = text;

		if (isSound)
		{
			Utility::ResourceManager::playSound(SoundKind::MESSAGE);
		}
		
		// テキストの内容を元にサイズ取得
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		int lineCount = 1;

		DxLib::GetDrawStringSizeToHandle(&shape_.w, &shape_.h, &lineCount, text.c_str(), 
			DxLib::GetStringLength(text.c_str()), rm.getHdlFont(FONT_TYPE));

		shape_.w += PADDING + TEXT_PADDING_RIGHT;
		shape_.h += PADDING + PADDING_TOP + CLOSE_H;

		shape_.x = WIN_W - shape_.w;
		shape_.y = BATTLE_UI_AREA_Y - MARGIN - shape_.h; // Y座標

		// アニメーション開始
		joinAnimationList(AnimationKind::SHOW);

		displayTimeMs_ = displayTimeMs;
		
	}

	/**
	 * @fn
	 * 非表示
	 */
	void Message::hide()
	{
		text_ = "";
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * マウスクリックイベント
	 * @param (x) x座標
	 * @param (y) y座標
	 */
	void Message::onMouseClick(int x, int y)
	{
		// 閉じるボタン押下時に閉じる
		int closeX;
		int closeY;
		getClosePos(&closeX, &closeY);
		if (Shape(closeX, closeY, CLOSE_SIZE, CLOSE_SIZE).isHit(x, y))
		{
			hide();
		}
	}

	/**
	 * @fn
	 * 描画
	 */
	void Message::render() const
	{
		if (text_ == "")
		{
			return;
		}

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);

		// 枠
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR_ON), TRUE);
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR_ON_DARK), FALSE);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 閉じるボタン
		int closeX;
		int closeY;
		getClosePos(&closeX, &closeY);
		DxLib::DrawStringToHandle(closeX, closeY, "×",
			rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S24));

		// テキスト
		DxLib::DrawStringToHandle(shape_.x + PADDING, shape_.y + PADDING_TOP + CLOSE_H, text_.c_str(),
			rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FONT_TYPE));
	}

	/**
	 * @fn
	 * アニメーション更新
	 */
	bool Message::animationUpdate()
	{
		if (animationId_ == AnimationKind::SHOW || animationId_ == AnimationKind::DELAY_HIDE) // 表示時アニメーション
		{
			if (animation_.update(&shape_.x, WIN_W, WIN_W - shape_.w - MARGIN))
			{
				if (animationId_ == AnimationKind::SHOW)
				{
					if (displayTimeMs_ != -1) // 表示時間が制限されている場合
					{
						changeAnimation(AnimationKind::DELAY_HIDE);
						return false;
					}
					else
					{
						return true;
					}
				}
				else
				{
					hide(); // 非表示
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * @fn
	 * アニメーション作成
	 * @param (animationId) アニメーションの種類
	 */
	bool Message::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::SHOW) // 表示時アニメーション
		{
			animation_ = Animation(ANIME_MS);
			return true;
		}
		else if (animationId_ == AnimationKind::DELAY_HIDE) // 一定時間後非表示
		{
			animation_ = Animation(ANIME_MS, Animation::Direction::REVERSE, 1, displayTimeMs_);
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * 閉じるボタンの矩形座標取得
	 * @return 閉じるボタンの矩形座標
	 */
	void Message::getClosePos(int* x, int* y) const
	{
		*x = shape_.getX2() - PADDING - CLOSE_SIZE;
		*y = shape_.y + PADDING_TOP;
	}
}