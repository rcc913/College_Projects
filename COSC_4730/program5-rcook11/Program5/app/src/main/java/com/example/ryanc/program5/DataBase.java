package com.example.ryanc.program5;

import android.content.ContentProvider;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.SQLException;
import android.net.Uri;


public class DataBase extends ContentProvider {

    public static final String PROVIDER_NAME = "edu.cs4730.prog4db";

    TransactionDB db;

    private static final int CATEGORY = 1;
    private static final int CATEGORY_ID = 2;
    private static final int TRANSACTION = 3;
    private static final int TRANSACTION_ID = 4;
    private static final UriMatcher uriMatcher = new UriMatcher(0);

    static {
        uriMatcher.addURI(PROVIDER_NAME, "Category", CATEGORY);
        uriMatcher.addURI(PROVIDER_NAME, "Category/#", CATEGORY_ID);
        uriMatcher.addURI(PROVIDER_NAME, "Accounts/transactions/1", TRANSACTION);
        uriMatcher.addURI(PROVIDER_NAME, "Accounts/transactions/1/#", TRANSACTION_ID);
    }

    public String getType(Uri uri) {
        switch (uriMatcher.match(uri)) {
            case TRANSACTION:
                return "vnd.android.cursor.dir/vnd.cs4730.stuff";
            case CATEGORY:
                return "vnd.android.cursor.dir/vnd.cs4730.stuff";
            case TRANSACTION_ID:
                return "vnd.android.cursor.item/vnd.cs4730.stuff";
            case CATEGORY_ID:
                return "vnd.android.cursor.item/vnd.cs4730.stuff";
            default:
                throw new IllegalArgumentException("Unsupported URI: " + uri);
        }
    }

    public boolean onCreate() {
        this.db = new TransactionDB(getContext());
        this.db.open();
        return true;
    }

    public int delete(Uri uri, String selection, String[] selectionArgs) {
        int count;
        switch (uriMatcher.match(uri)) {
            case TRANSACTION:
                count = this.db.cpDelete(DatabaseHelper.TABLE_CHECKING, selection, selectionArgs);
                break;
            case TRANSACTION_ID:
                selection = fixit(selection, "_id = " + uri.getLastPathSegment());
                count = this.db.cpDelete(DatabaseHelper.TABLE_CHECKING, selection, selectionArgs);
                break;
            case CATEGORY:
                count = this.db.cpDelete(DatabaseHelper.TABLE_CAT, selection, selectionArgs);
                break;
            case CATEGORY_ID:
                selection = fixit(selection, "_id = " + uri.getLastPathSegment());
                count = this.db.cpDelete(DatabaseHelper.TABLE_CAT, selection, selectionArgs);
                break;
            default:
                throw new IllegalArgumentException("Unknown URI " + uri);
        }
        getContext().getContentResolver().notifyChange(uri, null);
        return count;
    }


    public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
        Cursor c;
        switch (uriMatcher.match(uri)) {
            case TRANSACTION:
                c = this.db.cpQueryJoin(DatabaseHelper.TABLE_CHECKING, projection, selection, selectionArgs, sortOrder);
                break;
            case TRANSACTION_ID:
                selection = fixit(selection, DatabaseHelper.TABLE_CHECKING + "._id = " + uri.getLastPathSegment());
                c = this.db.cpQueryJoin(DatabaseHelper.TABLE_CHECKING, projection, selection, selectionArgs, sortOrder);
                break;
            case CATEGORY:
                c = this.db.cpQuery(DatabaseHelper.TABLE_CAT, projection, selection, selectionArgs, sortOrder);
                break;
            case CATEGORY_ID:
                c = this.db.cpQuery(DatabaseHelper.TABLE_CAT, projection, fixit(selection, "_id = " + uri.getLastPathSegment()), selectionArgs, sortOrder);
                break;
            default:
                throw new IllegalArgumentException("Unknown URI " + uri);
        }
        c.setNotificationUri(getContext().getContentResolver(), uri);
        return c;
    }

    public Uri insert(Uri uri, ContentValues values) {
        long row;
        if (values == null) {
            throw new IllegalArgumentException("Illegal Data " + uri);
        }
        switch (uriMatcher.match(uri)) {
            case TRANSACTION:
                row = this.db.cpInsert(DatabaseHelper.TABLE_CHECKING, values);
                break;
            case CATEGORY:
                row = this.db.cpInsert(DatabaseHelper.TABLE_CAT, values);
                break;
            default:
                throw new IllegalArgumentException("Unknown URI " + uri);
        }
        if (row > 0) {
            Uri noteUri = ContentUris.withAppendedId(uri, row);
            getContext().getContentResolver().notifyChange(noteUri, null);
            return noteUri;
        }
        else
        {
            throw new SQLException("Failed to insert row into " + uri);
        }

    }

    public int update(Uri uri, ContentValues values, String selection, String[] selectionArgs) {
        int count;
        switch (uriMatcher.match(uri)) {
            case TRANSACTION:
                count = this.db.cpUpdate(DatabaseHelper.TABLE_CHECKING, values, selection, selectionArgs);
                break;
            case TRANSACTION_ID:
                selection = fixit(selection, "_id = " + uri.getLastPathSegment());
                count = this.db.cpUpdate(DatabaseHelper.TABLE_CHECKING, values, selection, selectionArgs);
                break;
            case CATEGORY:
                count = this.db.cpUpdate(DatabaseHelper.TABLE_CAT, values, selection, selectionArgs);
                break;
            case CATEGORY_ID:
                selection = fixit(selection, "_id = " + uri.getLastPathSegment());
                count = this.db.cpUpdate(DatabaseHelper.TABLE_CAT, values, selection, selectionArgs);
                break;
            default:
                throw new IllegalArgumentException("Unknown URI " + uri);
        }
        getContext().getContentResolver().notifyChange(uri, null);
        return count;
    }

    private String fixit(String tofix, String item) {
        if (tofix == null) {
            tofix = item;
        } else if (tofix.compareTo("") ==0) {
            tofix = item;
        }else {
            tofix += " AND " + item;
        }
        return tofix;
    }

}
