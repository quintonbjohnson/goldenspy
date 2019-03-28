# Goldenspy

![Goldenspy](http://i.imgur.com/Khw7wsx.png)

By QuintonJ

Main.c is the file that contains the game logic and actual components.

This is a Mode 3 GBA game in which you play a spy who is trying to traverse a path of enemies in order to get to the other side. You have 3 lives, and if you are caught by an enemy, you lose a life. If you get to the other side, you win!

SELECT 	returns to the title screen at any point

UP	moves the character up the screen

DOWN	moves the character down the screen

LEFT 	moves the character left on the screen

RIGHT	moves the character right on the screen

A 	starts the game

## Installation

1 | Install the C compiler

`sudo apt-get install build-essential`



2 | Install [nin10kit](https://github.com/TricksterGuy/nin10kit#installation)

`sudo apt-get install nin10kit`



3 | Install [VBA-M](https://github.com/visualboyadvance-m/visualboyadvance-m)
```cd ~ && mkdir src && cd src

git clone https://github.com/visualboyadvance-m/visualboyadvance-m.git

cd visualboyadvance-m

./installdeps

