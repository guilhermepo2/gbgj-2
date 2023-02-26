#define GUEEPO2D_MAIN
#include <gueepo2d.h>

#include "Tile.h"
#include "Dungeon.h"
#include "Monster.h"

static gueepo::FontSprite* dogicaPixel = nullptr;
static gueepo::Texture* mainTexture = nullptr;
static gueepo::TextureRegion* portrait = nullptr;

static gueepo::TextureRegion* passable = nullptr;
static gueepo::TextureRegion* unpassable = nullptr;
static gueepo::TextureRegion* playerPortal = nullptr;

static Monster* hero;


static struct {
	gueepo::SpriteAnimation heroIdle;
} heroData;

class GBGJ2 : public gueepo::Application {
public:
    GBGJ2() : Application("goblin bunker!!", 1280, 720) {}
	~GBGJ2() {}

	virtual void Application_OnInitialize() override;
	virtual void Application_OnDeinitialize() override;
	virtual void Application_OnUpdate(float DeltaTime) override;
	virtual void Application_OnInput(const gueepo::InputState& currentInputState) override;
	virtual void Application_OnRender() override;
	virtual void Application_OnEvent(gueepo::Event& e) override;

private:
	gueepo::OrtographicCamera* m_Camera;
};

static void LoadLevel(int level) {
	gueepo::json levels("./assets/levels.json");

	if (levels.IsValid()) {
		gueepo::json levelsArray;
		levels.GetArray("levels", levelsArray);

		if (levelsArray.IsArray()) {
			gueepo::json firstLevel;
			levelsArray.GetObjectInArray(level, firstLevel);
			Dungeon::InitializeFromJson(firstLevel);
		}
	}
}

void GBGJ2::Application_OnInitialize() {
	m_Camera = new gueepo::OrtographicCamera(640, 360);
	m_Camera->SetPosition(gueepo::math::vec3(0.2f, 0.55f, 0.0f));
	m_Camera->SetBackgroundColor(0.1f, 0.1f, 0.1f, 1.0f);

	gueepo::Font* dogicaPixelFontFile = gueepo::Font::CreateNewFont("./assets/font/dogicapixelbold.ttf");

	if (dogicaPixelFontFile != nullptr) {
		dogicaPixel = new gueepo::FontSprite(dogicaPixelFontFile, 8);
		dogicaPixel->SetLineGap(8.0f);
	}

	mainTexture = gueepo::Texture::Create("./assets/sheet.png");
	portrait = new gueepo::TextureRegion(mainTexture, 0, 0, 32, 32);
	passable = new gueepo::TextureRegion(mainTexture, 176, 160, 16, 16);
	unpassable = new gueepo::TextureRegion(mainTexture, 176, 144, 16, 16);
	playerPortal = new gueepo::TextureRegion(mainTexture, 160, 160, 16, 16);

	// Hero Idle Animation
	heroData.heroIdle.AddAnimationFrame(new gueepo::TextureRegion(mainTexture, 0, 176, 16, 16),  0.4f);
	heroData.heroIdle.AddAnimationFrame(new gueepo::TextureRegion(mainTexture, 16, 176, 16, 16), 0.2f);
	heroData.heroIdle.AddAnimationFrame(new gueepo::TextureRegion(mainTexture, 32, 176, 16, 16), 0.4f);
	heroData.heroIdle.AddAnimationFrame(new gueepo::TextureRegion(mainTexture, 48, 176, 16, 16), 0.2f);
	hero = new Monster(passable, gueepo::math::vec2(2.0f, 2.0f), 3, true);

	//
	LoadLevel(1);
	hero->SetPosition(Dungeon::GetCurrentPlayerStartPosition());
}

void GBGJ2::Application_OnDeinitialize() {
	delete m_Camera;
	delete dogicaPixel;
}

void GBGJ2::Application_OnUpdate(float DeltaTime) {
	gueepo::SpriteAnimation_Update(heroData.heroIdle, DeltaTime);

	// Handling Offset
	hero->TickOffset(DeltaTime);
}

