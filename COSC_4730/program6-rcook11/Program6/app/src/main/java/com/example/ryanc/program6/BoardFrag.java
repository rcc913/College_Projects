package com.example.ryanc.program6;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;


public class BoardFrag extends Fragment {

    Board b;

    public BoardFrag() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        //holds the board everything is done in the board class
        View myView = inflater.inflate(R.layout.fragment_board, container, false);
        b = (Board) myView.findViewById(R.id.board);
        return myView;
    }

}
