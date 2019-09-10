//
//  
//  Chessy
//
//  Created by Leonardo Bohac on 15/07/19.
//  Copyright © 2019 Leonardo Bohac. All rights reserved.
//

// This is a purely C Chess Engine, made for simulating chess games.
// The code is written almost as a pseudocode, using only the basic
// programming operations (ifses, elses and fors).

// Most of the focus is directed toward providing a very fast and friendly
// chess games simulator.

// At this stage of development, the code is able to play a random game, and return
// a winning team. It’d be the backbone of any chess engine.

// The main and most important function is the one name ‘Check’, which tells if a given king
// is currently under check. It has to be called each time a new move is being evaluated as
// valid or not. The verification starts at the king’s position, and looks for the squares it
// could have been being checked by some enemy team (looks for knights on knight squares around
// the king, looks for pawns, bishops or queens on diagonal direction squares, etc)

#include <stdio.h>
#include <stdlib.h>

void *memcpy(void *str1, const void *str2, size_t n);

void Write_Board(void);
void Write_Score(double s);
void Write_Train(int depth, int reps);

int min(int x, int y)
{
    if(x <= y){return x;}
    return y;
}

void Show(int b[8][8])
{
    for (int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < 8; j++)
        {
            int piece = b[i][j];
            
            if(piece == 0)
            {
                printf("   0   ");
            }
            if(piece > 0)
            {
                if(piece < 9)
                {
                    printf(" Pa(W) ");
                }
                if(piece == 9 || piece == 16)
                {
                    printf(" Ro(W) ");
                }
                if(piece == 10 || piece == 15)
                {
                    printf(" Kn(W) ");
                }
                if(piece == 11 || piece == 14)
                {
                    printf(" Bi(W) ");
                }
                if(piece == 12 || piece > 16)
                {
                    printf(" Qu(W) ");
                }
                if(piece == 13)
                {
                    printf(" Ki(W) ");
                }
            }
            if(piece < 0)
            {
                if(piece > -9)
                {
                    printf(" Pa(B) ");
                }
                if(piece == -9 || piece == -16)
                {
                    printf(" Ro(B) ");
                }
                if(piece == -10 || piece == -15)
                {
                    printf(" Kn(B) ");
                }
                if(piece == -11 || piece == -14)
                {
                    printf(" Bi(B) ");
                }
                if(piece == -12 || piece < -16)
                {
                    printf(" Qu(B) ");
                }
                if(piece == -13)
                {
                    printf(" Ki(B) ");
                }
            }
        }
        printf("\n\n\n");
    }
}

int Board[8][8] = {{ 9,  10,  11,  12,  13,  14,  15,  16},
                   { 1,   2,   3,   4,   5,   6,   7,   8},
                   { 0,   0,   0,   0,   0,   0,   0,   0},
                   { 0,   0,   0,   0,   0,   0,   0,   0},
                   { 0,   0,   0,   0,   0,   0,   0,   0},
                   { 0,   0,   0,   0,   0,   0,   0,   0},
                   {-1,  -2,  -3,  -4,  -5,  -6,  -7,  -8},
                   {-9, -10, -11, -12, -13, -14, -15, -16}};


int Pieces[2][25][2] = {{{8,8},
                         {1,0},{1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},
                         {8,8},{8,8},{8,8},{8,8},{8,8},{8,8},{8,8},{8,8}},
                        {{8,8},
                         {6,0},{6,1},{6,2},{6,3},{6,4},{6,5},{6,6},{6,7},{7,0},{7,1},{7,2},{7,3},{7,4},{7,5},{7,6},{7,7},
                         {8,8},{8,8},{8,8},{8,8},{8,8},{8,8},{8,8},{8,8}}};

int MoveStack[500][6] = {0};

int QCastle_W = 1;   // Still can Queen Castle
int QCastle_B = 1;

int KCastle_W = 1;   // Still can King Castle
int KCastle_B = 1;

int PQueens_W = 0;   // Promoted Queens
int PQueens_B = 0;

