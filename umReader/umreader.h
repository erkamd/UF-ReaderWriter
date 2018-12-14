#include <fstream>
#include <vector>
#include <regex>

class umreader
{
private:
	static std::fstream reader;
	static std::string pathStore;
	static std::string allText, allTextWithNewLines;
	static std::vector<std::string> allTextLineByLine;
	static bool allTextTaken;

	static std::string descBegin, descEnd, parBegin, parEnd;

	//FORMAT EXAMPLE
	//for description: "<d* Something *d> Some description *d>"
	//for parameters: "<p* Something *p> Some, parameters, is, coming, here *p>
	static void replace(std::string &str, const std::string &oldStr, const std::string &newStr);

	static void eraseBeginEndSpaces(std::string &str);

	static std::vector<std::string> split(std::string whole, std::string splitter);

	static void changeParametersLowLevel(std::string parameterHeader, std::string value);

public:
	static bool isNoMoreLine(std::string input);

	static void trim(std::string& str, bool left = true, bool right = true);

	//WRITE PATH WITH ".TXT"
	static void openTxtFile(std::string path);
	static void closeTxtFile();
	static std::string ReadLine();
	static std::string getAll();

	static std::vector<std::string> getAllLines();

	static std::string getDescription(std::string name);
	static std::vector<std::string> getParameters(std::string name);

	static void changeDescription(std::string descriptionHeader, std::string value);
	static void changeParameters(std::string parameterHeader, std::vector<std::string> values);

	umreader();
	~umreader();
};

