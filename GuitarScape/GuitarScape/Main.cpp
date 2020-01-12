#include <SFML/Graphics.hpp>
#include "GuitarListener.h"
#include "Timer.h"
#include "Scaper.h"
#include "ThreadPool.h"

int main()
{
	const unsigned int measureInterval = 5;
	utilities::Timer timer;
	GuitarListener listener;
	bool audio = listener.detect();

	if(!audio)
	{
		std::cout << "Error! Cannot record audio!";
		getchar();
		return 0;
	}

	bool running = true;
	utilities::ThreadPool threadPool(5);

	while (running)
	{
		if (timer.measure(utilities::TimeUnit::MILLIS) > measureInterval)
		{
			threadPool.enqueue([buffer{listener.listen()}]()
			{
					Scaper::processData(buffer);
			});
			timer.reset();
		}
	}

	return 0;
}
