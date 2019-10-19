package com.example.ryanc.program5;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteQueryBuilder;


public class TransactionDB {

    private DatabaseHelper DBHelper;
    public SQLiteDatabase db;

    public TransactionDB(Context ctx) {
        this.DBHelper = new DatabaseHelper(ctx);
    }

    public void open() throws SQLException {
        this.db = DBHelper.getWritableDatabase();
    }

    public boolean isOpen() throws SQLException {
        return this.db.isOpen();
    }

    public void close() {
        this.DBHelper.close();
        this.db.close();
    }

    public long cpInsert(String name, ContentValues values) {
        return this.db.insert(name, null, values);
    }

    public Cursor cpQuery(String name, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
        SQLiteQueryBuilder qb = new SQLiteQueryBuilder();
        qb.setTables(name);
        return qb.query(this.db, projection, selection, selectionArgs, null, null, sortOrder);
    }

    public Cursor cpQueryJoin(String TableName, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
        SQLiteQueryBuilder qb = new SQLiteQueryBuilder();
        qb.setTables(TableName
                + " INNER JOIN "
                + DatabaseHelper.TABLE_CAT
                + " ON "
                + DatabaseHelper.KEY_CAT
                + " = "
                + (DatabaseHelper.TABLE_CAT + "." + DatabaseHelper.KEY_ROWID)
        );
        if (projection != null) {
            projection = append(projection, DatabaseHelper.KEY_CATNAME);
        }
        return qb.query(db, projection, selection, selectionArgs, null, null, sortOrder);
    }

    public int cpUpdate(String name, ContentValues values, String selection, String[] selectionArgs) {
        return this.db.update(name, values, selection, selectionArgs);
    }

    public int cpDelete(String name, String selection, String[] selectionArgs) {
        return this.db.delete(name, selection, selectionArgs);
    }


    private String[] append(String[] previous, String next)
    {
        String[] forReturn = new String[previous.length+1];
        for(int i = 0; i < previous.length; i++)
        {
            forReturn[i] = previous[i];
        }
        forReturn[previous.length] = next;
        return  forReturn;
    }
}
