#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <sstream>

struct RenderSettings
{
	bool Watermark;
	bool Wireframe;
	int xres;
	int yres;
	int depth;
	sf::Color DefaultText;
	uint32_t WindowMode;
};



int main()
{
	float Runtime = 0.0;
	
	//SETTINGS
	RenderSettings ini;
	ini.Watermark = true;
	ini.Wireframe = true;
	ini.xres = 1920;
	ini.yres = 1080;
	ini.depth = 32;
	ini.DefaultText.r = 230;
	ini.DefaultText.g = 210;
	ini.DefaultText.b = 210;
	ini.DefaultText.a = 200;
	ini.WindowMode = sf::Style::Fullscreen;
	
	//VERSION NUMBER
	sf::String versionno = "HEAT DEATH 0.00";
	sf::String watermark;
	sf::String sFPS;
	int FPS;

	//Seed Random Number Generator
	srand(time(NULL));

	//Initialize Text
	sf::Font CourierNew;
	CourierNew.loadFromFile("CourierNew.ttf");
	sf::Text watermarktext;
	watermarktext.setPosition(10.0f, 10.0f);
	watermarktext.setFont(CourierNew);
	watermarktext.setCharacterSize(25);
	watermarktext.setFillColor(ini.DefaultText);

	//Initialize Sound

	//Define Monochrome Tint
	sf::Color heat(230, 210, 210, 20);

	//Initialize Background Static
	sf::Image backim;
	backim.create(ini.xres, ini.yres, heat);
	sf::Texture backtex;
	sf::Sprite background;
	backtex.loadFromImage(backim);
	background.setTexture(backtex);

	//Initialize Gameplay
	float Shipx = 0;
	float Shipy = 0;
	float Shiprot = 0;
	sf::Texture Shiptex;
	Shiptex.loadFromFile("Ship.png");
	sf::Sprite Ship;
	Ship.setTexture(Shiptex);
	Ship.setPosition(ini.xres/2,ini.yres/2);
	Ship.setRotation(Shiprot);
	Ship.setOrigin(219,50);
	Ship.setScale(0.75f,0.75f);

	//Open Window
	sf::RenderWindow mywindow(sf::VideoMode(ini.xres, ini.yres), versionno, ini.WindowMode);

	//Begin Measuring Time
	sf::Clock clock;



	//Run Program Until User Closes Window
	while (mywindow.isOpen())
	{
		//Check All New Events
		sf::Event event;
		while (mywindow.pollEvent(event))
		{
			//If an event is type "Closed", close the window
			if (event.type == sf::Event::Closed)
			{
				mywindow.close();
			}
		}

		//Get Time Since Last Frame
		sf::Time delta = clock.restart();
		Runtime = Runtime + delta.asSeconds();

		//Reset Window to Blank Background
		mywindow.clear(sf::Color::Black);
		for (int i = 0; i < ini.xres; i++) {
			for (int j = 0; j < ini.yres; j++) {
				backim.setPixel(i, j, sf::Color::Color(230,210,210,rand() % 51));
			}
		}
		backtex.update(backim);
		mywindow.draw(background);

		//Update Game Logic
		mywindow.draw(Ship);
		Shiprot += delta.asSeconds()*0.1;
		Ship.setRotation(Shiprot);

		//Draw Watermark
		if (ini.Watermark) {
			FPS = 1;
			FPS /= delta.asSeconds();
			sFPS = std::to_string(FPS);
			watermark = versionno;
			watermark += " (";
			watermark += sFPS;
			watermark += " FPS)";

			
			watermarktext.setString(watermark);
			mywindow.draw(watermarktext);
		}
		

		//Draw Frame to Window
		mywindow.display();
	}
	return 0;

}