#include <SFML/Graphics.hpp>
#include <vector>
#include<iostream>

using namespace std;

class Block
{
 private:
   int Fcost, Hcost, Gcost;
   char property;;
 public:
    sf::RectangleShape rect;

    bool isSource(){
      return property == 's';
    }
    bool isDestination(){
      return property == 'd';
    }
    bool isObstacle(){
      return property == 'o';
    }
    bool isBlank(){
      return property == 'b';
    }
    void setProp(char p){
      property = p;
    }

    char getProp(){
      return property;
    }


    Block(int x,int y, int w,int h){
        rect.setPosition(x,y);
        rect.setSize(sf::Vector2f(w,h));
    }
    Block(){}
};


vector<vector<Block>> generateGrid(){
  // Create Grid
  int rows = 100;
  int columns = 100;
  vector<vector<Block>> grid(rows,vector<Block>(columns));
  sf::Vector2f cellSize(15.0f, 15.0f);
  for(int i=0;i<rows;i++){
          for(int j=0;j<columns;j++){
              grid[i][j].rect.setSize(cellSize);
              grid[i][j].rect.setOutlineColor(sf::Color::Black);
              grid[i][j].rect.setOutlineThickness(1.0f);
              grid[i][j].rect.setPosition(i*cellSize.x + 5.0f, j*cellSize.y + 5.0f);
              grid[i][j].setProp('b');
          }
  }
  return grid;
  // End Create Grid
}

vector<vector<int>> findSurronding(Block currBlock,vector<vector<Block>> grid,int rows,int columns){
  //vector<Block> minVector;
  vector<vector<int>> minVector;
  for(int i=0;i<rows;i++){
          for(int j=0;j<columns;j++){
              if(grid[i][j].rect.getPosition() == currBlock.rect.getPosition()){
                /*
                //Top Row
                minVector.push_back(grid[i-1][j-1]);
                minVector.push_back(grid[i-1][j]);
                minVector.push_back(grid[i-1][j+1]);
                // Middle or Current
                minVector.push_back(grid[i][j-1]);
                minVector.push_back(grid[i][j+1]);
                //Bottom Row
                minVector.push_back(grid[i+1][j-1]);
                minVector.push_back(grid[i+1][j]);
                minVector.push_back(grid[i+1][j+1]);
                */
                //Top Row
                minVector.push_back(vector<int> {i-1,j-1});
                minVector.push_back(vector<int> {i-1,j});
                minVector.push_back(vector<int> {i-1,j+1});
                // Middle or Current
                minVector.push_back(vector<int> {i,j-1});
                minVector.push_back(vector<int> {i,j+1});
                //Bottom Row
                minVector.push_back(vector<int> {i+1,j-1});
                minVector.push_back(vector<int> {i+1,j});
                minVector.push_back(vector<int> {i+1,j+1});
              }
          }
  }
  /*
  for(int p = 0;p<minVector.size();p++){
    if(.getProp() == 'o')
      minVector.erase(minVector.begin()+p);
    //minVector[p].rect.setFillColor(sf::Color::Green);
  }
  */
  return minVector;


  }


int main()
{
    sf::RenderWindow window(sf::VideoMode(900,700), "A* Visualizer");
    Block src,des;
    vector<vector<int>> minVector;
    char mode = 's';
    int rows = 100;
    int columns = 100;
    auto grid = generateGrid();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
              if (event.key.code == sf::Keyboard::Escape){
                minVector = findSurronding(src,grid,rows,columns);
                cout << "Min Vectors Obtained" << endl;
              }
            }
            if (event.type == sf::Event::MouseButtonPressed)
              {
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                // std::cout << localPosition.x << "," << localPosition.y << std::endl;
                for(int i=0;i<columns;i++){
                    for(int j=0;j<rows;j++){
                        sf::FloatRect rect = grid[i][j].rect.getGlobalBounds();
                        if(rect.contains(localPosition.x,localPosition.y)){
                            if (mode == 'd'){
                                grid[i][j].rect.setFillColor(sf::Color::Red);
                                grid[i][j].setProp('d');
                                mode = 'o';
                            }
                            else if(mode == 's'){
                                grid[i][j].rect.setFillColor(sf::Color::Blue);
                                grid[i][j].setProp('s');
                                src = grid[i][j];
                                mode = 'd';
                            }
                            else if(mode == 'o'){
                                grid[i][j].rect.setFillColor(sf::Color::Black);
                                grid[i][j].setProp('o');
                            }
                        }
                }
            }
          }
        }

        /*
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mode == 'o')
        {
          sf::Vector2i localPosition = sf::Mouse::getPosition(window);
          // std::cout << localPosition.x << "," << localPosition.y << std::endl;
          for(int i=0;i<columns;i++){
              for(int j=0;j<rows;j++){
                  sf::FloatRect rect = grid[i][j].rect.getGlobalBounds();
                  if(rect.contains(localPosition.x,localPosition.y)){
                          grid[i][j].rect.setFillColor(sf::Color::Black);
                  }
              }
          }
        }
        */

        window.clear();
        for(int i=0;i<columns;i++){
            for(int j=0;j<rows;j++){
                window.draw(grid[i][j].rect);
            }
        }

        for(int p = 0;p<minVector.size();p++){
          int l = minVector[p][0];
          int k = minVector[p][1];
          grid[l][k].rect.setFillColor(sf::Color::Green);
        }
        window.display();
    }

    return 0;
}
