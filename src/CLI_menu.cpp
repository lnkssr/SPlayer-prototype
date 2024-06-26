#include "CLI_menu.h"
#include <vector>
#include <iostream>
#include <ncurses.h>
#include <dirent.h>

std::vector<std::string> getFiles(const std::string &path) {
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                files.push_back(ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Failed to open directory");
    }

    return files;
}

std::string browseFile(const std::string &path) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    std::vector<std::string> files = getFiles(path);

    if (files.empty()) {
        endwin();
        std::cerr << "No files found in the directory." << std::endl;
        return "";
    }

    int highlight = 0;
    int choice;
    int n_choices = files.size();

    while (1) {
        clear();
        for (int i = 0; i < n_choices; ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
                mvprintw(i, 0, "%s", files[i].c_str());
                attroff(A_REVERSE);
            } else {
                mvprintw(i, 0, "%s", files[i].c_str());
            }
        }

        choice = getch();
        switch (choice) {
            case KEY_UP:
                if (highlight > 0) {
                    --highlight;
                }
                break;
            case KEY_DOWN:
                if (highlight < n_choices - 1) {
                    ++highlight;
                }
                break;
            case 10: // Enter key
                endwin();
                return files[highlight];
            default:
                break;
        }
    }

    endwin();
    return "";
}