void Start(void)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Board[i][j] = 0;
        }
    }
    
    Board[0][0] = 9;
    Board[0][1] = 10;
    Board[0][2] = 11;
    Board[0][3] = 12;
    Board[0][4] = 13;
    Board[0][5] = 14;
    Board[0][6] = 15;
    Board[0][7] = 16;
    Board[1][0] = 1;
    Board[1][1] = 2;
    Board[1][2] = 3;
    Board[1][3] = 4;
    Board[1][4] = 5;
    Board[1][5] = 6;
    Board[1][6] = 7;
    Board[1][7] = 8;
    
    Board[7][0] = -9;
    Board[7][1] = -10;
    Board[7][2] = -11;
    Board[7][3] = -12;
    Board[7][4] = -13;
    Board[7][5] = -14;
    Board[7][6] = -15;
    Board[7][7] = -16;
    Board[6][0] = -1;
    Board[6][1] = -2;
    Board[6][2] = -3;
    Board[6][3] = -4;
    Board[6][4] = -5;
    Board[6][5] = -6;
    Board[6][6] = -7;
    Board[6][7] = -8;
    
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 25; j++)
        {
            Pieces[i][j][0] = 8;
            Pieces[i][j][1] = 8;
        }
    }
    
    Pieces[0][1][0] = 1;
    Pieces[0][1][1] = 0;
    
    Pieces[0][2][0] = 1;
    Pieces[0][2][1] = 1;
    
    Pieces[0][3][0] = 1;
    Pieces[0][3][1] = 2;
    
    Pieces[0][4][0] = 1;
    Pieces[0][4][1] = 3;
    
    Pieces[0][5][0] = 1;
    Pieces[0][5][1] = 4;
    
    Pieces[0][6][0] = 1;
    Pieces[0][6][1] = 5;
    
    Pieces[0][7][0] = 1;
    Pieces[0][7][1] = 6;
    
    Pieces[0][8][0] = 1;
    Pieces[0][8][1] = 7;
    
    Pieces[0][9][0] = 0;
    Pieces[0][9][1] = 0;
    
    Pieces[0][10][0] = 0;
    Pieces[0][10][1] = 1;
    
    Pieces[0][11][0] = 0;
    Pieces[0][11][1] = 2;
    
    Pieces[0][12][0] = 0;
    Pieces[0][12][1] = 3;
    
    Pieces[0][13][0] = 0;
    Pieces[0][13][1] = 4;
    
    Pieces[0][14][0] = 0;
    Pieces[0][14][1] = 5;
    
    Pieces[0][15][0] = 0;
    Pieces[0][15][1] = 6;
    
    Pieces[0][16][0] = 0;
    Pieces[0][16][1] = 7;
    
    
    Pieces[1][1][0] = 6;
    Pieces[1][1][1] = 0;
    
    Pieces[1][2][0] = 6;
    Pieces[1][2][1] = 1;
    
    Pieces[1][3][0] = 6;
    Pieces[1][3][1] = 2;
    
    Pieces[1][4][0] = 6;
    Pieces[1][4][1] = 3;
    
    Pieces[1][5][0] = 6;
    Pieces[1][5][1] = 4;
    
    Pieces[1][6][0] = 6;
    Pieces[1][6][1] = 5;
    
    Pieces[1][7][0] = 6;
    Pieces[1][7][1] = 6;
    
    Pieces[1][8][0] = 6;
    Pieces[1][8][1] = 7;
    
    Pieces[1][9][0] = 7;
    Pieces[1][9][1] = 0;
    
    Pieces[1][10][0] = 7;
    Pieces[1][10][1] = 1;
    
    Pieces[1][11][0] = 7;
    Pieces[1][11][1] = 2;
    
    Pieces[1][12][0] = 7;
    Pieces[1][12][1] = 3;
    
    Pieces[1][13][0] = 7;
    Pieces[1][13][1] = 4;
    
    Pieces[1][14][0] = 7;
    Pieces[1][14][1] = 5;
    
    Pieces[1][15][0] = 7;
    Pieces[1][15][1] = 6;
    
    Pieces[1][16][0] = 7;
    Pieces[1][16][1] = 7;
    
    QCastle_W = 1;
    QCastle_B = 1;
    
    KCastle_W = 1;
    KCastle_B = 1;
    
    PQueens_W = 0;
    PQueens_B = 0;
}

