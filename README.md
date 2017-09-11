# bottomless-block-barrage 
Bottomless Block Barrage (BBB) aims to be a Puzzle League clone for the 3ds with new styles / modes of gameplay.
This game is a work in progress!

## Roadmap
As of now the basic engine is complete, work is now being done implementing the various modes.  Basic graphics are used now until better ones are found/made.

Game Modes that will be supported
* Endless Barrage - Play until you lose.
* Score Barrage - Get the highest score in a certain amount of time/lines.
* Puzzle Barrage - Solve all puzzles in a certain amount of moves (Normal and Action modes)
* Mission Barrage - Similar to the same mode in the Puyo Pop games
* Versus Barrage - 2 player game with AI (and possibly online play)

New Gameplay features
* Introducing C-Links (Chain Links) in which you match groups of 3 or more rapidly before blocks are removed from the board.


## The demo
### Screenshots
![Title](https://github.com/TricksterGuy/bottomless-block-barrage/blob/master/screenshots/title_wip.png)
![Config](https://github.com/TricksterGuy/bottomless-block-barrage/blob/master/screenshots/config_wip.png)
![Score Mdoe](https://github.com/TricksterGuy/bottomless-block-barrage/blob/master/screenshots/score_wip.png)
![Endless Mode](https://github.com/TricksterGuy/bottomless-block-barrage/blob/master/screenshots/endless_wip.png)

### Controls
* Up/Down/Left/Right moves the cursor
* A swaps the two panels over the selector
* R quick rises the panels
* Start instant quits and returns to Title Screen


## Compiling
### Prerequisites
* Compiling cia and 3ds targets requires having [makerom](https://github.com/profi200/Project_CTR) and [bannertool](https://github.com/Steveice10/bannertool) in your $PATH
* [sf2dlib](https://github.com/xerpi/sf2dlib) is required along with [sfillib](https://github.com/xerpi/sfillib) with [portlibs](https://github.com/xerpi/3ds_portlibs)
* devkitPro and [ctrulib](https://github.com/smealum/ctrulib)

1) Once all of the above is in order simply type make and you will get .elf for citra, .3dsx for homebrew launcher, .cia for emunand, and .3ds for gateway/sky3ds.

2) If you care not to build everything then just type make [elf, cia, 3ds, 3dsx] to build the corresponding format, or make citra to run it in citra (requires citra to be installed and in your $PATH)

## Credits
Title screen/banner and icon by cots

[Panel Graphics](http://thewolfbunny.deviantart.com/art/Pokemon-Puzzle-Challenge-Panels-TA-Style-510289235) by TheWolfBunny

[Panel Graphics](https://github.com/TricksterGuy/bottomless-block-barrage/blob/master/graphics/panels2_gfx.png) by SRKTiberious

Font is generated from http://www.codehead.co.uk/cbfg/, and using flarn2006's SF2DBmpFont library to render it.
