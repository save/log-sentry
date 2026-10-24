#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

// yes i know the validation is a bit of a yeouch give me time
bool isIPv4(const std::string& token) {
    int dotCount = 0;
    for (char c : token) {
        if (c == '.') {
            dotCount++;
        }
        else if (!isdigit(c)) {
            return false;
        }
    }
}

std::unordered_map<std::string, int> analyzeLogFile(const std::string& filename) {
    std::ifstream file(filename);
    std::unordered_map<std::string, int> failedIpCounts;

    if (!file) {
        std::cerr << "[-] Failed to open " << filename << "\n";
        return failedIpCounts;
    }

    std::cout << "[+] Successfully opened: " << filename << "\n";

    for (std::string line; std::getline(file, line); ) {

        if (line.find("Failed") != std::string::npos) {
           std::istringstream stream(line);
           std::string token;

           while (stream >> token) {
               if (isIPv4(token)) {
                   failedIpCounts[token]++;
               }
           }

       
        }
        else if (line.find("sudo") != std::string::npos) {
			// do nothing - might add more logic here in the future if needed
        }
    }

    file.close();
    return failedIpCounts;
}

void generateReport(const std::unordered_map<std::string, int>& failedIpCounts, const std::string& reportFilename) {
    std::ofstream reportFile(reportFilename);

    if (!reportFile) {
        std::cerr << "[-] Failed to create " << reportFilename << "\n";
        return;
    }

    std::string title = "\nSummary of failed login attempts:\n";
    std::string border(title.length(), '-');
	std::cout << title << border << "\n";
    reportFile << title;

    for (const auto& pair : failedIpCounts) {
        std::string ip = pair.first;
        int count = pair.second;

        std::string summaryLine = "IP Address: " + ip + ", Failed Attempts: " + std::to_string(count);

        std::cout << summaryLine << "\n";
        reportFile << summaryLine << "\n";

        if (count >= 3) {
            std::string alertLine = "[-] ALERT: IP Address " + ip + " has " + std::to_string(count) + " failed login attempts.";
            std::cout << alertLine << "\n";
            reportFile << alertLine << "\n";
        }
    }

    std::cout << border;
    reportFile << border;

    reportFile.close();
    std::string reportMsg = "[+] Report generated successfully in ";
    std::cout << reportMsg << reportFilename << border << "\n";
}

int main(int argc, char* argv[]) {
    std::string filename = "logs.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    std::unordered_map<std::string, int> failedIpCounts = analyzeLogFile(filename);

    if (failedIpCounts.empty() && !std::ifstream(filename)) {
        return 1;
    }

    generateReport(failedIpCounts, "log_report.txt");

    return 0;
}
