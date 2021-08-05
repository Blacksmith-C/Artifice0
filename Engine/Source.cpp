#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <sstream>

class RenderSettings
{
public:
	bool Watermark;
	bool Wireframe;
	int xres;
	int yres;
	sf::Color DefaultText;
};



int main()
{
	float Runtime = 0.0;

	float timeconstant = 0.00000000001;
	sf::SoundBuffer buffer;
	sf::Sound sound;
	const sf::Int16* samples;
	std::vector<sf::Int16> goodsamples;
	std::size_t count;
	std::vector<sf::Int16> wetsamples;
	int channels;
	int sampleRate;
	sf::SoundBuffer wetbuffer;
	sf::Sound sound2;
	bool lastplayed = true;
	float alpha = 0.0f;
	
	//SETTINGS
	RenderSettings ini;
	ini.Watermark = true;
	ini.Wireframe = true;
	ini.xres = 1920;
	ini.yres = 1080;
	ini.DefaultText.r = 200;
	ini.DefaultText.g = 200;
	ini.DefaultText.b = 200;
	ini.DefaultText.a = 200;
	
	//VERSION NUMBER
	sf::String versionno = "Artifice 0.110";
	sf::String watermark;
	sf::String sFPS;
	int FPS;

	//Initialize Text
	sf::Font CourierNew;
	CourierNew.loadFromFile("CourierNew.ttf");
	sf::Text watermarktext;
	watermarktext.setPosition(10.0f, 10.0f);
	watermarktext.setFont(CourierNew);
	watermarktext.setCharacterSize(25);
	watermarktext.setFillColor(ini.DefaultText);

	//Initialize Sound
	sf::SoundBufferRecorder recorder;
	recorder.start();
	bool Recording = true;

	
	//Open Window
	sf::RenderWindow mywindow(sf::VideoMode(ini.xres, ini.yres), versionno, sf::Style::Fullscreen);

	//Begin Measuring Time
	sf::Clock clock;

	//Define Window Background Color
	sf::Color sky(20, 20, 40, 255);

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
			if (event.type == sf::Event::TextEntered)
			{

			}
		}

		//Get Time Since Last Frame
		sf::Time delta = clock.restart();
		Runtime = Runtime + delta.asSeconds();

		//Reset Window to Blank Background
		mywindow.clear(sky);

		//Update Game Logic
		if (Runtime >= 5.0 && Recording == true) {
			recorder.stop();
			Recording = false;
			buffer = recorder.getBuffer();
			buffer.saveToFile("test.ogg");
			sound.setBuffer(buffer);

			samples = buffer.getSamples();
			count = buffer.getSampleCount();
			channels = buffer.getChannelCount();
			sampleRate = buffer.getSampleRate();

			wetsamples.reserve(count);

			float b = 1.0f - timeconstant;
			float z = 0;

			for (int n = 0; n < count; n++)
			{
				z = samples[n];
				wetsamples.push_back(z);
			}
			for (int n = 0; n < count; n++)
			{
				z = samples[n];
				goodsamples.push_back(z);
			}


			wetbuffer.loadFromSamples(&wetsamples[0], wetsamples.size(), channels, count);
			sound2.setBuffer(wetbuffer);
			sound.play();
		}

		if (!Recording) {
			
			for (int n = 1; n <= count; n++) {
				if ((n % 100) == 0) {
					sf::Vertex line[] = { sf::Vertex(sf::Vector2f(50 + n / 100,(goodsamples[n] / 50) + 400)), sf::Vector2f(50 + n / 100,400) };
					mywindow.draw(line, 2, sf::Lines);
					sf::Vertex line2[] = { sf::Vertex(sf::Vector2f(50 + n / 100,(wetsamples[n] / 50) + 800)), sf::Vector2f(50 + n / 100,800) };
					mywindow.draw(line2, 2, sf::Lines);
				}
			}
			
			if (sound.getStatus() != sf::Sound::Status::Playing && sound2.getStatus() != sf::Sound::Status::Playing) {
				if (lastplayed) {
					sound2.play();
					sound2.setPitch(0.05f);
					lastplayed = false;
				}
				else {
					sound.play();
					lastplayed = true;
				}
			}
		}

	
		if (ini.Watermark) {
			FPS = 1;
			FPS /= delta.asSeconds();
			sFPS = std::to_string(FPS);
			watermark = versionno;
			watermark += " (";
			std::string devicename = recorder.getDevice();
			watermark += " Device: )";
			watermark += devicename;
			watermark += " - )";
			watermark += sFPS;
			watermark += " FPS )";

			
			watermarktext.setString(watermark);
			mywindow.draw(watermarktext);
		}
		

		//Draw Frame to Window
		mywindow.display();
	}
	return 0;

}