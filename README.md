# Emoji-Karel
Using emojis instead of words to create a programming language that moves a robot in a 2D world. Based on the programming language called [Karel](https://en.wikipedia.org/wiki/Karel_(programming_language)), which is used to teach basic programming skills, like loops and conditions.

Use the following emojis:
1. ➡   Moves the robot (Karel) forward
2. ↺   Turns Karel left
3. 🤔  Indicates 'if'
4. 🙁  Indicates 'else'
5. 🔄  Indicates 'while' loops
6. 🚫  Indicates 'not'
7. 🧱  Makes Karel check if the wall to its front is blocked

Sample if-else statement (Karel turns left if the front is blocked, otherwise, it moves forward): 
```
🤔 (🧱) {
    ↺
} 🙁 {
    ➡
}
```

Sample while loops (Note that you can use while loops in two ways - either put a condition inside the parenthesis, in which case the loop will repeat until the condition becomes false; or put a number inside the parenthesis, which repeats the loop that many times similar to that of a for loop)
1. Condition-based while loop (Karel moves forward until it hits a wall):
```
🔄 (🚫 🧱) {
    ➡
}
```
2. Counter-based while loop a.k.a for loops (Karel turns left 4 times):
```
🔄 (4) {
    ↺
}
```

Example code (Makes Karel move around the edge of the 2D world once)
```
🔄 (4) {
    🔄 (🚫 🧱) {
        ➡
    }
    ↺
}
```

**For now, the program is only tested on linux**

## Requirements
[SFML](https://www.sfml-dev.org/)

## Setup
The main file to be run is `main.out`. To generate this file, run the following command in the terminal
```
make main.out
```

## How to use
Run the following command to start the program:
```
./main.out
```
Then type the code you want to run in the terminal and press `EOF`. A window should appear where you will see Karel in the bottom right of a 8x8 board. Press space to make Karel execute the next movement instruction (➡ or ↺). If the program successfully runs without Karel crashing towards any wall, the bottom region of the window shoud show _Karel has executed successfully_. But if Karel moves towards a wall, the bottom region will show _Karel can't move in that direction_. In either case, pressing space again will quit the window (and the program).

Alternatively, you can write your program in a text file and send it to main.out as standard input using `<`. 

Karel also creates a temporary text file containing a custom assembly language for Karel. Whenever space is pressed, Karel executes instructions from this assembly file. By default, the file's name is `temp.krl`. However, you can specifiy a name for the temporary text file by passing the file name as an argument to the command (that is, as `argv[1]`).
