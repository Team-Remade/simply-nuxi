
# Mine Imator Simply Remade: Nuxi

Animate your Minecraft ideas!


## Dev Environment

### Setup Windows

- Install [MSYS2](https://github.com/msys2/msys2-installer/releases/download/2025-08-30/msys2-x86_64-20250830.exe).

- Install scoop.
```bash
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression
```

- Install xmake.
```bash
scoop install main/xmake
```

- Install cmake.
```
winget.exe install --id "Kitware.CMake" --exact --source winget --accept-source-agreements --disable-interactivity --version "3.31.8" --silent --accept-package-agreements --force
```

- Open MSYS2 terminal.
```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```

- Press enter for the package members and press Y when prompted to proceed.

- Add `C:\msys64\ucrt64\bin` to your `Path` environment variable.

### Check mingw
```bash
gcc --version
g++ --version
gdb --version
```

### Build/run the project
```bash
xmake
xmake run
```

If you get `assertion failed: ` during the build process, run `xmake` again.