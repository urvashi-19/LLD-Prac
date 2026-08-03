// Snake & Ladder

// Snake
// Ladder
// Dice
// Board
// User
// Game

#include<bits/stdc++.h>
using namespace std;


class Snake{
    
    private:
    
    int head;
    int tail;
    
    public:
    
    Snake(int head ,int  tail){
        this->head = head;
        this->tail = tail;
    }
    
    
    int getHead(){
        return head;
    }
    
    int getTail(){
        return tail;
    }
};


class Ladder{
    
    private:
    
    int start;
    int end;
    
    public:
    
    Ladder(int start , int end){
        this->start = start;
        this->end = end;
    }
    
     int getStart(){
        return start;
    }
    
    int getEnd(){
        return end;
    }
};

class User{
    
    private:
    
    string name;
    int score;
    
    public:
    
      User(string name){
          this->name = name;
          this->score = 0;
      }
      
      void scoreScored(int score){
          this->score = score;
      }
      
      int getScore(){
          return score;
      }
      
      string getName(){
          return name;
      }
};


class Dice{
    
    public:
    
    int roll(){
        return rand()%6 +1 ;
    }
};


class Board{
    
    private :
    
    int n;
    unordered_map<int , Snake*>snakes;
    unordered_map<int , Ladder*>ladders;
    
    
    public:
    
    Board(int n){
        this->n = n ;
    }
    
    
    int size(){
        return n*n;
    }
    
    void addSnake(int pos , Snake* s){
        snakes[pos] = s;
    }
    
     void addLadder(int pos , Ladder* l){
        ladders[pos] = l;
    }
    
    
     int getPosition(int step , User * u){
         int currPos = u->getScore() + step;
         
         
          if(currPos > size()){
             return currPos = u->getScore();
         }
         
         if(snakes.find(currPos) != snakes.end()){
             currPos = snakes[currPos]->getTail();
         }
         
         else if(ladders.find(currPos) != ladders.end()){
             currPos = ladders[currPos]->getEnd();
         }
         
         u->scoreScored(currPos);
         
         return currPos;
    
     }
    
};


class Game{
    private:
    
    vector<User*>player;
    Board* board;
    Dice* d;
    User* winner;
    
    public:
    
    Game(Board* board , Dice* d , vector<User*>player){
        this->board = board;
        this->d = d;
        this->player = player;
        winner = nullptr;
    }
    
    void start(){
        
        cout<<" Game  Started!!! "<<endl;
        while(true){
            
            bool flag = false;
            for(User* u : player){
                int diceNumber = d->roll();
                
                int pos = board->getPosition(diceNumber , u);
                
                if(pos==board->size()){
                    winner = u;
                    flag = true;
                    break;
                }
                
               
            }
            
            if(flag == true) break;
        }
        
        cout<<" Game  Ended!!! "<<endl;
    }
    
    
    User* getWinner(){
        if(winner){
             cout<<" winner found!!! "<<winner->getName()<<endl;
            return winner;
        }
        
        cout<<"No winner found!!!"<<endl;
        return nullptr;
            
    }
};



int main(){
    
    Snake* s1 = new Snake(27 , 4);
    Snake* s2 = new Snake(89 , 51);
    
    
    Ladder* l1 = new Ladder(11 , 43);
    Ladder* l2 = new  Ladder(51 , 78);
    
    
    User* p1 = new User("Urvashi");
    User* p2 = new User("Kartik");
    vector<User*>players;
    players.push_back(p1);
    players.push_back(p2);
    
    
    cout<<"Tell size of board"<<endl;
    int n; cin>>n;
    
    
    Board* b = new Board(n);
    b->addSnake(27, s1);
    b->addSnake(89, s2);

    b->addLadder(11, l1);
    b->addLadder(51, l2);
    
    Dice*d = new Dice();
    
    Game* g = new Game(b , d , players);
    
    g->start();
    g->getWinner();
    
    cout<<"Exit "<<endl;
    
   
    
    
    
    
}