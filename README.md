NRC (Not Real Core) is a core designed to simplify and speed up the creation of C/C++ pseudo-OS.

# What is currently available:
- A manager for managing users (and users themselves).
- A class that allows you to easily insert your commands (now with arguments!) into the kernel without changing the kernel itself. ( handlerCommands ).
- Data and file managers (one works with .json files, the other contains simple commands to create folders and files, check them, delete them...).
- Commands (whoim, delete_user, etc.).
- Built-in OS (bin/source/OS). Modify it as you want.
- And also print (with support for outputting colors, which are in core::colors).
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
- EN: https://github.com/SpaceOC/NRC/wiki
- RU: https://spaceoc.gitbook.io/nrc-docs-ru/
