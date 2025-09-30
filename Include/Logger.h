#pragma once
#include <fstream>
#include <mutex>
#include <string>

class Logger
{
  public:
    static void Init(const std::wstring &filename = L"debug.log")
    {
        static bool initialized = false;
        if (!initialized)
        {
            logStream.open(filename, std::ios::trunc);
            initialized = true;
        }
    }

    static void Write(const std::wstring &msg)
    {
        std::lock_guard<std::mutex> lock(mutex);
        logStream << msg << L"\n";
        logStream.flush();
    }

    static void LogDeathsAndBorn(int died, int born, int &genCounter)
    {
        genCounter++;
        Logger::Write(L"Generation " + std::to_wstring(genCounter) + L": born = " + std::to_wstring(born) +
                      L", died = " + std::to_wstring(died));
    }

  private:
    static inline std::wofstream logStream;
    static inline std::mutex mutex;
};
