package hu.ait.android.penpal;

import android.app.Application;

import com.parse.Parse;
import com.parse.ParseInstallation;
import com.parse.ParsePush;

/**
 * Created by emilyold on 11/20/15.
 */
public class PenPalApplication extends Application {

    public static final String APP_ID =
            "HI1LidGgTSRFdVkoOiaIdGuHWRBBvylY4acb8hgE";
    public static final String CLIENT_ID =
            "wpQv3He0WAHh3uMRmHXZTwelG7SREjRYuRMyFchL";

    @Override
    public void onCreate() {
        super.onCreate();

        Parse.initialize(this, APP_ID, CLIENT_ID);

        // new phone has started to use the application
        ParseInstallation.getCurrentInstallation().saveInBackground();

        ParsePush.subscribeInBackground("AITChat");


    }

}
