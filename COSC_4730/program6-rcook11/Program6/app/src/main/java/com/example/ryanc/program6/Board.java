package com.example.ryanc.program6;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;



public class Board extends View {
    Paint black, black2;
    public int incr, size = 3;
    int mheight = 0, mwidth = 0;
    int leftside, rightside, boardwidth;
    public int myBoard[][];
    public Context myContext;
    boolean xTurn;

    public Board(Context context) {
        super(context);
        myContext = context;
        setup();
    }

    public Board(Context context, AttributeSet attrs) {
        super(context, attrs);
        myContext = context;
        setup();
    }

    public Board(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        myContext = context;
        setup();
    }

    public void setup() {
        //creating the paints to be used to draw
        black = new Paint();
        black.setColor(Color.BLACK);
        black2 = new Paint();
        black2.setColor(Color.BLACK);
        black2.setStrokeWidth(20);
        xTurn = true;
        if (myBoard != null) {
            myBoard = null;
        }
        //first declaration of the board
        myBoard = new int[size][size];
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                myBoard[i][j] = 0;
            }
        }
        if (mheight > 0) setsizes();
    }

    public void setsizes() {
        //seting the size for the game board on the screen
        incr = mwidth / (size+2);
        leftside = incr;
        rightside = incr * 4;
        boardwidth = incr * size;
    }


    void clearboard() {
        //clearing the board for a new game
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                myBoard[i][j] = 0;
            }
        }
        xTurn = true;
        invalidate();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        float x = incr;
        float y = incr;
        canvas.drawColor(Color.WHITE);
        //placing the x and o on the grid in the correct loaction
        for (int yi = 0; yi < size; yi++) {
            for (int xi = 0; xi < size; xi++) {
                if (myBoard[xi][yi] == 1) {
                    black.setTextSize(200);
                    String player = "X";
                    canvas.drawText(player, x + (incr/4), y + (incr*3/4), black);
                } else if (myBoard[xi][yi] == 2) {
                    black.setTextSize(200);
                    String player = "O";
                    canvas.drawText(player, x + (incr/4), y +(incr*3/4), black);
                }
                x += incr;
            }
            x = incr;
            y += incr;
        }
        //drawing the grid for the game
        canvas.drawLine(incr*1, incr*2, incr*4, incr*2, black2);
        canvas.drawLine(incr*1, incr*3, incr*4, incr*3, black2);
        canvas.drawLine(incr*2, incr*1, incr*2, incr*4, black2);
        canvas.drawLine(incr*3, incr*1, incr*3, incr*4, black2);
        String turn = (xTurn) ? "X's Turn" : "O's Turn";
        black.setTextSize(200);
        canvas.drawText(turn, mwidth/4, mheight, black);
    }

    boolean where(int x, int y, int num) {
        //determining which square was pressed and only update on a press
        int cx = -1, cy = -1;
        //if press on board
        if (y >= leftside && y < rightside &&
                x >= leftside && x < rightside) {
            y -= incr;
            x -= incr;
            cx = x / incr;
            cy = y / incr;
            if (cx < size && cy < size) {
                if (myBoard[cx][cy] == 0) {
                    myBoard[cx][cy] = num;
                    xTurn = !xTurn;
                }
            }
            //game over checks
            int win = checkForWin();
            int tie = checkForTie();
            if(win == 1 || win == 2 || tie == 1)
            {
                //if the game is over display the correct output in the dialog box
                Dialog dialog = null;
                AlertDialog.Builder builder;
                builder = new AlertDialog.Builder(myContext);
                if(win == 1) // x wins
                {
                    builder.setMessage("'X' wins. Play again?")
                            .setCancelable(false)
                            .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    clearboard();
                                }
                            })
                            .setNegativeButton("No", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    System.exit(1);
                                }
                    });
                }
                else if (win == 2) // o wins
                {
                    builder.setMessage("'O' wins. Play again?")
                            .setCancelable(false)
                            .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    clearboard();
                                }
                            })
                            .setNegativeButton("No", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                System.exit(1);
                            }
                    });
                }
                else // tie
                {
                    builder.setMessage("Tie. Play again?")
                            .setCancelable(false)
                            .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    clearboard();
                                }
                            })
                            .setNegativeButton("No", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    System.exit(1);
                                }
                            });
                }
                dialog = builder.create();
                dialog.show();
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    int checkForWin()
    {
        //checking all 8 ways to win the game
        if(myBoard[0][0] == myBoard[1][0] && myBoard[2][0] == myBoard[0][0] && (myBoard[0][0] == 1 || myBoard[0][0] == 2))
        {
            return myBoard[0][0];
        }
        else if(myBoard[0][1] == myBoard[1][1] && myBoard[2][1] == myBoard[0][1] && (myBoard[0][1] == 1|| myBoard[0][1] == 2))
        {
            return myBoard[0][1];
        }
        else if(myBoard[0][2] == myBoard[1][2] && myBoard[2][2] == myBoard[0][2] && (myBoard[0][2] == 1 || myBoard[0][2] == 2))
        {
            return myBoard[0][2];
        }
        else if(myBoard[0][0] == myBoard[0][1] && myBoard[0][2] == myBoard[0][0] && (myBoard[0][0] == 1 || myBoard[0][0] == 2))
        {
            return myBoard[0][0];
        }
        else if(myBoard[1][0] == myBoard[1][1] && myBoard[1][2] == myBoard[1][0] && (myBoard[1][0] == 1 || myBoard[1][0] == 2))
        {
            return myBoard[1][0];
        }
        else if(myBoard[2][0] == myBoard[2][1] && myBoard[2][2] == myBoard[2][0] && (myBoard[2][0] == 1 || myBoard[2][0] == 2))
        {
            return myBoard[2][0];
        }
        else if(myBoard[0][0] == myBoard[1][1] && myBoard[2][2] == myBoard[0][0] && (myBoard[0][0] == 1 || myBoard[0][0] == 2))
        {
            return myBoard[0][0];
        }
        else if(myBoard[0][2] == myBoard[1][1] && myBoard[2][0] == myBoard[0][2] && (myBoard[0][2] == 1 || myBoard[0][2] == 2))
        {
            return myBoard[0][2];
        }
        else
        {
            return 0;
        }
    }

    int checkForTie()
    {
        //seeing if any of the spaces aren't filled yet
        if(myBoard[0][0] == 0 || myBoard[0][1] == 0 || myBoard[0][2] == 0 ||
                myBoard[1][0] == 0 || myBoard[1][1] == 0 || myBoard[1][2] == 0 ||
                myBoard[2][0] == 0 || myBoard[2][1] == 0 || myBoard[2][2] == 0)
        {
            return 0;
        }
        return 1;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getAction();
        int x = (int) event.getX();
        int y = (int) event.getY();
        //there was a click then lets see who clicked and call where the click happened
        switch (action) {
            case MotionEvent.ACTION_DOWN:
                if(xTurn)
                {
                    where(x, y, 1);
                    invalidate();
                }
                else
                {
                    where(x, y, 2);
                    invalidate();
                }
                break;
        }
        return true;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        //getting screen dimensions
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        mwidth = getMeasuredWidth();
        mheight = getMeasuredHeight();
        if (mheight > 0 && mwidth > mheight) {
            mwidth = mheight;
        } else if (mheight == 0) {
            mheight = mwidth;
        }
        setsizes();
        setMeasuredDimension(mwidth, mheight);
    }
}
