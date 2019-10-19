package com.example.ryanc.program5;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class DatabaseHelper extends SQLiteOpenHelper{

    private static final String DATABASE_NAME = "myCheckBook.db";
    private static final int DATABASE_VERSION = 1;
    public static final String KEY_AMOUNT = "Amount";
    public static final String KEY_CAT = "Category";
    public static final String KEY_CATNAME = "CatName";
    public static final String KEY_DATE = "Date";
    public static final String KEY_NAME = "Name";
    public static final String KEY_ROWID = "_id";
    public static final String KEY_TYPE = "CheckNum";
    public static final String TABLE_CAT = "Category";
    public static final String TABLE_CHECKING = "checking";

    private static final String CREATE_TABLE_CAT =
            "CREATE TABLE " + TABLE_CAT + " (" +
                    KEY_ROWID + " integer PRIMARY KEY autoincrement," +  //this line is required for the cursorAdapter.
                    KEY_CATNAME + " TEXT );";


    // table creation sql statement for checking
    private static final String CREATE_TABLE_CHECKING =
            "CREATE TABLE " + TABLE_CHECKING + " (" +
                    KEY_ROWID + " integer PRIMARY KEY autoincrement," +  //this line is required for the cursorAdapter.
                    KEY_DATE + " TEXT, " +
                    KEY_TYPE + " TEXT, " +
                    KEY_NAME + " TEXT, " +
                    KEY_AMOUNT + " REAL, " +
                    KEY_CAT + " INTEGER, " +
                    "FOREIGN KEY(" + KEY_CAT + ") REFERENCES " + TABLE_CAT + "(" + KEY_CAT + ")" +" );";


    public DatabaseHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    public void onCreate(SQLiteDatabase db) {
        db.execSQL(CREATE_TABLE_CAT);
        db.execSQL(CREATE_TABLE_CHECKING);
    }


    public void onUpgrade(SQLiteDatabase d, int oldVersion, int newVersion) {
        d.execSQL("DROP TABLE IF EXISTS category");
        Cursor mCursor = d.rawQuery("select * from accounts", null);
        if (mCursor != null) {
            mCursor.moveToFirst();
            mCursor.moveToFirst();
            while (!mCursor.isAfterLast()) {
                d.execSQL("DROP TABLE IF EXISTS " + mCursor.getString(mCursor.getColumnIndex(KEY_NAME)));
                mCursor.moveToNext();
            }
        }
        d.execSQL("DROP TABLE IF EXISTS accounts");
        onCreate(d);
    }
}


