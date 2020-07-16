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

int g_cost = 0;
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
bool gameOver = false;
void initGrid(){
  grid = generateGrid();
}

vector<vector<int>> findSurronding(Block currBlock,int rows,int columns){
  try{
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
    if(grid[l][k].isBlank() || grid[l][k].isDestination()){
      surVector.push_back(minVector[p]);
    }
  }
  return surVector;
}
catch(const char* msg){
  cout << msg;
}
  }

vector<int> findMinimum(vector<vector<int>> surVector,Block destination){
  try{
  int f_cost_min = 100000;
  vector<int> minVector;
  int goal_x = destination.row;
  int goal_y = destination.col;
  int h_cost;
  int f_cost;
  //cout << "\n------------------------------------------------" << endl;
  //cout << "Surrounding Blocks";
  if(surVector.size() == 0){
    gameOver = true;
  }
  for(int p = 0;p < surVector.size();p++){
    int curr_x = surVector[p][0];
    int curr_y = surVector[p][1];
    h_cost = sqrt(pow(curr_x-goal_x,2) + pow(curr_y-goal_y,2));
    //cout << "("<< curr_x << "," << curr_y <<") ";
    f_cost = h_cost + g_cost;
    if(curr_x == destination.row &&curr_y == destination.col){
      gameOver = true;
      return surVector[p];
    }
    if(f_cost<f_cost_min){
      f_cost_min = f_cost;
      minVector = surVector[p];
    }
  }
  //cout << "\nMin Blocks";
  //cout << "("<< minVector[0] << "," << minVector[1] <<") ";
  //cout << "\n------------------------------------------------" << endl;
  return minVector;
  }
  catch(const char* msg){

  }

}


int main()
{    try{
    sf::RenderWindow window(sf::VideoMode(1366,720), "A* Visualizer");
    Block src,des;
    Block curr;
    vector<vector<int>> surVector;
    vector<int> minVector;
    vector<vector<int>> path;
    char mode = 's';
    int rows = 100;
    int columns = 100;
    initGrid();
    // Panel
    sf::Font font;
    if (!font.loadFromFile("roboto.ttf"))
    {
        cout << "Failed to Load Font" << endl;
    }
    sf::RectangleShape panel(sf::Vector2f(0, 0));
    panel.setSize(sf::Vector2f(1366.f, 155.f));
    panel.setFillColor(sf::Color::Black);

    // Instructions

    sf::Text inst;
    {
    inst.setString("Instructions: \n\t1. Select Source Node \n\t2. Select Destination Node \n\t3. Draw out the Boundries \n\t4. Press 'S' to start");
    inst.setFont(font);
    inst.setCharacterSize(20);
    inst.setFillColor(sf::Color::White);
    inst.setPosition(10,10);
    }
    sf::Text shorts;
    {
    shorts.setString("Shortcuts: \n\t1. Start -> S \n\t2. Reset -> R \n\t3. Quit -> Q");
    shorts.setFont(font);
    shorts.setCharacterSize(20);
    shorts.setFillColor(sf::Color::White);
    shorts.setPosition(1000,15);
    }


    g_cost = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
              if (event.key.code == sf::Keyboard::S){

                //cout << minVector[0] << " " << minVector[1];
                mode = 'b';
              }
              if (event.key.code == sf::Keyboard::R){
                gameOver = false;
                window.close();
                main();
              }
              if (event.key.code == sf::Keyboard::Q){
                window.close();
              }

            }
            if (event.type == sf::Event::MouseButtonPressed && mode != 'b')
              {
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                // std::cout << localPosition.x << "," << localPosition.y << std::endl;
                for(int i=0;i<columns;i++){
                    for(int j=0;j<rows;j++){
                        sf::FloatRect rect = grid[i][j].rect.getGlobalBounds();
                        sf::FloatRect panelRect = panel.getGlobalBounds();
                        if(rect.contains(localPosition.x,localPosition.y) && !panelRect.contains(localPosition.x,localPosition.y)){
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
        // A Star start
        if(mode == 'b' && !gameOver){
          surVector = findSurronding(curr,rows,columns);
          minVector = findMinimum(surVector,des);
          path.push_back(minVector);
          curr = grid[minVector[0]][minVector[1]];


          for(int p = 0;p<surVector.size();p++){
            int l = surVector[p][0];
            int k = surVector[p][1];
            if(surVector[p][0] != des.row || surVector[p][1] != des.col){
            grid[l][k].rect.setFillColor(sf::Color::Yellow);
          }
          }

          for(int p = 0;p<path.size();p++){
            int l = path[p][0];
            int k = path[p][1];
            grid[l][k].setProp('o');
            grid[l][k].rect.setFillColor(sf::Color::Green);
            grid[des.row][des.col].rect.setFillColor(sf::Color::Red);
            g_cost+=1;
          }
        }


        for(int i=0;i<columns;i++){
            for(int j=0;j<rows;j++){
                window.draw(grid[i][j].rect);
            }
        }

        window.draw(panel);
        window.draw(inst);
        window.draw(shorts);

        window.display();
      }
      }
      catch(const char* msg) {
      }
      return 0;



}
