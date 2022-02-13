# screenshot
Simple screenshotting utility written in C, uses CSFML, shotgun, xclip  

CSFML - https://github.com/SFML/CSFML

shotgun - https://github.com/neXromancers/shotgun (or sudo pacman -S shotgun)

xclip - https://github.com/astrand/xclip (or sudo pacman -S xclip)

Shift+C copies the selected image to your clipboard, so you can Ctrl+v it into an image editor or some other program.

Shift+S saves the selected region of the image to TEMPORARY_FILE_DIRECTORY/screenshots/screenshot-n.png, where n is the nth screenshot.
