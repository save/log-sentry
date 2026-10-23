#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, int> analyzeLogFile(const std::string& filename) {
    std::ifstream file(filename);
    std::unordered_map<std::string, int> failedIpCounts;

    if (!file) {
        std::cerr << "[-] Failed to open " << filename << "\n";
        return failedIpCounts;
    }

    std::cout << "[+] Successfully opened: " << filename << "\n";
    std::cout << "----------------------------------------\n";

    for (std::string line; std::getline(file, line); ) {
        std::cout << line << '\n';

        if (line.find("Failed") != std::string::npos) {
            size_t fromPos = line.find("from");
            if (fromPos != std::string::npos) {
                std::string ipAddress = line.substr(fromPos + 5);
                failedIpCounts[ipAddress]++;
                std::cout << "[-] Failed login attempt from: " << ipAddress << "\n";
            }
        }
        else if (line.find("sudo") != std::string::npos) {
            std::cout << "[+] Found a sudo log entry: " << line << "\n";
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

    std::cout << "\n===============================\n";
    std::cout << "Summary of failed login attempts:\n";

    reportFile << "Summary of failed login attempts:\n";

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

    std::cout << "===============================\n";
    reportFile << "===============================\n";

    reportFile.close();
    std::cout << "[+] Report generated successfully in " << reportFilename << "\n";
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