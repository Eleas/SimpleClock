# SimpleClock
## What it is

It's just a dead simple wall-clock, written in C++.

There's not much to say, really. This is a simple clock application. It draws an image of a clock on the screem, which shows the time.

##  How do I run it?

Currently it's a Visual Studio based, Windows-only project, so VS 2022 using the latest cpp compiler should be the way to go.

Also, rendering is handled by the [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine). Just paste the [single header file](https://raw.githubusercontent.com/OneLoneCoder/olcPixelGameEngine/master/olcPixelGameEngine.h) into the source folder, and you'll be good to go.

## What is it even for?

Well, it's a code kata. In order to be better and faster at writing code, I need to do these things. I've not written something based on Windows time yet, so there were a few gotchas. But at this point, I think, the code can be considered acceptable.

## What remains to be done?

I could (should) convert it to CMake, and ensure that it is platform independent. Getting the correct system time works differently on Linux and OSX. I'm frankly disappointed with how std::chrono handles this -- trying to look up how it should be done in the official documentation is like trying to go to MSDN to find answers.

## Can I use it?

Sure. It's under the [Unlicense](https://unlicense.org/). Knock yourself out.
