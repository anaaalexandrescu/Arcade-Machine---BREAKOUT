# Breakout Game Project

## 1. Overview

This project is a complete 2D implementation of the classic arcade game *Breakout, developed in C++ using a custom OpenGL framework. The application features two distinct scenes: a **Level Editor, where players design their own paddle, and the **Game Scene*, where the core gameplay takes place.

## 2. Project Structure

The application logic is divided into three main components:

* *Tema1 (Main Scene)*: Manages the state machine, switching between the EDITOR and GAME scenes.
* *Editor*: Handles the grid logic, mouse input for building the paddle, and level validation.
* *Game*: Manages the physics engine, collision detection, game rules (lives, score), and object updates.

## 3. Key Features

### The Editor

Before playing, the user interacts with a 9x17 grid to construct a custom paddle (ship).

* *Block Placement*: Users can drag blocks from a source palette and drop them onto the grid.
* *Validation Logic*: The editor ensures that the constructed paddle is valid (blocks must be contiguous on a single row) before enabling the "Start" button.
* *Dynamic Resources*: The number of available blocks is limited to 10. Removing a block returns it to the counter.

### The Game

* *Dynamic Paddle*: The paddle's width is calculated directly from the number of blocks placed in the editor.
* *Brick System*: The level consists of a grid of bricks with varying "health" (resistance).
* Colors indicate brick health (e.g., Green = 1 hit, Orange = 3 hits).
* Special "indestructible" bricks (Gold) require multiple hits to break.
* *Animations*: Bricks shrink over time when destroyed before disappearing completely.


* *Scoring & Lives*: Players start with 3 lives. Points are awarded based on the resistance of the destroyed brick.

### Rendering System

* *Custom 2D Meshes*: All game objects (squares, circles, rectangles) are drawn using procedurally generated meshes via the object2D and render classes.
* *Text Rendering*: A custom system draws text and numbers using a matrix of small squares (5x3 grid per character), avoiding external font libraries.

## 4. Technical Implementation Details

### Drag and Drop Mechanics

The editor uses a state-based approach to handle mouse interactions:

1. *Initiation (LeftClick)*: When the user clicks on the source block, the system calculates an offset vector (difference between the mouse position and the block's corner). This ensures the block moves naturally relative to the cursor, without "snapping" to the center instantly.
2. *Dragging (MouseDrag)*: As long as the mouse button is held, the block's render position (dragPos) is updated by adding the calculated offset to the current mouse coordinates.
3. *Placement (LeftRelease)*: Upon release, the system checks if the block's center falls within the boundaries of a grid cell using Axis-Aligned Bounding Box (AABB) logic. If the cell is empty and valid, the block snaps to the cell's center; otherwise, the action is cancelled.

### Ball Physics & Collision

The physics engine handles movement and reflection logic:

* *Movement*: The ball's position is updated every frame using ball.x += ball.dx * deltaTime.
* *Wall Collisions*: Simple coordinate checks reverse the velocity vector (dx = -dx or dy = -dy) when the ball hits the screen boundaries.
* *Brick Collisions*: The game checks overlaps between the ball (circle) and bricks (rectangles). Depending on the overlap depth on the X or Y axis, the system determines which side was hit and reflects the ball accordingly.
* *Paddle Reflection (Angle Calculation)*: The reflection off the paddle is not simple. The code calculates the impact point relative to the paddle's center.
* This value is normalized to a range of [-1, 1].
* It is then converted to an angle (max 60 degrees).
* New velocity vectors are computed using sin and cos, allowing players to "aim" the ball by hitting it with the edge of the paddle.



## 5. Controls

### Editor Scene

* *Left Mouse Button (Hold)*: Drag and drop blocks to build the ship.
* *Right Mouse Button*: Remove a placed block from the grid.
* *Left Click (Start Button)*: Start the game (only if the ship design is valid).

### Game Scene

* *Left Arrow*: Move paddle left.
* *Right Arrow*: Move paddle right.
* *Spacebar*: Launch the ball.
* *Mouse*: Not used during gameplay.
