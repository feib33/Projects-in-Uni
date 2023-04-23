#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Mar 23 18:24:14 2018

@author: Huiyuan Meng, Fei Bao
"""
import collections
import fileinput


# initialize the board from input file
def read_input():
    board=[]
    for line in fileinput.input():
        strings = line.strip("\n").split(" ")
        data = [str(n) for n in strings]
        board.append(data)
    return board


# if the 9th line is Moves,count possible moves for every pieces
def count_moves(board):
    board.pop()
    board_size = len(board)
    black_moves = 0
    white_moves = 0
    
    # iterate board, base on the rule to count possible moves of white
    for i in range(board_size):
        for j in range(board_size):
            if board[i][j] == "O":
                # move by one step
                if i - 1 >= 0 and board[i - 1][j] == "-":
                    white_moves += 1
                
                if i + 1 < board_size and board[i + 1][j] == "-":
                    white_moves += 1
                
                if j - 1 >= 0 and board[i][j - 1] == "-":
                    white_moves += 1
                
                if j + 1 < board_size and board[i][j + 1] ==  "-":
                    white_moves += 1
                    
                # move by jumping over a white piece or a black piece   
                if (i - 2 >= 0 and 
                   (board[i - 1][j] == "@" or board[i - 1][j] == "O")
                    and board[i - 2][j] == "-"):
                    white_moves += 1
                
                if(j - 2 >= 0 and 
                   (board[i][j - 1] == "@" or board[i][j - 1] == "O")
                    and board[i][j - 2] == "-"):
                    white_moves += 1
                
                if(i + 2 < board_size and 
                   (board[i + 1][j] == "@" or board[i + 1][j] == "O")
                    and board[i + 2][j] == "-"):
                    white_moves += 1
                
                if(j + 2 < board_size and 
                   (board[i][j+1] == "@" or board[i][j + 1] == "O")
                    and board[i][j + 2] == "-"):
                    white_moves += 1
    # iterate board, base on the rule to count possible moves of black                 
    for i in range(board_size):
        for j in range(board_size):
            if board[i][j] == "@":
            	# move by one step
                if i - 1 >= 0 and board[i - 1][j] == "-":
                    black_moves += 1
                if i + 1 < board_size and board[i + 1][j] == "-":
                    black_moves += 1
                if j - 1 >= 0 and board[i][j - 1] == "-":
                    black_moves += 1
                if j + 1 < board_size and board[i][j + 1] == "-":
                    black_moves += 1
                    
                # move by jumping over a white piece or a black piece    
                if(i - 2 >= 0 and 
                   (board[i - 1][j] == "@" or board[i - 1][j] == "O")
                    and board[i - 2][j] == "-"):
                    black_moves += 1
                if(j - 2 >= 0 and 
                   (board[i][j-1] == "@" or board[i][j - 1] == "O")
                    and board[i][j - 2] == "-"):
                    black_moves += 1
                if(i + 2 < board_size and 
                   (board[i + 1][j] == "@" or board[i + 1][j] == "O")
                    and board[i + 2][j] == "-"):
                    black_moves += 1
                if(j + 2 < board_size and
                   (board[i][j + 1] == "@" or board[i][j + 1] == "O")
                    and board[i][j + 2] == "-"):
                    black_moves += 1
    
    print (white_moves)
    print (black_moves)


# if the 9th line is "Massacre", 
# caculate a sequence of legal moves for white pieces
def massacre(board):    
    board.pop()
    blacks = find_black(board)
    whites = find_white(board)
    for piece in blacks:
        whites = sort_all_whites(whites, piece)
        goal = find_goal(board, piece)
        
        if len(goal) == 1:
            path = bfs(board, whites[1], goal[0])
            update(board, whites[1], goal[0])
            eliminate(board)
            print_path(path)
        else:
            i = 0
            for each_goal in goal: 
                whites = find_white(board)
                whites = sort_all_whites(whites, piece)
                path = bfs(board, whites[i], each_goal)
                update(board, whites[i], each_goal)
                eliminate(board)
                i += 1
                print_path(path)


# return a list of positions of black pieces              
def find_black(grid):
    blacks = []
    board_size = len(grid)
    for i in range(board_size):
        for j in range(board_size):
            if grid[i][j] == "@":
                blacks.append((j,i))
    return blacks


# return a list of positions of white pieces
def find_white(grid):
    all_whites = []
    for row in range(len(grid)):
        for col in range(len(grid[row])):
            if grid[row][col] == "O":
                all_whites.append((col, row))
    return all_whites


#sort  all whites depends on their distance to the black piece
def sort_all_whites(lists, black):
    sorted_list = []
    width = 8
    height = 8
    while lists:
        min_coord = lists[0]
        min_dist = ((width - 1) + (height - 1));
        for (x, y) in lists:
            temp_dist = abs(x - black[0]) + abs(y - black[-1]) 
            if temp_dist <= min_dist:
                min_dist = temp_dist
                min_coord = (x, y)
        lists.remove(min_coord)
        sorted_list.append(min_coord)
    return sorted_list


# find the goal positions that could eliminate black piece
def find_goal(grid, black):
    col = black[0]
    row = black[1]
    goals = []
    # when the black is next to one white or corner
    if row - 1 >= 0 and grid[row - 1][col] in ("O", "X"):
        goals.append((row + 1, col)[::-1])
        return goals
    if row + 1 >= 0 and grid[row + 1][col] in ("O", "X"):
        goals.append((row - 1, col)[::-1])
        return goals
    if col - 1 >= 0 and grid[row][col - 1] in ("O", "X"):
        goals.append((row, col + 1)[::-1])
        return goals
    if col + 1 >= 0 and grid[row][col + 1] in ("O", "X"):
        goals.append((row, col - 1)[::-1])
        return goals
    # when four directions of the black are all unoccupied
    if (row - 1 >= 0 and grid[row - 1][col] == "-"
        and grid[row + 1][col] == "-"):
        goals.append((row - 1, col)[::-1])
        goals.append((row + 1, col)[::-1])
        return goals
    if (col - 1 >= 0 and grid[row][col-1] == "-" 
        and grid[row][col + 1] == "-"):
        goals.append((row, col - 1)[::-1])
        goals.append((row, col + 1)[::-1])
        return goals


# find the path that white piece moves to the destination
def bfs(grid, start,goal):
    queue = collections.deque([[start]])
    # a list that contains positions of white pieces has checked
    checked = set([start])
    width = 8
    height = 8
    while queue:
        path = queue.popleft()
        x, y = path[-1]
        # return path when white piece has arrived at the destination
        if (x,y) == goal:
            return path
        # check four directions that white piece may move
        for x2, y2 in ((x+1, y), (x - 1, y), (x, y + 1), (x, y - 1)):
            if (0 <= x2 < height  and 0 <= y2 < width 
            and (x2, y2) not in checked):
                # white piece would jump over one square
                if ((x2, y2) == (x + 1, y) and grid[y2][x2] in ("@", "O") 
                    and grid[y][x + 2] == "-"):
                    queue.append(path + [((x + 2), y)])
                    checked.add(((x + 2), y))
                elif ((x2, y2) == (x - 1, y) and grid[y2][x2] in ("@", "O") 
                    and grid[y][x - 2] == "-"):
                    queue.append(path + [((x - 2), y)])
                    checked.add(((x - 2), y))
                elif ((x2, y2) == (x, y+1) and grid[y2][x2] in ("@", "O") 
                    and grid[y + 2][x] == "-"):
                    queue.append(path + [(x, y + 2)])
                    checked.add((x, y + 2))
                elif ((x2, y2) == (x, y - 1) and grid[y2][x2] in ("@", "O") 
                    and grid[y - 2][x] == "-"):
                    queue.append(path + [(x, y - 2)])
                    checked.add((x, y - 2))
                # white piece would move one step
                elif grid[y2][x2] == "-":
                    queue.append(path + [(x2, y2)])
                    checked.add((x2, y2))


# get rid of black piece if they are surrounded
def eliminate(grid):
    board_size=len(grid)
    for i in range(board_size):
        for j in range(board_size):
            if grid[i][j] == "@":
                if (i + 1 < board_size and i - 1 >= 0
                    and (grid[i + 1][j] == "O" or grid[i + 1][j] == "X")
                    and (grid[i - 1][j] == "O" or grid[i - 1][j] == "X")):
                    grid[i][j] = "-";
                elif (j + 1 < board_size and j - 1 >= 0
                      and (grid[i][j - 1] == "O" or grid[i][j + 1] == "X")
                      and (grid[i][j - 1] == "O" or grid[i][j + 1] == "X")):
                    grid[i][j] = "-";


# update the board when moves are made
def update(grid,start_pos,dest_pos):
    row1 = start_pos[1]
    col1 = start_pos[0]
    row2 = dest_pos[1]
    col2 = dest_pos[0]
    piece = grid[row1][col1]
    grid[row1][col1] = grid[row2][col2]
    grid[row2][col2] = piece


# print path that all white pieces moved
def print_path(path_list):
    for i in range(len(path_list) - 1):
        print(path_list[i], "->", path_list[i + 1])


# import board
board = read_input()
if board[-1][-1] == "Moves":
    count_moves(board)
elif board[-1][-1] == "Massacre":
    massacre(board)
          