int Check(int team)
{
    int R,F; // King's rank and file
    
    if(team == 0)
    {
        R = Pieces[0][13][0];
        F = Pieces[0][13][1];
        int p;   // Piece at board
        
        for(int i = 1; i < 8-R; i++)  // N
        {
            p = Board[R+i][F];
            
            if(p == 0){continue;}
            else if(p > 0){break;}
            else if(i == 1 && p == -13){return 1;}
            else if(p == -9 || p == -12 || p <= -16){return 1;}  // <= -16 : rook or (promoted) queen
            else{break;}
        }
        for(int i = 1; i < R+1; i++)  // S
        {
            p = Board[R-i][F];
            
            if(p == 0){continue;}
            else if(p > 0){break;}
            else if(i == 1 && p == -13){return 1;}
            else if(p == -9 || p == -12 || p <= -16){return 1;}  // <= -16 : rook or (promoted) queen
            else{break;}
        }
        for(int i = 1; i < 8-F; i++)  // E
        {
            p = Board[R][F+i];
            
            if(p == 0){continue;}
            else if(p > 0){break;}
            else if(i == 1 && p == -13){return 1;}
            else if(p == -9 || p == -12 || p <= -16){return 1;}  // <= -16 : rook or (promoted) queen
            else{break;}
        }
        for(int i = 1; i < F+1; i++)  // W
        {
            p = Board[R][F-i];
            
            if(p == 0){continue;}
            else if(p > 0){break;}
            else if(i == 1 && p == -13){return 1;}
            else if(p == -9 || p == -12 || p <= -16){return 1;}  // <= -16 : rook or (promoted) queen
            else{break;}
        }
        
        for(int i = 1; i < min(8-R,8-F); i++)  // NE
        {
            p = Board[R+i][F+i];
            
            if(p == 0){continue;}
            else if(p > 0){break;}
            else if(i == 1 && (p > -9 || p == -13)){return 1;}
            else if(p == -11 || p == -12 || p == -14 || p < -16){return 1;}
            else{break;}
        }
        for(int i = 1; i < min(8-R,F+1); i++)  // NW
        {
            p = Board[R+i][F-i];
            
            if(p == 0){continue;}
            else if(p > 0){break;}
            else if(i == 1 && (p > -9 || p == -13)){return 1;}
            else if(p == -11 || p == -12 || p == -14 || p < -16){return 1;}
            else{break;}
        }
        for(int i = 1; i < min(R+1,F+1); i++)  // SW
        {
            p = Board[R-i][F-i];
            
            if(p == 0){continue;}
            else if(p > 0){break;}
            else if(i == 1 && p == -13){return 1;}
            else if(p == -11 || p == -12 || p == -14 || p < -16){return 1;}
            else{break;}
        }
        for(int i = 1; i < min(R+1,8-F); i++)  // SE
        {
            p = Board[R-i][F+i];
            
            if(p == 0){continue;}
            else if(p > 0){break;}
            else if(i == 1 && p == -13){return 1;}
            else if(p == -11 || p == -12 || p == -14 || p < -16){return 1;}
            else{break;}
        }
        
        if(R < 7 && F < 6)  // Kn_1
        {
            p = Board[R+1][F+2];
            
            if(p == -10 || p == -15){return 1;}
        }
        if(R < 6 && F < 7)  // Kn_2
        {
            p = Board[R+2][F+1];
            
            if(p == -10 || p == -15){return 1;}
        }
        if(R < 6 && F > 0)  // Kn_3
        {
            p = Board[R+2][F-1];
            
            if(p == -10 || p == -15){return 1;}
        }
        if(R < 7 && F > 1)  // Kn_4
        {
            p = Board[R+1][F-2];
            
            if(p == -10 || p == -15){return 1;}
        }
        if(R > 0 && F > 1)  // Kn_5
        {
            p = Board[R-1][F-2];
            
            if(p == -10 || p == -15){return 1;}
        }
        if(R > 1 && F > 0)  // Kn_6
        {
            p = Board[R-2][F-1];
            
            if(p == -10 || p == -15){return 1;}
        }
        if(R > 1 && F < 7)  // Kn_7
        {
            p = Board[R-2][F+1];
            
            if(p == -10 || p == -15){return 1;}
        }
        if(R > 0 && F < 6)  // Kn_8
        {
            p = Board[R-1][F+2];
            
            if(p == -10 || p == -15){return 1;}
        }
        
        return 0;
    }
    
    else
    {
        R = Pieces[1][13][0];
        F = Pieces[1][13][1];
        int p;   // Piece at board
        
        for(int i = 1; i < 8-R; i++)  // N
        {
            p = Board[R+i][F];
            
            if(p == 0){continue;}
            else if(p < 0){break;}
            else if(i == 1 && p == 13){return 1;}
            else if(p == 9 || p == 12 || p >= 16){return 1;}  // >= 16 : rook or (promoted) queen
            else{break;}
        }
        for(int i = 1; i < R+1; i++)  // S
        {
            p = Board[R-i][F];
            
            if(p == 0){continue;}
            else if(p < 0){break;}
            else if(i == 1 && p == 13){return 1;}
            else if(p == 9 || p == 12 || p >= 16){return 1;}
            else{break;}
        }
        for(int i = 1; i < 8-F; i++)  // E
        {
            p = Board[R][F+i];
            
            if(p == 0){continue;}
            else if(p < 0){break;}
            else if(i == 1 && p == 13){return 1;}
            else if(p == 9 || p == 12 || p >= 16){return 1;}
            else{break;}
        }
        for(int i = 1; i < F+1; i++)  // W
        {
            p = Board[R][F-i];
            
            if(p == 0){continue;}
            else if(p < 0){break;}
            else if(i == 1 && p == 13){return 1;}
            else if(p == 9 || p == 12 || p >= 16){return 1;}
            else{break;}
        }
        
        for(int i = 1; i < min(8-R,8-F); i++)  // NE
        {
            p = Board[R+i][F+i];
            
            if(p == 0){continue;}
            else if(p < 0){break;}
            else if(i == 1 && p == 13){return 1;}
            else if(p == 11 || p == 12 || p == 14 || p > 16){return 1;}
            else{break;}
        }
        for(int i = 1; i < min(8-R,F+1); i++)  // NW
        {
            p = Board[R+i][F-i];
            
            if(p == 0){continue;}
            else if(p < 0){break;}
            else if(i == 1 && p == 13){return 1;}
            else if(p == 11 || p == 12 || p == 14 || p > 16){return 1;}
            else{break;}
        }
        for(int i = 1; i < min(R+1,F+1); i++)  // SW
        {
            p = Board[R-i][F-i];
            
            if(p == 0){continue;}
            else if(p < 0){break;}
            else if(i == 1 && (p < 9 || p == 13)){return 1;}
            else if(p == 11 || p == 12 || p == 14 || p > 16){return 1;}
            else{break;}
        }
        for(int i = 1; i < min(R+1,8-F); i++)  // SE
        {
            p = Board[R-i][F+i];
            
            if(p == 0){continue;}
            else if(p < 0){break;}
            else if(i == 1 && (p < 9 || p == 13)){return 1;}
            else if(p == 11 || p == 12 || p == 14 || p > 16){return 1;}
            else{break;}
        }
        
        if(R < 7 && F < 6)  // Kn_1
        {
            p = Board[R+1][F+2];
            
            if(p == 10 || p == 15){return 1;}
        }
        if(R < 6 && F < 7)  // Kn_2
        {
            p = Board[R+2][F+1];
            
            if(p == 10 || p == 15){return 1;}
        }
        if(R < 6 && F > 0)  // Kn_3
        {
            p = Board[R+2][F-1];
            
            if(p == 10 || p == 15){return 1;}
        }
        if(R < 7 && F > 1)  // Kn_4
        {
            p = Board[R+1][F-2];
            
            if(p == 10 || p == 15){return 1;}
        }
        if(R > 0 && F > 1)  // Kn_5
        {
            p = Board[R-1][F-2];
            
            if(p == 10 || p == 15){return 1;}
        }
        if(R > 1 && F > 0)  // Kn_6
        {
            p = Board[R-2][F-1];
            
            if(p == 10 || p == 15){return 1;}
        }
        if(R > 1 && F < 7)  // Kn_7
        {
            p = Board[R-2][F+1];
            
            if(p == 10 || p == 15){return 1;}
        }
        if(R > 0 && F < 6)  // Kn_8
        {
            p = Board[R-1][F+2];
            
            if(p == 10 || p == 15){return 1;}
        }
        
        return 0;
    }
}

