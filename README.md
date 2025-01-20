[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# Rock Paper Scissors Tournament

### Group 1

David Chen
       
### Project Description:

The tournament starts off waiting for an even number of players(still determining) and matchmakes those players into a tournament system. There will be one winner decided by rock paper scissors.
  
### Instructions:

How does the user install/compile/run the program?

"make compile" is first ran to create the executable files. Then, "make main" is ran to run the server, which waits for eight players to run "make play" to join the tournament. 
The user can use "make clean" to remove the executables and .o files created by "make compile".


How does the user interact with this program?

Each player will either go first or wait for their opponent to make a choice between 1(rock), 2(paper), or 3(scissors) and enter it into the terminal. Once their opponent has made a choice, they can also enter a choice, which will lead to the result(win, loss, draw) to print on their screen. If they win, they will wait for another winner for another match. If they lose, they're program will quit, and their opponent will wait for another match. If they draw, then they will have to repeat the previous steps mentioned. This will continue until the final winner is chosen, which is when all programs should exit. 
The server will have each action printed onto their terminal, along with a file called "history.txt" that contains all the actions of the players.



Video:
