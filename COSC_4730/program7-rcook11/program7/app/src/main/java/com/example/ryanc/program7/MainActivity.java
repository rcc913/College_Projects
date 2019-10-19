package com.example.ryanc.program7;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;


public class MainActivity extends AppCompatActivity {

    Thread myNet;
    private Boolean GameOver = Boolean.valueOf(false);
    private String send = "Ryan 0 0 4";
    Button mup;
    Button mdown;
    Button mleft;
    Button mright;
    Button muleft;
    Button muright;
    Button mdleft;
    Button mdright;
    Button stop;
    Button angleup;
    Button angledown;
    int direction = 0;
    int fireangle = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mup = (Button) findViewById(R.id.mUp);
        mdown = (Button) findViewById(R.id.mDown);
        mleft = (Button) findViewById(R.id.mLeft);
        mright = (Button) findViewById(R.id.mRight);
        muleft = (Button) findViewById(R.id.mUpLeft);
        muright = (Button) findViewById(R.id.mUpRight);
        mdleft = (Button) findViewById(R.id.mDownLeft);
        mdright = (Button) findViewById(R.id.mDownRight);
        stop = (Button) findViewById(R.id.mstop);
        angleup = (Button) findViewById(R.id.angleUp);
        angledown = (Button) findViewById(R.id.angleDown);
        doNetwork stuff = new doNetwork();
        myNet = new Thread(stuff);
        myNet.start();
    }


    class doNetwork implements Runnable {
        private String host = "10.216.217.131";
        private int port = 3012;
        private PrintWriter out = null;
        private java.io.BufferedReader in = null;

        public void run() {
            String h = host;
            try {
                InetAddress serverAddr = InetAddress.getByName(h);
                Socket socket = new Socket(serverAddr, port);
                //made connection, setup the read (in) and write (out)
                out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            } catch (Exception e) {}
            play();
        }

        public void write(String s) {
            out.println(s);
        }

        public String read() {
            String str = "";
            try {
                str = in.readLine();
            } catch (java.io.IOException localIOException) {}
            return str;
        }

        public String[] token(String s) {
            java.util.StringTokenizer localStringTokenizer = new java.util.StringTokenizer(s);
            String[] arrayOfString = new String[localStringTokenizer.countTokens()];
            for (int i = 0; i < arrayOfString.length; i++)
                arrayOfString[i] = localStringTokenizer.nextToken();
            return arrayOfString;
        }

        public void play() {

            mup.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    direction =1;
                }
            });
            mdown.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    direction = 5;
                }
            });
            mright.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    direction = 3;
                }
            });
            mleft.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    direction = 7;
                }
            });
            muright.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    direction = 2;
                }
            });
            muleft.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    direction = 8;
                }
            });
            mdleft.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    direction = 6;
                }
            });
            mdright.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    direction = 4;
                }
            });
            angleup.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if(fireangle == 345)
                    {
                        fireangle = 0;
                    }
                    else
                    {
                        fireangle = fireangle+15;
                    }

                }
            });
            angledown.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if(fireangle == 0)
                    {
                        fireangle = 345;
                    }
                    else
                    {
                        fireangle = fireangle-15;
                    }
                }
            });
            stop.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    direction = 0;
                }
            });
            String[] arrayOfString = token(read());
            write(send);
            String str = read();
            while (!GameOver.booleanValue()) {
                str = read();
                arrayOfString = token(str);
                while (arrayOfString[0].equals("Info")) {
                    if ((arrayOfString[1].equals("Dead")) || (arrayOfString[1].equals("GameOver"))) {
                        GameOver = Boolean.valueOf(true);
                        break;
                    }
                    if (arrayOfString[1].equals("Alive")) {}
                    if (arrayOfString[1].equals("PowerUp")) {
                        if (arrayOfString[2].equals("Teleport")) {}
                        else if (arrayOfString[2].equals("ArmorUp")) {}
                        else if (arrayOfString[2].equals("MoveFaster")) {}
                        else if (arrayOfString[2].equals("FireFaster")) {}
                        else if (arrayOfString[2].equals("FireUp")) {}
                        else if (arrayOfString[2].equals("FireMoveFaster")) {}
                    }
                    str = read();
                    arrayOfString = token(str);
                }
                if (!GameOver.booleanValue()) {
                    int moving = Integer.parseInt(arrayOfString[3]);
                    int shooting = Integer.parseInt(arrayOfString[4]);
                    if(shooting == 0)
                    {
                        write("fire " + fireangle);
                    }
                    else if(moving == 0)
                    {
                        switch (direction)
                        {
                            case 0:
                                write("scan");
                                while (true){
                                    str = read();
                                    arrayOfString = token(str);
                                    if(arrayOfString[1].equals("done"))
                                    {
                                        break;
                                    }
                                    //scan ability is here, don't want to do anything with the values
                                }
                                break;
                            case 1:
                                write("move 0 -1");
                                break;
                            case 2:
                                write("move 1 -1");
                                break;
                            case 3:
                                write("move 1 0");
                                break;
                            case 4:
                                write("move 1 1");
                                break;
                            case 5:
                                write("move 0 1");
                                break;
                            case 6:
                                write("move -1 1");
                                break;
                            case 7:
                                write("move -1 0");
                                break;
                            case 8:
                                write("move -1 -1");
                                break;
                            default:
                                direction = 0;
                                break;
                        }
                    }
                    else
                    {
                        write("noop");
                    }
                }
            }
        }

    }


}

