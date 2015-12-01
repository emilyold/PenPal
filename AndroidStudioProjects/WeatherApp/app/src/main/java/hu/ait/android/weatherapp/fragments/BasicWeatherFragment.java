package hu.ait.android.weatherapp.fragments;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.bumptech.glide.Glide;

import de.greenrobot.event.EventBus;
import hu.ait.android.weatherapp.CityDetails;
import hu.ait.android.weatherapp.R;
import hu.ait.android.weatherapp.adapter.CityRecyclerAdapter;
import hu.ait.android.weatherapp.data.WeatherResult;

/**
 * Created by emilyold on 11/30/15.
 */
public class BasicWeatherFragment extends Fragment{

    public static final String TAG = "BASIC_WEATHER_TAG";

    private TextView header;
    private TextView tvTemp;
    private TextView tvDesc;
    private ImageView ivIcon;


    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View rootView = inflater.inflate(
                R.layout.basic_weather, container, false);

        header = (TextView) rootView.findViewById(R.id.tvBasicHeader);
        tvTemp = (TextView) rootView.findViewById(R.id.tvCurrTemp);
        tvDesc = (TextView) rootView.findViewById(R.id.tvCurrCond);
        ivIcon = (ImageView) rootView.findViewById(R.id.ivWeatherIcon);

        return rootView;
    }


    @Override
    public void onResume() {
        super.onResume();
        EventBus.getDefault().register(this);
    }

    @Override
    public void onPause() {
        super.onPause();
        EventBus.getDefault().unregister(this);
    }

    public void onEventMainThread(WeatherResult weatherResult){
        Double temp = weatherResult.getMain().getTemp();
        String description = weatherResult.getWeather().get(0).getDescription();
        String icon = weatherResult.getWeather().get(0).getIcon();
        String cityName = weatherResult.getName();
        Glide.with(this).load("http://openweathermap.org/img/w/" + icon + ".png").into(ivIcon);
        header.setText(cityName);
        tvDesc.setText(description);
        tvTemp.setText(String.valueOf(temp) + "°F");
    }
}
