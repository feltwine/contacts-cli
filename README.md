# contacts-cli
CLI application for contacts list writen in C language.

## Premake

This project uses Premake for build configuration. Follow these steps to build the project using Premake:

### Installation

Ensure you have Premake installed on your system. You can download Premake from [here](https://premake.github.io) and follow the installation instructions provided.

### Building the Project
1. **Generate project files**: Run Premake to generate Makefiles:

   For Linux:
   ```bash
   premake5 gmake
   ```
   
   For mac: 
   ```bash
   premake5 xcode4
   ```

2. **Build project from Makefile**:
   ```bash
   make
   ```
