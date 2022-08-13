# screenshot
Simple screenshotting utility written in C, uses CSFML, shotgun, xclip  

# How to use

Edit the source code and change the C macros to suit your needs.

TEMPORARY_SCREENSHOT_FILE - the filename of the temporary screenshot. For example, "temp_screenshot.png", and this file is saved to the directory specified by TEMPORARY_SCREENSHOT_DIRECTORY

FONT_NAME - the path to the font. Make sure it is a .ttf font.

the other macros are self explanatory so I won't explain them...

# Dependencies

CSFML - https://github.com/SFML/CSFML (or sudo pacman -S csfml)

shotgun - https://github.com/neXromancers/shotgun (or sudo pacman -S shotgun)

xclip - https://github.com/astrand/xclip (or sudo pacman -S xclip)

Shift+C copies the selected image to your clipboard, so you can Ctrl+v it into an image editor or some other program, like a Discord chat.

Shift+S saves the selected region of the image to TEMPORARY_FILE_DIRECTORY/screenshots/screenshot-n.png, where n is the nth screenshot.

Planned to-dos/features: 
- [Already added] a better way to save the image (e.g. select a directory, type in a file name, etc.)
- clean up the code

next update: probably never.
edit: I actually did end up updating this multiple times
