package com.example.ryanc.program4;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {


    TransactionFragment tfrag;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (savedInstanceState == null) {
            tfrag = new TransactionFragment();
            getSupportFragmentManager().beginTransaction()
                    .add(R.id.myframe, tfrag).commit();
        }
    }

}
