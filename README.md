#### Other version of this file
[ja-jp](README.ja-jp.md): 日本語\
[ko-kr](README.ko-kr.md): 한국어

# Reversi
Simple Reversi Library for Windows 10. Free to use under any conditions.

## Getting started ([C++ API](CPPAPI.md) / [C API](CAPI.md) / [C# API](CSAPI.md))
The library supports C++, C and C#.

## Exploring the repository
The repository contains four Visual Studio projects.
### ReversiNative (C++)
The core of the library. The project includes the actual Reversi code. ReversiManaged is no more than an interop.
### ReversiNativeTest (C++ / C)
This is an example of an application using ReversiNative. In the project are two files with the same name but different file extensions(.c and .cpp). Two files do exactly the same thing.
### ReversiManaged (C#)
An interop using ReversiNative.
### ReversiManagedText
This is an example of an application using ReversiManaged.
