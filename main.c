#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

//create a subdirctory under TEMPORARY_SCREENSHOT_DIRECTORY, name it "screenshots". There, your screenshots will be stored.
#define TEMPORARY_SCREENSHOT_DIRECTORY "/home/abdulwahab/documents/projects-010/screenshot/"
#define TEMPORARY_SCREENSHOT_FILE "temp.temp.temp.DELETE_ME_IF_I_EXIST.png"
#define FONT_NAME "DejaVuSansMono.ttf"


/*
bool isLess(sfVector2f a, sfVector2f b){
	return a.x < b.x || a.y < b.y;
}
*/

/*
int whichIsFirst(std::string a, std::string b){
	if(a.length() == 0){
		return 0;
	}
	if(b.length () == 0){
		return 1;
	}
	int shorterLength = a.length();
	if(b.length() < shorterLength){
		shorterLength = b.length();
	}
	for(int i = 0; i < shorterLength; i++){
		if(a[i] < b[i]){
			return 0;
		}else if(b[i] < a[i]){
			return 1;
		}
	}
	if(a.length() < b.length()){
		return 0;
	}else{
		return 1;
	}
	return 0; // this is IMPOSSIBLE!
}
*/

int isValidCharForFilenames(char a_character){
	if (a_character >= 65 && a_character <= 90){
		return 1;
	}
	if(a_character >= 97 && a_character <= 122){
		return 1;
	}
	if(a_character >= 48 && a_character <= 57){
		return 1;
	}
	switch(a_character){
		case ' ': return 1;
		case '.': return 1;
		case '-': return 1;
		case '_': return 1;
	default: return 0;
	}
	return 0;
}

void updateFilenames(DIR* dir, char* directory, char* allFiletypes, char** allFilenames, int* filesInDir){
	if( (dir=opendir(directory)) == NULL){
		printf("Error opening directory!\n");
		return;
	}
	//allFilenames[0] = "..";
	for(int i = 1; i < 100; i++){
		for(int j = 0; j < 100; j++){
			allFilenames[i][j] = 0;
		}
		//printf("lel\n");
		//allFilenames[i] = "";
		//strcpy(allFilenames[i], j);
	}
	/*
	int fileCount = 1; // ..
	while((ent=readdir(dir)) != NULL){
		if(ent->d_name[0] == '.'){
			continue;
		}
		fileCount++;
	}
	*/
	//dir=opendir(directory);
	int i = 1;
	struct dirent* ent;
	while((ent=readdir(dir)) != NULL){
		if(i >= 100) break;
		if(ent->d_name[0] == '.'){
			continue;
		}
		//allFilenames[i] = ent->d_name;
		//printf("%s filename\n", ent->d_name);

		/*
		for(int a = 0; a < strlen(ent->d_name); a++){
			printf("%d, %d\n", i, a);
			allFilenames[i][a] = ent->d_name[a];
		}
		*/

		strcpy(allFilenames[i], ent->d_name);
		allFiletypes[i] = ent->d_type;
		//printf("%d %s\n", allFiletypes[i], allFilenames[i]);
		i++;
	}
	*filesInDir = i;
	closedir(dir);
	/*
	for(i = 0; i < *filesInDir; i++){
		printf("%d %s\n", allFiletypes[i], allFilenames[i]);
	}
	*/
}

void setFilenames(char** allFilenames){
	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 100; j++){
	//printf("hi\n");
			allFilenames[i][j] = rand() % 26 + 65;
		}
	}
}

/*
int attemptToDescend(DIR* dir, char* directory, const char* subdirectory){
	printf("currently in %s, now going down to %s.\n", directory, subdirectory);
	int len = strlen(directory);
	int len2 = strlen(subdirectory);
	char newDirectory[len+len2+2];
	strcpy(newDirectory, directory);
	for(int i = 0; i < len2; i++){
		newDirectory[i+len] = subdirectory[i];
	}
	newDirectory[len+len2] = '/';
	newDirectory[len+len2+1] = 0;
	dir = opendir(newDirectory);
	if( dir != NULL ){
		strcpy(directory, newDirectory);
		printf("%s new dir, lel\n", directory);
		closedir(dir);
		return 1;
	}else{
		printf("Error opening directory: %s\n", newDirectory);
		return 0;
	}
}
*/

