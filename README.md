# Reversi-MiniMax-AI

## What is this program about? 

Reversi is a strategy board game played on an 8x8 board. It is played between two players and details on the rules can be found here: https://en.wikipedia.org/wiki/Reversi#Rules

This program provides a platform to play reversi on three difficulties against the MiniMax AI.

MiniMax is a recursive algorithm which simulates every possible move for a defined depth in each game scenario and then returns the move that would result in the minimum possible loss for the computer. The process can be visualized using a tree with each node representing a game scenario.

![image](https://user-images.githubusercontent.com/105610097/181623077-a0ed12f5-2fa7-4170-b787-5c00a35eb14f.png)

## How does the difficulty of the AI work? 

The game tree involves the computer simulating each immediate possible move and then assessing all possible moves from the opponent. More levels of depth allows the computer to make a better informed move, however it comes with the limit of longer processing time due to the recursive algorithm. The AI uses a search algorithm called alpha-beta pruning to decrease processing time by identifying which paths shouldn't be further evaluated.

## What are the difficulties?

Easy - MiniMax program uses a depth of 3 

Medium - MiniMax program uses a depth of 4

Hard - MiniMax program uses a depth of 5 
