# Emoji-Karel
Using emojis instead of words to create a programming language that moves a robot in a 2D world

Use the following emojis:
1. ➡   Moves the robot (Karel) forward
2. ↺   Turns Karel left
3. 🤔  Indicates 'if'
4. 🙁  Indicates 'else'
5. 🔄  Indicates 'while'
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

## Setup
The main file to be run is `main.out`. To generate this file, run the following command in the terminal
```
make main.out
```