int attemptToDescend(DIR* dir, char* directory, int indexOfSubdirectoryString, const char* allFiletypes, const char** allFilenames){
	if(allFiletypes[indexOfSubdirectoryString] != 4){ // 4 = dir
		printf("Refusing to enter directory %s which is probably an ordinary file.\n", allFilenames[indexOfSubdirectoryString]);
		return 0; // failed to move to directory (it's not a directory)
	}
	//printf("AS OF NOW: in %s, now going down to %s.\n", directory, allFilenames[indexOfSubdirectoryString]);
	int len = strlen(directory);
	int len2 = strlen(allFilenames[indexOfSubdirectoryString]);
	char newDirectory[len+len2+2];
	strcpy(newDirectory, directory);
	for(int i = 0; i < len2; i++){
		newDirectory[i+len] = allFilenames[indexOfSubdirectoryString][i];
	}
	newDirectory[len+len2] = '/';
	newDirectory[len+len2+1] = 0;
	dir = opendir(newDirectory);
	if( dir != NULL ){
		strcpy(directory, newDirectory);
		//printf("%s new dir, lel\n", directory);
		closedir(dir);
		return 1;
	}else{
		printf("Error opening directory: %s\n", newDirectory);
		return 0;
	}
}

char* concat(char* a, char* b){
	int length = strlen(a)+strlen(b) + 2;
	char* returned = malloc( /*sizeof(char) * */ length);
	for(int i = 0; i < length; i++){
		returned[i] = 0;
	}
	strcat(returned, a);
	strcat(returned, b);
	//strcpy(returned, a);
	//strcpy(&returned[strlen(a)], b);
	//printf("returned: %s\n", returned);
	return returned;
}

