# include "Player.hpp"

void Player::drawWalk() const {
	// 歩行のアニメーションのインデックス(x, y)
	Vec2 animationIndex{ 0, 0 };

	// 横方向のインデックス（歩行モーション）
	if (characterDirection == 4) animationIndex.x = 0;
	else animationIndex.x = static_cast<int32>(Scene::Time() * 10 / 2) % 6;

	// 縦方向のインデックス（プレイヤーの向き）
	// プレイヤーが移動中でない場合は、最後に向いていた方向を使用する
	if (characterDirection == 4) animationIndex.y = lastCharacterDirection;
	else animationIndex.y = characterDirection;

	// 描画
	CharacterTexture((characterTextureSize.x * animationIndex.x), (characterTextureSize.y * animationIndex.y), characterTextureSize.x, characterTextureSize.y)
		.draw(
			  characterPosition.x - characterTextureCenter.x,
					characterPosition.y - characterTextureCenter.y
		);
}

