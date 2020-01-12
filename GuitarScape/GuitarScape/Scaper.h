#pragma once
#include <SFML/Audio.hpp>
#include <cmath>
#include <numeric>

class Scaper
{
public:
	static void processData(sf::SoundBuffer buffer);

private:

	template<typename T>
	static void calcDevAndMean(std::vector<T>& vector, float& stdDev, float& mean);
};

inline void Scaper::processData(sf::SoundBuffer buffer)
{
	//Detect if new input is given.
	const sf::Int16* samples = buffer.getSamples();
	const std::size_t count = buffer.getSampleCount();

	//std::vector<unsigned int> input;
	//input.assign(samples, samples + count);

	////lag 5 for the smoothing functions
	//const int lag = 50;
	////3.5 standard deviations for signal
	//const float threshold = 3.5f;
	////between 0 and 1, where 1 is normal influence, 0.5 is half
	//const float influence = 0.3f;

	//if (count <= lag + 2)
	//{
	//	std::cout << "Warning! Scaper cannot analyze audio without enough samples. Need at least: " << std::to_string(lag + 3) << "." << std::endl;
	//}

	////Initialise variables
	//std::vector<int> signals(input.size(), 0.0);
	//std::vector<float> filteredY(input.size(), 0.0);
	//std::vector<float> avgFilter(input.size(), 0.0);
	//std::vector<float> stdFilter(input.size(), 0.0);
	//std::vector<unsigned int> subVecStart(input.begin(), input.begin() + lag);

	//float stdDev;
	//float mean;
	//calcDevAndMean(subVecStart, stdDev, mean);
	//avgFilter[lag] = mean;
	//stdFilter[lag] = stdDev;

	//for (size_t i = lag + 1; i < input.size(); i++)
	//{
	//	if (std::abs(input[i] - avgFilter[i - 1]) > threshold* stdFilter[i - 1])
	//	{
	//		if (input[i] > avgFilter[i - 1])
	//		{
	//			signals[i] = 1; //# Positive signal
	//		}
	//		else
	//		{
	//			signals[i] = -1; //# Negative signal
	//		}
	//		//Make influence lower
	//		filteredY[i] = influence * input[i] + (1 - influence) * filteredY[i - 1];
	//	}
	//	else
	//	{
	//		signals[i] = 0; //# No signal
	//		filteredY[i] = input[i];
	//	}
	//	//Adjust the filters
	//	std::vector<float> subVec(filteredY.begin() + i - lag, filteredY.begin() + i);

	//	float stdDev;
	//	float mean;
	//	calcDevAndMean(subVec, stdDev, mean);
	//	avgFilter[i] = mean;
	//	stdFilter[i] = stdDev;
	//}

	//int s = 0;
	//for(auto& signal : signals)
	//{
	//	if(signal != 0)
	//	{
	//		++s;
	//	}
	//}

	//std::cout << "Signals detected: " << s << std::endl;


	//int lowest = 99999999;
	//int highest = 0;

	//for(int i = 0; i < count; ++i)
	//{
	//	auto sample = samples[i];
	//	if(sample > 0)
	//	{
	//		if (sample < lowest) lowest = sample;
	//		if (sample > highest) highest = sample;
	//	}
	//}

	//int dif = abs(lowest - highest);

	//std::cout << "Highest: " << highest << ". Lowest: " << lowest << ". " << "Difference: " << dif << "." << std::endl;

	//The amount of samples per batch to filter out.
	const int batchSize = 50;
	const int multiplier = 10;
	const int cutOff = 200;

	//The growth required to detect a peak (between 0 and n). 1 = 100% growth.
	const float peakGrowThreshold = 100.f;

	int batches = std::ceilf(static_cast<float>(count) / 50.f);
	std::vector<int> batchPeaks(batches, 0.f);

	for(int i = 0; i < batches; ++i)
	{
		int index = i * batchSize;
		int last = index + batchSize - 1;
		last = (last > count ? count : last);

		for(int j = index; j < last; ++j)
		{
			auto sample = samples[j] * 2;
			if(sample > batchPeaks[i])
			{
				batchPeaks[i] = sample;
			}
		}
	}

	int peaks = 0;

	for(int i = 1; i < batches; ++i)
	{
		auto sample1 = batchPeaks[i - 1];
		auto sample2 = batchPeaks[i];

		if(sample2 > sample1 && sample2 - sample1 > cutOff)
		{
			auto dif = (static_cast<float>(sample2) / static_cast<float>(sample1)) - 1.f;

			if(dif > peakGrowThreshold)
			{
				++peaks;
			}
		}
	}

	std::cout << "Peaks detected: " << peaks << std::endl;






	//int maxDif = 0;
	//int minDif = 99999;

	//for(int i = 1; i < count; ++i)
	//{
	//	auto sample = samples[i];
	//	auto prev = samples[i - 1];

	//	auto dif = abs(sample - prev);
	//	if (dif > maxDif) maxDif = dif;
	//	if (dif < minDif) minDif = dif;


	//}

	//auto dif = abs(maxDif - minDif);
	//std::cout << "Highest Dif: " << maxDif << ". Lowest Dif: " << minDif << ". " << "Difference: " << dif << "." << std::endl;
}

template<typename T>
inline void Scaper::calcDevAndMean(std::vector<T>& vector, float& stdDev, float& mean)
{
	double sum = std::accumulate(std::begin(vector), std::end(vector), 0.0);
	mean = sum / vector.size();
	double accum = 0.0;
	std::for_each(std::begin(vector), std::end(vector), [&](const double d) {
		accum += (d - mean) * (d - mean);
		});

	stdDev = sqrt(accum / (vector.size() - 1));



}
