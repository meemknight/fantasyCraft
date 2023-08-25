#pragma once
#include <sstream>
#include <iostream>
#include <fstream>

//usage: llog(DefaultLog, "Loaded game", 1, 2, 3);
//usage: llog(ErrorLog, "Loaded game", 1, 2, 3);
//usage: llog(LogToAFile("test.txt"), "Loaded game", 1, 2, 3

class LogType
{
public:
	virtual void log(std::stringstream &ss) const = 0;
};


class DefaultLog : public LogType
{
public:
	void log(std::stringstream &ss) const override
	{
		std::cout << ss.str() << "\n";
	};
};

class ErrorLog: public LogType
{
public:
	void log(std::stringstream &ss) const override
	{
	#ifdef PLATFORM_WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		std::cout << ss.str() << "\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	#else
		std::cout << "Error: " << ss << "\n";
	#endif
	};
};

class LogToAFile: public LogType
{
	std::string name;
public:
	LogToAFile(std::string name):name(name)
	{}

	void log(std::stringstream &ss) const override
	{
		std::ofstream f(name);
		f << ss.str() << "\n";
		f.close();
	};

};


inline void llog(const LogType &l, std::stringstream &ss)
{
	l.log(ss);
}

template<class T, class ...T2>
inline void llog(const LogType &l, std::stringstream &ss, const T &t, const T2 &...t2)
{
	ss << t << " ";
	llog(l, ss, t2...);
}

template<class T, class ...T2>
inline void llog(const LogType &l, const T &t, const T2 &...t2)
{
	std::stringstream ss;
	ss << t << " ";
	llog(l, ss, t2...);
}
