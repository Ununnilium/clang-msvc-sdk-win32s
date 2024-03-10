# Cross toolchain configuration for using clang-cl on non-Windows hosts to target Windows 3.1 with WIN32S.

Based on https://github.com/ProGTX/clang-msvc-sdk.


## Introduction: Manually compile for Windows 3.1 with WIN32S with Clang

1.  Install _Microsoft Windows Server 2003 DDK (5.2.3790)_ on Windows, this creates a folder c:\WINDDK which can be moved to Linux (e.g. to HOME)
2.  Create the following test program hello.c:
    ```C
    #include <windows.h>
    #include <stdio.h>

    int main()
    {
        FILE *fptr;
        fptr = fopen("c:\\filename.txt", "w");
        fprintf(fptr, "Some text");
        fclose(fptr);
        return MessageBox(NULL, "hello, world", "caption", 0);
    }
    ```
3.  Build it with
    ```bash
    clang-cl-17 hello.c -o hello.exe --target=i386-pc-windows-msvc -fuse-ld=lld /imsvc ~/WINDDK/3790/inc/crt /imsvc ~/WINDDK/3790/inc/wnet /link /libpath:~/WINDDK/3790/lib/wnet/i386 /subsystem:WINDOWS,3.10 /entry:mainCRTStartup user32.lib
    ```
    or
    ```bash
    clang-17 -o hello.exe -static -target i386-pc-windows-msvc -isystem ~/WINDDK/3790/inc/crt -isystem ~/WINDDK/3790/inc/wnet -fuse-ld=lld -L~/WINDDK/3790/lib/wnet/i386 -luser32  hello.c -Xlinker /subsystem:WINDOWS,3.10 -Xlinker /force:multiple -Xlinker /entry:mainCRTStartup
    ```
