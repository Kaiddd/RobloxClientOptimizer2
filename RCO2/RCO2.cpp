#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <thread>
#include "curl/curl.h"

using std::string;

bool isConsoleHidden = false;
bool isRcoEnabled = false;

std::string rootDir("C:\\RClientOptimizer2");
string robloxVersionFolder;

char* buf = nullptr;
size_t sz = 0;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HWND consoleWindow = GetConsoleWindow();

//Tray icon code from stackoverflow and various other internet sources, I was unsure on how it worked.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LPCWSTR lpszClass = L"__hidden__";

int traySystem() {
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASS wc;
    HWND hWnd;
    MSG msg;

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = nullptr;
    wc.hCursor = nullptr;
    wc.hIcon = nullptr;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = lpszClass;
    wc.lpszMenuName = nullptr;
    wc.style = 0;
    RegisterClass(&wc);

    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static NOTIFYICONDATA nid;
    std::ofstream isHiddenFile;

    switch (iMsg) {
        case WM_CREATE:
            std::memset(&nid, 0, sizeof(nid));
            nid.cbSize = sizeof(nid);
            nid.hWnd = hWnd;
            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
            nid.uCallbackMessage = WM_APP + 1;
            nid.hIcon = (HICON)LoadImageA(NULL, (rootDir + "\\animegirl.ico").c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
            memcpy_s(nid.szTip, sizeof(nid.szTip), L"RCO <3\nClick to toggle console", sizeof(wchar_t[31]));
            Shell_NotifyIcon(NIM_ADD, &nid);
            return 0;
        case WM_APP + 1:
            switch (lParam) {
            case WM_LBUTTONDBLCLK:
                if (isConsoleHidden) {
                    ShowWindow(consoleWindow, SW_SHOW);
                    isConsoleHidden = false;
                    isHiddenFile.open(rootDir + "\\isHidden.rco");
                    isHiddenFile << "f";
                    isHiddenFile.close();
                } else {
                    ShowWindow(consoleWindow, SW_HIDE);
                    isConsoleHidden = true;
                    isHiddenFile.open(rootDir + "\\isHidden.rco");
                    isHiddenFile << "t";
                    isHiddenFile.close();
                }
                break;
            case WM_LBUTTONDOWN:
                if (isConsoleHidden) {
                    ShowWindow(consoleWindow, SW_SHOW);
                    isConsoleHidden = false;
                    isHiddenFile.open(rootDir + "\\isHidden.rco");
                    isHiddenFile << "f";
                    isHiddenFile.close();
                } else {
                    ShowWindow(consoleWindow, SW_HIDE);
                    isConsoleHidden = true;
                    isHiddenFile.open(rootDir + "\\isHidden.rco");
                    isHiddenFile << "t";
                    isHiddenFile.close();
                }
                break;
            }
            break;
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void printMainText() {
    system("cls");
    std::cout << "Roblox Client Optimizer 2 coded in C++ by Kaid | fflag list by nul\n\nRCO is currently: ";
    if (isRcoEnabled) {
        SetConsoleTextAttribute(hConsole, 10);
        std::cout << "Enabled";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "\nPress enter to ";
        SetConsoleTextAttribute(hConsole, 12);
        std::cout << "Disable";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << " RCO.\n\n";
    }
    else {
        SetConsoleTextAttribute(hConsole, 12);
        std::cout << "Disabled";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "\nPress enter to ";
        SetConsoleTextAttribute(hConsole, 10);
        std::cout << "Enable";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << " RCO.\n\n";
    }

    SetConsoleTextAttribute(hConsole, 6);
    std::cout << "This window can be hidden via the RCO tray icon!\nYou can ";
    SetConsoleTextAttribute(hConsole, 12);
    std::cout << "close";
    SetConsoleTextAttribute(hConsole, 6);
    std::cout << " RCO with ALT+F4 or any other similar method.\n";
    SetConsoleTextAttribute(hConsole, 7);
}

static auto WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) -> size_t { //Thank you stackoverflow... (https://stackoverflow.com/a/60516083)
    static_cast<string*>(userdata)->append(ptr, size * nmemb);
    return size * nmemb;
}

void mainThread() {
    while (true) {
        while (!isRcoEnabled) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }

        //Bit of error checking
        std::string robloxVersionStr;
        CURL* req = curl_easy_init();
        CURLcode res;
        curl_easy_setopt(req, CURLOPT_URL, "http://setup.roblox.com/version");
        curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(req, CURLOPT_WRITEDATA, &robloxVersionStr);
        res = curl_easy_perform(req);
        if (res != CURLE_OK) {
            std::cout << "\nNETWORK ERROR | PLEASE CHECK YOUR INTERNET CONNECTION | TRYING AGAIN IN 30 SECONDS. | 0x5\n";
            curl_easy_cleanup(req);
            std::this_thread::sleep_for(std::chrono::milliseconds(30000));
            printMainText();
            continue;
        }
        curl_easy_cleanup(req);
        if (std::filesystem::exists(robloxVersionFolder + "\\" + robloxVersionStr) == false) {
            std::cout << "\nYour Roblox install is outdated. Please update Roblox. | TRYING AGAIN IN 10 SECONDS. | 0x6\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
            printMainText();
            continue;
        }

        //Okay! Lets see if the flag list needs to be updated...
        string storedFflagVersion;

        std::ifstream flagVersionFile(rootDir + "\\flagversion.rco");
        flagVersionFile.seekg(0, std::ios::end);
        size_t size = flagVersionFile.tellg();
        string buffer(size, ' ');
        flagVersionFile.seekg(0);
        flagVersionFile.read(&buffer[0], size);
        storedFflagVersion = buffer;
        flagVersionFile.close();

        std::string latestFflagVersion;
        CURL* req2 = curl_easy_init();
        CURLcode res2;
        curl_easy_setopt(req2, CURLOPT_URL, "https://raw.githubusercontent.com/L8X/Roblox-Client-Optimizer/main/flagversion.rco"); //change to nul's url when she forks this
        curl_easy_setopt(req2, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(req2, CURLOPT_WRITEDATA, &latestFflagVersion);
        res2 = curl_easy_perform(req2);
        if (res2 != CURLE_OK) {
            std::cout << "\nNETWORK ERROR | PLEASE CHECK YOUR INTERNET CONNECTION | TRYING AGAIN IN 30 SECONDS. | 0x7\n";
            curl_easy_cleanup(req2);
            std::this_thread::sleep_for(std::chrono::milliseconds(30000));
            printMainText();
            continue;
        }
        curl_easy_cleanup(req2);
        
        if (storedFflagVersion != (latestFflagVersion + ' ') || std::filesystem::exists(robloxVersionFolder + "\\" + robloxVersionStr + "\\ClientSettings\\ClientAppSettings.json") == false) { //We need to do an update!!!!
            if (std::filesystem::exists(robloxVersionFolder + "\\" + robloxVersionStr + "\\ClientSettings") == false) {
                std::filesystem::create_directory(robloxVersionFolder + "\\" + robloxVersionStr + "\\ClientSettings");
            }

            std::string latestFflagList;
            CURL* req3 = curl_easy_init();
            CURLcode res2;
            curl_easy_setopt(req3, CURLOPT_URL, "https://raw.githubusercontent.com/L8X/Roblox-Client-Optimizer/main/ClientAppSettings.json"); //change to nul's url when she forks this
            curl_easy_setopt(req3, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(req3, CURLOPT_WRITEDATA, &latestFflagList);
            res2 = curl_easy_perform(req3);
            if (res2 != CURLE_OK) {
                std::cout << "\nNETWORK ERROR | PLEASE CHECK YOUR INTERNET CONNECTION | TRYING AGAIN IN 30 SECONDS. | 0x8\n";
                curl_easy_cleanup(req3);
                std::this_thread::sleep_for(std::chrono::milliseconds(30000));
                printMainText();
                continue;
            }
            curl_easy_cleanup(req3);

            std::ofstream fflagList;
            fflagList.open(robloxVersionFolder + "\\" + robloxVersionStr + "\\ClientSettings\\ClientAppSettings.json");
            fflagList << latestFflagList;
            fflagList.close();

            //Update the flag version file since we just updated the flags!
            std::ofstream flagversion;
            flagversion.open(rootDir + "\\flagversion.rco");
            flagversion << latestFflagVersion;
            flagversion.close();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(30000)); //We check for fflag updates every 30 seconds.
    }
}

int main() {
    //Preinit
    SetConsoleTitle(L"Roblox Client Optimizer");

    if (std::filesystem::exists(rootDir) == false) {
        std::cout << "Could not find proper RCO files, please reinstall RCO | 0x1\n";
        std::cin.get();
        return 1;
    }

    if (std::filesystem::exists(rootDir + "\\animegirl.ico") == false) {
        FILE* file;
        if (fopen_s(&file, (rootDir + "\\animegirl.ico").c_str(), "wb") != 0) {
            std::cout << "Could not open filepoint... | 0x2\n";
            std::cin.get();
            return 2;
        }

        CURL* req = curl_easy_init();
        CURLcode res;
        curl_easy_setopt(req, CURLOPT_URL, "https://raw.githubusercontent.com/L8X/Roblox-Client-Optimizer/main/animegirl.ico");
        curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(req, CURLOPT_WRITEDATA, file);
        res = curl_easy_perform(req);
        if (res != CURLE_OK) {
            std::cout << "NETWORK ERROR | PLEASE CHECK YOUR INTERNET CONNECTION | 0x4\n";
            std::cin.get();
            return 4;
        }
        curl_easy_cleanup(req);

        fclose(file);
    }

    if (std::filesystem::exists(rootDir + "\\flagversion.rco") == false) {
        std::ofstream flagversion;
        flagversion.open(rootDir + "\\flagversion.rco");
        flagversion << "0";
        flagversion.close();
    }

    if (std::filesystem::exists(rootDir + "\\isHidden.rco") == false) {
        std::ofstream isHiddenFile;
        isHiddenFile.open(rootDir + "\\isHidden.rco");
        isHiddenFile << "f";
        isHiddenFile.close();
    }

    if (std::filesystem::exists(rootDir + "\\isEnabled.rco") == false) {
        std::ofstream isEnabledFile;
        isEnabledFile.open(rootDir + "\\isEnabled.rco");
        isEnabledFile << "f";
        isEnabledFile.close();
    }

    if (std::filesystem::exists("C:\\Program Files (x86)\\Roblox\\Versions") == true) {
        std::cout << "Detected an Administrative Roblox install at C:\\Program Files (x86)\\Roblox\\Versions\nPlease reinstall Roblox without administrator, as RCO does not have Administrative permissions. | 0xA\nIf you've already reinstalled and still see this, please delete C:\\Program Files (x86)\\Roblox\n";
        std::cin.get();
        return 10;
    }

    if (!(_dupenv_s(&buf, &sz, "localappdata") == 0 && buf != nullptr)) {
        std::cout << "Error finding LocalAppData folder | 0xB\n";
        std::cin.get();
        return 11;
    }

    robloxVersionFolder = buf + string("\\Roblox\\Versions");
    free(buf);

    if (std::filesystem::exists(robloxVersionFolder) == false) {
        std::cout << "Roblox not found. Please reinstall Roblox | 0x3\n";
        std::cin.get();
        return 3;
    }

    //Initialize the tray icon system
    std::thread t1(traySystem);

    //Set Hidden and Enabled based on saved file
    std::ifstream hiddenFile(rootDir + "\\isHidden.rco");
    hiddenFile.seekg(0, std::ios::end);
    size_t size = hiddenFile.tellg();
    string buffer(size, ' ');
    hiddenFile.seekg(0);
    hiddenFile.read(&buffer[0], size);
    if (buffer == "t") {
        isConsoleHidden = true;
    }
    hiddenFile.close();

    std::ifstream enabledFile(rootDir + "\\isEnabled.rco");
    enabledFile.seekg(0, std::ios::end);
    size = enabledFile.tellg();
    buffer = string(size, ' ');
    enabledFile.seekg(0);
    enabledFile.read(&buffer[0], size);
    if (buffer == "t") {
        isRcoEnabled = true;
    }
    enabledFile.close();

    //Handle Hidden Value
    if (isConsoleHidden) {
        ShowWindow(consoleWindow, SW_HIDE);
    } else {
        ShowWindow(consoleWindow, SW_SHOW);
    }

    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MINIMIZEBOX);
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX);
    EnableMenuItem(GetSystemMenu(consoleWindow, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

    //Call the main fflag updating thread
    std::thread t2(mainThread);

    //Input loop
    while (true) {
        printMainText();
        string t; //Throwaway
        std::getline(std::cin, t);

        isRcoEnabled = !isRcoEnabled;
        std::ofstream isEnabledFile;
        isEnabledFile.open(rootDir + "\\isEnabled.rco");
        if (isRcoEnabled) {
            isEnabledFile << "t";
        } else {
            isEnabledFile << "f";
            std::string robloxVersionStr;
            CURL* req = curl_easy_init();
            CURLcode res;
            curl_easy_setopt(req, CURLOPT_URL, "http://setup.roblox.com/version");
            curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(req, CURLOPT_WRITEDATA, &robloxVersionStr);
            res = curl_easy_perform(req);
            if (res != CURLE_OK) {
                std::cout << "\nNETWORK ERROR | FAILED TO REMOVE FFLAG LIST, DELETE MANUALLY AT " + robloxVersionFolder + "\\current-roblox-version\\ClientSettings\\ClientAppSettings.json | 0x9\n";
            }
            curl_easy_cleanup(req);
            if (std::filesystem::exists(robloxVersionFolder + "\\" + robloxVersionStr + "\\ClientSettings\\ClientAppSettings.json") == true) {
                remove((robloxVersionFolder + "\\" + robloxVersionStr + "\\ClientSettings\\ClientAppSettings.json").c_str());
            }
        }
        isEnabledFile.close();
    }
}
