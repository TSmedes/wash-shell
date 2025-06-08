# Wash Shell

A simple, lightweight shell implementation written in C. Wash provides basic shell functionality.

## Features

- **Built-in Commands**
  - `exit`: Exit the shell
  - `echo`: Print arguments to console
  - `pwd`: Print current working directory
  - `cd`: Change directory (defaults to home if no argument)
  - `setpath`: Set the PATH for command lookup
  - `help`: Display help message

- **External Command Support**
  - Execute commands from system PATH
  - Custom commands: `head_nine`, `cat`, `ls`, `wc`, `tail`
  - These commands are found in the system PATH and are executed in a new forked process

## Installation

### Prerequisites

- C compiler (gcc recommended)
- Make
- CMake (version 3.10 or higher)
- Basic Unix/Linux environment

### Build steps

1. Create a build directory:
```bash
mkdir build && cd build
```

2. Build using CMake:
```bash
cmake ..
```
```bash
make
```

3. The wash shell executable will be in the project root directory. External commands will be in the `bin` directory.

### Man Page

To install the man page, run the following command from the project root directory:
```bash
cp README /usr/share/man/man1/wash.1
```

## Usage

### Starting the Shell

```bash
./wash [-h]
```

The `-h` flag displays the help message and exits.

### Basic Commands

```bash
# Print current directory
pwd

# Change directory
cd /path/to/directory
```

### Extra Commands

To be able to use the extra commands, you must make sure they are in the PATH. This is by default `/bin`. If you use `CMakeLists.txt` to build, these will be in `build/bin`. So to add them to the path:

```bash
setpath /bin bin
```

```bash
# List files
ls

# Display file contents
cat filename

# Count lines, words, and characters
wc filename

# Show last 10 lines of a file
tail filename

# Show first 9 lines of a file
head_nine filename
```

### Setting PATH

The initial PATH contains only `/bin`. Use `setpath` to add more directories:

```bash
# Add multiple directories to PATH
setpath /bin /usr/bin /usr/local/bin

# Reset PATH to a single directory
setpath /bin
```


## Project Structure

```
wash-shell/
├── wash.c          # Main shell implementation
├── commands/       # Directory containing custom commands
│   ├── head_nine.c # Custom head command
│   ├── cat.c      # Simple cat implementation
│   ├── ls.c       # Simple ls implementation
│   ├── wc.c       # Simple wc implementation
│   └── tail.c     # Simple tail implementation
├── CMakeLists.txt # Build configuration
├── README         # MAN page for wash
└── README.md      # This file
```