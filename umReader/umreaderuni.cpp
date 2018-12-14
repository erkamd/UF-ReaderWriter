#include "umreaderuni.h"

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <fcntl.h>


std::wifstream umreaderuni::reader;
std::wstring umreaderuni::pathStore;
std::wstring umreaderuni::allText;
bool umreaderuni::allTextTaken = false;

std::vector<std::wstring> umreaderuni::allTextLineByLine;

std::string umreaderuni::descBegin = "<d*", umreaderuni::descEnd = "*d>", umreaderuni::parBegin = "<p*", umreaderuni::parEnd = "*p>";

void umreaderuni::replace(std::wstring & str, const std::wstring & oldStr, const std::wstring & newStr)
{
	std::wstring::size_type pos = 0u;
	while ((pos = str.find(oldStr, pos)) != std::wstring::npos)
	{
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

void umreaderuni::eraseBeginEndSpaces(std::wstring & str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == ' ')
		{
			str.erase(str.begin() + i);
			i--;
		}
		else
		{
			break;
		}
	}

	for (int i = str.size() - 1; i >= 0; i--)
	{
		if (str[i] == ' ')
		{
			str.erase(str.begin() + i);
			i++;
		}
		else
		{
			break;
		}
	}
}

bool umreaderuni::isNoMoreLine(std::wstring input)
{
	if (input == L"<EOF>")
	{
		return true;
	}
	return false;
}

void umreaderuni::openTxtFile(std::wstring path)
{
	if (reader.is_open())
	{
		if (pathStore == path)
		{
			return;
		}
		else
		{
			reader.close();
		}
	}
#ifdef __linux__
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::string converted = converter.to_bytes(path);

	reader.open(converted, std::fstream::in);
#else
	reader.open(path, std::fstream::in);
#endif

	allTextTaken = false;

	pathStore = path;
}

void umreaderuni::closeTxtFile()
{
	reader.close();
}

std::wstring umreaderuni::ReadLine()
{
	if (!reader.is_open())
	{
		return L"<FNF>"; //FAILED TO REACH DATA
	}
	if (!reader.eof())
	{
		std::wstring line;
		wchar_t chr[1000];

		reader.getline(chr, 1000);

		line = chr;

		return line;
	}
	else
	{
		return L"<EOF>"; //NO MORE LINE
	}
}

std::wstring umreaderuni::getAll()
{
	if (reader.is_open())
	{
		if (allTextTaken)
		{
			return allText;
		}

		//openTxtFile(pathStore);
		std::wstring all;
		allTextLineByLine.clear();

		do
		{
			std::wstring line = ReadLine();

			if (isNoMoreLine(line))
			{
				break;
			}
			else
			{
				allTextLineByLine.push_back(line);
				all += line;
			}
		} while (true);

		allText = all;
		allTextTaken = true;

		return all;
	}
	else
	{
		return L"";
	}
}

std::vector<std::wstring> umreaderuni::getAllLines()
{
	getAll();
	return allTextLineByLine;
}

std::vector<std::wstring> umreaderuni::getAllLines_Seperate(std::wstring path)
{
	using namespace std;
	vector<wstring> list;

	reader.close();
	wstring txtline;
	int c = 0;

#ifdef __linux__
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::string converted = converter.to_bytes(path);

	reader.open(converted, std::fstream::in);
#else
	reader.open(path, std::fstream::in);
#endif

	if (!reader.is_open())
	{
		//std::wcerr << L"Unable to open file" << std::endl;
		return list;
	}
	// We are going to read an UTF-8 file
	reader.imbue(locale(reader.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	while (getline(reader, txtline))
	{

		list.push_back(txtline);
	}

#ifdef __linux__
	for (size_t i = 0; i < list.size(); i++)
	{
		if (list[i].back() == L'\r')
		{
			list[i].pop_back();
		}
	}
#endif

	return list;
}

std::vector<std::wstring> umreaderuni::getAllLines_Seperate(std::string path)
{
	return getAllLines_Seperate(stringTowstring(path));
}

std::wstring umreaderuni::stringTowstring(std::string input)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(input);
	input.pop_back();

	return wide;
}

umreaderuni::umreaderuni()
{
}

umreaderuni::~umreaderuni()
{
}
