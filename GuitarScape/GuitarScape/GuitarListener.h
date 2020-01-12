#pragma once
#include <SFML/Audio.hpp>
#include <windows.h>
#include <memory>
#include <iostream>

class GuitarListener
{
public:
	bool detect();

	sf::SoundBuffer listen() const;

private:
	std::unique_ptr<sf::SoundBufferRecorder> recorder;
};

inline bool GuitarListener::detect()
{
	if (!sf::SoundBufferRecorder::isAvailable())
	{
		return false;
	}

	recorder = std::make_unique<sf::SoundBufferRecorder>();


	std::cout << "[Available Recording Devices]\n";
	auto devices = recorder->getAvailableDevices();
	int i = 0;
	for(auto& device : devices)
	{
		std::cout << "[" << std::to_string(i) << "] " << device << ".\n";
		++i;
	}

	std::cout << "Please enter the number of the desired device." << std::endl;

	char c = '0';
	int chosen = -1;
	while (chosen == -1)
	{
		std::cin >> c;

		const int integer = c - '0';
		if (integer >= 0 && integer < devices.size())
		{
			chosen = integer;
		}
		else
		{
			std::cout << "That is not a valid device ID!" << std::endl;
			std::string throwaway;
			std::cin >> throwaway;
		}
	}

	std::cout << "You have chosen device #" << std::to_string(chosen) << "." << std::endl;

	recorder->setDevice(devices[chosen]);

	//Start recording
	recorder->start();

	return true;
}

inline sf::SoundBuffer GuitarListener::listen() const
{
	recorder->stop();

	//Copy the old sound buffer.
	const sf::SoundBuffer buffer = recorder->getBuffer();

	//Start recording the next one.
	recorder->start();

	return buffer;
}
