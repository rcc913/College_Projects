package com.example.ryanc.program4;

import android.content.ContentValues;
import android.content.Context;
import android.content.DialogInterface;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.CursorLoader;
import android.support.v4.content.Loader;
import android.support.v4.widget.CursorAdapter;
import android.support.v4.widget.SimpleCursorAdapter;
import android.support.v7.view.ContextThemeWrapper;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class TransactionFragment extends Fragment implements
        LoaderManager.LoaderCallbacks<Cursor> {

    Button btnadd;
    Button btncat;
    public Context myContext;
    public ListView list;
    private SimpleCursorAdapter dataAdapter;
    public static final String PROVIDER="edu.cs4730.prog4db";
    public static Uri CONTENT_URI_cat = Uri.parse("content://"+PROVIDER+"/Category");
    public static Uri CONTENT_URI_trans1 = Uri.parse("content://"+PROVIDER+"/Accounts/transactions/1");
    String[] cv_trans = new String[]{KEY_DATE, KEY_TYPE, KEY_NAME, KEY_AMOUNT, KEY_CAT};
    String[] projection_trans = new String[]{TABLE_CHECKING+"."+KEY_ROWID, KEY_DATE,  KEY_TYPE, KEY_NAME, KEY_AMOUNT, KEY_CAT};
    String[] projection_cat = new String[]{KEY_ROWID, KEY_CATNAME};
    String[] cv_cat = new String[]{KEY_CATNAME};
    public static final String KEY_NAME = "Name";
    public static final String KEY_CATNAME = "CatName";
    public static final String KEY_DATE = "Date";
    public static final String KEY_TYPE = "CheckNum";
    public static final String KEY_AMOUNT = "Amount";
    public static final String KEY_CAT = "Category";
    public static final String KEY_ROWID = "_id";
    public static final String TABLE_CHECKING = "checking";

    public TransactionFragment() {
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View myView = inflater.inflate(R.layout.fragment_transaction, container, false);
        list = (ListView) myView.findViewById(R.id.listView);
        btnadd = (Button) myView.findViewById(R.id.btnadd);
        btncat = (Button) myView.findViewById(R.id.btncat);
        btncat.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final EditText et_catadd;
                final LayoutInflater inflater = LayoutInflater.from(getActivity());
                final View myView = inflater.inflate(R.layout.cat_dialog, null);
                final String catadd = "";
                et_catadd = (EditText) myView.findViewById(R.id.et_catadd);
                if (catadd != null) et_catadd.setText(catadd);
                android.support.v7.app.AlertDialog.Builder builder = new  android.support.v7.app.AlertDialog.Builder(new ContextThemeWrapper(getActivity(), R.style.AppTheme));
                builder.setView(myView).setTitle("Add Category");
                builder.setPositiveButton("Save", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        ContentValues values = createData(cv_cat, new String[]{et_catadd.getText().toString()});
                        getActivity().getContentResolver().insert(CONTENT_URI_cat, values);
                    }});
                builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });
                builder.show();
            }
        });
        btnadd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final EditText et_name, et_amount, et_type, et_category, et_date;
                final LayoutInflater inflater = LayoutInflater.from(getActivity());
                final View myView = inflater.inflate(R.layout.fragment_multi_line_input_dialog, null);
                final Cursor cCursor = getActivity().getContentResolver().query(CONTENT_URI_cat, cv_cat, null, null, null);
                String name ="";
                String amount ="";
                String type ="";
                String category ="";
                String date ="";
                et_name = (EditText) myView.findViewById(R.id.et_name);
                if (name != null) et_name.setText(name);
                et_amount = (EditText) myView.findViewById(R.id.et_amount);
                if (amount != null) et_amount.setText(amount);
                et_type = (EditText) myView.findViewById(R.id.et_type);
                if (type != null) et_type.setText(type);
                et_date = (EditText) myView.findViewById(R.id.et_date);
                if (date != null) et_date.setText(date);
                et_category = (EditText) myView.findViewById(R.id.et_category);
                if (category != null) et_category.setText(category);
                cCursor.moveToFirst();
                String myS="Category Key:\n";
                for(int i = 1; i<=cCursor.getCount(); i++)
                {
                    myS += i+" : "+cCursor.getString(cCursor.getColumnIndexOrThrow(KEY_CATNAME)) + "\n";
                    cCursor.moveToNext();
                }
                TextView t = myView.findViewById(R.id.catshow);
                t.setText(myS);
                android.support.v7.app.AlertDialog.Builder builder = new  android.support.v7.app.AlertDialog.Builder(new ContextThemeWrapper(getActivity(), R.style.AppTheme));
                builder.setView(myView).setTitle("Add Transaction");
                builder.setPositiveButton("Save", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                       String [] returnlist = new String[]{
                                et_name.getText().toString(),
                                et_amount.getText().toString(),
                                et_type.getText().toString(),
                                et_category.getText().toString(),
                                et_date.getText().toString()};
                       int catnum = Integer.parseInt(returnlist[3]);
                       if(catnum > cCursor.getCount())
                       {
                           Toast.makeText(getActivity(), ("Category not valid there are "+cCursor.getCount()+ " categories"), Toast.LENGTH_SHORT).show();
                           dialog.cancel();
                       }
                       else
                       {
                           ContentValues values = createData(cv_trans, new String[]{returnlist[4], returnlist[2], returnlist[0], returnlist[1], returnlist[3]});
                           getActivity().getContentResolver().insert(CONTENT_URI_trans1, values);
                       }
                    }});
                builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                    });
                builder.show();

            }
        });
        transactionList();
        return myView;
    }

    public void transactionList() {
        list.setClickable(true);
        String[] columns = new String[]{
                KEY_TYPE,
                KEY_NAME,
                KEY_AMOUNT,
                KEY_CATNAME,
                KEY_DATE
        };
        int[] to = new int[]{
                R.id.type,
                R.id.name,
                R.id.amount,
                R.id.cat,
                R.id.date
        };
        getLoaderManager().initLoader(0x01, null, this);
        dataAdapter = new SimpleCursorAdapter(
                getActivity().getApplicationContext(), R.layout.transaction_display,
                null,
                columns,
                to,
                CursorAdapter.FLAG_REGISTER_CONTENT_OBSERVER);
        list.setAdapter(dataAdapter);
        list.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View v, final int position, long id) {
                final Cursor cursor = (Cursor) list.getItemAtPosition(position);
                final Cursor cCursor = getActivity().getContentResolver().query(CONTENT_URI_cat, cv_cat, null, null, null);
                final String pos = cursor.getString(cursor.getColumnIndexOrThrow(KEY_ROWID));
                String name = cursor.getString(cursor.getColumnIndex(KEY_NAME));
                String amount = cursor.getString(cursor.getColumnIndex(KEY_AMOUNT));
                String type = cursor.getString(cursor.getColumnIndex(KEY_TYPE));
                String category = cursor.getString(cursor.getColumnIndex(KEY_CAT));
                String date = cursor.getString(cursor.getColumnIndex(KEY_DATE));
                final EditText et_name, et_amount, et_type, et_category, et_date;
                final LayoutInflater inflater = LayoutInflater.from(getActivity());
                final View myView = inflater.inflate(R.layout.fragment_multi_line_input_dialog, null);
                et_name = (EditText) myView.findViewById(R.id.et_name);
                if (name != null) et_name.setText(name);
                et_amount = (EditText) myView.findViewById(R.id.et_amount);
                if (amount != null) et_amount.setText(amount);
                et_type = (EditText) myView.findViewById(R.id.et_type);
                if (type != null) et_type.setText(type);
                et_category = (EditText) myView.findViewById(R.id.et_category);
                if (category != null) et_category.setText(category);
                et_date = (EditText) myView.findViewById(R.id.et_date);
                if (date != null) et_date.setText(date);
                cCursor.moveToFirst();
                String myS="Category Key:\n";
                for(int i = 1; i<=cCursor.getCount(); i++)
                {
                    myS += i+" : "+cCursor.getString(cCursor.getColumnIndexOrThrow(KEY_CATNAME)) + "\n";
                    cCursor.moveToNext();
                }
                TextView t = myView.findViewById(R.id.catshow);
                t.setText(myS);
                android.support.v7.app.AlertDialog.Builder builder = new  android.support.v7.app.AlertDialog.Builder(new ContextThemeWrapper(getActivity(), R.style.AppTheme));
                builder.setView(myView).setTitle("Edit");
                builder.setPositiveButton("Save", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        String [] returnlist = new String[]{
                                et_name.getText().toString(),
                                et_amount.getText().toString(),
                                et_type.getText().toString(),
                                et_category.getText().toString(),
                                et_date.getText().toString()};
                        int catnum = Integer.parseInt(returnlist[3]);
                        if(catnum > cCursor.getCount())
                        {
                            Toast.makeText(getActivity(), ("Category not valid there are "+cCursor.getCount()+ " categories"), Toast.LENGTH_SHORT).show();
                            dialog.cancel();
                        }
                        else
                        {
                            ContentValues values = createData(cv_trans, new String[]{returnlist[4], returnlist[2], returnlist[0], returnlist[1], returnlist[3]});
                            getActivity().getContentResolver().update(Uri.parse(CONTENT_URI_trans1 + "/" + pos), values, null, null);
                        }
                    }});
                builder.setNegativeButton("Delete", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        getActivity().getContentResolver().delete(Uri.parse(CONTENT_URI_trans1+"/"+pos), null, null);
                    }
                });
                builder.show();
            }
        });
    }

    @Override
    public Loader<Cursor> onCreateLoader(int id, Bundle args) {
            projection_trans = new String[]{TABLE_CHECKING + "." + KEY_ROWID, KEY_DATE, KEY_TYPE, KEY_NAME, KEY_AMOUNT, KEY_CAT};
            CursorLoader cursorLoader = new CursorLoader(getActivity(),
                    CONTENT_URI_trans1, projection_trans, null, null, null);
            return cursorLoader;

    }

    @Override
    public void onLoadFinished(Loader<Cursor> loader, Cursor cursor) {
        dataAdapter.swapCursor(cursor);
    }

    @Override
    public void onLoaderReset(Loader<Cursor> loader) {
        dataAdapter.swapCursor(null);
    }

    public ContentValues createData(String[] key, String[] data) {
        ContentValues cv = new ContentValues();
        for (int i =0; i<key.length; i++) {
            cv.put(key[i], data[i]);
        }
        return cv;
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        myContext = context;
    }
}
