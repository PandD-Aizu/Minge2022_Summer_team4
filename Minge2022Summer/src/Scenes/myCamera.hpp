# pragma once

class BasicCamera
{
protected:
	Rect m_screen = Scene::Rect();	// 投影先の領域
	Vec2 m_center = Scene::Center();	// 切り取り領域の中心
	double m_scale = 1.0;				// 切り取り領域の拡大率

public:
	BasicCamera() = default;

	BasicCamera(const Vec2& center, double scale)
		: m_center(center)
		, m_scale(scale)
	{}

	// 描画対象の切り取り領域を返す
	[[nodiscard]] RectF getCameraRect() const { return RectF(m_screen.size / m_scale).setCenter(m_center); }
	[[nodiscard]] Mat3x2 getCursorMat3x2() const { return Mat3x2::Translate(-m_center).scaled(m_scale).translated(m_screen.center()); }
	[[nodiscard]] Mat3x2 getGraphics2DMat3x2() const { return Mat3x2::Translate(-m_center).scaled(m_scale).translated(m_screen.size / 2.0); }
	[[nodiscard]] ScopedViewport2D createScopedViewport() const { return ScopedViewport2D(m_screen); }
	[[nodiscard]] Transformer2D createTransformer() const { return Transformer2D(getGraphics2DMat3x2(), getCursorMat3x2()); }

	// 描画対象の切り取り領域を設定する
	void setCameraRect(const RectF& rect)
	{
		setCenter(rect.center());
		setScale(Max(Scene::Width() / rect.w, Scene::Height() / rect.h));
	}

	// 描画対象の切り取り領域の中心を設定する
	void setCenter(const Vec2& center) { m_center = center; }

	// 投影する領域を設定する
	void setScreen(const Rect& screen) { m_screen = screen; }

	void setScale(double scale) { m_scale = scale; }

	// 描画対象の切り取り領域の中心を返す
	const Vec2& getCenter() const noexcept { return m_center; }
	const Rect& getScreen() const noexcept { return m_screen; }

	double getScale() const noexcept { return m_scale; }
};

class MyCamera : public BasicCamera
{
	// 追従目標の値
	Vec2 m_targetCenter = Scene::Size() * 0.5;
	double m_targetScale = 1.0;

	// 追従速度 (0.0～1.0)
	double m_followingSpeed = 0.25;

	// 拡大の感度 (0.0～)
	double m_scalingSensitivity = 0.1;

	// 移動の速度 (dot per frame)
	double m_movingSensitivity = 0.02;

	// 画面外でもコントロールを有効にする
	bool m_controlOutOfScreenEnabled = false;



	void follow()
	{
		m_center = Math::Lerp(m_center, m_targetCenter, m_followingSpeed);
		m_scale = 1.0 / Math::Lerp(1.0 / m_scale, 1.0 / m_targetScale, m_followingSpeed);
	}

public:
	MyCamera() = default;

	void update(bool controlEnabled = true)
	{
		follow();
	}


	void setFollowingSpeed(double followingSpeed) noexcept { m_followingSpeed = followingSpeed; }
	void setMagnifyingSensitivity(double scalingSensitivity) noexcept { m_scalingSensitivity = scalingSensitivity; }
	void setMovingSensitivity(double movingSensitivity) noexcept { m_movingSensitivity = movingSensitivity; }

	void setTargetCenter(const Vec2& targetCenter) noexcept { m_targetCenter = targetCenter; }
	void setTargetScale(double targetScale) noexcept { m_targetScale = targetScale; }
	void setControlOutOfScreenEnabled(bool enabled) noexcept { m_controlOutOfScreenEnabled = enabled; }

	RectF getTargetCameraRect() const { return RectF(m_screen.size / m_targetScale).setCenter(m_targetCenter); }
};
