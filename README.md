# SimpleClock
## A dead simple wall-clock, written in C++

There's not much to say, really. This is a dead simple clock application. It draws an image of a clock, and shows the time.

## Why would you do this?

Well, it's a code kata. In order to be better and faster at writing code, I need to do these things. I've not written something based on Windows time yet, so there were a few gotchas. But at this point, I think, the code can be considered acceptable.

## What remains to be done?

I could (should) convert it to CMake, and ensure that it is platform independent. Getting the correct system time works differently on Linux and OSX. I'm frankly disappointed with how std::chrono handles this -- trying to look up how it should be done in the official documentation is like trying to go to MSDN to find answers.

## Can I use it?

Sure. It uses the Unlicense. Knock yourself out.
