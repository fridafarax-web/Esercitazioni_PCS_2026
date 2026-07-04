#pragma once

#include <chrono>
#include <stdexcept>

//Es 4/5

class timecounter
{
	std::chrono::high_resolution_clock::time_point start_time;
	bool running = false;

public:
	timecounter() {}

	void tic()
	{
		start_time = std::chrono::high_resolution_clock::now();
		running = true;
	}

	double toc()
	{
		if (!running)
			throw std::runtime_error("timecounter not running");

		auto end_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end_time - start_time;
		running = false;
		return elapsed.count();
	}
};
