#include <SFML/Graphics.hpp>
#include <vector>
#include<iostream>
#include <cmath>

using namespace std;

class Block
{
 private:
   int Fcost, Hcost, Gcost;
   char property;
 public:
    int row,col;
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
              grid[i][j].row = i;
              grid[i][j].col = j;
              grid[i][j].setProp('b');
          }
  }
  return grid;
  // End Create Grid
}

vector<vector<Block>> grid;

void initGrid(){
  grid = generateGrid();
}

vector<vector<int>> findSurronding(Block currBlock,int rows,int columns){
  vector<vector<int>> minVector;
  for(int i=0;i<rows;i++){
          for(int j=0;j<columns;j++){
              if(grid[i][j].rect.getPosition() == currBlock.rect.getPosition()){
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
  vector<vector<int>> surVector;

  for(int p = 0;p<minVector.size();++p){
    int a = p;
    int l = minVector[p][0];
    int k = minVector[p][1];
    if(grid[l][k].isBlank()){
      surVector.push_back(minVector[p]);
    }
  }
  return surVector;
  }

vector<int> findMinimum(vector<vector<int>> surVector,Block destination){
  int f_cost_min = 100000;
  vector<int> minVector;
  int goal_x = destination.row;
  int goal_y = destination.col;
  int h_cost;
  for(int p = 0;p < surVector.size();p++){
    int curr_x = surVector[p][0];
    int curr_y = surVector[p][1];

    h_cost = sqrt(pow(curr_x-goal_x,2) + pow(curr_y-goal_y,2));
    if(h_cost<f_cost_min){
      f_cost_min = h_cost;
      minVector = surVector[p];
    }
  }
  return minVector;
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(900,700), "A* Visualizer");
    Block src,des;
    Block curr;
    vector<vector<int>> surVector;
    vector<int> minVector;
    vector<vector<int>> path;
    char mode = 's';
    int rows = 100;
    int columns = 100;
    initGrid();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
              if (event.key.code == sf::Keyboard::S){
                surVector = findSurronding(curr,rows,columns);
                minVector = findMinimum(surVector,des);
                path.push_back(minVector);
                curr = grid[minVector[0]][minVector[1]];
                //cout << minVector[0] << " " << minVector[1];
                mode = 'b';
              }
              if (event.key.code == sf::Keyboard::R){
                window.close();
                main();
              }

            }
            if (event.type == sf::Event::MouseButtonPressed && mode != 'b')
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
                                des = grid[i][j];
                                mode = 'o';
                            }
                            else if(mode == 's'){
                                grid[i][j].rect.setFillColor(sf::Color::Blue);
                                grid[i][j].setProp('s');
                                src = grid[i][j];
                                curr = src;
                                mode = 'd';
                            }
                            else if(mode == 'o'){
                                grid[i][j].rect.setFillColor(sf::Color::Black);
                                grid[i][j].setProp('o');
                                mode = 'c';
                            }
                        }
                }
            }
          }
        }


        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mode == 'c')
        {
          sf::Vector2i localPosition = sf::Mouse::getPosition(window);
          // std::cout << localPosition.x << "," << localPosition.y << std::endl;
          for(int i=0;i<columns;i++){
              for(int j=0;j<rows;j++){
                  sf::FloatRect rect = grid[i][j].rect.getGlobalBounds();
                  if(rect.contains(localPosition.x,localPosition.y)){
                          grid[i][j].rect.setFillColor(sf::Color::Black);
                          grid[i][j].setProp('o');
                  }
              }
          }
        }



        window.clear();
        for(int p = 0;p<surVector.size();p++){
          int l = surVector[p][0];
          int k = surVector[p][1];
          grid[l][k].rect.setFillColor(sf::Color::Yellow);

        }
        for(int p = 0;p<path.size();p++){
          int l = path[p][0];
          int k = path[p][1];
          grid[l][k].rect.setFillColor(sf::Color::Green);
        }


        for(int i=0;i<columns;i++){
            for(int j=0;j<rows;j++){
                window.draw(grid[i][j].rect);
            }
        }


        window.display();
    }

    return 0;
}