/entry:mainCRTStartup allows using "main" not only for /subsystem:console applications (see [here](https://stackoverflow.com/a/13872211)).


## Installation of Windows 2003 DDK
1.  Install _Microsoft Windows Server 2003 DDK (5.2.3790)_ on Windows, this creates a folder c:\WINDDK which can be moved to Linux, e.g. to HOME
2.  Create a folder ~/WINDDK/clang with this structure (symlinks):
    ```
    clang
    ├── MSVC_BASE
    │   ├── include -> ../../3790/inc/crt
    │   └── lib
    │       ├── x64 -> ../../../3790/lib/wnet/amd64
    │       └── x86 -> ../../../3790/lib/wnet/i386
    └── WINSDK_BASE
        ├── Include
        │   └── 3790
        │       └── um -> ../../../../3790/inc/wnet
        └── Lib
            └── 3790
                └── um
                    ├── x64 -> ../../../../../3790/lib/wnet/amd64
                    └── x86 -> ../../../../../3790/lib/wnet/i386
    ```
3.  Create the following symlinks in WINDDK/3790/inc/wnet:
    ```
    Windows.h -> WINDOWS.H
    comcat.h -> COMCAT.H
    commctrl.h -> COMMCTRL.H
    commdlg.h -> COMMDLG.H
    cpl.h -> CPL.H
    dde.h -> DDE.H
    ddeml.h -> DDEML.H
    diskguid.h -> DISKGUID.H
    imm.h -> IMM.H
    nspapi.h -> NSPAPI.H
    objbase.h -> OBJBASE.H
    objerror.h -> OBJERROR.H
    poppack.h -> POPPACK.H
    prsht.h -> PRSHT.H
    pshpack1.h -> PSHPACK1.H
    pshpack2.h -> PSHPACK2.H
    pshpack4.h -> PSHPACK4.H
    pshpack8.h -> PSHPACK8.H
    setupapi.h -> SETUPAPI.H
    shellapi.h -> SHELLAPI.H
    uuids.h -> UUIDS.H
    vddsvc.h -> VDDSVC.H
    wdbgexts.h -> WDBGEXTS.H
    winbase.h -> WINBASE.H
    windef.h -> WINDEF.H
    windows.h -> WINDOWS.H
    windowsx.h -> WINDOWSX.H
    winerror.h -> WINERROR.H
    wingdi.h -> WINGDI.H
    winnls.h -> WINNLS.H
    winnls32.h -> WINNLS32.H
    winnt.h -> WINNT.H
    winperf.h -> WINPERF.H
    winreg.h -> WINREG.H
    winresrc.h -> WINRESRC.H
    winspool.h -> WINSPOOL.H
    wintrust.h -> WINtrust.H
    winuser.h -> WINUSER.H
    winver.h -> WINVER.H
    ```

4.  Test the example:
    ```bash
    cd example
    mkdir build
    cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../../clang-cl-msvc.cmake -DMSVC_BASE=/home/fheller/Downloads/WINDDK/clang/MSVC_BASE -DWINSDK_VER=3790 -DWINSDK_BASE=/home/fheller/Downloads/WINDDK/clang/WINSDK_BASE -DCLANG_VER=17 -DLLVM_VER=17 -DCMAKE_C_COMPILER=/usr/bin/clang-cl-17 -DCMAKE_CXX_COMPILER=/usr/bin/clang-cl-17 -DCMAKE_LINKER=/usr/bin/lld-link-17 -DHOST_ARCH=x86 && cmake --build .
    wine hello-world.exe
    ```

## Usage 

```
cmake -G Ninja
   -DCMAKE_TOOLCHAIN_FILE=/path/to/clang-cl-msvc.cmake
   -DHOST_ARCH=[i686|x86|x86_64|x64]
   -DMSVC_BASE=/path/to/msvc
   -DWINSDK_BASE=/path/to/winsdk
   -DWINSDK_VER=windows sdk version folder name
   -DLLVM_VER=7
   -DCLANG_VER=7
```

##### HOST_ARCH:
   The architecture to build for.

##### MSVC_BASE:
  *Absolute path* to the folder containing MSVC headers and system libraries.
  The layout of the folder matches that which is intalled by MSVC 2017 on
  Windows, and should look like this:

```
${MSVC_BASE}
  include
    vector
    stdint.h
    etc...
  lib
    x64
      libcmt.lib
      msvcrt.lib
      etc...
    x86
      libcmt.lib
      msvcrt.lib
      etc...
```

For versions of MSVC < 2017, or where you have a hermetic toolchain in a
custom format, you must use symlinks or restructure it to look like the above.

##### WINSDK_BASE:
  Together with WINSDK_VER, determines the location of Windows SDK headers
  and libraries.

##### WINSDK_VER:
  Together with WINSDK_BASE, determines the locations of Windows SDK headers
  and libraries.

WINSDK_BASE and WINSDK_VER work together to define a folder layout that matches
that of the Windows SDK installation on a standard Windows machine.  It should
match the layout described below.

Note that if you install Windows SDK to a windows machine and simply copy the
files, it will already be in the correct layout.

```
${WINSDK_BASE}
  Include
    ${WINSDK_VER}
      shared
      ucrt
      um
        windows.h
        etc...
  Lib
    ${WINSDK_VER}
      ucrt
        x64
        x86
          ucrt.lib
          etc...
      um
        x64
        x86
          kernel32.lib
          etc
```

IMPORTANT: In order for this to work, you will need a valid copy of the Windows
SDK and C++ STL headers and libraries on your host.  Additionally, since the
Windows libraries and headers are not case-correct, this toolchain file sets
up a VFS overlay for the SDK headers and case-correcting symlinks for the
libraries when running on a case-sensitive filesystem.

##### LLVM_VER (optional):
  Version of the LLVM/LLD installation on the machine, it assumes version 7 in case it is
  not set.

##### CLANG_VER (optional):
  Version of the clang(-cl) installation on the machine, it assumes version 7 in case it is
  not set.

## Obtaining the llvm/clang toolchain

The LLVM/Clang toolchain apt repositories can be found at:

http://apt.llvm.org/

The following packages are required to be present on your system:

- llvm-7
- clang-7
- clang-tools-7

## Obtaining the Windows SDK and MSVC files

The Windows SDK can downloaded directly from Microsoft at:

https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk

The MSVC Base files can be obtained from any recent installation of Visual Studio assuming the VC++ components have been installed.
The files can generally be found at the following location on your machine:

C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Tools\MSVC

## Examples

### Using a makefile for building a project on debian9 for win32

```
.PHONY: all clean clang-msvc-sdk

MSVC_SDK_URL={Git URL to a copy of this repository which includes the MSVC SDK}

HOSTS=win32-clang deb9
BUILD_TYPES=debug release

TARGETS=$(foreach h,$(HOSTS),$(addprefix $(h)-,$(BUILD_TYPES)))

define build_type
$(subst r,R,$(subst d,D,$(subst /,,$(lastword $(subst -, ,$(dir $1))))))
endef
 
.SECONDEXPANSION:
$(TARGETS): build/$$@/build.ninja
    @cmake --build build/$@

.clang-msvc-sdk:
    git clone $(MSVC_SDK_URL) .clang-msvc-sdk

clang-msvc-sdk: .clang-msvc-sdk
    @cd .clang-msvc-sdk && git pull

build/deb9-%/build.ninja: Makefile
    @mkdir -p $(dir $@)
    @cd $(dir $@) && cmake ../.. -G Ninja \
        -DCMAKE_BUILD_TYPE=$(call build_type,$@)

build/win32-clang-%/build.ninja: Makefile .clang-msvc-sdk
    @mkdir -p $(dir $@)
    @cd $(dir $@) && cmake ../.. -G Ninja \
        -DCMAKE_BUILD_TYPE=$(call build_type,$@) \
        -DCMAKE_TOOLCHAIN_FILE=$(abspath .clang-msvc-sdk)/clang-cl-msvc.cmake \
        -DMSVC_BASE=$(abspath .clang-msvc-sdk)/msvc/14.14.26428 \
        -DWINSDK_BASE=$(abspath .clang-msvc-sdk)/winsdk \
        -DWINSDK_VER=10.0.17134.0 \
        -DHOST_ARCH=x86
```

The above make file provides the following targets:

```
$ make
all                          win32-clang-debug          deb9-debug
clang-msvc-sdk               win32-clang-release        deb9-release
clean
```
