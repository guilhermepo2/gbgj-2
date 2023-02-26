#include "Monster.h"

Monster::Monster(gueepo::TextureRegion* monsterTexture, gueepo::math::vec2 position, int hp) : m_position(position), m_monsterTexture(monsterTexture), m_hp(hp), m_isPlayer(false) {}

Monster::Monster(gueepo::TextureRegion* monsterTexture, gueepo::math::vec2 position, int hp, bool isPlayer) : m_position(position), m_monsterTexture(monsterTexture), m_hp(hp), m_isPlayer(isPlayer) {}

void Monster::Move(int dx, int dy) {
	if (dx != 0) {
		m_isFacingRight = dx > 0;
	}

	m_position.x += dx;
	m_position.y += dy;

	// the offset is counted in pixels
	m_offset.x = -dx * 32;
	m_offset.y = -dy * 32;
}

void Monster::Attack(int dx, int dy) {
	m_offset.x = dx;
	m_offset.y = dy;
}

void Monster::TickOffset(float deltaTime) {
	if (m_offset.x == 0.0f && m_offset.y == 0.0f) {
		return;
	}

	m_offset.x -= gueepo::math::sign(m_offset.x) * deltaTime * 256;
	m_offset.y -= gueepo::math::sign(m_offset.y) * deltaTime * 256;

	if (gueepo::math::abs(m_offset.x) < 3.0f) {
		m_offset.x = 0.0f;
	}

	if (gueepo::math::abs(m_offset.y) < 3.0f) {
		m_offset.y = 0.0f;
	}
}

void Monster::DealDamage(int amount) {
	m_hp -= amount;
}

bool Monster::operator==(const Monster& rhs) const {
	return m_isFacingRight == rhs.m_isFacingRight &&
		m_position == rhs.m_position &&
		m_offset == rhs.m_offset &&
		m_hp == rhs.m_hp &&
		m_isPlayer == rhs.m_isPlayer &&
		m_monsterTexture == rhs.m_monsterTexture;
}

