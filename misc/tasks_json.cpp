// g++ main.cpp -o main
// g++ -std=c++20 -O0 -Wall -Wextra -Wshadow -Wconversion -D_GLIBCXX_DEBUG 
//      main.cpp -o main

// Save as .vscode/tasks.json
// Path: replace PUT G++ PATH HERE with the output of which g++ (Linux) 

{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "process",
            "label": "c++ competitive programming",
            "command": "PUT_G++_PATH_HERE",
            "args": [
                "-std=c++20",
                "-Wall", "-Wextra", "-Wshadow", "-Wconversion",
                "-D_GLIBCXX_DEBUG",
             // "-fsanitize=address,undefined" -- Linux only
                "${file}", "-o",
                "${fileDirname}/${fileBasenameNoExtension}.exe"
            ],
            "problemMatcher": ["$gcc"],
            "group": "build",
            "detail": "ICPC COMPETITION",
            "presentation": {
                "showReuseMessage": true,
                "clear": true
            }
        }
    ]
}
