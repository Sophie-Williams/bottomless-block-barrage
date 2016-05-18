# bottomless-block-barrage
Bottomless Block Barrage (BBB) aims to be a Panel de Pon clone for the 3ds with new styles / modes of gameplay.  (Work in progrees)

## Roadmap
As of now the basic engine is almost complete sans a couple of timing issues

Game Modes that will be supported
* Endless Barrage - Play until you lose.
* Score Barrage - Get the highest score in a certain amount of time/lines.
* Puzzle Barrage - Solve all puzzles in a certain amount of moves (Normal and Action modes)
* Mission Barrage - Similar to the same mode in the Puyo Pop games
* Versus Barrage - 2 player game with AI (and possibly online play)

New Gameplay features
* New concept of "Chaining" (chaining as defined by previous entries is renamed to cascading here) in which you match groups of 3 rapidly before blocks are removed from the board.


## The demo
### Controls
* UP/DOWN/LEFT/RIGHT moves the cursor
* A swaps the two panels over the selector
* X increases the level
* Y decreases the level
* R quick rises the panels
* SELECT generates a new board


## Compiling
### Prerequisites
* Compiling cia and 3ds targets requires having [makerom](https://github.com/profi200/Project_CTR) and [bannertool](https://github.com/Steveice10/bannertool) in your $PATH
* [sf2dlib](https://github.com/xerpi/sf2dlib) is required along with devkitPro and [ctrulib](https://github.com/smealum/ctrulib) 
* Lastly [nin10kit](https://github.com/TricksterGuy/nin10kit) is required to be installed and in your $PATH

1) Once all of the above is in order simply type make and you will get .elf for citra, .3dsx for homebrew launcher, .cia for emunand, and .3ds for gateway/sky3ds.

2) If you care not to build everything then just type make [elf, cia, 3ds, 3dsx] to build the corresponding format, or make citra to run it in citra (requires citra to be installed and in your $PATH)

## Credits
Panel Graphics are based off http://thewolfbunny.deviantart.com/art/Pokemon-Puzzle-Challenge-Panels-TA-Style-510289235 by TheWolfBunny
Text is generated from http://mifki.com/df/fontgen.html
