package com.example.ryanc.program1;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;



public class NameFragment extends Fragment implements Button.OnClickListener {

    Button mybutton;
    EditText myedit;
    String name;
    Context myactivity;

    public NameFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View myView = inflater.inflate(R.layout.fragment_name, container, false);
        //creating a button
        mybutton = (Button) myView.findViewById(R.id.Button01);
        mybutton.setOnClickListener(this);
        Log.i("NameFragment","OnCreateView has been reached");
        return myView;
    }
    @Override
    public void onClick(View v) {
        //creating an edit text
        myedit = (EditText) getView().findViewById(R.id.Edit01);
        name = myedit.getText().toString();
        try {
            //calling the interface
            ((ClickHappened) myactivity).printName(name);
        }catch (ClassCastException cce){}
    }

    @Override
    public void onAttach(Context activity)
    {
        super.onAttach(activity);
        this.myactivity = activity;
    }

    public interface ClickHappened {
        //calling function in main activity to update name
        public void printName(String hello);
    }
}
