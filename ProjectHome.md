So why another unit testing framework? There was nothing out there that met my requirements. Namely:

  1. It has to work in Microsoft Windows (yeah, yeah I know)
  1. Tests should be independent. That means global/static variables HAVE to be in a known state. That means each test has to execute as a seperate process.
  1. Something free would be nice.

Maybe there is something out there, but I didn't find it :-)

Note: If you're using C++, just rename main.c to main.cpp.

I'm interested to know how useful this tool is, so please post any questions and feedback to [the mailing list](http://groups.google.com/group/CUnitWin32).
