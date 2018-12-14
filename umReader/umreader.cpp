#include "umreader.h"

#include <string>

std::fstream umreader::reader;
std::string umreader::pathStore;
std::string umreader::allText, umreader::allTextWithNewLines;
bool umreader::allTextTaken = false;

std::vector<std::string> umreader::allTextLineByLine;

std::string umreader::descBegin = "<d*", umreader::descEnd = "*d>", umreader::parBegin = "<p*", umreader::parEnd = "*p>";

void umreader::replace(std::string & str, const std::string & oldStr, const std::string & newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = str.find(oldStr, pos)) != std::string::npos)
	{
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

void umreader::eraseBeginEndSpaces(std::string & str)
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

std::vector<std::string> umreader::split(std::string whole, std::string splitter)
{
	std::regex regex(splitter);

	std::vector<std::string> result;

	std::sregex_token_iterator it(whole.begin(), whole.end(), regex, -1);
	std::sregex_token_iterator reg_end;

	for (; it != reg_end; ++it) {
		if (!it->str().empty()) //token could be empty:check
			result.emplace_back(it->str());
	}

	return result;

	//return std::vector<std::string>();
}

void umreader::changeParametersLowLevel(std::string parameterHeader, std::string value)
{
	getAll();

	std::string::size_type pos = 0u;
	std::string::size_type pos2 = 0u;
	std::string::size_type pos3 = 0u;
	std::string nameLcl;
	std::string description;

	while ((pos = allTextWithNewLines.find(parBegin, pos)) != std::string::npos)
	{
		pos2 = allTextWithNewLines.find(parEnd, pos);

		if (pos2 != std::string::npos)
		{
			nameLcl.clear();

			pos += parBegin.length() + 1; //Consider 1 line space
			nameLcl.append(allTextWithNewLines.begin() + pos, allTextWithNewLines.begin() + pos2 - 1);

			if (nameLcl == parameterHeader)
			{
				pos3 = allTextWithNewLines.find(parEnd, pos2 + parEnd.length());

				if (pos3 != std::string::npos)
				{
					allTextWithNewLines.erase(allTextWithNewLines.begin() + pos2 + parEnd.length() + 1, allTextWithNewLines.begin() + pos3 - 1);
					allTextWithNewLines.insert(allTextWithNewLines.begin() + pos2 + parEnd.length() + 1, value.begin(), value.end());
					break;
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			return; //WRONG FORMAT
		}
	}

	umreader::closeTxtFile();

	std::ofstream myfile;
	myfile.open(umreader::pathStore);
	myfile << allTextWithNewLines;
	myfile.close();
}

bool umreader::isNoMoreLine(std::string input)
{
	if (input == "<EOF>")
	{
		return true;
	}
	return false;
}

void umreader::trim(std::string & str, bool left, bool right)
{
	static const std::string delims = " \t\r\n";
	if (right)
		str.erase(str.find_last_not_of(delims) + 1); // trim right
	if (left)
		str.erase(0, str.find_first_not_of(delims)); // trim left
}

void umreader::openTxtFile(std::string path)
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


	reader.open(path, std::fstream::in);
	allTextTaken = false;

	pathStore = path;
}

void umreader::closeTxtFile()
{
	reader.close();
}

std::string umreader::ReadLine()
{
	if (!reader.is_open())
	{
		return "<FNF>"; //FAILED TO REACH DATA
	}
	if (!reader.eof())
	{
		std::string line;
		std::getline(reader, line);
		trim(line);
		return line;
	}
	else
	{
		return "<EOF>"; //NO MORE LINE
	}
}

std::string umreader::getAll()
{
	if (reader.is_open())
	{
		if (allTextTaken)
		{
			return allText;
		}

		//openTxtFile(pathStore);
		std::string all, allWLine;
		allTextLineByLine.clear();

		do
		{
			std::string line = ReadLine();

			if (isNoMoreLine(line))
			{
				break;
			}
			else
			{
				allTextLineByLine.push_back(line);
				all += line;
				allWLine += line + "\n";
			}
		} while (true);

		allText = all;
		allTextWithNewLines = allWLine;
		allTextTaken = true;

		return all;
	}
	else
	{
		return "";
	}
}

std::vector<std::string> umreader::getAllLines()
{
	getAll();
	return allTextLineByLine;
}

std::string umreader::getDescription(std::string name)
{
	getAll();

	std::string::size_type pos = 0u;
	std::string::size_type pos2 = 0u;
	std::string::size_type pos3 = 0u;
	std::string nameLcl;
	std::string description;

	while ((pos = allText.find(descBegin, pos)) != std::string::npos)
	{
		pos2 = allText.find(descEnd, pos);

		if (pos2 != std::string::npos)
		{
			nameLcl.clear();

			pos += descBegin.length() + 1; //Consider 1 line space
			nameLcl.append(allText.begin() + pos, allText.begin() + pos2 - 1);

			if (nameLcl == name)
			{
				pos3 = allText.find(descEnd, pos2 + descEnd.length());

				if (pos3 != std::string::npos)
				{
					description.append(allText.begin() + pos2 + descEnd.length() + 1, allText.begin() + pos3 - 1);
					return description;
				}
				else
				{
					return "WRONG FORMAT - 2";
				}
			}
		}
		else
		{
			return "WRONG FORMAT"; //WRONG FORMAT
		}
	}

	return "";
}

std::vector<std::string> umreader::getParameters(std::string name)
{
	getAll();

	std::string::size_type pos = 0u;
	std::string::size_type pos2 = 0u;
	std::string::size_type pos3 = 0u;
	std::string nameLcl;
	std::string parameters;

	while ((pos = allText.find(parBegin, pos)) != std::string::npos)
	{
		nameLcl.clear();
		pos2 = allText.find(parEnd, pos);

		if (pos2 != std::string::npos)
		{
			pos += parBegin.length() + 1; //Consider 1 line space
			nameLcl.append(allText.begin() + pos, allText.begin() + pos2 - 1);

			if (nameLcl == name)
			{
				pos3 = allText.find(parEnd, pos2 + parEnd.length());

				if (pos3 != std::string::npos)
				{
					parameters.append(allText.begin() + pos2 + parEnd.length(), allText.begin() + pos3 - 1);
					break;
				}
				else
				{
					return std::vector<std::string> { "WRONG FORMAT - 2" };
				}
			}
		}
		else
		{
			return std::vector<std::string> { "WRONG FORMAT" }; //WRONG FORMAT
		}
	}

	//replace(parameters, " ", "");
	std::vector<std::string> output = split(parameters, ",");
	for (size_t i = 0; i < output.size(); i++)
	{
		eraseBeginEndSpaces(output[i]);
	}

	return output;
}

void umreader::changeDescription(std::string descriptionHeader, std::string value)
{
	getAll();

	std::string::size_type pos = 0u;
	std::string::size_type pos2 = 0u;
	std::string::size_type pos3 = 0u;
	std::string nameLcl;
	std::string description;

	while ((pos = allTextWithNewLines.find(descBegin, pos)) != std::string::npos)
	{
		pos2 = allTextWithNewLines.find(descEnd, pos);

		if (pos2 != std::string::npos)
		{
			nameLcl.clear();

			pos += descBegin.length() + 1; //Consider 1 line space
			nameLcl.append(allTextWithNewLines.begin() + pos, allTextWithNewLines.begin() + pos2 - 1);

			if (nameLcl == descriptionHeader)
			{
				pos3 = allTextWithNewLines.find(descEnd, pos2 + descEnd.length());

				if (pos3 != std::string::npos)
				{
					allTextWithNewLines.erase(allTextWithNewLines.begin() + pos2 + descEnd.length() + 1, allTextWithNewLines.begin() + pos3 - 1);
					allTextWithNewLines.insert(allTextWithNewLines.begin() + pos2 + descEnd.length() + 1, value.begin(), value.end());
					break;
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			return; //WRONG FORMAT
		}
	}

	umreader::closeTxtFile();

	std::ofstream myfile;
	myfile.open(umreader::pathStore);
	myfile << allTextWithNewLines;
	myfile.close();
}

void umreader::changeParameters(std::string parameterHeader, std::vector<std::string> values)
{
	std::string valuesStr;

	for (size_t i = 0; i < values.size(); i++)
	{
		valuesStr += values[i];
		if (i != values.size() - 1) valuesStr += ", ";
	}

	changeParametersLowLevel(parameterHeader, valuesStr);
}

umreader::umreader()
{
}


umreader::~umreader()
{
}
