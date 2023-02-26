#pragma once
#include <gueepo2d.h>

class Monster {
public:
	Monster(gueepo::TextureRegion* monsterTexture, gueepo::math::vec2 position, int hp);
	Monster(gueepo::TextureRegion* monsterTexture, gueepo::math::vec2 position, int hp, bool isPlayer);

	void Move(int dx, int dy);
	void Attack(int dx, int dy);
	void TickOffset(float deltaTime);
	void DealDamage(int amount);

	bool IsFacingRight() { return m_isFacingRight; }
	inline gueepo::math::vec2 GetPosition() const { return m_position; }
	void SetPosition(int x, int y) {
		m_position.x = static_cast<float>(x);
		m_position.y = static_cast<float>(y);
	}
	void SetPosition(gueepo::math::vec2 pos) {
		m_position = pos;
	}
	inline gueepo::math::vec2 GetOffset() const { return m_offset; }
	inline gueepo::math::vec2 GetRenderPosition() const { return m_position + m_offset; }

	inline int GetHP() const { return m_hp; }
	inline void SetHP(int value) { m_hp = value; }
	inline gueepo::TextureRegion* GetTexture() const { return m_monsterTexture; }
	inline bool IsPlayer() const { return m_isPlayer; }

	bool operator==(const Monster& rhs) const;

private:
	bool m_isFacingRight;
	gueepo::math::vec2 m_position;
	gueepo::math::vec2 m_offset;

	int m_hp;
	gueepo::TextureRegion* m_monsterTexture;
	bool m_isPlayer;
};