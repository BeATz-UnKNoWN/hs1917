package com.andrew.dev.audioplayer;

import com.andrew.dev.audioplayer.fragments.AlbumArtFragment;
import com.andrew.dev.audioplayer.fragments.AllSongsFragment;

import android.content.DialogInterface;
import android.support.v4.app.FragmentManager;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

import java.io.File;

public class MainActivity extends AppCompatActivity implements NavigationDrawerCallbacks, AllSongsFragment.OnSongSelected{
    private NavigationDrawerFragment navigationDrawerFragment;
    private Toolbar toolbar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        toolbar = (Toolbar) findViewById(R.id.toolbar_actionbar);
        setSupportActionBar(toolbar);

        navigationDrawerFragment = (NavigationDrawerFragment) getFragmentManager().findFragmentById(R.id.fragment_drawer);
        navigationDrawerFragment.setup(R.id.fragment_drawer, (DrawerLayout) findViewById(R.id.drawer), toolbar);

        //REST OF THIS FUNCTION IS PART OF XPOSED DEFENSE
        
        // create our dialogue that will prevent the user from using the app
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage(R.string.xposed_found);
        // not cancelable means they can't close the dialogue without pressing a button
        builder.setCancelable(false);
        builder.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                // as soon as the click "ok" finish (exit) the app
                finish();
            }
        });

        // if the XposedBridge.jar has been found
        if (isXposedInstalled()) {
            // show the dialogue that prevents the app from being used
            builder.show();
        }
    }

    @Override
    public void onNavigationDrawerItemSelected(int position) {
        // managing the swapping between fragments (screen)
        FragmentManager fragmentManager = getSupportFragmentManager();
        switch(position) {
            case 0:
                fragmentManager.beginTransaction()
                        .replace(R.id.container, AlbumArtFragment.newInstance())
                        .commit();
                break;
            case 1:
                fragmentManager.beginTransaction()
                        .replace(R.id.container, AllSongsFragment.newInstance())
                        .commit();
                break;
        }
    }

    @Override
    public void onBackPressed() {
        // if the side bar is open, back button will close it
        if (navigationDrawerFragment.isDrawerOpen())
            navigationDrawerFragment.closeDrawer();
        else
            super.onBackPressed();
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        if (!navigationDrawerFragment.isDrawerOpen()) {
            getMenuInflater().inflate(R.menu.main, menu);
            return true;
        }
        return super.onCreateOptionsMenu(menu);
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        // settings not implemented so just provide a toast message
        // to verify it works
        if (id == R.id.action_settings) {
            Toast.makeText(this, "pressed", Toast.LENGTH_SHORT).show();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onSongSelected(View view) {
        //trackSelected(view);
    }

    public void onSectionAttached(int number) {

    }

    // THIS FUNCTION IS PART OF XPOSED DEFENSE
    public boolean isXposedInstalled() {
        boolean installed = false;

        try {
            File xposed = new File("/system/framework/XposedBridge.jar");
            if (xposed.exists()) {
                installed = true;
            }
        } catch (Exception ignored) {
            // we don't care about at exception, so no code here
            // we just will leave installed as false
        }

        return installed;
    }
}