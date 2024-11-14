NRC (Not Real Core) is a core designed to simplify and speed up the creation of C/C++ pseudo-OS.

# What is currently available:
- A manager for managing users (and users themselves).
- A class that allows you to easily insert your commands (now with arguments!) into the kernel without changing the kernel itself. ( HandlerCommands ).
- Data and file managers (one works with .json files, the other contains simple commands to create folders and files, check them, delete them...).
- Pseudo-FS!!!
- Commands (whoim, delete_user, etc.).
- Embedded OS (bin/source/OS). Modify it as you wish.
- And also printing (print) (with support for outputting colors that are in core::PrintColors).
- And something else, but I'm lazy.

# Requirements
- ~~Computer~~
- Windows operating system. Linux is also possible
- C++ standard at least 17 (I guess)
- nlohmann's json library: https://github.com/nlohmann/json

# Compilation
```bash
cmake -B "build"
cd build
make
```

# Documentation
- EN: https://github.com/SpaceOC/NRC/wiki (not finished + outdated)
- RU: https://spaceoc.gitbook.io/nrc-docs-ru/ (outdated)
