package com.example.ryanc.program1;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;


public class MainActivity extends AppCompatActivity implements NameFragment.ClickHappened {

    NameFragment myFormFragment;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (savedInstanceState == null) {
            myFormFragment = new NameFragment();
            getSupportFragmentManager().beginTransaction()
                    .add(R.id.myframe, myFormFragment).commit();
            Log.i("MainActivity","Frame Created in Main Activity");
        }
    }

    @Override
    public void printName(String hello)
    {
        //making a toast to say hello
        CharSequence toPrint = "Hello " + hello;
        Toast toast = Toast.makeText(getApplicationContext(), toPrint,Toast.LENGTH_LONG);
        toast.show();
    }
}
