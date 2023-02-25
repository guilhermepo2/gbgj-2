#define GUEEPO2D_MAIN
#include <gueepo2d.h>

#include "Tile.h"
#include "Dungeon.h"

static gueepo::FontSprite* dogicaPixel = nullptr;
static gueepo::Texture* mainTexture = nullptr;
static gueepo::TextureRegion* portrait = nullptr;

static gueepo::TextureRegion* passable = nullptr;
static gueepo::TextureRegion* unpassable = nullptr;

static struct {
	int x;
	int y;

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

void GBGJ2::Application_OnInitialize() {
	m_Camera = new gueepo::OrtographicCamera(640, 360);
	m_Camera->SetPosition(gueepo::math::vec3(0.2f, -0.75f, 0.0f));
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

	// Hero Idle Animation
	heroData.heroIdle.AddAnimationFrame(new gueepo::TextureRegion(mainTexture, 0, 176, 16, 16),  0.4f);
	heroData.heroIdle.AddAnimationFrame(new gueepo::TextureRegion(mainTexture, 16, 176, 16, 16), 0.2f);
	heroData.heroIdle.AddAnimationFrame(new gueepo::TextureRegion(mainTexture, 32, 176, 16, 16), 0.4f);
	heroData.heroIdle.AddAnimationFrame(new gueepo::TextureRegion(mainTexture, 48, 176, 16, 16), 0.2f);
	heroData.x = 2;
	heroData.y = -2;

	// Dungeon
	Dungeon::Initialize();
}

void GBGJ2::Application_OnDeinitialize() {
	delete m_Camera;
	delete dogicaPixel;
}

void GBGJ2::Application_OnUpdate(float DeltaTime) {
	gueepo::SpriteAnimation_Update(heroData.heroIdle, DeltaTime);
}

void GBGJ2::Application_OnInput(const gueepo::InputState& currentInputState) {
	unreferenced(currentInputState);
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

			if (t->isPassable) {
				gueepo::Renderer::Draw(passable, t->x * DungeonSpriteSize, t->y * DungeonSpriteSize, DungeonSpriteSize, DungeonSpriteSize);
			}
			else {
				gueepo::Renderer::Draw(unpassable, t->x * DungeonSpriteSize, t->y * DungeonSpriteSize, DungeonSpriteSize, DungeonSpriteSize);
			}

		}
	}

	gueepo::Renderer::Draw(
		heroData.heroIdle.GetCurrentFrameTextureRegion(), 
		heroData.x * DungeonSpriteSize, heroData.y * DungeonSpriteSize, DungeonSpriteSize, DungeonSpriteSize
	);

	/*
	gueepo::Renderer::Draw(portrait, -250, -125, 64, 64);
	gueepo::Renderer::DrawString(
		dogicaPixel, "Alright, I will do that.\nTomorrow.", gueepo::math::vec2(-200.0f, -110.0f), 1.0f, gueepo::Color(1.0f, 1.0f, 1.0f, 1.0f)
	);
	*/
	gueepo::Renderer::EndFrame();
}

void GBGJ2::Application_OnEvent(gueepo::Event& e) {
	unreferenced(e);
}

gueepo::Application* gueepo::CreateApplication() {
	return new GBGJ2();
}
