package com.example.ryanc.program2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.TextView;
import android.view.View;
import android.widget.SeekBar.OnSeekBarChangeListener;

import java.text.DecimalFormat;

public class MainActivity extends AppCompatActivity {

    private SeekBar tip;
    private TextView t;
    private TextView tv;
    private double billTip;
    private double total;
    private double perPerson;
    private int people;
    private int newtip = 20;
    private EditText etTotal;
    private RadioGroup round;
    private RadioButton noRound;
    private RadioButton tipRound;
    private RadioButton totalRound;
    private Button calculate;
    private EditText etSplit;
    private String output;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //variable intitiation
        tip = (SeekBar) findViewById(R.id.tip_bar);
        t = (TextView) findViewById(R.id.textTip);
        tv = (TextView) findViewById(R.id.output);
        etTotal = (EditText) findViewById(R.id.editTextTotal);
        etSplit = (EditText) findViewById(R.id.etSplit);
        calculate = (Button) findViewById(R.id.btncalc);
        noRound = (RadioButton) findViewById(R.id.noRound);
        tipRound = (RadioButton) findViewById(R.id.tipRound);
        totalRound= (RadioButton) findViewById(R.id.totalRound);
        round=(RadioGroup)findViewById(R.id.radioGroup);
        tip.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

            @Override
            public void onProgressChanged(SeekBar seekBar, int change, boolean fromUser) {
                //updating tip bar
                newtip = change;
                t.setText("Tip Amount "+newtip+"%");
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });
        calculate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //getting all values on calculate button press
                String sTotal = etTotal.getText().toString();
                total = 0;
                try { total = Double.parseDouble(sTotal); }
                catch(NumberFormatException nfe){}
                billTip = setTip(newtip, total);
                String sSplit = etSplit.getText().toString();
                people = 1;
                try { people = Integer.parseInt(sSplit); }
                catch(NumberFormatException nfe){}
                if(noRound.isChecked()) {
                    perPerson = (total + billTip) / (1.00*people);
                    output = "Subtotal: " +r2(total) + "\nTip: " + r2(billTip) + "\nTotal: " +r2(total+billTip) + "\nPer Person: " + r2(perPerson);
                }
                else if(tipRound.isChecked())
                {
                    //Math to adjust the tip per person
                    double billTipPP = billTip/(1.00*people);
                    billTipPP = r0(billTipPP);
                    billTip = billTipPP*people;
                    perPerson = billTipPP + (total/(1.00*people));
                    output = "Subtotal: " +r2(total) + "\nTip: " + r2(billTip) + "\nTotal: " +r2(total+billTip) + "\nTip Per Person: " + r2(billTipPP) + "\nPer Person: " + r2(perPerson);
                }
                else
                {
                    //Math to adjust the total to an even dollar amount
                    double temp = r0((total+billTip)/people);
                    perPerson = temp;
                    billTip = (perPerson*people)-total;
                    output = "Subtotal: " +r2(total) + "\nTip: " + r2(billTip) + "\nTotal: " + r2(temp*1.00*people) + "\nPer Person: " + r2(perPerson);
                }
                tv.setText(output);
            }
        });

    }

    public double setTip(int current, double total)
    {
        //setting tip to a percentage of total
        double temp = ((double) current);
        temp = temp/100;
        return temp*total;
    }

    double r2(double d)
    {
        //round to two decimal places
        DecimalFormat twoDForm = new DecimalFormat("#.##");
        return Double.valueOf(twoDForm.format(d));
    }
    double r0(double d)
    {
        //round to no decimal places
        DecimalFormat twoDForm = new DecimalFormat("#");
        return Double.valueOf(twoDForm.format(d));
    }

}
