package com.andrew.dev.audioplayer;

import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;
import android.util.Log;

public class AudioFile {
    private long id;
    private String title;
    private String artist;
    private String album;

    public AudioFile(long trackId, String trackTitle, String trackArtist, String trackAlbum) {
        id = trackId;
        title = trackTitle;
        artist = trackArtist;
        album = trackAlbum;
    }

    public long getId() {
        return id;
    }

    // this is the function that gets hooked
    // so this is where we want the defense implementation
    public String getTitle() {
        // pass in the Shared.main field that was given an instance of MainActivity
        AlertDialog.Builder builder = new AlertDialog.Builder(Shared.main);
        builder.setMessage(R.string.xposed_found);
        // prevent the user from being able to bypass the message
        builder.setCancelable(false);
        builder.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                // call the finish() method from out MainActivity instance
                Shared.main.finish();
            }
        });

        // since getTitle gets called a lot (for each song)
        // and scanning the stack trace is to an extent demanding
        // we only want to do it one, else the app will lag out
        if (!Shared.xposedChecked && isXposedActive()) {
            // we show the dialog to prevent the user's use of the app
            builder.show();
            // we set this value to false so we don't try to check again
            Shared.xposedChecked = true;
        }

        return title;
    }

    public String getArtist() {
        return artist;
    }

    public String getAlbum() {
        return album;
    }

    public boolean isXposedActive() {
        boolean active = false;
        try {
            // throw an exception so we execute the catch block
            // and access the stack trace
            throw new Exception("test");
        } catch (Exception e) {
            int zygoteCallCount = 0;
            // a foreach loop
            for (StackTraceElement traceElement : e.getStackTrace()) {
                if (traceElement.getClassName().equals("com.internal.os.ZygoteInit")) {
                    zygoteCallCount++;
                    // if there are 2 instance of calls to the ZygoteInit
                    // we know the altered (by Xposed) version is present and working
                    if (zygoteCallCount == 2) {
                        // so we log it and return xposed being found as true
                        Log.wtf("Audio Player", "Xposed Framework is active on this device");
                        active = true;
                    }
                }
                if (traceElement.getClassName().equals("de.robv.android.xposed.XposedBridge") &&
                    traceElement.getMethodName().equals("handleHookedMethod")) {
                    // alternatively if the handleHookedMethod in XposedBridge is working
                    // we also know that our app is being hooked
                    Log.wtf("Audio Player", "A method has been hooked by Xposed");
                    active = true;
                }
            }
        }
        return active;
    }
}
