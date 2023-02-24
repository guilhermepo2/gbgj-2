#define GUEEPO2D_MAIN
#include <gueepo2d.h>

class GBGJ2 : public gueepo::Application {
public:
    GBGJ2() : Application("goblin bunker!!", 640, 360) {}
	~GBGJ2() {}
};

gueepo::Application* gueepo::CreateApplication() {
	return new GBGJ2();
}