void Move(int type, int arg0, int arg1, int arg2, int arg3, int arg4)
{
    if(type == 0)  // Normal move
    {
        int mp = arg0;  // Moving piece
        
        int o_r = arg1;  // Original rank
        int o_f = arg2;  // Original file
        int d_r = arg3;  // Destiny rank
        int d_f = arg4;  // Destiny file
        
        int bp = Board[d_r][d_f];  // Board piece
        
        if(mp > 0)
        {
            Board[o_r][o_f] = 0;
            Board[d_r][d_f] = mp;
            
            Pieces[0][mp][0] = d_r;
            Pieces[0][mp][1] = d_f;
            
            if(bp != 0)
            {
                Pieces[1][-bp][0] = 8;  // 8 = out of board
                Pieces[1][-bp][1] = 8;
            }
        }
        else
        {
            Board[o_r][o_f] = 0;
            Board[d_r][d_f] = mp;
            
            Pieces[1][-mp][0] = d_r;
            Pieces[1][-mp][1] = d_f;
            
            if(bp != 0)
            {
                Pieces[0][bp][0] = 8;
                Pieces[0][bp][1] = 8;
            }
        }
    }
    if(type == 1)  // Queen's Castle
    {
        int team = arg0;
        
        if(team == 0)
        {
            Board[0][0] = 0;
            Board[0][1] = 0;
            Board[0][2] = 13;
            Board[0][3] = 9;
            Board[0][4] = 0;
            
            Pieces[0][9][0] = 0;
            Pieces[0][9][1] = 3;
            
            Pieces[0][13][0] = 0;
            Pieces[0][13][1] = 2;
        }
        else
        {
            Board[7][0] = 0;
            Board[7][1] = 0;
            Board[7][2] = -13;
            Board[7][3] = -9;
            Board[7][4] = 0;
            
            Pieces[1][9][0] = 7;
            Pieces[1][9][1] = 3;
            
            Pieces[1][13][0] = 7;
            Pieces[1][13][1] = 2;
        }
    }
    if(type == 2)  // King's Castle
    {
        int team = arg0;
        
        if(team == 0)
        {
            Board[0][4] = 0;
            Board[0][5] = 16;
            Board[0][6] = 13;
            Board[0][7] = 0;
            
            Pieces[0][13][0] = 0;
            Pieces[0][13][1] = 6;
            
            Pieces[0][16][0] = 0;
            Pieces[0][16][1] = 5;
        }
        else
        {
            Board[7][4] = 0;
            Board[7][5] = -16;
            Board[7][6] = -13;
            Board[7][7] = 0;
            
            Pieces[1][13][0] = 7;
            Pieces[1][13][1] = 6;
            
            Pieces[1][16][0] = 7;
            Pieces[1][16][1] = 5;
        }
    }
    if(type == 3)  // En Passant
    {
        int mp = arg0;  // Moving pawn
        
        int o_r = arg1;
        int o_f = arg2;
        int d_r = arg3;
        int d_f = arg4;
        
        if(mp > 0)
        {
            int fp = Board[d_r-1][d_f];  // Falling pawn
            
            Board[o_r][o_f] = 0;
            Board[d_r][d_f] = mp;
            Board[d_r-1][d_f] = 0;
            
            Pieces[0][mp][0] = d_r;
            Pieces[0][mp][1] = d_f;
            
            Pieces[1][-fp][0] = 8;
            Pieces[1][-fp][1] = 8;
        }
        else
        {
            int fp = Board[d_r+1][d_f];
            
            Board[o_r][o_f] = 0;
            Board[d_r][d_f] = mp;
            Board[d_r+1][d_f] = 0;
            
            Pieces[1][-mp][0] = d_r;
            Pieces[1][-mp][1] = d_f;
            
            Pieces[0][fp][0] = 8;
            Pieces[0][fp][1] = 8;
        }
    }
}

