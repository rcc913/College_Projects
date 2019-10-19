package com.example.ryanc.program3;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    Button btn0;
    Button btn1;
    Button btn2;
    Button btn3;
    Button btn4;
    Button btn5;
    Button btn6;
    Button btn7;
    Button btn8;
    Button btn9;
    Button btnplus;
    Button btnsub;
    Button btnmul;
    Button btndiv;
    Button btnpow;
    Button btnneg;
    Button btnequal;
    Button btnclr;
    Button btndec;
    String value = "";
    String modifier = "";
    boolean isnumber = false;
    boolean isneg = false;
    double lhs;
    double rhs;
    TextView equation;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        equation = (TextView) findViewById(R.id.equation);
        btn0 = (Button) findViewById(R.id.btn0);
        btn1 = (Button) findViewById(R.id.btn1);
        btn2 = (Button) findViewById(R.id.btn2);
        btn3 = (Button) findViewById(R.id.btn3);
        btn4 = (Button) findViewById(R.id.btn4);
        btn5 = (Button) findViewById(R.id.btn5);
        btn6 = (Button) findViewById(R.id.btn6);
        btn7 = (Button) findViewById(R.id.btn7);
        btn8 = (Button) findViewById(R.id.btn8);
        btn9 = (Button) findViewById(R.id.btn9);
        btnplus = (Button) findViewById(R.id.btnplus);
        btnsub = (Button) findViewById(R.id.btnsub);
        btnmul = (Button) findViewById(R.id.btnmul);
        btndiv = (Button) findViewById(R.id.btndiv);
        btnpow = (Button) findViewById(R.id.btnpow);
        btnneg = (Button) findViewById(R.id.btnneg);
        btnequal = (Button) findViewById(R.id.btnequal);
        btnclr = (Button) findViewById(R.id.btnclr);
        btndec = (Button) findViewById(R.id.btndec);
        //each below will call a function to do its work
        btn0.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                enteringNum("0");
            }
        });
        btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                enteringNum("1");
            }
        });
        btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                enteringNum("2");
            }
        });
        btn3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                enteringNum("3");
            }
        });
        btn4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                enteringNum("4");
            }
        });
        btn5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                enteringNum("5");
            }
        });
        btn6.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                enteringNum("6");
            }
        });
        btn7.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                enteringNum("7");
            }
        });
        btn8.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                enteringNum("8");
            }
        });
        btn9.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                enteringNum("9");
            }
        });
        btnplus.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                modifierpressed("+");
            }
        });
        btnsub.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                modifierpressed("-");
            }
        });
        btnmul.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                modifierpressed("*");
            }
        });
        btndiv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                modifierpressed("/");
            }
        });
        btnpow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                modifierpressed("^");
            }
        });
        btnneg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                isneg = !isneg;
            }
        });
        btnequal.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                modifierpressed("=");
            }
        });
        btnclr.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                lhs = 0;
                rhs = 0;
                modifier="";
                value="";
                isneg=false;
                isnumber=false;
                equation.setText("");
            }
        });
        btndec.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(value.contains("."))
                {
                    String toPrint = "Decimal already pressed";
                    Toast toast = Toast.makeText(getApplicationContext(), toPrint,Toast.LENGTH_SHORT);
                    toast.show();
                }
                else
                {
                    enteringNum(".");
                }
            }
        });

    }

    void enteringNum(String current)
    {
        if(modifier.equals("="))
        {
            //Setting lhs after an equal sign
            String toPrint = "Press CLR to start new Equation or press a modifier to use as in next calculation";
            Toast toast = Toast.makeText(getApplicationContext(), toPrint,Toast.LENGTH_SHORT);
            toast.show();
        }
        else
        {
            //adding number to string for next calculation
            value = value + current;
            equation.setText(value);
            isnumber = true;
        }

    }
    void modifierpressed(String current)
    {
        boolean equal = false;
        if(modifier.equals("="))
        {
            //only modifier to leave a number
            equal = true;
        }
        if(isnumber == false && equal == false)
        {
            //make sure a modifier cant be pressed twice
            String toPrint = "Modifier already pressed";
            Toast toast = Toast.makeText(getApplicationContext(), toPrint,Toast.LENGTH_SHORT);
            toast.show();
        }
        else
        {
            if(modifier.equals(""))
            {
                //For the first modifier hit
                if(isneg){lhs = -1*Double.parseDouble(value);}
                else{lhs = Double.parseDouble(value);}
                isneg = false;
                equation.setText("" + lhs);
                modifier = current;
                isnumber = false;
            }
            else if (modifier.equals("="))
            {
                //setting lhs
                if(isneg){lhs = -1*lhs;}
                else{lhs = lhs;}
                isneg = false;
                isnumber = false;
                equation.setText("" + lhs);
                modifier = current;
            }
            else
            {
                //performing calculations on a new modifier press
                if (modifier.equals("+")) {
                    rhs = Double.parseDouble(value);
                    if(isneg){lhs = lhs + (-1*rhs);}
                    else{lhs = lhs + rhs;}
                    isneg = false;
                } else if (modifier.equals("-")) {
                    rhs = Double.parseDouble(value);
                    if(isneg){lhs = lhs - (-1*rhs);}
                    else{lhs = lhs - rhs;}
                    isneg = false;
                } else if (modifier.equals("*")) {
                    rhs = Double.parseDouble(value);
                    if(isneg){lhs = lhs * (-1*rhs);}
                    else{lhs = lhs * rhs;}
                    isneg = false;
                } else if (modifier.equals("/")) {
                    rhs = Double.parseDouble(value);
                    if(isneg){lhs = lhs / (-1*rhs);}
                    else{lhs = lhs / rhs;}
                    isneg = false;
                } else if (modifier.equals("^")) {
                    rhs = Double.parseDouble(value);
                    if(isneg){lhs = Math.pow(lhs, (-1*rhs));}
                    else{lhs = Math.pow(lhs, rhs);}
                    isneg = false;
                } else {
                    if(isneg){lhs = -1*Double.parseDouble(value);}
                    else{lhs = Double.parseDouble(value);}
                    isneg = false;
                }
                modifier = current;
                String temp = "" + lhs;
                equation.setText(temp);
                isnumber = false;
            }
        }
        value = "";
    }
}
