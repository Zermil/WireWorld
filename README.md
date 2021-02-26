# WireWorld
Simple console simulation of [Wireworld](https://en.wikipedia.org/wiki/Wireworld) in C++ with added initialization.

![preview](./preview_image.jpg)

## Quick start (Windows with g++)
### `Build`:
```console
PATH\TO\FILES> build
```

### `Run`:
```console
PATH\TO\FILES> main
```

## Instructions
After running the "program" input the size of the board, example:
```console
5 5
```
*Note: minimum size is 5x5 board and coordinates are given as X-axis first, Y-axis second*

~~**Important:** *Console output may be slow for big sizes (ex. 50x50)*~~

(Update: Fixed, you can now have large board sizes, like 50x50 or more... but why?)

After that you'll enter 'Normal Mode'.
### `Normal Mode`:
- `Arrow Keys`: Move
- `'1'`: change state at indicator ('@') to EMPTY -> `' '`
- `'2'`: change state at indicator ('@') to ELECTRON HEAD -> `'O'`
- `'3'`: change state at indicator ('@') to ELECTRON TAIL -> `'o'`
- `'4'`: change state at indicator ('@') to CONDUCTOR -> `'#'`
- `SPACE`: Enter 'Simulation Mode'
- `ESC`: Exit program

### `Simulation Mode`:
- `'e'`: exit 'Simulation Mode' -> enter 'Normal Mode'
- `'r'`: Restart 'Simulation'
- `Any other key`: One step forward in 'Simulation'

# Initialize from (text) file
While initializing from file make sure:
- All lines have the same length (no `SPACE` at the end)
- Minimum size is also 5x5 (5 lines, 5 characters)
- All signs are 'legal':
	- `'.'` = (dot) EMPTY; for clarity and easier readability
	- `'O'` = (big letter O) ELECTRON HEAD
	- `'o'` = (small letter o) ELECTRON TAIL
	- `'#'` = (hash) CONDUCTOR