void GBGJ2::Application_OnInput(const gueepo::InputState& currentInputState) {
	int heroMoveX = 0;
	int heroMoveY = 0;

	if (currentInputState.Keyboard.WasKeyPressedThisFrame(gueepo::Keycode::KEYCODE_W)) {
		heroMoveY = 1;
	}
	else if (currentInputState.Keyboard.WasKeyPressedThisFrame(gueepo::Keycode::KEYCODE_S)) {
		heroMoveY = -1;
	}
	else if (currentInputState.Keyboard.WasKeyPressedThisFrame(gueepo::Keycode::KEYCODE_D)) {
		heroMoveX = 1;
	}
	else if (currentInputState.Keyboard.WasKeyPressedThisFrame(gueepo::Keycode::KEYCODE_A)) {
		heroMoveX = -1;
	}

	// making sure the player has moved before going on about changing its positions and offsets
	if (heroMoveX != 0 || heroMoveY != 0) {
		int desiredX = hero->GetPosition().x + heroMoveX;
		int desiredY = hero->GetPosition().y + heroMoveY;
		if (Dungeon::IsPassable(desiredX, desiredY)) {
			hero->Move(heroMoveX, heroMoveY);
		}
	}
}

void GBGJ2::Application_OnRender() {
	gueepo::Color bgColor = m_Camera->GetBackGroundColor();
	gueepo::Renderer::BeginFrame(*m_Camera);
	gueepo::Renderer::Clear(bgColor.rgba);

	// Drawing the Dungeon
	const int DungeonSpriteSize = 32;
	for (int x = 0; x < DUNGEON_DIMENSION; x++) {
		for (int y = 0; y < DUNGEON_DIMENSION; y++) {
			Tile* t = Dungeon::GetTile(x, y);

			if (t->isPlayerPortal) {
				gueepo::Renderer::Draw(playerPortal, t->x * DungeonSpriteSize, t->y * DungeonSpriteSize, DungeonSpriteSize, DungeonSpriteSize);
			} else if (t->isPassable) {
				gueepo::Renderer::Draw(passable, t->x * DungeonSpriteSize, t->y * DungeonSpriteSize, DungeonSpriteSize, DungeonSpriteSize);
			}
			else {
				gueepo::Renderer::Draw(unpassable, t->x * DungeonSpriteSize, t->y * DungeonSpriteSize, DungeonSpriteSize, DungeonSpriteSize);
			}

		}
	}

	//
	const int HeroFacingSign = hero->IsFacingRight() ? 1 : -1;
	int offsetX = hero->GetOffset().x;
	int offsetY = hero->GetOffset().y;
	gueepo::Renderer::Draw(
		heroData.heroIdle.GetCurrentFrameTextureRegion(), 
		(hero->GetPosition().x * DungeonSpriteSize) + offsetX,
		(hero->GetPosition().y * DungeonSpriteSize) + offsetY + 8,
		HeroFacingSign * DungeonSpriteSize, DungeonSpriteSize
	);

	gueepo::Renderer::Draw(portrait, -200, -32, 64, 64);
	gueepo::Renderer::DrawString(
		dogicaPixel, "HUD Stuff", gueepo::math::vec2(-160.0f, -10.0f), 1.0f, gueepo::Color(1.0f, 1.0f, 1.0f, 1.0f)
	);
	gueepo::Renderer::DrawString(
		dogicaPixel, "HUD Stuff", gueepo::math::vec2(-160.0f, -26.0f), 1.0f, gueepo::Color(1.0f, 1.0f, 1.0f, 1.0f)
	);
	gueepo::Renderer::DrawString(
		dogicaPixel, "HUD Stuff", gueepo::math::vec2(-160.0f, -42.0f), 1.0f, gueepo::Color(1.0f, 1.0f, 1.0f, 1.0f)
	);
	gueepo::Renderer::DrawString(
		dogicaPixel, "HUD Stuff", gueepo::math::vec2(-160.0f, -58.0f), 1.0f, gueepo::Color(1.0f, 1.0f, 1.0f, 1.0f)
	);
	gueepo::Renderer::DrawString(
		dogicaPixel, "Sample Dialogue Here.", gueepo::math::vec2(-232.0f, 16.0f), 1.0f, gueepo::Color(1.0f, 1.0f, 1.0f, 1.0f)
	);
	gueepo::Renderer::EndFrame();
}

void GBGJ2::Application_OnEvent(gueepo::Event& e) {
	unreferenced(e);
}

gueepo::Application* gueepo::CreateApplication() {
	return new GBGJ2();
}
