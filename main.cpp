#include<iostream>
#include<filesystem>
#include<vector>
#include<string>
#include<wchar.h>
#include<thread>

void coutAccessibleFile(std::vector<std::string> &paths) {
	FILE* pFile;
	for (size_t i = 0; i < paths.size(); i++) {
		std::filesystem::path entry = paths[i];
		_wfopen_s(&pFile, entry.wstring().c_str(), L"r");
		if (pFile != NULL) {
			fclose(pFile);
			if (std::filesystem::is_regular_file(entry))
				std::cout << paths[i] << " is_regular_file" << "\n";
		}
	}
}

int main(void) {
	std::vector<std::wstring> _filepaths;
	try {
		for (const auto& entry : std::filesystem::recursive_directory_iterator("C:\\", std::filesystem::directory_options::skip_permission_denied)) {
			_filepaths.push_back(entry.path().wstring());
		}
	}
	catch (std::filesystem::filesystem_error const &e) {
		std::cout << e.what() << "\n";
	}
  
	int hardwareCount = std::thread::hardware_concurrency();
	int workLoad = _filepaths.size() / hardwareCount;
	int restLoad = _filepaths.size() - (workLoad * hardwareCount);

	std::vector<std::string> *Splits = new std::vector<std::string>[hardwareCount];
	int control = 0;
	for (size_t i = 0; i < hardwareCount; i++) {
		if (i == hardwareCount - 1)
			workLoad += restLoad;
		for (size_t j = 0; j < workLoad; j++) { 
			std::string str(_filepaths[j+control].begin(), _filepaths[j+control].end());
			Splits[i].push_back(str);
		}
		control += workLoad;
	}

	for (size_t i = 0; i < hardwareCount; i++) {
		std::cout << Splits[i].size() << std::endl;
	}
  
	std::vector<std::thread> pool(hardwareCount - 1);
	for (int i = 0; i < hardwareCount - 1; i++) {
		pool[i] = std::thread(coutAccessibleFile, std::ref(Splits[i]));
	}
	for (auto & t : pool) {
		t.join();
	}

	delete[] Splits;
	system("cls");
	std::cout << "Exceptions using std::filesystem DEFEATED!!!" << std::endl;
	system("pause");
	return 0;
}
