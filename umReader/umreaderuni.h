#pragma once

#include <fstream>
#include <vector>
#include <regex>


class umreaderuni
{
private:
	static std::wifstream reader;
	static std::wstring pathStore;
	static std::wstring allText;
	static std::vector<std::wstring> allTextLineByLine;
	static bool allTextTaken;

	static std::string descBegin, descEnd, parBegin, parEnd;

	//FORMAT EXAMPLE
	//for description: "<d* Something *d> Some description *d>"
	//for parameters: "<p* Something *p> Some, parameters, is, coming, here *p>
	static void replace(std::wstring &str, const std::wstring &oldStr, const std::wstring &newStr);

	static void eraseBeginEndSpaces(std::wstring &str);


public:
	static bool isNoMoreLine(std::wstring input);


	//WRITE PATH WITH ".TXT"
	static void openTxtFile(std::wstring path);
	static void closeTxtFile();
	static std::wstring ReadLine();
	static std::wstring getAll();

	static std::vector<std::wstring> getAllLines();
	static std::vector<std::wstring> getAllLines_Seperate(std::wstring path);
	static std::vector<std::wstring> getAllLines_Seperate(std::string path);

	static std::wstring stringTowstring(std::string input);


public:
	umreaderuni();
	~umreaderuni();
};

