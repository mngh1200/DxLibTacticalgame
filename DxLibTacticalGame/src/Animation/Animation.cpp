#include "Animation.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 * @param (timeMs) アニメーション時間
	 * @param (direction) 再生方向の種類
	 * @param (repeat) リピート回数
	 * @param (delayMs) 開始までの遅延時間
	 * @param (func) 実行速度（イージング関数指定）< Easing.hの関数から選択
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
			// 逆順から始まる場合
			isNormalOrder_ = false;
			frameCount_ = frameMax_;
		}
	}

	/**
	 * @fn
	 * フレームカウント
	 * @return 終了判定 true: 終了
	 */
	bool Animation::increaseFrame()
	{
		// 遅延時間処理
		if (delayFrame_ > 0)
		{
			delayFrame_--;
		}
		// フレームカウントが最大フレームカウントまで進んでいた場合
		else if ((isNormalOrder_ && frameCount_ >= frameMax_) ||
					(!isNormalOrder_ && frameCount_ <= 0))
		{
			// 無限ループの場合は終了判定しない
			if (repeat_ != REPEAT_INFINITE)
			{
				--repeat_; // リピート回数消化
				if (repeat_ == 0) // リピート回数が0になった時点で終了
				{
					// 終了判定
					return true;
				}
			}

			if (direction_ == Direction::AlTERNATE || direction_ == Direction::AlTERNATE_REVERSE)
			{
				// 逆順にする
				isNormalOrder_ = !isNormalOrder_;
			}

			// フレームカウントリセット
			if (isNormalOrder_)
			{
				frameCount_ = 0;
			}
			else
			{
				frameCount_ = frameMax_;
			}
		}
		else if (isNormalOrder_) // フレームカウント
		{
			++frameCount_;
		}
		else // フレームカウント逆順
		{
			--frameCount_;
		}
		return false;
	}

	/**
	 * @fn
	 * アニメーションを更新可能か
	 * @return true: 更新可能
	 */
	inline bool Animation::getAbleUpdate() const
	{
		return delayFrame_ <= 0 && repeat_ != 0;
	}

	/**
	 * @fn
	 * float値を少しずつ変化する
	 * @param (y) 変化値
	 * @param (y0) 初期値
	 * @param (y1) 終了値
	 * @param (isIncreaseFrame) フレームカウントを実行するか
	 * @return true: 終了判定
	 */
	bool Animation::update(float* y, float y0, float y1, bool isIncreaseFrame)
	{
		bool isFin = false;

		// フレームカウント
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
	 * float値を少しずつ変化する
	 * @param (y) 変化値
	 * @param (y0) 初期値
	 * @param (y1) 終了値
	 * @param (isIncreaseFrame) フレームカウントを実行するか
	 * @return true: 終了判定
	 */
	bool Animation::update(int* y, int y0, int y1, bool isIncreaseFrame)
	{
		bool isFin = false;

		// フレームカウント
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
	 * 2つのint値を少しずつ変化する
	 * @param (nowX) 変化値x
	 * @param (nowY) 変化値y
	 * @param (x0) x初期値
	 * @param (y0) y初期値
	 * @param (x1) x終了値
	 * @param (y1) y終了値
	 * @param (isIncreaseFrame) フレームカウントを実行するか
	 * @return true: 終了判定
	 */
	bool Animation::update(int* nowX, int* nowY, int x0, int y0, int x1, int y1, bool isIncreaseFrame)
	{
		bool isFin = false;

		// フレームカウント
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
	 * Shapeクラスのサイズを変化させる
	 * @param (nowShape) 代入用
	 * @param (baseShape) 基準の大きさ（初期値）
	 * @param (scale) 倍率
	 * @param (isIncreaseFrame) フレームカウントを実行するか
	 * @return true: 終了判定
	 */
	bool Animation::update(Shape* nowShape, Shape baseShape, float scale, bool isIncreaseFrame)
	{
		bool isFin = false;

		// フレームカウント
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
	 * 色を変化させる
	 * @param (nowColor) 代入用
	 * @param (r0) r値の初期値
	 * @param (g0) g値の初期値
	 * @param (b0) b値の初期値
	 * @param (r1) r値の終了値
	 * @param (g1) g値の終了値
	 * @param (b1) b値の終了値
	 * @param (isIncreaseFrame) フレームカウントを実行するか
	 * @return true: 終了判定
	 */
	bool Animation::update(int* nowColor, int r0, int g0, int b0, int r1, int g1, int b1, bool isIncreaseFrame)
	{
		bool isFin = false;

		// フレームカウント
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
	 * 現在値を基準にフレーム値を調整する
	 * @param (y) 現在値
	 * @param (y0) 初期値
	 * @param (y1) 終了値
	 */
	void Animation::adjustFrame(float y, float y0, float y1)
	{
		// 値が増加していくか(通常再生の場合に)
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
	 * 現在値を基準にフレーム値を調整する
	 * @param (nowShape) 現在サイズ
	 * @param (baseShape) 基準サイズ
	 * @param (scale) 変更倍率
	 */
	void Animation::adjustFrame(Shape nowShape, Shape baseShape, float scale)
	{
		adjustFrame((float)nowShape.w / (float)baseShape.w, 1.0, scale);
	}
}