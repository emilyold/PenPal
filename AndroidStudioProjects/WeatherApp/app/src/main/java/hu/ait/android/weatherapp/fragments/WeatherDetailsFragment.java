package hu.ait.android.weatherapp.fragments;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.bumptech.glide.Glide;

import org.w3c.dom.Text;

import java.text.DateFormat;
import java.util.Date;

import de.greenrobot.event.EventBus;
import hu.ait.android.weatherapp.R;
import hu.ait.android.weatherapp.data.WeatherResult;

/**
 * Created by emilyold on 11/30/15.
 */
public class WeatherDetailsFragment extends Fragment{

    public static final String TAG = "WEATHER_DETAILS_TAG";

    private TextView tvCityName;
    private TextView tvDate;
    private TextView tvTemp;
    private TextView tvPressure;
    private TextView tvHumidity;
    private TextView tvMinTemp;
    private TextView tvMaxTemp;
    private TextView tvSunrise;
    private TextView tvSunset;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View rootView = inflater.inflate(
                R.layout.details_fragment, container, false);

        tvCityName = (TextView) rootView.findViewById(R.id.tvDetailsCityName);
        tvDate = (TextView) rootView.findViewById(R.id.tvDate);
        tvTemp = (TextView) rootView.findViewById(R.id.tvDetailsTemp);
        tvPressure = (TextView) rootView.findViewById(R.id.tvPressure);
        tvHumidity = (TextView) rootView.findViewById(R.id.tvHumidity);
        tvMinTemp = (TextView) rootView.findViewById(R.id.tvMinTemp);
        tvMaxTemp = (TextView) rootView.findViewById(R.id.tvMaxTemp);
        tvSunrise = (TextView) rootView.findViewById(R.id.tvSunrise);
        tvSunset = (TextView) rootView.findViewById(R.id.tvSunset);


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
        Double minTemp = weatherResult.getMain().getTempMin();
        Double maxTemp = weatherResult.getMain().getTempMax();
        Double pressure = weatherResult.getMain().getPressure();
        Integer humidity = weatherResult.getMain().getHumidity();
        String sunrise = (new Date(new Long(weatherResult.getSys().getSunrise()))).toString().substring(11, 19);
        String sunset = (new Date(new Long(weatherResult.getSys().getSunset()))).toString().substring(11, 19);
        String cityName = weatherResult.getName();
        String date = new Date().toString().substring(0, 10);

        tvCityName.setText(cityName);
        tvDate.setText(date);
        tvTemp.append(String.valueOf(temp) + "°F");
        tvMinTemp.append(String.valueOf(minTemp) + "°F");
        tvMaxTemp.append(String.valueOf(maxTemp) + "°F");
        tvPressure.append(String.valueOf(pressure));
        tvHumidity.append(String.valueOf(humidity) + "%");
        tvSunrise.append(sunrise);
        tvSunset.append(sunset);
    }



}
