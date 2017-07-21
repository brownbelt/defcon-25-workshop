# defcon-25-workshop
Windows Post-Exploitation / Malware Forward Engineering

### Notes

All example code has been stripped down to barebones functionality for simplicity and demonstration purposes. As such, there may not be appropriate error checking.

### Catalog

1. asynclog - basic keylogger using GetAsyncKeyState()
2. asyncdebounce - adds debouncing to the basic keylogger
3. hooklog - keylogger using LowLevelKeyboardProc() callback
4. IGO - pre-main execution with C++ initialization
5. tlscallback - pre-main execution with Thread Local Storage callback
6. importless - PE using WinAPI that has no imports
7. printscreen - takes a screenshot by simulation of printscreen keypress
8. screenshot - takes a screenshot using device context and GDI+
9. reverseshell - basic reverse TCP shell
10. passfilter - password complexity filter DLL with logging
11. locklogger - injects into winlogon.exe and keylogs
12. puppetstrings - take a free ride into ring 0
13. ThreadContinue - injection using SetThreadContext() and NtContinue()
14. getsystem - gets system using Named Pipe impersonation
15. steamroll - brute forces login credentials
16. combrowser - using IE COM object to make web requests
17. httpbrowser - using HTTP API to make web requests
18. toxicserpent - log all network traffic, poison, port knock C2
19. RunShellcode - run shellcode from .NET
20. offsetfix - converting static analysis offsets with ASLR
21. rawhook - simple example showing function prologue hooking
22. wmiquery - shows how to look up AV using WMI

### Disclaimer
Code samples are provided for educational purposes. Adequate defenses can only be built by researching attack techniques available to malicious actors. Using this code against target systems without prior permission is illegal in most jurisdictions. The authors are not liable for any damages from misuse of this information or code.

### Acknowledgements
Special thanks to research done by the following individuals:

- [@subTee](https://twitter.com/subTee)
- [@enigma0x3](https://twitter.com/enigma0x3)
- [@tiraniddo](https://twitter.com/tiraniddo)
- [@harmj0y](https://twitter.com/harmj0y)
- [@gentilkiwi](https://twitter.com/gentilkiwi)
- [@mattifestation](https://twitter.com/mattifestation)
- [@aionescu](https://twitter.com/aionescu)
- clymb3r
- [@Aleph_\__Naught](https://twitter.com/Aleph___Naught)
- [@The_Naterz](https://twitter.com/The_Naterz)
- [@JennaMagius](https://twitter.com/JennaMagius)
- [@zerosum0x0](https://twitter.com/zerosum0x0)
