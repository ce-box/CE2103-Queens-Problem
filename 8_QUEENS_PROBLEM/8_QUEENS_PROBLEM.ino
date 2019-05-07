
 /*                     GNU GENERAL PUBLIC LICENSE
                         Version 3, 29 June 2007

     Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
     Everyone is permitted to copy and distribute verbatim copies
     of this license document, but changing it is not allowed.
 *
 *              8 QUEENS PROBLEM -- SOLVED BY @estalvgs1999
 *  -----------------------------------------------------------------------
 *  Course: Algorithms & Data structures [TEC CR] 2019
 *  Prof.Lic.Luis Diego Noguera
 *
 */

// Libraries
#include <LedControlMS.h>

// Declare methods
void initSolution();
void printBoard();
void showBoard();
bool solve_NQueens();
bool solve_aux(int col);
bool is_safe(int row,int col);

// Maximun of matrix that the system will control
#define matrix_Num 1
#define N 8
#define t 60

/* --------------------------------------------------------------------------
     * Create a new controler
     * Params :
     * dataPin    pin on the Arduino where data gets shifted out
     * clockPin   pin for the clock
     * csPin      pin for selecting the device
     * numDevices maximum number of devices that can be controled

    LedControl(int dataPin DIN:12, int clkPin CLK:11, int csPin CS:10, int numDevices=1);
   --------------------------------------------------------------------------*/

// Create an instance of LedControl
LedControl led_matrix = LedControl(12,11,10,matrix_Num);

int board[N][N]= { {0,0,0,0},
                   {0,0,0,0},
                   {0,0,0,0},
                   {0,0,0,0}};
int *sol = calloc(N,sizeof(int));
bool permisson = true;

void setup() {

  /*--------------------------------------------------------------------------
                    QUICK DOCUMENTATION OF LedControlMS.h
    --------------------------------------------------------------------------
     * Set the shutdown (power saving) mode for the device
     * Params :
     * addr  The address of the display to control
     * status If true the device goes into power-down mode. Set to false
     *    for normal operation.

    void shutdown(int addr, bool status);
     --------------------------------------------------------------------------
     * Set the brightness of the display.
     * Params:
     * addr		the address of the display to control
     * intensity	the brightness of the display. (0..15)

    void setIntensity(int addr, int intensity);
    --------------------------------------------------------------------------
     * Switch all Leds on the display off.
     * Params:
     * addr	address of the display to control

    void clearDisplay(int addr);
    -------------------------------------------------------------------------- */

    for(int i = 0; i < matrix_Num; i++){
        led_matrix.shutdown(i,false); // Activate the matrix
        led_matrix.setIntensity(i,8); // Set brigthness
        led_matrix.clearDisplay(i); // Clear display
    }

    Serial.begin(9600); // Init serial port for comunication
}

void loop() {
  if(permisson){ 
   initSolution();
   solve_NQueens();
   permisson = false;
  }
  // LLEGA A LA SOLUCION #3
       
}

// Shows the board into the led_matrix
void showBoard(){
  for(int col= 0; col<N; col++){
      // Muestra las posiciones donde hay una reina
      if(sol[col]!=-1){
          led_matrix.setLed(0,sol[col],col,true);
      }
  }
}

// Shows the board on console
void printBoard(){
  Serial.println("------BOARD STATUS-------");
    for(int row= 0; row<N; row++){
        for(int col = 0; col <N;col++){
            Serial.print("[");
            Serial.print(board[row][col],DEC);
            Serial.print("]");
        }
        Serial.println();
    }
    Serial.println("-------------------------");
}

// Init the solution array (By default takes -1 in each position)
void initSolution(){
  for (int col = 0; col < N;col++){
      sol[col] = -1;
  }
}

// Main function to solve the problem
bool solve_NQueens(){

    led_matrix.writeString(0,"CE 8QA");
    delay(100);

    //Serial.println(" << N QUEENS PROBLEM >>");
    if(!solve_aux(0)){
        Serial.println("Problem doesn't have solution! :(");
        return false;
    }
    
    Serial.println("Problem solved!");
    printBoard();
    return true;

}

// Backtracking function to solve the algorithm (RECURTION)
bool solve_aux(int col){

    //Serial.print("col->");
    //Serial.println(col,DEC);
    // This is the base case, if the alg can arrive there
    // is because the solution is complete
    if(col >= N)
        
        return true;
    // Iterates the board by rows
    for(int row = 0; row < N; row++){
        
        
        sol[col] = row;
    
        // Shows the movement of the queen
        led_matrix.clearDisplay(0);
        showBoard();
        delay(t);
            
        if(is_safe(row,col)){
            
            board[row][col]=1;
            //printBoard();
            

            // Recursive for place rest of the queens
            if(solve_aux(col+1))
                return true;

            // if placing the queen doesn't lead to a solution
            board[row][col]=0; //Backtrack
            sol[col]=-1; //Backtrack

            //Shows the movement of the queen
            led_matrix.clearDisplay(0);
            showBoard();
            //printBoard();
            delay(t);

        }
        
        sol[col]=-1; //Backtrack

        //Shows the movement of the queen
        led_matrix.clearDisplay(0);
        showBoard();
        //printBoard();
        delay(t);
    }
    return false;
}

void show_Collition(int row1,int col1,int row2,int col2){
  for(int a = 0; a<5;a++){
    led_matrix.setLed(0,row1,col1,true);
    led_matrix.setLed(0,row2,col2,true);
    delay(60);
    led_matrix.setLed(0,row1,col1,false);
    led_matrix.setLed(0,row2,col2,false);
    delay(60);              
  }
}

// Indicates if a queen on i,j position is in danger or not
bool is_safe(int row, int col){
     
    int i, j; 
  
    /* Check this row on left side */
    for (i = 0; i < col; i++){ 
        if (board[row][i]){ 
          show_Collition(row,col,row,i);
          return false;}} 
        
  
    /* Check upper diagonal on left side */
    for (i=row, j=col; i>=0 && j>=0; i--, j--){ 
        if (board[i][j]){
          show_Collition(row,col,i,j);
          return false;}}
         
    
  
    /* Check lower diagonal on left side */
    for (i=row, j=col; j>=0 && i<N; i++, j--){ 
        if (board[i][j]){
          show_Collition(row,col,i,j);
          return false;}}
         
  
   return true; 
  
}