int Legal(int type, int arg0, int arg1, int arg2, int arg3, int arg4)
{
    int L;
    
    if(type == 0)
    {
        int mp = arg0;
        
        int o_r = arg1;
        int o_f = arg2;
        int d_r = arg3;
        int d_f = arg4;
        
        int bp = Board[d_r][d_f];
        
        if(mp > 0)
        {
            if(mp == 13)
            {
                Pieces[0][13][0] = d_r;
                Pieces[0][13][1] = d_f;
            }
            
            Board[o_r][o_f] = 0;
            Board[d_r][d_f] = mp;
            
            L = !Check(0);
            
            Board[o_r][o_f] = mp;
            Board[d_r][d_f] = bp;
            
            if(mp == 13)
            {
                Pieces[0][13][0] = o_r;
                Pieces[0][13][1] = o_f;
            }
            return L;
        }
        else
        {
            if(mp == -13)
            {
                Pieces[1][13][0] = d_r;
                Pieces[1][13][1] = d_f;
            }
            
            Board[o_r][o_f] = 0;
            Board[d_r][d_f] = mp;
            
            L = !Check(1);
            
            Board[o_r][o_f] = mp;
            Board[d_r][d_f] = bp;
            
            if(mp == -13)
            {
                Pieces[1][13][0] = o_r;
                Pieces[1][13][1] = o_f;
            }
            
            return L;
        }
    }
    if(type == 1)
    {
        int team = arg0;
        
        if(team == 0)
        {
            if(Board[0][0] == 9 && Board[0][1] == 0 && Board[0][2] == 0 && Board[0][3] == 0 && Board[0][4] == 13)
            {
                if(!Check(0))
                {
                    Board[0][3] = 13;
                    Board[0][4] = 0;
                    
                    Pieces[0][13][0] = 0;
                    Pieces[0][13][1] = 3;
                    
                    if(!Check(0))
                    {
                        Board[0][0] = 0;
                        Board[0][2] = 13;
                        Board[0][3] = 9;
                        
                        Pieces[0][13][0] = 0;
                        Pieces[0][13][1] = 2;
                        
                        L = !Check(0);
                        
                        Board[0][0] = 9;
                        Board[0][2] = 0;
                        Board[0][3] = 0;
                        Board[0][4] = 13;
                        
                        Pieces[0][13][0] = 0;
                        Pieces[0][13][1] = 4;
                        
                        return L;
                    }
                    Board[0][3] = 0;
                    Board[0][4] = 13;
                    
                    Pieces[0][13][0] = 0;
                    Pieces[0][13][1] = 4;
                    return 0;
                }
                return 0;
            }
            return 0;
        }
        else
        {
            if(Board[7][0] == -9 && Board[7][1] == 0 && Board[7][2] == 0 && Board[7][3] == 0 && Board[7][4] == -13)
            {
                if(!Check(1))
                {
                    Board[7][3] = -13;
                    Board[7][4] = 0;
                    
                    Pieces[1][13][0] = 7;
                    Pieces[1][13][1] = 3;
                    
                    if(!Check(1))
                    {
                        Board[7][0] = 0;
                        Board[7][2] = -13;
                        Board[7][3] = -9;
                        
                        Pieces[1][13][0] = 7;
                        Pieces[1][13][1] = 2;
                        
                        L = !Check(1);
                        
                        Board[7][0] = -9;
                        Board[7][2] = 0;
                        Board[7][3] = 0;
                        Board[7][4] = -13;
                        
                        Pieces[1][13][0] = 7;
                        Pieces[1][13][1] = 4;
                        
                        return L;
                    }
                    Board[7][3] = 0;
                    Board[7][4] = -13;
                    
                    Pieces[1][13][0] = 7;
                    Pieces[1][13][1] = 4;
                    return 0;
                }
                return 0;
            }
            return 0;
        }
    }
    if(type == 2)
    {
        int team = arg0;
        
        if(team == 0)
        {
            if(Board[0][4] == 13 && Board[0][5] == 0 && Board[0][6] == 0 && Board[0][7] == 16)
            {
                if(!Check(0))
                {
                    Board[0][4] = 0;
                    Board[0][5] = 13;
                    
                    Pieces[0][13][0] = 0;
                    Pieces[0][13][1] = 5;
                    
                    if(!Check(0))
                    {
                        Board[0][5] = 16;
                        Board[0][6] = 13;
                        Board[0][7] = 0;
                        
                        Pieces[0][13][0] = 0;
                        Pieces[0][13][1] = 6;
                        
                        L = !Check(0);
                        
                        Board[0][4] = 13;
                        Board[0][5] = 0;
                        Board[0][6] = 0;
                        Board[0][7] = 16;
                        
                        Pieces[0][13][0] = 0;
                        Pieces[0][13][1] = 4;
                        
                        return L;
                    }
                    Board[0][4] = 13;
                    Board[0][5] = 0;
                    
                    Pieces[0][13][0] = 0;
                    Pieces[0][13][1] = 4;
                    return 0;
                }
                return 0;
            }
            return 0;
        }
        else
        {
            if(Board[7][4] == -13 && Board[7][5] == 0 && Board[7][6] == 0 && Board[0][7] == -16)
            {
                if(!Check(1))
                {
                    Board[7][4] = 0;
                    Board[7][5] = -13;
                    
                    Pieces[1][13][0] = 7;
                    Pieces[1][13][1] = 5;
                    
                    if(!Check(1))
                    {
                        Board[7][5] = -16;
                        Board[7][6] = -13;
                        Board[7][7] = 0;
                        
                        Pieces[1][13][0] = 7;
                        Pieces[1][13][1] = 6;
                        
                        L = !Check(1);
                        
                        Board[7][4] = -13;
                        Board[7][5] = 0;
                        Board[7][6] = 0;
                        Board[7][7] = -16;
                        
                        Pieces[1][13][0] = 7;
                        Pieces[1][13][1] = 4;
                        
                        return L;
                    }
                    Board[7][4] = -13;
                    Board[7][5] = 0;
                    
                    Pieces[1][13][0] = 7;
                    Pieces[1][13][1] = 4;
                    return 0;
                }
                return 0;
            }
            return 0;
        }
    }
    if(type == 3)
    {
        int mp = arg0;
        
        int o_r = arg1;
        int o_f = arg2;
        int d_r = arg3;
        int d_f = arg4;
        
        int fp;
        
        if(mp > 0)
        {
            fp = Board[d_r-1][d_f];
            
            Board[o_r][o_f] = 0;
            Board[d_r][d_f] = mp;
            Board[d_r-1][d_f] = 0;
            
            L = !Check(0);
            
            Board[o_r][o_f] = mp;
            Board[d_r][d_f] = 0;
            Board[d_r-1][d_f] = fp;
            
            return L;
        }
        else
        {
            fp = Board[d_r+1][d_f];
            
            Board[o_r][o_f] = 0;
            Board[d_r][d_f] = mp;
            Board[d_r+1][d_f] = 0;
            
            L = !Check(1);
            
            Board[o_r][o_f] = mp;
            Board[d_r][d_f] = 0;
            Board[d_r+1][d_f] = fp;
            
            return L;
        }
    }
    return 0;
}

