# logsentry

a simple c++ log analyzer and brute-force detector (yes i know it can be better ill prolly make it decent soon)

---

reads a log file line-by-line, parses failed login attempts and sudo actions, tracks ip failure counts using hash maps, and exports a summary report.

## requirements

- c++17 compatible compiler (g++, clang, or msvc)

## build

```bash
g++ -std=c++17 main.cpp -o logsentry
```

# usage
```bash
./logsentry <logfile>
./logsentry logs.txt
```
if no file argument is provided, it defaults to looking for logs.txt in the current directory.

# output 
```bash
[+] Successfully opened: logs.txt

Summary of failed login attempts:
---------------------------------
IP Address: 192.168.1.105, Failed Attempts: 3
[-] ALERT: IP Address 192.168.1.105 has 3 failed login attempts.
---------------------------------
[+] Report generated successfully in log_report.txt
```
