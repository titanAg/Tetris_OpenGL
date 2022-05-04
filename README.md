# Tetris_OpenGL

## This is a mostly complete implementation of the Tetris game
### This is a really great project and it is one of my favorites

## Dependencies
- To Run project GLEW and GLFW libraries must be in parent directory of project
- GLEW -> http://glew.sourceforge.net/
- GLFW -> https://www.glfw.org/
- Folder structure -> "External Libraries/GLEW/" & "External Libraries/GLFW/"

## Controls
- Esc / Q.......................exits game
- R.............................restarts game
- P.............................pauses game
- C.............................hard drop tetromino
- UP Arrow......................rotate tetromino
- LEFT/RIGHT Arrow..............move tetromino left/right
- DOWN Arrow....................move tetromino down


## Score:
- Row removals
	- 1 row adds 50 * level
	- 2 rows adds 150 * level
	- 3 rows adds 350 * level
	- 4 rows adds 1000 * level
	
- Down arrow drop
	- 1 point * distance * level
	
- C key drop (hard drop)
    - 2 points * distance * level
	

## Levels

- levels (10 provided)
	- advance by clearing an increasing amount of rows
	- speed increases by 0.12 per level (max speed 0.4 seconds)
	- rows of dead blocks apear at the beginning of each level
		- spawns 1 row * level
		- rows can be cleared


## Gameplay

![Tetris Gameplay](https://user-images.githubusercontent.com/97991414/166834242-70d8f0a4-5a24-472a-ac3f-43feb81d3561.gif)

![2022-05-04_15h06_54](https://user-images.githubusercontent.com/97991414/166834280-b70d2e95-2c03-4e59-9dba-74812a964582.png)
![2022-05-04_15h08_03](https://user-images.githubusercontent.com/97991414/166834287-a6ed8fae-ba28-4503-9178-e204fb74de88.png)
![2022-05-04_15h08_53](https://user-images.githubusercontent.com/97991414/166834293-1d5f0093-ed25-44da-9f17-a6f959512c98.png)
![2022-05-04_15h09_18](https://user-images.githubusercontent.com/97991414/166834308-5c4fe58b-8dc1-4069-8fda-5b46bda1c263.png)

