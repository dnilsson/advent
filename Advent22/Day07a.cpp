#include "AdventUtil.h"

#include <iostream>
#include <unordered_map>

const std::string FILENAME = "Day07.txt";

const int DIRECTORY_SIZE_LIMIT = 100000;

enum LineType
{
	CdCommand,
	LsCommand,
	DirectoryEntry,
	FileEntry
};

struct ParsedLine
{
	ParsedLine(LineType type, const std::string& path, int size = 0) : type(type), path(path), size(size) {}
	
	LineType type;
	std::string path;	// Not for type LsCommand
	int size;			// Only for type FileEntry
};

std::vector<ParsedLine> Parse(const std::vector<std::string>& lines)
{
	std::vector<ParsedLine> result;
	
	for (const auto& line : lines)
	{
		auto s = AdventUtil::SplitString(line, ' ');
		if (s.size() == 2 && s[0] == "dir")
			result.emplace_back(DirectoryEntry, s[1]);
		else if (s.size() == 2 && s[0] == "$" && s[1] == "ls")
			result.emplace_back(LsCommand, "");
		else if (s.size() == 3 && s[0] == "$" && s[1] == "cd")
			result.emplace_back(CdCommand, s[2]);
		else if (s.size() == 2)
			result.emplace_back(FileEntry, s[1], std::atol(s[0].c_str()));
	}
	
	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////

class Directory
{
private:
	std::unordered_map<std::string, std::shared_ptr<Directory>> _subDirectories;
	std::unordered_map<std::string, int> _files;
	Directory* _parentDirectory;
	int _size = -1;
	
public:
	Directory()
		: _parentDirectory(nullptr)
	{
	}

	Directory(Directory& parentDirectory)
		: _parentDirectory(&parentDirectory)
	{
	}

	void AddSubDirectory(const std::string& name)
	{
		_subDirectories.insert({ name, std::make_shared<Directory>(*this) });
	}
	
	void AddFile(const std::string& name, int size)
	{
		_files.insert({ name, size });
	}
	
	Directory& GetSubDirectory(const std::string& name)
	{
		auto it = _subDirectories.find(name);
		if (it == _subDirectories.end())
			it = _subDirectories.insert({ name, std::make_shared<Directory>(*this) }).first;
		return *(it->second);
	}
	
	Directory& GetParentDirectory()
	{
		return *_parentDirectory;
	}
	
	int GetSize()
	{
		if (_size == -1)
		{
			_size = 0;
			for (const auto& entry : _subDirectories)
				_size += entry.second->GetSize();
			for (const auto& entry : _files)
				_size += entry.second;
		}
		return _size;
	}

	int GetSummedSizeOfSmallDirectories()
	{
		int result = 0;
		for (auto& entry : _subDirectories)
			result += entry.second->GetSummedSizeOfSmallDirectories();
		int size = GetSize();
		if (size <= DIRECTORY_SIZE_LIMIT)
			result += size;
		return result;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////

Directory BuildDirectoryTree(const std::vector<ParsedLine> lines)
{
	Directory rootDirectory;
	Directory* currentDirectory = &rootDirectory;

	for (const auto& line : lines)
	{
		switch (line.type)
		{
		case CdCommand:
			if (line.path == "/")
				currentDirectory = &rootDirectory;
			else if (line.path == "..")
				currentDirectory = &currentDirectory->GetParentDirectory();
			else
				currentDirectory = &currentDirectory->GetSubDirectory(line.path);
			break;

		case DirectoryEntry:
			currentDirectory->AddSubDirectory(line.path);
			break;

		case FileEntry:
			currentDirectory->AddFile(line.path, line.size);
			break;
			
		default:
			break;
		}
	}
	
	return rootDirectory;
}

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	auto parsedLines = Parse(lines);
	auto rootDirectory = BuildDirectoryTree(parsedLines);
	auto result = rootDirectory.GetSummedSizeOfSmallDirectories();
	std::cout << "Result: " << result << "\n";
	return 0;
}
