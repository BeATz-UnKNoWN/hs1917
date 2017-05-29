package com.andrew.dev.audioplayer.xposed.overriddenreturns;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

import static de.robv.android.xposed.XposedHelpers.findAndHookMethod;

public class MainModule implements IXposedHookLoadPackage {
    // handleLoadPackage will execute every time an app is executed by zygote
    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam lpparam) throws Throwable {
        // will detect if the app was the audioplayer
        if (lpparam.packageName.equalsIgnoreCase("com.andrew.dev.audioplayer")) {
            // log the audio player's package name
            printLog("Loaded app", lpparam.packageName);
            // hook our getTitle function of our AudioFile class
            findAndHookMethod("com.andrew.dev.audioplayer.AudioFile", lpparam.classLoader, "getTitle",
                    new XC_MethodHook() {
                        // this block of code will after the specified method executes
                        // this will allow use to overwrite the return value
                        // if we changed the return value before the method executes
                        // the method will just set the return value to the actual value
                        @Override
                        protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                            // log the before val
                            printLog("Original title", param.getResult());
                            // setResult() accepts an object so it can be anything
                            // we know that the getTitle function is of type String
                            // so just be sure the result we set is a String
                            param.setResult("Never Gonna Give You Up");
                            // log the after val to verify that the hooking worked
                            printLog("New title", param.getResult());
                        }
                    });

            findAndHookMethod("com.andrew.dev.audioplayer.AudioFile", lpparam.classLoader, "getAlbum",
                    new XC_MethodHook() {
                        // this block of code will after the specified method executes
                        // this will allow use to overwrite the return value
                        // if we changed the return value before the method executes
                        // the method will just set the return value to the actual value
                        @Override
                        protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                            // log the before val
                            printLog("Original album", param.getResult());
                            // setResult() accepts an object so it can be anything
                            // we know that the getTitle function is of type String
                            // so just be sure the result we set is a String
                            param.setResult("Whenever You Need Somebody");
                            // log the after val to verify that the hooking worked
                            printLog("New album", param.getResult());
                        }
                    });

            findAndHookMethod("com.andrew.dev.audioplayer.AudioFile", lpparam.classLoader, "getArtist",
                    new XC_MethodHook() {
                        // this block of code will after the specified method executes
                        // this will allow use to overwrite the return value
                        // if we changed the return value before the method executes
                        // the method will just set the return value to the actual value
                        @Override
                        protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                            // log the before val
                            printLog("Original artist", param.getResult());
                            // setResult() accepts an object so it can be anything
                            // we know that the getTitle function is of type String
                            // so just be sure the result we set is a String
                            param.setResult("Rick Astley");
                            // log the after val to verify that the hooking worked
                            printLog("New artist", param.getResult());
                        }
                    });
        }
    }

    // just a little helper function to print logs of a "text: data" format
    private static void printLog (String text, Object data) {
        XposedBridge.log(text + ": " + data);
    }
}
