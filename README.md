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

