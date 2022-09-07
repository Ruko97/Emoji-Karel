# Emoji-Karel
Using emojis instead of words to create a programming language that moves a robot in a 2D world

Use the following emojis:
➡   Moves the robot (Karel) forward
↺   Turns Karel left
🤔  Indicates 'if'
🙁  Indicates 'else'
🔄  Indicates 'while'
🚫  Indicates 'not'
🧱  Makes Karel check if the wall to its front is blocked

Example code (Makes Karel move around the edge of the 2D world once)
🔄 (4) {
    🔄 (🚫 🧱) {
        ➡
    }
    ↺
}
