# Final Project Proposal

## Group Members:

David Chen
       
# Intentions:

A statement of the problem you are solving and/or a high level description of the project.
I plan on creating a 1v1 game played between seperate devices using shared memory and semaphores. The game will be RNG based, where each player has HP and will roll a random weapon to deplete the HP of their opponent. Additionally, I plan on creating a matchmaking system between an even number of players(more than just 2). This will be managed by my main server, which will be forking subservers to manage multiple games at once. The subservers will print out the results of each match, which will eventually update to the winner of the tournament. 
    
# Intended usage:

An even number of players run the game. Once the program is ran, a prompt will ask if the player wants to play. Each player will wait until they find a player to play against. Once one player loses all their HP, the player wins, and either waits for another player in the matchmaking queue(tournament) or is prompted if they want to play again. 
  
# Technical Details:

A description of your technical design. This should include:
   

Each player program will be given a random number(the player program reads a random number, then it will send that number to the server where it gets sorted), where I will be using a sorting algorithm(most likely merge sort) to randomly match players up. I will use a semaphore with a max value of 2(for each subserver), allowing only the programs with the correct random number in the current match to play. 
During the match, each player will read a random number from the dev/random file, where then it will use that random number to read a random item from a file. The item in that file will be formatted in a way such that information of its attack/abilities will be shown. The player program then sends the information to the subserver, which will then do the action described through that information. The server will know when the HP of one player depletes, where it will then continue onto the other games. It will resort as the brackets end.
I will be using the union semun and the struct sembuf for the semaphores, and I will be using all the functions for shared memory, random file, reading files, etc. I will be allocating memory for arrays and basic data structures. 
I will be doing everything for this project.

# Intended pacing:

A timeline with expected completion dates of parts of the project.

1/10: Finish basic forking and servers
1/12: Finish basic game mechanics

