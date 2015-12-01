package hu.ait.android.weatherapp;

import android.support.v4.app.Fragment;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import java.util.concurrent.TimeUnit;

import de.greenrobot.event.EventBus;
import hu.ait.android.weatherapp.adapter.CityDetailsAdapter;
import hu.ait.android.weatherapp.adapter.CityRecyclerAdapter;
import hu.ait.android.weatherapp.data.WeatherResult;
import hu.ait.android.weatherapp.fragments.BasicWeatherFragment;
import hu.ait.android.weatherapp.network.HttpAsyncTask;

public class CityDetails extends AppCompatActivity {

    public static final String KEY_TEMP = "KEY_TEMP";
    public static final String KEY_DESCRIPTION = "KEY_DESCRIPTION";
    private ViewPager viewPager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_city_details);

        String cityName = getIntent().getExtras().getString(CityRecyclerAdapter.KEY_CITY_NAME);

        HttpAsyncTask task = new HttpAsyncTask(getApplicationContext());
        task.execute(
                "http://api.openweathermap.org/data/2.5/weather?q=" + cityName + "&units=imperial&appid=4a463b1b8a728bdd248edd86a39a0c39"
        );

        viewPager = (ViewPager) findViewById(R.id.pager);
        viewPager.setAdapter(new CityDetailsAdapter(getSupportFragmentManager()));
    }

}
