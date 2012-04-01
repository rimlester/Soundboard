#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string.h>
#include <fstream>
#include <windows.h>
#include <process.h>

const int NUM_COL = 4;
const int MAX_PER_COL = 8;
const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 60;
const int HORIZ_OFFSET = 40;
const int VERT_OFFSET = 20;
const int TOP_OFFSET = 20;
const int W_HEIGHT = 600;
const int W_WIDTH = 800;


int getTopLeftX(int col){
    int ret = (col*(HORIZ_OFFSET+BUTTON_WIDTH) + HORIZ_OFFSET);
    return ret;
}
int getTopLeftY(int row){
    int ret = (row *(VERT_OFFSET+BUTTON_HEIGHT) + TOP_OFFSET);
    return ret;
}

bool checktangle(int col, int row, float x, float y){ //geddit? checktangle, because it checks if a point is in a rectangle huehuehue
    float boxX = x - getTopLeftX(col); //mouse location minus the top left coords of box
    float boxY = y - getTopLeftY(row);
    return (boxX >=0 && boxX < BUTTON_WIDTH && boxY >=0 && boxY < BUTTON_HEIGHT);
}

int howManyInFolder(int folderNumber){
    std::ifstream otr;
    char filepath [10];
    sprintf(filepath, "%d\\n.txt", folderNumber);
    otr.open(filepath);
    int rtn;
    std::string tempBuff;
    std::getline(otr, tempBuff);
    rtn = atoi(tempBuff.c_str());
    otr.close();
    rtn = ((rtn <= MAX_PER_COL) ? rtn: MAX_PER_COL); //make sure they aren't trying to load more buttons then
    return rtn; //there is screen space for
}

int loadBuffers(sf::SoundBuffer bufferArrToLoad[], int colNum){ //load wave files to sound buffer.
    int numWav = howManyInFolder(colNum);
    for(int i = 0; i < numWav; i++){
        char buff[20];
        sprintf(buff, "%d\\%d.wav", colNum, i);
        std::string temp;
        temp.assign(buff);
        bufferArrToLoad[i].LoadFromFile(temp);
    }
    return numWav; //returns number of elements loaded
}

int loadImages(sf::Image images[], int colNum){
    int numImg = howManyInFolder(colNum);
    for(int i = 0; i < numImg; i++ ){
        char buff[20];
        sprintf(buff, "%d\\%d.png", colNum, i);
        std::string temp;
        temp.assign(buff);
        images[i].LoadFromFile(temp);
    }
    return numImg; //returns number of elements loaded

}

void setSprites(sf::Image btimg[], sf::Sprite sprites[], int numImg){
    for(int i = 0; i <  numImg; i++){
        sprites[i].SetImage(btimg[i]);
    }
}

void setSpritesPos(sf::Sprite sprites[], int col, int numInCol){
    for(int i = 0; i < numInCol; i++){
        sprites[i].SetX(getTopLeftX(col));
        sprites[i].SetY(getTopLeftY(i));
    }
}

void setBuffers(sf::SoundBuffer buffs[], sf::Sound sounds[], int numSnds){
    for(int i = 0; i <  numSnds; i++){
        sounds[i].SetBuffer(buffs[i]);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    sf::SoundBuffer soundBuffers[NUM_COL][MAX_PER_COL];
    sf::Sound sounds[NUM_COL][MAX_PER_COL];
    sf::Image buttons[NUM_COL][MAX_PER_COL];
    sf::Sprite btns[NUM_COL][MAX_PER_COL];
    sf::RenderWindow mainWin(sf::VideoMode(W_WIDTH, W_HEIGHT, 32), "Soundboard - by eyeosk", sf::Style::Close);
    sf::Image background;
    sf::Image header;
    sf::Sprite bg;
    sf::Sprite hd;
    hd.SetY(0);
    hd.SetX(W_WIDTH/4);
    bg.SetX(0);
    bg.SetY(0);
    background.LoadFromFile("background.png");
    header.LoadFromFile("header.png");
    bg.SetImage(background);
    hd.SetImage(header);
    int numInCol[NUM_COL];
    for(int i = 0; i < NUM_COL; i++){
        loadBuffers(soundBuffers[i], i);
        numInCol[i] = loadImages(buttons[i], i);
        setSprites(buttons[i], btns[i], numInCol[i]);
        setBuffers(soundBuffers[i], sounds[i], numInCol[i]);
        setSpritesPos(btns[i], i, numInCol[i]);
    }
    mainWin.SetFramerateLimit(360);
    while(mainWin.IsOpened()){
        sf::Event mainEvent;
        while(mainWin.GetEvent(mainEvent)){
            if(mainEvent.Type == sf::Event::Closed  || (mainEvent.Type == sf::Event::KeyPressed && mainEvent.Key.Code == sf::Key::Escape)){
                mainWin.Close();
            }
            if(mainEvent.Type == sf::Event::MouseButtonPressed){
                for(int i = 0; i < NUM_COL; i++){
                    for(int j = 0; j < numInCol[i]; j++){
                        if(checktangle(i, j, mainEvent.MouseButton.X, mainEvent.MouseButton.Y)){
                            sounds[i][j].Play();
                        }
                    }
                }
            }
        }
        mainWin.Clear();
        mainWin.Draw(bg);
        mainWin.Draw(hd);
        for(int i =0; i < NUM_COL; i++){
            for(int j = 0; j < numInCol[i]; j++){
                mainWin.Draw(btns[i][j]);
            }
        }
        mainWin.Display();
    }
    return 0;
}
