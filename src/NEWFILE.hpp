#include <filesystem>
#include <fstream>
#include "MainLibraries/main.hpp"
class NEWFILE {
public:
    NEWFILE() {}

    void createNewFile(int howMany = 5, bool byTheLoop = true) {
        if (byTheLoop) {
            for (int i = 0; i < howMany; i++) {

                std::string path = "assets/JSON/save" + std::to_string(i) + ".json";

                // --- SKIP if file already exists ---
                if (std::filesystem::exists(path)) {
                    std::cout << "Skipping (already exists): " << path << "\n";
                    continue;
                }

                // --- Create file only if not exists ---
                std::ofstream file(path);
                if (!file.is_open()) {
                    std::cout << "Failed to create: " << path << "\n";
                    continue;
                }

                file << "{}";
                file.close();

                std::cout << "Created: " << path << "\n";
            }
        }
    }
};
