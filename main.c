#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>

//create a subdirctory under TEMPORARY_SCREENSHOT_DIRECTORY, name it "screenshots". There, your screenshots will be stored.
#define TEMPORARY_SCREENSHOT_DIRECTORY "/home/abdulwahab/documents/projects-010/screenshot/"
#define TEMPORARY_SCREENSHOT_FILE "temp.temp.temp.DELETE_ME_IF_I_EXIST.png"
#define FONT_NAME "DejaVuSansMono.ttf"

int main(){
	system("shotgun " TEMPORARY_SCREENSHOT_DIRECTORY TEMPORARY_SCREENSHOT_FILE);

	sfRenderWindow* window = sfRenderWindow_create( (sfVideoMode){.width=1366, .height=768, .bitsPerPixel=24}, "Save Screenshot", sfFullscreen, NULL);
	sfRenderWindow_setFramerateLimit(window, 30);
	//sfRenderWindow_setPosition(window, (sfVector2i){.x=400, .y=130});

	sfImage* originalImage = sfImage_createFromFile(TEMPORARY_SCREENSHOT_DIRECTORY TEMPORARY_SCREENSHOT_FILE);

	sfTexture* dispImageT = sfTexture_createFromFile(TEMPORARY_SCREENSHOT_DIRECTORY TEMPORARY_SCREENSHOT_FILE, NULL);

	sfSprite* dispImageWhole = sfSprite_create();
		sfSprite_setTexture(dispImageWhole, dispImageT, sfFalse);
		sfSprite_setColor(dispImageWhole, (sfColor){ 255, 255, 255, 130 });

	sfRectangleShape* dispImageSelectedBorder = sfRectangleShape_create();
		sfRectangleShape_setSize(dispImageSelectedBorder, (sfVector2f){0.0f, 0.0f});
		sfRectangleShape_setOutlineThickness(dispImageSelectedBorder, 2.0f);
		sfRectangleShape_setOutlineColor(dispImageSelectedBorder, sfRed);

	sfSprite* dispImageSelected = sfSprite_create();
		sfSprite_setTexture(dispImageSelected, dispImageT, sfFalse);
	
	int editingSelectedRegion = 0; // false
	sfIntRect selectedRegion = (sfIntRect){0, 0, 0, 0};
		sfSprite_setTextureRect(dispImageSelected, selectedRegion);
	
	sfFont* dispRegionSizeFont = sfFont_createFromFile(TEMPORARY_SCREENSHOT_DIRECTORY FONT_NAME);
	sfText* dispRegionSize = sfText_create();
		sfText_setFont(dispRegionSize, dispRegionSizeFont);
		sfText_setCharacterSize(dispRegionSize, 25);
		sfText_setFillColor(dispRegionSize, sfRed);
		sfText_setOutlineColor(dispRegionSize, sfBlack);
		sfText_setOutlineThickness(dispRegionSize, 3.0f);
	char dispRegionSizeString[4+1+4+1]; 	// 4 characters to display the window width, 
						// 1 for the "x" (to indicate dimensions, width x height), 
						// 4 more chars for the height, 
						// 1 char for the null-termination character
	while(sfRenderWindow_isOpen(window)){
		sfEvent e;
		while(sfRenderWindow_pollEvent(window, &e)){
			if(e.type == sfEvtClosed){
				sfRenderWindow_close(window);
			}else if(e.type == sfEvtTextEntered){
				if(e.text.unicode == 27){
					sfRenderWindow_close(window);
				}
			}
		}
		if(!editingSelectedRegion){
			if(sfMouse_isButtonPressed(sfMouseLeft)){
				editingSelectedRegion = 1;
				selectedRegion.left = sfMouse_getPosition(window).x;
				selectedRegion.top = sfMouse_getPosition(window).y;
				selectedRegion.width = 0;
				selectedRegion.height= 0;
				sfSprite_setTextureRect(dispImageSelected, selectedRegion);
				sfSprite_setPosition(dispImageSelected, (sfVector2f){selectedRegion.left, selectedRegion.top});

				sfRectangleShape_setSize(dispImageSelectedBorder, (sfVector2f){selectedRegion.width, selectedRegion.height});
				sfRectangleShape_setPosition(dispImageSelectedBorder, (sfVector2f){selectedRegion.left, selectedRegion.top});
			}
		}else{
			if(!sfMouse_isButtonPressed(sfMouseLeft)){
				editingSelectedRegion = 0;
			}
			selectedRegion.width = sfMouse_getPosition(window).x  - selectedRegion.left;
			selectedRegion.height = sfMouse_getPosition(window).y - selectedRegion.top;
			if(sfKeyboard_isKeyPressed(sfKeyLShift)){
				selectedRegion.width -= selectedRegion.width % 10;
				selectedRegion.height -= selectedRegion.height % 10;
			}
			sfSprite_setTextureRect(dispImageSelected, selectedRegion);

			sfRectangleShape_setSize(dispImageSelectedBorder, (sfVector2f){selectedRegion.width, selectedRegion.height});

			sprintf(dispRegionSizeString, "%04dx%04d", selectedRegion.width, selectedRegion.height);
			sfText_setString(dispRegionSize, dispRegionSizeString);
			sfText_setPosition(dispRegionSize, (sfVector2f){selectedRegion.left+selectedRegion.width+5, selectedRegion.top});

		}
			if(sfKeyboard_isKeyPressed(sfKeyLShift)){
				if(sfKeyboard_isKeyPressed(sfKeyS)){
					//save to file
					sfImage* output = sfImage_create( selectedRegion.width, selectedRegion.height ); /*filename*/
					for(int y = 0; y < selectedRegion.height; y++){
						for(int x = 0; x < selectedRegion.width; x++){
							sfImage_setPixel(output, x, y, sfImage_getPixel(originalImage, selectedRegion.left+x, selectedRegion.top+y));
						}
					}
					sfImage_saveToFile(output, TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/TEMP_TEMP_TEMP_TEMP_output.png");
					system("mv " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/TEMP_TEMP_TEMP_TEMP_output.png " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/screenshot-\"$(ls " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/ | wc -l)\".png");
					sfRenderWindow_close(window);
					break;
				}else if(sfKeyboard_isKeyPressed(sfKeyC)){
					sfImage* output = sfImage_create( selectedRegion.width, selectedRegion.height ); /*filename*/
					for(int y = 0; y < selectedRegion.height; y++){
						for(int x = 0; x < selectedRegion.width; x++){
							sfImage_setPixel(output, x, y, sfImage_getPixel(originalImage, selectedRegion.left+x, selectedRegion.top+y));
						}
					}
					sfImage_saveToFile(output, TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/TEMP_TEMP_TEMP_TEMP_output.png");
					system("cat " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/TEMP_TEMP_TEMP_TEMP_output.png | xclip -selection clipboard -target image/png -i");
					sfRenderWindow_close(window);
					break;
				}
			}
		sfRenderWindow_clear(window, sfBlack);
		sfRenderWindow_drawSprite(window, dispImageWhole, NULL);
		sfRenderWindow_drawRectangleShape(window, dispImageSelectedBorder, NULL);
		sfRenderWindow_drawSprite(window, dispImageSelected, NULL);
		sfRenderWindow_drawText(window, dispRegionSize, NULL);
		sfRenderWindow_display(window);
	}
	/*
*/
}
