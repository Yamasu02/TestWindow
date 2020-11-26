#pragma once

#include <fstream>
#include <string>
#include <optional>

class Logger
{
public:

	std::fstream file;
	int fileSize;
	struct Errors
	{
		int OpenFailed = 0;

	}Errors;

public:

	Logger(const char* FileName, bool DeleteOld = false)
	{
		try
		{
			DeleteOld ? file.open(FileName, std::ios::in | std::ios::out | std::ios::trunc) : file.open(FileName, std::ios::in | std::ios::out | std::ios::ate | std::ios::app);
			if (!file.is_open())
			{
				throw Errors.OpenFailed;
			}
			GetFileSize(fileSize);
		}
		catch (int& err)
		{
			exit(-1);
		}
	}


	bool GetFileSize(int& size)
	{
		if (file.is_open())
		{
			SetInputPos(std::ios::end, 0);
			size = GetInputPos();
			SetInputPos(std::ios::beg, 0);
			return true;
		}
		return false;
	}


	bool SetInputPos(std::ios_base::seekdir direction, int index = 0)
	{
		file.seekg(index, direction);
		return file.good();
	}


	std::streampos GetInputPos()
	{
		return file.tellg();
	}


	bool SetOutputPos(std::ios_base::seekdir direction, int index = 0)
	{
		file.seekp(index, direction);
		return file.good();
	}


	std::streampos GetOutputPos()
	{
		return file.tellp();
	}


	bool ReachedEnd()
	{
		return file.eof();
	}


	std::optional<std::pair<const std::string, const int>> ReadAllContent()
	{
		if (file.is_open())
		{
			std::string str;
			SetInputPos(std::ios::beg, 0);
			while (!ReachedEnd())
			{
				str.push_back(file.get());
			}
			str.pop_back();
			file.clear();
			return std::make_pair(str, str.size());
		}
		return std::nullopt;
	}


	std::optional<std::pair<const std::string, const int>> ReadLine(int index = 0)
	{
		if (file.is_open())
		{
			std::string str;
			SetInputPos(std::ios::beg, 0);
			for (int x = 0; x <= index; x++)
			{
				std::getline(file, str);
			}
			return std::make_pair(str, str.size());
		}
		return std::nullopt;
	}


	void WriteLine(const char* str, bool endl)
	{
		if (file.is_open())
		{
			file << str;
			if (endl) file << '\n';
		}
	}


	void FlushBuffer()
	{
		file.flush();
	}


	bool IsAllGood()
	{
		return file.good();
	}


	bool compareString(std::string str2Compare1, std::string str2Compare2)
	{
		return !str2Compare1.compare(str2Compare2);
	}


	~Logger()
	{
		file.close();
	}
};