int LegalMoves(int p, int p_r, int p_f, int LM)
{
    if(p > 0)
    {
        int bp;  // Board piece
        
        if(p < 9)
        {
            if(p_r == 1)
            {
                if(Board[p_r+1][p_f] == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                    }
                    if(Board[p_r+2][p_f] == 0)
                    {
                        if(Legal(0, p, p_r, p_f, p_r+2, p_f))
                        {
                            MoveStack[LM][0] = 0;
                            MoveStack[LM][1] = p;
                            MoveStack[LM][2] = p_r;
                            MoveStack[LM][3] = p_f;
                            MoveStack[LM][4] = p_r+2;
                            MoveStack[LM][5] = p_f;
                            
                            LM += 1;
                        }
                    }
                }
            }
            if(p_f == 0)
            {
                if(Board[p_r+1][1] < 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, 1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = 1;
                        
                        LM += 1;
                    }
                }
            }
            else if(p_f == 7)
            {
                if(Board[p_r+1][6] < 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, 6))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = 6;
                        
                        LM += 1;
                    }
                }
            }
            else
            {
                if(Board[p_r+1][p_f+1] < 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
                if(Board[p_r+1][p_f-1] < 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
        }
        if(p == 9 || p == 16)
        {
            for(int i = 1; i < 8-p_r; i++)  // N
            {
                bp = Board[p_r+i][p_f];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < p_r+1; i++)  // S
            {
                bp = Board[p_r-i][p_f];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < 8-p_f; i++)  // E
            {
                bp = Board[p_r][p_f+i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < p_f+1; i++)  // W
            {
                bp = Board[p_r][p_f-i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
        }
        if(p == 10 || p == 15)
        {
            if(p_r < 7 && p_f < 6)  // Kn_1
            {
                bp = Board[p_r+1][p_f+2];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f+2))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f+2;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r < 6 && p_f < 7)  // Kn_2
            {
                bp = Board[p_r+2][p_f+1];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+2, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+2;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r < 6 && p_f > 0)  // Kn_3
            {
                bp = Board[p_r+2][p_f-1];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+2, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+2;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r < 7 && p_f > 1)  // Kn_4
            {
                bp = Board[p_r+1][p_f-2];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f-2))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f-2;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 0 && p_f > 1)  // Kn_5
            {
                bp = Board[p_r-1][p_f-2];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f-2))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f-2;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 1 && p_f > 0)  // Kn_6
            {
                bp = Board[p_r-2][p_f-1];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-2, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-2;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 1 && p_f < 7)  // Kn_7
            {
                bp = Board[p_r-2][p_f+1];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-2, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-2;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 0 && p_f < 6)  // Kn_8
            {
                bp = Board[p_r-1][p_f+2];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f+2))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f+2;
                        
                        LM += 1;
                    }
                }
            }
        }
        if(p == 11 || p == 14)
        {
            for(int i = 1; i < min(8-p_r, 8-p_f); i++)  // NE
            {
                bp = Board[p_r+i][p_f+i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(8-p_r, p_f+1); i++)  // NW
            {
                bp = Board[p_r+i][p_f-i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(p_r+1, p_f+1); i++)  // SW
            {
                bp = Board[p_r-i][p_f-i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(p_r+1, 8-p_f); i++)  // SE
            {
                bp = Board[p_r-i][p_f+i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
        }
        if(p == 12 || p > 16)
        {
            for(int i = 1; i < 8-p_r; i++)  // N
            {
                bp = Board[p_r+i][p_f];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < p_r+1; i++)  // S
            {
                bp = Board[p_r-i][p_f];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < 8-p_f; i++)  // E
            {
                bp = Board[p_r][p_f+i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < p_f+1; i++)  // W
            {
                bp = Board[p_r][p_f-i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(8-p_r, 8-p_f); i++)  // NE
            {
                bp = Board[p_r+i][p_f+i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(8-p_r, p_f+1); i++)  // NW
            {
                bp = Board[p_r+i][p_f-i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(p_r+1, p_f+1); i++)  // SW
            {
                bp = Board[p_r-i][p_f-i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(p_r+1, 8-p_f); i++)  // SE
            {
                bp = Board[p_r-i][p_f+i];
                
                if(bp > 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
        }
        if(p == 13)
        {
            if(p_r < 7)
            {
                bp = Board[p_r+1][p_f];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 0)
            {
                bp = Board[p_r-1][p_f];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                    }
                }
            }
            if(p_f < 7)
            {
                bp = Board[p_r][p_f+1];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_f > 0)
            {
                bp = Board[p_r][p_f-1];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r < 7 && p_f < 7)
            {
                bp = Board[p_r+1][p_f+1];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r < 7 && p_f > 0)
            {
                bp = Board[p_r+1][p_f-1];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 0 && p_f > 0)
            {
                bp = Board[p_r-1][p_f-1];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 0 && p_f < 7)
            {
                bp = Board[p_r-1][p_f+1];
                
                if(bp <= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
            }
        }
    }
    else
    {
        int bp;
        
        if(p > -9)
        {
            if(p_r == 6)
            {
                if(Board[p_r-1][p_f] == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                    }
                    if(Board[p_r-2][p_f] == 0)
                    {
                        if(Legal(0, p, p_r, p_f, p_r-2, p_f))
                        {
                            MoveStack[LM][0] = 0;
                            MoveStack[LM][1] = p;
                            MoveStack[LM][2] = p_r;
                            MoveStack[LM][3] = p_f;
                            MoveStack[LM][4] = p_r-2;
                            MoveStack[LM][5] = p_f;
                            
                            LM += 1;
                        }
                    }
                }
            }
            if(p_f == 0)
            {
                if(Board[p_r-1][1] > 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, 1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = 1;
                        
                        LM += 1;
                    }
                }
            }
            else if(p_f == 7)
            {
                if(Board[p_r-1][6] > 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, 6))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = 6;
                        
                        LM += 1;
                    }
                }
            }
            else
            {
                if(Board[p_r-1][p_f+1] > 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
                if(Board[p_r-1][p_f-1] > 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
        }
        if(p == -9 || p == -16)
        {
            for(int i = 1; i < 8-p_r; i++)  // N
            {
                bp = Board[p_r+i][p_f];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < p_r+1; i++)  // S
            {
                bp = Board[p_r-i][p_f];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < 8-p_f; i++)  // E
            {
                bp = Board[p_r][p_f+i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < p_f+1; i++)  // W
            {
                bp = Board[p_r][p_f-i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
        }
        if(p == -10 || p == -15)
        {
            if(p_r < 7 && p_f < 6)  // Kn_1
            {
                bp = Board[p_r+1][p_f+2];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f+2))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f+2;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r < 6 && p_f < 7)  // Kn_2
            {
                bp = Board[p_r+2][p_f+1];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+2, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+2;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r < 6 && p_f > 0)  // Kn_3
            {
                bp = Board[p_r+2][p_f-1];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+2, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+2;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r < 7 && p_f > 1)  // Kn_4
            {
                bp = Board[p_r+1][p_f-2];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f-2))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f-2;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 0 && p_f > 1)  // Kn_5
            {
                bp = Board[p_r-1][p_f-2];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f-2))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f-2;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 1 && p_f > 0)  // Kn_6
            {
                bp = Board[p_r-2][p_f-1];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-2, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-2;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 1 && p_f < 7)  // Kn_7
            {
                bp = Board[p_r-2][p_f+1];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-2, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-2;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 0 && p_f < 6)  // Kn_8
            {
                bp = Board[p_r-1][p_f+2];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f+2))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f+2;
                        
                        LM += 1;
                    }
                }
            }
        }
        if(p == -11 || p == -14)
        {
            for(int i = 1; i < min(8-p_r, 8-p_f); i++)  // NE
            {
                bp = Board[p_r+i][p_f+i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(8-p_r, p_f+1); i++)  // NW
            {
                bp = Board[p_r+i][p_f-i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(p_r+1, p_f+1); i++)  // SW
            {
                bp = Board[p_r-i][p_f-i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(p_r+1, 8-p_f); i++)  // SE
            {
                bp = Board[p_r-i][p_f+i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
        }
        if(p == -12 || p < -16)
        {
            for(int i = 1; i < 8-p_r; i++)  // N
            {
                bp = Board[p_r+i][p_f];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < p_r+1; i++)  // S
            {
                bp = Board[p_r-i][p_f];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < 8-p_f; i++)  // E
            {
                bp = Board[p_r][p_f+i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < p_f+1; i++)  // W
            {
                bp = Board[p_r][p_f-i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(8-p_r, 8-p_f); i++)  // NE
            {
                bp = Board[p_r+i][p_f+i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(8-p_r, p_f+1); i++)  // NW
            {
                bp = Board[p_r+i][p_f-i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r+i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(p_r+1, p_f+1); i++)  // SW
            {
                bp = Board[p_r-i][p_f-i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f-i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f-i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
            for(int i = 1; i < min(p_r+1, 8-p_f); i++)  // SE
            {
                bp = Board[p_r-i][p_f+i];
                
                if(bp < 0){break;}
                
                else if(bp == 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        continue;
                    }
                }
                else
                {
                    if(Legal(0, p, p_r, p_f, p_r-i, p_f+i))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-i;
                        MoveStack[LM][5] = p_f+i;
                        
                        LM += 1;
                        break;
                    }
                }
            }
        }
        if(p == -13)
        {
            if(p_r < 7)
            {
                bp = Board[p_r+1][p_f];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 0)
            {
                bp = Board[p_r-1][p_f];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f;
                        
                        LM += 1;
                    }
                }
            }
            if(p_f < 7)
            {
                bp = Board[p_r][p_f+1];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_f > 0)
            {
                bp = Board[p_r][p_f-1];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r < 7 && p_f < 7)
            {
                bp = Board[p_r+1][p_f+1];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r < 7 && p_f > 0)
            {
                bp = Board[p_r+1][p_f-1];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r+1, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r+1;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 0 && p_f > 0)
            {
                bp = Board[p_r-1][p_f-1];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f-1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f-1;
                        
                        LM += 1;
                    }
                }
            }
            if(p_r > 0 && p_f < 7)
            {
                bp = Board[p_r-1][p_f+1];
                
                if(bp >= 0)
                {
                    if(Legal(0, p, p_r, p_f, p_r-1, p_f+1))
                    {
                        MoveStack[LM][0] = 0;
                        MoveStack[LM][1] = p;
                        MoveStack[LM][2] = p_r;
                        MoveStack[LM][3] = p_f;
                        MoveStack[LM][4] = p_r-1;
                        MoveStack[LM][5] = p_f+1;
                        
                        LM += 1;
                    }
                }
            }
        }
    }
    return LM;
}

int Promotion(int type, int arg0, int arg1, int arg2, int arg3, int arg4, int nQueens)
{
    if(type == 0)
    {
        if(arg0 > 0 && arg0 < 9)
        {
            if(arg3 == 7)
            {
                int newQueen = 17 + nQueens;
                
                Board[7][arg4] = newQueen;
                
                Pieces[0][arg0][0] = 8;
                Pieces[0][arg0][1] = 8;
                
                Pieces[0][newQueen][0] = 7;
                Pieces[0][newQueen][1] = arg4;
                
                return 1;
            }
        }
        if(arg0 < 0 && arg0 > -9)
        {
            if(arg3 == 0)
            {
                int newQueen = 17 + nQueens;
                
                Board[0][arg4] = -newQueen;
                
                Pieces[1][-arg0][0] = 8;
                Pieces[1][-arg0][1] = 8;
                
                Pieces[1][newQueen][0] = 0;
                Pieces[1][newQueen][1] = arg4;
                
                return 1;
            }
        }
    }
    return 0;
}

int Play(int rounds, int new)
{
    int type;
    int arg0;
    int arg1;
    int arg2;
    int arg3;
    int arg4;
    
    int LM;   // Legal moves
    int rm;   // Random move
    
    int Last_Move[6];
    
    if(new){Start();}
    
    for(int round = 1; round <= rounds; round++)
    {
        LM = 0;
        
        for(int p = 1; p < 25; p++)   // Normal moves
        {
            int p_r = Pieces[0][p][0];
            int p_f = Pieces[0][p][1];
            
            if(p_r != 8)
            {
                LM = LegalMoves(p, p_r, p_f, LM);
            }
        }
        
        if(QCastle_W)
        {
            if(Legal(1,0,0,0,0,0))
            {
                MoveStack[LM][0] = 1;
                MoveStack[LM][1] = 0;
                MoveStack[LM][2] = 0;
                MoveStack[LM][3] = 0;
                MoveStack[LM][4] = 0;
                MoveStack[LM][5] = 0;
                
                LM += 1;
            }
        }
        if(KCastle_W)
        {
            if(Legal(2,0,0,0,0,0))
            {
                MoveStack[LM][0] = 2;
                MoveStack[LM][1] = 0;
                MoveStack[LM][2] = 0;
                MoveStack[LM][3] = 0;
                MoveStack[LM][4] = 0;
                MoveStack[LM][5] = 0;
                
                LM += 1;
            }
        }
        
        if(Last_Move[0] == 0 && Last_Move[1] > -9 && Last_Move[2] == 6 && Last_Move[4] == 4)   // Try EnPassant
        {
            for(int p = 1; p < 9; p++)
            {
                int p_r = Pieces[0][p][0];
                int p_f = Pieces[0][p][1];
                
                if(p_r == 4)
                {
                    if(p_f == Last_Move[5] + 1 || p_f == Last_Move[5] - 1)
                    {
                        if(Legal(3, p, p_r, p_f, 5, Last_Move[3]))
                        {
                            MoveStack[LM][0] = 3;
                            MoveStack[LM][1] = p;
                            MoveStack[LM][2] = p_r;
                            MoveStack[LM][3] = p_f;
                            MoveStack[LM][4] = 5;
                            MoveStack[LM][5] = Last_Move[3];
                            
                            LM += 1;
                        }
                    }
                }
            }
        }
        
        if(LM == 0){return 1;}
        
        rm = rand() % LM;   // Random move
        
        type = MoveStack[rm][0];
        arg0 = MoveStack[rm][1];
        arg1 = MoveStack[rm][2];
        arg2 = MoveStack[rm][3];
        arg3 = MoveStack[rm][4];
        arg4 = MoveStack[rm][5];
        
        Move(type, arg0, arg1, arg2, arg3, arg4);
        
        Last_Move[0] = type;
        Last_Move[1] = arg0;
        Last_Move[2] = arg1;
        Last_Move[3] = arg2;
        Last_Move[4] = arg3;
        Last_Move[5] = arg4;
        
        if(Promotion(type, arg0, arg1, arg2, arg3, arg4, PQueens_W))
        {
            PQueens_W += 1;
        }
        
        if(QCastle_W || KCastle_W)
        {
            if(arg0 == 13 || type == 1 || type == 2)
            {
                QCastle_W = 0;
                KCastle_W = 0;
            }
            else if(arg0 == 9){QCastle_W = 0;}
            else if(arg0 == 16){KCastle_W = 0;}
        }
        
        ///////////////////////////////////////////////////////////////////////////////////

        LM = 0;
        
        for(int p = -1; p > -25; p--)
        {
            int p_r = Pieces[1][-p][0];
            int p_f = Pieces[1][-p][1];
            
            if(p_r != 8)
            {
                LM = LegalMoves(p, p_r, p_f, LM);
            }
        }
        
        if(QCastle_B)
        {
            if(Legal(1,1,0,0,0,0))
            {
                MoveStack[LM][0] = 1;
                MoveStack[LM][1] = 1;
                MoveStack[LM][2] = 0;
                MoveStack[LM][3] = 0;
                MoveStack[LM][4] = 0;
                MoveStack[LM][5] = 0;
                
                LM += 1;
            }
        }
        if(KCastle_B)
        {
            if(Legal(2,1,0,0,0,0))
            {
                MoveStack[LM][0] = 2;
                MoveStack[LM][1] = 1;
                MoveStack[LM][2] = 0;
                MoveStack[LM][3] = 0;
                MoveStack[LM][4] = 0;
                MoveStack[LM][5] = 0;
                
                LM += 1;
            }
        }
        
        if(Last_Move[0] == 0 && Last_Move[1] < 9 && Last_Move[2] == 1 && Last_Move[4] == 3)
        {
            for(int p = -1; p > -9; p--)
            {
                int p_r = Pieces[1][-p][0];
                int p_f = Pieces[1][-p][1];
                
                if(p_r == 3)
                {
                    if(p_f == Last_Move[5] + 1 || p_f == Last_Move[5] - 1)
                    {
                        if(Legal(3, p, p_r, p_f, 2, Last_Move[3]))
                        {
                            MoveStack[LM][0] = 3;
                            MoveStack[LM][1] = p;
                            MoveStack[LM][2] = p_r;
                            MoveStack[LM][3] = p_f;
                            MoveStack[LM][4] = 2;
                            MoveStack[LM][5] = Last_Move[3];
                            
                            LM += 1;
                        }
                    }
                }
            }
        }
        
        if(LM == 0){return 0;}

        rm = rand() % LM;
        
        type = MoveStack[rm][0];
        arg0 = MoveStack[rm][1];
        arg1 = MoveStack[rm][2];
        arg2 = MoveStack[rm][3];
        arg3 = MoveStack[rm][4];
        arg4 = MoveStack[rm][5];
        
        Move(type, arg0, arg1, arg2, arg3, arg4);
        
        Last_Move[0] = type;
        Last_Move[1] = arg0;
        Last_Move[2] = arg1;
        Last_Move[3] = arg2;
        Last_Move[4] = arg3;
        Last_Move[5] = arg4;
        
        if(Promotion(type, arg0, arg1, arg2, arg3, arg4, PQueens_B))
        {
            PQueens_B += 1;
        }
        
        if(QCastle_B || KCastle_B)
        {
            if(arg0 == -13 || type == 1 || type == 2)
            {
                QCastle_B = 0;
                KCastle_B = 0;
            }
            else if(arg0 == -9){QCastle_B = 0;}
            else if(arg0 == -16){KCastle_B = 0;}
        }
    }
    
    return -1;
}

int main()
{
    Play(100,1);
}