int main(){
	system("shotgun " TEMPORARY_SCREENSHOT_DIRECTORY TEMPORARY_SCREENSHOT_FILE);

	sfRenderWindow* window = sfRenderWindow_create( (sfVideoMode){.width=1366, .height=768, .bitsPerPixel=24}, "Take Screenshot", sfFullscreen, NULL);
	sfRenderWindow_setFramerateLimit(window, 40);
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
	sfVector2i endPoint = (sfVector2i){0, 0};
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
	int savingToFile = 0; // 0 = false, 1 = true

	sfImage* output;
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
			endPoint = sfMouse_getPosition(window);
			selectedRegion.width = endPoint.x  - selectedRegion.left;
			selectedRegion.height = endPoint.y - selectedRegion.top;

			/*
			if(selectedRegion.width < 0){
				selectedRegion.width  = -selectedRegion.width;
				selectedRegion.left  -=  selectedRegion.width;

				sfRectangleShape_setPosition(dispImageSelectedBorder, (sfVector2f){selectedRegion.left, selectedRegion.top});
				sfSprite_setPosition(dispImageSelected, (sfVector2f){selectedRegion.left, selectedRegion.top});
			}
			if(selectedRegion.height < 0){
				selectedRegion.height = -selectedRegion.height;
				selectedRegion.top   -=  selectedRegion.height;

				sfRectangleShape_setPosition(dispImageSelectedBorder, (sfVector2f){selectedRegion.left, selectedRegion.top});
				sfSprite_setPosition(dispImageSelected, (sfVector2f){selectedRegion.left, selectedRegion.top});
			}
			*/
			if(sfKeyboard_isKeyPressed(sfKeyLShift)){
				selectedRegion.width -= selectedRegion.width % 10;
				selectedRegion.height -= selectedRegion.height % 10;
			}
			//sfSprite_setPosition(dispImageSelected, (sfVector2f){selectedRegion.left, selectedRegion.top});

			sfIntRect region = selectedRegion;
			if(region.width < 0){
				region.width *= -1;
				region.left -= region.width;
			}
			if(region.height < 0){
				region.height *= -1;
				region.top -= region.height;
			}
			sfSprite_setPosition(dispImageSelected, (sfVector2f){region.left, region.top});
			sfSprite_setTextureRect(dispImageSelected, region);

			sfRectangleShape_setSize(dispImageSelectedBorder, (sfVector2f){selectedRegion.width, selectedRegion.height});

			sprintf(dispRegionSizeString, "%04dx%04d", selectedRegion.width, selectedRegion.height);
			sfText_setString(dispRegionSize, dispRegionSizeString);
			sfText_setPosition(dispRegionSize, (sfVector2f){selectedRegion.left+selectedRegion.width+5, selectedRegion.top});

		}
			if(sfKeyboard_isKeyPressed(sfKeyLShift)){
				if(sfKeyboard_isKeyPressed(sfKeyS)){
					//save to file
					if(selectedRegion.width < 0){
						selectedRegion.width *= -1;
						selectedRegion.left -= selectedRegion.width;
					}else if(selectedRegion.width == 0){
						printf("Error! Width of selected region is 0.\nRefusing to do anything...\n");
						sfRenderWindow_close(window);
						break;
					}
					if(selectedRegion.height < 0){
						selectedRegion.height *= -1;
						selectedRegion.top -= selectedRegion.height;
					}else if(selectedRegion.height == 0){
						printf("Error! Height of selected region is 0.\nRefusing to do anything...\n");
						sfRenderWindow_close(window);
						break;
					}
					output = sfImage_create( selectedRegion.width, selectedRegion.height ); /*filename*/
					for(int y = 0; y < selectedRegion.height; y++){
						for(int x = 0; x < selectedRegion.width; x++){
							sfImage_setPixel(output, x, y, sfImage_getPixel(originalImage, selectedRegion.left+x, selectedRegion.top+y));
						}
					}
					savingToFile = 1;
					/*
					sfImage_saveToFile(output, TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/TEMP_TEMP_TEMP_TEMP_output.png");
					system("mv " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/TEMP_TEMP_TEMP_TEMP_output.png " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/screenshot-\"$(ls " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/ | wc -l)\".png");
					sfRenderWindow_close(window);
					*/
					sfRenderWindow_close(window);
					break;
				}else if(sfKeyboard_isKeyPressed(sfKeyC)){
					if(selectedRegion.width < 0){
						selectedRegion.width *= -1;
						selectedRegion.left -= selectedRegion.width;
					}else if(selectedRegion.width == 0){
						printf("Error! Width of selected region is 0.\nRefusing to do anything...\n");
						//note: as an alternative to QUITTING THE ENTIRE PROGRAM, maybe just do nothing at all, and let the user re-select the region?
						//but if you quit the program, maybe the user wants that, like maybe they change their mind and say "I don't want to save/copy this image anymore."
						//You already have the mod+Shift+Q shortcut to close the program, but if I were to (for example) give this program to someone else, they might not have i3-gaps like I do.
						//TODO: Add other ways to exit the program.
						sfRenderWindow_close(window);
						break;
						//return 0;
					}
					if(selectedRegion.height < 0){
						selectedRegion.height *= -1;
						selectedRegion.top -= selectedRegion.height;
					}else if(selectedRegion.height == 0){
						printf("Error! Height of selected region is 0.\nRefusing to do anything...\n");
						sfRenderWindow_close(window);
						break;
					}
					sfImage* output = sfImage_create( selectedRegion.width, selectedRegion.height ); /*filename*/
					for(int y = 0; y < selectedRegion.height; y++){
						for(int x = 0; x < selectedRegion.width; x++){
							sfImage_setPixel(output, x, y, sfImage_getPixel(originalImage, selectedRegion.left+x, selectedRegion.top+y));
						}
					}
					//printf("HEEYA GO!\n");
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
	if(savingToFile == 1){
		int saveAsDialogBoxWidth  = 500;
		int saveAsDialogBoxHeight = 500;
		window = sfRenderWindow_create( (sfVideoMode){.width=saveAsDialogBoxWidth, .height=saveAsDialogBoxWidth, .bitsPerPixel=24}, "Save Screenshot", sfClose, NULL);
		sfRenderWindow_setFramerateLimit(window, 40);
		sfRenderWindow_setPosition(window, (sfVector2i){.x=250, .y=130});
		sfRectangleShape* displayFileBackground = sfRectangleShape_create();
		int displayFileBackgroundHeight = 23;
			sfRectangleShape_setSize(displayFileBackground, (sfVector2f){.x=sfRenderWindow_getSize(window).x, displayFileBackgroundHeight});
			sfRectangleShape_setFillColor(displayFileBackground, (sfColor){20, 20, 20, 255});
			sfRectangleShape_setPosition(displayFileBackground, (sfVector2f){0, 0});

		DIR* dir;
		//if( (dir=opendir(TEMPORARY_SCREENSHOT_DIRECTORY)) == NULL){
		//	printf("Error opening directory!\n");
		//	return 0;
		//}
		struct dirent* ent;
		int filesInDir = 1;
		//int fileCount = 1; // ..
		/*
		while((ent=readdir(dir)) != NULL){
			if(ent->d_name[0] == '.'){
				continue;
			}
			//fileCount++;
		}
		closedir(dir);
		*/
		char filetypes[128];
		//char* filenames[128];
		//char filenames[128][128];

		/*
		*/
		char** filenames = malloc(sizeof(char*) * 128);
		for(int i = 0; i < 128; i++){
			filenames[i] = malloc(sizeof(char) * 128);
		}

		
		filenames[0][0] = '.';
		filenames[0][1] = '.';
		filenames[0][2] = 0;
		/*
		filenames[0] = "..";
		*/
		char directory[200] = TEMPORARY_SCREENSHOT_DIRECTORY;
		/*
		dir=opendir(directory);
		int i = 1;
		while((ent=readdir(dir)) != NULL){
			if(ent->d_name[0] == '.'){
				continue;
			}
			filenames[i] = ent->d_name;
			i++;
			filesInDir++;
		}
		*/

		updateFilenames(dir, directory, filetypes, filenames, &filesInDir);
		//setFilenames(filenames); filesInDir = 100;
		
		//for(int i = 0; i < filesInDir; i++) printf("%s\n", filenames[i]);
		char fileToSaveAs[100];
		int fileToSaveAs_cursorLoc = 0;
		int fileToSaveAs_cursorLocMax = 100;

		sfText* displayFileName = sfText_create();
			sfText_setFont(displayFileName, dispRegionSizeFont);
			sfText_setCharacterSize(displayFileName, 14);
			sfText_setFillColor(displayFileName, sfWhite);

		sfText* displayFileToSaveAs = sfText_create();
			sfText_setFont(displayFileToSaveAs, dispRegionSizeFont);
			sfText_setCharacterSize(displayFileToSaveAs, 17);
			sfText_setFillColor(displayFileToSaveAs, sfWhite);
			sfText_setPosition(displayFileToSaveAs, (sfVector2f){30, sfRenderWindow_getSize(window).y - 25});
		sfVector2i mousePos;
		int jc = 0;

		int firstFileToDisplay = 0;
		int filesToDisplay = 30;
		int jpScroll = 0;
		while(sfRenderWindow_isOpen(window)){
			sfEvent e;
			while(sfRenderWindow_pollEvent(window, &e)){
				if(e.type == sfEvtClosed){
					sfRenderWindow_close(window);
				}else if(e.type == sfEvtTextEntered){
					if(e.text.unicode == 27){
						sfRenderWindow_close(window);
					}else if(isValidCharForFilenames(e.text.unicode)){
						if(fileToSaveAs_cursorLoc < fileToSaveAs_cursorLocMax-1){
							if(fileToSaveAs_cursorLoc == -1){
								fileToSaveAs_cursorLoc = 0;
							}
							fileToSaveAs[fileToSaveAs_cursorLoc] = e.text.unicode;
							fileToSaveAs_cursorLoc++;
							sfText_setString(displayFileToSaveAs, fileToSaveAs);
						}
					}else if(e.text.unicode == 8){
						if(fileToSaveAs_cursorLoc >= 0){
							fileToSaveAs_cursorLoc--;
							fileToSaveAs[fileToSaveAs_cursorLoc] = 0;
							sfText_setString(displayFileToSaveAs, fileToSaveAs);
						}
					}else if(e.text.unicode == 13){
						char* xd = concat(directory, fileToSaveAs);
						//printf("xd: %s\n", xd);
						sfImage_saveToFile(output, xd);
						sfRenderWindow_close(window);
						free(xd);
						break;
					}
				}else if(e.type == sfEvtMouseWheelScrolled){
					//printf("fowkjf %f\n", e.mouseWheelScroll.delta);
					if(e.mouseWheelScroll.delta < 0.0f){
						if(firstFileToDisplay < filesInDir-1){
							firstFileToDisplay += 1;
						}
					}else{
						if(firstFileToDisplay > 0){
							firstFileToDisplay -= 1;
						}
					}
					//firstFileToDisplay -= e.mouseWheelScroll.delta;
				}
			}
			sfRenderWindow_clear(window, sfBlack);
			mousePos = sfMouse_getPosition(window);
			//printf("DIR: %s\n", directory);
			for(int i = firstFileToDisplay; i < firstFileToDisplay+filesToDisplay; i++){
				/*
				if(i-firstFileToDisplay*displayFileBackgroundHeight > saveAsDialogBoxHeight){
					break;
				}
				*/
				if(filenames[i] == "") break;
				sfRectangleShape_setFillColor(displayFileBackground, (sfColor){20, 20, 20, 255});
				if(mousePos.x > 0 && mousePos.x < saveAsDialogBoxWidth){
					if(mousePos.y >= (i-firstFileToDisplay)*displayFileBackgroundHeight && mousePos.y < (i-firstFileToDisplay+1)*displayFileBackgroundHeight){
						sfRectangleShape_setFillColor(displayFileBackground, (sfColor){50, 50, 50, 255});
						if(sfMouse_isButtonPressed(sfMouseLeft)){
							if(jc) continue;
							jc = 1;
							//if(filenames[i] == ".."){
							if(i == 0){
								if(strlen(directory) > 1){
									for(int j = strlen(directory)-2; j > 0; j--){
										if(directory[j] == '/'){
											//printf("%s new dir\n", directory);
											break;
										}
										directory[j] = 0;
									}
									//printf("GOING UP!!\n");
									updateFilenames(dir, directory, filetypes, filenames, &filesInDir);
									break;
								}
							}else{
								int result = attemptToDescend(dir, directory, i, filetypes, filenames);
								if(result == 1){ // if a directory
									updateFilenames(dir, directory, filetypes, filenames, &filesInDir);
								}else{
									strcpy(fileToSaveAs, filenames[i]);
									sfText_setString(displayFileToSaveAs, fileToSaveAs);
									fileToSaveAs_cursorLoc = strlen(fileToSaveAs);
								}
							}
						}else{
							jc = 0;
						}
					}
				}
				sfRectangleShape_setPosition(displayFileBackground, (sfVector2f){0, (i-firstFileToDisplay)*displayFileBackgroundHeight});
				sfRenderWindow_drawRectangleShape(window, displayFileBackground, NULL);
				sfText_setPosition(displayFileName, (sfVector2f){0, (i-firstFileToDisplay)*displayFileBackgroundHeight});
				sfText_setString(displayFileName, filenames[i]);
				//if(sfKeyboard_isKeyPressed(sfKeyA)){
				//printf("\t%d %s of len %d, first char '%c'\n", i, filenames[i], strlen(filenames[i]), filenames[i][0]);
				//}
				if(sfKeyboard_isKeyPressed(sfKeyU)){
				updateFilenames(dir, directory, filetypes, filenames, &filesInDir);
				}
				sfRenderWindow_drawText(window, displayFileName, NULL);
			}
			
				/*
			if(sfKeyboard_isKeyPressed(sfKeyT)){ if(jpScroll==0)firstFileToDisplay--;jpScroll=1; }
			else if(sfKeyboard_isKeyPressed(sfKeyG)){ if(jpScroll==0)firstFileToDisplay++;jpScroll=1; }
			else jpScroll = 0;
			*/

			/*
			if(sfKeyboard_isKeyPressed(sfKeyA))
				for(int i=0;i<filesInDir;i++){
				//printf("\t%d %s of len %d, chars::: ", i, filenames[i], strlen(filenames[i]));
				for(int j=0;j<strlen(filenames[i]);j++){
					printf("'%c' ", filenames[i][j]);
				}
					printf("\n");
				}
			*/
			sfRenderWindow_drawText(window, displayFileToSaveAs, NULL);
			sfRenderWindow_display(window);
			
			// for some reason, it fails to display properly unless I RELOAD the text??? WTF??
			// TODO: FIX THIS GARBAGE
			/*
			if(aafdsa++ <= 1){
				updateFilenames(dir, directory, filetypes, filenames, &filesInDir);
			}
			*/
		}
	}
	//sfImage* output = sfImage_create( selectedRegion.width, selectedRegion.height ); /*filename*/
					//system("mv " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/TEMP_TEMP_TEMP_TEMP_output.png " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/screenshot-\"$(ls " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/ | wc -l)\".png");
	/*
					sfImage_saveToFile(output, TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/TEMP_TEMP_TEMP_TEMP_output.png");
					system("mv " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/TEMP_TEMP_TEMP_TEMP_output.png " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/screenshot-\"$(ls " TEMPORARY_SCREENSHOT_DIRECTORY "screenshots/ | wc -l)\".png");
					*/
	/*
*/
}
