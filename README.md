SAWD
====

Developed for an RPGDX contest. Managed to win first place with this complete traditional console style role playing game rendered completely with ASCII characters. A custom built Win32 console engine sits behind this complex 24-hour game project.


## Original readme.txt below
SAWD Small ASCII Walk-around Demo
(C) Copyright 2008, CCPS Solutions
http://dexcode.googlepages.com/rpgs

What is included in this archive?

The complete C++ source code that was used to create the game.

The data files that shipped with the game.

I compiled the game using Microsoft Visual C++ 2008 Express Edition (MSVC9) I suggest doing the same.

The game will not compile on linux because I make use of Windows API calls, however if you
were to port those calls to linux terminal or ncurses, then please let me know about it!

How to build in MSVC9:

1. Create a new empty console application (I assume you know how)

2. Add the source & header files to the project.

3. Right click the project, open properties, and make sure its set to use Multibyte Characters, not unicode.

4. Set the C++ Code Generation options to use Multithreaded Debug NOT Multithreaded Debug DLL (default)

That should do it. Any trouble, just let me know about it.

This source is provided under the GNU GPL license terms.
Read license.txt included with the source for details.

Thank you.
-DeveloperX
Richard Marks
CEO, CCPS Solutions
ccpsceo@gmail.com
