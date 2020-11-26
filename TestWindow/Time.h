#pragma once
#include <chrono>


class Time
{
public:

	std::chrono::system_clock::time_point GetTimeNow()
	{
		return std::chrono::system_clock::now();
	}

	template<typename T>
	T GetTimeDifference(std::chrono::system_clock::time_point start)
	{
		std::chrono::duration<T> diff = GetTimeNow() - start;
		return diff.count();
	}
};