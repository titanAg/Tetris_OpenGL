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